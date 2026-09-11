#include "types.h"

#define SceneState_ApplyThreeRects Func_02000398
#define SceneState_ConfigureRegion26_30AndClearActor18Mode Func_020005a0
#define SceneActor_SetupSlotTwenty Func_02000640
#define SceneActor_InitSlots10To15AndStartTask Func_02000c08
#define SceneActor_SetupActors11To14AndInstallTask Func_02000c50
void Func_02001476(s32, s32, s32, s32, s32, s32);
void Func_0200148a(s32, s32, s32, s32, s32, s32);
void Func_020014a4(s32, s32, s32, s32, s32, s32);
u8 *Func_020016d8(s32);
void Func_0200168e(s32, s32, s32, s32, s32, s32);
u8 *Func_020016f8(s32);
void Func_020016e4(s32);
u8 *Func_02001778(s32);
void Func_0200172e(s32, s32, s32, s32, s32, s32);
void Func_0200174a(u8 *, s32);
u8 *Func_020017a0(s32);
void Func_0200178e(s32);
s32 *Func_02001d42();
void Func_02001cf8();
void Func_02001cb6();
s32 *Func_02001d8a();
void Func_02001d40();
void Func_02001dc8();
void Func_02001d08();

void SceneState_ApplyThreeRects(void)
{
    s32 strip = 16;

    {
        s32 fifth = 80;
        s32 sixth = 50;

        Func_02001476(87, 50, 2, 4, fifth, sixth);
    }
    Func_0200148a(23, 52, 1, 2, strip, 52);
    Func_020014a4(16, 52, 1, 1, strip, 53);
}

void SceneState_ConfigureRegion26_30AndClearActor18Mode(void)
{
    u8 *record = Func_020016d8(18);
    /* The two stack arguments each need their own local: the reference builds
     * both into separate registers before storing either, and a literal pair
     * lets the compiler reuse one register for both. */
    s32 a = 24;
    s32 b = 34;

    Func_0200168e(26, 30, 1, 1, a, b);

    if (record != 0) {
        /* The record is reloaded with the same selector before this store. */
        Func_020016f8(18)[85] = 0;
        record[35] = 1;
    }

    Func_020016e4(0x202);
}

void SceneActor_SetupSlotTwenty(void)
{
    u8 *rec = Func_02001778(20);
    /* The two stack arguments each need their own local: the reference builds
     * both into separate registers before storing either, and a literal pair
     * lets the compiler reuse one register for both. */
    s32 fifth = 26;
    s32 sixth = 34;

    Func_0200172e(26, 30, 1, 1, fifth, sixth);

    if (rec != 0) {
        Func_0200174a(rec, 0);
        /* The rec is reloaded with the same selector before this store. */
        Func_020017a0(20)[85] = 0;
        rec[35] = 1;
    }

    Func_0200178e(0x204);
}

void SceneActor_InitSlots10To15AndStartTask(void)
{
    s32 selector = 10;
    s32 remaining = 5;

    do {
        s32 *record;

        Func_02001cf8(Func_02001d42(selector), 0);
        record = Func_02001d4e(selector);
        record[17] = 0x1999;
        record[18] = 0;
        remaining--;
        record[3] = 0x00ff0000;
        selector++;
    } while (remaining >= 0);

    {
        s32 rank = 0xc80;

        Func_02001cb6(0x02008aa1, rank);
    }
}

void SceneActor_SetupActors11To14AndInstallTask(void)
{
    s32 no = 11;
    s32 i = 0;

    do {
        s32 *rec;

        Func_02001d40(Func_02001d8a(no), 0);
        rec = Func_02001d96(no);
        rec[17] = 0x1999;
        rec[18] = 0;
        rec[3] = 0x00ff0000;
        Func_02001dc8(i + 11, 1);
        i++;
        no++;
    } while (i <= 3);

    {
        s32 rate = 0xc80;

        Func_02001d08(0x020089c1, rate);
    }
}
