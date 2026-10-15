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
#include "resource.h"
#include "render_input.h"
#include "battle_unit.h"
#include "text/msg_ids.h"

void UiText_DrawCharacterAtOffsetFar();
void UiText_DrawNumberAtOffsetFar();
void UiIcon_CreateStatChangeArrow();

void UiText_DrawStatComparison(const struct BattleUnit *alt,
                               const struct BattleUnit *base, s32 work) {
  UiText_DrawCharacterAtOffsetFar((s32)MsgPanelStatLabel, work, 0, 32);
  UiText_DrawNumberAtOffsetFar(base->atk, 3, work, 16, 40);
  if (alt->atk != base->atk) {
    UiText_DrawNumberAtOffsetFar(alt->atk, 3, work, 64, 40);
    if (alt->atk > base->atk) {
      UiIcon_CreateStatChangeArrow(work, 44, 36, 0);
    } else {
      UiIcon_CreateStatChangeArrow(work, 44, 36, 1);
    }
  }
  UiText_DrawCharacterAtOffsetFar((s32)MsgPanelDefenseLabel, work, 0, 48);
  UiText_DrawNumberAtOffsetFar(base->def, 3, work, 16, 56);
  if (alt->def != base->def) {
    UiText_DrawNumberAtOffsetFar(alt->def, 3, work, 64, 56);
    if (alt->def > base->def) {
      UiIcon_CreateStatChangeArrow(work, 44, 52, 0);
    } else {
      UiIcon_CreateStatChangeArrow(work, 44, 52, 1);
    }
  }
  UiText_DrawCharacterAtOffsetFar((s32)MsgAgilityLabel, work, 0, 64);
  UiText_DrawNumberAtOffsetFar(base->agi, 3, work, 16, 72);
  if (alt->agi != base->agi) {
    UiText_DrawNumberAtOffsetFar(alt->agi, 3, work, 64, 72);
    if (alt->agi > base->agi) {
      UiIcon_CreateStatChangeArrow(work, 44, 68, 0);
    } else {
      UiIcon_CreateStatChangeArrow(work, 44, 68, 1);
    }
  }
}

s32 UiIcon_CreateWithResourceVariant(struct RenderInput *window, s32 x, s32 y) {
  s32 slot;
  s32 icon;

  icon = 0;
  slot = Resource_FindFreeEntry();
  if (slot != 0) {
    VramBlock_LoadResourceFar(slot, 0x80, (s32)&ResourceId_UiIconTiles);
    icon = (s32)RenderOutput_CreateFar(slot, 0x40000000, window, x, y);
  }
  return icon;
}
