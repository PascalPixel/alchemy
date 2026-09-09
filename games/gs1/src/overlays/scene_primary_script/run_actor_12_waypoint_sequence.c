#include "types.h"

#define FieldScene_RunActor12WaypointSequence Func_020015d0

extern u8 *Func_02004394(s32);
extern void Func_02004382(void);
extern void Func_0200248e(s32, s32);
extern void Func_02002386(s32, s32, s32, s32);
extern void Func_02004398(s32);
extern void Func_02002102(s32, s32, s32, s32, s32, s32, s32, s32);
extern void Func_02004476(s32, s32);
extern void Func_02004448(s32, s32, s32);
extern void Func_020043ce(s32);
extern void Func_0200443e(s32, s32);
extern void Func_02004494(s32, s32);
extern void Func_020043ee(s32);
extern void Func_020023ee(s32, s32, s32, s32);
extern void Func_0200447c(s32, s32, s32);
extern void Func_0200440a(s32);
extern void Func_0200240a(s32, s32, s32, s32);
extern void Func_02004498(s32, s32, s32);
extern void Func_02004426(s32);
extern void Func_02002426(s32, s32, s32, s32);
extern void Func_020044b4(s32, s32, s32);
extern void Func_02004442(s32);
extern void Func_02002442(s32, s32, s32, s32);
extern void Func_020044d0_a(s32, s32, s32);
extern void Func_0200445e(s32);
extern void Func_020044c0(s32, s32, s32);
extern void Func_02004466(s32);
extern void Func_020044d0_b(s32, s32, s32);
extern void Func_0200448c(void);

void FieldScene_RunActor12WaypointSequence(void)
{
    u8 *slot;

    slot = Func_02004394(12);

    Func_02004382();

    Func_0200248e(12, 1);
    Func_02002386(12, 0x188, 104, 0x70000);
    Func_02004398(10);

    Func_02002102(*(s32 *)(slot + 8), *(s32 *)(slot + 12),
                  *(s32 *)(slot + 16) + 0x40000,
                  0, 0, 0, 1, 0);

    Func_02004476(12, 1);
    Func_02004448(12, 0, 0);
    Func_020043ce(20);
    Func_0200443e(12, 2);
    Func_02004494(12, 258);
    Func_020043ee(60);

    Func_020023ee(12, 0x1a8, 120, 0x30000);
    Func_0200447c(0, 12, 0);
    Func_0200440a(6);

    Func_0200240a(12, 0x1a8, 168, 0x30000);
    Func_02004498(0, 12, 0);
    Func_02004426(6);

    Func_02002426(12, 0x1a8, 208, 0x30000);
    Func_020044b4(0, 12, 0);
    Func_02004442(6);

    Func_02002442(12, 0x1a8, 232, 0x30000);
    Func_020044d0_a(0, 12, 0);
    Func_0200445e(6);

    Func_020044c0(12, 0, 0);
    Func_02004466(0x303);
    Func_020044d0_b(15, 0, 0);

    Func_0200448c();
}
