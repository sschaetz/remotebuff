#ifndef REMOTE_BLOCK_OUTPUT_ITERATOR_HPP_INCLUDED
#define REMOTE_BLOCK_OUTPUT_ITERATOR_HPP_INCLUDED

#include <cbe_mpi/core/memalign/aligned_ptr.hpp>
#include <cbe_mpi/core/memalign/aligned_malloc.hpp>

/**
 * remote block output iterator
 *
 * this class can iterate over a remote block in a multi-buffering manner
 *
 */

template<typename T, uint8_t depth>
class remote_block_output_iterator
{

private: // ____________________________________________________________________

  int size;                                      //!< number of Ts in one buffer
  uint8_t current;                        //!<  which buffer is currently in use

  int tags[depth];                        //!< tags we use for the DMA transfers
  aligned_ptr<T, CBE_MPI_DATA_ALIGNMENT> buffers[depth];            //!< buffers

  int n;                                               //!< number of iterations
  addr64 base_address;              //!< base address of the data we access
                                       //! function to calculate the next access
  boost::function<int32_t (uint32_t n)> addr_offset_calc;
  bool dirty;                   //!< indicate if the current buffer was accessed

public: // _____________________________________________________________________

  /**
   * ctor
   */
  remote_block_output_iterator(int _size,
    boost::function<int32_t (uint32_t n)> _addr_offset_calc, int * _tags) :
    size(_size*sizeof(T)), current(0), n(0),
    addr_offset_calc(_addr_offset_calc), dirty(false)
  {
    for(uint8_t i=0; i<depth; i++)               // allocate and initialize data
    {
      buffers[i] = (aligned_ptr<T, CBE_MPI_DATA_ALIGNMENT>)
        aligned_malloc<CBE_MPI_DATA_ALIGNMENT>(size);
      tags[i] = _tags[i];
    }
  }

  /**
   * assignment operator (to assign to remote vector for example)
   */
  inline remote_block_output_iterator & operator= (
    const ext::addr64 & base_address_)
  {
    base_address.ull = base_address_.ull;
    init();
    return *this;
  }

  /**
   * assignment operator (to assign to remote vector for example)
   */
  inline remote_block_output_iterator & operator= (
    const remote_block_iterator<T> & it)
  {
    base_address.ull = it.address().ull;
    init();
    return *this;
  }

  /**
   * indirection operator to get a pointer to the current finished data
   */
  inline T* operator *()
  {
    dma_synchronize_c(tags[current]);
    dirty = true;
    return buffers[current].get();
  }

  /**
   * increment operator to advance the iterator to the next block
   */
  inline void operator ++(int)
  {
    dirty = false;
                             // we are finished with current buffer, download it
    int32_t addr_offset = addr_offset_calc(n);
    if(addr_offset < 0)             // we don't store data if offset is negative
    {
      return;
    }
    spe_ppe_put_async_c(base_address+(addr_offset*sizeof(T)),
      buffers[current].get(), size, tags[current]);
    n++;
    current = (current + 1) % depth;
    return;
  }

  ~remote_block_output_iterator()
  {
    uinit();
  }

  /**
   * less than operator
   */
  bool operator <(const remote_block_iterator<T> b) const
  {
           // offset that was used for data that is current after next increment
    int32_t next_offset = addr_offset_calc(n+1)*sizeof(T);
    addr64 baddr = b.address();
    if(next_offset >= 0 && baddr.ull >
       base_address.ull+next_offset+size)
    {
      return true;
    }
    return false;
  }

  /**
   * greater than operator
   */
  bool operator >(const remote_block_iterator<T> b) const
  {
           // offset that was used for data that is current after next increment
    int32_t next_offset = addr_offset_calc(n+1)*sizeof(T);
    if(next_offset < 0 || b.address().ull <=
       base_address.ull+next_offset+size)
    {
      return true;
    }
    return false;
  }

 private:


  void init()
  {
    current = 0;
    n = 0;
    dirty = false;
  }

  void uinit()
  {
    if(dirty)        // download the last block because it probably was modified
    {
      int32_t addr_offset = addr_offset_calc(n);
      if(addr_offset < 0)           // we don't store data if offset is negative
      {
        return;
      }
      spe_ppe_put_async_c(base_address+(addr_offset*sizeof(T)),
        buffers[current].get(), size, tags[current]);
    }
    for(uint8_t i=0; i<depth; i++)                               // free buffers
    {
      dma_synchronize_c(tags[i]);
      aligned_free(buffers[i]);
    }
  }


};


#endif // REMOTE_BLOCK_OUTPUT_ITERATOR_HPP_INCLUDED
