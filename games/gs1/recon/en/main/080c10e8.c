#include "types.h"

extern u8 *Data_03001e74;
void WaitFrames(s32);
void Func_0800387c(u32, u32);
s32 Func_080041d8(u32, s32);
void Func_08004278(u32);
u32 Func_080b6c08(s32, s16 *);
void Func_080c0f98(s16, s32);
void Func_080c1054(void);

void Func_080c10e8(u16 *actors, s32 mode)
{
    s16 active_actors[14];
    u8 *battle = Data_03001e74;
    u32 count;
    u32 i;

    if (mode == 0) {
        Func_08004278(0x080c1085);
        *(volatile u16 *)0x04000054 = mode;
        Func_080c1054();
        WaitFrames(1);
        Func_0800387c(0x04000050, 0);
    }
    if (battle != 0 && mode != 0) {
        *(u16 *)(battle + 0x650) = mode;
        *(u16 *)(battle + 0x64e) = 0;
        *(volatile u16 *)0x04000054 = 0;
        *(volatile u16 *)0x04000052 = 16;

        count = Func_080b6c08(3, active_actors);
        for (i = 0; i < count; i++)
            Func_080c0f98(active_actors[i], mode & 1);

        if (actors != 0) {
            u16 actor = *actors++;
            for (i = 0; actor != 0xff && i <= 13; i++) {
                Func_080c0f98((s16)actor, (mode & 1) ^ 1);
                actor = *actors++;
            }
        }
        WaitFrames(1);
        Func_0800387c(0x04000050, 0);
        Func_080041d8(0x080c1085, 0x480);
    }
}
