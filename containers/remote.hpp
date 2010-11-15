#ifndef REMOTE_HPP_INCLUDED
#define REMOTE_HPP_INCLUDED

#include <vector>
#include <stdlib.h>
#include <sdk/addr64.hpp>
#include <containers/local.hpp>

template<class T>
struct remote
{
  struct vector;
  typedef typename remote<T>::vector VectorType;
  typedef typename local<T>::vector VectorLocalType;

  struct vector
  {
    ext::addr64 addr;
    std::size_t size;

    vector() : addr(0), size(0) {}

    vector(const VectorLocalType & vec)
    {
      addr = &vec[0];
      size = vec.size();
    }

    inline VectorType & operator= (const VectorLocalType & vec)
    {
      addr = &vec[0];
      size = vec.size();
      return *this;
    }
  };
};


#endif // REMOTE_HPP_INCLUDED
