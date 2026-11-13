/// @file
/// Avoid.
// Part of radial.c.

#include "../radial_internal.h"
#include "flag_call.h"
#include "gamepak/far_call/window.h"
#include "module/field/field.h"
#include "module/field/radial.h"
#include "text/msg_ids.h"

/* VIEW(launch): tag struct ArcEffectObject as struct ArcEffectObject__launch; field_psynergy_work.h has another definition. */
struct ArcEffectObject__launch {
  u8 unk00[6];
  u16 angle;
  u8 unk08[0x48];
  struct ArcGroup *records;
  u8 unk54[0x10];
  s16 phase;
  u16 step;
  u8 unk68[4];
  void (*callback)(struct ArcEffectObject__launch *);
};

/// The caster during Avoid (BattleEffect_RunEffect16): sheds a pair of falling
/// arcs every 5 frames (7 in scene Ov64a), and spins in phase 1.
// As ☀️'s function of the same name.
void BattleEffect_UpdatePairedArcSpawner(struct ArcEffectObject__launch *caster) {
  s16 phase;
  s32 elapsedFrames;
  struct MainRoutineWork *party;

  phase = caster->phase;
  elapsedFrames = (s16)caster->step++;
  party = &gMainRoutineWork;

  if (party->map == (s32)&SceneId_Ov64a) {
    if (elapsedFrames % 7 == 0)
      Func_080e1024(caster);
  } else if (elapsedFrames % 5 == 0) {
    Func_080e1024(caster);
  }

  if (phase == 1)
    caster->angle += 0xc00;
}

void FieldEvent_ShowStatusMessage(void) {
  gMainRoutineWork.avoidDistance = 0;
  Func_080cb82c();
  if (gMainRoutineWork.avoidByItem == 0) {
    MessageArgument_PushFar(150, 4);
    FieldMessage_ShowFar((s32)&MsgAbilityWoreOff, 1);
  } else {
    MessageArgument_PushFar(236, 2);
    FieldMessage_ShowFar((s32)&MsgItemWoreOff, 1);
  }
  Func_080cb8a4();
}

/// Psynergy effect 16: the user spins, rains arc particles, then flashes.
void BattleEffect_RunEffect16(void) {
  struct ArcScene *scene;
  struct ArcEffectObject__launch *object;
  struct ArcGroup *group;
  struct ArcEntry *entry;
  const u8 *tiles;
  u32 angle;
  s32 flags;
  s32 slot;
  s32 count;
  s32 phase;

  scene = Ram_HeapSlots->effectWork;
  object = scene->mainObject;
  group = object->records;
  entry = group->entries[0];
  angle = object->angle;
  flags = group->flags;
  slot = Resource_FindFreeEntry();
  scene->arcTileSlot = slot;
  // tiles holds the picture's address, loaded between the slot's two shifts;
  // as the address in the call, 2 of 156 instructions differ in tla-en.
  tiles = gData_080ed90c;
  VramCache_Load((s16)slot, 0x100, tiles);
  gMainRoutineWork.avoidDistance = 0x09600000;
  gMainRoutineWork.avoidByItem = GameFlag_Test(0x145);
  Animation_ApplyChildValuesFar__launch(object, 0);
  object->callback = BattleEffect_UpdatePairedArcSpawner;
  object->phase = 0;
  object->step = 0;
  Sound_PlayFar(140);
  wait(15);
  object->phase = 1;
  wait(10);
  for (count = 0; count < 20; count++) {
    entry->param = 7;
    group->dirty = 1;
    wait(2);
    entry->param = 0;
    group->dirty = 1;
    group->flags = 1;
    wait(3);
  }
  object->callback = NULL;
  object->angle = angle;

  // For 70 frames, set and clear flags bit 2 on a five-frame cycle.
  for (count = 0; count < 70; count++) {
    phase = count % 5;
    if (phase == 0) {
      group->dirty = 1;
      group->flags = flags | 2;
    } else if (phase == 2) {
      group->dirty = 1;
      group->flags = flags;
      entry->param = 0;
    }
    if (count == 15)
      Sound_PlayFar(174);
    wait(1);
  }
  {
    /* FAKEMATCH(temporary): the flags restored through a pointer of their
     * own, which sets the param's 0 after the flags' copy; as group->flags =
     * flags, 2 of 156 instructions differ in tla-en. */
    u8 *restore = &group->flags;

    *restore = flags;
  }
  group->dirty = 1;
  entry->param = 0;
  // An empty once-only loop keeps the wait's argument after the param store;
  // without it, 2 of 156 instructions differ in tla-en.
  do {
  } while (0);
  wait(1);

  Resource_ResetEntry(scene->arcTileSlot);
  FieldMessage_ShowFar((s32)&MsgMonstersAttackLess, 1);
}
