// Draft, differ 163 of 211: brings a unit back from a saved battle object: its sprite spins up
// and fades out over 16 frames, its records are released and the saved 44
// bytes copied back over its battle object, then the party (or the one unit)
// is rebuilt and the sprite fades back in over 16 frames at its target
// position. No ☀️ twin found. Left: register allocation throughout; the
// native keeps unitId on the stack (read back as a halfword for the one-unit
// list), the id list in r9 with a copy in r11 for the first party loop, which
// indexes it by i * 2 (an inlined helper taking the list would give that
// copy; not tried), and the object in r10.
#include "types.h"
#include "battle.h"
#include "battle_session.h"
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

void WaitFrames(s32 frames);
s32 BattleParty_ListActorIds(s32 side, u16 *ids);
s32 Func_0811a4e0(s32 unitId);
void ResourceSlot_LoadFar(s32 slot, s32 a, s32 b, s32 c);

void Func_0811e830(struct SavedBattleObj *saved) {
  u16 ids[14];
  u16 *list;
  struct MotionObject *object;
  s32 unitId;
  s32 count;
  s32 frame;
  s32 i;

  unitId = saved->unitId;
  object = BattleObject_Get(unitId)->object;
  QueueIoWriteDelay2(0x04000050, 0x3f40);
  BattleHighlight_SetSpriteMode(unitId, 1);
  for (frame = 0; frame != 16; frame++) {
    s32 fade;

    object->y = frame << 18;
    object->angle += 0x2000;
    fade = frame * 2 - 16;
    if (fade < 0)
      fade = 0;
    if (fade <= 15)
      *(volatile u16 *)0x04000052 = (fade << 8) | (16 - fade);
    WaitFrames(1);
  }
  ReleaseBattleObjectRecords(unitId);
  Iwram_CopyWords(BattleObject_Get(saved->unitId), saved, 44);
  if (saved->reactivate) {
    BattleObject_MarkForRelease(unitId);
    BattleMotion_DestroyAllSlotObjects();
  }
  ResourceSlot_LoadFar(7, 0, 0, 0);
  if (saved->single) {
    list = ids;
    list[0] = unitId;
    list[1] = 255;
    Func_0811b75c((s16 *)list, 1, 0);
    BattleHighlight_SetSpriteMode(unitId, 1);
  } else {
    list = ids;
    count = BattleParty_ListActorIds((saved->unitId != 0) + 1, list);
    for (i = 0; i < count; i++) {
      s32 resource = Func_0811a4e0(list[i]);

      if (resource)
        BattleObject_Get(list[i])->resource = resource;
    }
    Func_0811b75c((s16 *)list, 1, 0);
    for (i = 0; i < count; i++)
      BattleHighlight_SetSpriteMode(list[i], 1);
  }
  ObjMotRst(unitId);
  object->x = object->targetX;
  object->y = object->targetY;
  object->z = object->targetZ;
  for (frame = 0; frame <= 15; frame++) {
    object->y = 0;
    *(volatile u16 *)0x04000052 = ((16 - frame) << 8) | frame;
    WaitFrames(1);
  }
  if (saved->single) {
    BattleHighlight_SetSpriteMode(unitId, 0);
  } else {
    count = BattleParty_ListActorIds((saved->unitId != 0) + 1, list);
    for (i = 0; i < count; i++)
      BattleHighlight_SetSpriteMode(list[i], 0);
  }
}
