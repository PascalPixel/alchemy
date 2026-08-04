#include "types.h"

/*
 * Screen placement for one scene object: rebuild the shared transform from
 * the scene context at 0x03001e80, project the object's position into the
 * caller's screen block, then pull the block's y coordinate up by the
 * object's own height.  The pull-up is the projection scale times the
 * object's record field at +0x18, times the object's scene height shifted
 * down by 17 (the 16.16 fraction halved).  The function always reports zero.
 *
 * The two `mov ip, pc ; bx r5` sequences are ordinary indirect calls through
 * the pooled 0x03000118, the ARM fixed-point multiply relocated into IWRAM:
 * `mov ip, pc` sets the return address to the halfword after the `bx`, so
 * each returns into the same frame with the live registers unchanged.  The
 * shape is the one already resolved at 0x0800ebec and is written here as a
 * plain call through a function pointer.  The `movs r0, r0` before the second
 * is alignment filler.
 *
 * One direct caller, 0x080e395c.
 */

/* 0x03000118, the IWRAM-relocated ARM fixed-point multiply. */
typedef s32 (*Multiply_080e3994)(s32 a, s32 b);

struct Object_080e3994 {
    u8 unknown_00[8];
    s32 position_08[3];
};

struct Record_080e3994 {
    u8 unknown_00[0x18];
    s32 scale_18;
};

struct Slot_080e3994 {
    struct Object_080e3994 *object;
};

struct Screen_080e3994 {
    s32 x;
    s32 y;
    s32 depth;
};

void Func_080049ac(void);
void Func_080051d8(void *context, void *transform);
s32 Func_08005268(const s32 *source, s32 *projected);
s32 Func_080b5070(s32 id);
struct Slot_080e3994 *Func_080b5098(s32 id);
struct Record_080e3994 *Func_080b50d8(struct Object_080e3994 *object,
                                      s32 kind);

s32 Func_080e3994(s32 id, struct Screen_080e3994 *screen)
{
    Multiply_080e3994 multiply = (Multiply_080e3994)0x03000118;
    void *context = *(void **)0x03001e80;
    struct Object_080e3994 *object = Func_080b5098(id)->object;
    struct Record_080e3994 *record = Func_080b50d8(object, 0);
    s32 scale;

    Func_080049ac();
    Func_080051d8(context, (u8 *)context + 12);

    scale = multiply(Func_08005268(object->position_08, (s32 *)screen),
                     record->scale_18);
    screen->y -= multiply(scale, Func_080b5070(id) >> 17);
    return 0;
}
