#ifndef IMAGE_HPP_INCLUDED
#define IMAGE_HPP_INCLUDED

#include <stdlib.h>
#include <vector>
#include <containers/local.hpp>


template<typename T>
struct make
{
  typedef typename local<T>::image ImageType;

  static ImageType image(std::size_t dimX, std::size_t dimY)
  {
    return ImageType(dimX * dimY);
  }
};


#endif // IMAGE_HPP_INCLUDED
