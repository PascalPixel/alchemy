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

void Text_DrawCharacterAtOffsetFar();
void Icon_CreateStatChangeArrow();

void Text_DrawStatComparison(const struct BattleUnit *alt,
                               const struct BattleUnit *base, s32 work) {
  Text_DrawCharacterAtOffsetFar((s32)MsgPanelStatLabel, work, 0, 32);
  Text_DrawNumberAtOffsetFar(base->atk, 3, work, 16, 40);
  if (alt->atk != base->atk) {
    Text_DrawNumberAtOffsetFar(alt->atk, 3, work, 64, 40);
    if (alt->atk > base->atk) {
      Icon_CreateStatChangeArrow(work, 44, 36, 0);
    } else {
      Icon_CreateStatChangeArrow(work, 44, 36, 1);
    }
  }
  Text_DrawCharacterAtOffsetFar((s32)MsgPanelDefenseLabel, work, 0, 48);
  Text_DrawNumberAtOffsetFar(base->def, 3, work, 16, 56);
  if (alt->def != base->def) {
    Text_DrawNumberAtOffsetFar(alt->def, 3, work, 64, 56);
    if (alt->def > base->def) {
      Icon_CreateStatChangeArrow(work, 44, 52, 0);
    } else {
      Icon_CreateStatChangeArrow(work, 44, 52, 1);
    }
  }
  Text_DrawCharacterAtOffsetFar((s32)MsgAgilityLabel, work, 0, 64);
  Text_DrawNumberAtOffsetFar(base->agi, 3, work, 16, 72);
  if (alt->agi != base->agi) {
    Text_DrawNumberAtOffsetFar(alt->agi, 3, work, 64, 72);
    if (alt->agi > base->agi) {
      Icon_CreateStatChangeArrow(work, 44, 68, 0);
    } else {
      Icon_CreateStatChangeArrow(work, 44, 68, 1);
    }
  }
}

s32 Icon_CreateWithResourceVariant(struct RenderInput *window, s32 x, s32 y) {
  s32 slot;
  s32 icon;

  icon = 0;
  slot = Resource_FindFreeEntry();
  if (slot != 0) {
    VramBlock_LoadResourceFar(slot, 0x80, (s32)&ResourceId_UiIconTiles);
    icon = (s32)WindowSprite_CreateFar(slot, OUTPUT_SIZE_16X16, window, x, y);
  }
  return icon;
}
