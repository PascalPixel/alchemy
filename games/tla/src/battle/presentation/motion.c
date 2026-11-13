/// @file
/// A unit's motion graphics in battle: setting its records' frames, the flash
/// when it is hit, its removal from the formations and the graphics of a
/// defeated unit, with routines around them not yet understood.
// Part of presentation.c.

#include "../presentation_internal.h"
#include "module/battle/battle.h"
#include "module/battle/presentation.h"
#include "module/battle/presentation.h"
#include "module/system/system.h"
#include "module/tla.h"
#include "text/msg_ids.h"

void BattleObject_Reset();

struct SavedBattleObj *Func_0811e3ac(struct BattlePlan *plan,
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
  Actor_SetMode(actor, 3);
  Actor_SetAnimationSpeedFar((struct Actor *)actor, 16);
  QueueIoWriteDelay2(0x04000050, 0x3f40);
  Sound_PlayFar(154);
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
      BattleMotion_ReleaseObjectSlotByValue(ids[i]);
    }
  }
  Iwram_CopyWordsEntry(saved, BattleObject_Get(saved->unitId), 44);
  BattleObject_Reset(BattleObject_Get(saved->unitId), single);
  BattleObject_Get(saved->unitId)->scale = 0x13333;
  if (single)
    saved->buffer = Heap_AllocEwram(0x8000);
  else
    saved->buffer = Data_02018000;
  i = ResourceSlot_LoadFar(7, saved->buffer,
                           BattleClass_GetSprite(saved->classId),
                           BattleClass_GetPalette(saved->classId));
  if (i != 0) {
    battleObject->resource = BattleClass_GetSprite(saved->classId) + 0x7000;
    if (!single)
      saved->buffer = NULL;
  } else {
    battleObject->resource = BattleClass_GetSprite(saved->classId);
    if (single)
      Heap_Release(saved->buffer);
    saved->buffer = (u32 *)i;
  }
  ids[0] = saved->unitId;
  ids[1] = 255;
  BattleObject_SpawnObjectsForList((s16 *)ids, 1, 0);
  object->y = 0x280000;
  object->angle = plan->targetIds[0] <= 7 ? 0 : 0x8000;
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

#define Func_080219ccFar Func_08020370

