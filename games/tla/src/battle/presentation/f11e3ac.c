// ---- games/tla/src/battle/presentation/f11e3ac.c ----
#include "types.h"
#include "actor.h"
#include "battle.h"
#include "battle_type.h"
#include "battle_action.h"
#include "battle_display.h"
#include "motobj.h"
#include "io_write_queue.h"
#include "iwram_call.h"
#include "io_reg.h"
#include "heap_release.h"

struct SavedBattleObj {
  struct BattleObj object;
  u32 *buffer;
  u8 unitId;
  u8 single;
  u8 reactivate;
  u16 classId;
};

extern u32 Data_02018000[];

void wait(s32 frames);
s32 BattleParty_ListActorIds(s32 side, u16 *ids);
void BattleObject_Reset();
s32 BattleClass_GetSprite(s32 index);
s32 BattleClass_GetPalette(s32 index);
s32 ResourceSlot_LoadFar(u32 slot, u32 *buffer, s32 number, u32 variant);

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
  Iwram_CopyWords(saved, BattleObject_Get(saved->unitId), 44);
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
  Func_0811b75c((s16 *)ids, 1, 0);
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

#include "edition.h"

struct Work0811e7dc {
  u8 unk00[0x2c];
  void *buffer;
  u8 objectId;
};

struct Record0811e7dc {
  u8 unk00[26];
  u8 flags;
};

// Far call to Func_080219cc.
void Func_08020370(void *record);
#define Func_080219ccFar Func_08020370
void ResourceMetadata_ReleaseSlotFar(void *record, s32 slot);

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

#include "battle_obj.h"

s32 SummonSlot_RegisterActorSprites(s32 unitId);

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
      s32 resource = SummonSlot_RegisterActorSprites(ids[i]);

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

#include "battle_unit.h"
#include "battle_work.h"
#include "battle_event.h"
#include "battle_side.h"
#include "task.h"
#include "ram_buffer.h"

struct AttackAnimationEntry {
  s16 animationId;
};

struct AttackAnimation {
  u8 unk00[40];
  struct AttackAnimationEntry *entries[1];
};

extern u8 MsgNimblyDodgesBlow[];

s32 ArcTan2(s32 x, s32 y);
void *Unit_Get(s32 owner);
s32 ResourceMetadata_SumCommandLengthsFar(s32 animation, s32 a, s32 b);
/* VIEW(f11e3ac): routine BattleClass_GetAttackHop as s32 BattleClass_GetAttackHop(s32 classId); the definition has void Func_08126700(s32 x, s32 depth, s32 y, s32 zoom) { void Func_08126700(s32 x, s32 depth, s32 y) { s32 span = 0x01fe0000; s32 zoom; struc...; tried as the one declaration s32 BattleClass_GetAttackHop(s32 classId), it does not compile in tla-ja: games/tla/src/battle/presentation/f11e3ac.c:7993: conflicting types for `BattleClass_GetAttackHop'. */
extern s32 BattleClass_GetAttackHop__f11e3ac(s32 classId) __asm__("BattleClass_GetAttackHop");
void BattleMotion_ApproachTarget(s32 actorId, s32 targetId, s32 frames,
                                 s32 hop);
void BattleCamera_SetViewAtPairMidpoint(s32 first, s32 second, s32 scale);
void BattleParty_ListAllUnitsAndSubmit(void);
void Func_08138008(struct BattlePresentationWork *work);
void Func_08138018(struct BattlePresentationWork *work);

