/* Near miss: differ 6 of 68 (three reordered rows; differ 8 before). The empty asm on the two addresses and the one-pass block on the source fix the table load and the position add. Left: the packed-field store masks the matrix index (movs r3, #31) before it reads the byte at 23, where the native reads first, as in Animation_SetStateFlags (the bitfield store emits the mask of the value before the read; an explicit byte expression uses another mask, 193 for -63); and with the one-pass block in the same basic block two more pairs move (ands r3, r4 / lsrs r1; lsls r0 / ands r3, r2). */
#include "types.h"
#include "iwram_call.h"
#include "scene.h"
void UiWindow_SetTilemapEntry(s32, s32, s32, s32, s32);

struct Effect {
    unsigned x : 16;
    unsigned y : 16;
    unsigned z : 16;
    unsigned unk : 16;
};

struct Object {
    u8 filler0[6];
    u16 src_6;
    u8 src_8;
    u8 filler9[6];
    u8 out_15;
    u8 filler16[4];
    u8 out_20;
    u8 mode_21 : 2;
    u8 rest_21 : 6;
    u16 pos_22 : 9;
    u16 affine_22 : 5;
    u16 rest_22 : 2;
};

extern u32 gFrameTick;
extern s32 Ui_ObjectPulseScales[];
s32 AffineMatrix_BuildForEffect(struct Effect *efx);

void Ui_ApplyTableScaleToObject(struct Object *obj)
{
    s32 v;
    u32 *tick = &gFrameTick;
    u32 source;
    struct Effect efx;

    v = (s32)Ui_ObjectPulseScales;
    /* FAKEMATCH: the empty asm holds both addresses: the native loads the tick address and the table before it reads the tick and builds the mask 7 (two reordered rows). */
    __asm__("" : "+l"(tick), "+l"(v));
    v = *(s32 *)(v + (((*tick >> 1) & 7) << 2));

    if (v < 0)
        v += 255;
    v >>= 8;

    if (obj != 0) {
        efx.x = v;
        efx.y = v;
        efx.z = 0;
        obj->affine_22 = AffineMatrix_BuildForEffect(&efx);
        obj->mode_21 = 3;
        /* FAKEMATCH: the one-pass block ends the scheduling block: the source is read before 0xfff0 is built (one reordered row). */
        do {
            source = obj->src_6;
        } while (0);
        obj->pos_22 = source + 0xfff0;
        obj->out_20 = obj->src_8 + 0xf0;
        obj->out_15 = 0xfc;
    }
}
