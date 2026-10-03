#ifndef ALCHEMY_FXBLEND_H
#define ALCHEMY_FXBLEND_H

#include "TYPES.H"
#include "LAYOUT_GUARD.H"

/* Heap slot 30. The field constructors clear the whole block and build
   two pictures and their per-frame difference; the storm task plays them. */
struct FieldBlendWork {
    s16 from[0x540];
    s16 to[0x540];
    s16 delta[0x540];
    s16 timer;
    s16 enabled;
    s16 loud;
    u8 unknown_1f86[2];
};

LAYOUT_SIZE_GUARD(FieldBlendWork_Size, struct FieldBlendWork, 0x1f88);
LAYOUT_OFFSET_GUARD(FieldBlendWork_To, struct FieldBlendWork, to, 0xa80);
LAYOUT_OFFSET_GUARD(FieldBlendWork_Delta, struct FieldBlendWork, delta, 0x1500);
LAYOUT_OFFSET_GUARD(FieldBlendWork_Timer, struct FieldBlendWork, timer, 0x1f80);
LAYOUT_OFFSET_GUARD(FieldBlendWork_Loud, struct FieldBlendWork, loud, 0x1f84);

#endif