s32 Func_0811ea0c(struct BattlePlan *plan, s32 mode) {
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
    BattleMotion_ApproachTarget(work.actor, work.actors[0], frames,
                                BattleClass_GetAttackHop__f11e3ac(unit->enemyId) << 16);
  if ((mode & 32) && plan->actorId != plan->actorId2 &&
      plan->actorId2 != (work.actors[0] & 0x80)) {
    object = BattleObject_Get(plan->actorId2);

    animation = GetMotionRecord(((struct BattleObj *)object)->object, 0);
    frames = ResourceMetadata_SumCommandLengthsFar(
        animation->entries[0]->animationId, 2, 1);
    BattleMotion_ApproachTarget(
        plan->actorId2, work.actors[0], frames,
        BattleClass_GetAttackHop__f11e3ac(
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
        Func_08138008(&work);
      else
        Func_08138018(&work);
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

// Func_0811edb0: The Broken Seal's BattleStaging_RunItem
// (games/tbs/src/battle/battle/staging.c), ported: plays an item's use, the
// camera turned to the user's side, the user raising the item and, for an item
// with an effect, the screen dimming while the effect plays with the events;
// then spends the item as its use type says. This game sets the user's mode
// from the battle work's byte 0x869. The dimming loop reads the battle work
// through a heap pointer, which keeps 0x03000000 in r11 as the native. Message
// 3176 is MsgItemBroke here, a name not yet in the catalogs.
// Func_0811ddd8 is void (r0 is set before r1 for the call, as the
// native).
#include "battle_system.h"
#include "item.h"

struct ItemAnimationEntry {
  u8 unk00[5];
  u8 param;
};

struct ItemAnimation {
  u8 unk00[27];
  u8 count;
  u8 unk1c[12];
  struct ItemAnimationEntry *entries[1];
};

extern u8 MsgItemBroke[];

void ColorBuffer_Scale(u16 *source, u32 destination, s32 value, s32 size);
void Func_0811ddd8(struct BattlePlan *plan,
                   struct BattlePresentationWork *work);
s32 Inventory_BreakFar(s32 owner, s32 slot);

s32 Func_0811edb0(struct BattlePlan *plan, s32 unused,
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
      Func_08138008(&work);
    else
      Func_08138018(&work);
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

struct RecordChild {
  u8 unk00[5];
  u8 param;
  u8 unk06[0x10];
  u8 frame;
};

struct ChildRecord {
  u8 unk00[0x1b];
  u8 count;
  u8 unk1c[0xc];
  struct RecordChild *entries[4];
};

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

// Func_0811f088: The Broken Seal's BattleStaging_RunPsynergyCast
// (games/tbs/src/battle/battle/staging.c), ported: turns the camera to the
// caster (across the field or reversed when the plan's flags say so), raises
// the caster's hand with its element's glow, then either fails ("But the
// Psynergy was blocked!" or "...But doesn't have enough PP!") or plays the
// effect with the events. This game adds a second effect object
// (Func_0811e3ac, shown by Func_0811e7dc, put back and freed by
// Func_0811e830), sets the caster's mode from the battle work's byte 0x869,
// and skips the playback callback and its wait when the plan's word at 100 is
// set. mode: 1 a Djinni, 2 an enemy's cast. The two fixed yaws are stores in
// each arm of an if (as a ternary or one value the arms are folded into a
// default and an override).

struct CastAnimationEntry {
  u8 unk00[5];
  u8 param;
};

struct CastAnimation {
  u8 unk00[27];
  u8 count;
  u8 unk1c[12];
  struct CastAnimationEntry *entries[1];
};

extern u8 MsgCastLacksPp[];
extern u8 MsgPsynergyBlocked[];

void Func_08127308(s32 unitId, s32 element, s32 mode, s32 value);
void BattleBackground_RestorePalette(void);

s32 Func_0811f088(struct BattlePlan *plan, s32 mode) {
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
        Func_08138008(&work);
      else
        Func_08138018(&work);
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

#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "owner_state.h"
#include "field_event.h"

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
    BattleLayout_HighlightPartyPanelsFar(selected);
    BattleMotion_SetRecordChildValues(BattleObject_Get(id)->object, 7);
    wait(2);

    selected[0] = id;
    BattleLayout_HighlightPartyPanelsFar(selected);
    BattleMotion_SetRecordChildValues(BattleObject_Get(id)->object,
                                      BattleMotion_GetSlotPalette(id));
    wait(2);
    remaining--;
  } while (remaining >= 0);

  PartyStatus_DrawFar(Ram_HeapSlots->battleWork->partyStatus);
}

// Battle: clean up defeated units' graphics and formation entries.

#include "battle_summon.h"
#include "sprite.h"

void Animation_ApplyChildArgumentFar(void *, s32);

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

// ---- games/tla/src/battle/strongest_unit.c ----
/* VIEW(strongest_unit): routine Unit_Get as struct BattleUnit *Unit_Get(s32 unitId); owner_state.h has void *Unit_Get(s32 owner); tried as the definition's void *Unit_Get(s32 owner), which the object already declares, it does not compile in tla-ja: games/tla/src/battle/presentation/f11e3ac.c:1004: request for member `hp' in something not a structure or union. */
extern struct BattleUnit *Unit_Get__strongest_unit(s32 unitId) __asm__("Unit_Get");
/* VIEW(strongest_unit): routine Battle_RandomFar as u32 Battle_RandomFar(void); battle_system.h has s32 Battle_RandomFar(void); tried as the rest of the object declares it, Func_0811f4d4 moves: 1 of 95 instructions in tla-ja. */
extern u32 Battle_RandomFar__strongest_unit(void) __asm__("Battle_RandomFar");

/// Picks a random living target on one side: the party (target codes 0x100 +
/// slot) when party is set, otherwise the enemies (0x180 + slot); 0 when none
/// lives.
// No ☀️ twin found.
s32 Func_0811f4d4(s32 party) {
  struct BattleWork *work;
  u16 targets[6];
  s32 count;
  s32 i;

  count = 0;
  work = Ram_HeapSlots->battleWork;

  if (party != 0) {
    for (i = 0; work->partyUnits[i] != 255; i++) {
      if (work->partyUnits[i] != 254 && Unit_Get__strongest_unit(work->partyUnits[i])->hp != 0)
        targets[count++] = i | 0x100;
    }
  } else {
    for (i = 0; work->enemyIds[i] != 255; i++) {
      if (work->enemyIds[i] != 254 && Unit_Get__strongest_unit(work->enemyIds[i])->hp != 0)
        targets[count++] = i | 0x180;
    }
  }
  if (count == 0)
    return 0;
  return targets[(count * Battle_RandomFar__strongest_unit()) >> 16];
}

/// Finds the living unit with the highest Attack on unitId's side (the party
/// for ids 0 to 7, else the enemies), other than unitId itself; unitId when no
/// other lives.
// No ☀️ twin found.
s32 Func_0811f5a4(u32 unitId) {
  struct BattleWork *work = Ram_HeapSlots->battleWork;
  u32 best = unitId;
  s32 bestAttack = Unit_Get__strongest_unit(unitId)->atk;
  s32 i;
  s32 id;

  if (unitId <= 7) {
    for (i = 0; (id = work->partyUnits[i]) != 255; i++) {
      if (id != 254 && Unit_Get__strongest_unit(id)->hp != 0) {
        if (best == unitId || (id != unitId && Unit_Get__strongest_unit(id)->atk > bestAttack)) {
          best = id;
          bestAttack = Unit_Get__strongest_unit(best)->atk;
        }
      }
    }
  } else {
    for (i = 0; (id = work->enemyIds[i]) != 255; i++) {
      if (id != 254 && Unit_Get__strongest_unit(id)->hp != 0) {
        if (best == unitId || (id != unitId && Unit_Get__strongest_unit(id)->atk > bestAttack)) {
          best = id;
          bestAttack = Unit_Get__strongest_unit(best)->atk;
        }
      }
    }
  }
  return best;
}

// ---- games/tla/src/battle/f11f66c.c ----
#include "character.h"

#define TARGET_MODE_NONE 0
#define TARGET_MODE_FOES 1
#define TARGET_MODE_ALLIES 2
#define TARGET_MODE_SELF 4

#define PLACE_PARTY 0x100
#define PLACE_ENEMY 0x180

/* VIEW(f11f66c): routine Unit_Get as void *Unit_Get(s32 owner); strongest_unit has struct BattleUnit *Unit_Get(s32 unitId); tried as the definition's void *Unit_Get(s32 owner), which the object already declares, Func_0811f66c moves: 5 of 776 instructions, BattleActor_ResetRuntimeFields moves: 1 of 97 instructions, BattleActor_DestroyTemporaryObject moves: 1 of 24 instructions, 32 more in tla-ja. */
extern void *Unit_Get__f11f66c(s32 owner) __asm__("Unit_Get");
const struct EnemyDefinition *Enemy_GetDefinitionFar(s32 id);

#define COUNT_PARTIAL_CURES(unit, count)                                       \
  {                                                                            \
    if ((unit)->delusion != 0)                                                 \
      (count)++;                                                               \
    if ((u8)(unit)->confusion != 0)                                            \
      (count)++;                                                               \
    if ((unit)->charm != 0)                                                    \
      (count)++;                                                               \
    if ((unit)->sleep != 0)                                                    \
      (count)++;                                                               \
    if ((unit)->psySeal != 0)                                                  \
      (count)++;                                                               \
    if ((unit)->grimReaper != 0)                                               \
      (count)++;                                                               \
  }

#define COUNT_ALL_CURES(unit, count)                                           \
  {                                                                            \
    COUNT_PARTIAL_CURES((unit), (count));                                      \
    if ((unit)->haunt != 0)                                                    \
      (count)++;                                                               \
    if ((unit)->poison != 0)                                                   \
      (count)++;                                                               \
  }

#define COUNT_POSITIVE_MODIFIERS(unit, count)                                  \
  {                                                                            \
    if ((unit)->attackStage > 0)                                               \
      (count)++;                                                               \
    if ((unit)->defenseStage > 0)                                              \
      (count)++;                                                               \
    if ((unit)->resistanceStage > 0)                                           \
      (count)++;                                                               \
    if ((unit)->elementLevels[0] > 0)                                          \
      (count)++;                                                               \
    if ((unit)->elementLevels[1] > 0)                                          \
      (count)++;                                                               \
    if ((unit)->elementLevels[2] > 0)                                          \
      (count)++;                                                               \
    if ((unit)->elementLevels[3] > 0)                                          \
      (count)++;                                                               \
  }

#define CHECK_EFFECT_TARGET(unit, action, applies, kindIndex)                  \
  {                                                                            \
    (applies) = 0;                                                             \
    switch ((action)->effect) {                                                \
    case ABILITY_EFFECT_ATTACK_UP_2:                                           \
    case ABILITY_EFFECT_ATTACK_UP:                                             \
      if ((unit)->attackStage + 1 <= 4)                                        \
        (applies) = 1;                                                         \
      if ((unit)->attackTurns == 1)                                            \
        (applies)++;                                                           \
      break;                                                                   \
    case ABILITY_EFFECT_ATTACK_DOWN_2:                                         \
    case ABILITY_EFFECT_ATTACK_DOWN:                                           \
      if ((unit)->attackStage - 1 >= -4)                                       \
        (applies) = 1;                                                         \
      if ((unit)->attackTurns == 1)                                            \
        (applies)++;                                                           \
      break;                                                                   \
    case ABILITY_EFFECT_DEFENSE_UP_2:                                          \
    case ABILITY_EFFECT_DEFENSE_UP:                                            \
      if ((unit)->defenseStage + 1 <= 4)                                       \
        (applies) = 1;                                                         \
      if ((unit)->defenseTurns == 1)                                           \
        (applies)++;                                                           \
      break;                                                                   \
    case ABILITY_EFFECT_DEFENSE_DOWN_2:                                        \
    case ABILITY_EFFECT_DEFENSE_DOWN:                                          \
      if ((unit)->defenseStage - 1 >= -4)                                      \
        (applies) = 1;                                                         \
      if ((unit)->defenseTurns == 1)                                           \
        (applies)++;                                                           \
      break;                                                                   \
    case ABILITY_EFFECT_RESISTANCE_UP_2:                                       \
    case ABILITY_EFFECT_RESISTANCE_UP:                                         \
      if ((unit)->resistanceStage + 1 <= 4)                                    \
        (applies) = 1;                                                         \
      if ((unit)->resistanceTurns == 1)                                        \
        (applies)++;                                                           \
      break;                                                                   \
    case ABILITY_EFFECT_RESISTANCE_DOWN_2:                                     \
    case ABILITY_EFFECT_RESISTANCE_DOWN:                                       \
      if ((unit)->resistanceStage - 1 >= -4)                                   \
        (applies) = 1;                                                         \
      if ((unit)->resistanceTurns == 1)                                        \
        (applies)++;                                                           \
      break;                                                                   \
    case ABILITY_EFFECT_CURE_POISON:                                           \
      if ((unit)->poison != 0)                                                 \
        (applies) = 1;                                                         \
      break;                                                                   \
    case ABILITY_EFFECT_RESTORE:                                               \
      COUNT_PARTIAL_CURES((unit), (applies));                                  \
      break;                                                                   \
    case ABILITY_EFFECT_HEAL_HP_60:                                            \
    case ABILITY_EFFECT_HEAL_HP_30:                                            \
    case 70:                                                                   \
    case 71:                                                                   \
      if ((unit)->hp < (unit)->maxHp)                                          \
        (applies) = 1;                                                         \
      break;                                                                   \
    case ABILITY_EFFECT_BREAK:                                                 \
      COUNT_POSITIVE_MODIFIERS((unit), (applies));                             \
      break;                                                                   \
    case ABILITY_EFFECT_CURE_ALL:                                              \
      COUNT_ALL_CURES((unit), (applies));                                      \
      break;                                                                   \
    case ABILITY_EFFECT_CURSE:                                                 \
      if ((unit)->grimReaper == 0)                                             \
        (applies) = 1;                                                         \
      break;                                                                   \
    case ABILITY_EFFECT_POISON:                                                \
      if ((unit)->poison == 0)                                                 \
        (applies) = 1;                                                         \
      break;                                                                   \
    case ABILITY_EFFECT_DEADLY_POISON:                                         \
      if ((unit)->poison <= 1)                                                 \
        (applies) = 1;                                                         \
      break;                                                                   \
    case ABILITY_EFFECT_STUN:                                                  \
    case 85:                                                                   \
      if ((unit)->stun == 0)                                                   \
        (applies) = 1;                                                         \
      break;                                                                   \
    case ABILITY_EFFECT_SLEEP:                                                 \
      if ((unit)->sleep == 0)                                                  \
        (applies) = 1;                                                         \
      break;                                                                   \
    case ABILITY_EFFECT_HAUNT:                                                 \
      if ((unit)->haunt == 0)                                                  \
        (applies) = 1;                                                         \
      break;                                                                   \
    case ABILITY_EFFECT_REVIVE:                                                \
    case ABILITY_EFFECT_REVIVE_HALF:                                           \
    case ABILITY_EFFECT_REVIVE_80:                                             \
    case 73:                                                                   \
      if ((unit)->hp == 0)                                                     \
        (applies) = 100;                                                       \
      break;                                                                   \
    case ABILITY_EFFECT_TOTAL_DEFENSE:                                         \
    case 88:                                                                   \
      if ((unit)->guardLevel <= 1)                                             \
        (applies) = 1;                                                         \
      break;                                                                   \
    case 0:                                                                    \
    case 1:                                                                    \
    case 2:                                                                    \
      break;                                                                   \
    default:                                                                   \
      (applies) = 1;                                                           \
      break;                                                                   \
    }                                                                          \
    if ((unit)->hp == 0 && Battle_IsReviveEffectFar((action)->effect) == 0)    \
      (applies) = 0;                                                           \
    if ((applies) == 0) {                                                      \
      (kindIndex) = ((action)->flags & ABILITY_KIND_MASK) - 1;                 \
      switch (kindIndex) {                                                     \
      case ABILITY_KIND_ATTACK - 1:                                            \
      case ABILITY_KIND_ATTACK_MULTIPLIED - 1:                                 \
        if ((unit)->hp != 0)                                                   \
          (applies)++;                                                         \
        break;                                                                 \
      case ABILITY_KIND_DAMAGE - 1:                                            \
      case ABILITY_KIND_UNLEASHED_DAMAGE - 1:                                  \
      case ABILITY_KIND_SUMMON - 1:                                            \
        if ((action)->power != 0 && (unit)->hp != 0)                           \
          (applies)++;                                                         \
        break;                                                                 \
      case ABILITY_KIND_PP_DAMAGE - 1:                                         \
        if ((unit)->pp != 0)                                                   \
          (applies)++;                                                         \
        break;                                                                 \
      case ABILITY_KIND_HEAL_HP - 1:                                           \
        if ((unit)->hp != 0 && (unit)->hp < (unit)->maxHp)                     \
          (applies)++;                                                         \
        break;                                                                 \
      case ABILITY_KIND_HEAL_PP - 1:                                           \
        if ((unit)->pp < (unit)->maxPp)                                        \
          (applies)++;                                                         \
        break;                                                                 \
      case ABILITY_KIND_EFFECT - 1:                                            \
      case 9 - 1:                                                              \
        if ((action)->power != 0 && (unit)->hp != 0)                           \
          (applies)++;                                                         \
        break;                                                                 \
      case 7 - 1:                                                              \
        (applies)++;                                                           \
        break;                                                                 \
      }                                                                        \
    }                                                                          \
  }

#define STRATEGY_MOST_HP 0
#define STRATEGY_MOST_MAX_HP 1
#define STRATEGY_RANDOM 2

s32 Func_0811f66c(s32 actorId, struct BattleAct *action) {
  u16 orderPositions[6];
  s32 unitIds[6];
  s32 targetPositions[6];
  struct BattleWork *turnOrder;
  struct BattleUnit *unit;
  struct BattleUnit *nextUnit;
  struct BattleUnit *thisUnit;
  s32 candidateCount;
  s32 targetIndex;
  s32 targetCount;
  s32 innerIndex;
  s32 selected;
  s32 unitId;
  s32 value;
  s32 nextValue;
  s32 temp;
  s32 ownSide;
  s32 kindIndex;
  s32 applies;
  u32 roll;
  s16 *slot;

  candidateCount = 0;
  turnOrder = Ram_HeapSlots->battleWork;
  targetCount = 0;

  if (action->targetMode != TARGET_MODE_NONE) {
    ownSide = 0;
    if (action->targetMode == TARGET_MODE_ALLIES ||
        action->targetMode == TARGET_MODE_SELF)
      ownSide = 1;

    if (((actorId < 0 || actorId > 7) && ownSide == 0) ||
        (actorId >= 0 && actorId <= 7 && ownSide != 0)) {
      targetIndex = 0;
      while (turnOrder->partyUnits[targetIndex] != BATTLE_UNIT_LIST_END) {
        slot = &turnOrder->partyUnits[targetIndex];
        unitId = *slot;
        if (unitId != BATTLE_UNIT_REMOVED) {
          if (action->targetMode != TARGET_MODE_SELF || unitId == actorId) {
            unitIds[candidateCount] = unitId;
            orderPositions[candidateCount] = PLACE_PARTY;
            orderPositions[candidateCount] |= targetIndex;
            candidateCount++;
          }
        }
        targetIndex++;
      }
    } else {
      targetIndex = 0;
      while (turnOrder->enemyIds[targetIndex] != BATTLE_UNIT_LIST_END) {
        unitId = turnOrder->enemyIds[targetIndex];
        if (unitId != BATTLE_UNIT_REMOVED) {
          if (action->targetMode != TARGET_MODE_SELF || unitId == actorId) {
            unitIds[candidateCount] = unitId;
            orderPositions[candidateCount] = PLACE_ENEMY;
            orderPositions[candidateCount] |= targetIndex;
            candidateCount++;
          }
        }
        targetIndex++;
      }
    }
  }

  if (candidateCount == 0)
    return -2;

  for (targetIndex = 0; targetIndex < candidateCount; targetIndex++) {
    unit = Unit_Get__f11f66c(unitIds[targetIndex]);
    CHECK_EFFECT_TARGET(unit, action, applies, kindIndex);
    if (applies != 0) {
      unitIds[targetCount] = unitIds[targetIndex];
      targetPositions[targetCount] = orderPositions[targetIndex];
      targetCount++;
    }
  }

  if (targetCount == 0)
    return -1;

  if (action->targetMode == TARGET_MODE_FOES && action->range == 1 &&
      Enemy_GetDefinitionFar(((struct BattleUnit *)Unit_Get__f11f66c(actorId))->enemyId)
              ->targetStrategy != STRATEGY_RANDOM &&
      (u32)((action->flags & ABILITY_KIND_MASK) - ABILITY_KIND_ATTACK) <= 2) {
    selected = -1;
    for (targetIndex = 0; targetIndex < targetCount; targetIndex++) {
      for (innerIndex = targetIndex; innerIndex < targetCount - 1;
           innerIndex++) {
        thisUnit = Unit_Get__f11f66c(unitIds[innerIndex]);
        nextUnit = Unit_Get__f11f66c(unitIds[innerIndex + 1]);
        if (Enemy_GetDefinitionFar(
                ((struct BattleUnit *)Unit_Get__f11f66c(actorId))->enemyId)
                ->targetStrategy == STRATEGY_MOST_HP) {
          value = thisUnit->hp;
          nextValue = nextUnit->hp;
        } else {
          value = thisUnit->maxHp;
          nextValue = nextUnit->maxHp;
        }
        if (value < nextValue) {
          temp = unitIds[innerIndex];
          unitIds[innerIndex] = unitIds[innerIndex + 1];
          unitIds[innerIndex + 1] = temp;
          temp = targetPositions[innerIndex];
          targetPositions[innerIndex] = targetPositions[innerIndex + 1];
          targetPositions[innerIndex + 1] = temp;
        }
      }
    }

    switch (targetCount) {
    case 1:
      selected = 0;
      break;
    case 2:
      roll = (11 * Random16()) >> 16;
      selected = 0;
      if (roll > 5)
        selected = 1;
      break;
    case 3:
      selected = (15 * Random16()) >> 16;
      if (selected <= 5)
        selected = 0;
      else if (selected <= 10)
        selected = 1;
      else
        selected = 2;
      break;
    case 4:
      selected = (18 * Random16()) >> 16;
      if (selected <= 5)
        selected = 0;
      else if (selected <= 10)
        selected = 1;
      else if (selected <= 14)
        selected = 2;
      else
        selected = 3;
      break;
    }

    if (selected >= 0)
      return targetPositions[selected];
  }
  return targetPositions[(Random16() * targetCount) >> 16];
}

#include "scene.h"
#include "battle_status_icon.h"

struct BattleObj *BattleObject_Get(s32 objectId);

void BattleActor_ResetRuntimeFields(s32 actor) {
  u8 *state;
  u8 *cursor;
  s32 count;
  u8 zero;

  state = Unit_Get__f11f66c(actor);
  zero = 0;
  count = 3;
  cursor = state + 0x12f;
  do {
    count--;
    *cursor-- = zero;
  } while (count >= 0);

  state[0x131] = 0;
  state[0x132] = 0;
  state[0x133] = 0;
  state[0x134] = 0;
  state[0x135] = 0;
  state[0x136] = 0;
  state[0x137] = 0;
  state[0x138] = 0;
  state[0x139] = 0;
  state[0x13a] = 0;
  state[0x13b] = 0;
  state[0x13c] = 0;
  state[0x13d] = 0;
  state[0x13e] = 0;
  state[0x13f] = 0;
  state[0x140] = 0;
  state[0x141] = 0;
  state[0x142] = 0;
  state[0x143] = 0;
  state[0x144] = 0;
  state[0x145] = 0;
  state[0x146] = 0;
  state[0x147] = 0;
  state[0x148] = 0;

  Unit_CalculateFar(actor);
  BattleUnit_BuildStatusFlags(actor, BattleObject_Get(actor));
}

#include "input.h"
#include "io_write.h"
#include "frame.h"

struct AdvanceSprite {
  struct AdvanceSprite *next;
  u16 y : 8;
  u16 flags : 8;
  u16 x : 9;
  u16 size : 7;
  u16 tile : 10;
  u16 palette : 6;
  u16 affineParam;
};

struct AdvanceWindow {
  u8 unk00[0xc];
  s16 x; ///< tiles
  s16 y; ///< tiles
};

struct AdvanceOffset {
  u8 unk00[4];
  u16 x; ///< 1/256 pixels
  u16 y; ///< 1/256 pixels
};

/// The battle message window and its scroll offset, in heap slot 0x94.
struct AdvanceDisplay {
  struct AdvanceWindow *window;
  struct AdvanceOffset *offset;
};

extern u8 Data_0812996c[];

s32 Message_AllTypedFar(void);
s32 VramCache_AllocEntry(s32 size);
s32 VramCache_Reload(s32 slot, s32 source);
s32 Resource_ResetEntry(u32 slot);
void QueueIoWriteDelay10(u32 address, u32 value);
void QueueIoWriteDelay6(u32 address, u32 value);
s32 Trig_Sin(s32 angle);
void Oam_AddSpriteAtDepth(void *entry, s32 priority);

/// Waits for the player to advance a battle message: once the window has
/// finished printing, shows the bobbing arrow at its corner until B is held,
/// or A, B, L or R is pressed (or held, after 16 frames), then plays the
/// cursor sound. The Broken Seal's BattleMessage_WaitForAdvance, with the
/// message window read from its heap slot.
void BattleMessage_WaitForAdvance(void) {
  struct AdvanceSprite *prompt;
  struct AdvanceSprite promptStorage;
  s32 frame;
  s32 tileSlot;
  s32 pictureTiles;
  s32 sheet;
  struct AdvanceWindow *window;
  struct AdvanceOffset *offset;

  while (!Message_AllTypedFar())
    wait(1);
  prompt = &promptStorage;
  // The arrow sheet's address held in a local is added after the window's
  // loads; at its use, 36 of 146 instructions differ in tla-en.
  sheet = (s32)Data_0812996c;
  tileSlot = VramCache_AllocEntry(128);
  for (frame = 0;; frame++) {
    // Cycle the eight pictures, one every four frames.
    pictureTiles = ((gFrameCount >> 2) & 7) * 128 + sheet;
    window = ((struct AdvanceDisplay *)Ram_HeapSlots->unk84[4])->window;
    offset = ((struct AdvanceDisplay *)Ram_HeapSlots->unk84[4])->offset;
    QueueIoWriteDelay10((u32)&REG_WINOUT, 4);
    QueueIoWriteDelay6((u32)&REG_WINOUT, 16);
    Io_Write16(16, &REG_BLDALPHA);
    ((u32 *)prompt)[1] = 0xa400;
    ((u32 *)prompt)[2] = 0;
    prompt->tile = VramCache_Reload(tileSlot, pictureTiles);
    prompt->x = window->x * 8 + (offset->x >> 8) + 4;
    // Bob up and down by up to 2 pixels, once every 16 frames.
    prompt->y = Trig_Sin(gFrameCount << 12) / 32768 + window->y * 8 +
                (offset->y >> 8) + 6;
    Oam_AddSpriteAtDepth(prompt, 240);
    // A held advance key counts only after 16 frames, so one still held from
    // the previous message does not skip this one.
    // The held keys reread through a volatile pointer for the last test, a
    // load apart from the B test's; as gInput.held, 14 of 146 instructions
    // differ in tla-en.
    if ((gInput.held & 2) || (gInput.pressed & 0x303) ||
        (frame > 15 && (*(volatile u32 *)&gInput.held & 0x303)))
      break;
    wait(1);
  }
  Sound_PlayFar(111);
  Resource_ResetEntry(tileSlot);
  wait(1);
}

struct CursorSprite {
  struct CursorSprite *next;
  u16 y : 8;
  u16 affine : 2;
  u16 mode : 2;
  u16 mosaic : 1;
  u16 colors : 1;
  u16 shape : 2;
  u16 x : 9;
  u16 affineIndex : 5;
  u16 size : 2;
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u16 unk;
};

struct MenuPoint {
  s32 x;
  s32 y;
};

s32 WindowSprite_GetPointerTilesFar(s32 index);

/// The Broken Seal's BattleMessage_WaitForAdvanceAt: shows the cursor
/// bobbing at (x, y) and waits for A, B, L or R; returns 1.
s32 Func_08120060(s32 x, s32 y) {
  struct MenuPoint position;
  s32 pointerTiles;
  struct CursorSprite promptStorage;
  struct CursorSprite *prompt;
  s32 tileSlot;

  pointerTiles = WindowSprite_GetPointerTilesFar(0);
  position.x = x;
  position.y = y;
  while (!Message_AllTypedFar())
    wait(1);
  prompt = &promptStorage;
  tileSlot = VramCache_AllocEntry(128);
  while (1) {
    QueueIoWriteDelay10((u32)&REG_WINOUT, 4);
    QueueIoWriteDelay6((u32)&REG_WINOUT, 16);
    Io_Write16(16, &REG_BLDALPHA);
    ((s32 *)prompt)[1] = 0x40000000;
    ((s32 *)prompt)[2] = 0;
    prompt->tile = VramCache_Reload(tileSlot, pointerTiles);
    prompt->x = position.x + ((gFrameCount & 4) >> 1) - 4;
    prompt->y = position.y - ((gFrameCount & 4) >> 2) - 8;
    Oam_AddSpriteAtDepth(prompt, 240);
    if (gInput.pressed & 0x303)
      break;
    wait(1);
  }
  Resource_ResetEntry(tileSlot);
  wait(1);
  return 1;
}

void Func_08120158(void) {
  while (!Message_AllTypedFar())
    wait(1);
  ((s32 *)Ram_HeapSlots->unk84[4])[2] = 1;
}

#include "objdisp.h"

s32 BattleObject_MarkForRelease(s32 arg0);

s32 BattleActor_DestroyTemporaryObject(s32 arg0) {
  // UB: declared s32 but returns no value; no caller reads it.
  struct BattleUnit *creature;
  struct BattleObj *runtime;

  creature = Unit_Get__f11f66c(arg0);
  if (creature->side == 1) {
    Unit_SetHpFar(creature, 0);
    BattleActor_RemoveFromLists(arg0);
    BattleObject_MarkForRelease(arg0);
    runtime = BattleObject_Get(arg0);
    Actor_Destroy((struct Actor *)runtime->object);
    runtime->object = 0;
    runtime->active = 0;
  }
}

struct BattleEventFlagWork {
  u8 unk00[0x16c];
  u32 flags;
};

void BattleEvent_GrantDjinnBonus(struct BattleEventFlagWork *runtime,
                                 u32 operand) {
  runtime->flags |= 1;
}

struct BattleDisplayFlags {
  u8 unk00[8];
  s32 marked;
};

/* VIEW(f11f66c): routine BattleMotion_FlashHit as void BattleMotion_FlashHit(u32 unitId, s32 a, s32 b); the definition has void BattleMotion_FlashHit(s32 id); tried as the definition's void BattleMotion_FlashHit(s32 id), which the object already declares, it does not compile in tla-ja: games/tla/src/battle/presentation/f11e3ac.c:1722: too many arguments to function `BattleMotion_FlashHit'. */
extern void BattleMotion_FlashHit__f11f66c(u32 unitId, s32 a, s32 b) __asm__("BattleMotion_FlashHit");
s32 Func_0812824c(s32 unitId, s32 earned);
void BattleObject_UpdatePose(s32 id);
u32 BattlePlayback_Reset(void);

/// Runs the queued battle events in order, then resets the queue.
u32 BattleEvent_DispatchQueued(void) {
  struct BattleWork *work;
  struct BattleEventState *runtime;
  struct BattleEventQueue *queue;
  s32 i;

  work = Ram_HeapSlots->battleWork;
  runtime = &work->events;
  queue = &runtime->queue;

  for (i = 0; i < queue->count; i++) {
    u8 opcode = queue->opcodes[i];
    if (opcode <= 15)
      switch (opcode) {
      case BATTLE_EVENT_DJINN_BONUS:
        BattleEvent_GrantDjinnBonus(runtime, queue->operands[i]);
        break;
      case BATTLE_EVENT_REMOVE_UNIT:
        BattleActor_DestroyTemporaryObject(queue->operands[i]);
        break;
      case BATTLE_EVENT_UNIT_NAME:
        MessageArgument_PushFar(queue->operands[i], 1);
        break;
      case BATTLE_EVENT_NUMBER:
        MessageArgument_PushFar(queue->operands[i], 5);
        break;
      case BATTLE_EVENT_ITEM_NAME:
        MessageArgument_PushFar(queue->operands[i] & 0x1ff, 2);
        break;
      case BATTLE_EVENT_ABILITY_NAME:
        MessageArgument_PushFar(queue->operands[i] & 0x3fff, 4);
        break;
      case BATTLE_EVENT_MARK:
        ((struct BattleDisplayFlags *)Ram_HeapSlots->unk84[4])->marked = 1;
        break;
      case BATTLE_EVENT_CLEAR_ARGUMENTS:
        MessageArgument_ClearAllFar();
        break;
      case BATTLE_EVENT_MESSAGE:
        if ((s32)queue->operands[i] >= 0)
          Message_ShowAndWaitCoreFar(queue->operands[i]);
        BattleMessage_WaitForAdvance();
        MessageArgument_ClearAllFar();
        break;
      case BATTLE_EVENT_MESSAGE_NO_WAIT:
        if ((s32)queue->operands[i] >= 0)
          Message_ShowAndWaitCoreFar(queue->operands[i]);
        MessageArgument_ClearAllFar();
        break;
      case BATTLE_EVENT_HIT:
        if (runtime->pendingCue > 0)
          Sound_PlayFar(runtime->pendingCue);
        BattleMotion_FlashHit__f11f66c(queue->operands[i], 0, 0);
        break;
      case BATTLE_EVENT_DEFEAT:
        Func_0812824c(queue->operands[i], runtime->flags);
        BattleActor_ResetRuntimeFields(queue->operands[i]);
        BattleMotion_ShowDowned(queue->operands[i]);
        break;
      case BATTLE_EVENT_DRAW_STATUS:
        PartyStatus_DrawFar(Ram_HeapSlots->battleWork->partyStatus);
        break;
      case BATTLE_EVENT_REFRESH_UNIT:
        BattleUnit_BuildStatusFlags(queue->operands[i],
                                    BattleObject_Get(queue->operands[i]));
        BattleObject_UpdatePose(queue->operands[i]);
        break;
      case 15:
        BattleActor_RemoveFromLists(queue->operands[i]);
        break;
      }
  }
  return BattlePlayback_Reset();
}

void BattleEvent_Push(u32 opcode, u32 operand) {
  struct BattleEventQueue *queue = &Ram_HeapSlots->battleWork->events.queue;
  u32 index = queue->count;

  queue->opcodes[index] = opcode;
  queue->operands[index] = operand;
  queue->count = index + 1;
}

// Battle: identify effects that apply additional effects to defeated targets.

/// Returns 1 for an effect that still applies when the ability's damage has
/// just knocked its target out: the drains, which give the damage they dealt
/// back to the user.
/// effect: effect ID.
s32 BattleEffect_CheckDead(s32 effect) {
  switch (effect) {
  case 31: // Drain HP.
  case 32: // Drain PP.
  case 60: // Drain HP (half).
  case 69: // Drain PP (one tenth).
    return 1;
  default:
    return 0;
  }
}


s32 BattleUnit_KeepsOneHp(s32 unitId) {
  struct BattleUnit *unit = Unit_Get__f11f66c(unitId);

  if (unit->enemyId == 110 || unit->enemyId == 113)
    return 1;
  return 0;
}

void BattleParty_InsertUnitCentered(u16 *list, s32 unit) {
  s32 counts[2] = {0, 0};
  s32 side;
  s32 slot;
  s32 divider;
  s32 i;
  u16 *entry;

  slot = -1;
  side = 0;
  divider = slot;
  i = 0;
  entry = list;
  do {
    u32 value = *entry++;

    if (value == 254) {
      if (divider >= 0 && slot >= 0) {
        s32 distance = i - divider;

        if (distance < 0)
          distance = divider - i;
        /* FAKEMATCH(barrier): an empty asm that, with the one below, keeps
         * side's 1 and counts' address inside the loop instead of hoisted
         * into r8 and ip; without it, 32 of 67 instructions differ in
         * tla-en. */
        __asm__("");
        if (slot - divider >= 0) {
          if (distance >= slot - divider)
            goto next;
        } else if (distance >= divider - slot) {
          goto next;
        }
      }
      slot = i;
      /* FAKEMATCH(barrier): the second empty asm keeping the loop's 1 and
       * counts' address inside it; without it, 32 of 67 instructions differ
       * in tla-en. */
      __asm__("");
    next:;
    } else {
      if (value == 255)
        break;
      if (value == 128) {
        side ^= 1;
        divider = i;
      }
      counts[side]++;
    }
    i++;
  } while (i <= 5);
  if (slot >= 0) {
    list[slot] = unit;
  } else {
    list[i] = unit;
    list[i + 1] = 255;
  }
}

// An ability applied to one target: damage, healing and its added effect.

#include "minigame.h"
#include "range_falloff.h"
#include "battle_effect.h"
#include "battle_message.h"
#include "main_routine_work.h"

#define ALLY 2 /* Ally. */

#define STEP_LIM(v)                                                            \
  {                                                                            \
    if ((v) < -4)                                                              \
      (v) = -4;                                                                \
    if ((v) > 4)                                                               \
      (v) = 4;                                                                 \
  }

#define ALLY_CHECK(id) ((id) >= 0 && (id) < 8)

#define ELEMENT_BONUS()                                                        \
  {                                                                            \
    if (element != 4)                                                          \
      bonus = power - target->element[element].resistance;                     \
  }

#define MESSAGE_PUSH(m) BattleEvent_Push(BATTLE_EVENT_MESSAGE, (u32)(m))

#define MESSAGE_SIDE(m, e)                                                     \
  {                                                                            \
    if (ALLY_CHECK(tid))                                                       \
      MESSAGE_PUSH(m);                                                         \
    else                                                                       \
      MESSAGE_PUSH(e);                                                         \
  }

#define GUARD_CUT()                                                            \
  {                                                                            \
    guard = target->guardLevel;                                                \
    if (guard != 0) {                                                          \
      if (guard == 1)                                                          \
        dmg /= 2;                                                              \
      else if (guard == 2)                                                     \
        dmg = dmg * 2 / 5;                                                     \
      else                                                                     \
        dmg /= 10;                                                             \
    }                                                                          \
  }

#define RES_STEP(d, v, message)                                                \
  {                                                                            \
    target->resistanceStage += (d);                                            \
    STEP_LIM(target->resistanceStage);                                         \
    BattleEvent_Push(BATTLE_EVENT_NUMBER, (v));                                \
    MESSAGE_PUSH(message);                                                     \
    target->resistanceTurns = 7;                                               \
  }

#define STAT_SET(f, message)                                                   \
  {                                                                            \
    MESSAGE_PUSH(message);                                                     \
    (f) = 7;                                                                   \
  }

/// Applies the acting unit's ability to one of its targets: damage or
/// healing by the ability kind, then its added effect, as The Broken Seal's
/// BattleAbility_ApplyToTarget does, with this game's differences.
s32 BattleTarget_Execute(struct BattlePlan *plan, s32 slot) {
  struct BattleAct *action;
  s32 abilityId;
  struct BattleUnit *actor;
  s32 aid;
  s32 element;
  s32 bonus;
  struct BattleWork *work;
  s32 dist;
  s32 half;
  s32 adj;
  s32 dealt;
  s32 crush;
  s32 hit;
  s32 crit;
  s32 skip;
  s32 type;
  s32 aff;
  struct BattleUnit *bak;
  s32 power;
  s32 hp;
  s32 kind;
  s32 scale;
  s32 pass;
  s16 *cmd;
  u8 tid;
  s8 *rm;
  struct BattleUnit *target;
  u32 n;
  s32 value;
  s32 dmg;
  s32 ap;
  s8 *am;
  s32 guard;
  s16 list[8];
  s32 cnt;
  s32 tmp;
  s32 size;
  bonus = 0;
  work = Ram_HeapSlots->battleWork;
  half = 0;
  dealt = 0;
  crush = 0;
  skip = 0;
  aff = 0;
  size = sizeof(struct BattleUnit);
  bak = Heap_Alloc(size);

  if (abilityId == 312 || abilityId == 316)
    aid = plan->actorId2;
  else
    aid = plan->actorId;
  tid = plan->targetIds[slot];
  abilityId = plan->abilityId;
  element = plan->element;
  adj = plan->targetMultiplier[slot];
  crit = plan->targetCritical[slot];

  action = BattleAction_Get(abilityId);
  actor = Unit_Get__f11f66c(aid);
  target = Unit_Get__f11f66c(tid);
  Iwram_CopyWords(bak, target, size);
  switch (target->enemyId) {
  case 221:
    if (plan->command == 1 && (u32)(aid ^ tid) >> 7 != 0) {
      BattleEvent_Push(BATTLE_EVENT_REFRESH_UNIT, tid);
      BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, tid);
      MESSAGE_PUSH(MsgDissipated);
      goto done;
    }
    break;
  case 101:
  case 102:
  case 103:
    if (abilityId != 573 && work->unk868 != 0 && (u32)(aid ^ tid) >> 7 != 0) {
      BattleEvent_Push(BATTLE_EVENT_REFRESH_UNIT, tid);
      BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, tid);
      MESSAGE_PUSH(MsgFieldProtects);
      goto done;
    }
    break;
  }
  if (target->guardLevel == 4 && (u32)(aid ^ tid) >> 7 != 0) {
    BattleEvent_Push(BATTLE_EVENT_REFRESH_UNIT, tid);
    BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, tid);
    MESSAGE_PUSH(MsgNoEffect);
    goto done;
  }

  if (action->range != 255) {
    dist = plan->targetOffset[slot];
    if (dist < 0)
      dist = -dist;
  } else {
    dist = 0;
  }

  if (element != 4) {
    s32 i;

    value = target->baseElement[element].resistance;
    for (i = 0; i < 4 && value >= target->baseElement[i].resistance; i++)
      ;
    if (i == 4)
      aff = -1;

    for (i = 0; i < 4 && value <= target->baseElement[i].resistance; i++)
      ;
    if (i == 4)
      aff = 1;
  }

  if (plan->element >= 0 && plan->element <= 3) {
    cmd = &plan->command;
    if (*cmd != 2) {
      power = actor->element[plan->element].power;
      goto power_end;
    }
  } else
    cmd = &plan->command;
  power = 100;
power_end:

  if (plan->command == 5 && (plan->element >= 0 && plan->element <= 3) &&
      aff > 0) {
    s32 chance;

    chance = power - target->element[plan->element].resistance;
    chance += 30;
    chance *= 65536 / 100;
    if (chance > (Battle_RandomFar() & 0xffff))
      BattleEvent_Push(BATTLE_EVENT_DJINN_BONUS, 5);
  }

  if (abilityId == 686 || abilityId == 357)
    Battle_ApplyActionExtras(tid, 0, 0);
  if (abilityId == 720 || abilityId == 721)
    Battle_ApplyActionExtras(tid, 1, 0);
  if (abilityId == 728)
    Battle_ApplyActionExtras(tid, 1, 1);
  type = action->flags & 15;
  {
    s32 res;

    res = plan->targetHit[slot];
    if (res == -1)
      hit = BattleEffect_RollSuccessFar(aid, tid, element, action->effect,
                                        gHitTable[dist]);
    else
      hit = res;
  }
  if (action->effect == 50 || action->effect == 51 || action->effect == 86 ||
      action->effect == 87) {
    s32 cls;
    s32 ent;
    s32 chg;

    cls = actor->enemyId;
    chg = -1;
    ent = Enemy_GetFreeSlot();
    if (action->effect == 51) {
      cls = BattleFormation_GetRandomMember(work->enemyGroup);
    } else if (action->effect == 86) {
      if (actor->enemyId == 164)
        cls = (Battle_RandomFar() & 3) + 378;
      else
        cls = 81;
    } else if (action->effect == 87) {
      if (work->queueCount != 0) {
        cls = work->queuedSummons[work->queueHead];
        chg = work->queuedCharges[work->queueHead];
      } else
        hit = 0;
    }
    if (hit != 0 && Enemy_CheckRoom(cls) != 0 && ent >= 0) {
      if (chg == -1) {
        chg = BattleEnemy_ClaimNameLetter(cls, 1);
        if (chg & 0x8000)
          BattleEnemy_LetterFirstOfClass(cls);
      }
      BattleUnit_SetFar(ent, cls, chg & 0x7fff);
      if (action->effect == 87) {
        work->queueCount--;
        work->queueHead = (work->queueHead + 1) % 2;
      }
      if (actor->enemyId == 164)
        BattleParty_InsertUnitCentered(work->enemyIds, ent);
      else {
        s32 i;

        for (i = 0; i < 6; i++) {
          if (work->enemyIds[i] == 254) {
            work->enemyIds[i] = ent;
            break;
          }
          if (work->enemyIds[i] == 255) {
            work->enemyIds[i] = ent;
            work->enemyIds[i + 1] = 255;
            break;
          }
        }
      }
      Enemy_Update();
      {
        struct BattleObj *obj;

        obj = BattleObject_Get(ent);
        BattleObject_Make((struct BattleObj *)obj, ent, obj->anchorX / 0x10000,
                          obj->anchorZ / 0x10000);
      }
      BattleObject_Put();
      {
        s32 num;

        num = BattleEnemy_List(list);
        for (cnt = 0; cnt < num; cnt++)
          ObjMotRst((u16)list[cnt]);
      }
      BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, ent);
      if (action->effect == 87)
        MESSAGE_PUSH(MsgUpToFight);
      else if (abilityId != 503)
        MESSAGE_PUSH(MsgAppears);
      else
        MESSAGE_PUSH(MsgSplitOff);
    } else if (abilityId == 503) {
      MESSAGE_PUSH(MsgSplitFailed);
    } else {
      MESSAGE_PUSH(MsgNoOneCame);
    }
  }

  if (hit != 0) {
    if (action->effect == 53 || action->effect == 83) {
      hit = 0;
      for (n = 0; n < 20; n++) {
        if (work->actions[n].unitId == tid) {
          hit = 1;
          break;
        }
      }
    } else if (action->effect == 35) {
      half = 1;
    } else if (action->effect == 34) {
      crush = 1;
    } else if (action->effect == 27) {
      skip = 1;
    } else if (action->effect == 55) {
      if (actor->hp != 0)
        BattleEvent_Push(BATTLE_EVENT_REMOVE_UNIT, aid);
    } else if (action->effect == 32) {
      if (target->pp != 0)
        type = 10;
      else
        hit = 0;
    } else if (action->effect == 90) {
      half = 2;
    } else if (action->effect == 91) {
      half = 2;
    }
  }

  if ((action->effect != 75 || slot == 0) && skip == 0 &&
      (target->hp != 0 || Battle_IsReviveEffectFar(action->effect) != 0)) {
    s32 pp;
    s32 cur;

    switch (type) {
    case 3:
    case 4: {
      s32 def;

      if (target->guardLevel == 4)
        break;
      hp = target->hp;
      def = target->def;
      scale = def;
      if (half != 0) {
        if (half == 2)
          scale = 0;
        else
          scale = def / 2;
      }
      pass = 1;
      do {
        ELEMENT_BONUS();
        if (pass == 0)
          bonus = 0;
        ap = action->power;
        if (type == 4)
          dmg = Battle_CalculateAttackDamageFar(actor->atk, scale, 0, bonus) *
                ap / 10;
        else {
          s32 atk;

          atk = actor->atk;
          if (abilityId == 312 || abilityId == 316) {
            atk = ((struct BattleUnit *)Unit_Get__f11f66c(plan->actorId))->atk;
            if (abilityId == 312)
              atk += ((struct BattleUnit *)Unit_Get__f11f66c(plan->actorId2))->atk / 4;
          }
          dmg = Battle_CalculateAttackDamageFar(atk, scale, ap, bonus);
        }
        dmg *= adj;
        if (crit != 0) {
          if (crit == 1)
            dmg = dmg * 5 / 4;
          else
            dmg = dmg * 3 / 2;
          dmg += (target->level / 5) + 6;
          if (pass == 0) {
            BattleEvent_Push(BATTLE_EVENT_MARK, 0);
            {
              s32 message;

              message = (s32)MsgCritical;
              if (ALLY_CHECK(tid))
                message += 1;
              BattleEvent_Push(BATTLE_EVENT_MESSAGE_NO_WAIT, message);
            }
          }
        }
        dmg += Battle_RandomFar() & 3;
        GUARD_CUT();
        if (dmg <= 0)
          dmg = 1;
        if (crush != 0) {
          if (dmg < hp - 1) {
            dmg = hp - 1;
            if (dmg <= 0)
              dmg = 1;
          }
        }
        if (GameFlag_Test(366) != 0 && plan->command == 5 && hp <= dmg) {
          dmg = hp - 1;
        }
        pass++;
      } while (pass <= 1);
      hp -= dmg;
      BattleEvent_Push(BATTLE_EVENT_HIT, tid);
      BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, tid);
      BattleEvent_Push(BATTLE_EVENT_NUMBER, dmg);
      {
        s32 message;

        if (ALLY_CHECK(tid))
          message = (s32)MsgDmgEmphP + aff;
        else
          message = (s32)MsgDmgEmphE + aff;
        BattleEvent_Push(BATTLE_EVENT_MESSAGE, message);
      }

      if (hp <= 0 && BattleUnit_KeepsOneHp(tid) != 0)
        hp = 1;
      if (hp <= 0) {
        hp = 0;
        BattleEvent_Push(BATTLE_EVENT_DEFEAT, tid);
        if (target->enemyId != 369 && target->enemyId != 373) {
          BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, tid);
          MESSAGE_SIDE(MsgGoesDown, MsgFelled);
        }
      } else
        BattleEvent_Push(BATTLE_EVENT_REFRESH_UNIT, tid);
      dealt = target->hp - hp;
      target->hp = hp;
      Unit_UpdateRatiosFar(tid);
      break;
    }

    case 10: {
      if (target->guardLevel == 4)
        break;
      if (action->power == 0)
        break;
      pp = target->pp;
      ELEMENT_BONUS();
      ap = action->power;
      dmg = Battle_CalculatePsynergyPowerFar(ap, bonus, 256);
      dmg = dmg * gPpLossTable[dist] / 100;
      dmg *= adj;
      GUARD_CUT();
      if (action->effect == 32 && dmg > pp)
        dmg = pp;
      if (dmg > pp)
        dmg = pp;

      BattleEvent_Push(BATTLE_EVENT_HIT, tid);
      BattleEvent_Push(BATTLE_EVENT_NUMBER, dmg);
      BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, tid);
      {
        s32 message;

        if (ALLY_CHECK(tid))
          message = (s32)MsgPpLossP;
        else
          message = (s32)MsgPpLossE;
        BattleEvent_Push(BATTLE_EVENT_MESSAGE, message);
        pp -= dmg;
      }
      if (pp <= 0)
        pp = 0;
      BattleEvent_Push(BATTLE_EVENT_REFRESH_UNIT, tid);
      dealt = target->pp - pp;
      target->pp = pp;
      Unit_UpdateRatiosFar(tid);
      break;
    }

    case 1: {
      if (action->power == 0)
        break;
      cur = target->hp;
      if (cur == 0)
        break;

      ap = action->power;
      dmg = Battle_CalculateRecoveryFar(ap, element == 4 ? 100 : power, 256);
      dmg = dmg * gHpRecoveryTable[dist] / 100;
      dmg *= adj;
      dmg += Battle_RandomFar() & 3;
      cur += dmg;
      if (cur > target->maxHp) {
        cur = target->maxHp;
        dmg = cur - target->hp;
      }
      BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, tid);
      if (cur == target->maxHp)
        MESSAGE_PUSH(MsgHpFull);
      else {
        BattleEvent_Push(BATTLE_EVENT_NUMBER, dmg);
        MESSAGE_PUSH(MsgHpRecover);
      }
      dealt = target->hp - cur;
      target->hp = cur;
      Unit_UpdateRatiosFar(tid);
      break;
    }

    case -1: {
      if (target->guardLevel == 4)
        break;
      if (action->power == 0)
        break;
      pp = target->pp;
      ELEMENT_BONUS();
      ap = action->power;
      dmg = Battle_CalculatePsynergyPowerFar(ap, bonus, 256);
      dmg = dmg * gPpDmgTable[dist] / 100;
      dmg *= adj;
      GUARD_CUT();
      BattleEvent_Push(BATTLE_EVENT_HIT, tid);
      BattleEvent_Push(BATTLE_EVENT_NUMBER, dmg);
      BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, tid);
      {
        s32 message;

        if (ALLY_CHECK(tid))
          message = (s32)MsgDmgP;
        else
          message = (s32)MsgDmgE;
        BattleEvent_Push(BATTLE_EVENT_MESSAGE, message);
        pp -= dmg;
      }
      if (pp <= 0)
        pp = 0;
      BattleEvent_Push(BATTLE_EVENT_REFRESH_UNIT, tid);
      target->pp = pp;
      Unit_UpdateRatiosFar(tid);
      break;
    }

    case 5:
    case 6:
    case 8: {
      s32 round;

      if (target->guardLevel == 4)
        break;
      if (action->power == 0)
        break;
      cur = target->hp;
      round = 1;
      do {
        ELEMENT_BONUS();
        if (round == 0)
          bonus = 0;
        ap = action->power;
        if (abilityId == 683 || abilityId == 673 || abilityId == 724 ||
            plan->command == 6 || plan->command == 10) {
          s32 rate;

          rate = 0;
          switch (abilityId) {
          default:
          case 393:
            break;
          case 406:
          case 388:
          case 396:
          case 380:
            rate = 3;
            break;
          case 409:
          case 399:
          case 391:
          case 383:
            rate = 12;
            break;
          case 401:
            rate = 12;
            break;
          case 683:
            rate = 35;
            break;
          case 407:
          case 400:
          case 397:
          case 389:
          case 384:
          case 381:
            rate = 6;
            break;
          case 398:
          case 390:
          case 408:
          case 382:
            rate = 9;
            break;
          case 392:
          case 410:
            rate = 9;
            break;
          case 402:
            rate = 7;
            break;
          case 411:
          case 403:
          case 385:
            rate = 15;
            break;
          case 394:
            rate = 21;
            break;
          case 412:
            rate = 24;
            break;
          case 673:
          case 386:
            rate = 30;
            break;
          case 724:
          case 404:
            rate = 40;
            break;
          }
          value = target->maxHp;
          if (value > 10000)
            value = 10000;
          ap += value * rate / 100;
        }
        dmg = Battle_CalculatePsynergyPowerFar(ap, bonus, 256);
        dmg *= adj;
        switch (type & 15) {
        case 5:
          dmg = gHpDmgTable5[dist] * dmg / 100;
          break;
        case 8:
          dmg = dmg * gHpDmgTable8[dist] / 100;
          break;
        case 6:
          dmg = dmg * gHpDmgTable6[dist] / 100;
          break;
        }
        dmg += Battle_RandomFar() & 3;
        GUARD_CUT();
        if ((gMainRoutineWork.battleRule24b == 6 ||
             (GameFlag_Test(366) != 0 && plan->command == 6)) &&
            cur > dmg) {
          dmg = cur;
        }
        round++;
      } while (round <= 1);
      BattleEvent_Push(BATTLE_EVENT_HIT, tid);
      BattleEvent_Push(BATTLE_EVENT_NUMBER, dmg);
      BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, tid);
      {
        s32 message;

        if (ALLY_CHECK(tid))
          message = (s32)MsgDmgEmphP + aff;
        else
          message = (s32)MsgDmgEmphE + aff;
        BattleEvent_Push(BATTLE_EVENT_MESSAGE, message);
        cur -= dmg;
      }

      if (cur <= 0 && BattleUnit_KeepsOneHp(tid) != 0)
        cur = 1;
      if (gMainRoutineWork.battleRule24b == 6)
        cur = 0;
      if (cur <= 0) {
        BattleEvent_Push(BATTLE_EVENT_DEFEAT, tid);
        BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, tid);
        {
          s32 message;

          cur = 0;
          if (ALLY_CHECK(tid))
            message = (s32)MsgGoesDown;
          else
            message = (s32)MsgFelled;
          BattleEvent_Push(BATTLE_EVENT_MESSAGE, message);
        }
      } else
        BattleEvent_Push(BATTLE_EVENT_REFRESH_UNIT, tid);
      dealt = target->hp - cur;
      target->hp = cur;
      Unit_UpdateRatiosFar(tid);
      break;
    }

    case 11: {
      if (action->power == 0)
        break;
      pp = target->pp;
      ap = action->power;
      dmg = Battle_CalculateRecoveryFar(ap, element == 4 ? 100 : power, 256);
      dmg = dmg * gPpRecoveryTable[dist] / 100;
      dmg *= adj;
      pp += dmg;
      if (pp > target->maxPp) {
        pp = target->maxPp;
        dmg = pp - target->pp;
      }
      BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, tid);
      if (pp == target->maxPp)
        MESSAGE_PUSH(MsgPpFull);
      else {
        BattleEvent_Push(BATTLE_EVENT_NUMBER, dmg);
        MESSAGE_PUSH(MsgPpRecover);
      }
      target->pp = pp;
      Unit_UpdateRatiosFar(tid);
      break;
    }

    case 0:
    case 7:
    case 9:
      break;

    case 2:
      if (target->guardLevel == 4)
        break;
      if (hit != 0) {
        if (action->power == 0)
          break;
        cur = target->hp;
        ELEMENT_BONUS();
        ap = action->power;
        dmg = Battle_CalculatePsynergyPowerFar(ap, bonus, 256);
        dmg *= adj;
        dmg = dmg * gHpDmgTable[dist] / 100;
        GUARD_CUT();
        BattleEvent_Push(BATTLE_EVENT_HIT, tid);
        BattleEvent_Push(BATTLE_EVENT_NUMBER, dmg);
        BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, tid);
        {
          s32 message;

          if (ALLY_CHECK(tid))
            message = (s32)MsgDmgP;
          else
            message = (s32)MsgDmgE;
          BattleEvent_Push(BATTLE_EVENT_MESSAGE, message);
          cur -= dmg;
        }

        if (cur <= 0 && BattleUnit_KeepsOneHp(tid) != 0)
          cur = 1;
        if (cur <= 0) {
          BattleEvent_Push(BATTLE_EVENT_DEFEAT, tid);
          cur = 0;
          if (target->enemyId != 369 && target->enemyId != 373) {
            BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, tid);
            MESSAGE_SIDE(MsgGoesDown, MsgFelled);
          }
        } else
          BattleEvent_Push(BATTLE_EVENT_REFRESH_UNIT, tid);
        dealt = target->hp - cur;
        target->hp = cur;
        Unit_UpdateRatiosFar(tid);
        break;
      }
      BattleEvent_Push(BATTLE_EVENT_REFRESH_UNIT, tid);
      BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, tid);
      MESSAGE_PUSH(MsgNoEffect);
      break;
    }
  }

  if (work->unk868 != 0 && abilityId == 573) {
    BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, tid);
    MESSAGE_PUSH(MsgFieldBroken);
    work->unk868 = 0;
  }
  BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, tid);
  if ((action->effect != 75 || slot != 0) &&
      (Battle_IsReviveEffectFar(action->effect) != 0 || target->hp != 0 ||
       BattleEffect_CheckDead(action->effect) != 0) &&
      hit != 0) {
    switch (action->effect) {

    case 64:
      if (target->delusion != 0) {
        target->delusion = 0;
        MESSAGE_PUSH(MsgCureDelusion);
        BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, tid);
      }
      if (target->stun != 0) {
        target->stun = 0;
        BattleEvent_Push(BATTLE_EVENT_CLEAR_ARGUMENTS, 0);
        BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, tid);
        MESSAGE_PUSH(MsgCureStun);
      }
      if (target->sleep != 0) {
        target->sleep = 0;
        MESSAGE_PUSH(MsgWakes);
        BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, tid);
      }
      if (target->psySeal != 0) {
        target->psySeal = 0;
        MESSAGE_PUSH(MsgCureSeal);
        BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, tid);
      }
      if (target->grimReaper != 0) {
        target->grimReaper = 0;
        MESSAGE_PUSH(MsgCureCurse);
        BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, tid);
      }
      if (target->haunt != 0) {
        target->haunt = 0;
        MESSAGE_PUSH(MsgCureSpirit);
        BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, tid);
      }
      if (target->poison != 0) {
        MESSAGE_PUSH(MsgCurePoison);
        target->poison = 0;
      }
      BattleEvent_Push(BATTLE_EVENT_CLEAR_ARGUMENTS, 0);
      break;

    case 4:
      if (target->delusion != 0) {
        target->delusion = 0;
        BattleEvent_Push(BATTLE_EVENT_CLEAR_ARGUMENTS, 0);
        BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, tid);
        MESSAGE_PUSH(MsgCureDelusion);
      }
      if (target->stun != 0) {
        target->stun = 0;
        BattleEvent_Push(BATTLE_EVENT_CLEAR_ARGUMENTS, 0);
        BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, tid);
        MESSAGE_PUSH(MsgCureStun);
      }
      if (target->sleep != 0) {
        target->sleep = 0;
        BattleEvent_Push(BATTLE_EVENT_CLEAR_ARGUMENTS, 0);
        BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, tid);
        MESSAGE_PUSH(MsgWakes);
      }
      if (target->psySeal != 0) {
        target->psySeal = 0;
        BattleEvent_Push(BATTLE_EVENT_CLEAR_ARGUMENTS, 0);
        BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, tid);
        MESSAGE_PUSH(MsgCureSeal);
      }
      if (target->grimReaper == 0)
        break;
      target->grimReaper = 0;
      BattleEvent_Push(BATTLE_EVENT_CLEAR_ARGUMENTS, 0);
      BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, tid);
      MESSAGE_PUSH(MsgCureCurse);
      break;

    case 61:
    case 62:
    case 70:
    case 71:
    case 76: {
      s32 old;
      s32 maxu;
      s32 maxv;
      s32 heal;
      u16 *pt;

      /* FAKEMATCH(temporary): a pointer walked down from hp to maxHp reads each
       * stat twice, unsigned and signed; as reads of target->hp and
       * target->maxHp, 94 of 3451 instructions differ in tla-en. */
      pt = (u16 *)&target->hp;
      old = *pt--;
      heal = *(s16 *)(pt + 1);
      if (action->effect == 76) {
        s32 m;

        pt--;
        maxu = *pt--;
        m = *(s16 *)(pt + 1);
        heal += m * 2 / 5;
      } else if (action->effect == 71) {
        pt--;
        maxu = *pt--;
        maxv = *(s16 *)(pt + 1);
        heal += maxv * 7 / 10;
      } else if (action->effect == 70) {
        maxu = (u16)target->maxHp;
        heal += (s16)maxu / 2;
        /* FAKEMATCH(barrier): an empty asm on maxu, so the clamp shifts maxu
         * left by 16 again instead of sharing this halving's shift; without
         * it, 45 of 3451 instructions differ in tla-en. */
        asm("" : "+r"(maxu));
      } else {
        pt--;
        maxu = *pt--;
        maxv = *(s16 *)(pt + 1);
        if (action->effect == 61) {
          s32 p = maxv * 60;
          heal += p / 100;
        } else {
          s32 p = maxv * 30;
          heal += p / 100;
        }
      }
      if (heal > (s16)maxu)
        heal = (s16)maxu;
      tmp = heal - (s16)old;
      if (tmp == 0 && type != 1)
        break;
      if (heal == (s16)maxu)
        MESSAGE_PUSH(MsgHpFull);
      else {
        BattleEvent_Push(BATTLE_EVENT_NUMBER, tmp);
        MESSAGE_PUSH(MsgHpRecover);
      }
      target->hp = heal;
      Unit_UpdateRatiosFar(tid);
      break;
    }

    case 63:
    case 77:
    case 78: {
      s32 old;
      s32 maxv;
      s32 heal;
      s32 maxu;
      u16 *pt;

      /* FAKEMATCH(temporary): a pointer walked down from pp to maxPp reads each
       * stat twice, unsigned and signed; as reads of target->pp and
       * target->maxPp, 255 of 3451 instructions differ in tla-en. */
      pt = (u16 *)&target->pp;
      old = *pt--;
      heal = *(s16 *)(pt + 1);
      if (action->effect == 77) {
        maxv = target->maxPp;
        maxu = (u16)target->maxPp;
        heal += (s16)maxv / 10;
      } else if (action->effect == 78) {
        pt--;
        maxu = *pt--;
        maxv = *(s16 *)(pt + 1);
        heal += maxv * 3 / 10;
      } else {
        pt--;
        maxu = *pt--;
        maxv = *(s16 *)(pt + 1);
        heal += maxv * 7 / 100;
      }
      if (heal > (s16)maxu)
        heal = (s16)maxu;
      tmp = heal - (s16)old;
      if (tmp == 0 && type != 11)
        break;
      if (heal == (s16)maxu)
        MESSAGE_PUSH(MsgPpFull);
      else {
        BattleEvent_Push(BATTLE_EVENT_NUMBER, tmp);
        MESSAGE_PUSH(MsgPpRecover);
      }
      target->pp = heal;
      Unit_UpdateRatiosFar(tid);
      break;
    }

    case 59:
      target->agilityStage = 8;
      target->agilityTurns = 5;
      Unit_CalculateFar(tid);
      BattleEvent_Push(BATTLE_EVENT_NUMBER, target->agi - bak->agi);
      MESSAGE_PUSH(MsgAgiUp);
      break;

    case 58:
      am = &target->agilityStage;
      {
        u8 v;

        v = -4;
        *am = v;
      }
      target->agilityTurns = 5;
      Unit_CalculateFar(tid);
      BattleEvent_Push(BATTLE_EVENT_NUMBER, bak->agi - target->agi);
      MESSAGE_PUSH(MsgAgiDown);
      break;

    case 9:
      target->attackStage -= 1;
      STEP_LIM(target->attackStage);
      Unit_CalculateFar(tid);
      BattleEvent_Push(BATTLE_EVENT_NUMBER, bak->atk - target->atk);
      MESSAGE_PUSH(MsgAtkDown);
      target->attackTurns = 7;
      break;

    case 8:
      target->attackStage -= 2;
      STEP_LIM(target->attackStage);
      Unit_CalculateFar(tid);
      BattleEvent_Push(BATTLE_EVENT_NUMBER, bak->atk - target->atk);
      MESSAGE_PUSH(MsgAtkDown);
      target->attackTurns = 7;
      break;

    case 7:
      target->attackStage += 1;
      STEP_LIM(target->attackStage);
      Unit_CalculateFar(tid);
      BattleEvent_Push(BATTLE_EVENT_NUMBER, target->atk - bak->atk);
      MESSAGE_PUSH(MsgAtkUp);
      target->attackTurns = 7;
      break;

    case 6:
      target->attackStage += 2;
      STEP_LIM(target->attackStage);
      Unit_CalculateFar(tid);
      BattleEvent_Push(BATTLE_EVENT_NUMBER, target->atk - bak->atk);
      MESSAGE_PUSH(MsgAtkUp);
      target->attackTurns = 7;
      break;

    case 13:
      target->defenseStage -= 1;
      STEP_LIM(target->defenseStage);
      Unit_CalculateFar(tid);
      BattleEvent_Push(BATTLE_EVENT_NUMBER, bak->def - target->def);
      MESSAGE_PUSH(MsgDefDown);
      target->defenseTurns = 7;
      break;

    case 12:
      target->defenseStage -= 2;
      STEP_LIM(target->defenseStage);
      Unit_CalculateFar(tid);
      BattleEvent_Push(BATTLE_EVENT_NUMBER, bak->def - target->def);
      MESSAGE_PUSH(MsgDefDown);
      target->defenseTurns = 7;
      break;

    case 11:
      target->defenseStage += 1;
      STEP_LIM(target->defenseStage);
      Unit_CalculateFar(tid);
      BattleEvent_Push(BATTLE_EVENT_NUMBER, target->def - bak->def);
      MESSAGE_PUSH(MsgDefUp);
      target->defenseTurns = 7;
      break;

    case 10:
      target->defenseStage += 2;
      STEP_LIM(target->defenseStage);
      Unit_CalculateFar(tid);
      BattleEvent_Push(BATTLE_EVENT_NUMBER, target->def - bak->def);
      MESSAGE_PUSH(MsgDefUp);
      target->defenseTurns = 7;
      break;

    case 5:
      if (target->hp != 0)
        break;
      MESSAGE_PUSH(MsgRevived);
      target->hp = target->maxHp;
      Unit_UpdateRatiosFar(tid);
      break;

    case 56:
      if (target->hp != 0)
        break;
      MESSAGE_PUSH(MsgRevived);
      target->hp = target->maxHp / 2;
      Unit_UpdateRatiosFar(tid);
      break;

    case 57:
      if (target->hp != 0)
        break;
      MESSAGE_PUSH(MsgRevived);
      target->hp = target->maxHp * 8 / 10;
      Unit_UpdateRatiosFar(tid);
      break;

    case 73:
      if (target->hp != 0)
        break;
      MESSAGE_PUSH(MsgRevived);
      target->hp = (s16)(target->maxHp * 6 / 10);
      Unit_UpdateRatiosFar(tid);
      break;

    case 3:
      if (target->poison != 0)
        MESSAGE_PUSH(MsgCurePoison);
      target->poison = 0;
      break;

    case 17:
      RES_STEP(-1, (bak->resistanceStage - target->resistanceStage) * 20,
               MsgResDown);
      break;

    case 16: {
      target->resistanceStage -= 2;
      if (-4 > target->resistanceStage)
        target->resistanceStage = -4;
      if (target->resistanceStage > 4)
        target->resistanceStage = 4;
      BattleEvent_Push(BATTLE_EVENT_NUMBER,
                       (bak->resistanceStage - target->resistanceStage) * 20);
      MESSAGE_PUSH(MsgResDown);
      target->resistanceTurns = 7;
    } break;

    case 15:
      RES_STEP(1, (target->resistanceStage - bak->resistanceStage) * 20,
               MsgResUp);
      break;

    case 14: {
      target->resistanceStage += 2;
      if (*(rm = &target->resistanceStage) < -4)
        target->resistanceStage = -4;
      if (target->resistanceStage > 4)
        target->resistanceStage = 4;
      BattleEvent_Push(BATTLE_EVENT_NUMBER,
                       (target->resistanceStage - bak->resistanceStage) * 20);
      MESSAGE_PUSH(MsgResUp);
      target->resistanceTurns = 7;
    } break;

    case 18:
      if (target->poison != 0)
        break;
      MESSAGE_PUSH(MsgPoisoned);
      target->poison = 1;
      break;

    case 19:
      if (target->poison > 1)
        break;
      MESSAGE_PUSH(MsgVenom);
      target->poison = 2;
      break;

    case 20:
    case 75:
      STAT_SET(target->delusion, MsgDelusion);
      break;

    case 21:
      STAT_SET(target->confusion, MsgConfused);
      break;

    case 22:
      STAT_SET(target->charm, MsgCharmed);
      break;

    case 23:
    case 85:
      STAT_SET(target->stun, MsgStunned);
      break;

    case 24:
      STAT_SET(target->sleep, MsgAsleep);
      break;

    case 25:
      MESSAGE_SIDE(MsgPsyBlock, MsgPsySeal);
      target->psySeal |= 7;
      break;

    case 67:
      MESSAGE_SIDE(MsgPsyBlock, MsgPsySeal);
      target->psySeal |= 16;
      break;

    case 27:
      if (BattleUnit_KeepsOneHp(tid) != 0)
        break;
      BattleEvent_Push(BATTLE_EVENT_DEFEAT, tid);
      if (target->side == ALLY)
        MESSAGE_PUSH(MsgKoDown);
      else if (abilityId == 219)
        MESSAGE_PUSH(MsgSuffocate);
      else
        MESSAGE_PUSH(MsgSpiritDrain);
      target->hp = 0;
      Unit_UpdateRatiosFar(tid);
      break;

    case 29:
      MESSAGE_PUSH(MsgHpRegen);
      target->refrain = 5;
      break;

    case 30:
      STAT_SET(target->reflect, MsgReflect);
      break;

    case 31:
    case 60:
      ap = dealt;
      dmg = actor->hp;
      /* FAKEMATCH(barrier): an empty asm on dmg, so its copy comes before
       * dealt is reloaded from the stack; without it, 2 of 3451 instructions
       * differ in tla-en. */
      __asm__("" : "=r"(dmg) : "0"(dmg));
      if (action->effect == 60)
        ap /= 2;
      kind = ap;
      dmg += kind;
      if (dmg > actor->maxHp) {
        dmg = actor->maxHp;
        kind = dmg - actor->hp;
      }
      BattleEvent_Push(BATTLE_EVENT_CLEAR_ARGUMENTS, 0);
      BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, aid);
      if (dmg == actor->maxHp)
        MESSAGE_PUSH(MsgHpFull);
      else {
        BattleEvent_Push(BATTLE_EVENT_NUMBER, kind);
        MESSAGE_PUSH(MsgHpRecover);
      }
      actor->hp = dmg;
      Unit_UpdateRatiosFar(aid);
      break;

    case 32: {
      s32 heal;
      s32 amt;

      heal = actor->pp;
      amt = dealt;
      heal += amt;
      if (heal > actor->maxPp) {
        heal = actor->maxPp;
        amt = heal - actor->pp;
      }
      BattleEvent_Push(BATTLE_EVENT_CLEAR_ARGUMENTS, 0);
      BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, aid);
      if (heal == actor->maxPp)
        MESSAGE_PUSH(MsgPpFull);
      else {
        BattleEvent_Push(BATTLE_EVENT_NUMBER, amt);
        MESSAGE_PUSH(MsgPpRecover);
      }
      actor->pp = heal;
      Unit_UpdateRatiosFar(aid);
      break;
    }

    case 69:
      dmg = dealt / 10;
      if (target->pp < dmg)
        dmg = target->pp;
      if (actor->pp + dmg > actor->maxPp)
        dmg = actor->maxPp - actor->pp;
      if (dmg == 0)
        break;
      BattleEvent_Push(BATTLE_EVENT_NUMBER, dmg);
      MESSAGE_SIDE(MsgLeechTake, MsgLeechGain);
      Unit_AddPpFar(aid, dmg);
      break;

    case 84:
      dmg = (s16)(target->maxPp / 10);
      if (target->pp < dmg)
        dmg = target->pp;
      if (dmg == 0)
        break;
      BattleEvent_Push(BATTLE_EVENT_NUMBER, dmg);
      MESSAGE_SIDE(MsgMorePpLossP, MsgMorePpLossE);
      Unit_AddPpFar(tid, -dmg);
      break;

    case 33:
      if (target->attackStage > 0) {
        target->attackStage = 0;
        target->attackTurns = 0;
      }
      if (target->defenseStage > 0) {
        target->defenseStage = 0;
        target->defenseTurns = 0;
      }
      if (target->resistanceStage > 0) {
        target->resistanceStage = 0;
        target->resistanceTurns = 0;
      }
      if (target->agilityStage > 0)
        target->agilityStage = 0;
      target->elementLevels[0] = 0;
      target->elementLevels[1] = 0;
      target->elementLevels[2] = 0;
      target->elementLevels[3] = 0;
      MESSAGE_PUSH(MsgBuffsReset);
      break;

    case 26:
      MESSAGE_PUSH(MsgEvilSpirit);
      target->haunt = 1;
      break;

    case 28:
    case 80:
      if (target->grimReaper == 0) {
        MESSAGE_PUSH(MsgDeathCurse);
        target->grimReaper = 7;
        break;
      }
      if (target->grimReaper <= 1)
        break;
      target->grimReaper -= 1;
      BattleEvent_Push(BATTLE_EVENT_NUMBER, target->grimReaper);
      MESSAGE_PUSH(MsgDeathCount);
      break;

    case 66:
      MESSAGE_PUSH(MsgReadies);
      target->ready = 2;
      break;

    case 54:
      MESSAGE_PUSH(MsgChallenge);
      target->challenge = 1;
      if (ALLY_CHECK(tid))
        work->challengeFlag |= 2;
      break;

    case 53:
    case 83:

      MESSAGE_PUSH(MsgImmobile);
      target->immobile = 1;
      break;

    case 46:

      MESSAGE_PUSH(MsgAura);
      if (target->guardLevel > 0)
        break;
      target->guardLevel = 1;
      break;

    case 72:
      MESSAGE_PUSH(MsgAura);
      if ((u32)target->guardLevel > 1)
        break;
      target->guardLevel = 2;
      break;

    case 47:
    case 88:
      MESSAGE_PUSH(MsgAura2);
      if (target->guardLevel > 2)
        break;
      target->guardLevel = 3;
      break;

    case 79:
      MESSAGE_PUSH(MsgVanished);
      if ((u32)target->guardLevel > 3)
        break;
      target->guardLevel = 4;
      break;

    case 45:
      BattleEvent_Push(BATTLE_EVENT_MESSAGE, -1);
      break;

    case 81:
      MESSAGE_PUSH(MsgAllResting);
      work->unk046[1] = 1;
      break;

    case 82:
      MESSAGE_PUSH(MsgBlownAway);
      BattleEvent_Push(15, tid);
      break;

    case 74:
      MESSAGE_PUSH(MsgCounterReady);
      target->equipBonus[1] = 1;
      break;

    default:
      break;
    }
  }

