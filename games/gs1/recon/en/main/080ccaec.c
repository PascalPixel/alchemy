#include "types.h"

/*
 * Battle-presentation setup at 0x080ccaec, reached through the long-call
 * veneer Func_080c9038.
 *
 * Allocates the kind-39 work block (0x782c bytes) and the kind-40 block
 * (0x4000 bytes), resets the presentation through Func_080cd594, writes the
 * BG2PA identity scale and the 0x1010 blend coefficients, selects one of
 * five palette resources by the caller's kind, streams its 128 bytes into
 * palette RAM through the IWRAM word-copy kernel at 0x03001388, seeds three
 * work-block fields, then schedules Func_080cc960 and Func_080cd260 as frame
 * callbacks.  Field offsets follow the kind-39 work block already used by
 * games/gs1/src/battle/effects/puff_arc/run.c and member_orbit/run.c.
 *
 * Value_ symbols are the established spelling for a small resource id the
 * reference loads from its literal pool instead of materializing.
 *
 * Func_080041d8 is declared with its adopted s32 result
 * (games/gs1/src/runtime/scheduler/callback_scheduler.c).  The result is
 * unused here, but the value-returning call form is load-bearing: it makes
 * the call the last setter of r0, which is what places each callback's pool
 * load after the order argument's shift in the reference.  A void prototype
 * costs four differing halfwords.
 */

typedef s32 (*WordCopyFn)(void *dest, const void *src, s32 words);

extern u8 Value_000000c8;
extern u8 Value_000000cf;
extern u8 Value_000000b4;
extern u8 Value_000000cb;
extern u8 Value_000000be;

s32 Func_080048b0(s32 kind, s32 size);
void Func_080cd594(s32 mode);
void *Func_08002f40(s32 id);
s32 Func_080041d8(s32 callback, s32 order);
void Func_080cc960(void);
void Func_080cd260(void);

void Func_080ccaec(s32 kind)
{
    u8 *work;
    void *palette;
    s32 id;

    work = (u8 *)Func_080048b0(39, 0x782c);
    Func_080048b0(40, 0x4000);
    Func_080cd594(0);
    *(s32 *)(work + 0x77b4) = 24;
    *(s16 *)0x04000020 = 0x100;
    *(s16 *)0x04000052 = 0x1010;
    switch (kind) {
    case 0:
        id = (s32)&Value_000000c8;
        break;
    case 1:
        id = (s32)&Value_000000cf;
        break;
    case 2:
        id = (s32)&Value_000000b4;
        break;
    case 3:
        id = (s32)&Value_000000cb;
        break;
    case 4:
    default:
        id = (s32)&Value_000000be;
        break;
    }
    palette = Func_08002f40(id);
    ((WordCopyFn)0x03001388)((void *)0x05000000, palette, 128);
    *(s32 *)(work + 0x778c) = 0;
    *(s32 *)(work + 0x7780) = 3;
    *(s32 *)(work + 0x7784) = 0x06060606;
    Func_080041d8((s32)Func_080cc960, 0xC80);
    Func_080041d8((s32)Func_080cd260, 0x480);
}
