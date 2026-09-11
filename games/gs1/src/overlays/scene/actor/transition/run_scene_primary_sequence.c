#include "types.h"
#include "scene.h"

extern u8 gVal[];
extern u8 gCell[];
extern u8 gWork[];
extern s16 gCell2[][1];

s32 Actor_Run(void)
{
    u32 i;
    s32 record;

    *(s32 *)((*(u8 *volatile *)gWork + 0x1c0)) = 0x204;
    if (Actor_Check(0x109) == 0) {
        if (gCell2[224][0] != (s32)gVal) {
            goto L_0200178c;
        }
        Actor_Do(0x144);
        Actor_unk2_4();
    } else {
        L_0200178c:;
        Actor_unk3_4();
    }
    return 0;
}