done:
  BattleEvent_Push(BATTLE_EVENT_CLEAR_ARGUMENTS, 0);
  if (plan->command != 9 && (s8)target->equipBonus[1] != 0 &&
      action->targetMode == 1 && target->hp != 0) {
    work->unk858 = tid;
    work->unk85c = aid;
  }

  if (target->hp != 0) {
    if (target->sleep != 0)
      if (target->sleep <= 6 && dealt > 0 && (3 & Battle_RandomFar()) == 0) {
        target->sleep = 0;
        BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, tid);
        MESSAGE_PUSH(MsgWakes);
      }
  }
  Heap_Release(bak);
  Unit_CalculateFar(tid);
  PartyStatus_DrawFar(Ram_HeapSlots->battleWork->partyStatus);
  if (target->hp != 0)
    BattleEvent_Push(BATTLE_EVENT_REFRESH_UNIT, tid);
  if (abilityId != 403 && actor->haunt != 0 && (Battle_RandomFar() & 3) == 0 &&
      dealt > 0) {
    s32 share;

    share = dealt >> 2;
    if (share == 0)
      share = 1;
    plan->hauntDamage += share;
  }
}

// Battle: test whether an ability uses PP.

/// Returns 1 for abilities that cost PP, and for Psy Drain whatever its cost.
/// abilityId: ability ID.
u32 Ability_IsPsynergy(s32 abilityId) {
  u8 pp;

  if (abilityId == 126) // Psy Drain.
    return 1;
  pp = BattleAction_Get(abilityId)->ppCost;
  return (u32)((0 - pp) | pp) >> 31; // 1 if pp is nonzero.
}

#include "game/item.h"

struct AIState {
  unsigned initialized : 1;
  unsigned selection : 3;
  unsigned form : 2;
  unsigned remaining : 26;
};

struct AIEnemyRest {
  u8 unk00[0x2c];
  u16 abilities[8];
};

struct AIBattleWork {
  u8 unk000[0x564];
  u16 queuedSummons[2];
  u8 queuedCharges[2];
  s8 queueHead;
  s8 queueCount;
  u8 unk56c[0x869 - 0x56c];
  u8 unk869;
  u8 unk86a[2];
  u32 aiStates[4];
};

#define AI_BATTLE ((struct AIBattleWork *)Ram_HeapSlots->battleWork)

s32 Func_0811f4d4(s32 party);
s32 Battle_FindTaggedSlotByValue(u32 id);
s32 BattleParty_CountStandbyDjinn(u32 arg0, u8 *counts);
s32 Summon_ListLearnedFar(u8 *list);

extern u8 Data_0812cc5c[];
extern u8 Data_0812cc64[];
extern u8 Data_0812cc6c[];
extern u16 Data_08128960[];

s32 Func_08122514(struct BattleActionRecord *request, s32 retry) {
  struct BattleUnit *unit;
  u32 selection;
  struct AIState *selectionMemory;
  const struct EnemyDefinition *enemy;
  s32 itemsAllowed;
  s32 failedChoices;
  s32 needsChoice;
  const struct AIEnemyRest *halfwords;
  const s8 *actionStrategy;
  const u8 *itemActionMask;
  s16 command;
  s32 allyTarget;
  s32 opponentTarget;
  s32 allyIsParty;
  s32 opponentIsParty;
  struct AIBattleWork *battle;
  struct ItemDefinition *item;
  s32 useItem;
  s8 strategyKind;
  struct BattleAct *ability;
  s32 abilityId;
  u8 counts[4];
  u8 summons[36];
  s32 summonIndex;
  s32 element;
  const struct SummonDefinition *summon;

  s32 Func_081224d8(u8 *weights) {
    s32 roll;
    s32 total;
    s32 selectedAction;
    s32 actionIndex;

    roll = Battle_RandomFar() & 0xff;
    total = weights[0];
    selectedAction = 0;
    actionIndex = 0;
    if (roll >= total) {
      while (++actionIndex <= 7) {
        total += weights[actionIndex];
        if (roll < total) {
          selectedAction = actionIndex;
          break;
        }
      }
    }
    return selectedAction;
  }

  unit = Unit_Get__f11f66c(request->unitId);
  itemsAllowed = 1;
  failedChoices = 0;
  needsChoice = 1;
  selection = -1U;
  if (unit->classIndex != 0)
    return -1;

  selectionMemory = (struct AIState *)&unit->rewards;
  enemy = Enemy_GetDefinitionFar(unit->enemyId);
  if (unit->enemyId == 101) {
    selectionMemory =
        (struct AIState *)&((struct AIBattleWork *)Ram_HeapSlots->battleWork)
            ->aiStates[request->unk0e];
    if (request->unk0e == 1)
      enemy = Enemy_GetDefinitionFar(102);
  } else if (unit->enemyId == 369) {
    selectionMemory = (struct AIState *)&AI_BATTLE->aiStates[request->unk0e];
    switch (request->unk0e) {
    case 1:
      AI_BATTLE->unk869 = 6;
      enemy = Enemy_GetDefinitionFar(371);
      break;
    case 2:
      AI_BATTLE->unk869 = 3;
      enemy = Enemy_GetDefinitionFar(370);
      break;
    case 3:
      AI_BATTLE->unk869 = 7;
      enemy = Enemy_GetDefinitionFar(372);
      break;
    }
  } else if (unit->enemyId == 373) {
    selectionMemory = (struct AIState *)&AI_BATTLE->aiStates[request->unk0e];
    switch (request->unk0e) {
    case 1:
      AI_BATTLE->unk869 = 6;
      enemy = Enemy_GetDefinitionFar(375);
      break;
    case 2:
      AI_BATTLE->unk869 = 3;
      enemy = Enemy_GetDefinitionFar(374);
      break;
    }
  } else if (unit->enemyId == 376) {
    selectionMemory = (struct AIState *)&AI_BATTLE->aiStates[request->unk0e];
    if (request->unk0e == 1) {
      AI_BATTLE->unk869 = 3;
      enemy = Enemy_GetDefinitionFar(377);
    }
  } else if (unit->enemyId == 218) {
    if (selectionMemory->initialized == 0)
      selectionMemory->form = (Random16() * 3) >> 16;
    enemy = Enemy_GetDefinitionFar(Data_08128960[selectionMemory->form]);
  }

  if (retry != 0 && request->command != COMMAND_ABILITY) {
    switch (enemy->actionStrategy) {
    case 3:
    case 4:
      selectionMemory->selection = selectionMemory->selection + 1;
      break;
    }
    goto done;
  }

  actionStrategy = &enemy->actionStrategy;
  itemActionMask = &enemy->itemActions;
  halfwords = (const struct AIEnemyRest *)((const u8 *)enemy + 2);

select_again:
  strategyKind = *actionStrategy;
  switch (strategyKind) {
  case 0:
    selection = Func_081224d8(Data_0812cc5c);
    break;
  case 1:
    selection = Func_081224d8(Data_0812cc64);
    break;
  case 2:
    selection = Func_081224d8(Data_0812cc6c);
    break;
  case 3:
    if (selectionMemory->initialized == 0) {
      selectionMemory->selection = Battle_RandomFar();
      selectionMemory->initialized = 1;
    }
  case 4:
    selection = selectionMemory->selection;
    if (retry != 0)
      selectionMemory->selection = selection + 1;
    break;
  case 5:
    selection++;
    break;
  case 6:
    break;
  }

  useItem = (*itemActionMask >> selection) & 1;
  useItem &= itemsAllowed;
  abilityId = halfwords->abilities[selection];
  request->command = COMMAND_ABILITY;
  if (useItem != 0 && retry != 0) {
    if (!(0x1ff & unit->inventory[0])) {
      useItem = 0;
      if (enemy->targetStrategy == 0) {
        request->command = COMMAND_ITEM;
        {
          s32 cantUse = 509;

          request->parameter = cantUse;
        }
        goto done;
      }
    }
    if (useItem != 0) {
      item = Item_Get(unit->inventory[0]);
      if (item->useType != 1) {
        useItem = 0;
      } else {
        BattleAction_Get(item->actionId);
        abilityId = item->actionId;
        request->command = COMMAND_ITEM;
        request->parameter = 0;
      }
    }
    if (useItem == 0)
      itemsAllowed = 0;
  }

  if (needsChoice != 0) {
    ability = BattleAction_Get(abilityId);
    switch (ability->effect) {
    case ABILITY_EFFECT_DEFEND:
      request->command = COMMAND_DEFEND;
      request->target = Battle_FindTaggedSlotByValue(request->unitId);
      break;
    case ABILITY_EFFECT_TOTAL_DEFENSE:
      request->command = COMMAND_DEFEND2;
      request->target = Battle_FindTaggedSlotByValue(request->unitId);
      break;
    case ABILITY_EFFECT_FLEE:
      request->command = COMMAND_FLEE;
      request->target = Battle_FindTaggedSlotByValue(request->unitId);
      break;
    case 74:
    case 88:
      if (retry == 0)
        request->priority += 10000;
      break;
    }

    if (retry == 0) {
      switch (request->command) {
      case COMMAND_DEFEND:
      case COMMAND_DEFEND2:
        break;
      default:
        goto done;
      }
    }

    if (request->command != COMMAND_ITEM &&
        Ability_IsPsynergy(abilityId) != 0) {
      request->command = COMMAND_PSYNERGY;
      request->parameter = abilityId;
      if (ability->ppCost > unit->pp && enemy->targetStrategy != 0)
        goto next_candidate;
      if (unit->psySeal != 0 && enemy->targetStrategy == 2)
        goto next_candidate;
    }
    command = request->command;
    if (command == COMMAND_FLEE && (u8)unit->challenge != 0)
      goto next_candidate;
    if (retry != 0 && (command == COMMAND_DEFEND || command == COMMAND_DEFEND2))
      goto next_candidate;

    if (command == COMMAND_ABILITY) {
      request->parameter = abilityId;
      if (abilityId == 1)
        request->command = COMMAND_ATTACK;
      if (abilityId == 729) {
        request->command = COMMAND_SUMMON;
        BattleParty_CountStandbyDjinn(0, counts);
        summonIndex = Summon_ListLearnedFar(summons);
        /* FAKEMATCH(dead): an empty once-only block after the summons are
         * listed, so the djinn count's 0 argument is set after counts' address;
         * without it, 2 of 755 instructions differ in tla-en. */
        do {
        } while (0);
        for (summonIndex--; summonIndex >= 0; summonIndex--) {
          summon = SummonDefinition_Get(summons[summonIndex]);
          for (element = 0; element <= 3 &&
                            summon->djinnRequired[element] <= counts[element];
               element++)
            ;
          if (element == 4)
            break;
        }
        if (summonIndex < 0)
          goto next_candidate;
        request->parameter = summons[summonIndex];
        abilityId = SummonDefinition_Get(summons[summonIndex])->nameMessageId;
        ability = BattleAction_Get(abilityId);
      }
    }

    request->range = ability->range;
    switch (ability->targetMode) {
    case 2:
    case 4:
      if (abilityId == 723 || abilityId == 731)
        allyTarget = Func_0811f66c(128, ability);
      else
        allyTarget = Func_0811f66c(request->unitId, ability);
      if (allyTarget == -2) {
        allyIsParty = 0;
        if ((u16)request->unitId <= 7)
          allyIsParty = 1;
        allyTarget = Func_0811f4d4(allyIsParty);
      }
      if (allyTarget != -1) {
        request->target = allyTarget;
        needsChoice = 0;
      }
      break;
    case 1:
      opponentTarget = Func_0811f66c(request->unitId, ability);
      if (opponentTarget == -2) {
        opponentIsParty = 0;
        if ((u16)request->unitId <= 7)
          opponentIsParty = 1;
        opponentTarget = Func_0811f4d4(opponentIsParty);
      }
      if (opponentTarget != -1) {
        request->target = opponentTarget;
        needsChoice = 0;
      }
      break;
    case 3:
      request->target = Battle_FindTaggedSlotByValue(request->unitId);
      break;
    default:
      if (ability->effect == 86 &&
          !Enemy_CheckRoom(unit->enemyId == 164 ? 378 : 81))
        break;
      battle = (struct AIBattleWork *)Ram_HeapSlots->battleWork;
      if (ability->effect == 87) {
        if (battle->queueCount == 0)
          break;
        if (!Enemy_CheckRoom(battle->queuedSummons[battle->queueHead]))
          break;
      }
      request->target = Battle_FindTaggedSlotByValue(request->unitId);
      needsChoice = 0;
      break;
    }
  }
  if (retry == 0)
    needsChoice = 0;
next_candidate:
  if (needsChoice != 0 && failedChoices > 16) {
    request->command = COMMAND_DEFEND;
    needsChoice = 0;
  }
  failedChoices++;
  if (needsChoice != 0)
    goto select_again;
done:
  if (unit->enemyId == 218 && retry == 1 && selectionMemory->selection == 0)
    selectionMemory->form = (selectionMemory->form + 1) % 3;
  return 0;
}

#include "dma.h"

u32 Math_UnsignedMulHigh(u32 a, u32 b);

void Func_08122c18(void) {
  Math_UnsignedMulHigh(5, 0);
  Dma_Set(0, 0, DMA_COPY_WORDS(0), REG_DMA3);
  Dma_Set(0, 0, DMA_COPY_WORDS(0), REG_DMA3);
  Dma_Set(0, 0, DMA_COPY_WORDS(0), REG_DMA3);
}

void BattlePlayback_Start(s32 cue) {
  struct BattleEventState *ev;

  ev = &Ram_HeapSlots->battleWork->events;
  if (ev->phase == BATTLE_PLAYBACK_IDLE) {
    ev->phase = BATTLE_PLAYBACK_RESOLVE_TARGET;
    if (cue != 0) {
      if (ev->unk16a == 0)
        ev->pendingCue = cue;
    }
  }
}

s32 BattlePlayback_SchedulePhase(s32 frames) {
  struct BattleWork *work;
  struct BattleEventState *ev;

  work = Ram_HeapSlots->battleWork;
  ev = &work->events;
  ev->queue.count = 0;
  ev->eventIndex = 0;
  ev->timer = frames;
  ev->phase = BATTLE_PLAYBACK_DISPATCH;
  work->unk670[0x0d] = 0;
  return Task_Add(BattleEvent_Playback, TASK_PASS_VBLANK);
}

#include "vram_tab.h"

#define OBJ_VRAM 0x06010000
#define KEYS_HELD (*(volatile u32 *)&gInput.held)

struct AnimationRecord {
  u8 unk00[0x10];
  u8 slot;
  u8 unk11[3];
  u8 width;
  u8 height;
};

struct ArrowSprite {
  void *next;
  u16 y : 8;
  u16 flags : 8;
  u16 x : 9;
  u16 size : 7;
  u16 tile : 10;
  u16 palette : 6;
  u16 affineParam;
};

struct PlaybackEvents {
  struct BattleEventQueue queue;
  s32 phase;
  s32 eventIndex;
  s32 timer;
  struct ArrowSprite arrow;
  s32 arrowResource;
  s32 actorId;
  s16 pendingCue;
  s16 unk16a;
  s32 flags;
};

