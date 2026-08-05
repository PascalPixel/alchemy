#include "types.h"

/*
 * resource_387 paired-actor scene owner at 0x0200043c, complete 144-byte span
 * through its three-word pool. It stages actors 0 and 9, derives their shared
 * offset from workspace +364, advances both, then runs the tile/flag owner.
 */

extern u8 *Data_03001ebc;












extern void Func_02001570(void);
extern void Func_020015d0(s32 actor, s32 mode);
extern void Func_02001576(s32 frames);
extern void Func_020015a8(s32 actor, s32 scale, s32 secondaryScale);
extern void Func_020015b2(s32 actor, s32 scale, s32 secondaryScale);
extern void Func_02001698(s32 cue);
extern void Func_020015f4(s32 actor, s32 offset, s32 mode);
extern void Func_020015fe(s32 actor, s32 offset, s32 mode);
extern void Func_0200160c(s32 actor);
extern void Func_02001612(s32 actor);
extern void Func_020015c8(s32 frames);
extern void Func_02001630(s32 actor, s32 mode);
extern void Func_020007f8(void);
extern void Func_020016d8(void);
extern void Func_020015ec(void);
void Func_0200043c(void)
{
    s32 offset;

    Func_02001570();
    Func_020015d0(0, 8);
    Func_02001576(20);
    Func_020015a8(0, 0x3333, 0x1999);
    Func_020015b2(9, 0x3333, 0x1999);
    Func_02001698(185);

    Func_020015f4(0, offset, 0);
    offset = (11 - *(s16 *)(Data_03001ebc + 364) * 2) << 4;
    Func_020015fe(9, offset, 0);
    Func_0200160c(0);
    Func_02001612(9);
    Func_020015c8(20);
    Func_02001630(0, 1);
    Func_020007f8();
    Func_020016d8();
    Func_020015ec();
}
