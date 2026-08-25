#include "types.h"

extern u8 *Data_03001e74;
void WaitFrames(s32);
void Func_0800387c(u32, u32);
s32 Func_080041d8(u32, s32);
void Func_08004278(u32);
u32 Func_080b6c08(s32, s16 *);
void Func_080c0f98(s32, s32);
void Func_080c1054(void);

#define set_actor_modes Func_080c10e8

void set_actor_modes(u16 *actors, s32 mode)
{
    s16 active_actors[14];
    u8 *battle = Data_03001e74;
    u32 count;
    u32 i;
    volatile u16 *blend_y;

    if (mode == 0) {
        Func_08004278(0x080c1085);
        *(volatile u16 *)0x04000054 = mode;
        Func_080c1054();
        WaitFrames(1);
        Func_0800387c(0x04000050, 0);
    }
    if (battle != 0 && mode != 0) {
        u32 zero = 0;
        u32 sixteen;

        *(u16 *)(battle + 0x650) = mode;
        *(u16 *)(battle + 0x64e) = zero;
        blend_y = (volatile u16 *)0x04000054;
        *blend_y = zero;
        sixteen = 16;
        /* Preserve the volatile register-store scheduling used by agbcc. */
        do {
            blend_y[-1] = sixteen;
        } while (0);

        count = Func_080b6c08(3, active_actors);
        for (i = 0; i < count; i++)
            Func_080c0f98(active_actors[i], mode & 1);

        if (actors != 0) {
            u32 actor = *actors;
            i = 0;
            actors++;
            if (actor != 0xff) {
                do {
                    Func_080c0f98(actor, (mode & 1) ^ 1);
                    i++;
                    if (i > 13)
                        break;
                    actor = *actors++;
                } while (actor != 0xff);
            }
        }
        WaitFrames(1);
        Func_0800387c(0x04000050, 0);
        Func_080041d8(0x080c1085, 0x480);
    }
}