struct PlaybackBattleWork {
  u8 unk000[0x54];
  s32 arrowTiles;
  u8 unk058[0x67c - 0x58];
  struct BattlePlan plan;
  u8 unk6e0[4];
  struct PlaybackEvents events;
};

struct MessageWindow {
  u8 unk00[0x0c];
  s16 x;
  s16 y;
};

struct MessageOffset {
  u8 unk00[4];
  u16 x;
  u16 y;
};

struct BattleMessageDisplay {
  struct MessageWindow *window;
  struct MessageOffset *offset;
  s32 marked;
};

struct ChildEntry {
  u8 unk00[5];
  u8 colorMode;
  u8 unk06[16];
  u8 frame;
};

struct MotionRecord {
  u8 unk00[40];
  struct ChildEntry *entries[1];
};

extern u32 gVBlankCount;

void Func_080381d0(s32 message);
void Animation_ApplyChildArgumentFar(void *record, s32 value);
u32 BattleClass_GetDefeatSoundKind(s32 index);
void Map_RenderAnimatedTilePictureFar(void *record, s32 step);

void BattleEvent_Playback(void) {
  u16 selection[2];
  struct AnimationRecord *records[4];
  struct BattleMessageDisplay *display;
  struct PlaybackBattleWork *battle =
      (struct PlaybackBattleWork *)Ram_HeapSlots->battleWork;
  struct PlaybackEvents *events = &battle->events;

  void Func_08122cd0(struct AnimationRecord * record, s32 unused) {
    u8 *tiles = (u8 *)OBJ_VRAM + gResourceTableEntries[record->slot].offset;

    Iwram_ClearWords(tiles, record->width * record->height);
  }

  if (battle->events.phase == BATTLE_PLAYBACK_IDLE)
    return;
  for (;;) {
    if (events->phase == BATTLE_PLAYBACK_READY)
      return;
    if (events->phase == BATTLE_PLAYBACK_RESOLVE_TARGET) {
      if (events->queue.targetIndex < battle->plan.targetCount) {
        events->queue.count = 0;
        events->eventIndex = 0;
        events->timer = 0;
        BattleTarget_Execute(&battle->plan, events->queue.targetIndex);
        events->queue.targetIndex++;
        events->phase = BATTLE_PLAYBACK_DISPATCH;
      } else {
        events->phase = BATTLE_PLAYBACK_READY;
      }
    } else if (events->phase == BATTLE_PLAYBACK_DISPATCH) {
      while (events->eventIndex < events->queue.count) {
        {
          s32 eventIndex = events->eventIndex;

          if (events->phase != BATTLE_PLAYBACK_DISPATCH)
            break;
          if (events->timer != 0) {
            events->timer--;
            return;
          }
          switch (events->queue.opcodes[eventIndex]) {
          case BATTLE_EVENT_SOUND:
            Sound_PlayFar(events->queue.operands[eventIndex]);
            break;
          case BATTLE_EVENT_DJINN_BONUS:
            BattleEvent_GrantDjinnBonus(events,
                                        events->queue.operands[eventIndex]);
            break;
          case BATTLE_EVENT_UNIT_NAME:
            MessageArgument_PushFar(events->queue.operands[eventIndex], 1);
            break;
          case BATTLE_EVENT_NUMBER:
            MessageArgument_PushFar(events->queue.operands[eventIndex], 5);
            break;
          case BATTLE_EVENT_ITEM_NAME:
            MessageArgument_PushFar(events->queue.operands[eventIndex] & 0x1ff,
                                    2);
            break;
          case BATTLE_EVENT_ABILITY_NAME:
            MessageArgument_PushFar(events->queue.operands[eventIndex] & 0x3fff,
                                    4);
            break;
          case BATTLE_EVENT_MARK:
            ((struct BattleMessageDisplay *)Ram_HeapSlots->unk84[4])->marked =
                1;
            break;
          case BATTLE_EVENT_MESSAGE:
            if ((s32)events->queue.operands[eventIndex] >= 0)
              Func_080381d0(events->queue.operands[eventIndex]);
            events->phase = 3;
            gInput.pendingPressed = 0;
            break;
          case BATTLE_EVENT_MESSAGE_NO_WAIT:
            if ((s32)events->queue.operands[eventIndex] >= 0)
              Func_080381d0(events->queue.operands[eventIndex]);
            events->phase = 13;
            break;
          case BATTLE_EVENT_CLEAR_ARGUMENTS:
            MessageArgument_ClearAllFar();
            break;
          case BATTLE_EVENT_REMOVE_UNIT:
            BattleActor_DestroyTemporaryObject(
                events->queue.operands[eventIndex]);
            break;
          case BATTLE_EVENT_HIT:
            if (events->pendingCue > 0)
              Sound_PlayFar(events->pendingCue);
            events->actorId = events->queue.operands[eventIndex];
            Actor_SetMode(BattleObject_Get(events->actorId)->object, 5);
            events->phase = 10;
            events->timer = 0;
            break;
          case BATTLE_EVENT_DEFEAT: {
            struct BattleUnit *unit;
            void *record;
            s32 recordIndex;

            events->actorId = events->queue.operands[eventIndex];
            Func_0812824c(events->actorId, events->flags);
            BattleActor_ResetRuntimeFields(events->actorId);
            unit = Unit_Get__f11f66c(events->actorId);
            for (recordIndex = 0;
                 (record =
                      GetMotionRecord(BattleObject_Get(events->actorId)->object,
                                      recordIndex)) != 0;
                 recordIndex++) {
              if (unit->side != 1)
                Animation_ApplyChildArgumentFar(record, 4);
              else
                Animation_ApplyChildArgumentFar(record, 5);
            }
            if (unit->side == 1) {
              events->phase = 11;
              events->timer = 0;
            }
            break;
          }
          case BATTLE_EVENT_DRAW_STATUS:
            PartyStatus_DrawFar(Ram_HeapSlots->battleWork->partyStatus);
            break;
          case BATTLE_EVENT_REFRESH_UNIT: {
            struct BattleObj *slot;

            BattleUnit_BuildStatusFlags(
                events->queue.operands[eventIndex],
                BattleObject_Get(events->queue.operands[eventIndex]));
            slot = BattleObject_Get(events->queue.operands[eventIndex]);
            BattleMotion_SetRecordChildValues(
                slot->object, BattleMotion_GetSlotPalette(
                                  events->queue.operands[eventIndex]));
            BattleObject_UpdatePose(events->queue.operands[eventIndex]);
            break;
          }
          case 15:
            BattleActor_RemoveFromLists(events->queue.operands[eventIndex]);
            break;
          }
          events->eventIndex++;
        }
      }
      if (events->phase == BATTLE_PLAYBACK_DISPATCH)
        events->phase = BATTLE_PLAYBACK_RESOLVE_TARGET;
    } else if (events->phase == 3 || events->phase == 13) {
      if (Message_AllTypedFar() == 0)
        return;
      if (events->phase == 13) {
        events->phase = BATTLE_PLAYBACK_DISPATCH;
        events->timer = 0;
      } else {
        events->phase = 5;
        events->arrowResource = -1;
        events->timer = gVBlankCount;
      }
    } else if (events->phase == 5) {
      struct ArrowSprite *prompt = &events->arrow;
      u8 *arrowTiles = Data_0812996c;
      s32 pictureTiles = (s32)arrowTiles + ((gFrameCount >> 2) & 7) * 128;
      struct BattleMessageDisplay *display =
          (struct BattleMessageDisplay *)Ram_HeapSlots->unk84[4];
      struct MessageWindow *window = display->window;
      struct MessageOffset *offset = display->offset;
      s32 x = 0;

      if (events->arrowResource == -1)
        events->arrowResource = battle->arrowTiles;
      MessageArgument_ClearAllFar();
      QueueIoWriteDelay10(0x0400004a, 4);
      QueueIoWriteDelay6(0x0400004a, 16);
      ((u32 *)prompt)[1] = 0xa000;
      ((u32 *)prompt)[2] = x;
      /* FAKEMATCH(barrier): an empty asm after x is stored, so prompt's copy
       * (mov r1, r9) comes before x's (mov r2, r8); without it, 4 of 844
       * instructions differ in tla-en. */
      asm("");
      prompt->tile = VramCache_Reload(events->arrowResource, pictureTiles);
      x = window->x * 8 + (offset->x >> 8) + 4;
      prompt->x = x;
      prompt->y = Trig_Sin(gFrameCount << 12) / 32768 + window->y * 8 +
                  (offset->y >> 8) + 6;
      // The held keys reread through a volatile pointer for the last test, a
      // load apart from the B test's; as gInput.held, 16 of 844 instructions
      // differ in tla-en.
      if ((gInput.held & 2) || (gInput.pendingPressed & 0x303) ||
          ((u32)(gVBlankCount - events->timer) > 10 && (KEYS_HELD & 0x303))) {
        Sound_PlayFar(111);
        events->phase = BATTLE_PLAYBACK_DISPATCH;
        events->timer = 0;
      } else {
        Oam_AddSpriteAtDepth(prompt, 240);
        return;
      }
    } else if (events->phase == 10) {
      if (gLastFrameVBlanks)
        events->timer |= 1;
      if (events->timer & 1) {
        if (events->timer & 2) {
          struct BattleObj *slot;

          selection[0] = 255;
          slot = BattleObject_Get(events->actorId);
          BattleMotion_SetRecordChildValues(
              slot->object, BattleMotion_GetSlotPalette(events->actorId));
        } else {
          selection[0] = events->actorId;
          selection[1] = 255;
          BattleMotion_SetRecordChildValues(
              BattleObject_Get(events->actorId)->object, 7);
        }
        BattleLayout_HighlightPartyPanelsFar(selection);
      }
      events->timer++;
      if (events->timer <= 7)
        return;
      events->phase = BATTLE_PLAYBACK_DISPATCH;
      events->timer = 0;
    } else if (events->phase == 11) {
      if (events->timer == 0 || events->timer >= 0x400) {
        s32 colorMode = 6;

        if (events->timer == 0 && events->flags != 0) {
          s32 kind = BattleClass_GetDefeatSoundKind(
              ((struct BattleUnit *)Unit_Get__f11f66c(events->actorId))->enemyId);

          if (kind >= 0) {
            kind--;
            if (kind < 0)
              kind = 0;
            Sound_PlayFar(146 + kind);
          }
          events->timer = 0x400;
        }
        if (events->timer > 0x400 + 30 - 1)
          events->timer = 0;
        if (events->timer == 0) {
          s32 kind = BattleClass_GetDefeatSoundKind(
              ((struct BattleUnit *)Unit_Get__f11f66c(events->actorId))->enemyId);

          if (kind >= 0)
            Sound_PlayFar(146 + kind);
        }
        if (events->timer >= 0x400)
          colorMode = (events->timer - 0x400) / 8 % 5 + 1;
        if (colorMode == 6 || (events->timer & 7) == 0) {
          struct MotionRecord *record;
          s32 recordIndex;
          struct MotionRecord **slot;
          s32 lastFrameMask;

          for (recordIndex = 0, slot = (struct MotionRecord **)records,
              lastFrameMask = 0xff;
               (record =
                    GetMotionRecord(BattleObject_Get(events->actorId)->object,
                                    recordIndex)) != 0;
               recordIndex++) {
            struct ChildEntry *child;
            u8 previousFrame;

            *slot++ = record;
            child = record->entries[0];
            previousFrame = child->frame;
            child->colorMode = colorMode;
            child->frame = previousFrame | lastFrameMask;
          }
        }
        events->timer++;
        return;
      }
      if (events->timer == 4) {
        BattleActor_RemoveFromLists(events->actorId);
        events->timer++;
        return;
      }
      if (events->timer > 4) {
        struct BattleObj *slot;
        void *record;
        s32 count;
        s32 step;
        s32 base;

        slot = BattleObject_Get(events->actorId);
        slot->fading = 1;
        for (count = 0; (record = GetMotionRecord(slot->object, count)) != 0;
             count++)
          records[count] = record;
        base = events->timer * 4;
        step = base - 20;
        if (step > 127) {
          s32 recordIndex;

          for (recordIndex = 0; recordIndex < count; recordIndex++)
            Func_08122cd0(records[recordIndex], 0);
          BattleObject_MarkForRelease(events->actorId);
          events->phase = BATTLE_PLAYBACK_DISPATCH;
          events->timer = 0;
          return;
        }
        {
          s32 recordIndex;

          for (recordIndex = 0; recordIndex < count; recordIndex++) {
            Map_RenderAnimatedTilePictureFar(records[recordIndex], step);
            Map_RenderAnimatedTilePictureFar(records[recordIndex], base - 19);
            Map_RenderAnimatedTilePictureFar(records[recordIndex], base - 18);
            Map_RenderAnimatedTilePictureFar(records[recordIndex], base - 17);
          }
        }
        events->timer++;
        return;
      }
      events->timer++;
      return;
    }
  }
}

u32 BattlePlayback_Reset(void) {
  struct BattleEventState *events;
  struct BattleWork *work;

  work = Ram_HeapSlots->battleWork;
  events = &work->events;
  events->phase = BATTLE_PLAYBACK_IDLE;
  events->queue.count = 0;
  events->eventIndex = 0;
  events->timer = 0;
  events->queue.targetIndex = 0;
  events->pendingCue = 0x86;
  events->unk16a = 0;
  events->flags = 0;
}

s32 BattlePlayback_WaitForReady(void) {
  s32 state;
  struct BattleEventState *events;

  events = &Ram_HeapSlots->battleWork->events;
  state = events->phase;
  if (state == BATTLE_PLAYBACK_IDLE) {
    events->phase = BATTLE_PLAYBACK_RESOLVE_TARGET;
    state = BATTLE_PLAYBACK_RESOLVE_TARGET;
  }
  if (state != BATTLE_PLAYBACK_READY) {
    do {
      wait(1U);
    } while (events->phase != BATTLE_PLAYBACK_READY);
  }
  Task_Remove((u32)BattleEvent_Playback);
  return BattlePlayback_Reset();
}

// Battle: test whether a unit is in a battle formation.

/// Returns 1 if the unit is in its side's formation.
/// id: unit ID (0-7 allies, all others enemies).
u32 BattleParty_IsUnitListed(u32 id) {
  u16 list[8];
  s32 side;
  s32 cnt;
  s32 i;

  side = BATTLE_SIDE_PARTY;
  if (id > 7)
    side = BATTLE_SIDE_ENEMIES;
  cnt = BattleParty_ListActorIds(side, list);

  for (i = 0; i < cnt; i++) {
    if (list[i] == id)
      break;
  }

  return i != cnt;
}

struct PlacementList {
  struct DjinnRecoveryEntry entries[72];
  s32 count;
};
struct PlacementTable {
  u8 padding[8];
  struct PlacementList list;
};

s32 BattleParty_CountStandbyDjinn(u32 arg0, u8 *counts) {
  u16 values[8];
  struct PlacementList *list;
  s32 total;
  s32 found;
  s32 i;
  s32 j;
  s32 kind;
  s32 owner;

  found = 0;
  kind = 1;
  if (arg0 > 7)
    kind = 2;
  total = BattleParty_ListActorIds(kind, (u16 *)values);
  owner = 0;
  if (arg0 > 7)
    owner = 1;
  list = &Djinn_GetRecoveryTableFar(owner)->list;
  if (counts != 0)
    for (j = 3; j >= 0; j--)
      counts[j] = 0;
  i = 0;
  if (list->count != 0) {
    do {
      if (list->entries[i].turns == -1) {
        for (j = 0; j < total; j++)
          if (values[j] == list->entries[i].unitId)
            break;
        if (j != total) {
          if (counts != 0)
            counts[list->entries[i].element]++;
          found++;
        }
      }
      i++;
    } while (i != list->count);
  }
  return found;
}

struct ActionRunBattleWork {
  u8 unk000[0x44];
  u8 twoSided;
  u8 unk045[0x58 - 0x45];
  s16 partyUnits[7];
  s16 enemyIds[7];
  u8 unk074[0x862 - 0x74];
  u16 summonedMask;
  u8 unk864;
  u8 summonCount;
};

struct DjinnRecoveryEntryDraft {
  u8 element;
  u8 index;
  u8 unitId;
  s8 turns;
};

struct DjinnRecoveryListDraft {
  struct DjinnRecoveryEntryDraft entries[72];
  s32 count;
};

extern const u32 Data_08128c50[];
extern u8 Data_08128972[];
extern u8 Data_0812896c[];

s32 Battle_GetTaggedSlotValue(s32 tag);
s32 Unit_AddHpFar(s32 owner, s32 delta);
s32 GameFlag_Test(s32 flag);
s32 Inventory_GetEquippedItemFar(struct BattleUnit *owner, s32 slot);
s32 Djinn_GetDefinitionHeaderFar(s32 element, s32 index);
s32 Djinn_IsActiveFar(s32 unitId, s32 element, s32 index);
s32 Djinn_IsReadyFar(s32 unitId, s32 element, s32 index);
u32 *DjinnRecovery_AddStandbyFar(u32 owner, u32 element, u32 djinn);
void Func_08127308(s32 unitId, s32 element, s32 mode, s32 value);
s32 Item_GetEquippedElementFar(s32 unitId);
u32 BattleClass_GetAttackEffect(u32 index);
s32 Func_0811d79c(s32 classId);
s32 Equipment_GetUnleashRateBonusFar(struct BattleUnit *owner);

s32 Func_0812381c(struct BattleActionRecord *request, struct BattlePlan *plan) {
  struct BattleUnit *actor = Unit_Get__f11f66c(request->unitId);
  struct ActionRunBattleWork *battle =
      (struct ActionRunBattleWork *)Ram_HeapSlots->battleWork;
  s32 selectedTargetId;
  s32 abilityId;

  s32 Func_08123648(s32 abilityId) {
    struct BattleAct *ability = BattleAction_Get(abilityId);
    s32 mode = ability->targetMode;
    s32 includeDowned = 0;
    s32 targetCount;
    s32 partyCount;
    s32 enemyCount;
    s32 aimedPlace;
    s32 range;
    s32 place;
    s32 targetId;
    s32 pass;

    if (Battle_IsReviveEffectFar(ability->effect))
      includeDowned = 1;
    switch (mode) {
    case 0:
      plan->targetCount = 1;
      plan->targetOffset[0] = mode;
      plan->targetIds[0] = selectedTargetId;
      plan->targetMultiplier[0] = 1;
      break;
    case 4:
      plan->targetOffset[0] = 0;
      plan->targetCount = 1;
      plan->targetIds[0] = selectedTargetId;
      plan->targetMultiplier[0] = 1;
      break;
    default:
      targetCount = 0;
      range = request->range;
      for (pass = 0; pass <= 1; pass++) {
        for (place = 0; battle->partyUnits[place] != 255; place++)
          ;
        partyCount = place;
        for (place = 0; battle->enemyIds[place] != 255; place++)
          ;
        enemyCount = place;
        aimedPlace = request->target & 15;
        {
          s32 first = aimedPlace - range + 1;
          s32 last = aimedPlace + range - 1;

          for (place = first; place <= last; place++) {
            if (place < 0)
              continue;
            if (request->target & 128) {
              if (place >= enemyCount)
                continue;
              targetId = battle->enemyIds[place];
              if (targetId == 254)
                continue;
              if (!includeDowned &&
                  ((struct BattleUnit *)Unit_Get__f11f66c(targetId))->hp == 0)
                continue;
              plan->targetMultiplier[targetCount] = 1;
              plan->targetOffset[targetCount] = place - aimedPlace;
              plan->targetIds[targetCount] = targetId;
              targetCount++;
            } else {
              if (place >= partyCount)
                continue;
              targetId = battle->partyUnits[place];
              if (targetId == 254)
                continue;
              if (!includeDowned &&
                  ((struct BattleUnit *)Unit_Get__f11f66c(targetId))->hp == 0)
                continue;
              plan->targetMultiplier[targetCount] = 1;
              plan->targetOffset[targetCount] = place - aimedPlace;
              plan->targetIds[targetCount] = targetId;
              targetCount++;
            }
          }
        }
        plan->targetCount = targetCount;
        if (targetCount <= 0) {
          MessageArgument_PushFar(request->unitId, 1);
          Message_ShowAndWaitCoreFar((s32)&MsgActorDefends);
          if (actor->guardLevel == 0)
            actor->guardLevel = 1;
          return -1;
        }
        if (ability->effect != 75)
          break;
        range = 11;
      }
      break;
    }
    return 0;
  }

  selectedTargetId = Battle_GetTaggedSlotValue(request->target);
  BattlePlayback_Reset();
  ((s32 *)plan)[25] = 0;
  plan->hauntDamage = 0;
  plan->actorId = request->unitId;
  plan->targetCount = 0;
  plan->presentationFlags = 0;
  plan->failure = 0;
  plan->actorId2 = request->unitId;
  plan->element = 4;
  MessageArgument_ClearAllFar();
  if (actor->hp == 0)
    return -2;

  if (gDebugMode && GameFlag_Test(365) && (gInput.held & 0x100)) {
    s32 enemy = 1;
    s32 index;
    s32 id;

    // The held keys reread through a volatile pointer, a load apart from the R
    // test's; as gInput.held, 12 of 1601 instructions differ in tla-en.
    if (KEYS_HELD & 4)
      enemy = 0;
    for (index = 0;; index++) {
      if (enemy)
        id = battle->enemyIds[index];
      else
        id = battle->partyUnits[index];
      if (id == 255)
        break;
      if (id == 254)
        continue;
      if (Unit_AddHpFar(id, 0xc0000000) == 0) {
        BattleEvent_Push(BATTLE_EVENT_HIT, id);
        BattleEvent_Push(BATTLE_EVENT_DEFEAT, id);
      }
    }
    BattleEvent_DispatchQueued();
    return -2;
  }

  MessageArgument_ClearAllFar();
  if (request->command != 9) {
    if (actor->immobile) {
      actor->immobile = 0;
      MessageArgument_PushFar(request->unitId, 1);
      Message_ShowAndWaitCoreFar((s32)&MsgUnableToMove);
      return -1;
    }
    if (actor->sleep) {
      MessageArgument_PushFar(request->unitId, 1);
      Message_ShowAndWaitCoreFar((s32)&MsgIsAsleep);
      return -1;
    }
    if (actor->stun) {
      MessageArgument_PushFar(request->unitId, 1);
      Message_ShowAndWaitCoreFar((s32)&MsgIsParalyzed);
      return -1;
    }
    if ((actor->restraint & 1) && request->command != COMMAND_DEFEND &&
        (Battle_RandomFar() & 3) == 0) {
      MessageArgument_PushFar(request->unitId, 1);
      Message_ShowAndWaitCoreFar((s32)&MsgIsBound);
      return -1;
    }
  }
  if (request->command == COMMAND_NONE)
    return -2;
  abilityId = 1;
  {
    s32 index;

    for (index = 0; index < 14; index++)
      plan->targetCritical[index] = 0;
    for (index = 0; index < 14; index++)
      plan->targetHit[index] = -1;
  }

  switch (request->command) {
  case COMMAND_FLEE:
    if ((u16)request->unitId <= 7) {
      Message_ShowAndWaitCoreFar((s32)&MsgPartyFlees);
    } else {
      MessageArgument_PushFar(request->unitId, 1);
      Message_ShowAndWaitCoreFar((s32)&MsgActorRuns);
    }
    BattleMessage_WaitForAdvance();
    plan->outcome = 7;
    return 0;

  case COMMAND_ATTACK:
    abilityId = RollWeaponUnleashFar(actor);
    if (Func_08123648(abilityId) == -1)
      return -1;
    if (abilityId != 1) {
      MessageArgument_PushFar(request->unitId, 1);
      MessageArgument_PushFar(Inventory_GetEquippedItemFar(actor, 1), 2);
      Message_ShowAndWaitCoreFar((s32)&MsgWeaponHowls);
      Func_08120158();
      MessageArgument_PushFar(abilityId, 4);
      Message_ShowAndWaitCoreFar((s32)(MsgWeaponHowls + 1));
    }
    break;

  case 9:
    abilityId = 299;
    if ((u16)(actor->enemyId - 101) <= 2)
      abilityId = 1;
    if (selectedTargetId == 255)
      return -2;
    plan->targetCount = 1;
    plan->targetOffset[0] = 0;
    plan->targetIds[0] = selectedTargetId;
    plan->targetMultiplier[0] = 1;
    break;

  case COMMAND_PSYNERGY: {
    struct BattleAct *ability;
    s32 canCast = 1;

    abilityId = request->parameter;
    ability = BattleAction_Get(abilityId);
    if (Func_08123648(abilityId) == -1)
      return -1;
    MessageArgument_PushFar(request->unitId, 1);
    MessageArgument_PushFar(abilityId, 4);
    switch (abilityId) {
    case 600 ... 627:
      Message_ShowAndWaitCoreFar((s32)&MsgActorCastsSpecial);
      break;
    case 590 ... 597:
      Message_ShowAndWaitCoreFar((s32)&MsgActorCastsDjinni);
      break;
    default:
      Message_ShowAndWaitCoreFar((s32)&MsgActorCasts);
      break;
    }
    if (actor->pp < ability->ppCost) {
      plan->failure = 2;
      canCast = 0;
    }
    if (actor->psySeal) {
      plan->failure = 1;
      canCast = 0;
    }
    if (canCast) {
      plan->failure = 0;
      actor->pp -= ability->ppCost;
      Unit_UpdateRatiosFar(request->unitId);
      if (actor->pp < 0)
        actor->pp = 0;
      if (actor->pp > actor->maxPp)
        actor->pp = actor->maxPp;
    }
  } break;

  case COMMAND_ITEM: {
    struct ItemDefinition *item;

    if (request->parameter < 0) {
      MessageArgument_PushFar(request->unitId, 1);
      Message_ShowAndWaitCoreFar((s32)&MsgItemAlreadyUsed);
      return -1;
    }
    item = Item_Get(actor->inventory[request->parameter]);
    abilityId = item->actionId;
    if (abilityId == 0 || (actor->inventory[request->parameter] & 0x400)) {
      MessageArgument_PushFar(request->unitId, 1);
      Message_ShowAndWaitCoreFar((s32)&MsgActorDefends);
      if (actor->guardLevel == 0)
        actor->guardLevel = 1;
      return -1;
    }
    if (Func_08123648(abilityId) == -1)
      return -1;
    MessageArgument_PushFar(request->unitId, 1);
    MessageArgument_PushFar(actor->inventory[request->parameter], 2);
    if (item->useType == 2 || item->useType == 0) {
      switch (item->type) {
      case 1:
      case 3:
      case 6:
      case 7:
      case 8:
        Message_ShowAndWaitCoreFar((s32)&MsgActorRaisesItem);
        break;
      default:
        Message_ShowAndWaitCoreFar((s32)&MsgActorUsesBattleItem);
        break;
      }
    } else {
      Message_ShowAndWaitCoreFar((s32)&MsgActorUsesBattleItem);
    }
  } break;

  case COMMAND_DEFEND:
  case COMMAND_DEFEND2:
    MessageArgument_PushFar(request->unitId, 1);
    Message_ShowAndWaitCoreFar((s32)&MsgActorDefends);
    return -1;

  case COMMAND_NONE:
    return -2;

  case COMMAND_ABILITY: {
    s32 message;

    abilityId = request->parameter;
    if (Func_08123648(abilityId) == -1)
      return -1;
    MessageArgument_PushFar(request->unitId, 1);
    MessageArgument_PushFar(abilityId, 4);
    if ((BattleAction_Get(abilityId)->flags & 15) == 6)
      message = (s32)&MsgActorUnleashesAbility;
    else
      message = (s32)&MsgActorUsesAbility;
    switch (abilityId) {
    case 435:
    case 436:
      message = (s32)&MsgEnemyAbility0d53;
      break;
    case 224:
      message = (s32)&MsgActorCasts;
      break;
    case 500:
      message = (s32)&MsgEnemyAbility0d58;
      break;
    case 501:
      message = (s32)&MsgEnemyAbility0d59;
      break;
    case 499:
      message = (s32)&MsgEnemyAbility0d5a;
      break;
    case 494:
      message = (s32)&MsgEnemyAbility0d5b;
      break;
    case 437:
    case 438:
    case 439:
    case 440:
    case 441:
      message = (s32)&MsgEnemyAbility0d5c;
      break;
    case 442:
    case 443:
    case 444:
      message = (s32)&MsgActorUsesAbility;
      break;
    case 472:
      message = (s32)&MsgEnemyAbility0d5d;
      break;
    case 488:
      message = (s32)&MsgEnemyAbility0d5e;
      break;
    case 492:
      message = (s32)&MsgEnemyAbility0d60;
      break;
    case 495:
      message = (s32)&MsgEnemyAbility0d5f;
      break;
    case 503:
      message = (s32)&MsgEnemyAbility0d61;
      break;
    case 504:
      message = (s32)&MsgEnemyAbility0d62;
      break;
    case 508:
      message = (s32)&MsgEnemyAbility0d63;
      break;
    case 733:
      message = (s32)&MsgEnemyAbility0d65;
      break;
    }
    Message_ShowAndWaitCoreFar(message);
  } break;

  case COMMAND_DJINN:
    abilityId = Djinn_GetDefinitionHeaderFar((request->parameter >> 8) & 15,
                                             request->parameter & 255);
    if (!Djinn_IsActiveFar(request->unitId, (request->parameter >> 8) & 15,
                           request->parameter & 255)) {
      if (Djinn_IsReadyFar(request->unitId, (request->parameter >> 8) & 15,
                           request->parameter & 255)) {
        BattleAction_Get(abilityId);
        BattleHighlight_SetModes(0, 0);
        Djinn_ActivateFar(request->unitId, (request->parameter >> 8) & 15,
                          request->parameter & 255);
        DjinnRecovery_RemoveFar(request->unitId, (request->parameter >> 8) & 15,
                                request->parameter & 255);
        Unit_CalculateFar(request->unitId);
        BattlePlayback_Reset();
        BattlePlayback_SchedulePhase(30);
        BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, request->unitId);
        BattleEvent_Push(BATTLE_EVENT_ABILITY_NAME,
                         ((request->parameter >> 8) & 15) * 20 +
                             (request->parameter & 255) + 300);
        BattleEvent_Push(BATTLE_EVENT_SOUND, 175);
        BattleEvent_Push(BATTLE_EVENT_DRAW_STATUS, 0);
        BattleEvent_Push(BATTLE_EVENT_MESSAGE, (u32)&MsgDjinnIsSet);
        BattleEvent_Push(BATTLE_EVENT_REFRESH_UNIT, request->unitId);
        Sound_PlayFar(212);
        Actor_SetMode(BattleObject_Get(request->unitId)->object, 3);
        Actor_SetAnimationSpeedFar(
            (struct Actor *)BattleObject_Get(request->unitId)->object, 32);
        Func_08127308(request->unitId, (request->parameter >> 8) & 15, 3, 0);
        BattlePlayback_WaitForReady();
        return -2;
      }
      MessageArgument_PushFar(request->unitId, 1);
      MessageArgument_PushFar(abilityId, 4);
      Sound_PlayFar(114);
      Message_ShowAndWaitCoreFar((s32)&MsgDjinnInRecovery);
      wait(60);
      return -1;
    }
    {
      struct BattleAct *ability;

      if (Func_08123648(abilityId) == -1)
        return -1;
      DjinnRecovery_AddStandbyFar(request->unitId,
                                  (request->parameter >> 8) & 15,
                                  request->parameter & 255);
      ability = BattleAction_Get(abilityId);
      MessageArgument_PushFar(request->unitId, 1);
      MessageArgument_PushFar(abilityId, 4);
      Message_ShowAndWaitCoreFar((s32)&MsgActorUnleashesDjinn);
      plan->element = ability->element;
    }
    break;

  case COMMAND_SUMMON: {
    const struct SummonDefinition *summon;
    struct DjinnRecoveryListDraft *list;
    s32 side;
    u8 counts[4];
    s32 index;

    summon = SummonDefinition_Get(request->parameter);
    BattleParty_CountStandbyDjinn(
        ((struct ActionRunBattleWork *)Ram_HeapSlots->battleWork)->twoSided
            ? request->unitId
            : 0,
        counts);
    side = 0;
    if ((u16)request->unitId > 7 &&
        ((struct ActionRunBattleWork *)Ram_HeapSlots->battleWork)->twoSided)
      side = 1;
    list = (struct DjinnRecoveryListDraft *)((u8 *)Djinn_GetRecoveryTableFar(
                                                 side) +
                                             8);
    for (index = 0; index < 4 && counts[index] >= summon->djinnRequired[index];
         index++)
      counts[index] = summon->djinnRequired[index];
    abilityId = summon->nameMessageId;
    if (Func_08123648(abilityId) == -1)
      return -1;
    if (index != 4) {
      MessageArgument_PushFar(request->unitId, 1);
      MessageArgument_PushFar(abilityId, 4);
      Message_ShowAndWaitCoreFar((s32)&MsgSummonLacksDjinn);
      return -1;
    }
    MessageArgument_PushFar(request->unitId, 1);
    MessageArgument_PushFar(abilityId, 4);
    Message_ShowAndWaitCoreFar((s32)&MsgActorSummons);
    for (index = 0; index != list->count; index++) {
      struct DjinnRecoveryEntryDraft *entry = &list->entries[index];

      if (entry->turns == -1 && BattleParty_IsUnitListed(entry->unitId) &&
          counts[entry->element] != 0) {
        entry->turns = -2;
        counts[entry->element]--;
      }
    }
    if (abilityId == 402) {
      if ((u16)request->unitId > 7)
        battle->summonedMask |= 1 << battle->summonCount;
      battle->summonCount++;
    }
  } break;
  }

  if (abilityId == 1) {
    struct BattleUnit *target = Unit_Get__f11f66c(plan->targetIds[0]);

    plan->abilityId = 1;
    plan->element = Item_GetEquippedElementFar(request->unitId);
    plan->outcome = 2;
    if (actor->classIndex == 0) {
      plan->presentationFlags =
          BattleClass_GetAttackEffect(actor->enemyId) | 0x4000;
    } else {
      plan->presentationFlags = 0;
      plan->presentationFlags = Func_0811d79c(actor->enemyId);
    }
    MessageArgument_PushFar(request->unitId, 1);
    Message_ShowAndWaitCoreFar((s32)&MsgActorAttacks);
    // The rolls in a one-pass block left by break, which places them ahead of
    // the five tests; as an if on the negated tests, 119 of 1601 instructions
    // differ in tla-en.
    do {
      if (target->hp == 0 || target->sleep != 0 || target->stun != 0 ||
          target->immobile != 0 || target->charm != 0)
        break;
      if (actor->delusion != 0 && (Battle_RandomFar() & 255) <= 152)
        plan->targetMultiplier[0] = 0;
      if ((Battle_RandomFar() & 31) == 0)
        plan->targetMultiplier[0] = 0;
    } while (0);
    if (GameFlag_Test(366))
      plan->targetMultiplier[0] = 0;
    if (target->hp != 0) {
      if ((Battle_RandomFar() & 31) == 0)
        plan->targetCritical[0] = 1;
      else if ((Equipment_GetUnleashRateBonusFar(actor) << 16) / 200 >
               (Battle_RandomFar() & 0xffff))
        plan->targetCritical[0] = 1;
    }
  } else {
    struct BattleAct *ability = BattleAction_Get(abilityId);
    s32 chance;
    s32 amount;

    plan->abilityId = abilityId;
    plan->element = ability->element;
    plan->presentationFlags = 0;
    if (ability->effect == 65 || ability->effect == 41 ||
        ability->effect == 42 || ability->effect == 43 ||
        ability->effect == 44 || ability->effect == 68 ||
        ability->effect == 89) {
      s32 index;

      if (ability->effect == 89) {
        s32 roll = Battle_RandomFar() & 255;

        if (roll <= 178) {
          amount = 1;
          if (roll <= 127)
            amount = 2;
          for (index = 0; index < plan->targetCount; index++)
            plan->targetMultiplier[index] += amount;
        }
      } else {
        if (ability->effect == 65 || ability->effect == 68)
          chance = 153;
        else if (ability->effect == 41 || ability->effect == 43)
          chance = 32;
        else
          chance = 64;
        if (ability->effect == 65 || ability->effect == 41 ||
            ability->effect == 42)
          amount = 1;
        else
          amount = 2;
        if ((Battle_RandomFar() & 255) < chance) {
          for (index = 0; index < plan->targetCount; index++)
            plan->targetMultiplier[index] += amount;
        }
      }
    } else if (ability->effect >= 36 && ability->effect <= 40) {
      s32 mask;

      switch (ability->effect) {
      case 36:
        mask = 63;
        break;
      case 37:
        mask = 31;
        break;
      case 38:
        mask = 15;
        break;
      case 39:
        mask = 7;
        break;
      case 40:
      default:
        mask = 3;
        break;
      }
      if ((Battle_RandomFar() & mask) == 0) {
        s32 index;

        for (index = 0; index < plan->targetCount; index++)
          plan->targetCritical[index] = 2;
      }
    } else if (abilityId == 178 || abilityId == 374 || ability->effect == 82) {
      s32 index;

      for (index = 0; index < plan->targetCount; index++) {
        s32 rate;

        if (ability->targetMode == 1) {
          rate = Data_0812896c[__builtin_abs(plan->targetOffset[index])];
        } else {
          rate = 100;
        }
        plan->targetHit[index] = BattleEffect_RollSuccessFar(
            request->unitId, plan->targetIds[index], ability->element,
            ability->effect, rate);
      }
    }
    if (abilityId == 312) {
      plan->actorId2 = Func_0811f5a4(plan->actorId);
    } else if (abilityId == 316) {
      s32 tries = 0;

      plan->actorId2 = plan->targetIds[0];
      do {
        plan->targetIds[0] =
            Battle_GetTaggedSlotValue(Func_0811f4d4(plan->actorId2 <= 7));
        if (plan->targetIds[0] != plan->actorId2)
          break;
      } while (++tries <= 9);
    }
    if (abilityId >= 0 && abilityId <= 732) {
      plan->presentationFlags = Data_08128c50[abilityId];
      if (plan->targetMultiplier[0] > 1)
        plan->presentationFlags = Data_08128c50[abilityId] +
                                  (plan->targetMultiplier[0] << 12) - 0x1000;
    }
    if ((abilityId >= 0 && abilityId <= 732) && Data_08128972[abilityId] != 0)
      plan->outcome = Data_08128972[abilityId];
    else if (Ability_IsPsynergy(abilityId))
      plan->outcome = 3;
    else if (plan->presentationFlags != 0) {
      if (actor->classIndex == 0)
        plan->outcome = 8;
      else
        plan->outcome = 3;
    } else
      plan->outcome = 1;
    if (Battle_IsReviveEffectFar(ability->effect))
      plan->presentationFlags |= 0x10000;
    if ((abilityId == 178 || abilityId == 374 || ability->effect == 82) &&
        plan->targetHit[0] != 0)
      plan->presentationFlags |= 0x1000;
  }
  if (request->command == COMMAND_ITEM && plan->outcome != 5 &&
      plan->outcome != 9)
    plan->outcome = 4;
  plan->command = request->command;
  return 0;
}

