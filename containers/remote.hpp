#ifndef REMOTE_HPP_INCLUDED
#define REMOTE_HPP_INCLUDED

#include <vector>
#include <stdlib.h>
#include <sdk/addr64.hpp>
#include <cbe_mpi/sdk/addr64.hpp>
#include <containers/local.hpp>
#include <iterators/remote_block_base_iterator.hpp>

#ifdef __SPE__
  #include <cbe_mpi/core/bootstrap/init.spe.hpp>
#endif

struct remote
{
  template<class T>
  struct vector
  {
   public:

    typedef typename remote::vector<T> VectorType;
    typedef typename local::vector<T> VectorLocalType;

   private:

      ext::addr64 addr;
      std::size_t size_;

   public:

    vector() : addr(0), size_(0) {}

    vector(const VectorLocalType & vec)
    {
#ifdef CBE_MPI_CELL_SPE_SUPPORT
      // to get the correct address we need to add the SPE's local store address
      addr.ull = (uint64_t)&vec[0] + ControlBlock->ls_ad[SPE_Rank()].ull;
#else
      addr = &vec[0];
#endif
      size_ = vec.size();
    }

    inline VectorType & operator= (const VectorLocalType & vec)
    {
#ifdef CBE_MPI_CELL_SPE_SUPPORT
      // to get the correct address we need to add the SPE's local store address
      addr.ull = (uint64_t)&vec[0] + ControlBlock->ls_ad[SPE_Rank()].ull;
#else
      addr = &vec[0];
#endif
      size_ = vec.size();
      return *this;
    }

    std::size_t size() { return size_; }

    remote_block_base_iterator<T> begin() const
    {
      cbe_mpi::addr64 a;
      a.ull = addr.ull;
      return remote_block_base_iterator<T>(a);
    }

    remote_block_base_iterator<T> end() const
    {
      cbe_mpi::addr64 a;
      a.ull = addr.ull+size_*sizeof(T);
      return remote_block_base_iterator<T>(a);
    }

  };
};


#endif // REMOTE_HPP_INCLUDED
