#include "types.h"






static void SetProgressState(s32 value)
{
    u8 *workspace = *(u8 **)0x03001ebc;
    *(s16 *)(workspace + 386) = (s16)value;
}

/* Advance the three timed scene flags as the shared progress clock rises. */
extern s32 Func_02002976();
extern s32 Func_02002a3e();
extern s32 Func_02002a48();
extern void Func_02002a66();
extern void Func_02002a6c();
extern void Func_02002a74();
extern void Func_02002a7a();
extern s32 Func_02002a70();
extern void Func_02002a8e();
extern void Func_02002a94();
extern void Func_02002a9c();
extern void Func_02002aa2();
extern s32 Func_02002a9a();
extern void Func_02002aba();
extern void Func_02002ac0();
extern void Func_02002ac8();
extern void Func_02002ace();
extern s32 Func_02002ac6();
extern void Func_02002ade();
extern s32 Func_02002ae6();
extern void Func_02002afc();
extern s32 Func_02002b04();
extern void Func_02002b1a();
void Func_02000cd0(void)
{
    s16 *timeline = (s16 *)0x02000240;
    s32 progress = Func_02002976(timeline[281] * 100, timeline[278]);

    if (Func_02002a3e(0x201) != 0)
        return;

    if (Func_02002a48(0x302) != 0 && progress <= 74) {
        Func_02002a66(0x302);
        Func_02002a6c(0x303);
        Func_02002a74(0x304);
        Func_02002a7a(0x305);
    }
    if (Func_02002a70(0x301) != 0 && progress <= 49) {
        Func_02002a8e(0x301);
        Func_02002a94(0x303);
        Func_02002a9c(0x304);
        Func_02002aa2(0x305);
    }
    if (Func_02002a9a(0x300) != 0 && progress <= 24) {
        Func_02002aba(0x300);
        Func_02002ac0(0x303);
        Func_02002ac8(0x304);
        Func_02002ace(0x305);
    }

    if (Func_02002ac6(0x300) == 0 && progress > 24) {
        Func_02002ade(0x300);
        SetProgressState(1);
    }
    if (Func_02002ae6(0x301) == 0 && progress > 49) {
        Func_02002afc(0x301);
        SetProgressState(2);
    }
    if (Func_02002b04(0x302) == 0 && progress > 74) {
        Func_02002b1a(0x302);
        SetProgressState(3);
    }
}
