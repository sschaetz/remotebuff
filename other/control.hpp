#ifndef CONTROL_HPP_INCLUDED
#define CONTROL_HPP_INCLUDED

#include <containers/remote.hpp>

struct control
{
  remote::vector<float> input;
  remote::vector<float> output;
};

#endif // CONTROL_HPP_INCLUDED
