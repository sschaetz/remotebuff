#ifndef VECTOR_SLICER_HPP_INCLUDED
#define VECTOR_SLICER_HPP_INCLUDED

struct vector_slicer
{
  int iterationsize;
  int rankoffset;
  vector_slicer(std::size_t buffersize_):
  iterationsize(SPE_Size() * buffersize_), rankoffset(SPE_Rank() * buffersize_)
  { }

  int32_t operator()(uint32_t iteration)
  {
    return iteration * iterationsize + rankoffset;
  }
};

#endif // VECTOR_SLICER_HPP_INCLUDED
