#ifndef REMOTE_HPP_INCLUDED
#define REMOTE_HPP_INCLUDED

#include <vector>
#include <stdlib.h>
#include <sdk/addr64.hpp>
#include <cbe_mpi/sdk/addr64.hpp>
#include <containers/local.hpp>
#include <iterators/remote_block_iterator.hpp>

template<class T>
struct remote
{
  struct vector;
  typedef typename remote<T>::vector VectorType;
  typedef typename local<T>::vector VectorLocalType;

  struct vector
  {
   private:

      ext::addr64 addr;
      std::size_t size_;

   public:

    vector() : addr(0), size_(0) {}

    vector(const VectorLocalType & vec)
    {
      addr = &vec[0];
      size_ = vec.size();
    }

    inline VectorType & operator= (const VectorLocalType & vec)
    {
      addr = &vec[0];
      size_ = vec.size();
      return *this;
    }

    std::size_t size() { return size_; }

    remote_block_iterator<T> begin() const
    {
      cbe_mpi::addr64 a;
      a.ull = addr.ull;
      return remote_block_iterator<T>(a);
    }

    remote_block_iterator<T> end() const
    {
      cbe_mpi::addr64 a;
      a.ull = addr.ull+size_*sizeof(T);
      return remote_block_iterator<T>(a);
    }

  };
};


#endif // REMOTE_HPP_INCLUDED
