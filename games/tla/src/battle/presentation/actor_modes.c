#include "types.h"
#include "battle.h"
#include "btldisp.h"
#include "btlpty.h"
#include "btlwork.h"
#include "callback_scheduler.h"
#include "io_reg.h"
#include "io_write_queue.h"
#include "iowrite.h"
#include "ram_buffer.h"
#include "system.h"

void BattlePres_ClearAllActorRecordModes(void);
void Graphics_AdvancePaletteCycle(void);

// Mode 0 clears every actor's OBJ mode and stops the palette cycle. Any other
// mode starts the cycle: every actor takes mode & 1, and the actors listed
// (up to 14, ended by 0xff) take the other.
void BattlePres_SetActorModes(u16 *actors, s32 mode)
{
    s16 activeActors[14];
    struct BtlWork *battle = Ram_HeapSlots->battle_work;
    u32 count;
    u32 i;

    if (mode == 0)
    {
        Scheduler_RemoveCallback(Graphics_AdvancePaletteCycle);
        Io_Write16(mode, &REG_BLDY);
        BattlePres_ClearAllActorRecordModes();
        WaitFrames(1);
        QueueIoWriteDelay2(0x04000050, 0);
    }
    if (battle != NULL && mode != 0)
    {
        battle->highlight_mode = mode;
        battle->highlight_step = 0;
        REG_BLDY = 0;
        Io_Write16(16, &REG_BLDALPHA);

        count = BattleParty_ListActorIds(BATTLE_SIDE_BOTH, (u16 *)activeActors);
        for (i = 0; i < count; i++)
            BattlePres_SetActorRecordMode(activeActors[i], mode & 1);

        if (actors != NULL)
        {
            u32 actor = *actors;

            i = 0;
            actors++;
            if (actor != 0xff)
            {
                do
                {
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
        Scheduler_AddOrUpdateCallback(Graphics_AdvancePaletteCycle, 0xc80);
    }
}
