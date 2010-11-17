#ifndef LOCAL_HPP_INCLUDED
#define LOCAL_HPP_INCLUDED

#include <vector>
#include <memory/allocator.hpp>


struct local
{
  template<class T>
  struct vector : public std::vector<T, memory::allocator<T> >
  {

    vector()
    : std::vector<T, memory::allocator<T> >() { }

    explicit
    vector(std::size_t __n, const T& __value = T())
    : std::vector<T, memory::allocator<T> >(__n, __value)
    { }

    vector(const vector& __x)
    : std::vector<T, memory::allocator<T> >(__x)
    { }

    template<typename _InputIterator>
    vector(_InputIterator __first, _InputIterator __last)
    : std::vector<T, memory::allocator<T> >(__first, __last)
    { }

  };
};


#endif // LOCAL_HPP_INCLUDED
