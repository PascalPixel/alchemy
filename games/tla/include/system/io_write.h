#ifndef ALCHEMY_IO_WRITE_H
#define ALCHEMY_IO_WRITE_H

#include "types.h"

// A statement macro the whole game writes its registers through. An inline
// function does the same; the macro is what gives the original's code.
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

// An expression macro (the name is ours) at six sites, the saved background
// controls of the battle's mosaic fade and the blend level of Gaia Rock's
// slate: the variable is reached through its address.
/// Reads a 16-bit register or memory location into a variable.
#define Io_Read16(reg, variable) (*(variable) = *(reg))

#endif
