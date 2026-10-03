#include "TYPES.H"
#include "BATTLE_PARTY.H"
#include "SCENE.H"
#include "SYSTEM.H"

void Graphics_AdvancePaletteCycle(void);
void Scheduler_RemoveCallback(u32);
void BattlePres_ClearAllActorRecordModes(void);
void QueueIoWriteDelay2(u32, u32);
void BattlePres_SetActorRecordMode(s32, s32);
s32 Scheduler_AddOrUpdateCallback(u32, s32);

extern u8 *gBattleWork;

void BattlePres_SetActorModes(u16 *actors, s32 mode)
{
    s16 active_actors[14];
    u8 *battle = gBattleWork;
    u32 count;
    u32 i;
    volatile u16 *blend_y;

    if (mode == 0) {
        Scheduler_RemoveCallback((s32)Graphics_AdvancePaletteCycle);
        *(volatile u16 *)0x04000054 = mode;
        BattlePres_ClearAllActorRecordModes();
        WaitFrames(1);
        QueueIoWriteDelay2(0x04000050, 0);
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

        count = BattleParty_ListActorIds(3, (u16 *)active_actors);
        for (i = 0; i < count; i++)
            BattlePres_SetActorRecordMode(active_actors[i], mode & 1);

        if (actors != 0) {
            u32 actor = *actors;
            i = 0;
            actors++;
            if (actor != 0xff) {
                do {
                    BattlePres_SetActorRecordMode(actor, (mode & 1) ^ 1);
                    i++;
                    if (i > 13)
                        break;
                    actor = *actors++;
                } while (actor != 0xff);
            }
        }
        WaitFrames(1);
        QueueIoWriteDelay2(0x04000050, 0);
        Scheduler_AddOrUpdateCallback((s32)Graphics_AdvancePaletteCycle, 0x480);
    }
}