/// Frees the borrowed buffer and releases the motion graphics slot.
void Func_0811e7dc(struct Work0811e7dc *work) {
  struct MotionObject *object = BattleObject_Get(work->objectId)->object;

  if (work->buffer != NULL) {
    Func_080219ccFar(GetMotionRecord(object, 0));
    wait(1);
    Heap_Release(work->buffer);
    work->buffer = NULL;
    {
      struct Record0811e7dc *record = GetMotionRecord(object, 0);

      record->flags = 0x20 | record->flags;
    }
    ResourceMetadata_ReleaseSlotFar(GetMotionRecord(object, 0), 0);
  }
}

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
  Iwram_CopyWordsEntry(BattleObject_Get(saved->unitId), saved, 44);
  if (saved->reactivate) {
    BattleObject_MarkForRelease(unitId);
    BattleMotion_DestroyAllSlotObjects();
  }
  ResourceSlot_LoadFar(7, 0, 0, 0);
  if (saved->single) {
    ids[0] = unitId;
    ids[1] = 255;
    BattleObject_SpawnObjectsForList((s16 *)ids, 1, 0);
    BattleHighlight_SetSpriteMode(unitId, 1);
  } else {
    count = BattleParty_ListActorIds((saved->unitId != 0) + 1, ids);
    for (i = 0; i < count; i++) {
      s32 resource = SummonSlot_RegisterActorSprites(ids[i]);

      if (resource)
        BattleObject_Get(ids[i])->resource = resource;
    }
    BattleObject_SpawnObjectsForList((s16 *)ids, 1, 0);
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

/* VIEW(f11e3ac): routine BattleClass_GetAttackHop as s32
 * BattleClass_GetAttackHop(s32 classId); the definition has void
 * BattleCamera_SetViewAtDepth(s32 x, s32 depth, s32 y, s32 zoom) { void
 * BattleCamera_SetViewAtDepth(s32 x, s32 depth, s32 y) { s32 span = 0x01fe0000;
 * s32 zoom; struc...; tried as the one declaration s32
 * BattleClass_GetAttackHop(s32 classId), it does not compile in tla-ja:
 * games/tla/src/battle/presentation.c:7982: conflicting types for
 * `BattleClass_GetAttackHop'. */
extern s32 BattleClass_GetAttackHop__presentation(s32 classId) __asm__(
    "BattleClass_GetAttackHop");

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
    adjusted = angle - 0x2000;
    if (actorId > 7)
      adjusted = angle + 0x6000;
  }
  adjusted &= 0x7fff;
  facingAngle = (adjusted - 0x2000) / 2 + 0x2000;
  if (view->targetYaw == facingAngle) {
    view->targetYaw = facingAngle;
    wait(1);
  } else {
    view->targetYaw = facingAngle;
    wait(1);
  }

  BattleHighlight_SetModes(0, 0);
  Func_0811ddd8(plan, &work);
  if (work.kind == 135)
    PartyStatus_DrawFar(Ram_HeapSlots->battleWork->partyStatus & ~1);
  saved = Func_0811e3ac(plan, &work);
  unit = Unit_Get(work.actor);
  Unit_Get(work.actors[0]);
  critical = plan->targetCritical[0];
  missed = plan->targetMultiplier[0] == 0;

  object = BattleObject_Get(plan->actorId)->object;
  animation = GetMotionRecord(object, 0);
  frames = ResourceMetadata_SumCommandLengthsFar(
      animation->entries[0]->animationId, 2, 1);
  if (work.kind == 10)
    Actor_SetMode(object, 2);
  else
    BattleMotion_ApproachTarget(
        work.actor, work.actors[0], frames,
        BattleClass_GetAttackHop__presentation(unit->enemyId) << 16);
  if ((mode & 32) && plan->actorId != plan->actorId2 &&
      plan->actorId2 != (work.actors[0] & 0x80)) {
    object = BattleObject_Get(plan->actorId2);

    animation = GetMotionRecord(((struct BattleObj *)object)->object, 0);
    frames = ResourceMetadata_SumCommandLengthsFar(
        animation->entries[0]->animationId, 2, 1);
    BattleMotion_ApproachTarget(
        plan->actorId2, work.actors[0], frames,
        BattleClass_GetAttackHop__presentation(
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
    BattleMotion_ResetObjectAtScaledAnchor(work.actors[0]);
    wait(2);
    wait(4);
    wait(10);
    BattleEvent_Push(0, plan->targetIds[0]);
    BattleEvent_Push(4, (u32)&MsgNimblyDodgesBlow);
    BattleEvent_DispatchQueued();
    ObjMotRst(work.actors[0]);
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
    Task_Add(BattleEvent_Playback, TASK_PASS_VBLANK);
    if (work.kind != 0) {
      if (saved)
        Func_0811e7dc(saved);
      if (plan->presentationFlags & 0x4000)
        BattleEffect_DispatchByIdRangeFar(&work);
      else
        BattleEffect_RunTableEffectFar(&work);
    }
    if (saved) {
      Func_0811e830(saved);
      Heap_Release(saved);
    }
    BattlePlayback_WaitForReady();
    if (critical != 0) {
      view->flag = 0;
      BattleParty_ListAllUnitsAndSubmit();
      BattleCamera_SetView(0, 0, 0, 100);
    }
    ObjMotRst(work.actors[0]);
  }
  if (!saved)
    ObjMotRst(work.actor);
  return 0;
}

// BattleStaging_RunItem: The Broken Seal's BattleStaging_RunItem
// (games/tbs/src/battle/battle/staging.c), ported: plays an item's use, the
// camera turned to the user's side, the user raising the item and, for an item
// with an effect, the screen dimming while the effect plays with the events;
// then spends the item as its use type says. This game sets the user's mode
// from the battle work's byte 0x869. The dimming loop reads the battle work
// through a heap pointer, which keeps 0x03000000 in r11 as the native. Message
// 3176 is MsgItemBroke here, a name not yet in the catalogs.
// Func_0811ddd8 is void (r0 is set before r1 for the call, as the
// native).

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
    targetYaw = -0x2000;
  else
    targetYaw = 0x2000;
  if (transition->targetYaw != targetYaw)
    transition->targetYaw = targetYaw;
  Func_0811ddd8(plan, &work);
  BattleHighlight_SetModes(0, 0);
  {
    struct BattleObj *slot = BattleObject_Get(work.actor);
    struct BattleWork *battle;
    u8 *mode;

    battle = Ram_HeapSlots->battleWork;
    mode = (u8 *)battle + 0x869;
    object = slot->object;
    Actor_SetMode(object, *mode);
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

  Task_Add(BattleEvent_Playback, TASK_PASS_VBLANK);
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
      BattleEffect_RunTableEffectFar(&work);
  } else {
    wait(60);
  }
  BattlePlayback_WaitForReady();
  Actor_SetMode(object, 1);
  for (targetOrFadeIndex = 0; targetOrFadeIndex != work.count;
       targetOrFadeIndex++)
    ObjMotRst(work.actors[targetOrFadeIndex]);

  unit = Unit_Get(selection->unitId);
  inventoryEntry = unit->inventory[selection->parameter];
  itemDefinition = Item_Get(inventoryEntry);
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
      BattleEvent_Push(2, unit->inventory[selection->parameter]);
      BattleEvent_Push(4, (u32)&MsgItemBroke);
      Inventory_BreakFar(selection->unitId, selection->parameter);
      BattleEvent_DispatchQueued();
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
void BattleMotion_SetRecordChildValues(struct MotionObject *object, s32 value) {
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
// object (Func_0811e3ac, shown by Func_0811e7dc, put back and freed by
// Func_0811e830), sets the caster's mode from the battle work's byte 0x869,
// and skips the playback callback and its wait when the plan's word at 100 is
// set. mode: 1 a Djinni, 2 an enemy's cast. The two fixed yaws are stores in
// each arm of an if (as a ternary or one value the arms are folded into a
// default and an override).

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
      transition->targetYaw = -0x2000;
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
    targetYaw += ((actorId <= 7 ? 0x2000 : -0x2000) - targetYaw) * 3 / 4;
    if (!((actorId <= 7) ^ (target <= 7)))
      targetYaw = actorId <= 7 ? 0x2400 : -0x2400;
    if (transition->targetYaw != targetYaw)
      transition->targetYaw = targetYaw;
  }
  if (plan->presentationFlags & 0x80000) {
    if (plan->actorId <= 7)
      transition->targetYaw = -0x2000;
    else
      transition->targetYaw = 0x2000;
    transition->frames = 60;
  }

  Func_0811ddd8(plan, &work);
  isDjinni = mode & 1;
  if (isDjinni)
    work.flags = 1;
  BattleHighlight_SetModes(0, 0);
  heap = Ram_HeapSlots;
  PartyStatus_DrawFar(heap->battleWork->partyStatus & ~1);
  saved = Func_0811e3ac(plan, &work);
  object = BattleObject_Get(work.actor)->object;
  Actor_SetMode(object, ((u8 *)heap->battleWork)[0x869]);
  Actor_SetAnimationSpeedFar((struct Actor *)object, 16);
  Sound_PlayFar(154);
  if (mode & 2)
    Func_08127308(work.actor, plan->element, 1, 0);
  else if (!isDjinni)
    Func_08127308(work.actor, plan->element, 0, 0);
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
      BattleEvent_Push(0, plan->actorId);
      BattleEvent_Push(4, (u32)&MsgPsynergyBlocked);
    } else {
      BattleEvent_Push(4, (u32)&MsgCastLacksPp);
    }
    BattleEvent_DispatchQueued();
    BattleBackground_RestorePalette();
  } else {
    if (((s32 *)plan)[25] == 0)
      Task_Add(BattleEvent_Playback, TASK_PASS_VBLANK);
    if (work.kind) {
      if (saved)
        Func_0811e7dc(saved);
      if (plan->presentationFlags & 0x4000)
        BattleEffect_DispatchByIdRangeFar(&work);
      else
        BattleEffect_RunTableEffectFar(&work);
    } else {
      BattleBackground_RestorePalette();
    }
    if (saved) {
      Func_0811e830(saved);
      Heap_Release(saved);
    }
    if (((s32 *)plan)[25] == 0)
      BattlePlayback_WaitForReady();
    Actor_SetMode(object, 1);
    for (targetIndex = 0; targetIndex != work.count; targetIndex++)
      ObjMotRst(work.actors[targetIndex]);
  }
  return 0;
}

void BattleMotion_FlashHit(s32 id) {
  u16 selection[2];
  u16 *selected;
  s32 target;
  s32 remaining;

  Unit_Get(id);
  Actor_SetMode(BattleObject_Get(id)->object, 5);

  selected = selection;
  remaining = 1;
  do {
    target = 0xff;
    selected[1] = target;
    selected[0] = id;
    PartyStatus_HighlightMembersFar(selected);
    BattleMotion_SetRecordChildValues(BattleObject_Get(id)->object, 7);
    wait(2);

    selected[0] = id;
    PartyStatus_HighlightMembersFar(selected);
    BattleMotion_SetRecordChildValues(BattleObject_Get(id)->object,
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
s32 BattleActor_RemoveFromLists(s32 id) {
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
    BattleActor_RemoveFromLists(id);
    Map_RenderAllAnimatedTilePicturesFar(recs, n); // Erase pixel by pixel.
    BattleObject_MarkForRelease(id);
  }
}
#undef Func_080219ccFar
