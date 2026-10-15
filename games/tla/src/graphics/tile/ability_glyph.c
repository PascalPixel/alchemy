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

struct BattleActionGlyph {
  u8 unk00[4];
  u16 icon;
};

struct BattleActionGlyph *BattleActGet(s32 action);

void Ability_LoadGlyph(s32 action, s32 with_base, s32 *slot, s32 *tile,
                       s32 reuse) {
  UiIcon_BuildAbilityIconTiles(BattleActGet(action)->icon, with_base, slot,
                               tile, reuse);
}
