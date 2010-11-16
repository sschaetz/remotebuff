

#include <stdio.h>

#include <containers/local.hpp>
#include <containers/remote.hpp>
#include <containers/image.hpp>

#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/home/phoenix/function/function.hpp>
#include <boost/spirit/home/phoenix/operator/arithmetic.hpp>
#include <boost/spirit/home/phoenix/core/argument.hpp>


#include <boost/function.hpp>

using namespace boost;
using namespace boost::phoenix::arg_names;


int main(void)
{

  printf("hi!\n");
  local<float>::vector v(100);
  local<float>::image i = make<float>::image(100, 100);

  remote<float>::vector vr;
  vr = v;
  remote<float>::vector vr2 = v;

  float z = 13;

  boost::function<float (float x, float y, float z)> f =
    (arg1 + arg2) + z;

  printf("%f\n", f(1.0, 2.0, 3.0));

}


