#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "mem.h"
#include "battle_work.h"
#include "battle_random.h"
#include "battle_system.h"
#include "io_reg.h"
#include "ram_buffer.h"
#include "serial_runtime.h"
#include "iwram_call.h"

static inline void BattleAction_Swap(struct BattleActionRecord *a, struct BattleActionRecord *b)
{
    struct BattleActionRecord temporary;

    /* FAKEMATCH: the swap as an inline takes the temporary's address again at each copy (mov r0, sp), as the native does; written in the loop's body the address is kept in a register and the list pointer spills (differ 45). */
    Iwram_CopyWords(&temporary, a, sizeof(struct BattleActionRecord));
    Iwram_CopyWords(a, b, sizeof(struct BattleActionRecord));
    Iwram_CopyWords(b, &temporary, sizeof(struct BattleActionRecord));
}

// 行動を優先度の高い順に並べる
void BattleQueue_SortByPriority(struct BattleActionRecord *entries, s32 count)
{
    s32 i;
    s32 j;
    s32 swapped;

    for (i = 0; i < count; i++)
    {
        struct BattleActionRecord *entry = &entries[i];

        if (entry->command == 5)
        {
            struct BattleAct *action;

            UnitGet(entry->unitId);
            action = BattleActGet(Djinn_GetDefinitionHeaderFar(
                (entry->parameter) >> 8 & 15, entry->parameter & 0xff));
            if (action->effect == 72 || action->effect == 76 || action->effect == 79
                || action->effect == 74 || action->effect == 46 || action->effect == 47
                || action->effect == 88 || action->effect == 53)
            {
                u32 priority = entry->priority;

                /* FAKEMATCH: the barrier keeps the priority's load ahead of its bonus; added plainly the bonus is started first (differ 2). */
                __asm__("");
                entry->priority = priority + 10000;
            }
        }
    }

    do
    {
        swapped = 0;
        for (j = count - 1; j > 0; j--)
        {
            if ((s16)entries[j].priority > (s16)entries[j - 1].priority)
            {
                BattleAction_Swap(&entries[j], &entries[j - 1]);
                swapped++;
            }
        }
    } while (swapped != 0);
}
