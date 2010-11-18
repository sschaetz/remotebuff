#ifndef REMOTE_BLOCK_INPUT_ITERATOR_HPP_INCLUDED
#define REMOTE_BLOCK_INPUT_ITERATOR_HPP_INCLUDED

#include <cbe_mpi/core/memalign/aligned_ptr.hpp>
#include <cbe_mpi/core/memalign/aligned_malloc.hpp>

/**
 * remote block input iterator
 *
 * this class can iterate over a remote block in a multi-buffering manner
 *
 */

template<typename T>
class remote_block_input_iterator
{

private: // ____________________________________________________________________

  uint8_t depth;                                 //!< the number of buffers used
  int size;                                   //!< number of bytes in one buffer
  uint8_t current;                        //!<  which buffer is currently in use

  int * tags;                             //!< tags we use for the DMA transfers
  aligned_ptr<T, CBE_MPI_DATA_ALIGNMENT> * buffers;                 //!< buffers

  int n;                                               //!< number of iterations
  addr64 base_address;              //!< base address of the data we access
                                       //! function to calculate the next access
  boost::function<int32_t (uint32_t n)> addr_offset_calc;

public: // _____________________________________________________________________

  /**
   * ctor
   */
  remote_block_input_iterator(uint8_t _depth, int _size,
    boost::function<int32_t (uint32_t n)> _addr_offset_calc, int * _tags = 0) :
    depth(_depth), size(_size*sizeof(T)), current(0), n(0),
    addr_offset_calc(_addr_offset_calc)
  {
    tags = (int*) malloc(sizeof(int) * depth);
    buffers = (aligned_ptr<T, CBE_MPI_DATA_ALIGNMENT> *) malloc(
      sizeof(aligned_ptr<T, CBE_MPI_DATA_ALIGNMENT>)*depth);
    for(uint8_t i=0; i<depth; i++)               // allocate and initialize data
    {
      buffers[i] = (aligned_ptr<T, CBE_MPI_DATA_ALIGNMENT>)
        aligned_malloc<CBE_MPI_DATA_ALIGNMENT>(size);
      tags[i] = (_tags) ? _tags[i] : i+1;
    }
  }

  /**
   * assignment operator (to assign to remote vector for example)
   */
  inline remote_block_input_iterator & operator= (
    const ext::addr64 & base_address_)
  {
    base_address.ull = base_address_.ull;
    init();
    return *this;
  }

  /**
   * assignment operator (to assign to remote vector for example)
   */
  inline remote_block_input_iterator & operator= (
    const remote_block_base_iterator<T> & it)
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
    return buffers[current].get();
  }

  /**
   * increment operator to advance the iterator to the next block
   */
  inline void operator ++(int)
  {
                   // we are finished with current buffer, start laod of new one
    int32_t addr_offset = addr_offset_calc(n);
    if(addr_offset < 0)   // we don't fetch data if address offset is negative
    {
      return;
    }
    spe_ppe_get_async_c(buffers[n%depth].get(), base_address+
                        (addr_offset*sizeof(T)), size, tags[n%depth]);
    n++;
    current = (current + 1) % depth;
    return;
  }

  ~remote_block_input_iterator()
  {
    uinit();
  }

  /**
   * less than operator
   */
  bool operator <(const remote_block_base_iterator<T> b) const
  {
           // offset that was used for data that is current after next increment
    int32_t next_offset = addr_offset_calc(n-depth)*sizeof(T);
    addr64 baddr = b.address();
    if(next_offset >= 0 && baddr.ull >=
       base_address.ull+next_offset+size)
    {
      return true;
    }
    return false;
  }

  /**
   * greater than operator
   */
  bool operator >(const remote_block_base_iterator<T> b) const
  {
           // offset that was used for data that is current after next increment
    int32_t next_offset = addr_offset_calc(n-depth)*sizeof(T);
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
    for(uint8_t i=0; i<depth; i++)                            // start transfers
    {
      int32_t addr_offset = addr_offset_calc(n);
      if(addr_offset < 0)   // we don't fetch data if address offset is negative
      {
        return;
      }
      spe_ppe_get_async_c(buffers[i].get(),
        base_address+(addr_offset*sizeof(T)), size, tags[i]);
      n++;
    }
  }

  void uinit()
  {
    for(uint8_t i=0; i<depth; i++)                               // free buffers
    {
      aligned_free(buffers[i]);
    }
    free(tags);
    free(buffers);
  }


};


#endif // REMOTE_BLOCK_INPUT_ITERATOR_HPP_INCLUDED
