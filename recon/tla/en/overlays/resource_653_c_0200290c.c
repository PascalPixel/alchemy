/* Near miss, differ 2 of 117 (one reordered pair): the native loads the
 * pooled blend value 0xc08 before it builds the address of REG_BLDALPHA;
 * the draft builds the first half of the address first. One-pass blocks
 * around the two stores, a value temporary and struct or plain pointer
 * forms were tried. Func_020053a0 and Scheduler_AddOrUpdateCallback are
 * value-returning here (their first argument is loaded last). */
#include "../../../../games/tla/src/field/overlay_653/local.h"
#include "io_reg.h"

s32 Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 priority);
void Func_02005398(void);
s32 Func_020053a0(s32 a, s32 b, s32 c);
void Func_020053b8(const void *data);
void Func_02000b78(s32 id);
void Func_02000bac(s32 id);
void Func_02000c74(void);
void Func_02001634(void);
void Func_02001804(void);
void Func_02001b50(void);
void Func_02003684(void);
void Func_020037e0(void);
void Func_02003ae4(s32 id, s32 base);
extern s32 Data_020062a0;
extern s32 Data_020062a4;
extern u16 Data_020062a8;
extern u16 Data_020062aa;

void Func_0200290c(void)
{
    Func_02005398();
    Func_020053a0(0, 8, 9);
    Func_020053a0(1, 10, 11);
    Object_SetModeById(8, 4);
    Object_SetModeById(9, 4);
    Object_SetModeById(10, 4);
    Object_SetModeById(11, 4);
    Func_020053b8(Data_020054dc);
    REG_BLDALPHA = 0xc08;
    REG_BLDCNT = 0x3f10;
    Func_02005388(245);
    Scheduler_AddOrUpdateCallback(Func_02000c74, 0x480);
    Func_02005230(13, 0, 0);
    Func_02005230(14, 0, 0);
    Func_02005230(15, 0, 0);
    Func_02005230(16, 0, 0);
    Func_02000b78(13);
    Func_02000b78(14);
    Func_02000bac(15);
    Func_02000bac(16);
    Data_020062a0 = 0;
    Data_020062a4 = 0;
    Data_020062a8 = 0;
    Data_020062aa = 0;
    if (GameFlag_Test(0x109) == 0) {
        if (gPartyState.entrance == 5)
            Func_02003684();
        else if (gPartyState.entrance == 6)
            Func_020037e0();
    }
    if (GameFlag_Test(0x301))
        Scheduler_AddOrUpdateCallback(Func_02001634, 0x480);
    else if (GameFlag_Test(0x300))
        Scheduler_AddOrUpdateCallback(Func_02001804, 0x480);
    Scheduler_AddOrUpdateCallback(Func_02001b50, 0x480);
    Func_02005338(0);
}

