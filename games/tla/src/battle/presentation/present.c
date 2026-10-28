/// @file
/// Battle presentation: toggle translucency on unit graphics.

#include "types.h"
#include "battle_party.h"
#include "battle_display.h"
#include "battle_session.h"
#include "motobj.h"

struct ActorSpritePart {
  u8 unk00[4];
  u16 y : 8;
  u16 affine : 2;
  u16 objectMode : 2;
  u16 mosaic : 1;
  u16 fullColor : 1;
  u16 shape : 2;
  u8 unk06[0x16];
};

struct ActorSpriteRecord {
  struct ActorSpritePart part[2];
};

/// Sets the OBJ mode of both sprite parts of each of the actor's records.
void BattleHighlight_SetSpriteMode(s32 actorId, s32 mode) {
  struct BattleObj *slot = BattleObject_Get(actorId);
  struct MotionObject *object;
  struct ActorSpriteRecord *record;
  struct ActorSpriteRecord **records;
  s32 i;

  if (slot == 0)
    return;
  object = slot->object;
  if (object == 0)
    return;
  switch (object->recordStorageKind & 15) {
  case 1: {
    record = object->records;
    record->part[0].objectMode = mode;
    /* FAKEMATCH: the barrier stores the first part before the second part's
     * address, here and in the loop, as native; plainly, 4 instructions differ
     * in tla-en. */
    __asm__ volatile("");
    record->part[1].objectMode = mode;
    break;
  }
  case 2: {
    records = object->records;
    i = 0;
    do {
      record = *records++;
      if (record == 0)
        break;
      record->part[0].objectMode = mode;
      /* FAKEMATCH: the barrier of the store above. */
      __asm__ volatile("");
      record->part[1].objectMode = mode;
      i++;
    } while (i <= 3);
    break;
  }
  }
}

/// Makes every unit's graphics but except's translucent (OBJ mode 1).
/// except: unit ID to leave unchanged.
void BattleHighlight_SetOtherSpriteModes(s32 except) {
  s16 list[14];
  s32 cnt = BattleParty_ListActorIds(BATTLE_SIDE_BOTH, list);
  s32 i;

  for (i = 0; i < cnt; i++) {
    if (list[i] != except)
      BattleHighlight_SetSpriteMode(list[i], 1);
  }
}

/// Restores normal rendering (OBJ mode 0) for all units.
void BattleHighlight_ClearAllSpriteModes(void) {
  s16 list[14];
  s32 cnt = BattleParty_ListActorIds(BATTLE_SIDE_BOTH, list);
  s32 i;

  for (i = 0; i < cnt; i++)
    BattleHighlight_SetSpriteMode(list[i], 0);
}
