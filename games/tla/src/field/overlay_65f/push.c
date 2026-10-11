#include "ov65f.h"

void Func_02005194(s32 src_x, s32 src_y, s32 width, s32 height, s32 dest_x, s32 dest_y);
void Func_02005304(s32 a, s32 b);
void Func_0200530c(s32 a, s32 b);
void Func_02005314(void);
void Func_0200531c(void);
void Field_DispatchTypeHandler(s32 type);

static inline void BlendBack(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    /* FAKEMATCH: as in ☀️'s scene layer, the inline parameters give each stack argument its own register; a direct call reuses r3 */
    Func_02005194(a0, a1, a2, a3, a4, a5);
}

void Func_02001418(s32 actor)
{
    Func_02005304(144, 1);
    Func_0200530c(actor, 0);
    Field_DispatchTypeHandler(1);
    BlendBack(44, 5, 3, 3, 19, 5);
    Func_02005314();
    Func_0200531c();
}

void Func_02001454(s32 actor)
{
    Func_02005304(144, 1);
    Func_0200530c(actor, 0);
    Field_DispatchTypeHandler(3);
    BlendBack(39, 5, 3, 3, 19, 5);
    Func_02005314();
    Func_0200531c();
}
