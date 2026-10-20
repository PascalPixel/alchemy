// Draft, differ 15 of 26 (the nested function's 26 instructions all match;
// the 15 are the stub parent's, alchemy-devtools diff DRAFT 08122cd0):
// BattleSprite_ClearTiles, the function nested in BattleEvent_Playback
// (08122d10), as in ☀️'s (games/tbs/src/battle/battle.c): clears a
// record's sprite tiles in OBJ VRAM through the resident IwramClearWords.
// The parent here is a stub that only calls it; the nested function uses
// nothing of its parent's frame, so its code does not depend on the rest.
// It adopts with BattleEvent_Playback.
#include "types.h"
#include "iwram_call.h"
#include "vram_tab.h"

#define OBJ_VRAM 0x06010000

struct AnimationRecord {
  u8 unk00[0x10];
  u8 slot;
  u8 unk11[3];
  u8 width;
  u8 height;
};

void BattleEvent_Playback(void) {
  struct AnimationRecord *records[4];

  void BattleSprite_ClearTiles(struct AnimationRecord * record, s32 unused) {
    u8 *tiles = (u8 *)OBJ_VRAM + gResourceTableEntries[record->slot].offset;

    Iwram_ClearWords(tiles, record->width * record->height);
  }

  BattleSprite_ClearTiles(records[0], 0);
}
