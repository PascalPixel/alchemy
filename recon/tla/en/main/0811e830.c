// Matches: differ 0 (tla-en, 2026-11-01)
// Brings a unit back from a saved battle object: its sprite spins up and
// fades out over 16 frames, its records are released and the saved 44 bytes
// copied back over its battle object, then the party (or the one unit) is
// rebuilt and the sprite fades back in over 16 frames at its target position.
// No ☀️ twin found. What closed it: the id list is the stack array itself, not
// a pointer local (the copy in r11 and the offset-indexed first loop are the
// compiler's own); the last party list count is its own local; and one counter
// i serves every loop, which orders allocation. No devices.
#include "types.h"
#include "battle.h"
#include "battle_action.h"
#include "battle_display.h"
#include "battle_obj.h"
#include "motobj.h"
#include "io_write_queue.h"
#include "iwram_call.h"

struct SavedBattleObj {
  u8 object[44];
  u8 unk2c[4];
  u8 unitId;
  u8 single;
  u8 reactivate;
};

void wait(s32 frames);
s32 BattleParty_ListActorIds(s32 side, u16 *ids);
s32 Func_0811a4e0(s32 unitId);
void ResourceSlot_LoadFar(s32 slot, s32 a, s32 b, s32 c);

void Func_0811e830(struct SavedBattleObj *saved) {
  u16 ids[14];
  struct MotionObject *object;
  s32 unitId;
  s32 count;
  s32 i;

  unitId = saved->unitId;
  object = BattleObject_Get(unitId)->object;
  QueueIoWriteDelay2(0x04000050, 0x3f40);
  BattleHighlight_SetSpriteMode(unitId, 1);
  for (i = 0; i != 16; i++) {
    s32 fade;

    object->y = i << 18;
    object->angle += 0x2000;
    fade = i * 2 - 16;
    if (fade < 0)
      fade = 0;
    if (fade <= 15)
      *(volatile u16 *)0x04000052 = (fade << 8) | (16 - fade);
    wait(1);
  }
  ReleaseBattleObjectRecords(unitId);
  Iwram_CopyWords(BattleObject_Get(saved->unitId), saved, 44);
  if (saved->reactivate) {
    BattleObject_MarkForRelease(unitId);
    BattleMotion_DestroyAllSlotObjects();
  }
  ResourceSlot_LoadFar(7, 0, 0, 0);
  if (saved->single) {
    ids[0] = unitId;
    ids[1] = 255;
    Func_0811b75c((s16 *)ids, 1, 0);
    BattleHighlight_SetSpriteMode(unitId, 1);
  } else {
    count = BattleParty_ListActorIds((saved->unitId != 0) + 1, ids);
    for (i = 0; i < count; i++) {
      s32 resource = Func_0811a4e0(ids[i]);

      if (resource)
        BattleObject_Get(ids[i])->resource = resource;
    }
    Func_0811b75c((s16 *)ids, 1, 0);
    for (i = 0; i < count; i++)
      BattleHighlight_SetSpriteMode(ids[i], 1);
  }
  ObjMotRst(unitId);
  object->x = object->targetX;
  object->y = object->targetY;
  object->z = object->targetZ;
  for (i = 0; i <= 15; i++) {
    object->y = 0;
    *(volatile u16 *)0x04000052 = ((16 - i) << 8) | i;
    wait(1);
  }
  if (saved->single) {
    BattleHighlight_SetSpriteMode(unitId, 0);
  } else {
    s32 total = BattleParty_ListActorIds((saved->unitId != 0) + 1, ids);

    for (i = 0; i < total; i++)
      BattleHighlight_SetSpriteMode(ids[i], 0);
  }
}
