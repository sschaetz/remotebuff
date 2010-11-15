#ifndef LOCAL_HPP_INCLUDED
#define LOCAL_HPP_INCLUDED

#include <vector>
#include <memory/allocator.hpp>

template<class T>
struct local
{
  typedef std::vector<T, memory::allocator<T> > vector;
  typedef std::vector<T, memory::allocator<T> > image;
};


#endif // LOCAL_HPP_INCLUDED
