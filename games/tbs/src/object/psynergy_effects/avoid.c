// Part of psynergy_effects.c, compiled only through it.
// Avoid.

#include "../psynergy_effects_internal.h"

#define ITEM_SACRED_FEATHER 236 // Sacred Feather

// End of the effect: switches the caster's display flags between 3 and 1 on
// a 5-frame cycle.
void FieldFx_BlinkAvoidCaster(void) {
  struct ArcEffectObject *caster;
  struct AnimationObject *rec;
  struct AnimationEntry *entry;
  s32 phase;

  caster = ObjectTable_Get(gGameState.selectedActor);
  rec = caster->records;
  entry = rec->entries[0];
  phase = gFrameCount % 5;
  if (phase == 0) {
    rec->dirty = 1;
    rec->flags = 3;
  } else if (phase == 2) {
    entry->param = 0; // restore its colours
    rec->dirty = 1;
    rec->flags = 1;
  }
}

// The caster during the effect: sheds a pair of falling arcs every 5 frames
// (7 on the world map), and spins in phase 1.
void BattleFx_UpdatePairedArcSpawner(union FieldObject *fieldObject) {
  struct ArcEffectObject *caster = (struct ArcEffectObject *)fieldObject;
  s16 phase;
  s32 elapsedFrames;

  phase = caster->phase;
  elapsedFrames = caster->step++;

  if (gGameState.map == (s32)&SceneId_Clear) {
    if (elapsedFrames % 7 == 0)
      BattleFx_SpawnDescendingArcParticles(caster);
  } else if (elapsedFrames % 5 == 0) {
    BattleFx_SpawnDescendingArcParticles(caster);
  }

  if (phase == 1)
    caster->angle += 0xc00;
}

// Avoid (or the Sacred Feather) has worn off: "Avoid's effects wore off." (or
// "Sacred Feather's ...").
void FieldEvent_ShowStatusMessage(void) {
  gGameState.avoidDistance = 0;
  if (gGameState.avoidByItem == 0) {
    UiWork_PushValueSlotFar(ABILITY_AVOID, 4);
    UiText_ShowPositionedMessageAndWaitFar(MsgAbilityWoreOff, 1);
    return;
  }
  UiWork_PushValueSlotFar(ITEM_SACRED_FEATHER, 2);
  UiText_ShowPositionedMessageAndWaitFar(MsgItemWoreOff, 1);
}

// Abilities: Avoid (field kind 16), and the Sacred Feather.
//
// The caster spins shedding falling arcs and flashes 20 times, then blinks for
// 55 frames; encounters are kept away (avoidDistance 0x9600000) until it wears
// off, and the game says "Monsters won't attack so often now.". Flag 0x145
// marks it as cast from the Sacred Feather.
void FieldFx_RunAvoid(void) {
  struct BattleFxScene *scene;
  struct ArcEffectObject *object;
  struct AnimationObject *group;
  struct AnimationEntry *entry;
  u32 angle;
  s32 slot;
  s32 count;

  scene = gEffectWork;
  object = scene->mainObject;
  group = object->records;
  entry = group->entries[0];
  angle = object->angle;
  slot = Resource_FindFreeEntry();
  {
    s16 *tileSlot = &scene->arcTileSlot;
    s32 zero = 0;

    *tileSlot = slot;
    VramCache_Load((s16)slot, 0x100, gBattleFx_ArcSparkTiles);
    gGameState.avoidDistance = 0x09600000;
    gGameState.avoidByItem = GameFlag_TestFar(0x145);
    Animation_ApplyChildValuesFar(object, zero);
    object->callback = BattleFx_UpdatePairedArcSpawner;
    object->phase = zero;
    object->step = zero;
  }
  AudioCommand_PlayFar(SE_140);
  WaitFrames(15);
  object->phase = 1;
  WaitFrames(10);
  for (count = 0; count < 20; count++) {
    entry->param = SPRITE_COLOR_WHITE;
    group->dirty = 1;
    WaitFrames(2);
    group->dirty = 1;
    entry->param = 0;
    group->flags = 1;
    WaitFrames(3);
  }
  object->callback = NULL;
  object->angle = angle;
  Scheduler_AddOrUpdateCallback(FieldFx_BlinkAvoidCaster, 0xc80);
  WaitFrames(15);
  AudioCommand_PlayFar(SE_174);
  WaitFrames(55);
  Scheduler_RemoveCallback(FieldFx_BlinkAvoidCaster);
  if (gGameState.cloaked != 0)
    ObjectDispatch_SetSingleChildFlagsFar(object, 2);
  else
    ObjectDispatch_SetSingleChildFlagsFar(object, 1);
  Animation_ApplyChildValuesFar(object, 0);
  Resource_ResetEntry(scene->arcTileSlot);
  UiText_ShowPositionedMessageAndWaitFar(MsgMonstersAttackLess, 1);
}

#undef ITEM_SACRED_FEATHER