s32 BattleUnit_RollCounterEarlyEnd(s32 unitId, s32 turns, s32 bias) {
  struct BattleUnit *state = Unit_Get__f11f66c(unitId);

  if (turns <= 5) {
    s32 threshold = ((state->luck * 3 - turns * 5) + bias) * 0x28f;
    if (threshold >= (Battle_RandomFar() & 0xffff))
      return 1;
  }
  return 0;
}

s32 BattleUnit_CountDownAttackModifier(s32 unitId) {
  struct BattleUnit *state = Unit_Get__f11f66c(unitId);

  if (state->attackTurns != 0) {
    s32 zero;
    state->attackTurns--;
    zero = 0;
    if ((s8)state->attackTurns == 0) {
      state->attackStage = zero;
      return 1;
    }
    if (state->attackStage < 0) {
      if (BattleUnit_RollCounterEarlyEnd(unitId, state->attackTurns, 30) != 0) {
        state->attackStage = zero;
        state->attackTurns = zero;
        return 1;
      }
    }
  }
  return 0;
}

s32 BattleUnit_CountDownDefenseModifier(s32 unitId) {
  struct BattleUnit *state = Unit_Get__f11f66c(unitId);

  if (state->defenseTurns != 0) {
    state->defenseTurns--;
    if (state->defenseTurns == 0) {
      state->defenseStage = 0;
      return 1;
    }
    if (state->defenseStage < 0 &&
        BattleUnit_RollCounterEarlyEnd(unitId, state->defenseTurns, 20) != 0) {
      state->defenseStage = 0;
      state->defenseTurns = 0;
      return 1;
    }
  }
  return 0;
}

s32 BattleUnit_CountDownResistanceModifier(s32 unitId) {
  struct BattleUnit *state = Unit_Get__f11f66c(unitId);

  if (state->resistanceTurns != 0) {
    state->resistanceTurns--;
    if (state->resistanceTurns == 0) {
      state->resistanceStage = 0;
      return 1;
    }
    if (state->resistanceStage < 0 &&
        BattleUnit_RollCounterEarlyEnd(unitId, state->resistanceTurns, 20) !=
            0) {
      state->resistanceStage = 0;
      state->resistanceTurns = 0;
      return 1;
    }
  }
  return 0;
}

s32 BattleUnit_CountDownDelusion(s32 unitId) {
  struct BattleUnit *state = Unit_Get__f11f66c(unitId);
  if (state->delusion != 0) {
    state->delusion--;
    if (state->delusion == 0)
      return 1;
    if (BattleUnit_RollCounterEarlyEnd(unitId, state->delusion, 30) != 0) {
      state->delusion = 0;
      return 1;
    }
  }
  return 0;
}

s32 BattleUnit_CountDownConfusion(s32 unitId) {
  struct BattleUnit *state = Unit_Get__f11f66c(unitId);
  if ((u8)state->confusion != 0) {
    (*(u8 *)&state->confusion)--;
    if (state->confusion == 0)
      return 1;
    if (BattleUnit_RollCounterEarlyEnd(unitId, (u8)state->confusion, 60) != 0) {
      state->confusion = 0;
      return 1;
    }
  }
  return 0;
}

s32 BattleUnit_CountDownCharm(s32 unitId) {
  struct BattleUnit *state = Unit_Get__f11f66c(unitId);
  if (state->charm != 0) {
    state->charm--;
    if (state->charm == 0)
      return 1;
    if (BattleUnit_RollCounterEarlyEnd(unitId, state->charm, 70) != 0) {
      state->charm = 0;
      return 1;
    }
  }
  return 0;
}

s32 BattleUnit_CountDownStun(s32 unitId) {
  struct BattleUnit *state = Unit_Get__f11f66c(unitId);
  if (state->stun != 0) {
    state->stun--;
    if (state->stun == 0)
      return 1;
    if (BattleUnit_RollCounterEarlyEnd(unitId, state->stun, 40) != 0) {
      state->stun = 0;
      return 1;
    }
  }
  return 0;
}

s32 BattleUnit_CountDownSleep(s32 unitId) {
  struct BattleUnit *state = Unit_Get__f11f66c(unitId);
  if (state->sleep != 0) {
    state->sleep--;
    if (state->sleep == 0)
      return 1;
    if (BattleUnit_RollCounterEarlyEnd(unitId, state->sleep, 50) != 0) {
      state->sleep = 0;
      return 1;
    }
  }
  return 0;
}

s32 Battle_AdvanceCounterAndCheckChance(s32 id) {
  s32 next;
  s32 expired;
  u8 remaining;
  s32 turns;
  s32 packed;
  struct BattleUnit *unit;

  unit = Unit_Get__f11f66c(id);
  packed = unit->psySeal;
  turns = packed & 0xFF;
  if (turns != 0) {
    if ((u32)turns > 7U) {
      turns += 0xF8;
      unit->psySeal = turns;
      packed = turns;
    }
    if (packed & 7) {
      next = packed + 0xFF;
      unit->psySeal = next;
      packed = next;
    }
    expired = 1;
    remaining = packed;
    if (remaining == 0)
      return expired;
    if ((u32)remaining <= 7U &&
        BattleUnit_RollCounterEarlyEnd(id, unit->psySeal, 0x1E) != 0) {
      unit->psySeal = 0U;
      return 1;
    }
  }
  expired = 0;
  return expired;
}

#include "b5_context.h"

s32 sub_08124af8(s32 id) {
  struct BattleUnit *unit = Unit_Get__f11f66c(id);
  u8 *count = &unit->refrain;

  if (*count != 0) {
    s32 max;
    s32 before;
    s32 hp;
    s32 gain;

    BattlePlayback_Reset();
    hp = unit->hp;
    max = unit->maxHp;
    before = hp;
    hp += max * (*count + 1) / 10;
    if (hp > max)
      hp = max;
    gain = hp - before;
    if (gain != 0) {
      BattleEvent_Push(0, id);
      if (hp == unit->maxHp) {
        BattleEvent_Push(4, (u32)&MsgHpFull);
      } else {
        {
          u32 opcode = 1;

          /* FAKEMATCH(barrier): an empty asm on opcode, so its 1 is set
           * before gain is copied to r1; as BattleEvent_Push(1, gain), 2 of 74
           * instructions differ in tla-en. */
          asm volatile("" : "+l"(opcode));
          BattleEvent_Push(opcode, gain);
        }
        BattleEvent_Push(4, (u32)&MsgHpRecover);
      }
      unit->hp = hp;
      Unit_UpdateRatiosFar(id);
    }
    BattleEvent_DispatchQueued();
    if (--unit->refrain == 0)
      return 1;
  }
  return 0;
}

s32 BattleUnit_CountDownReflect(s32 id) {
  u8 *value = (u8 *)&((struct BattleUnit *)Unit_Get__f11f66c(id))->reflect;
  if (*value != 0) {
    (*value)--;
    if (*value == 0) {
      return 1;
    }
  }
  return 0;
}

s32 BattleUnit_CountDownAgilityModifier(s32 id) {
  struct BattleUnit *unit = Unit_Get__f11f66c(id);
  u8 *value = &unit->agilityTurns;
  if (*value != 0) {
    (*value)--;
    if (*value == 0) {
      unit->agilityStage = 0;
      return 1;
    }
  }
  return 0;
}

s32 DjinnRecovery_CountDown(void) {
  struct PlacementList *list;
  struct DjinnRecoveryEntry *timedEntry;
  struct DjinnRecoveryEntry *expiredEntry;
  s32 index;
  s32 removed;
  s32 initialCount;

  list = &Djinn_GetRecoveryTableFar(0)->list;
  initialCount = list->count;
  index = 0;
  removed = 0;
  if (index < initialCount) {
    timedEntry = list->entries;
    do {
      if (timedEntry->turns > 0 &&
          ((struct BattleUnit *)Unit_Get__f11f66c(timedEntry->unitId))->hp != 0) {
        timedEntry->turns--;
      }
      index++;
      timedEntry++;
    } while (index < list->count);
  }
  index = 0;
  if (index < list->count) {
    expiredEntry = list->entries;
    do {
      if (expiredEntry->turns == 0) {
        u8 id = expiredEntry->unitId;

        Djinn_ActivateFar(id, expiredEntry->element, expiredEntry->index);
        DjinnRecovery_RemoveFar(id, expiredEntry->element, expiredEntry->index);
        Unit_CalculateFar(id);
        removed = 1;
      } else {
        expiredEntry++;
        index++;
      }
    } while (index < list->count);
  }
  return removed;
}

s32 DjinnRecovery_CountDownTwenty(void) {
  s32 turns;

  turns = 19;
  do {
    turns--;
    DjinnRecovery_CountDown();
  } while (turns >= 0);
  return 0;
}

void Func_08124cc0(void) {}

/// Counts down one side's Djinn recovery at the turn's end: an owner must be in
/// the battle; each Djinni that comes back plays its event (its icon, sound
/// 175, the flash and its message) before it is Set. The message is
/// "... is set to ...!" Returns 1 when a count fell, 2 when one was Set, else
/// 0.
// ☀️'s DjinnRecovery_CountDown with ⚓️'s presentation.
s32 Func_08124cc4(s32 side) {
  struct PlacementList *list;
  struct DjinnRecoveryEntry *timedEntry;
  struct DjinnRecoveryEntry *entry;
  s32 result = 0;
  s32 i;
  s32 count;

  list = &Djinn_GetRecoveryTableFar(side)->list;
  i = 0;
  if (i < list->count) {
    timedEntry = list->entries;
    do {
      if (timedEntry->turns > 0 &&
          BattleParty_IsUnitListed(timedEntry->unitId) &&
          ((struct BattleUnit *)Unit_Get__f11f66c(timedEntry->unitId))->hp != 0) {
        timedEntry->turns--;
        result = 1;
      }
      count = list->count;

      i++;
      timedEntry++;
    } while (i < count);
  }
  i = 0;
  if (i < list->count) {
    entry = list->entries;
    do {
      if (entry->turns == 0) {
        u8 id = entry->unitId;

        if (BattleParty_IsUnitListed(id)) {
          u8 element;

          result = 2;
          BattlePlayback_Reset();
          BattlePlayback_SchedulePhase(30);
          BattleEvent_Push(0, id);
          BattleEvent_Push(3, entry->element * 20 + entry->index + 300);
          BattleEvent_Push(14, 175);
          BattleEvent_Push(10, 0);
          BattleEvent_Push(4, (u32)&MsgDjinnIsSet);
          BattleEvent_Push(11, id);
          if (BattleParty_IsUnitListed(id)) {
            Actor_SetMode((struct Actor *)BattleObject_Get(id)->object, 3);
            Actor_SetAnimationSpeedFar(
                (struct Actor *)BattleObject_Get(id)->object, 32);
          }
          element = entry->element;
          Djinn_ActivateFar(id, element, entry->index);
          DjinnRecovery_RemoveFar(id, entry->element, entry->index);
          Unit_CalculateFar(id);
          if (BattleParty_IsUnitListed(id))
            Func_08127308(id, element, 3, 0);
          BattlePlayback_WaitForReady();
        }
      } else {
        entry++;
        i++;
      }
    } while (i < list->count);
  }
  return result;
}

#include "owner_value.h"

// The Japanese text holds these messages in the same order two places later,
// but its labels for several of them belong to their neighbours, so the
// Japanese release reaches each of those by its place beside a message whose
// label fits its text.
#if LANGUAGE == LANGUAGE_JAPANESE
// ねむりからさめた! ("wakes from slumber"), three before MsgResNormal.
#define MSG_WAKES ((s32)&MsgResNormal - 3)
// すばやさが もどった! ("Agility returns to normal"), after MsgDefNormal.
#define MSG_AGI_NORMAL ((s32)&MsgDefNormal + 1)
// われにかえった! ("regains consciousness"), two after MsgDefNormal.
#define MSG_CONSCIOUS ((s32)&MsgDefNormal + 2)
// まぼろしを うちやぶった! ("sees clearly once again"), three before
// MsgConscious2.
#define MSG_CURE_DELUSION ((s32)&MsgConscious2 - 3)
// エナジーふうじがとけた! ("Psynergy seal is gone"), two before MsgConscious2.
#define MSG_CURE_SEAL ((s32)&MsgConscious2 - 2)
// しびれがきえた! ("no longer stunned"), the message before MsgConscious2.
#define MSG_CURE_STUN ((s32)&MsgConscious2 - 1)
// リフレクトのこうかがきえた! ("Reflect no longer affects"), after
// MsgRefrainEnd.
#define MSG_REFLECT_END ((s32)&MsgRefrainEnd + 1)
#else
#define MSG_WAKES ((s32)&MsgWakes)
#define MSG_AGI_NORMAL ((s32)&MsgAgiNormal)
#define MSG_CONSCIOUS ((s32)&MsgConscious)
#define MSG_CURE_DELUSION ((s32)&MsgCureDelusion)
#define MSG_CURE_SEAL ((s32)&MsgCureSeal)
#define MSG_CURE_STUN ((s32)&MsgCureStun)
#define MSG_REFLECT_END ((s32)&MsgReflectEnd)
#endif

struct TurnEndList {
  struct DjinnRecoveryEntry entries[72];
  s32 count;
};

struct TurnEndTable {
  u8 padding[8];
  struct TurnEndList list;
};

struct BattleGroupOrder {
  s32 group[2];
};

extern const struct BattleGroupOrder Data_081297c4;

void Func_0812628c(s32 mode, s32 background, s32 value);

#define ANNOUNCE_END(test, message)                                            \
  if (test(unitId) != 0) {                                                     \
    BattleUnit_BuildStatusFlags(unitId, BattleObject_Get(unitId));             \
    MessageArgument_PushFar(unitId, 1);                                        \
    Message_ShowAndWaitCoreFar(message);                                       \
    BattleMessage_WaitForAdvance();                                            \
  }

#define ANNOUNCE_END_POSE(test, message)                                       \
  if (test(unitId) != 0) {                                                     \
    BattleUnit_BuildStatusFlags(unitId, BattleObject_Get(unitId));             \
    MessageArgument_PushFar(unitId, 1);                                        \
    BattleObject_UpdatePose(unitId);                                           \
    Message_ShowAndWaitCoreFar(message);                                       \
    BattleMessage_WaitForAdvance();                                            \
  }

void Func_08124e20(void) {
  struct BattleWork *battle;
  s32 side;
  s32 sideCount;
  s32 count;
  s32 recoveryIndex;
  struct TurnEndList *list;
  struct DjinnRecoveryEntry *timedEntry;
  struct DjinnRecoveryEntry *expiredEntry;

  battle = Ram_HeapSlots->battleWork;
  sideCount = (battle->twoSided != 0) + 1;
  for (side = 0; side < sideCount; side++) {
    list = &((struct TurnEndTable *)Djinn_GetRecoveryTableFar(side))->list;
    recoveryIndex = 0;
    timedEntry = list->entries;
    if (recoveryIndex < list->count) {
      do {
        if (timedEntry->turns > 0 &&
            BattleObject_Get(timedEntry->unitId) != 0 &&
            ((struct BattleUnit *)Unit_Get__f11f66c(timedEntry->unitId))->hp != 0) {
          timedEntry->turns--;
        }
        recoveryIndex++;
        timedEntry++;
      } while (recoveryIndex < list->count);
    }
    recoveryIndex = 0;
    if (recoveryIndex < list->count) {
      expiredEntry = list->entries;
      do {
        if (expiredEntry->turns == 0) {
          u8 unitId = expiredEntry->unitId;
          u8 element;

          if (BattleObject_Get(unitId) != 0) {
            BattlePlayback_Reset();
            BattlePlayback_SchedulePhase(30);
            BattleEvent_Push(0, unitId);
            BattleEvent_Push(3, expiredEntry->element * 20 +
                                    expiredEntry->index + 300);
            BattleEvent_Push(14, 175);
            BattleEvent_Push(10, 0);
            BattleEvent_Push(4, (u32)&MsgDjinnIsSet);
            BattleEvent_Push(11, unitId);
            Sound_PlayFar(212);
            if (BattleParty_IsUnitListed(unitId)) {
              Actor_SetMode(BattleObject_Get(unitId)->object, 3);
              Actor_SetAnimationSpeedFar(
                  (struct Actor *)BattleObject_Get(unitId)->object, 32);
            }
            element = expiredEntry->element;
            Djinn_ActivateFar(unitId, element, expiredEntry->index);
            DjinnRecovery_RemoveFar(unitId, expiredEntry->element,
                                    expiredEntry->index);
            Unit_CalculateFar(unitId);
            if (BattleParty_IsUnitListed(unitId))
              Func_08127308(unitId, element, 3, 0);
            BattlePlayback_WaitForReady();
          }
        } else {
          expiredEntry++;
          recoveryIndex++;
        }
      } while (recoveryIndex < list->count);
    }
  }
  Func_0812628c(2, *(u16 *)&Ram_HeapSlots->battleWork->unk670[0], 0);
  {
    s32 *groupList;
    struct BattleGroupOrder order = Data_081297c4;
    u16 unitIds[16];
    s32 group;
    s32 unitIndex;

    if (battle->twoSided != 0 && battle->linkSide != 0) {
      order.group[0] = 2;
      order.group[1] = 1;
    }
    groupList = order.group;
    for (group = 0; group <= 1; group++) {
      count = BattleParty_ListActorIds(groupList[group], unitIds);
      if (groupList[group] == 1)
        count += BattleParty_PrepareReserveOwners(unitIds + count);
      for (unitIndex = 0; unitIndex < count; unitIndex++) {
        s32 hpAmount;
        s32 ppAmount;
        s32 unitId = unitIds[unitIndex];
        struct BattleUnit *unit = Unit_Get__f11f66c(unitId);

        unit->equipBonus[1] = 0;
        if (unit->ready != 0) {
          unit->ready--;
        }
        if (unit->hp != 0) {
          if (unit->hpRegen != 0 && unit->hp != unit->maxHp) {
            hpAmount = unit->hpRegen;
            switch (unit->enemyId) {
            case 91:
            case 92:
            case 93:
            case 94:
            case 95:
            case 96:
            case 97:
            case 218:
              hpAmount *= 10;
              break;
            }
            if (unit->hp + hpAmount > unit->maxHp) {
              hpAmount = unit->maxHp - unit->hp;
            }
            Unit_AddHpFar(unitId, hpAmount);
            MessageArgument_PushFar(unitId, 1);
            MessageArgument_PushFar(hpAmount, 5);
            if (unit->hp == unit->maxHp) {
              Message_ShowAndWaitCoreFar((s32)&MsgHpFull);
            } else {
              Message_ShowAndWaitCoreFar((s32)&MsgHpRecover);
            }
            Sound_PlayFar(175);
            BattleMessage_WaitForAdvance();
          }
          if (unit->ppRegen != 0 && unit->pp != unit->maxPp) {
            ppAmount = unit->ppRegen;
            if (unit->pp + ppAmount > unit->maxPp) {
              ppAmount = unit->maxPp - unit->pp;
            }
            Unit_AddPpFar(unitId, ppAmount);
            MessageArgument_PushFar(unitId, 1);
            MessageArgument_PushFar(ppAmount, 5);
            if (unit->pp == unit->maxPp) {
              Message_ShowAndWaitCoreFar((s32)&MsgPpFull);
            } else {
              Message_ShowAndWaitCoreFar((s32)&MsgPpRecover);
            }
            Sound_PlayFar(175);
            BattleMessage_WaitForAdvance();
          }
        }
        ANNOUNCE_END(BattleUnit_CountDownAgilityModifier, MSG_AGI_NORMAL);
        ANNOUNCE_END(BattleUnit_CountDownAttackModifier, (s32)&MsgAtkNormal);
        ANNOUNCE_END(BattleUnit_CountDownDefenseModifier, (s32)&MsgDefNormal);
        ANNOUNCE_END(BattleUnit_CountDownResistanceModifier,
                     (s32)&MsgResNormal);
        ANNOUNCE_END(BattleUnit_CountDownDelusion, MSG_CURE_DELUSION);
        ANNOUNCE_END(BattleUnit_CountDownConfusion, MSG_CONSCIOUS);
        ANNOUNCE_END(BattleUnit_CountDownCharm, (s32)&MsgConscious2);
        ANNOUNCE_END_POSE(BattleUnit_CountDownStun, MSG_CURE_STUN);
        ANNOUNCE_END_POSE(BattleUnit_CountDownSleep, MSG_WAKES);
        ANNOUNCE_END(Battle_AdvanceCounterAndCheckChance, MSG_CURE_SEAL);
        ANNOUNCE_END(sub_08124af8, (s32)&MsgRefrainEnd);
        ANNOUNCE_END(BattleUnit_CountDownReflect, MSG_REFLECT_END);
      }
    }
  }
}

void Djinn_DeactivateFar(s32 unitId, s32 element, s32 djinni);

void Battle_ApplyActionExtras(s32 unitId, s32 all, s32 announce) {
  u8 elements[10];
  u8 indices[10];
  s32 shown;
  s32 count;
  s32 active;
  s32 element;
  s32 djinni;
  s32 i;
  s32 owner;
  s32 recoveryTurn;
  struct PlacementList *list;
  struct PlacementList *pending;

  shown = 0;
again:
  {
    count = 0;
    active = 0;
    for (element = 0; element <= 3; element++) {
      for (djinni = 0; djinni <= 19; djinni++) {
        if (Djinn_IsActiveFar(unitId, element, djinni)) {
          elements[count] = element;
          indices[count] = djinni;
          count++;
          active = 1;
          if (count == 10)
            goto picked;
        }
      }
    }
    if (count == 0) {
      for (element = 0; element <= 3; element++) {
        for (djinni = 0; djinni <= 19; djinni++) {
          if (Djinn_IsReadyFar(unitId, element, djinni)) {
            elements[count] = element;
            indices[count] = djinni;
            count++;
            if (count == 10)
              goto picked;
          }
        }
      }
    }
  picked:
    if (count == 0)
      return;

    count = ((u32)Battle_RandomFar() * count) >> 16;
    element = elements[count];
    djinni = indices[count];
    if (active) {
      DjinnRecovery_AddStandbyFar(unitId, element, djinni);
      Djinn_DeactivateFar(unitId, element, djinni);
      if (announce) {
        BattleEvent_Push(0, unitId);
        BattleEvent_Push(3, element * 20 + djinni + 300);
        BattleEvent_Push(4, (u32)&MsgDjinnOnStandby);
        return;
      }
    } else if (announce) {
      return;
    }

    list = &Djinn_GetRecoveryTableFar(unitId < 0 || unitId > 7)->list;
    for (i = 0; i < list->count; i++) {
      if (list->entries[i].element == element &&
          list->entries[i].index == djinni && list->entries[i].turns == -1)
        list->entries[i].turns = -2;
    }

    pending = &Djinn_GetRecoveryTableFar(unitId < 0 || unitId > 7)->list;
    for (;;) {
      owner = -1;
      for (i = 0; i < pending->count; i++) {
        if (pending->entries[i].turns == -2) {
          owner = pending->entries[i].unitId;
          break;
        }
      }
      if (owner == -1)
        break;
      recoveryTurn = -1;
      for (i = 0; i < pending->count; i++) {
        if (pending->entries[i].unitId == owner &&
            pending->entries[i].turns > recoveryTurn)
          recoveryTurn = pending->entries[i].turns;
      }
      recoveryTurn++;
      if (recoveryTurn <= 1)
        recoveryTurn = 2;
      for (i = 0; i < pending->count; i++) {
        if (pending->entries[i].unitId == owner &&
            pending->entries[i].turns == -2) {
          pending->entries[i].turns = recoveryTurn;
          recoveryTurn++;
        }
      }
    }

    if (!shown) {
      shown = 1;
      BattleEvent_Push(0, unitId);
      if (!all) {
        s32 message;

        BattleEvent_Push(3, element * 20 + djinni + 300);
        message = (u32)&MsgDjinnDrained;
        if (unitId < 0 || unitId > 7)
          message++;
        BattleEvent_Push(4, message);
      } else {
        BattleEvent_Push(4, (u32)&MsgAllDjinnDrained);
      }
    }
    if (all)
      goto again;
  }
}

