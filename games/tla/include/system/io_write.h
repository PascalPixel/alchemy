#ifndef ALCHEMY_IO_WRITE_H
#define ALCHEMY_IO_WRITE_H

#include "types.h"

/// @file
/// The register writes and reads the whole game makes through a local.

/// Writes a value to a 16-bit register or memory location, through a local of
/// its own.
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

/// Reads a 16-bit register or memory location into the variable at
/// `variable`: the battle's mosaic fade saves its background controls so, and
/// Gaia Rock's slate and Anemos Inner Sanctum's symbol their blend levels.
#define Io_Read16(reg, variable) (*(variable) = *(reg))

#endif
