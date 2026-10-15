#include "garoh.h"

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

// The fourth argument is passed unextended (0xf000), where spawn.c's
// definition takes an s16: the callers saw an int parameter.
void Func_02001360(s32 x, s32 y, s32 z, s32 value, s32 timer);

void Func_020013b0(s32 id)
{
    struct ObjectRuntime *object = Object_GetById(id);
    s32 x = object->x;
    s32 dx;
    s32 y;

    /* FAKEMATCH: the native loads x, builds the x offset, loads y and then adds the offset; without these three barriers y is loaded after the add (one reordered pair, differ 2). */
    __asm__ volatile("" : "+r"(x));
    dx = 0x80000;
    y = object->y;
    /* FAKEMATCH: third barrier of the three above. */
    __asm__ volatile("" : "+r"(dx));
    Func_02001360(x + dx, y + 0x1c0000, object->z, 0x1000, 20);
    Func_02005394(151);
    Battle_WaitMode0(20);
    Func_02001360(object->x - 0x80000, object->y + 0x1c0000, object->z, 0xf000, 20);
    Func_02005394(151);
    Battle_WaitMode0(20);
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