void Func_0812561c(struct BattlePlan *plan) {
  u32 unitId;
  struct BattleUnit *unit;
  s32 battleContinues;
  s32 counts[4];
  struct PlacementList *list;
  s32 strongestElement;
  s32 recoveryTurn;

  unitId = plan->actorId;
  battleContinues = 0;
  unit = Unit_Get__f11f66c(unitId);
  if (unitId <= 7 ||
      ((struct BattleWork *)Ram_HeapSlots->battleWork)->twoSided != 0) {
    list = &Djinn_GetRecoveryTableFar(unitId > 7)->list;
    for (strongestElement = 0; strongestElement < list->count;
         strongestElement++) {
      if (list->entries[strongestElement].unitId == unitId &&
          list->entries[strongestElement].turns == -1) {
        Djinn_DeactivateFar(unitId, list->entries[strongestElement].element,
                            list->entries[strongestElement].index);
      }
    }
  }
  if (BattleParty_ListLivingUnits(1, 0) != 0 &&
      BattleParty_ListLivingUnits(2, 0) != 0) {
    battleContinues = 1;
  }
  {
    s32 i;
    s32 owner;
    struct PlacementList *pending;

    pending =
        &Djinn_GetRecoveryTableFar(
             unitId > 7 &&
             ((struct BattleWork *)Ram_HeapSlots->battleWork)->twoSided != 0)
             ->list;
    for (i = 0; i < 4; i++) {
      counts[i] = 0;
    }
    for (;;) {
      owner = -1;
      for (i = 0; i < pending->count; i++) {
        if (pending->entries[i].turns == -2) {
          owner = pending->entries[i].unitId;
          break;
        }
      }
      if (owner == -1) {
        break;
      }
      recoveryTurn = -1;
      for (i = 0; i < pending->count; i++) {
        if (pending->entries[i].unitId == owner &&
            pending->entries[i].turns > recoveryTurn) {
          recoveryTurn = pending->entries[i].turns;
        }
      }
      recoveryTurn++;
      if (recoveryTurn <= 1) {
        recoveryTurn = 2;
      }
      for (i = 0; i < pending->count; i++) {
        if (pending->entries[i].unitId == owner &&
            pending->entries[i].turns == -2) {
          pending->entries[i].turns = recoveryTurn;
          counts[pending->entries[i].element]++;
          recoveryTurn++;
        }
      }
    }
  }
  if (battleContinues != 0) {
    struct BattleUnit *before;
    s32 i;

    before = Heap_AllocEwram(sizeof(struct BattleUnit));
    Iwram_CopyWords(before, unit, sizeof(struct BattleUnit));
    for (i = 0; i <= 3; i++) {
      if (counts[i] > 4)
        counts[i] = 4;
      if (unit->elementLevels[i] < counts[i])
        unit->elementLevels[i] = counts[i];
    }
    Unit_CalculateFar(unitId);
    for (i = 0; i <= 3; i++) {
      s32 gain = unit->element[i].power - before->element[i].power;

      if (gain > 0) {
        BattlePlayback_Reset();
        BattlePlayback_SchedulePhase(25);
        BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, unitId);
        BattleEvent_Push(BATTLE_EVENT_NUMBER, gain);
        BattleEvent_Push(BATTLE_EVENT_SOUND, 175);
        // Messages 3285 to 3288, 3232, 3240, 3185, 3191 and 3188 in English;
        // all but MsgGoesDown still need their names in the PO catalogs.
        BattleEvent_Push(BATTLE_EVENT_MESSAGE, (u32)MsgEarthPowerUp + i);
        BattleEvent_Push(BATTLE_EVENT_REFRESH_UNIT, unitId);
        Sound_PlayFar(212);
        Actor_SetMode(BattleObject_Get(unitId)->object, 3);
        Actor_SetAnimationSpeedFar(
            (struct Actor *)BattleObject_Get(unitId)->object, 32);
        Func_08127308(unitId, i, 2, -1);
        BattlePlayback_WaitForReady();
      }
    }
    Heap_Release(before);
  }
  if (battleContinues != 0 && BattleObject_Get(unitId)->object != 0) {
    s8 *poison;
    u8 *reaper;

    BattlePlayback_Reset();
    if (plan->hauntDamage != 0) {
      BattleEvent_Push(BATTLE_EVENT_HIT, unitId);
      BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, unitId);
      BattleEvent_Push(BATTLE_EVENT_NUMBER, plan->hauntDamage);
      BattleEvent_Push(BATTLE_EVENT_MESSAGE, (u32)MsgCurseDamage);
      if (Unit_AddHpFar(unitId, -plan->hauntDamage) == 0) {
        if (BattleUnit_KeepsOneHp(unitId)) {
          Unit_AddHpFar(unitId, 1);
          BattleEvent_Push(BATTLE_EVENT_REFRESH_UNIT, unitId);
        } else {
          BattleEvent_Push(BATTLE_EVENT_DEFEAT, unitId);
          BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, unitId);
          BattleEvent_Push(BATTLE_EVENT_MESSAGE,
                           unitId <= 7 ? (u32)MsgGoesDown
                                       : (u32)MsgStrengthExhausted);
        }
      } else {
        BattleEvent_Push(BATTLE_EVENT_REFRESH_UNIT, unitId);
      }
    }
    BattleEvent_DispatchQueued();
    BattlePlayback_Reset();
    poison = &unit->poison;
    if (*poison != 0) {
      s32 damage = unit->maxHp * *poison / 10;
      struct BattleWork *state = Ram_HeapSlots->battleWork;

      BattleEvent_Push(BATTLE_EVENT_HIT, unitId);
      BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, unitId);
      BattleEvent_Push(BATTLE_EVENT_NUMBER, damage);
      BattleEvent_Push(BATTLE_EVENT_MESSAGE, (u32)MsgPoisonDamage);
      if (*poison != 0) {
        state->events.pendingCue = 134;
      } else {
        state->events.pendingCue = 133;
      }
      if (Unit_AddHpFar(unitId, -damage) == 0) {
        if (BattleUnit_KeepsOneHp(unitId)) {
          Unit_AddHpFar(unitId, 1);
          BattleEvent_Push(BATTLE_EVENT_REFRESH_UNIT, unitId);
        } else {
          BattleEvent_Push(BATTLE_EVENT_DEFEAT, unitId);
          BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, unitId);
          BattleEvent_Push(BATTLE_EVENT_MESSAGE,
                           unitId <= 7 ? (u32)MsgGoesDown
                                       : (u32)MsgStrengthExhausted);
        }
      } else {
        BattleEvent_Push(BATTLE_EVENT_REFRESH_UNIT, unitId);
      }
    }
    BattleEvent_DispatchQueued();
    BattlePlayback_Reset();
    reaper = &unit->grimReaper;
    if (*reaper != 0 && !BattleUnit_KeepsOneHp(unitId) && --*reaper == 0 &&
        Unit_AddHpFar(unitId, 0xc0000000) == 0) {
      BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, unitId);
      BattleEvent_Push(BATTLE_EVENT_MESSAGE, (u32)MsgReaperCalls);
      BattleEvent_Push(BATTLE_EVENT_HIT, unitId);
      BattleEvent_Push(BATTLE_EVENT_DEFEAT, unitId);
      BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, unitId);
      if (unitId <= 7) {
        BattleEvent_Push(BATTLE_EVENT_MESSAGE, (u32)MsgReaperCalls - 3);
      } else {
        BattleEvent_Push(BATTLE_EVENT_MESSAGE, (u32)MsgReaperCalls + 3);
      }
    }
    BattleEvent_DispatchQueued();
  }
  if (BattleObject_Get(unitId)->object != 0)
    Unit_CalculateFar(unitId);
}

#include "fixed_math.h"

void Fade_ToBlack(s32 arg0);

/// Fades the screen to black over sixteen frames as every background's
/// mosaic grows, then shows no layer. Sets REG_BLDCNT to darken the
/// backgrounds, leaves REG_MOSAIC at its largest and REG_DISPCNT in mode 1
/// with every layer off; the four BGxCNT are put back as they were. It draws
/// four random numbers a frame, which only advances the random number
/// generator. Returns 0.
s32 BattleTransition_RunMosaicFadeOut(void) {
  // volatile: the four saved BGxCNT values kept on the stack, each stored and
  // loaded again as in the ROM; without volatile, 65 of 94 instructions differ
  // in tla-en.
  volatile u16 bg0cnt;
  volatile u16 bg1cnt;
  volatile u16 bg2cnt;
  volatile u16 bg3cnt;
  s32 i;
  volatile u16 *reg;
  u32 saved;

  bg0cnt = REG_BG0CNT;
  REG_BG0CNT = bg0cnt | BGCNT_MOSAIC;
  reg = &REG_BG0CNT;
  bg1cnt = *++reg;
  *reg = bg1cnt | BGCNT_MOSAIC;
  bg2cnt = *++reg;
  *reg = bg2cnt | BGCNT_MOSAIC;
  bg3cnt = *++reg;
  *reg = bg3cnt | BGCNT_MOSAIC;
  reg += 0x21;
  *reg = 0x3eee;

  Fade_ToBlack(16);
  for (i = 0; i <= 15; i++) {
    Random16();
    Random16();
    Random16();
    Random16();
    REG_MOSAIC = (i << 8) | i;
    wait(1);
  }

  REG_DISPCNT = 1;
  wait(4);
  saved = bg0cnt;
  reg = &REG_BG0CNT;
  *reg++ = saved;
  *reg++ = bg1cnt;
  *reg++ = bg2cnt;
  *reg = bg3cnt;
  return 0;
}

// Tiles: build a map of consecutive byte values.

/// Writes consecutive bytes 0-255 followed by 0-223, then fills the remaining
/// 544 bytes with 0xff (1024 bytes in all).
void Graphics_BuildSequentialTileTable(s32 *dst) {
  s32 v;
  s32 n;
  s32 (*fill)(void *, s32, u32);

  // Advance by four entries because each store writes four bytes.
  v = 0x03020100;
  n = 0;
  do {
    n += 1;
    *dst++ = v;
    v += 0x04040404;
  } while (n >= 0 && n <= 63);

  v = 0x03020100;
  n = 0;
  do {
    n += 1;
    *dst++ = v;
    v += 0x04040404;
  } while (n >= 0 && n <= 55);

  // The fill's entry held in a local loads its address before the size's
  // shift; called directly, 2 of 25 instructions differ in tla-en.
  fill = Iwram_FillWordsEntry;
  fill(dst, 544, -1);
}

static __inline__ void FillWords_(void *destination, s32 size, u32 value) {
  Iwram_FillWords(destination, size, value);
}

/// Builds a flat tilemap: rows 0 to 3 0xffff, rows 4 and 5 tile 1023, rows 6
/// to 20 tiles 512 to 991 in order, then tile 1023. Called with screen block
/// 31 by Func_0812628c and Func_081263fc.
void BattleTransition_BuildTilemap(s32 *destination) {
  s32 entry;
  u32 index;

  /* FAKEMATCH(wrapper): each fill forwarded through FillWords_, so its size is
   * built in r1 at the call instead of shared with destination's step; as
   * direct Iwram_FillWords calls, 19 of 36 instructions differ in tla-en, and
   * the function is one instruction shorter. */
  FillWords_(destination, 0x100, -1);
  destination += 0x40;
  FillWords_(destination, 0x80, 0x03ff03ff);
  entry = (0x0201 << 16) | 0x0200;
  destination += 0x20;
  index = 0;
  do {
    index++;
    *destination++ = entry;
    entry += 0x00020002;
  } while (index <= 239);
  FillWords_(destination, 0x280, 0x03ff03ff);
}

#include "bg_scroll.h"

extern const u8 gData_0812cc74[];

/// Copies this frame's BG2CNT word and starts the HBlank DMA of the BG2 lines
/// and the affine block, while the background view is in mode 2.
void BattleStaging_UpdateHBlankScroll(void) {
  struct BattleAffineHdma *records;
  u16 *record;

  if (((struct BattleBackgroundView *)Ram_HeapSlots->transitionWork)->mode ==
      2) {
    records = Ram_HeapSlots->unk28[0];
    record = records->lines[records->page];
    Io_Write16(record[0], &REG_BG2CNT);
    Dma_Set(record + 1, (void *)&REG_BG2CNT, DMA_HBLANK_HALFWORD_PER_LINE,
            &REG_DMA0SAD);
    Dma_Set(&records->affine, (void *)&REG_BG2PA, DMA_COPY_WORDS(4), REG_DMA3);
  }
}

/// Copies one of eight tiles from gData_0812cc74 into 0x06005000, a new one
/// every four frames while the transition timer (unk84[9], as
/// BattleTransition_AdvanceTimer counts it) runs from 1 to 32.
void BattleTransition_UploadTileVariant(void) {
  u32 variant = *(s32 *)Ram_HeapSlots->unk84[9] - 1;

  if (variant <= 31)
    Dma_Set(gData_0812cc74 + (variant >> 2) * 32, (void *)0x06005000,
            DMA_COPY_WORDS(8), REG_DMA3);
}

void BattleTransition_AdvanceTimer(void) {
  s32 scroll;
  struct BattleCamera *camera;
  u32 *timer;
  u32 frame;
  u32 next;

  timer = (u32 *)Ram_HeapSlots->unk84[9];
  frame = *timer;
  camera = (struct BattleCamera *)Ram_HeapSlots->cameraWork;
  scroll = 0x34 - frame;
  if (scroll > 0x20)
    scroll = 0x20;
  if (scroll < 0)
    scroll = 0;
  gBgScroll[0].y = scroll;
  if (frame <= 0x50U)
    camera->yaw = 45 * frame * 8 + 0xaf80;
  next = (*timer = *timer + 1);
  if (next <= 0x50U) {
    BattleCamera_SetView(0, 0, 0, 0xb4 - next);
    return;
  }
  BattleCamera_SetView(0, 0, 0, 0x64);
}

/// Draws the transition's two rows of tiles, closing in from row 13 as the
/// timer runs; the lower row takes the vertically flipped tiles.
void BattleTransition_DrawRows(void) {
  u32 i;
  s32 frame;
  s32 aligned;
  s32 tile;
  u32 row;
  u16 *tiles;

  frame = *(s32 *)Ram_HeapSlots->unk84[9];
  if (frame >= 0 && frame <= 79) {
    tile = (7 & frame) + 0xf081;
    if (frame >= 0)
      aligned = frame;
    else
      aligned = frame + 7;
    row = 13 - (aligned >> 3);
    i = 0;
    tiles = (u16 *)((row << 6) + 0x06006000);
    do {
      i++;
      *tiles = tile;
      tiles++;
    } while (i != 32);

    tile |= 0x800;
    aligned = frame;
    if (frame < 0)
      aligned += 7;
    row = (aligned >> 3) + 13;
    if (row <= 20) {
      i = 0;
      tiles = (u16 *)((row << 6) + 0x06006000);
      do {
        i++;
        *tiles = tile;
        tiles++;
      } while (i != 32);
    }
  }
}

void Func_08125d68(void) {
  u32 zero = 0;

  REG_BG0VOFS = zero;
}

#include "irq.h"

#define DMA3 ((volatile u32 *)0x040000d4)

extern u8 Data_0812cd74[];

void PartyStatus_CreateWindowFar(s32 flags);
void Func_08138028(struct BattlePresentationWork *work);
void Func_08118bcc(s32 value);

extern char ResourceId_SerpentStarsBackdrop;

void Func_08125d74(s32 arg, s32 kind) {
  struct BattlePresentationWork work;
  u16 owners[14];
  u16 shown[14];
  s32 *timer;
  struct BattleWork *battle;
  struct BattleBackgroundView *view;
  struct HeapSlots *heap;
  s32 ownerCount;
  s32 count;
  u32 i;

  heap = Ram_HeapSlots;
  view = (struct BattleBackgroundView *)heap->transitionWork;
  timer = HeapSlot_Alloc(168, 4);
  if ((u32)(kind - 78) > 2) {
    s32 y;
    s32 zero;

    Dma_Set(Data_0812cd74, (void *)0x06005020, 0x84000008, DMA3);
    Dma_Set(Data_0812cd74 + 32, (void *)0x06005040, 0x84000008, DMA3);
    Dma_Set(Data_0812cd74 + 64, (void *)0x06005060, 0x84000008, DMA3);
    Dma_Set(Data_0812cd74 + 96, (void *)0x06005080, 0x84000008, DMA3);
    Dma_Set(Data_0812cd74 + 128, (void *)0x060050a0, 0x84000008, DMA3);
    Dma_Set(Data_0812cd74 + 160, (void *)0x060050c0, 0x84000008, DMA3);
    Dma_Set(Data_0812cd74 + 192, (void *)0x060050e0, 0x84000008, DMA3);
    view->secondMode = 1;
    view->busy = 0;
    view->mode = 1;
    REG_DISPCNT = 1;
    {
      u32 v = 0x33333333;
      /* FAKEMATCH(barrier): an empty asm on v, so the fill's value loads after
       * its size; as Iwram_FillWords((void *)0x06005000, 32, 0x33333333), 2
       * of 490 instructions differ in tla-en. */
      __asm__("" : "=r"(v) : "0"(v));
      Iwram_FillWords((void *)0x06005000, 32, v);
    }
    // The clear fill in a once-only block loads its address ahead of its size
    // and value; as the plain call, 6 of 490 instructions differ in tla-en.
    do {
      Iwram_FillWords((void *)0x06005100, 32, 0);
    } while (0);
    REG_BG1CNT = 0xc04;
    REG_BG0CNT |= 2;
    view->mode = 2;
    {
      u16 *map = (u16 *)0x06006000;

      for (i = 0; i < 32; i++) {
        u32 tile;
        u32 column;

        if (i <= 20)
          tile = 0xf080;
        else
          tile = 0xf088;
        for (column = 0; column < 32; column++)
          *map++ = tile;
      }
    }
    y = 32;
    gBgScroll[1].x = 8;
    gBgScroll[0].y = y;
    gBgScroll[1].y = y;
    zero = 0;
    wait(1);
    REG_WIN0H = 0xf0;
    REG_WIN0V = 0x88;
    REG_WIN1H = 0xf0;
    REG_WIN1V = 0x88;
    REG_WININ = 0x3537;
    REG_WINOUT = 0x3f21;
    QueueIoWriteDelay2(0x04000000, 0x7741);
    BattleCamera_SetView(0, 0, 0, 180);
    *timer = zero;
    Task_Add(BattleTransition_AdvanceTimer, TASK_PASS_VBLANK);
    Task_Add(BattleTransition_DrawRows, TASK_PASS_FRAME);
    Irq_SetHandler(2, 32, Func_08125d68);
    gBgScroll[0].y = y;
    wait(1);
    PartyStatus_CreateWindowFar(Ram_HeapSlots->battleWork->partyStatus);
    wait(20);
    QueueIoWriteDelay10(0x04000008, 2);
    QueueIoWriteDelay6(0x04000008, 0);
    if ((u32)(kind - 74) <= 2 || kind == 638)
      wait(60);
    else
      Func_08118bcc(arg);
    Task_Remove(BattleTransition_AdvanceTimer);
    Task_Remove(BattleTransition_DrawRows);
    gBgScroll[0].y = 0;
    Irq_SetHandler(2, 0, 0);
  } else {
    s32 zero;

    battle = heap->battleWork;
    view->secondMode = 1;
    view->busy = 0;
    ownerCount = BattleParty_PrepareActiveOwners(owners);
    ownerCount += BattleParty_PrepareReserveOwners(owners + ownerCount);
    for (i = 0; i != ownerCount; i++) {
      s32 id = owners[i];
      struct BattleObj *battleObject = BattleObject_Get(id);
      u16 enemyId = ((struct BattleUnit *)Unit_Get__f11f66c(id))->enemyId;

      if (enemyId != 357 && enemyId != 369 && enemyId != 373 && enemyId != 376)
        battleObject->scale = 0xb333;
    }
    IO_WRITE_QUEUE_PUSH(0x04000000, 0x6041, 0x20000);
    wait(1);
    {
      s32 background = (s32)&ResourceId_SerpentStarsBackdrop;
      u16 *slot = (u16 *)battle->unk670;

      zero = 0;
      *slot = background;
    }
    work.count = BattleParty_ListActorIds(2, (u16 *)work.actors);
    work.actors[work.count] = 255;
    BattleActor_SpawnObjectsForList(work.actors, 0);
    Func_08138028(&work);
    BattleCamera_SetView(0, 0, 0, 100);
    *timer = zero;
    Irq_SetHandler(2, 32, Func_08125d68);
    wait(1);
    wait(20);
    PartyStatus_CreateWindowFar(Ram_HeapSlots->battleWork->partyStatus);
    QueueIoWriteDelay10(0x04000008, 2);
    QueueIoWriteDelay6(0x04000008, 0);
    REG_BLDCNT = 0x3f40;
    count = BattleParty_ListActorIds(3, shown);
    shown[count] = 255;
    BattleActor_SpawnObjectsForList((s16 *)shown, 0);
    count = BattleParty_ListActorIds(1, shown);
    for (i = 0; i != count; i++)
      BattleHighlight_SetSpriteMode(shown[i], 1);
    for (i = 0; i != 16; i++) {
      REG_BLDALPHA = i | 0x1000;
      wait(1);
    }
    for (i = 0; i != count; i++)
      BattleHighlight_SetSpriteMode(shown[i], 0);
    Func_08118bcc(arg);
    gBgScroll[0].y = 0;
    wait(1);
    Irq_SetHandler(2, 0, 0);
  }
  Irq_SetHandler(2, 0, 0);
  REG_BG1CNT = 0x1f83;
  wait(1);
  REG_BG1CNT = 0x1f83;
  REG_BG0CNT &= 0xfffd;
  gBgScroll[1].x = 8;
  REG_DISPCNT = 0x1541;
  HeapSlot_Release(168);
}


/// Sets the palette brightness. Level 0 transfers the original colours
/// unchanged.
void BattleBackground_SetPaletteLevel(s32 unused, s32 level) {
  struct BattleWork *screen = Ram_HeapSlots->battleWork;
  u16 *palette = screen->palette;
  u32 ime;
  u32 *slot;

  // The save in a once-only block loads REG_IME's address after the stack
  // frame is made, and a loop on a zero local stores it through slot before
  // IME is cleared; without them, 8 of 54 instructions differ in tla-en.
  do {
    u16 *imeReg;
    s32 once;

    /* FAKEMATCH(temporary): a pointer to ime, which keeps the save in a stack
     * slot stored through r6; as ime directly, 33 of 54 instructions differ
     * in tla-en. */
    slot = &ime;
    imeReg = (u16 *)&REG_IME;
    once = 0;

    do {
      *slot = *imeReg;
      *imeReg = (u32)imeReg;
    } while (once);
  } while (0);

  if (level == 0) {
    Dma_Set(palette, (void *)&BG_PLTT_COLOR(6, 0), DMA_COPY_HALFWORDS(128),
            REG_DMA3);
  } else {
    ColorBuffer_Scale(palette, (u16 *)0x050000c0,
                      screen->brightness = 0x10000 - level * 1092, 128);
  }
  {
    u32 saved = *slot;

    REG_IME = saved;
  }
}

/// The Broken Seal's BattleBackground_SetMode, with this
/// game's heap slots, its callback BattleStaging_UpdateHBlankScroll at 0xcff
/// and its IO write of 0x1f83 to BG1CNT.
void Func_0812628c(s32 mode, s32 value, s32 fade) {
  struct BattleBackgroundView *transition =
      (struct BattleBackgroundView *)Ram_HeapSlots->transitionWork;

  if (transition->mode == 0)
    Task_Add(BattleStaging_UpdateHBlankScroll, 0xcff);
  transition->mode = mode;

  if (mode == 1)
    IO_WRITE_QUEUE_PUSH((u32)&REG_BG1CNT,
                        BGCNT_SCREEN_BASE(31) | BGCNT_256_COLORS |
                            BGCNT_PRIORITY(3),
                        IO_WRITE_16);

  Dma_Set((void *)OBJ_PLTT, (void *)&BG_PLTT_COLOR(5, 0),
          DMA_COPY_HALFWORDS(16), REG_DMA3);
  *(u16 *)0x050000bc = *(u16 *)0x050001e8;

  if (fade == 0x80) {
    struct BattleWork *work;
    u16 *palette;

    work = Ram_HeapSlots->battleWork;
    palette = work->palette;
    Dma_Set(palette, (void *)&BG_PLTT_COLOR(6, 0), DMA_COPY_HALFWORDS(128),
            REG_DMA3);
  } else if (fade != 0) {
    struct BattleWork *work;
    u16 *source;
    u16 *destination;
    s32 i;

    work = Ram_HeapSlots->battleWork;
    source = work->palette;
    destination = (u16 *)0x050000c0;

    for (i = 0; i != 128; i++) {
      s32 red = source[i] & 31;
      s32 green = (source[i] >> 5) & 31;
      s32 blue = (source[i] >> 10) & 31;

      if (red > fade)
        red -= fade;
      else
        red = 0;
      if (green > fade)
        green -= fade;
      else
        green = 0;
      if (blue > fade)
        blue -= fade;
      else
        blue = 0;
      destination[i] = (blue << 10) | (green << 5) | red;
    }
  }
  Graphics_BuildSequentialTileTable((void *)0x06003800);
  BattleTransition_BuildTilemap((void *)0x0600f800);
}

void Func_081263c4(void) {
  void *block = HeapSlot_AllocEwram(40, 0x2a0);
  struct BattlePresentationTransition *work = Ram_HeapSlots->transitionWork;

  // An empty once-only loop after the work's load keeps the fill's entry
  // ahead of the size's copy; without it, 4 of 18 instructions differ in
  // tla-en.
  do {
  } while (0);
  Iwram_FillWords(block, 0x2a0, 0);
  work->mode = 0;
}

void Func_081263f0(void) { HeapSlot_Release(40); }

#include "ram_code.h"

// The parts of the battle work the backdrop uses.
struct BattleBackdropWork {
  u8 unk000[0x56c];
  u16 palette[128];
  s32 brightness;
  u8 unk670[0x1f7];
  u8 keepBackdropPalette;
};

typedef void (*ModelDecoder)(const void *source, void *destination);

extern const u8 Model_DecodeCode[];
extern u8 Model_DecodeCodeSize[];

u8 *Resource_GetTableEntry(s32 id);

// Loads a battle backdrop: its picture decoded into BG VRAM, its palette
// (darkened by level steps of 1/60 when level is not negative), the tile
// table and tilemap, and the HBlank scroll the first time.
void Func_081263fc(s32 mode, s32 resource, s32 level) {
  struct BattlePresentationTransition *view = Ram_HeapSlots->transitionWork;
  u8 *data = Resource_GetTableEntry(resource);
  struct BattleBackdropWork *session =
      (struct BattleBackdropWork *)Ram_HeapSlots->battleWork;
  ModelDecoder decode;
  u16 *palette;

  RamCode_Load(decode, Model_DecodeCode, Model_DecodeCodeSize);
  {
    u32 offset = 0x100;
    void *vram;

    /* FAKEMATCH(dead): two updates that cancel, under a test that emits
     * nothing, end a CSE block so the VRAM address loads before the data's
     * add; without them, 2 of 137 instructions differ in tla-en. */
    if (session) {
      offset++;
      offset--;
    }
    vram = (void *)0x06008000;
    decode(data + offset, vram);
  }
  Heap_Release(decode);
  palette = session->palette;
  Dma_Set(data, palette, 0x84000040, REG_DMA3);
  if (level >= 0)
    ColorBuffer_Scale(palette, (u16 *)0x050000c0,
                      session->brightness = 0x10000 - level * 1092, 128);
  if (session->keepBackdropPalette == 0) {
    Dma_Set((void *)0x05000200, (void *)0x050000a0, 0x80000010, REG_DMA3);
    *(u16 *)0x050000bc = *(u16 *)0x050001e8;
  }
  Graphics_BuildSequentialTileTable((void *)0x06003800);
  BattleTransition_BuildTilemap((void *)0x0600f800);
  Iwram_ClearWords((void *)0x0600ffc0, 64);
  if (view->mode == 0)
    Task_Add(BattleStaging_UpdateHBlankScroll, 0xcff);
  view->mode = mode;
  if (mode == 1)
    REG_BG1CNT = 0x1f83;
}

void BattleCamera_SetRange(s32 centerX, s32 centerY, s32 offsetX, s32 offsetY,
                           s32 scale) {
  struct BattleBackgroundView *view =
      (struct BattleBackgroundView *)Ram_HeapSlots->transitionWork;
  struct BattleAffineHdma *scanlines = Ram_HeapSlots->unk28[0];
  struct BattleCamera *camera = Ram_HeapSlots->cameraWork;
  s32 horizon;
  u16 wrap;
  u16 *line;
  struct BattleAffine *affine;
  s32 ratio;
  s32 step;
  s32 x;
  s32 y;
  u32 scanline;
  u32 groundEndLine;

  horizon = 0x800;
  wrap = 0;
  // ratio is reused for Q16_ONE, the scale test's and the call's; as the
  // constant in both, 2 of 199 instructions differ in tla-en.
  ratio = Q16_ONE;
  if (scale >= ratio) {
    wrap = 0x2000;
    horizon = 0x6800 + -(s16)camera->yaw * 3;
  }
  if (scanlines == NULL)
    return;
  if ((view->mode == 1 || view->secondMode == 1) && view->busy == 0)
    gBgScroll[1].x = horizon >> 8;
  if (view->mode != 2)
    return;
  line = scanlines->lines[scanlines->page ^ 1];
  ratio = Iwram_RatioMulQ14(scale, ratio);
  step = ratio >> 8;
  affine = &scanlines->affine;
  affine->pa = step;
  affine->pb = 0;
  affine->pc = 0;
  affine->pd = step;
  x = Iwram_MulQ16(ratio, Iwram_MulQ16(centerX, scale - 0x10000));
  y = Iwram_MulQ16(ratio, Iwram_MulQ16(centerY, scale - 0x10000));
  affine->x = ((x + 0x7fff) >> 8) + offsetX + horizon;
  affine->y = ((y + 0x7fff) >> 8) + offsetY - 0x1000;
  groundEndLine =
      (Iwram_RatioMulQ14((s16)affine->pd, 0x4000 - affine->y) >> 16) + 1;
  scanline = 0;
  if (!GameFlag_Test(0x16b)) {
    for (; scanline < 16; scanline++)
      *line++ = 0x3f8e;
  }
  if (groundEndLine > 136)
    groundEndLine = 136;
  for (; scanline < groundEndLine; scanline++)
    *line++ = wrap | 0x478a;
  for (; scanline < 136; scanline++)
    *line++ = wrap | 0x478e;
  for (; scanline < 160; scanline++)
    *line++ = 0x3f8e;
  scanlines->page ^= 1;
}

#include "transform.h"

void Projection_Set(s32 span, u32 depth, s32 range);
void SceneTransform_ApplyPosition(s32 *position);
void SceneTransform_ApplyYaw(s32 yaw);
void SceneTransform_ApplyPitch(s32 pitch);

