#ifndef ALCHEMY_TBS_IOWRITE_H
#define ALCHEMY_TBS_IOWRITE_H

#include "types.h"

// 16 ビットのレジスタ（またはメモリ）に値を書く
/* A statement macro (S2: a layer the whole game uses, 106 sites): the value is
 * built in a word before the address and the store stays where it is written.
 * An inline function with a u32 parameter builds the same value but lets the
 * store move. */
#define Io_Write16(v, reg)                                                     \
  do {                                                                         \
    u32 value_ = (v);                                                          \
    *(reg) = value_;                                                           \
  } while (0)

#endif
