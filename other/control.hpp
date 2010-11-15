#ifndef CONTROL_HPP_INCLUDED
#define CONTROL_HPP_INCLUDED

#include <containers/remote.hpp>

struct control
{
  remote<float>::vector input;
  remote<float>::vector output;
};

#endif // CONTROL_HPP_INCLUDED
