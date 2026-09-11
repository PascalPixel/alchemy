#include "types.h"

/*
 * Battle presentation setup at 0x080ccaec.  Allocate the kind-39 and kind-40
 * work blocks, reset the presentation, write the BG2PA identity scale and the
 * blend coefficients, stream the palette selected by the caller's kind into
 * palette RAM through the IWRAM word-copy kernel, seed three work-block
 * fields, then schedule two frame callbacks.  The work-block offsets are
 * taken by position and are not verified.
 */

typedef s32 (*WordCopyFn)(void *dest, const void *src, s32 words);

/*
 * Value_ symbols name a small resource id the reference loads from its
 * literal pool rather than materializing, so they must not be folded into
 * constants.  Func_080041d8 is declared as returning s32 although the result
 * is unused: the value-returning call form keeps the call the last setter of
 * r0, which is what places each callback's pool load after the order
 * argument's shift.
 */

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
