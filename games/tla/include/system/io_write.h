#ifndef ALCHEMY_IO_WRITE_H
#define ALCHEMY_IO_WRITE_H

#include "types.h"

// A statement macro (S2: a layer the whole game uses, 43 sites): the value is
// built in a word before the address and the store stays where it is written.
// An inline function with a u32 parameter builds the same value but lets the
// store move.
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

// An expression macro (S2: six sites, the saved background controls of the
// battle's mosaic fade and the blend level of Gaia Rock's slate; the name is
// ours): the variable is reached through its address, so a halfword one lives
// in the frame and is read back from there after each call.
/// Reads a 16-bit register or memory location into a variable.
#define Io_Read16(reg, variable) (*(variable) = *(reg))

#endif
