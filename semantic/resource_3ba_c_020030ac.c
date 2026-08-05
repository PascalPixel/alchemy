#include "types.h"

/*
 * Resource 3ba owner at 0x020030ac (276 bytes), the independently resolved
 * counterpart of resource_3bc:3ddc.
 *
 * Complete owner: r8/sl-saving prologue at 0x020030ac through the sole
 * interworking return at 0x02003194-0x020031a0, followed by alignment and the
 * seven referenced pool words through 0x020031bf.  The next owner starts at
 * 0x020031c0.
 *
 * A story flag selects one of two workspace cues and caller handles.  The
 * selected scene record is positioned relative to workspace coordinates,
 * assigned an animation script, and waited to completion before the selected
 * handle and two shared handles are refreshed.  The routine returns the story
 * flag value preserved across all later calls.
 *
 * All thirteen calls resolve independently: the two local calls reach the
 * resource_3ba owner at 0x020020e8, while the remaining eleven reach the same
 * established imports as the resource_3bc family member.
 */

extern u8 Data_02000240[];

s32 Func_080770c0();
void Func_080770c8();
u8 *Func_0808a080();
void Func_08009150();
void Func_08009098();
void Func_080000c0();
void Func_020020e8();
void Func_08015120();
void Func_08015040();
void Func_08009148();

s32 Func_020030ac(s32 handleA, s32 handleB)
{
    s32 permuted_3;
    s32 permuted_4;
    u8 *workspace = *(u8 **)0x03001f3c;
    u8 *record;
    s32 flag = Func_080770c0(0x211);
    s32 selector = *(s32 *)&Data_02000240[500];
    s32 x;
    s32 z;
    u16 cue;

    record = Func_0808a080(selector);

    if (*(s32 *)(workspace + 232) < *(s32 *)(record + 8)) {
        x = *(s32 *)(workspace + 232) + 0xc0000;
    } else {
        x = *(s32 *)(workspace + 232) - 0xc0000;
    }

    if (flag != 0) {
        z = *(s32 *)(workspace + 236) + 0x100000;
        cue = *(u16 *)(workspace + 228);
    } else {
        permuted_3 = *(s32 *)(workspace + 236) - 0x100000;
        permuted_4 = *(u16 *)(workspace + 226);
        z  = permuted_3;
        cue  = permuted_4;
    }

    *(s32 *)(record + 52) = 0x4000;
    *(s16 *)(record + 100) = (s16)cue;
    *(s32 *)(record + 48) = 0x10000;

    Func_08009150(record, x, 0, z);
    Func_080770c8(0x211);
    Func_08009098(record, (void *)0x0200c6fc);

    while (*(s16 *)(record + 100) != 0) {
        Func_080000c0(1);
    }

    if (flag == 0) {
        Func_020020e8(0, handleA);
        Func_08015120(handleA, 2);
    } else {
        Func_020020e8(0, handleB);
        Func_08015120(handleB, 2);
    }

    Func_08015120(selector, 1);
    selector = *(s32 *)&Data_02000240[500];
    Func_08015040(0x96a, 3);
    Func_08009148(record);

    return flag;
}
