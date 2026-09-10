#include "types.h"

#define NULL ((void *)0)
#define OverlayObject_CreateAndInitialize Func_02000048
#define SceneEffect_UpdateObjectByFrameParity Func_020020dc
#define SceneEffect_AdvanceGatedRiseCounter Func_020059f0

void *Func_02005cc6(s32, s32, s32, s32);
void Func_02005d2c(void *, s32);
void Func_02005d44(void *, s32);
void Func_02005e74(void *, s32);
void Func_02007dc4(s32, s32);
void Func_02007dce(s32, s32);
s32 Func_02007cf8(s32, s32);
void Func_0200430e(s32);
void Func_0200b096(void *record);

/* The +98 byte is read directly in both tests and incremented in place rather
 * than cached in a local; that shape is what the reference holds. */

/*
 * Gated by the byte flag at +99: recompute a fixed-point offset from the byte
 * counter at +98 into the u32 field at +12, call a leaf helper, then advance
 * that counter while it stays in (0, 31]. The 56-byte owner has no literal
 * pool. obj is read before it is overwritten, so it is a real argument. The
 * layout behind +12, +76, +98 and +99 is not established.
 */

void *OverlayObject_CreateAndInitialize(s32 x, s32 y, s32 z, s32 kind)
{
    u8 *ret = Func_02005cc6(kind, x, y, z);

    if (ret != NULL) {
        u8 *obj = *(u8 **)(ret + 0x50);
        s32 flags;
        s32 mask = 13;

        flags = obj[9];
        mask = -mask;
        mask &= flags;
        obj[9] = mask;
        ret[0x55] = 0;
        ret[0x59] = 8;
        Func_02005d2c(ret, 0);
        Func_02005e74(ret, 14);
        Func_02005d44(ret, 1);
        return ret;
    }
    return NULL;
}

void SceneEffect_UpdateObjectByFrameParity(s32 a)
{
    if (*(s32 *)0x03001e40 & 2) {
        Func_02007dc4(a, 7);
    } else {
        Func_02007dce(a, 0);
    }
    if (Func_02007cf8(*(s32 *)0x03001e40, 15) == 0) {
        Func_0200430e(a);
    }
}

void SceneEffect_AdvanceGatedRiseCounter(u8 *obj)
{
    if (*(u8 *)(obj + 99) != 0) {
        u8 counter = *(u8 *)(obj + 98);

        *(u32 *)(obj + 12) = *(u32 *)(obj + 76) + ((u32)(counter >> 2) << 16);

        Func_0200b096(obj);

        {
            if (*(u8 *)(obj + 98) != 0) {
                if (*(u8 *)(obj + 98) <= 31) {
                    ++*(u8 *)(obj + 98);
                }
            }
        }
    }
}
