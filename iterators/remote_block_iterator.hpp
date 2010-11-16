#ifndef REMOTE_BLOCK_ITERATOR_HPP_INCLUDED
#define REMOTE_BLOCK_ITERATOR_HPP_INCLUDED

#include <cbe_mpi/core/memalign/aligned_ptr.hpp>
#include <cbe_mpi/core/memalign/aligned_malloc.hpp>
#include <cbe_mpi/sdk/addr64.hpp>
/**
 * remote block iterator
 *
 * this class is a dummy iterator that is used for assignment and comparison
 * of remote iterators
 *
 */
template<typename T>
class remote_block_iterator
{

private: // ____________________________________________________________________

  cbe_mpi::addr64 base_address;          //!< base address of the data we access

public: // _____________________________________________________________________

  remote_block_iterator() : base_address(0) {}
  remote_block_iterator(cbe_mpi::addr64 base_address_) :
    base_address(base_address_) {}
  ~remote_block_iterator() {};
  cbe_mpi::addr64 address() const { return base_address; }

  // equal/unequal operator with all the oter block iterators

};


#endif // REMOTE_BLOCK_ITERATOR_HPP_INCLUDED
