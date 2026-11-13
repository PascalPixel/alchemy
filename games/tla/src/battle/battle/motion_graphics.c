/// @file
/// A unit's motion graphics in battle: setting its records' frames, the flash
/// when it is hit, its removal from the formations and the graphics of a
/// defeated unit, with routines around them not yet understood.
// Part of battle.c.

#include "../battle_internal.h"
#include "fixed_math.h"
#include "sounds.h"
#include "module/battle/battle.h"
#include "gamepak/far_call/battle_effect.h"
#include "gamepak/far_call/field.h"
#include "module/battle/presentation.h"
#include "module/battle/presentation.h"
#include "module/system/system.h"
#include "module/tla.h"
#include "text/msg_ids.h"

/* VIEW(presentation): routine BattleObject_Reset as void BattleObject_Reset();
 * the definition has another declaration; tried as the definition's void
 * BattleObject_Reset(struct BattleObj *actor, s32 keep), which the object
 * already declares, it does not compile in tla-ja:
 * games/tla/src/battle/battle.c:5440: too few arguments to function
 * `BattleObject_Reset'. */
void BattleObject_Reset__presentation() __asm__("BattleObject_Reset");
#define BattleObject_Reset BattleObject_Reset__presentation

/// The Tamer's calls (abilities 600 to 606, Wild Wolf to Grand Golem): for
/// a call that lands, saves the battle object it borrows and brings in the
/// beast's class (Wild Wolf's 117 and so on) to play the attack; returns the
/// save, or NULL for any other ability. BattleMotion_DismissBeast undoes it.
struct SavedBattleObj *
BattleMotion_CallBeast(struct BattlePlan *plan,
                       struct BattlePresentationWork *work) {
  s32 single;
  struct BattleObj *battleObject;
  struct SavedBattleObj *saved;
  u16 ids[14];
  struct MotionObject *object;
  struct MotionObject *actor;
  s32 classId;
  s32 count;
  s32 i;

  if ((u32)(plan->outcome - 15) > 2 || plan->failure != 0)
    return NULL;
  {
    u8 sides = (plan->actorId ^ plan->targetIds[0]) & 0x80;
    single = sides != 0;
  }
  saved = Heap_AllocEwram(sizeof(struct SavedBattleObj));
  saved->single = single;
  saved->reactivate = 0;
  switch (plan->abilityId) {
  case 600:
    classId = 117;
    break;
  case 601:
    classId = 137;
    break;
  case 602:
    classId = 182;
    break;
  case 603:
    classId = 299;
    break;
  case 604:
    classId = 296;
    break;
  case 605:
    classId = 278;
    break;
  case 606:
    classId = 316;
    break;
  case 607:
    classId = 317;
    break;
  case 608:
    classId = 185;
    break;
  case 609:
    classId = 158;
    break;
  case 610:
    classId = 150;
    break;
  case 611:
    classId = 311;
    break;
  case 612:
    classId = 212;
    break;
  case 613:
    classId = 323;
    break;
  case 614:
    classId = 191;
    break;
  case 615:
    classId = 159;
    break;
  case 616:
    classId = 162;
    break;
  case 617:
    classId = 155;
    break;
  case 618:
    classId = 263;
    break;
  case 619:
    classId = 197;
    break;
  case 620:
    classId = 215;
    break;
  case 621:
    classId = 99;
    break;
  case 622:
    classId = 196;
    break;
  case 623:
    classId = 160;
    break;
  case 624:
    classId = 281;
    break;
  case 625:
    classId = 163;
    break;
  case 626:
    classId = 302;
    break;
  case 627:
    classId = 188;
    break;
  case 629:
    classId = 242;
    break;
  case 630:
    classId = 203;
    break;
  case 631:
    classId = 218;
    break;
  default:
    classId = 9;
    break;
  }
  saved->classId = classId;
  if (!single) {
    saved->unitId = (plan->actorId ^ 0x80) & 0x80;
    if (BattleObject_Get(saved->unitId)->object == NULL) {
      saved->reactivate = 1;
      BattleObject_Reset(BattleObject_Get(saved->unitId));
      BattleObject_Get(saved->unitId)->scale = 0x13333;
    }
  } else {
    saved->unitId = plan->actorId;
  }
  work->actor = saved->unitId;
  actor = BattleObject_Get(plan->actorId)->object;
  Actor_SelectAnimationFar(actor, 3);
  Actor_SetAnimationSpeedFar((struct Actor *)actor, 16);
  IoWriteQueue_Store16(0x04000050, 0x3f40);
  Sound_PlayFar(SE_CAST);
  wait(10);
  if (single) {
    BattleHighlight_SetSpriteMode(saved->unitId, 1);
  } else {
    count = BattleParty_ListActorIds((saved->unitId != 0) + 1, ids);
    for (i = 0; i < count; i++)
      BattleHighlight_SetSpriteMode(ids[i], 1);
  }
  for (i = 0; i != 16; i++) {
    REG_BLDALPHA = (i << 8) | (16 - i);
    wait(1);
  }

  battleObject = BattleObject_Get(saved->unitId);
  object = battleObject->object;
  if (single) {
    ReleaseBattleObjectRecords(saved->unitId);
  } else {
    count = BattleParty_ListActorIds((saved->unitId != 0) + 1, ids);
    for (i = 0; i < count; i++) {
      ReleaseBattleObjectRecords(ids[i]);
      BattleObject_ReleaseSpriteSlots(ids[i]);
    }
  }
  Iwram_CopyWordsEntry(saved, BattleObject_Get(saved->unitId), 44);
  BattleObject_Reset(BattleObject_Get(saved->unitId), single);
  BattleObject_Get(saved->unitId)->scale = 0x13333;
  if (single)
    saved->buffer = Heap_AllocEwram(0x8000);
  else
    saved->buffer = Data_02018000;
  i = SpritePictureCache_LoadFar(7, saved->buffer,
                                 BattleSprite_GetSprite(saved->classId),
                                 BattleSprite_GetPalette(saved->classId));
  if (i != 0) {
    battleObject->resource = BattleSprite_GetSprite(saved->classId) + 0x7000;
    if (!single)
      saved->buffer = NULL;
  } else {
    battleObject->resource = BattleSprite_GetSprite(saved->classId);
    if (single)
      Heap_Release(saved->buffer);
    saved->buffer = (u32 *)i;
  }
  ids[0] = saved->unitId;
  ids[1] = 255;
  BattleObject_SpawnObjectsForList((s16 *)ids, 1, 0);
  object->y = 0x280000;
  object->angle = plan->targetIds[0] <= 7 ? 0 : TRIG_TURN / 2;
  BattleHighlight_SetSpriteMode(saved->unitId, 1);
  for (i = 0; i <= 29; i++) {
    object->y = (29 - i) * 3 << 16;
    object->velocityY = 0;
    object->angle += 0x2222;
    if (i <= 15)
      REG_BLDALPHA = ((16 - i) << 8) | i;
    wait(1);
  }
  BattleHighlight_SetSpriteMode(saved->unitId, 0);
  return saved;
}

