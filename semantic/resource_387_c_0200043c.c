#include "types.h"

/*
 * resource_387 paired-actor scene owner at 0x0200043c, complete 144-byte span
 * through its three-word pool. It stages actors 0 and 9, derives their shared
 * offset from workspace +364, advances both, then runs the tile/flag owner.
 */

extern u8 *Data_03001ebc;

extern void Func_02000348(void);
extern void Func_0808a010(s32 frames);
extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern void Func_0808a090(s32 actor, s32 scale, s32 secondaryScale);
extern void Func_0808a0e0(s32 actor, s32 offset, s32 mode);
extern void Func_0808a0e8(s32 actor);
extern void Func_0808a100(s32 actor, s32 mode);
extern void Func_0808a5e8(void);
extern void Func_080f9010(s32 cue);

void Func_0200043c(void)
{
    s32 offset;

    Func_0808a018();
    Func_0808a100(0, 8);
    Func_0808a010(20);
    Func_0808a090(0, 0x3333, 0x1999);
    Func_0808a090(9, 0x3333, 0x1999);
    Func_080f9010(185);

    Func_0808a0e0(0, offset, 0);
    offset = (11 - *(s16 *)(Data_03001ebc + 364) * 2) << 4;
    Func_0808a0e0(9, offset, 0);
    Func_0808a0e8(0);
    Func_0808a0e8(9);
    Func_0808a010(20);
    Func_0808a100(0, 1);
    Func_02000348();
    Func_0808a5e8();
    Func_0808a020();
}
