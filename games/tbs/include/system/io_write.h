#ifndef ALCHEMY_IO_WRITE_H
#define ALCHEMY_IO_WRITE_H

/// @file
/// Io_Write8, Io_Write16 and Io_Write32, writes to a register or memory.

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

/// Writes a value to an 8-bit register or memory location, in Io_Write16's
/// shape.
#define Io_Write8(v, reg) Io_Write16(v, reg)

/// Reads a 16-bit register or memory location into a variable. The variable
/// is reached through its address, so a halfword one lives in the frame and
/// is read back from there after each call. The Lost Age has the same macro.
#define Io_Read16(reg, variable) (*(variable) = *(reg))

#endif