#ifdef UBFIX
void Func_08126700(s32 x, s32 depth, s32 y, s32 zoom) {
#else
void Func_08126700(s32 x, s32 depth, s32 y) {
#endif
  s32 span = 0x01fe0000;
  // UB: zoom is uninitialized; reads entry r4 directly (lsls r0, r4, #16).
#ifndef UBFIX
  s32 zoom;
#endif
  struct BattleCamera *scene = Ram_HeapSlots->cameraWork;
  s32 *pos = scene->pos;
  s32 scale = (zoom << 16) / 100;
  s32 renderBounds[3];
  s32 measuredBounds[3];
  s32 sourceBounds[3];
  s32 alpha;
  s32 half;
  s32 width;
  u32 result;

  pos[0] = x;
  pos[1] = depth;
  pos[2] = y;
  sourceBounds[0] = 0;
  alpha = 0xc000;
  sourceBounds[1] = 0;
  sourceBounds[2] = 0;

  result = Iwram_RatioMulQ14(span, alpha);
  Projection_Set(span, result, span * 2);
  Render_ResetTransformState();
  SceneTransform_ApplyPosition(pos);
  SceneTransform_ApplyYaw((s16)scene->yaw);
  SceneTransform_ApplyPitch((s16)scene->pitch);
  renderBounds[0] = 0;
  renderBounds[1] = 0;
  renderBounds[2] = scene->distance;
  Iwram_TransformVector(renderBounds, scene->eye);
  gCameraSceneParameters[3] = 120;
  gCameraSceneParameters[4] = 120;
  Render_ResetTransformState();
  Transform_SetLookAt(scene->eye, pos);
  Render_ProjectPoint(sourceBounds, measuredBounds);

  BattleCamera_SetRange(0x780000, 0x780000, (120 - measuredBounds[0]) << 8,
                        (120 - measuredBounds[1]) << 8, scale);
  half = scale * 0xff;
  width = half * 2;
  result = Iwram_RatioMulQ14(width, alpha);
  Projection_Set(width, result, half * 4);
}

// BattleCamera_SetView: The Broken Seal's function of that name
// with the camera from its heap slot. Places the battle camera at a position
// and zoom for a transition: sets the projection, turns the scene to the
// camera's yaw and pitch, projects the origin and gives the camera its
// range from where the origin lands on the screen.

void BattleCamera_SetView(s32 x, s32 depth, s32 y, s32 zoom) {
  struct BattleCamera *scene = Ram_HeapSlots->cameraWork;
  s32 *pos = scene->pos;
  s32 scale = (zoom << 16) / 100;
  s32 renderBounds[3];
  s32 measuredBounds[3];
  s32 sourceBounds[3];
  s32 span = 0x01fe0000;
  s32 alpha;
  s32 half;
  s32 width;
  u32 result;

  pos[0] = x;
  pos[1] = depth;
  pos[2] = y;
  sourceBounds[0] = 0;
  alpha = 0xc000;
  sourceBounds[1] = 0;
  sourceBounds[2] = 0;

  result = Iwram_RatioMulQ14(span, alpha);
  Projection_Set(span, result, span * 2);
  Render_ResetTransformState();
  SceneTransform_ApplyPosition(pos);
  SceneTransform_ApplyYaw((s16)scene->yaw);
  SceneTransform_ApplyPitch((s16)scene->pitch);
  {
    /* FAKEMATCH(pin): the camera pinned to r1, copied before the bound
     * stores; as Iwram_TransformVector(renderBounds, (s32 *)scene), 2 of
     * 111 instructions differ in tla-en. */
    register s32 *camera asm("r1") = (s32 *)scene;

    renderBounds[0] = 0;
    renderBounds[1] = 0;
    renderBounds[2] = span;
    Iwram_TransformVector(renderBounds, camera);
  }
  gCameraSceneParameters[3] = 120;
  gCameraSceneParameters[4] = 120;
  Render_ResetTransformState();
  Transform_SetLookAt(scene->eye, pos);
  Render_ProjectPoint(sourceBounds, measuredBounds);

  BattleCamera_SetRange(0x780000, 0x780000, (120 - measuredBounds[0]) << 8,
                        (120 - measuredBounds[1]) << 8, scale);
  half = scale * 0xff;
  width = half * 2;
  result = Iwram_RatioMulQ14(width, alpha);
  Projection_Set(width, result, half * 4);
}

// Battle presentation: center the camera between two objects.

/// Centres the camera on two objects' midpoint in x and z.
/// a, b: object IDs.
/// zoom: scale in percent.
void BattleCamera_SetViewAtPairMidpoint(s32 a, s32 b, s32 zoom) {
  struct MotionObject *obj_a = BattleObject_Get(a)->object;
  struct MotionObject *obj_b = BattleObject_Get(b)->object;
  s32 ax = obj_a->x;
  s32 bx = obj_b->x;
  s32 az = obj_a->z;
  s32 bz = obj_b->z;
  s32 x = (bx + ax) / 2;
  s32 z = (bz + az) / 2;

  BattleCamera_SetView(x, 0, z, zoom);
}

/// Fades BG2 out over the backdrop in eight frames, its blend weight falling
/// by 2 a frame from 15, while the backdrop's stays 16. Leaves REG_BLDCNT
/// blending BG2 over the backdrop.
void BattleBackground_FadeAlphaOut(void) {
  s32 step;

  REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_BD;
  for (step = 1; step <= 16; step += 2) {
    // Arithmetic on BLEND_ALPHA, not the macro's | form, which changes the
    // code.
    REG_BLDALPHA = BLEND_ALPHA(16, 16) - step;
    wait(1);
  }
}

/// Fades BG2 in over the backdrop in eight frames, its blend weight rising by
/// 2 a frame from 1, while the backdrop's stays 16. Leaves REG_BLDCNT
/// blending BG2 over the backdrop.
void BattleBackground_FadeAlphaIn(void) {
  s32 step;

  REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_BD;
  // Arithmetic on BLEND_ALPHA, not the macro's | form, which changes the
  // code.
  for (step = 1; step <= 16; step += 2) {
    REG_BLDALPHA = step + BLEND_ALPHA(0, 16);
    wait(1);
  }
}

void Func_081269ac(void) { REG_BLDCNT = 0xbf; }

s32 Func_081269bc(s32 a, s32 b) { return Iwram_MulQ16(a, b); }

struct BattleTransitionEntry {
  u32 value;
  u32 sum;
  u32 field8;
  u32 fieldc;
};

void BattleTransition_InitializeEntries(struct BattleTransitionEntry *entries) {
  u32 previous = entries[0].value;

  CAMELOT_ASM("the fixed-register identity store of transform.h")
  Transform_SetIdentity(entries);
  entries[0].sum = previous + entries[0].value;
}


struct MenuFlagWork {
  u8 unk000[0x506];
  u8 busy;
};

struct FxGroup {
  u8 unk00[32];
  struct Actor *objects[8];
  u16 values[8];
  s32 count;
};

struct DescriptorEntry {
  s16 id;
  u8 unk02[6];
};

extern void *gOverlayArea[];
extern struct DescriptorEntry gData_080eff28[];

void BattleEffect_StartTiledCanvas(s32 mode);
void BattleEffect_EndCanvasLayer(void);
void Func_0814ea58(void *effect, s32 mode);
void PartyStatus_CreateWindow(s32 flags);
s32 Func_08020310(s32 x, s32 z, s32 layer);
s32 Func_080dbde8(s32 x, s32 z, s32 layer);
u8 Func_080dbdf4(s32 x, s32 z, s32 layer, s32 value);
void Func_08108148(void);
s32 Inn_RoomPrice(s32 innId);
void Func_0810824c(void);

s32 Func_081269ec(s32 value) { return value / 16; }

// What Select picks: another value in the Japanese release.
#if LANGUAGE == LANGUAGE_JAPANESE
#define DEBUG_VALUE_SELECT 411
#else
#define DEBUG_VALUE_SELECT 544
#endif

struct DebugBattleWork {
  u8 unk000[0x854];
  s32 debugValue;
};

/// A debug choice of a battle value: with Start held, the d-pad and L/R step
/// the battle work's word at 0x854 (by 1, 10 and 100) until A takes it; Select
/// held picks DEBUG_VALUE_SELECT and R held 399; otherwise value comes back
/// unchanged.
s32 Func_081269f8(s32 value) {
  if (gInput.held & 8) {
    while (1) {
      struct DebugBattleWork *work =
          (struct DebugBattleWork *)Ram_HeapSlots->battleWork;

      if (gInput.repeat & 0x20)
        work->debugValue--;
      if (gInput.repeat & 0x10)
        work->debugValue++;
      if (gInput.repeat & 0x200)
        work->debugValue -= 100;
      if (gInput.repeat & 0x100)
        work->debugValue += 100;
      if (gInput.repeat & 0x40)
        work->debugValue -= 10;
      if (gInput.repeat & 0x80)
        work->debugValue += 10;
      if (gInput.pressed & 1) {
        value = work->debugValue;
        break;
      }
      wait(1);
    }
  }
  if (gInput.held & 4)
    value = DEBUG_VALUE_SELECT;
  // The held keys reread through a volatile pointer, a load apart from the
  // Select test's; as gInput.held, 11 of 114 instructions differ in tla-en.
  if (*(volatile u32 *)&gInput.held & 0x100)
    value = 399;
  return value;
}

// Battle presentation: toggle translucency on unit graphics.

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

// Steps the battle highlight's blend.

// The battle work block, as this function reads it.
struct HighlightWork {
  u8 unk000[0x676];
  u16 highlightStep;
  u16 highlightMode;
};

extern s8 gBattleEffect_HighlightBlendRamp[];

/// Advances the battle highlight while its mode is set: modes 6 and 4 fade the
/// blend in or out over 32 frames at half steps, mode 2 blends by the
/// sixteen-step ramp, and any other nonzero mode brightens by the ramp.
void Graphics_AdvancePaletteCycle(void) {
  struct HighlightWork *battle =
      (struct HighlightWork *)Ram_HeapSlots->battleWork;

  if (battle != NULL) {
    if (battle->highlightMode == 6) {
      u32 level = battle->highlightStep >> 1;

      if (level > 16)
        level = 16;
      REG_BLDCNT = 0x3f40;
      REG_BLDALPHA = ((0x10 - level) << 8) | level;
      battle->highlightStep++;
    } else if (battle->highlightMode == 4) {
      u32 level = battle->highlightStep >> 1;

      if (level > 16)
        level = 16;
      REG_BLDCNT = 0x3f40;
      REG_BLDALPHA = (level << 8) | (0x10 - level);
      battle->highlightStep++;
    } else if (battle->highlightMode == 2) {
      REG_BLDCNT = 0x3f40;
      REG_BLDALPHA =
          (gBattleEffect_HighlightBlendRamp[battle->highlightStep] << 8) |
          (0x10 - gBattleEffect_HighlightBlendRamp[battle->highlightStep]);
      battle->highlightStep = ((battle->highlightStep + 1) & 0xf) |
                              (battle->highlightStep > 14 ? 0x10 : 0);
    } else if (battle->highlightMode != 0) {
      REG_BLDCNT = 0x3f90;
      REG_BLDALPHA = 0x10;
      REG_BLDY = gBattleEffect_HighlightBlendRamp[battle->highlightStep];
      battle->highlightStep = ((battle->highlightStep + 1) & 0xf) |
                              (battle->highlightStep > 14 ? 0x10 : 0);
    }
  }
}

/// Mode 0 clears every actor's OBJ mode and stops the palette cycle. Any other
/// mode starts the cycle: every actor takes mode & 1, and the actors listed
/// (up to 14, ended by 0xff) take the other.
void BattleHighlight_SetModes(u16 *actors, s32 mode) {
  s16 activeActors[14];
  struct BattleWork *battle = Ram_HeapSlots->battleWork;
  u32 count;
  u32 i;

  if (mode == 0) {
    Task_Remove(Graphics_AdvancePaletteCycle);
    Io_Write16(mode, &REG_BLDY);
    BattleHighlight_ClearAllSpriteModes();
    wait(1);
    QueueIoWriteDelay2((u32)&REG_BLDCNT, 0);
  }
  if (battle != NULL && mode != 0) {
    battle->highlightMode = mode;
    battle->highlightStep = 0;
    REG_BLDY = 0;
    Io_Write16(16, &REG_BLDALPHA);

    count = BattleParty_ListActorIds(BATTLE_SIDE_BOTH, (u16 *)activeActors);
    for (i = 0; i < count; i++)
      BattleHighlight_SetSpriteMode(activeActors[i], mode & 1);

    if (actors != NULL) {
      u32 actor = *actors;

      i = 0;
      actors++;
      if (actor != 0xff) {
        do {
          BattleHighlight_SetSpriteMode(actor, (mode & 1) ^ 1);
          i++;
          if (i > 13)
            break;
          actor = *actors++;
        } while (actor != 0xff);
      }
    }
    wait(1);
    QueueIoWriteDelay2((u32)&REG_BLDCNT, 0);
    Task_Add(Graphics_AdvancePaletteCycle, TASK_PASS_FRAME);
  }
}

struct Spark {
  s32 pos[6]; // x, y, z and their velocities, 16.16
  s32 life;
};

struct Ring {
  s32 x;
  s32 y;
  s32 velX;
  s32 velY;
  s32 age;
};

struct SparkWork {
  u8 sheet[0x11c0];
  struct Spark sparks[16];
  struct Ring rings[3];
  s32 frames;
  s32 ready;
  s32 originX;
  s32 originY;
  s32 originPending;
};

typedef void (*DrawFunc)(void *dest, u8 *src, s32 x, s32 y, s32 width,
                         s32 height);

// The Broken Seal's star tables: each frame's size and sheet offset, and
// the ring's four frames.
extern const u8 Data_08129828[];
extern const s32 Data_0812980c[];
extern const s32 Data_08129830[];

// One frame of the star field: sixteen sparks fall in toward the centre
// along a damped spiral and shrink as they near it; while the field is
// young (24 frames) a spark that dies is born again on a random ring.
// Then three rings drift outward through their four frames. The canvas
// is marked ready for the transfer callback at the end.
void Func_08126e00(void) {
  s32 j;
  s32 frame;
  u32 size;
  s32 ringX;
  s32 ringY;
  s32 phase;
  void *dest;
  s32 i;
  struct SparkWork *work;
  DrawFunc draw;
  struct Spark *spark;
  struct Ring *ring;
  s32 *pos;
  s32 k;
  s32 dist;
  s32 scale;
  s32 step;
  u32 radius;
  s32 angle;
  u32 half;
  s32 x;
  s32 y;
  s32 life;
  s32 (*mul)(s32 a, s32 b);

  dest = Ram_HeapSlots->battleFxWork[1];
  work = (struct SparkWork *)Ram_HeapSlots->battleFxWork[0];
  work->ready = 0;
  draw = (DrawFunc)Ram_HeapSlots->unk64[1];
  for (i = 0; i < 16; i++) {
    spark = &work->sparks[i];
    life = spark->life;
    if (life != 0) {
      dist = FixedSqrt((spark->pos[0] >> 8) * (spark->pos[0] >> 8) +
                       (spark->pos[1] >> 8) * (spark->pos[1] >> 8) +
                       (spark->pos[2] >> 8) * (spark->pos[2] >> 8));
      if (dist <= 0xfff) {
        spark->life = 0;
      } else {
        scale = Iwram_RatioMulQ14(dist, 0x10000);
        spark->life--;
        // The multiply's entry held in a local keeps its address ahead of the
        // cursor and the axis count; called directly, 2 of 267 instructions
        // differ in tla-en.
        mul = Iwram_MulQ16;
        pos = spark->pos;
        for (k = 2; k >= 0; k--) {
          step = mul(mul(-*pos >> 8, scale), 0x13000);
          pos[3] = pos[3] - (pos[3] >> 7) + step;
          *pos += pos[3];
          pos++;
        }
      }
      life = spark->life;
    }
    if (life == 0 && work->frames <= 24) {
      angle = Random16();
      radius = Random16() + 0x10000;
      half = radius >> 1;
      spark->pos[0] = Iwram_MulQ16(Trig_Cos(angle), half);
      spark->pos[1] = Iwram_MulQ16(Trig_Sin(angle), half);
      if (spark->pos[0] & 1)
        spark->pos[0] = -spark->pos[0];
      if (spark->pos[1] & 1)
        spark->pos[1] = -spark->pos[1];
      spark->pos[2] = (u32)(Random16() + 0x8000) >> 2;
      spark->pos[3] = (-spark->pos[0] >> 7) + (spark->pos[1] >> 8);
      spark->pos[4] = (-spark->pos[1] >> 7) + (-spark->pos[0] >> 8);
      spark->pos[5] = 0;
      life = spark->life = (radius >> 13) + 1;
    }
    if (life != 0) {
      x = (spark->pos[0] >> 10) + 64;
      y = (spark->pos[1] >> 10) + 64;
      frame = life;
      if (frame < 0)
        frame = 0;
      else if (frame > 6)
        frame = 6;
      size = Data_08129828[frame];
      draw(dest, (u8 *)work + Data_0812980c[frame], x - (size >> 1),
           y - (size >> 1), size, size);
    }
  }
  draw = (DrawFunc)Ram_HeapSlots->unkb8[1];
  for (ring = work->rings, j = 2; j >= 0; j--, ring++) {
    ring->x += ring->velX;
    ring->y += ring->velY;
    ringX = ring->x >> 10;
    ringY = ring->y >> 10;
    phase = 3 - ring->age / 8;
    if (phase >= 0) {
      ring->age++;
      /* FAKEMATCH(dead): an empty once-only block after the age's store, so
       * Data_08129830's address loads after it; without it, 2 of 267
       * instructions differ in tla-en. */
      do {
      } while (0);
      draw(dest, (u8 *)work + Data_08129830[phase], ringX + 48, ringY + 48, 32,
           32);
    }
  }
  work->frames++;
  work->ready = 1;
}

s32 ColorBuffer_ScaleThreeQuarters(void *source, void *destination, s32 size);

s32 Func_08127038(void) {
  void *source = Ram_HeapSlots->battleFxWork[1];
  u8 *context = Ram_HeapSlots->battleFxWork[0];

  if (source != 0) {
    u32 *active = (u32 *)(context + 0x13c0);

    if (*active != 0) {
      *active = 0;
      return ColorBuffer_ScaleThreeQuarters(source, (void *)0x06004000, 0x4000);
    }
  }
  return (s32)source;
}

extern char ResourceId_YellowPaletteB;
extern char ResourceId_BlueRingSheet;
extern char ResourceId_RedPaletteC;
extern char ResourceId_VioletPaletteD;

s32 Resource_DecodeType01(const void *source, void *destination);
s32 Func_08138048(s32 slot, s32 modeAndFlags);
// The Broken Seal's five-argument blitter loader, named but never called.
s32 BattleEffect_BuildBlitterFar(s32 slot, s32 source, s32 destination,
                                 s32 flags, s32 mode);

// The rings' vectors go through these two polar helpers, as the native
// rebuilds 0x200 at each call (movs, lsls) and keeps only the radius and the
// multiply's entry in registers; written out, loop.c hoists 0x200 too (81 of
// 255 differ, 2026-11-08).
static __inline__ s32 PolarX(s32 angle, s32 radius) {
  return Iwram_MulQ16(Trig_Cos(angle), radius);
}

static __inline__ s32 PolarY(s32 angle, s32 radius) {
  return Iwram_MulQ16(Trig_Sin(angle), radius);
}

/// Sets up the star field behind a summon, the twin of The Broken Seal's
/// ElementGlow_InitializeStarField: sixteen sparks on random rings around the
/// centre, three rings leaving it at thirds of a turn, the blue ring sheet and
/// its palette with the mode's colour (0 yellow, 1 blue, 2 red, else violet)
/// over the first 32 colours, BG2 unscaled at the origin, the additive and max
/// blitters (Func_08138048, the veneer of BattleEffect_LoadBlitter), and the
/// two frame callbacks.
s32 Func_08127068(s32 mode) {
  struct SparkWork *work;
  struct Spark *spark;
  struct Ring *ring;
  s32 i;
  u8 *data;
  s32 zero;

  ((s32 *)Ram_HeapSlots->transitionWork)[2] = 1;
  work = HeapSlot_AllocEwram(92, sizeof *work);
  HeapSlot_Alloc(96, 0x4000);
  for (i = 0; i < 16; i++) {
    s32 angle = Random16();
    u32 radius = Random16() + 0x10000;
    u32 half = radius >> 1;

    spark = &work->sparks[i];
    spark->pos[0] = Iwram_MulQ16(Trig_Cos(angle), half);
    spark->pos[1] = Iwram_MulQ16(Trig_Sin(angle), half);
    if (spark->pos[0] & 1)
      spark->pos[0] = -spark->pos[0];
    if (spark->pos[1] & 1)
      spark->pos[1] = -spark->pos[1];
    spark->pos[2] = (u32)(Random16() + 0x8000) >> 2;
    spark->pos[3] = (-spark->pos[0] >> 7) + (spark->pos[1] >> 8);
    spark->pos[4] = (-spark->pos[1] >> 7) + (-spark->pos[0] >> 8);
    spark->pos[5] = 0;
    spark->life = (radius >> 13) + 1;
  }
  for (i = 0; i < 3; i++) {
    s32 radius = 0x1000;
    s32 angle;

    ring = &work->rings[i];
    angle = i * 0x5555;
    ring->x = PolarX(angle, radius);
    ring->y = PolarY(angle, radius);
    ring->velX = PolarX(angle, 0x200);
    ring->velY = PolarY(angle, 0x200);
    ring->age = 0;
  }
  work->frames = 0;
  work->ready = 0;
  work->originPending = 0;
  Iwram_ClearWords(Ram_HeapSlots->battleFxWork[1], 0x4000);
  data = Resource_GetTableEntry((s32)&ResourceId_BlueRingSheet);
  Iwram_CopyWords((void *)BG_PLTT, data, 128);
  data += 128;
  Resource_DecodeType01(data, work);
  switch (mode) {
  case 0:
    data = Resource_GetTableEntry((s32)&ResourceId_YellowPaletteB);
    break;
  case 1:
    data = Resource_GetTableEntry((s32)&ResourceId_BlueRingSheet);
    break;
  case 2:
    data = Resource_GetTableEntry((s32)&ResourceId_RedPaletteC);
    break;
  default:
    data = Resource_GetTableEntry((s32)&ResourceId_VioletPaletteD);
    break;
  }
  Dma_Set(data, (void *)BG_PLTT, 0x84000000 | 32, REG_DMA3);
  // A once-only block, as in The Broken Seal: it keeps the zero ahead of the
  // BG2X address, which reload derives from the DMA register's (8 without it).
  do {
    zero = 0;
    REG_BG2X = zero;
    REG_BG2Y = zero;
  } while (0);
  Io_Write16(0x100, &REG_BG2PA);
  Io_Write16(0, &REG_BG2PB);
  Io_Write16(0, &REG_BG2PC);
  Io_Write16(0x100, &REG_BG2PD);
  /* FAKEMATCH(dead): a never-taken five-argument call, which reserves the
   * frame's outgoing argument word below mode; as no call, 10 of 255
   * instructions differ in tla-en (6 with an unused u8[4] in its place). */
  if (0)
    BattleEffect_BuildBlitterFar(104, 7, 7, 3, 3);
  Func_08138048(104, 0x03);
  Func_08138048(188, 0x13);
  Task_Add(Func_08126e00, 0x480);
  return Task_Add(Func_08127038, 0x480);
}

/// Ends a star field: clears the tiles in character block 1, releases its
/// four heap slots, turns BG2 off and removes both its tasks. Called by
/// Func_08127308 once mode 0's field has played. Returns Task_Remove's
/// result, which the caller does not read.
s32 ElementGlow_End(s32 unused) {
  Iwram_ClearWords((void *)0x06004000, 0x4000);
  HeapSlot_Release(188);
  HeapSlot_Release(104);
  HeapSlot_Release(96);
  HeapSlot_Release(92);
  REG_DISPCNT = DISPCNT_OBJ_ON | DISPCNT_BG1_ON | DISPCNT_BG0_ON |
                DISPCNT_OBJ_1D_MAP | DISPCNT_MODE_1;
  Task_Remove(Func_08127038);
  return Task_Remove(Func_08126e00);
}

void Func_08127304(void) {}

#include "effect_step.h"

// The parts of the battle work this effect uses.
struct ElementSession {
  u8 unk000[0x56c];
  u16 palette[128];
  s32 brightness;
  u16 background;
};

s32 Func_0811c2b4(s32 slot, s32 *screen);
void Func_08138020(struct BattlePresentationWork *launch);
void Func_08138030(struct BattlePresentationWork *launch);
void Func_08138038(s32 mode);
void Func_08138040(void);

// Plays a unit's element effect in one of four ways: 0, the star field
// around the unit while the backdrop darkens over 25 frames (45 frames);
// 1, the prepared canvas scene of the kind, centred on the unit (40
// frames); 2, the ice fleck burst; 3, the gathering burst.
void Func_08127308(s32 unit, s32 kind, s32 mode, s32 variant) {
  struct EffectPosition position;
  struct EffectPosition position2;
  struct BattlePresentationWork launch;
  struct BattlePresentationWork launch2;
  s32 origin[2];
  struct ElementSession *session =
      (struct ElementSession *)Ram_HeapSlots->battleWork;
  volatile u16 *ime;
  struct SparkWork *work;
  s32 frame;
  s32 *brightness;
  struct EffectPosition *pos;

  wait(1);
  Func_0812628c(1, session->background, 0);
  Iwram_ClearWords((void *)0x06004000, 0x4000);
  QueueIoWriteDelay2(0x04000000, 0x3741);
  QueueIoWriteDelay2(0x0400000c, 0x784);
  QueueIoWriteDelay2(0x04000050, 0x3f44);
  wait(1);
  Io_Write16(240, &REG_WIN0H);
  Io_Write16(0x1088, &REG_WIN0V);
  Io_Write16(63, &REG_WININ);
  Io_Write16(17, &REG_WINOUT);
  if (mode == 0) {
    QueueIoWriteDelay2(0x04000052, 0x100e);
    Func_08127068(kind);
    for (frame = 0, pos = &position; frame <= 44; frame++) {
      brightness = &session->brightness;
      ime = &REG_IME;
      work = (struct SparkWork *)Ram_HeapSlots->battleFxWork[0];
      if (frame <= 24)
        ColorBuffer_Scale(session->palette, (u16 *)0x050000c0,
                          *brightness = 0x10000 - frame * 1092, 128);
      Func_0811c2b4(unit, (s32 *)pos);
      work->originX = (64 - pos->x) << 8;
      work->originY = (64 - pos->y) << 8;
      IO_WRITE_QUEUE_PUSH_IME(ime, 0x04000028, (u32)&work->originX, 0x84000002);
      work->originPending = 1;
      wait(1);
    }
    ElementGlow_End(kind);
  } else if (mode == 1) {
    Func_08138038(kind);
    for (frame = 0; frame < 40; frame++) {
      Func_0811c2b4(unit, (s32 *)&position2);
      origin[0] = (64 - position2.x) << 8;
      origin[1] = (64 - position2.y) << 8;
      IO_WRITE_QUEUE_PUSH(0x04000028, (u32)origin, 0x84000002);
      wait(1);
    }
    Func_08138040();
  } else if (mode == 2) {
    launch.flags = 0;
    launch.kind = kind;
    launch.variant = variant;
    launch.actor = unit;
    launch.actors[0] = unit;
    launch.firstTarget = unit;
    launch.count = 1;
    launch.initialValue = 1;
    Func_08138020(&launch);
  } else {
    launch2.flags = 0;
    launch2.kind = kind;
    launch2.variant = 0;
    launch2.actor = unit;
    launch2.firstTarget = unit;
    launch2.actors[0] = unit;
    launch2.count = 1;
    launch2.initialValue = 1;
    Func_08138030(&launch2);
  }
}

/// Puts the background's palette back undarkened, at level 0.
/// Called by Func_0811df70 and Func_0811f088.
void BattleBackground_RestorePalette(void) {
  BattleBackground_SetPaletteLevel(0, 0);
}

/// Removes the VCount interrupt's handler; nothing calls it.
void Irq_RemoveVCountHandler(void) { Irq_SetHandler(IRQ_VCOUNT, 0, 0); }

#include "battle_formation.h"

/// The Broken Seal's BattleFormation_AverageLevel without its level check:
/// the average level of a formation's enemy kinds, counting only kinds the
/// player has met (flag 0x608 + id, or all with flag 0x174); -1 for an empty
/// formation, -2 when one present kind is unmet, -3 when none counts.
s32 Func_08127588(s32 recordId) {
  struct BattleFormationRecord *record;
  const struct EnemyDefinition *enemy;
  u32 memberIndex;
  s32 enemyId;
  s32 levelTotal;
  s32 enemyCount;
  // Unused: gives the stack frame its 28 bytes.
  UNUSED s32 levels[7];

  enemyCount = 0;
  levelTotal = 0;
  record = &gBattleFormation_Records[recordId];
  for (memberIndex = 0; memberIndex < 5; memberIndex++) {
    if (record->minimumCounts[memberIndex] != 0)
      break;
  }
  if (memberIndex == 5)
    return -1;
  for (memberIndex = 0; memberIndex < 5; memberIndex++) {
    if (record->maximumCounts[memberIndex] == 0)
      continue;
    enemyId = record->memberIds[memberIndex];
    enemy = Enemy_GetDefinitionFar(enemyId + 8);
    if (enemy == 0)
      continue;
    if (!GameFlag_Test(0x174) && !GameFlag_Test(enemyId + 0x608))
      return -2;
    levelTotal += enemy->level;
    enemyCount++;
  }
  if (enemyCount == 0)
    return -3;
  return levelTotal / enemyCount;
}

/// One kept formation: its record and its average level, -1 while empty.
struct FormationCandidate {
  s16 recordId;
  s32 score : 16;
};

/// The enemy flags (0x600 + id) cleared before every search.
extern const u16 Data_08130c5c[];

u8 GameFlag_GetByte(s32 flag);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);

/// Keeps up to thirty-two eligible formations near the party's adjusted
/// average level, chooses one randomly and returns its level adjustment
/// through outMargin. It first sets flags from others, clears the enemy flags
/// of Data_08130c5c, sets flags 71, 230 and 117, and searches 660 formations.
// As The Broken Seal's function of the same name, which has no flag setup.
s32 BattleFormation_SelectLevelMatchedCandidate(s32 *outMargin) {
  s32 matchCount = 0;
  struct FormationCandidate *pool = Heap_AllocEwram(128);
  u16 party[8];
  s32 targetLevel = 0;
  s32 unitCount = BattleParty_PrepareActiveOwners(party);
  s32 candidateOrUnitIndex;
  s32 poolIndex;
  s32 result;

  if (unitCount > 0) {
    u16 *unitCursor = party;

    candidateOrUnitIndex = unitCount;
    do {
      u8 level = ((struct BattleUnit *)Unit_Get__f11f66c(*unitCursor))->level;

      candidateOrUnitIndex--;
      unitCursor++;
      targetLevel += level;
    } while (candidateOrUnitIndex != 0);
  }

  targetLevel = targetLevel / unitCount;
  targetLevel += (s8)GameFlag_GetByte(0x3f8);
  if (targetLevel <= 0)
    targetLevel = 1;
  if (targetLevel > 99)
    targetLevel = 99;

  for (candidateOrUnitIndex = 0; candidateOrUnitIndex <= 31;
       candidateOrUnitIndex++)
    pool[candidateOrUnitIndex].score = -1;

  if (GameFlag_Test(0x65b))
    GameFlag_SetBit(0x65f);
  if (GameFlag_Test(0x65c))
    GameFlag_SetBit(0x65f);
  if (GameFlag_Test(0x65d))
    GameFlag_SetBit(0x65f);
  if (GameFlag_Test(0x65e))
    GameFlag_SetBit(0x65f);
  if (GameFlag_Test(0x66b))
    GameFlag_SetBit(0x66c);
  if (GameFlag_Test(0x668))
    GameFlag_SetBit(0x66a);
  if (GameFlag_Test(0x669))
    GameFlag_SetBit(0x66a);
  GameFlag_ClearBit(0x771);
  if (GameFlag_Test(0x778))
    GameFlag_SetBit(0x771);

  for (candidateOrUnitIndex = 0; (u32)candidateOrUnitIndex <= 86;
       candidateOrUnitIndex++)
    GameFlag_ClearBit(Data_08130c5c[candidateOrUnitIndex] + 0x600);
  GameFlag_SetBit(71);
  GameFlag_SetBit(230);
  GameFlag_SetBit(117);

  for (candidateOrUnitIndex = 0; (u32)candidateOrUnitIndex < 660;
       candidateOrUnitIndex++) {
    s32 score = Func_08127588(candidateOrUnitIndex);

    if (score >= 0 && score <= targetLevel + 3) {
      s32 lowestIndex = -1;
      s32 lowestLevel;

      for (lowestLevel = 999, poolIndex = 0; poolIndex <= 31; poolIndex++) {
        if (pool[poolIndex].score < lowestLevel) {
          lowestLevel = pool[poolIndex].score;
          lowestIndex = poolIndex;
        }
      }

      if (lowestIndex >= 0) {
        pool[lowestIndex].score = score;
        pool[lowestIndex].recordId = candidateOrUnitIndex;
        matchCount++;
      }
    }
  }

  if (matchCount > 32)
    matchCount = 32;

  if (matchCount != 0) {
    struct FormationCandidate *chosen = &pool[(matchCount * Random16()) >> 16];

    result = chosen->recordId;
    *outMargin = targetLevel - chosen->score;
  } else {
    *outMargin = matchCount;
    result = 1;
  }

  Heap_Release(pool);
  return result;
}

