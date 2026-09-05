#include "types.h"

s32 *Func_02001d8a();
void Func_02001d40();
void Func_02001dc8();
void Func_02001d08();

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
