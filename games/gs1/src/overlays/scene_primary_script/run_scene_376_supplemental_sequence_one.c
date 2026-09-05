#include "types.h"

#define FieldScene_RunSupplementalSequenceOne Func_02000368

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_02000240[];
extern u8 Data_03001ebc[];
extern s16 Data_02000240_t[][1];
void Func_02000aba();
s32 Func_02001500();
void Func_0200157a();
void Func_0200158e();
void Func_02001598();
void Func_020015a2();
void Func_020015ac();
void Func_020015b6();
void Func_020015c0();
s32 Func_020015c4();
void Func_020015ca();
void Func_020015d4();
s32 Func_020015de();
void Func_020015e8();
void Func_020015f2();
void Func_020015fc();
void Func_02001606();
s32 Func_0200160c();
void Func_02001610();
void Func_0200161a();
void Func_02001620();
u8 *Func_0200162c();
u8 *Func_02001664();
void Func_0200169e();
void Func_020016ba();
void Func_020016ce();
void Func_020016ec();
void Func_02001706();
void Func_0200171a();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

s32 Func_02000368(void)
{
    u32 i;
    u8 *record;
    u8 *volatile *scene = (u8 *volatile *)Data_03001ebc;

    *(s32 *)(scene[0] + 0x1c0) = 0x209;
    if (Value1(Func_02001500, 0x834) != 0) {
        Func_0200158e(8, 0, 0);
        Func_02001598(9, 0, 0);
        Func_020015a2(10, 0, 0);
        Func_020015ac(11, 0, 0);
        Func_020015b6(12, 0, 0);
        Func_020015c0(13, 0, 0);
        Func_020015ca(14, 0, 0);
        Func_020015d4(15, 0, 0);
        ((void (*)())Func_020015de)(16, 0, 0);
        Func_020015e8(17, 0, 0);
        Func_020015f2(18, 0, 0);
        Func_020015fc(19, 0, 0);
        Func_02001606(20, 0, 0);
        Func_02001610(21, 0, 0);
        Func_0200161a(22, 0, 0);
        Func_020016ce();
        {
            u16 *target = (u16 *)(scene[3] + 0x1f84);
            s32 shown = 1;
        
            *target = shown;
        }
        Func_020016ec();
        Func_0200157a(30);
        Func_02001706();
        Func_0200171a();
        Func_02001706();
    }
    if (Value1(Func_020015c4, 0x87a) != 0) {
        if (Data_02000240_t[225][0] == 6) {
            if (Value1(Func_020015de, 0x81d) == 0) {
                Func_02000aba();
            }
        }
        {
            u8 *record = Func_0200162c(10);
            u8 value = *(volatile u8 *)&record[89];
        
            record[89] = (u8)(value | 128);
        }
    }
    if (Data_02000240_t[225][0] == 2) {
        if (Value1(Func_0200160c, 0x815) != 0) {
            Call3(Func_0200169e, 13, 0x1c60000, 0x960000);
            record = Func_02001664(13);
            Func_0200161a((s32)record, 0);
            Func_020016ba(13, 5);
            Func_02001620(4);
        }
    }
    return 0;
}
