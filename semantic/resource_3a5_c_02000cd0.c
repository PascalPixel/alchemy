#include "types.h"

extern s32 Func_03000380();
extern s32 Func_080770c0();
extern void Func_080770c8();
extern void Func_080770d0();

static void SetProgressState(s32 value)
{
    u8 *workspace = *(u8 **)0x03001ebc;
    *(s16 *)(workspace + 386) = (s16)value;
}

/* Advance the three timed scene flags as the shared progress clock rises. */
void Func_02000cd0(void)
{
    s16 *timeline = (s16 *)0x02000240;
    s32 progress = Func_03000380(timeline[281] * 100, timeline[278]);

    if (Func_080770c0(0x201) != 0)
        return;

    if (Func_080770c0(0x302) != 0 && progress <= 74) {
        Func_080770d0(0x302);
        Func_080770d0(0x303);
        Func_080770d0(0x304);
        Func_080770d0(0x305);
    }
    if (Func_080770c0(0x301) != 0 && progress <= 49) {
        Func_080770d0(0x301);
        Func_080770d0(0x303);
        Func_080770d0(0x304);
        Func_080770d0(0x305);
    }
    if (Func_080770c0(0x300) != 0 && progress <= 24) {
        Func_080770d0(0x300);
        Func_080770d0(0x303);
        Func_080770d0(0x304);
        Func_080770d0(0x305);
    }

    if (Func_080770c0(0x300) == 0 && progress > 24) {
        Func_080770c8(0x300);
        SetProgressState(1);
    }
    if (Func_080770c0(0x301) == 0 && progress > 49) {
        Func_080770c8(0x301);
        SetProgressState(2);
    }
    if (Func_080770c0(0x302) == 0 && progress > 74) {
        Func_080770c8(0x302);
        SetProgressState(3);
    }
}
