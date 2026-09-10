#include "types.h"

#define FieldScene_RunSceneEntryHook Func_02001a0c
extern s16 Data_02000240[];
extern u8 Value_0000006a;

u8 *Func_02003566();
void Func_02003524();
u8 *Func_02003572();
void Func_02003530();
u8 *Func_0200357e();
void Func_0200353c();
u8 *Func_0200358a();
void Func_02003548();
u8 *Func_02003596();
s32 Func_02003568();
void Func_02001c7c();
s32 Func_02003576();
void Func_02001dfe();
s32 Func_02003586();
void Func_02001b5e();
s32 Func_02003594();
void Func_02003610();
s32 Func_020035a6();
void Func_02003622();

/*
 * Scene-entry hook for resource_3ad: seed the workspace scene id, then run
 * the per-actor setup and the flag-gated steps for that scene.
 */

/* Old-style declarations: overlay import arities vary per call site. */

/*
 * The owner returns 0 on every path, and its 200 bytes include the seven
 * pool words past the return.  0x204 is stored into the workspace at +448
 * and read again later as a story-flag id, so both uses share one value.
 * The scene id is compared against 0x6a spelled as the address of
 * Value_0000006a; that spelling is what reproduces the reference.
 */
s32 FieldScene_RunSceneEntryHook(void)
{
    u8 *workspace;
    workspace = *(u8 **)0x03001ebc;
    *(s32 *)(workspace + 448) = 0x204;

    if (Data_02000240[224] == (s32)&Value_0000006a) {
    Func_02003524(Func_02003566(8), 0);
    Func_02003530(Func_02003572(9), 0);
    Func_0200353c(Func_0200357e(10), 0);
    Func_02003548(Func_0200358a(11), 0);

    /* Word field at +28 of record 11.  0xf333 comes from the pool. */
    *(s32 *)(Func_02003596(11) + 28) = 0xf333;

    if (Func_02003568(0x201) != 0) {
        Func_02001c7c();
    }
    if (Func_02003576(0x202) != 0) {
        Func_02001dfe();
    }
    /* 0x200 is built here rather than taken from the pool. */
    if (Func_02003586(0x200) != 0) {
        Func_02001b5e();
    }
    if (Func_02003594(0x203) != 0) {
        Func_02003610(11, 5);
    }
    if (Func_020035a6(0x204) != 0) {
        Func_02003622(9, 5);
    }
    }

    return 0;
}
