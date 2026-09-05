#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_00001f95[];
extern u8 Data_00001f96[];
extern u8 Data_00001faa[];
extern u8 Data_00001fab[];
extern u8 Data_000021e2[];
extern u8 Data_000021e3[];
extern u8 Data_000021f5[];
extern u8 Data_000021f6[];
extern u8 Data_00002365[];
extern u8 Data_00002366[];
extern u8 Data_00002371[];
extern u8 Data_00002372[];
extern u8 Data_03001ebc[];
void Func_0200106a();
s32 Func_02001070();
s32 Func_0200107a();
s32 Func_0200108c();
void Func_0200109a();
s32 Func_020010a0();
s32 Func_020010ae();
void Func_0200110e();
void Func_0200113c();
void Func_0200114a();
s32 Func_02001150();
s32 Func_0200115a();
s32 Func_0200116c();
void Func_0200116e();
void Func_0200117a();
s32 Func_02001180();
s32 Func_0200118e();
void Func_020011a0();
void Func_020011ee();
void Func_0200121c();
void Func_0200124c();
void Func_0200124e();
void Func_02001280();


void Func_020003d0(void)
{
    u8 *__restrict p5;
    s32 rec;
    s32 p8;
    s32 msg;

    p5 = *(u8 **)Data_03001ebc;
    rec = Func_0200115a(17);
    p8 = *(s16 *)(rec + 6);
    Func_0200114a();
    {
        volatile u16 *flags = (volatile u16 *)(rec + 100);
        u16 value = *flags;

        *flags = (u16)(value | 2);
    }
    if (*(s16 *)(p5 + 0x17e) == 0) {
        if (Func_02001150(0x950) != 0) {
            msg = (s32)Data_00002366;
        } else if (Func_0200116c(0x962) != 0) {
            msg = (s32)Data_000021e3;
        } else {
            msg = (s32)Data_00001f96;
        }
    } else {
        if (Func_02001180(0x950) != 0) {
            msg = (s32)Data_00002372;
        } else if (Func_0200118e(0x962) != 0) {
            msg = (s32)Data_000021f6;
        } else {
            msg = (s32)Data_00001fab;
        }
    }
    Func_0200124c(msg);
    Func_0200121c(17, 0);
    Func_0200124e(17, 0, 2);
    Func_02001280(17, 0, 10);
    *(volatile u16 *)(rec + 6) = p8;
    Func_0200117a(1);
    *(volatile u16 *)(rec + 100) &= 1;
    Func_020011ee();
}