/// Frees the borrowed buffer and releases the motion graphics slot.
void BattleMotion_ReleaseBuffer(struct Work0811e7dc *work) {
  struct MotionObject *object = BattleObject_Get(work->objectId)->object;

  if (work->buffer != NULL) {
    Animation_SkipToStopFar(GetMotionRecord(object, 0));
    wait(1);
    Heap_Release(work->buffer);
    work->buffer = NULL;
    {
      struct Record0811e7dc *record = GetMotionRecord(object, 0);

      record->flags = 0x20 | record->flags;
    }
    Sprite_RemoveAnimationAtFar(GetMotionRecord(object, 0), 0);
  }
}

/// Sends the called beast away and puts back the battle object
/// BattleMotion_CallBeast saved, then frees the save.
void BattleMotion_DismissBeast(struct SavedBattleObj *saved) {
  u16 ids[14];
  struct MotionObject *object;
  s32 unitId;
  s32 count;
  s32 i;

  unitId = saved->unitId;
  object = BattleObject_Get(unitId)->object;
  IoWriteQueue_Store16(0x04000050, 0x3f40);
  BattleHighlight_SetSpriteMode(unitId, 1);
  for (i = 0; i != 16; i++) {
    s32 fade;

    object->y = i << 18;
    object->angle += TRIG_TURN / 8;
    fade = i * 2 - 16;
    if (fade < 0)
      fade = 0;
    if (fade <= 15)
      *(volatile u16 *)0x04000052 = (fade << 8) | (16 - fade);
    wait(1);
  }
  ReleaseBattleObjectRecords(unitId);
  Iwram_CopyWordsEntry(BattleObject_Get(saved->unitId), saved, 44);
  if (saved->reactivate) {
    BattleObject_MarkForRelease(unitId);
    BattleMotion_DestroyAllSlotObjects();
  }
  SpritePictureCache_LoadFar(7, 0, 0, 0);
  if (saved->single) {
    ids[0] = unitId;
    ids[1] = 255;
    BattleObject_SpawnObjectsForList((s16 *)ids, 1, 0);
    BattleHighlight_SetSpriteMode(unitId, 1);
  } else {
    count = BattleParty_ListActorIds((saved->unitId != 0) + 1, ids);
    for (i = 0; i < count; i++) {
      s32 resource = EnemySprite_LoadIntoSlots(ids[i]);

      if (resource)
        BattleObject_Get(ids[i])->resource = resource;
    }
    BattleObject_SpawnObjectsForList((s16 *)ids, 1, 0);
    for (i = 0; i < count; i++)
      BattleHighlight_SetSpriteMode(ids[i], 1);
  }
  BattleMotion_ReturnToSpot(unitId);
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

s32 BattleStaging_RunAttack(struct BattlePlan *plan, s32 mode) {
  struct BattlePresentationWork work;
  void *object;
  struct AttackAnimation *animation;
  struct BattleUnit *unit;
  s16 criticalPair[3];
  s32 critical;
  struct BattlePresentationTransition *view;
  void *saved;
  s32 angle;
  s32 adjusted;
  s32 facingAngle;
  s32 frames;
  s32 missed;
  s32 index;
  s32 zoomOnCritical;
  u32 actorId;
  struct BattleObj *battleObject;

  view = Ram_HeapSlots->transitionWork;
  actorId = plan->actorId;
  battleObject = BattleObject_Get(actorId);
  {
    s32 x = battleObject->anchorX;
    s32 z;

    battleObject->object->x = x;
    z = battleObject->anchorZ;
    battleObject->object->z = z;
    angle = (u16)ArcTan2(x, z);
    adjusted = angle - TRIG_TURN / 8;
    if (actorId > 7)
      adjusted = angle + TRIG_TURN * 3 / 8;
  }
  adjusted &= 0x7fff;
  facingAngle = (adjusted - TRIG_TURN / 8) / 2 + TRIG_TURN / 8;
  if (view->targetYaw == facingAngle) {
    view->targetYaw = facingAngle;
    wait(1);
  } else {
    view->targetYaw = facingAngle;
    wait(1);
  }

  BattleHighlight_SetModes(0, 0);
  BattleStaging_BuildTargetList(plan, &work);
  if (work.kind == 135)
    PartyStatus_DrawFar(Ram_HeapSlots->battleWork->partyStatus & ~1);
  saved = BattleMotion_CallBeast(plan, &work);
  unit = Unit_Get(work.actor);
  Unit_Get(work.actors[0]);
  critical = plan->targetCritical[0];
  missed = plan->targetMultiplier[0] == 0;

  object = BattleObject_Get(plan->actorId)->object;
  animation = GetMotionRecord(object, 0);
  frames = SpriteAnimation_SumCommandLengthsFar(
      animation->entries[0]->animationId, 2, 1);
  if (work.kind == 10)
    Actor_SelectAnimationFar(object, 2);
  else
    BattleMotion_ApproachTarget(work.actor, work.actors[0], frames,
                                BattleSprite_GetAttackHop(unit->enemyId) << 16);
  if ((mode & 32) && plan->actorId != plan->actorId2 &&
      plan->actorId2 != (work.actors[0] & 0x80)) {
    object = BattleObject_Get(plan->actorId2);

    animation = GetMotionRecord(((struct BattleObj *)object)->object, 0);
    frames = SpriteAnimation_SumCommandLengthsFar(
        animation->entries[0]->animationId, 2, 1);
    BattleMotion_ApproachTarget(
        plan->actorId2, work.actors[0], frames,
        BattleSprite_GetAttackHop(
            ((struct BattleUnit *)Unit_Get(plan->actorId2))->enemyId)
            << 16);
    Actor_SetAnimationSpeedFar(
        (struct Actor *)((struct BattleObj *)object)->object, 16);
  }
  Actor_SetAnimationSpeedFar(
      (struct Actor *)BattleObject_Get(work.actor)->object, 16);

  if ((u16)work.actors[0] <= 7)
    work.side = 1;
  else
    work.side = 0;

  REG_WIN0H = 0xf0;
  REG_WIN0V = 0x1088;
  REG_WIN1H = 0xf0;
  REG_WIN1V = 0x1088;
  REG_WININ = 0x3537;
  REG_WINOUT = 0x3f21;
  REG_DISPCNT |= 0x6000;

  if (missed != 0) {
    wait(10);
    BattleMotion_HopBack(work.actors[0]);
    wait(2);
    wait(4);
    wait(10);
    BattlePlayback_Push(0, plan->targetIds[0]);
    BattlePlayback_Push(4, (u32)&MsgNimblyDodgesBlow);
    BattlePlayback_RunAll();
    BattleMotion_ReturnToSpot(work.actors[0]);
  } else {
    zoomOnCritical = 0;
    work.flags = 0;
    if (plan->presentationFlags != 0)
      work.flags = 1;
    if (critical != 0)
      work.kind += 200;
    if (work.kind != 210 && critical != 0) {
      zoomOnCritical = 1;
      view->flag = 1;
      criticalPair[0] = work.actor;
      criticalPair[1] = work.firstTarget;
      criticalPair[2] = 255;
      BattleActor_SpawnObjectsForList(criticalPair, 0);
    }
    frames -= 8;
    if (frames <= 0)
      frames = 1;
    {
      s32 first;
      s32 second;

      for (index = 0; index != frames; index++) {
        if (zoomOnCritical != 0) {
          first = work.actor;
          second = work.firstTarget;
          BattleCamera_SetViewAtPairMidpoint(first, second,
                                             index * 30 / frames + 100);
        }
        wait(1);
      }
    }
    Task_Add(BattlePlayback_Run, TASK_PASS_VBLANK);
    if (work.kind != 0) {
      if (saved)
        BattleMotion_ReleaseBuffer(saved);
      if (plan->presentationFlags & 0x4000)
        BattleEffect_DispatchByIdRangeFar(&work);
      else
        BattleEffect_RunFromTableFar(&work);
    }
    if (saved) {
      BattleMotion_DismissBeast(saved);
      Heap_Release(saved);
    }
    BattlePlayback_WaitForReady();
    if (critical != 0) {
      view->flag = 0;
      BattleUnit_ListAllAndSubmit();
      BattleCamera_SetView(0, 0, 0, 100);
    }
    BattleMotion_ReturnToSpot(work.actors[0]);
  }
  if (!saved)
    BattleMotion_ReturnToSpot(work.actor);
  return 0;
}

s32 Inventory_BreakFar(s32 owner, s32 slot);

/// Plays an item's use: the camera turns to the user's side, the user raises
/// the item and, for an item with a battle effect, the screen dims while it
/// plays with its playback. Then the item is spent as its use type says; when
/// its slot empties, later item commands of the same party member this turn
/// follow their items to their new slots (or lose theirs, -1). Called by
/// BattleStaging_DispatchAction for STAGING_ITEM, with the action's entry as
/// selection. Sets gBattleBackgroundWork's yaw and, while an battle effect
/// plays, dims gBattleWork->brightness and BG palettes 6 to 13; changes the
/// user's inventory (Inventory_RemoveFar, Inventory_BreakFar, or the Empty
/// Bottle) and the parameters of gBattleWork->actions.
s32 BattleStaging_RunItem(struct BattlePlan *plan, s32 unused,
                          struct BattleActionRecord *selection) {
  struct BattlePresentationWork work;
  struct MotionObject *object;
  s32 targetOrFadeIndex;
  s32 inventoryEntry;
  struct BattleUnit *unit;
  struct ItemDefinition *itemDefinition;
  struct BattlePresentationTransition *transition =
      (struct BattlePresentationTransition *)Ram_HeapSlots->transitionWork;
  s32 targetYaw;

  if (plan->actorId > 4)
    targetYaw = -TRIG_TURN / 8;
  else
    targetYaw = TRIG_TURN / 8;
  if (transition->targetYaw != targetYaw)
    transition->targetYaw = targetYaw;
  BattleStaging_BuildTargetList(plan, &work);
  BattleHighlight_SetModes(0, 0);
  {
    struct BattleObj *slot = BattleObject_Get(work.actor);
    struct BattleWork *battle;
    u8 *mode;

    battle = Ram_HeapSlots->battleWork;
    mode = (u8 *)battle + 0x869;
    object = slot->object;
    Actor_SelectAnimationFar(object, *mode);
  }
  Actor_SetAnimationSpeedFar((struct Actor *)object, 16);
  if (plan->targetIds[0] <= 7)
    work.side = 1;
  else
    work.side = 0;

  for (targetOrFadeIndex = 0; targetOrFadeIndex != work.count;
       targetOrFadeIndex++) {
    struct ItemAnimation *animation = GetMotionRecord(
        BattleObject_Get(work.actors[targetOrFadeIndex])->object, 0);
    s32 entryIndex;

    for (entryIndex = 0; entryIndex != animation->count - 1; entryIndex++)
      work.params[targetOrFadeIndex][entryIndex] =
          animation->entries[entryIndex]->param;
  }

  Task_Add(BattlePlayback_Run, TASK_PASS_VBLANK);
  if (work.kind != 0) {
    s32 fade;
    struct HeapSlots *heap;

    targetOrFadeIndex = 0;
    heap = Ram_HeapSlots;
    fade = 0;
    do {
      struct BattleWork *battle = heap->battleWork;

      if (targetOrFadeIndex < 20) {
        u16 *palette = battle->palette;
        s32 value = 0x10000 - fade;

        battle->brightness = value;
        ColorBuffer_Scale(palette, 0x050000c0, value, 128);
      }
      wait(1);
      targetOrFadeIndex++;
      fade += 0x444;
    } while (targetOrFadeIndex < 20);
    if (plan->presentationFlags & 0x4000)
      BattleEffect_DispatchByIdRangeFar(&work);
    else
      BattleEffect_RunFromTableFar(&work);
  } else {
    wait(60);
  }
  BattlePlayback_WaitForReady();
  Actor_SelectAnimationFar(object, 1);
  for (targetOrFadeIndex = 0; targetOrFadeIndex != work.count;
       targetOrFadeIndex++)
    BattleMotion_ReturnToSpot(work.actors[targetOrFadeIndex]);

  unit = Unit_Get(selection->unitId);
  inventoryEntry = unit->inventory[selection->parameter];
  itemDefinition = Item_GetDirectFar(inventoryEntry);
  if (itemDefinition->useType == 1) {
    s32 result = Inventory_RemoveFar(selection->unitId, selection->parameter);
    s32 index = selection->parameter;
    if (result == 2) {
      struct BattleWork *battle = Ram_HeapSlots->battleWork;
      u32 row;

      for (row = 0; row < 20; row++) {
        if (battle->actions[row].command == 2 &&
            battle->actions[row].unitId == selection->unitId) {
          if (battle->actions[row].parameter == index)
            battle->actions[row].parameter = -1;
          else if (battle->actions[row].parameter > index)
            battle->actions[row].parameter--;
        }
      }
    }
  } else if (itemDefinition->useType == 2) {
    if ((Battle_RandomFar() & 7) == 0) {
      BattlePlayback_Push(2, unit->inventory[selection->parameter]);
      BattlePlayback_Push(4, (u32)&MsgItemBroke);
      Inventory_BreakFar(selection->unitId, selection->parameter);
      BattlePlayback_RunAll();
    }
  } else if (itemDefinition->useType == 4) {
    if ((inventoryEntry & 0x1ff) == 184)
      inventoryEntry = 185;
    unit->inventory[selection->parameter] = inventoryEntry;
  }
  return 0;
}

/// Sets the first child's value of each of the object's records and clears the
/// others, hiding every child's frame.
void BattleObject_SetColorMode(struct MotionObject *object, s32 value) {
  s32 index = 0;
  s32 child_index;
  s32 count;
  struct ChildRecord *record;
  struct RecordChild *child;
  struct RecordChild **children;
  u8 mask = 0xff;
  u8 frame;

  while ((record = GetMotionRecord(object, index)) != NULL) {
    children = &record->entries[1];
    child = record->entries[0];
    frame = child->frame;
    count = record->count;
    child->param = value;
    child->frame = frame | mask;
    for (child_index = 1; child_index < count; child_index++) {
      child = *children++;
      child->param = 0;
      child->frame |= 0xff;
    }
    index++;
  }
}

// BattleStaging_RunPsynergyCast: The Broken Seal's
// BattleStaging_RunPsynergyCast (games/tbs/src/battle/battle/staging.c),
// ported: turns the camera to the caster (across the field or reversed when the
// plan's flags say so), raises the caster's hand with its element's glow, then
// either fails ("But the Psynergy was blocked!" or "...But doesn't have enough
// PP!") or plays the effect with the events. This game adds a second effect
// object (BattleMotion_CallBeast, shown by BattleMotion_ReleaseBuffer, put back
// and freed by BattleMotion_DismissBeast), sets the caster's mode from the
// battle work's byte 0x869, and skips the playback callback and its wait when
// the plan's word at 100 is set. mode: 1 a Djinni, 2 an enemy's cast. The two
// fixed yaws are stores in each arm of an if (as a ternary or one value the
// arms are folded into a default and an override).

/// Plays Psynergy, a Djinni or an enemy's cast. The camera turns toward
/// the targets from the caster's side, the caster raises its hand and glows
/// in its element (no glow for a Djinni); then the cast fails ("blocked" or
/// too little PP) or its battle effect plays with its playback, and the targets
/// return to their spots.
/// Called by BattleStaging_DispatchAction for STAGING_CAST
/// (CAST_PSYNERGY), STAGING_DJINNI (CAST_DJINNI) and STAGING_ENEMY_CAST
/// (CAST_ENEMY). Sets gBattleBackgroundWork's yaw (and its frames for a fixed
/// view), redraws the party status window without its PP row, and schedules
/// BattlePlayback_Run around the battle effect.
s32 BattleStaging_RunPsynergyCast(struct BattlePlan *plan, s32 mode) {
  struct BattlePresentationWork work;
  struct BattlePresentationTransition *transition =
      Ram_HeapSlots->transitionWork;
  struct MotionObject *object;
  struct HeapSlots *heap;
  void *saved;
  s32 isDjinni;
  s32 targetIndex;

  if (plan->presentationFlags & 0x40000) {
    if (plan->actorId <= 7)
      transition->targetYaw = -TRIG_TURN / 8;
    else
      transition->targetYaw = 0x5000;
    transition->frames = 60;
  } else {
    struct MotionObject *actor = BattleObject_Get(plan->actorId)->object;
    u32 target = plan->targetIds[0];
    u32 actorId = plan->actorId;
    s32 angle = (u16)ArcTan2(actor->x, actor->z);
    s32 targetYaw;

    if (actorId <= 7)
      targetYaw = angle - 0x1800;
    else
      targetYaw = angle + 0x1800;
    targetYaw = (s16)targetYaw;
    targetYaw +=
        ((actorId <= 7 ? TRIG_TURN / 8 : -TRIG_TURN / 8) - targetYaw) * 3 / 4;
    if (!((actorId <= 7) ^ (target <= 7)))
      targetYaw = actorId <= 7 ? 0x2400 : -0x2400;
    if (transition->targetYaw != targetYaw)
      transition->targetYaw = targetYaw;
  }
  if (plan->presentationFlags & 0x80000) {
    if (plan->actorId <= 7)
      transition->targetYaw = -TRIG_TURN / 8;
    else
      transition->targetYaw = TRIG_TURN / 8;
    transition->frames = 60;
  }

  BattleStaging_BuildTargetList(plan, &work);
  isDjinni = mode & 1;
  if (isDjinni)
    work.flags = 1;
  BattleHighlight_SetModes(0, 0);
  heap = Ram_HeapSlots;
  PartyStatus_DrawFar(heap->battleWork->partyStatus & ~1);
  saved = BattleMotion_CallBeast(plan, &work);
  object = BattleObject_Get(work.actor)->object;
  Actor_SelectAnimationFar(object, ((u8 *)heap->battleWork)[0x869]);
  Actor_SetAnimationSpeedFar((struct Actor *)object, 16);
  Sound_PlayFar(SE_CAST);
  if (mode & 2)
    ElementGlow_Play(work.actor, plan->element, 1, 0);
  else if (!isDjinni)
    ElementGlow_Play(work.actor, plan->element, 0, 0);
  if (plan->targetIds[0] <= 7)
    work.side = 1;
  else
    work.side = 0;

  for (targetIndex = 0; targetIndex != work.count; targetIndex++) {
    struct CastAnimation *animation =
        GetMotionRecord(BattleObject_Get(work.actors[targetIndex])->object, 0);
    s32 entryIndex;

    for (entryIndex = 0; entryIndex != animation->count - 1; entryIndex++)
      work.params[targetIndex][entryIndex] =
          animation->entries[entryIndex]->param;
  }
  if (plan->failure != 0) {
    if (plan->failure == 1) {
      BattlePlayback_Push(0, plan->actorId);
      BattlePlayback_Push(4, (u32)&MsgPsynergyBlocked);
    } else {
      BattlePlayback_Push(4, (u32)&MsgCastLacksPp);
    }
    BattlePlayback_RunAll();
    BattleBackground_RestorePalette();
  } else {
    if (((s32 *)plan)[25] == 0)
      Task_Add(BattlePlayback_Run, TASK_PASS_VBLANK);
    if (work.kind) {
      if (saved)
        BattleMotion_ReleaseBuffer(saved);
      if (plan->presentationFlags & 0x4000)
        BattleEffect_DispatchByIdRangeFar(&work);
      else
        BattleEffect_RunFromTableFar(&work);
    } else {
      BattleBackground_RestorePalette();
    }
    if (saved) {
      BattleMotion_DismissBeast(saved);
      Heap_Release(saved);
    }
    if (((s32 *)plan)[25] == 0)
      BattlePlayback_WaitForReady();
    Actor_SelectAnimationFar(object, 1);
    for (targetIndex = 0; targetIndex != work.count; targetIndex++)
      BattleMotion_ReturnToSpot(work.actors[targetIndex]);
  }
  return 0;
}

void BattleMotion_FlashHit(s32 id) {
  u16 selection[2];
  u16 *selected;
  s32 target;
  s32 remaining;

  Unit_Get(id);
  Actor_SelectAnimationFar(BattleObject_Get(id)->object, 5);

  selected = selection;
  remaining = 1;
  do {
    target = 0xff;
    selected[1] = target;
    selected[0] = id;
    PartyStatus_HighlightMembersFar(selected);
    BattleObject_SetColorMode(BattleObject_Get(id)->object, 7);
    wait(2);

    selected[0] = id;
    PartyStatus_HighlightMembersFar(selected);
    BattleObject_SetColorMode(BattleObject_Get(id)->object,
                              BattleMotion_GetSlotPalette(id));
    wait(2);
    remaining--;
  } while (remaining >= 0);

  PartyStatus_DrawFar(Ram_HeapSlots->battleWork->partyStatus);
}

// Battle: clean up defeated units' graphics and formation entries.

/// Removes the unit from the ally and enemy formations and the planned
/// actions. Returns 0 on removal, or -1 if absent from both formations.
/// id: unit ID.
s32 BattleObject_RemoveFromLists(s32 id) {
  struct BattleWork *work;
  s32 i;
  u32 j;
  s32 unit;

  // Ally formation.
  work = Ram_HeapSlots->battleWork;
  ((struct BattleUnit *)Unit_Get(id))->side = BATTLE_UNIT_ABSENT;
  for (i = 0;; i++) {
    unit = work->partyUnits[i];
    if (unit == id) {
      work->partyUnits[i] = BATTLE_UNIT_REMOVED;
      goto removed;
    }
    if (unit == BATTLE_UNIT_LIST_END)
      break;
  }

  // Enemy formation.
  for (j = 0;; j++) {
    unit = work->enemyIds[j];
    if (unit == id) {
      work->enemyIds[j] = BATTLE_UNIT_REMOVED;
      goto removed;
    }
    if (unit == BATTLE_UNIT_LIST_END)
      break;
  }
  return -1;

removed:
  BattleEnemy_ReleaseNameLetter(id);
  for (j = 0; j < 20; j++) {
    if (work->actions[j].unitId == id)
      work->actions[j].unitId = BATTLE_UNIT_LIST_END;
  }
  return 0;
}

/// Defeated unit graphics: animation 4 for allies, 5 for enemies.
/// Enemies wait four more frames, leave the formation, then erase pixel by
/// pixel. id: unit ID.
void BattleMotion_ShowDowned(s32 id) {
  void *recs[4];
  struct BattleUnit *unit;
  struct AnimationObject *rec;
  struct AnimationEntry *ent;
  s32 n;

  unit = Unit_Get(id);
  n = 0;
  while ((rec = GetMotionRecord(BattleObject_Get(id)->object, n)) != 0) {
    if (unit->side != BATTLE_UNIT_ENEMY)
      Animation_ApplyChildArgumentFar(rec, 4);
    else
      Animation_ApplyChildArgumentFar(rec, 5);
    n++;
  }

  if (unit->side == BATTLE_UNIT_ENEMY) {
    n = 0;
    while ((rec = GetMotionRecord(BattleObject_Get(id)->object, n)) != 0) {
      ent = rec->entries[0];
      recs[n] = rec;
      ent->param = 6;
      ent->picture = 0xff;
      n++;
    }
    wait(4);
    BattleObject_RemoveFromLists(id);
    SpriteDissolve_RunManyFar(recs, n); // Erase pixel by pixel.
    BattleObject_MarkForRelease(id);
  }
}
