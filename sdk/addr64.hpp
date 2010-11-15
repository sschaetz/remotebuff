#ifndef ADDR64_HPP_INCLUDED
#define ADDR64_HPP_INCLUDED

#include <stdint.h>

namespace ext
{

  /**
   * @brief 64bit address helper
   *
   */
  union addr64
  {
    uint64_t  ull;
    uint32_t  ui[2];

    addr64() {}
    addr64(const uint64_t a) : ull(a) {}

    inline addr64& operator= (const uint64_t a)
    {
      this->ull = a;
      return *this;
    }

    inline addr64& operator= (const uint32_t a)
    {
      this->ui[0] = 0;
      this->ui[1] = a;
      return *this;
    }

    template<typename T>
    inline addr64& operator= (const T * a)
    {
#ifdef __LP64__
      this->ull = (uint64_t)a;
#else
      this->ui[0] = 0;
      this->ui[1] = (uint32_t)a;
#endif // __LP64__
      return *this;
    }

    inline addr64 operator+ (const uint64_t& b)
    {
      addr64 r;
      r.ull = this->ull + b;
      return r;
    }

    inline addr64 operator+ (const uint32_t& b)
    {
      addr64 r;
      r.ull = this->ull + (uint64_t) b;
      return r;
    }

    inline addr64 operator+ (const int& b)
    {
      addr64 r;
      r.ull = this->ull + (uint64_t) b;
      return r;
    }

#ifndef __LP64__
    inline addr64 operator+ (const long unsigned int& b)
    {
      addr64 r;
      r.ull = this->ull + (uint64_t) b;
      return r;
    }
#endif

  };
}
#endif // ADDR64_HPP_INCLUDED
