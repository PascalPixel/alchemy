#include "types.h"
#include "battle.h"
#include "battle_display.h"
#include "battle_side.h"
#include "battle_work.h"
#include "task.h"
#include "io_reg.h"
#include "io_write_queue.h"
#include "io_write.h"
#include "ram_buffer.h"
#include "system.h"

void BattleHighlight_ClearAllSpriteModes(void);
void Graphics_AdvancePaletteCycle(void);

/// Mode 0 clears every actor's OBJ mode and stops the palette cycle. Any other
/// mode starts the cycle: every actor takes mode & 1, and the actors listed
/// (up to 14, ended by 0xff) take the other.
void BattleHighlight_SetModes(u16 *actors, s32 mode) {
  s16 activeActors[14];
  struct BattleWork *battle = Ram_HeapSlots->battleWork;
  u32 count;
  u32 i;

  if (mode == 0) {
    Task_Remove(Graphics_AdvancePaletteCycle);
    Io_Write16(mode, &REG_BLDY);
    BattleHighlight_ClearAllSpriteModes();
    wait(1);
    QueueIoWriteDelay2((u32)&REG_BLDCNT, 0);
  }
  if (battle != NULL && mode != 0) {
    battle->highlightMode = mode;
    battle->highlightStep = 0;
    REG_BLDY = 0;
    Io_Write16(16, &REG_BLDALPHA);

    count = BattleParty_ListActorIds(BATTLE_SIDE_BOTH, (u16 *)activeActors);
    for (i = 0; i < count; i++)
      BattleHighlight_SetSpriteMode(activeActors[i], mode & 1);

    if (actors != NULL) {
      u32 actor = *actors;

      i = 0;
      actors++;
      if (actor != 0xff) {
        do {
          BattleHighlight_SetSpriteMode(actor, (mode & 1) ^ 1);
          i++;
          if (i > 13)
            break;
          actor = *actors++;
        } while (actor != 0xff);
      }
    }
    wait(1);
    QueueIoWriteDelay2((u32)&REG_BLDCNT, 0);
    Task_Add(Graphics_AdvancePaletteCycle, TASK_PASS_FRAME);
  }
}
