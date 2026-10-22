#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "system.h"
#include "motobj.h"
#include "owner_state.h"
#include "battle_work.h"
#include "field_event.h"

void BattleMotion_FlashHit(s32 id) {
  u16 selection[2];
  u16 *selected;
  s32 target;
  s32 remaining;

  Unit_Get(id);
  Object_SetMode(BattleObject_Get(id)->object, 5);

  selected = selection;
  remaining = 1;
  do {
    target = 0xff;
    selected[1] = target;
    selected[0] = id;
    BattleLayout_HighlightPartyPanelsFar(selected);
    BattleMotion_SetRecordChildValues(BattleObject_Get(id)->object, 7);
    WaitFrames(2);

    selected[0] = id;
    BattleLayout_HighlightPartyPanelsFar(selected);
    BattleMotion_SetRecordChildValues(BattleObject_Get(id)->object,
                                      BattleMotion_GetSlotPalette(id));
    WaitFrames(2);
    remaining--;
  } while (remaining >= 0);

  PartyStatus_DrawFar(Ram_HeapSlots->battleWork->partyStatus);
}
