#include "types.h"
#include "scene.h"

extern u8 *gBattleWork;
void WaitFrames(s32);

void BattlePres_SetActorModes(u16 *actors, s32 mode)
{
    s16 active_actors[14];
    u8 *battle = gBattleWork;
    u32 count;
    u32 i;
    volatile u16 *blend_y;

    if (mode == 0) {
        ScheduleCallback(0x080c1085);
        *(volatile u16 *)0x04000054 = mode;
        Actor_Run();
        WaitFrames(1);
        Actor_Apply(0x04000050, 0);
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

        count = Actor_Apply2(3, active_actors);
        for (i = 0; i < count; i++)
            Actor_Apply3(active_actors[i], mode & 1);

        if (actors != 0) {
            u32 actor = *actors;
            i = 0;
            actors++;
            if (actor != 0xff) {
                do {
                    Actor_Apply3(actor, (mode & 1) ^ 1);
                    i++;
                    if (i > 13)
                        break;
                    actor = *actors++;
                } while (actor != 0xff);
            }
        }
        WaitFrames(1);
        Actor_Apply(0x04000050, 0);
        ScheduleCallbackAfterFrames(0x080c1085, 0x480);
    }
}