struct OwnerElementStats {
  s16 power;
  s16 res;
};

struct OwnerStats {
  s16 maxHp;
  s16 maxPp;
  s16 hp;
  s16 pp;
  u16 atk;
  u16 def;
  u16 agi;
  u8 luck;
  u8 unk0f[5];
  struct OwnerElementStats element[4];
};

struct OwnerState {
  u8 unk00[15];
  u8 level;
  struct OwnerStats stats;
};

/// Raises an enemy by levels, The Broken Seal's BattleEnemy_AdjustLevel with
/// HP capped at 16383 instead of 9999. Returns nothing, though declared s32 as
/// there.
s32 Func_0812786c(s32 owner, s32 levels) {
  struct OwnerStats *base;
  struct OwnerState *state;
  struct OwnerStats *stats;
  s32 value;
  s32 floor;
  s32 i;

  base = Heap_AllocEwram(sizeof(struct OwnerStats));
  state = Unit_Get__f11f66c(owner);
  stats = &state->stats;
  Iwram_CopyWords(base, stats, sizeof(struct OwnerStats));

  value = stats->maxHp;
  value += __divsi3(levels * 97, 10);
  floor = __divsi3(base->maxHp * 7, 10);
  if (value < floor)
    value = floor;
  if (value > 16383)
    value = 16383;
  stats->maxHp = value;

  value = state->stats.maxPp;
  value += __divsi3(levels * 15, 10);
  floor = __divsi3(base->maxPp * 7, 10);
  if (value < floor)
    value = floor;
  if (value > 9999)
    value = 9999;
  state->stats.maxPp = value;

  floor = __divsi3(levels * 123, 10);
  value = state->stats.atk;
  value += floor;
  floor = __divsi3(base->atk * 7, 10);
  if (value < floor)
    value = floor;
  if (value > 999)
    value = 999;
  state->stats.atk = value;

  floor = __divsi3(levels * 33, 10);
  value = state->stats.def;
  value += floor;
  floor = __divsi3(base->def * 7, 10);
  if (value < floor)
    value = floor;
  if (value > 999)
    value = 999;
  state->stats.def = value;

  floor = __divsi3(levels * 51, 10);
  value = state->stats.agi;
  value += floor;
  floor = __divsi3(base->agi * 7, 10);
  if (value < floor)
    value = floor;
  if (value > 999)
    value = 999;
  state->stats.agi = value;

  for (i = 0; i < 4; i++) {
    value = state->stats.element[i].power + levels * 15;
    floor = __divsi3(base->element[i].power * 7, 10);
    if (value < floor)
      value = floor;
    if (value > 200)
      value = 200;
    state->stats.element[i].power = value;
  }

  state->level += levels;
  Unit_CalculateFar(owner);
  Heap_Release(base);
}

struct UnitBaseStats {
  s16 hp;
  s16 pp;
  s16 hpGauge;
  s16 ppGauge;
  u16 attack;
  u16 defense;
  u8 unk0c[24];
};

/// Raises a unit's base HP by half and base Attack and Defense by a quarter (at
/// least 7/10 of the old values, at most 16383 and 999), from a copy of its
/// base block, then recalculates its stats.
// No ☀️ twin found.
void Func_08127a0c(s32 unitId) {
  struct UnitBaseStats *saved = Heap_AllocEwram(36);
  struct BattleUnit *unit = Unit_Get__f11f66c(unitId);
  s32 value;
  s32 floor;

  Iwram_CopyWords(saved, &unit->baseHp, 36);
  value = unit->baseHp * 3 / 2;
  floor = saved->hp * 7 / 10;
  if (value < floor)
    value = floor;
  if (value > 16383)
    value = 16383;
  unit->baseHp = value;
  value = unit->baseAttack * 5 / 4;
  floor = saved->attack * 7 / 10;
  if (value < floor)
    value = floor;
  if (value > 999)
    value = 999;
  unit->baseAttack = value;
  value = unit->baseDefense * 5 / 4;
  floor = saved->defense * 7 / 10;
  if (value < floor)
    value = floor;
  if (value > 999)
    value = 999;
  unit->baseDefense = value;
  Unit_CalculateFar(unitId);
  Heap_Release(saved);
}

// The letters cycle through CH_CNT values: 26 in the Japanese release, 9
// elsewhere, as Func_08128228 reads them back.
#if LANGUAGE == LANGUAGE_JAPANESE
#define CH_CNT 26
#else
#define CH_CNT 9
#endif

struct SummonChargeState {
  u8 unk00[0x10];
  u16 classIds[6];
  u32 usedMasks[6];
  s8 channels[6];
  u8 unk3a[6];
  u8 count;
};

s32 BattleEnemy_ClaimNameLetter(s32 no, s32 n) {
  struct SummonChargeState *w;
  s32 num;
  s32 i;
  s32 retry;

  w = (struct SummonChargeState *)Ram_HeapSlots->battleWork;
  num = w->count;
  for (i = 0; i < num; i++) {
    if (w->classIds[i] == no)
      break;
  }
  if (i != num) {
    retry = 0;
    if (w->channels[i] < 0) {
      w->channels[i] = 1;
      w->usedMasks[i] = 3;
      return 0x8001;
    }
    for (; retry <= 31; retry++) {
      w->channels[i] = (w->channels[i] + 1) % CH_CNT;
      if ((w->usedMasks[i] & (1 << w->channels[i])) == 0)
        break;
    }
    w->usedMasks[i] |= 1 << w->channels[i];
    return w->channels[i];
  }
  if (num <= 4) {
    w->channels[num] = -1;
    w->classIds[num] = no;
    w->usedMasks[num] = 0;
    w->count = num + 1;
    return CH_CNT;
  }
  return -1;
}

#include "tla_edition.h"

#define LIST_MARKER_CHAR SUMMON_CHANNEL_MARKER

struct BattleActorDefinition {
  u8 name[14];
  u8 unk0e[0x11b];
  u8 unavailable;
  u8 unk12a[0x20];
  u16 classId;
};

s32 BattleEnemy_ReleaseNameLetter(s32 actorId) {
  struct SummonChargeState *state =
      (struct SummonChargeState *)Ram_HeapSlots->battleWork;
  struct BattleActorDefinition *actor;
  s32 count = state->count;
  s32 index;
  s32 nameLength;
  s32 bit;
  s32 classId;

  actor = Unit_Get__f11f66c(actorId);
  if (actor->unavailable != 0)
    return;

  classId = actor->classId;
  for (index = 0; index < count; index++) {
    if (state->classIds[index] == classId)
      break;
  }
  if (index == count || state->usedMasks[index] == 0)
    return;

  for (nameLength = 0; nameLength <= 13; nameLength++) {
    if (actor->name[nameLength] == 0)
      break;
  }

  bit = 32;
  if (nameLength > 0)
    bit = actor->name[nameLength - 1] - LIST_MARKER_CHAR;
  state->usedMasks[index] &= ~(1 << bit);
}

s32 BattleEnemy_LetterFirstOfClass(s32 enemyId) {
  struct BattleUnit *summon;
  s32 objectIndex;
  s32 slot;
  s32 nextSlot;
  u8 occupied;
  u8 marker;

  objectIndex = 0;
  marker = LIST_MARKER_CHAR;
scan_objects:
  summon = Unit_Get__f11f66c(objectIndex + 0x80);
  occupied = summon->side;
  if (occupied != 1)
    goto next_object;
  if (summon->enemyId != enemyId)
    goto next_object;
  slot = 0;
  if (summon->name[0] != 0)
    goto scan_slots;
  summon->name[0] = marker;
  summon->name[occupied] = slot;
  return;
scan_slots:
  slot++;
  if (slot > 13)
    return;
  occupied = summon->name[slot];
  if (occupied != 0)
    goto scan_slots;
  nextSlot = slot + 1;
  summon->name[slot] = marker;
  summon->name[nextSlot] = occupied;
  return;
next_object:
  objectIndex++;
  if (objectIndex <= 5)
    goto scan_objects;
}

s32 BattleFormation_GetRandomMember(s32 recordId) {
  s32 *choicesBase;
  s32 choices[5];
  struct BattleFormationRecord *record;
  s32 *choiceCursor;
  u8 *maximumCount;
  u16 *memberId;
  s32 remaining;
  u32 count;

  count = 0;
  if ((u32)recordId >= 660)
    recordId = 1;

  record = &gBattleFormation_Records[recordId];
  choicesBase = choices;
  memberId = record->memberIds;
  maximumCount = record->maximumCounts;
  choiceCursor = choicesBase;
  remaining = 4;
  do {
    if (*maximumCount++ != 0) {
      s32 id = *memberId;

      count++;
      *choiceCursor++ = id + 8;
    }
    remaining--;
    memberId++;
  } while (remaining >= 0);

  return choices[(Random16() * count) >> 16];
}

struct FormationBattleWork {
  u8 unk00[0x3c];
  u16 firstDefeated;
  u16 defeatState;
  u8 unk40;
  u8 unk41;
  u8 battleType;
  u8 unk43[0x825];
  u8 unk868;
};

s32 Func_08127cd4(s32 recordId) {
  u16 *enemyClasses;
  struct FormationBattleWork *battle;
  struct BattleFormationRecord *record;
  s32 freeSpriteSlots;
  s32 enemyCount;
  s32 levelAdjustment;
  u16 listBuffer[14];
  s32 extraCounts[5];
  s32 memberCounts[5];
  s32 memberOrder[5];
  s32 changed;
  s32 additionalRoom;
  s32 spriteSlots;
  s32 fittingCount;
  s32 classId;
  s32 remainingKinds;
  s32 copies;
  u32 memberOrEnemyIndex;
  s32 memberIndex;

  battle = (struct FormationBattleWork *)Ram_HeapSlots->battleWork;
  levelAdjustment = 0;
  // Through a byte pointer, which keeps the level adjustment stored first.
  *(u8 *)&battle->unk40 = 0;
  enemyClasses = listBuffer;
  if (GameFlag_Test(0x173))
    recordId = BattleFormation_SelectLevelMatchedCandidate(&levelAdjustment);
  if ((u32)recordId >= 660)
    recordId = 1;
  record = &gBattleFormation_Records[recordId];
  for (memberIndex = 0;
       memberIndex <= 4 && record->minimumCounts[memberIndex] == 0;
       memberIndex++)
    ;
  if (memberIndex == 5)
    record = &gBattleFormation_Records[1];

  freeSpriteSlots = 6;
  enemyCount = 0;
  for (memberOrEnemyIndex = 0; memberOrEnemyIndex <= 4; memberOrEnemyIndex++) {
    if (record->minimumCounts[memberOrEnemyIndex] != 0) {
      s32 requiredSlots =
          2 -
          (BattleClass_IsSmall(record->memberIds[memberOrEnemyIndex] + 8) != 0);

      freeSpriteSlots -=
          requiredSlots * record->minimumCounts[memberOrEnemyIndex];
    }
  }

  for (memberOrEnemyIndex = 0; memberOrEnemyIndex <= 4; memberOrEnemyIndex++) {
    s32 minimum;
    s32 maximum;

    memberCounts[memberOrEnemyIndex] = minimum =
        record->minimumCounts[memberOrEnemyIndex];
    maximum = record->maximumCounts[memberOrEnemyIndex];
    additionalRoom = maximum - minimum;
    if (additionalRoom > 0) {
      s32 memberSlots =
          2 -
          (BattleClass_IsSmall(record->memberIds[memberOrEnemyIndex] + 8) != 0);

      fittingCount = freeSpriteSlots / memberSlots;
      if (fittingCount < additionalRoom)
        additionalRoom = fittingCount;
      extraCounts[memberOrEnemyIndex] =
          ((additionalRoom + 1) * Random16()) >> 16;
    } else {
      extraCounts[memberOrEnemyIndex] = 0;
    }
  }

  do {
    changed = 0;
    for (memberOrEnemyIndex = 0; memberOrEnemyIndex <= 4;
         memberOrEnemyIndex++) {
      classId = record->memberIds[memberOrEnemyIndex] + 8;
      if (extraCounts[memberOrEnemyIndex] != 0) {
        spriteSlots = 2 - (BattleClass_IsSmall(classId) != 0);
        if (spriteSlots > freeSpriteSlots) {
          extraCounts[memberOrEnemyIndex] = 0;
        } else {
          memberCounts[memberOrEnemyIndex]++;
          extraCounts[memberOrEnemyIndex]--;
          freeSpriteSlots -= spriteSlots;
          changed = 1;
        }
      }
    }
  } while (changed);

  battle->battleType = record->unk14[0];
  switch (battle->battleType) {
  case 0:
    for (memberOrEnemyIndex = 0; memberOrEnemyIndex <= 4; memberOrEnemyIndex++)
      memberOrder[memberOrEnemyIndex] = memberOrEnemyIndex;
    for (memberOrEnemyIndex = 0; memberOrEnemyIndex <= 9;
         memberOrEnemyIndex++) {
      s32 first = (Random16() * 5) >> 16;
      s32 second = (Random16() * 5) >> 16;
      s32 swap = memberOrder[first];

      memberOrder[first] = memberOrder[second];
      memberOrder[second] = swap;
    }
    for (memberOrEnemyIndex = 0; memberOrEnemyIndex <= 4;
         memberOrEnemyIndex++) {
      s32 copyIndex;
      s32 slot = memberOrder[memberOrEnemyIndex];

      for (copyIndex = 0; copyIndex < memberCounts[slot]; copyIndex++)
        enemyClasses[enemyCount++] = record->memberIds[slot] + 8;
    }
    break;
  case 1:
    for (;;) {
      remainingKinds = 0;
      for (memberOrEnemyIndex = 0; memberOrEnemyIndex <= 4;
           memberOrEnemyIndex++) {
        if (memberCounts[memberOrEnemyIndex] != 0)
          memberOrder[remainingKinds++] = memberOrEnemyIndex;
      }
      if (remainingKinds == 0)
        break;
      spriteSlots = memberOrder[(Random16() * remainingKinds) >> 16];
      enemyClasses[enemyCount++] = record->memberIds[spriteSlots] + 8;
      memberCounts[spriteSlots]--;
    }
    break;
  default:
    for (memberOrEnemyIndex = 0; memberOrEnemyIndex <= 4;
         memberOrEnemyIndex++) {
      for (copies = 0; copies < memberCounts[memberOrEnemyIndex]; copies++)
        enemyClasses[enemyCount++] = record->memberIds[memberOrEnemyIndex] + 8;
    }
    break;
  }
  enemyClasses[enemyCount] = 0;
  battle->firstDefeated = 6;
  battle->defeatState = 0;
  for (memberOrEnemyIndex = 128; memberOrEnemyIndex < 134; memberOrEnemyIndex++)
    Iwram_ClearWords(Unit_Get__f11f66c(memberOrEnemyIndex), BATTLE_UNIT_SIZE);

  memberOrEnemyIndex = 0;
  if (enemyClasses[0] == 0)
    goto assigned;
assign_enemy:
  // Repeated enemy classes get distinct suffixes within this encounter.
  {
    s32 suffixCode =
        BattleEnemy_ClaimNameLetter(enemyClasses[memberOrEnemyIndex], 1);
    struct BattleUnit *unit;
    s32 enemyId;

    if (suffixCode & 0x8000)
      BattleEnemy_LetterFirstOfClass(enemyClasses[memberOrEnemyIndex]);
    BattleUnit_SetFar(memberOrEnemyIndex + 128,
                      enemyClasses[memberOrEnemyIndex], suffixCode & 0x7fff);
    unit = Unit_Get__f11f66c(memberOrEnemyIndex + 128);
    if (levelAdjustment != 0)
      Func_0812786c(memberOrEnemyIndex + 128, levelAdjustment);
    else if (!GameFlag_Test(0x173) && GameFlag_Test(0x2e))
      Func_08127a0c(memberOrEnemyIndex + 128);
    battle->unk868 = 0;
    enemyId = unit->enemyId;
    switch (enemyId) {
    case 101:
    case 102:
    case 103:
      if (!GameFlag_Test(0x173))
        battle->unk868 = 1;
      break;
    }
  }
  memberOrEnemyIndex++;
  if (memberOrEnemyIndex > 5)
    goto assigned;
  if (enemyClasses[memberOrEnemyIndex] != 0)
    goto assign_enemy;
assigned:
  return memberOrEnemyIndex;
}

u32 BattleClass_GetDefeatSoundKind(s32 index) {
  u32 kind = gSummon_EntryTable[index].layoutKind;

  if ((s32)kind > 4)
    kind = -1U;
  return kind;
}

s32 BattleClass_GetSprite(s32 index) {
  if ((u32)index > 386)
    return gSummon_EntryTable[0].animation;
  return gSummon_EntryTable[index].animation;
}

s32 BattleClass_GetPalette(s32 index) {
  if ((u32)index > 386)
    return gSummon_EntryTable[0].animation;
  return gSummon_EntryTable[index].layoutBits1;
}

s32 BattleClass_IsSmall(s32 index) {

  s32 result;
  u32 flags;

  if ((u32)index > 386)
    return 0;
  result = 0;
  flags = gSummon_EntryTable[index].spriteBit0;
  if (flags)
    result = 1;
  return result;
}

u32 BattleClass_GetAttackEffect(u32 index) {
  u16 value;

  if (index > 386)
    return 1;
  value = gSummon_EntryTable[index].spriteBits1;
  return value ? value : 1;
}

u32 BattleClass_GetAttackHop(u32 index) {
  u16 value;

  if (index > 386)
    return 0;
  value = gSummon_EntryTable[index].spriteBits5;
  return value ? value : 0;
}

s32 Summon_IsEntrySecondaryFlagged(s32 index) {
  if ((u32)index > 386)
    return 0;
  return gSummon_EntryTable[index].layoutBit0;
}

u32 BattleClass_GetEffectHeight(u32 index) {
  u8 *table;
  u32 offset;

  if (index > 386)
    return 0;
  table = (u8 *)gSummon_EntryTable;
  offset = index * 8;
  offset += 4;
  return table[offset];
}

// XXX: what this encodes, and whether item ids here are wider than The Broken
// Seal's 9 bits; its callers would settle it.
u32 BattleSpoils_RankItem(u32 value) {
  u32 bank = 0;
  u32 base = value & 0x1ff;
  if (base == 0)
    return 0;
  {
    u8 flags = Item_Get(base)->flags;

    if (flags & 8)
      bank = 1;
    bank <<= 1;
    if (flags & 4)
      bank++;
    bank <<= 9;
    bank += base;
  }
  return bank;
}

/// Empties the battle's spoils and the party's pending item, as The Broken
/// Seal's BattleSpoils_Clear does. Called by Func_081197f0.
void BattleSpoils_Clear(void) {
  struct BattleWork *battle;
  struct BattleSpoils *spoils;
  u16 *items;
  s32 index;

  battle = Ram_HeapSlots->battleWork;
  spoils = &battle->spoils;
  gMainRoutineWork.tally25c = 0;
  spoils->coins = 0;
  spoils->experience = 0;
  spoils->defeated = 0;
  items = battle->spoils.items;
  for (index = 3; index >= 0; index--)
    items[index] = 0;
}

// The scan looks for the first of FIRST_DIGIT_COUNT character codes from
// FIRST_DIGIT_BASE: the digits 1 to 9 outside Japan, and the codes 65 to 90 in
// the Japanese release.
#if LANGUAGE == LANGUAGE_JAPANESE
#define FIRST_DIGIT_BASE 65
#define FIRST_DIGIT_COUNT 26
#else
#define FIRST_DIGIT_BASE 49
#define FIRST_DIGIT_COUNT 9
#endif

s32 Func_08128228(u8 *s) {
  s32 i;

  for (i = 0; i <= 13; i++) {
    if ((u8)(s[i] - FIRST_DIGIT_BASE) <= FIRST_DIGIT_COUNT - 1)
      return s[i] - FIRST_DIGIT_BASE;
  }
  return FIRST_DIGIT_COUNT;
}

struct DefeatSpoils {
  s32 coins;
  s32 experience;
  s32 defeated;
  u16 items[4];
  u16 lastEnemyIds[2];
  u8 lastDigits[2];
  s8 lastSlot;
  s8 lastCount;
};

struct DefeatBattleWork {
  u8 unk00[0x10];
  u16 enemyClasses[6];
  u8 unk1c[0x20];
  u16 firstDefeated;
  u16 defeatState;
  u8 unk40[0x510];
  struct DefeatSpoils spoils;
};

static __inline__ s32 BattleEnemy_CalculateCoinReward(struct BattleUnit *unit,
                                                      u16 *reward) {
  s32 rollIndex;
  s32 bonus = 0;
  s32 base;
  s32 minimumBonus;

  for (rollIndex = 0; rollIndex < (u8)(unit->level / 10U) + 1; rollIndex++)
    bonus += ((Random16() * 6) >> 16) + 1;
  base = *reward;
  minimumBonus = base * 3 / 10;
  if (bonus < minimumBonus)
    bonus = minimumBonus;
  return bonus + base;
}

static __inline__ s32
BattleEnemy_CalculateExperienceReward(struct BattleUnit *unit, u16 *reward) {
  s32 rollIndex;
  s32 bonus = 0;
  s32 base;
  s32 minimumBonus;

  for (rollIndex = 0; rollIndex < (u8)(unit->level / 10U) + 1; rollIndex++)
    bonus += ((Random16() * 4) >> 16) + 1;
  base = *reward;
  minimumBonus = base * 3 / 10;
  if (bonus < minimumBonus)
    bonus = minimumBonus;
  return bonus + base;
}

/// Counts a fallen enemy and adds its spoils, The Broken Seal's
/// BattleEnemy_RecordDefeat with the battle work from its heap slot. This game
/// also keeps the last two defeated enemies (their ids and Func_08128228's
/// first digit of their names, alternating slots, with a count up to 2) and
/// sets the seen flag only for enemy ids below 0x200.
s32 Func_0812824c(s32 unitId, s32 djinnBonus) {
  struct BattleUnit *unit;
  const struct EnemyDefinition *enemy;
  struct DefeatBattleWork *battle;
  struct DefeatSpoils *spoils;
  s32 itemOrClassIndex;
  s32 dropShift;
  s32 classSlot;
  s32 lowestItemPriority;
  s32 lowestSlot;
  s32 itemPriority;
  s32 slot;
  s32 classIndex;

  unit = Unit_Get__f11f66c(unitId);
  battle = (struct DefeatBattleWork *)Ram_HeapSlots->battleWork;
  spoils = &battle->spoils;
  if (unitId >= 0 && unitId < 8)
    return -1;
  if (unit->classIndex != 0)
    return -2;
  slot = spoils->lastSlot = (spoils->lastSlot + 1) % 2;
  spoils->lastEnemyIds[slot] = unit->enemyId;
  spoils->lastDigits[slot] = Func_08128228((u8 *)unit);
  if (spoils->lastCount <= 1)
    spoils->lastCount++;
  classSlot = 0;
  classIndex = 0;
  if (battle->enemyClasses[0] != unit->enemyId) {
    do {
      if (++classIndex > 5)
        break;
    } while (battle->enemyClasses[classIndex] != unit->enemyId);
  }
  if (classIndex != 6)
    classSlot = classIndex;
  if (battle->defeatState != 2) {
    if (classSlot < battle->firstDefeated)
      battle->firstDefeated = classSlot;
    if (spoils->defeated)
      battle->defeatState = 1;
  }
  spoils->defeated++;
  if (GameFlag_Test(0x173))
    return 0;
  if (unit->enemyId < 0x200)
    GameFlag_SetBit(unit->enemyId + 0x600);
  enemy = Enemy_GetDefinitionFar(unit->enemyId);
  if (djinnBonus != 0) {
    if (enemy->coins != 0)
      spoils->coins += BattleEnemy_CalculateCoinReward(unit, &enemy->coins);
    if (enemy->experience != 0)
      spoils->experience +=
          BattleEnemy_CalculateExperienceReward(unit, &enemy->experience);
  } else {
    spoils->coins += enemy->coins;
    spoils->experience += enemy->experience;
  }
  if (enemy->item == 0 || enemy->itemChance == 0)
    return 0;
  itemOrClassIndex = 0;
  if (spoils->items[0] != enemy->item) {
    do {
      if (++itemOrClassIndex > 3)
        break;
    } while (spoils->items[itemOrClassIndex] != enemy->item);
  }
  if (itemOrClassIndex == 4) {
    dropShift = enemy->itemChance;
    if (djinnBonus != 0)
      dropShift -= 2;
    if (dropShift < 0)
      dropShift = 0;
    if ((0x20000 >> dropShift) > (Battle_RandomFar() & 0xffff)) {
      lowestSlot = -1;
      lowestItemPriority = 0x40000000;
      for (itemOrClassIndex = 0; itemOrClassIndex <= 3; itemOrClassIndex++) {
        itemPriority = BattleSpoils_RankItem(spoils->items[itemOrClassIndex]);
        if (itemPriority < lowestItemPriority) {
          lowestItemPriority = itemPriority;
          lowestSlot = itemOrClassIndex;
        }
      }
      if ((s32)BattleSpoils_RankItem(enemy->item) > lowestItemPriority)
        spoils->items[lowestSlot] = enemy->item;
    }
  }
  return 0;
}

#define ACTION_ID_MASK OWNER_ACTION_ID_MASK

extern u16 Data_0200049c;

s32 Func_0811a24c(s32 side, u16 *units);
s32 Func_080ad0b0(s32 unitId, s16 *gains);
s32 PartyInventory_AddFar(s32 item);

void Battle_AwardSpoils(void) {
  struct BattleSpoils *spoils;
  s32 living;
  u16 *list;
  s32 count;
  s32 i;
  s32 unitId;
  struct BattleUnit *unit;
  struct BattleUnit *backup;
  s32 cnt;
  u32 learned;
  s32 j;
  s32 best;
  s32 bestSlot;
  s32 slot;
  u16 *found;
  s32 value;
  s16 gains[8];
  u16 units[10];

  spoils = (struct BattleSpoils *)((u8 *)Ram_HeapSlots->battleWork + 0x550);
  if (spoils->experience != 0) {
    MessageArgument_PushFar(spoils->experience, 5);
    // "You got N experience points."
    Message_ShowAndWaitCoreFar((s32)MsgGotExperience);
    BattleMessage_WaitForAdvance();
  }
  list = units;
  living = BattleParty_ListLivingUnits(1, list);
  count = Func_0811a24c(1, list);
  backup = Heap_AllocEwram(sizeof(struct BattleUnit));
  for (i = 0; i < count; i++) {
    unitId = list[i];
    unit = Unit_Get__f11f66c(unitId);
    if (i >= living)
      unit->experience += (u32)spoils->experience >> 1;
    else
      unit->experience += spoils->experience;
    while (Iwram_CopyWords(backup, unit, sizeof(struct BattleUnit)),
           Func_080ad0b0(unitId, gains) != 0) {
      Sound_PlayFar(89);
      MessageArgument_ClearAllFar();
      MessageArgument_PushFar(unit->classIndex, 3);
      MessageArgument_PushFar(list[i], 1);
      MessageArgument_PushFar(unit->level, 5);
      // "<name> is now a level N ..."
      Message_ShowAndWaitCoreFar((s32)MsgLevelUp);
      BattleMessage_WaitForAdvance();
      for (cnt = 0; cnt < 32; cnt++) {
        learned = unit->actionSlots[cnt].encodedAction;
        if ((learned & ACTION_ID_MASK) && (learned >> 15)) {
          for (j = 0; j < 32; j++) {
            if (learned == backup->actionSlots[j].encodedAction)
              break;
          }
          if (j == 32) {
            MessageArgument_ClearAllFar();
            MessageArgument_PushFar(unit->classIndex, 3);
            MessageArgument_PushFar(unitId, 1);
            MessageArgument_PushFar(learned & ACTION_ID_MASK, 4);
            Sound_PlayFar(154);
            // "Mastered the <class>'s <ability>"
            Message_ShowAndWaitCoreFar((s32)MsgMasteredAbility);
            BattleMessage_WaitForAdvance();
          }
        }
      }
      if (gains[2] != 0) {
        MessageArgument_PushFar(gains[2], 5);
        // "Maximum HP rises by N!"
        Message_ShowAndWaitCoreFar((s32)MsgMaxHpRises);
        BattleMessage_WaitForAdvance();
      }
      if (gains[3] != 0) {
        MessageArgument_PushFar(gains[3], 5);
        Message_ShowAndWaitCoreFar((s32)MsgMaxPpRises);
        BattleMessage_WaitForAdvance();
      }
      if (gains[4] != 0) {
        MessageArgument_PushFar(gains[4], 5);
        Message_ShowAndWaitCoreFar((s32)MsgAttackRises);
        BattleMessage_WaitForAdvance();
      }
      if (gains[5] != 0) {
        MessageArgument_PushFar(gains[5], 5);
        Message_ShowAndWaitCoreFar((s32)MsgDefenseRises);
        BattleMessage_WaitForAdvance();
      }
      if (gains[6] != 0) {
        MessageArgument_PushFar(gains[6], 5);
        Message_ShowAndWaitCoreFar((s32)MsgAgilityRises);
        BattleMessage_WaitForAdvance();
      }
      if (gains[7] != 0) {
        MessageArgument_PushFar(gains[7], 5);
        // "Luck rises by N!"
        Message_ShowAndWaitCoreFar((s32)MsgLuckRises);
        BattleMessage_WaitForAdvance();
      }
    }
  }
  Heap_Release(backup);
  if (spoils->coins != 0) {
    MessageArgument_PushFar(spoils->coins, 5);
    // "You got N coins."
    Message_ShowAndWaitCoreFar((s32)MsgGotCoins);
    Party_AdjustCoinsFar(spoils->coins);
    BattleMessage_WaitForAdvance();
  }
  found = &Data_0200049c;
  for (;;) {
    bestSlot = -1;
    best = -1;
    for (slot = 0; slot <= 3; slot++) {
      if (spoils->items[slot] != 0) {
        value = BattleSpoils_RankItem(spoils->items[slot]);
        if (value >= best) {
          best = value;
          bestSlot = slot;
        }
      }
    }
    if (bestSlot == -1)
      break;
    MessageArgument_PushFar(spoils->items[bestSlot], 2);
    // "You got <item>."
    Message_ShowAndWaitCoreFar((s32)MsgGotItem);
    BattleMessage_WaitForAdvance();
    if (PartyInventory_AddFar(spoils->items[bestSlot]) == -1) {
      *found = spoils->items[bestSlot];
      break;
    }
    spoils->items[bestSlot] = 0;
  }
}
#undef TARGET_MODE_NONE
#undef TARGET_MODE_FOES
#undef TARGET_MODE_ALLIES
#undef TARGET_MODE_SELF
#undef PLACE_PARTY
#undef PLACE_ENEMY
#undef COUNT_PARTIAL_CURES
#undef COUNT_ALL_CURES
#undef COUNT_POSITIVE_MODIFIERS
#undef CHECK_EFFECT_TARGET
#undef STRATEGY_MOST_HP
#undef STRATEGY_MOST_MAX_HP
#undef STRATEGY_RANDOM
#undef ALLY
#undef STEP_LIM
#undef ALLY_CHECK
#undef ELEMENT_BONUS
#undef MESSAGE_PUSH
#undef MESSAGE_SIDE
#undef GUARD_CUT
#undef RES_STEP
#undef STAT_SET
#undef AI_BATTLE
#undef OBJ_VRAM
#undef KEYS_HELD
#undef MSG_WAKES
#undef MSG_AGI_NORMAL
#undef MSG_CONSCIOUS
#undef MSG_CURE_DELUSION
#undef MSG_CURE_SEAL
#undef MSG_CURE_STUN
#undef MSG_REFLECT_END
#undef ANNOUNCE_END
#undef ANNOUNCE_END_POSE
#undef DMA3
#undef DEBUG_VALUE_SELECT
#undef CH_CNT
#undef LIST_MARKER_CHAR
#undef FIRST_DIGIT_BASE
#undef FIRST_DIGIT_COUNT
#undef ACTION_ID_MASK
