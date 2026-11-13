#ifndef ALCHEMY_IO_WRITE_H
#define ALCHEMY_IO_WRITE_H

/// @file
/// Io_Write16 and Io_Write32, writes to a register or memory.

#include "types.h"

/// Writes a value to a 16-bit register or memory location.
#define Io_Write16(v, reg)                                                     \
  do {                                                                         \
    u32 value_ = (v);                                                          \
    *(reg) = value_;                                                           \
  } while (0)

/// Writes a value to a 32-bit register or memory location, in Io_Write16's
/// shape.
#define Io_Write32(v, reg) Io_Write16(v, reg)

#endif
