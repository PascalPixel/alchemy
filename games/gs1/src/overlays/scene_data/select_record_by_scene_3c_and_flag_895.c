#include "types.h"

/*
 * The Xian roof selector: the published scene word at Data_02000240 + 448
 * chooses one of three records. On the fall-through arm the record at
 * 0x0200c998 is primed with cue 0x895 in four slots and two fixed camera
 * words when story flag 0x895 is already set, then announced with
 * Func_020047dc. 0x02000240 is an external EWRAM global below the image;
 * the three records are in-image data at file offsets 0x48f0, 0x4ae8 and
 * 0x4998, so the stores below write the overlay's own writable EWRAM.
 */

struct SceneStage {
    u8 unk_000[0x7a];
    u16 cue_07a;
    u8 unk_07c[0x2e];
    u16 cue_0aa;
    u8 unk_0ac[0x1c];
    s32 field_0c8;
    u8 unk_0cc[4];
    s32 field_0d0;
    u8 unk_0d4[0x36];
    u16 cue_10a;
    u8 unk_10c[0x16];
    u16 cue_122;
};

extern s16 Data_02000240[];     /* external EWRAM globals, not overlay data */
extern u8 Value_0000003c;
extern u8 Data_0200c8f0[];      /* in-image at file offset 0x48f0 */
extern u8 Data_0200cae8[];      /* in-image at file offset 0x4ae8 */
extern u8 Data_0200c998[];      /* in-image at file offset 0x4998 */

/* Imports. Old-style declarations: one name can take different argument
 * counts at different sites in this overlay. */
s32 Func_02004772();            /* story-flag test */
void Func_020047dc();           /* record hand-off */

/* The flag id reaches the test straight in r0; calling directly would share
 * one pool constant with the stores below instead of reloading it. */
static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

#define SceneData_SelectRecordByScene3cAndFlag895 Func_02000388

s32 SceneData_SelectRecordByScene3cAndFlag895(void)
{
    if (Data_02000240[224] == (s32)&Value_0000003c) {
        return (s32)Data_0200c8f0;
    }
    if (Data_02000240[225] == 3) {
        return (s32)Data_0200cae8;
    }
    if (Value1(Func_02004772, 0x895) != 0) {
        struct SceneStage *stage = (struct SceneStage *)Data_0200c998;

        stage->cue_07a = 0x895;
        stage->cue_0aa = 0x895;
        stage->field_0c8 = 0x1200000;
        stage->field_0d0 = 0xf80000;
        stage->cue_10a = 0x895;
        stage->cue_122 = 0x895;
    }
    Func_020047dc((s32)Data_0200c998);
    return (s32)Data_0200c998;
}
