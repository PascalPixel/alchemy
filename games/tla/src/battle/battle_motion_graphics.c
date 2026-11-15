/// @file
/// A unit's motion graphics in battle: setting its records' frames, the flash
/// when it is hit, its removal from the formations and the graphics of a
/// defeated unit, with routines around them not yet understood.
// Part of battle.c.

#include "battle_internal.h"
#include "fixed_math.h"
#include "sounds.h"
#include "battle/battle.h"
#include "gamepak/far_call/battle_effect.h"
#include "gamepak/far_call/field.h"
#include "battle/presentation.h"
#include "system/system.h"
#include "text/msg_ids.h"
#include "macros.h"

void BattleObject_Reset(struct BattleObject *actor, s32 keep);

/// The beasts called up in battle (ABILITY_WILD_WOLF to ABILITY_CALL_DULLAHAN):
/// for a call that lands, saves the battle object it borrows and brings in the
/// beast's enemy (ENEMY_WILD_WOLF for Wild Wolf and so on) to play the attack;
/// returns the save, or NULL for any other ability. BattleMotion_DismissBeast
/// undoes it.
struct SavedBattleObj *
BattleMotion_CallBeast(struct BattlePlan *plan,
                       struct BattlePresentationWork *work) {
  s32 single;
  struct BattleObject *battleObject;
  struct SavedBattleObj *saved;
  u16 ids[UNIT_MAX];
  struct MotionObject *object;
  struct MotionObject *actor;
  s32 classId;
  s32 count;
  s32 i;

  if ((u32)(plan->staging - 15) > 2 || plan->failure != 0)
    return NULL;
  {
    u8 sides = (plan->unitId ^ plan->targetIds[0]) & 0x80;
    single = sides != 0;
  }
  saved = Heap_AllocEwram(sizeof(struct SavedBattleObj));
  saved->single = single;
  saved->reactivate = 0;
  switch (plan->abilityId) {
  case ABILITY_WILD_WOLF:
    classId = ENEMY_WILD_WOLF;
    break;
  case ABILITY_EMU:
    classId = ENEMY_EMU;
    break;
  case ABILITY_ROC:
    classId = ENEMY_ROC;
    break;
  case ABILITY_SALAMANDER:
    classId = ENEMY_SALAMANDER;
    break;
  case ABILITY_ORC:
    classId = ENEMY_ORC;
    break;
  case ABILITY_HARPY:
    classId = ENEMY_HARPY;
    break;
  case ABILITY_GRAND_GOLEM:
    classId = ENEMY_GRAND_GOLEM;
    break;
  case ABILITY_CERBERUS:
    classId = ENEMY_DREAD_HOUND;
    break;
  case ABILITY_WYVERN:
    classId = ENEMY_WYVERN;
    break;
  case ABILITY_PIXIE:
    classId = ENEMY_PIXIE;
    break;
  case ABILITY_DINOX:
    classId = ENEMY_DINOX;
    break;
  case ABILITY_GRYPHON:
    classId = ENEMY_GRYPHON;
    break;
  case ABILITY_LIVING_ARMOR:
    classId = ENEMY_LIVING_ARMOR;
    break;
  case ABILITY_CHIMERA:
    classId = ENEMY_CHIMERA;
    break;
  case ABILITY_BLUE_DRAGON:
    classId = ENEMY_BLUE_DRAGON;
    break;
  case ABILITY_FAERY:
    classId = ENEMY_FAERY;
    break;
  case ABILITY_ELDER_WOOD:
    classId = ENEMY_ELDER_WOOD;
    break;
  case ABILITY_LICH:
    classId = ENEMY_DOOMSAYER;
    break;
  case ABILITY_TROLL:
    classId = ENEMY_TROLL;
    break;
  case ABILITY_MINOTAUR:
    classId = ENEMY_MINOTAURUS;
    break;
  case ABILITY_GHOST_SOLDIER:
    classId = ENEMY_GHOST_ARMY;
    break;
  case ABILITY_MACETAIL:
    classId = ENEMY_MACETAIL;
    break;
  case ABILITY_FIRE_DRAGON:
    classId = ENEMY_FIRE_DRAGON;
    break;
  case ABILITY_WEIRD_NYMPH:
    classId = ENEMY_WEIRD_NYPMH;
    break;
  case ABILITY_SUCCUBUS:
    classId = ENEMY_SIREN;
    break;
  case ABILITY_ESTRE_WOOD:
    classId = ENEMY_ESTRE_WOOD;
    break;
  case ABILITY_MANTICORE:
    classId = ENEMY_MANTICORE;
    break;
  case ABILITY_PHOENIX:
    classId = ENEMY_PHOENIX;
    break;
  case ABILITY_CALL_ZOMBIE:
    classId = ENEMY_ZOMBIE;
    break;
  case ABILITY_CALL_DEMON:
    classId = ENEMY_RED_DEMON;
    break;
  case ABILITY_CALL_DULLAHAN:
    classId = ENEMY_DULLAHAN;
    break;
  default:
    classId = 9;
    break;
  }
  saved->classId = classId;
  if (!single) {
    saved->unitId = (plan->unitId ^ 0x80) & 0x80;
    if (BattleObject_Get(saved->unitId)->object == NULL) {
      saved->reactivate = 1;
#ifdef UBFIX
      BattleObject_Reset(BattleObject_Get(saved->unitId), FALSE);
#else
      // UB: keep is left out, so the routine reads whatever r1 holds.
      ((void (*)())BattleObject_Reset)(BattleObject_Get(saved->unitId));
#endif
      BattleObject_Get(saved->unitId)->scale = FIXED_ONE * 6 / 5;
    }
  } else {
    saved->unitId = plan->unitId;
  }
  work->actor = saved->unitId;
  actor = BattleObject_Get(plan->unitId)->object;
  Actor_SelectAnimationFar(actor, 3);
  Actor_SetAnimationSpeedFar((struct Actor *)actor, 16);
  IoWriteQueue_Store16((u32)&REG_BLDCNT, BLDCNT_TGT2_ALL | BLDCNT_EFFECT_BLEND);
  Sound_PlayFar(SE_CAST);
  wait(10);
  if (single) {
    BattleHighlight_SetSpriteMode(saved->unitId, 1);
  } else {
    count = BattleUnit_ListIds((saved->unitId != 0) + 1, ids);
    for (i = 0; i < count; i++)
      BattleHighlight_SetSpriteMode(ids[i], 1);
  }
  for (i = 0; i != BLEND_WEIGHT_MAX; i++) {
    REG_BLDALPHA = (i << 8) | (BLEND_WEIGHT_MAX - i);
    wait(1);
  }

  battleObject = BattleObject_Get(saved->unitId);
  object = battleObject->object;
  if (single) {
    ReleaseBattleObjectRecords(saved->unitId);
  } else {
    count = BattleUnit_ListIds((saved->unitId != 0) + 1, ids);
    for (i = 0; i < count; i++) {
      ReleaseBattleObjectRecords(ids[i]);
      BattleObject_ReleaseSpriteSlots(ids[i]);
    }
  }
  Iwram_CopyWordsEntry(saved, BattleObject_Get(saved->unitId),
                       sizeof(struct BattleObject));
  BattleObject_Reset(BattleObject_Get(saved->unitId), single);
  BattleObject_Get(saved->unitId)->scale = FIXED_ONE * 6 / 5;
  if (single)
    saved->buffer = Heap_AllocEwram(0x8000);
  else
    saved->buffer = gActorSpriteSlots;
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
  ids[1] = BATTLE_UNIT_LIST_END;
  BattleObject_SpawnObjectsForList((s16 *)ids, 1, 0);
  object->y = toFIXED(40);
  object->angle = plan->targetIds[0] <= ALLY_MAX ? 0 : DEGtoANG(180);
  BattleHighlight_SetSpriteMode(saved->unitId, 1);
  for (i = 0; i <= 29; i++) {
    object->y = (29 - i) * 3 << 16;
    object->velocityY = 0;
    object->angle += 0x2222;
    if (i < BLEND_WEIGHT_MAX)
      REG_BLDALPHA = ((BLEND_WEIGHT_MAX - i) << 8) | i;
    wait(1);
  }
  BattleHighlight_SetSpriteMode(saved->unitId, 0);
  return saved;
}

/// Frees the borrowed buffer and releases the motion graphics slot.
void BattleMotion_ReleaseBuffer(struct SavedBattleObj *saved) {
  struct MotionObject *object = BattleObject_Get(saved->unitId)->object;

  if (saved->buffer != NULL) {
    Animation_SkipToStopFar(GetMotionRecord(object, 0));
    wait(1);
    Heap_Release(saved->buffer);
    saved->buffer = NULL;
    {
      struct Sprite *record = GetMotionRecord(object, 0);

      record->flags = 0x20 | record->flags;
    }
    Sprite_RemoveAnimationAtFar(GetMotionRecord(object, 0), 0);
  }
}

/// Sends the called beast away and puts back the battle object
/// BattleMotion_CallBeast saved, then frees the save.
void BattleMotion_DismissBeast(struct SavedBattleObj *saved) {
  u16 ids[UNIT_MAX];
  struct MotionObject *object;
  s32 unitId;
  s32 count;
  s32 i;

  unitId = saved->unitId;
  object = BattleObject_Get(unitId)->object;
  IoWriteQueue_Store16((u32)&REG_BLDCNT, BLDCNT_TGT2_ALL | BLDCNT_EFFECT_BLEND);
  BattleHighlight_SetSpriteMode(unitId, 1);
  for (i = 0; i != 16; i++) {
    s32 fade;

    object->y = i << 18;
    object->angle += DEGtoANG(45);
    fade = i * 2 - 16;
    if (fade < 0)
      fade = 0;
    if (fade < BLEND_WEIGHT_MAX)
      REG_BLDALPHA = (fade << 8) | (BLEND_WEIGHT_MAX - fade);
    wait(1);
  }
  ReleaseBattleObjectRecords(unitId);
  Iwram_CopyWordsEntry(BattleObject_Get(saved->unitId), saved,
                       sizeof(struct BattleObject));
  if (saved->reactivate) {
    BattleObject_MarkForRelease(unitId);
    BattleMotion_DestroyAllSlotObjects();
  }
  SpritePictureCache_LoadFar(7, 0, 0, 0);
  if (saved->single) {
    ids[0] = unitId;
    ids[1] = BATTLE_UNIT_LIST_END;
    BattleObject_SpawnObjectsForList((s16 *)ids, 1, 0);
    BattleHighlight_SetSpriteMode(unitId, 1);
  } else {
    count = BattleUnit_ListIds((saved->unitId != 0) + 1, ids);
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
  SET_POSITION(object, object->targetX, object->targetY, object->targetZ);
  for (i = 0; i < BLEND_WEIGHT_MAX; i++) {
    object->y = 0;
    REG_BLDALPHA = ((BLEND_WEIGHT_MAX - i) << 8) | i;
    wait(1);
  }
  if (saved->single) {
    BattleHighlight_SetSpriteMode(unitId, 0);
  } else {
    s32 total = BattleUnit_ListIds((saved->unitId != 0) + 1, ids);

    for (i = 0; i < total; i++)
      BattleHighlight_SetSpriteMode(ids[i], 0);
  }
}

/// Plays an attack: the attacker runs up to its first target and the blow
/// lands. A miss makes the target hop back ("nimbly dodges the blow"); a
/// critical hit zooms in on the pair with only the two of them shown.
s32 BattleStaging_RunAttack(struct BattlePlan *plan, s32 mode) {
  struct BattlePresentationWork work;
  void *object;
  struct Sprite *animation;
  struct Unit *unit;
  s16 criticalPair[3];
  s32 critical;
  struct BattleBackgroundWork *view;
  void *saved;
  s32 angle;
  s32 adjusted;
  s32 facingAngle;
  s32 frames;
  s32 missed;
  s32 index;
  s32 zoomOnCritical;
  u32 actorId;
  struct BattleObject *battleObject;

  view = Ram_HeapSlots->battleBackground;
  actorId = plan->unitId;
  battleObject = BattleObject_Get(actorId);
  {
    s32 x = battleObject->anchorX;
    FIXED z;

    battleObject->object->x = x;
    z = battleObject->anchorZ;
    battleObject->object->z = z;
    angle = (u16)Trig_Atan(x, z);
    adjusted = angle - DEGtoANG(45);
    if (actorId > ALLY_MAX)
      adjusted = angle + TRIG_TURN * 3 / 8;
  }
  adjusted &= 0x7fff;
  facingAngle = (adjusted - DEGtoANG(45)) / 2 + DEGtoANG(45);
  if (view->targetYaw == facingAngle) {
    view->targetYaw = facingAngle;
    wait(1);
  } else {
    view->targetYaw = facingAngle;
    wait(1);
  }

  BattleHighlight_SetModes(0, HIGHLIGHT_OFF);
  BattleStaging_BuildTargetList(plan, &work);
  if (work.kind == 135)
    PartyStatus_DrawFar(Ram_HeapSlots->battleWork->partyStatus & ~1);
  saved = BattleMotion_CallBeast(plan, &work);
  unit = Unit_Get(work.actor);
  Unit_Get(work.actors[0]);
  critical = plan->targetCritical[0];
  missed = plan->targetMultiplier[0] == 0;

  object = BattleObject_Get(plan->unitId)->object;
  animation = GetMotionRecord(object, 0);
  frames = SpriteAnimation_SumCommandLengthsFar(
      animation->entries[0]->animationId, 2, 1);
  if (work.kind == 10)
    Actor_SelectAnimationFar(object, 2);
  else
    BattleMotion_ApproachTarget(work.actor, work.actors[0], frames,
                                BattleSprite_GetAttackHop(unit->enemyId) << 16);
  if ((mode & 32) && plan->unitId != plan->partnerId &&
      plan->partnerId != (work.actors[0] & 0x80)) {
    object = BattleObject_Get(plan->partnerId);

    animation = GetMotionRecord(((struct BattleObject *)object)->object, 0);
    frames = SpriteAnimation_SumCommandLengthsFar(
        animation->entries[0]->animationId, 2, 1);
    BattleMotion_ApproachTarget(
        plan->partnerId, work.actors[0], frames,
        BattleSprite_GetAttackHop(Unit_Get(plan->partnerId)->enemyId) << 16);
    Actor_SetAnimationSpeedFar(
        (struct Actor *)((struct BattleObject *)object)->object, 16);
  }
  Actor_SetAnimationSpeedFar(
      (struct Actor *)BattleObject_Get(work.actor)->object, 16);

  if ((u16)work.actors[0] <= ALLY_MAX)
    work.side = EFFECT_SIDE_PARTY;
  else
    work.side = EFFECT_SIDE_ENEMIES;

  REG_WIN0H = WIN_RANGE(0, DISPLAY_WIDTH);
  REG_WIN0V = WIN_RANGE(16, 136);
  REG_WIN1H = WIN_RANGE(0, DISPLAY_WIDTH);
  REG_WIN1V = WIN_RANGE(16, 136);
  REG_WININ = WININ_WIN0_BG0 | WININ_WIN0_BG1 | WININ_WIN0_BG2 |
              WININ_WIN0_OBJ | WININ_WIN0_CLR | WININ_WIN1_BG0 |
              WININ_WIN1_BG2 | WININ_WIN1_OBJ | WININ_WIN1_CLR;
  REG_WINOUT = WINOUT_WIN01_BG0 | WINOUT_WIN01_CLR | WINOUT_WINOBJ_ALL;
  REG_DISPCNT |= DISPCNT_WIN0_ON | DISPCNT_WIN1_ON;

  if (missed != 0) {
    wait(10);
    BattleMotion_HopBack(work.actors[0]);
    wait(2);
    wait(4);
    wait(10);
    BattlePlayback_Push(PLAYBACK_UNIT_NAME, plan->targetIds[0]);
    BattlePlayback_Push(PLAYBACK_MESSAGE, (u32)&MsgNimblyDodgesBlow);
    BattlePlayback_RunAll();
    BattleMotion_ReturnToSpot(work.actors[0]);
  } else {
    zoomOnCritical = 0;
    work.flags = 0;
    if (plan->battleEffect != 0)
      work.flags = 1;
    if (critical != 0)
      work.kind += 200;
    if (work.kind != 210 && critical != 0) {
      zoomOnCritical = 1;
      view->flag = 1;
      criticalPair[0] = work.actor;
      criticalPair[1] = work.firstTarget;
      criticalPair[2] = BATTLE_UNIT_LIST_END;
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
      if (plan->battleEffect & EFFECT_BY_ID_RANGE)
        BattleEffect_RunByIdRangeFar(&work);
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
      BattleCamera_SetView(0, 0, 0, BATTLE_ZOOM_NORMAL);
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
  struct Unit *unit;
  struct ItemDefinition *itemDefinition;
  struct BattleBackgroundWork *transition =
      (struct BattleBackgroundWork *)Ram_HeapSlots->battleBackground;
  s32 targetYaw;

  if (plan->unitId > 4)
    targetYaw = YAW_ENEMY_ACTION;
  else
    targetYaw = YAW_PARTY_ACTION;
  if (transition->targetYaw != targetYaw)
    transition->targetYaw = targetYaw;
  BattleStaging_BuildTargetList(plan, &work);
  BattleHighlight_SetModes(0, HIGHLIGHT_OFF);
  {
    struct BattleObject *slot = BattleObject_Get(work.actor);
    struct BattleWork *battle;
    u8 *mode;

    battle = Ram_HeapSlots->battleWork;
    mode = &battle->actingAnimation;
    object = slot->object;
    Actor_SelectAnimationFar(object, *mode);
  }
  Actor_SetAnimationSpeedFar((struct Actor *)object, 16);
  if (plan->targetIds[0] <= ALLY_MAX)
    work.side = EFFECT_SIDE_PARTY;
  else
    work.side = EFFECT_SIDE_ENEMIES;

  for (targetOrFadeIndex = 0; targetOrFadeIndex != work.count;
       targetOrFadeIndex++) {
    struct Sprite *animation = GetMotionRecord(
        BattleObject_Get(work.actors[targetOrFadeIndex])->object, 0);
    s32 entryIndex;

    for (entryIndex = 0; entryIndex != animation->count - 1; entryIndex++)
      work.params[targetOrFadeIndex][entryIndex] =
          animation->entries[entryIndex]->colorMode;
  }

  Task_Add(BattlePlayback_Run, TASK_PASS_VBLANK);
  if (work.kind != 0) {
    FIXED fade;
    struct HeapSlots *heap;

    targetOrFadeIndex = 0;
    heap = Ram_HeapSlots;
    fade = 0;
    do {
      struct BattleWork *battle = heap->battleWork;

      if (targetOrFadeIndex < 20) {
        u16 *palette = battle->palette;
        FIXED value = FIXED_ONE - fade;

        battle->brightness = value;
        ColorBuffer_Scale(palette, (u32)BATTLE_BG_PALETTE, value,
                          BATTLE_BG_PALETTE_COLORS);
      }
      wait(1);
      targetOrFadeIndex++;
      fade += FIXED_ONE / 60;
    } while (targetOrFadeIndex < 20);
    if (plan->battleEffect & EFFECT_BY_ID_RANGE)
      BattleEffect_RunByIdRangeFar(&work);
    else
      BattleEffect_RunFromTableFar(&work);
  } else {
    wait(60);
  }
  BattlePlayback_WaitForReady();
  Actor_SelectAnimationFar(object, POSE_STANDING);
  for (targetOrFadeIndex = 0; targetOrFadeIndex != work.count;
       targetOrFadeIndex++)
    BattleMotion_ReturnToSpot(work.actors[targetOrFadeIndex]);

  unit = Unit_Get(selection->unitId);
  inventoryEntry = unit->inventory[selection->parameter];
  itemDefinition = Item_GetDirectFar(inventoryEntry);
  if (itemDefinition->useType == ITEM_USE_CONSUMED) {
    s32 result = Inventory_RemoveFar(selection->unitId, selection->parameter);
    s32 index = selection->parameter;
    if (result == INVENTORY_REMOVE_LAST) {
      struct BattleWork *battle = Ram_HeapSlots->battleWork;
      u32 row;

      for (row = 0; row < BATTLE_ACTION_CAPACITY; row++) {
        if (battle->actions[row].command == COMMAND_ITEM &&
            battle->actions[row].unitId == selection->unitId) {
          if (battle->actions[row].parameter == index)
            battle->actions[row].parameter = -1;
          else if (battle->actions[row].parameter > index)
            battle->actions[row].parameter--;
        }
      }
    }
  } else if (itemDefinition->useType == ITEM_USE_MAY_BREAK) {
    if ((Battle_RandomFar() & 7) == 0) {
      BattlePlayback_Push(PLAYBACK_ITEM_NAME,
                          unit->inventory[selection->parameter]);
      BattlePlayback_Push(PLAYBACK_MESSAGE, (u32)&MsgItemBroke);
      Inventory_BreakFar(selection->unitId, selection->parameter);
      BattlePlayback_RunAll();
    }
  } else if (itemDefinition->useType == ITEM_USE_EMPTIES) {
    if ((inventoryEntry & INVENTORY_ITEM_ID_MASK) == ITEM_HERMES_WATER)
      inventoryEntry = ITEM_EMPTY_BOTTLE;
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
  struct Sprite *record;
  struct SpriteAnimation *child;
  struct SpriteAnimation **children;
  u8 mask = 0xff;
  u8 frame;

  while ((record = GetMotionRecord(object, index)) != NULL) {
    children = &record->entries[1];
    child = record->entries[0];
    frame = child->picture;
    count = record->count;
    child->colorMode = value;
    child->picture = frame | mask;
    for (child_index = 1; child_index < count; child_index++) {
      child = *children++;
      child->colorMode = 0;
      child->picture |= 0xff;
    }
    index++;
  }
}

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
  struct BattleBackgroundWork *transition = Ram_HeapSlots->battleBackground;
  struct MotionObject *object;
  struct HeapSlots *heap;
  void *saved;
  s32 isDjinni;
  s32 targetIndex;

  if (plan->battleEffect & EFFECT_VIEW_ACROSS) {
    if (plan->unitId <= ALLY_MAX)
      transition->targetYaw = YAW_ENEMY_ACTION;
    else
      transition->targetYaw = YAW_BEHIND_ENEMIES;
    transition->frames = 60;
  } else {
    struct MotionObject *actor = BattleObject_Get(plan->unitId)->object;
    u32 target = plan->targetIds[0];
    u32 actorId = plan->unitId;
    s32 angle = (u16)Trig_Atan(actor->x, actor->z);
    s32 targetYaw;

    if (actorId <= ALLY_MAX)
      targetYaw = angle - TRIG_TURN * 3 / 32;
    else
      targetYaw = angle + TRIG_TURN * 3 / 32;
    targetYaw = (s16)targetYaw;
    targetYaw += ((actorId <= ALLY_MAX ? YAW_PARTY_ACTION : YAW_ENEMY_ACTION) -
                  targetYaw) *
                 3 / 4;
    if (!((actorId <= ALLY_MAX) ^ (target <= ALLY_MAX)))
      targetYaw =
          actorId <= ALLY_MAX ? TRIG_TURN * 9 / 64 : -(TRIG_TURN * 9 / 64);
    if (transition->targetYaw != targetYaw)
      transition->targetYaw = targetYaw;
  }
  if (plan->battleEffect & EFFECT_VIEW_REVERSED) {
    if (plan->unitId <= ALLY_MAX)
      transition->targetYaw = YAW_ENEMY_ACTION;
    else
      transition->targetYaw = YAW_PARTY_ACTION;
    transition->frames = 60;
  }

  BattleStaging_BuildTargetList(plan, &work);
  isDjinni = mode & CAST_DJINNI;
  if (isDjinni)
    work.flags = 1;
  BattleHighlight_SetModes(0, HIGHLIGHT_OFF);
  heap = Ram_HeapSlots;
  PartyStatus_DrawFar(heap->battleWork->partyStatus & ~1);
  saved = BattleMotion_CallBeast(plan, &work);
  object = BattleObject_Get(work.actor)->object;
  Actor_SelectAnimationFar(object, heap->battleWork->actingAnimation);
  Actor_SetAnimationSpeedFar((struct Actor *)object, 16);
  Sound_PlayFar(SE_CAST);
  if (mode & CAST_ENEMY)
    ElementGlow_Play(work.actor, plan->element, GLOW_STREAKS, 0);
  else if (!isDjinni)
    ElementGlow_Play(work.actor, plan->element, GLOW_STAR_FIELD, 0);
  if (plan->targetIds[0] <= ALLY_MAX)
    work.side = EFFECT_SIDE_PARTY;
  else
    work.side = EFFECT_SIDE_ENEMIES;

  for (targetIndex = 0; targetIndex != work.count; targetIndex++) {
    struct Sprite *animation =
        GetMotionRecord(BattleObject_Get(work.actors[targetIndex])->object, 0);
    s32 entryIndex;

    for (entryIndex = 0; entryIndex != animation->count - 1; entryIndex++)
      work.params[targetIndex][entryIndex] =
          animation->entries[entryIndex]->colorMode;
  }
  if (plan->failure != ACTION_FAILURE_NONE) {
    if (plan->failure == ACTION_FAILURE_SEALED) {
      BattlePlayback_Push(PLAYBACK_UNIT_NAME, plan->unitId);
      BattlePlayback_Push(PLAYBACK_MESSAGE, (u32)&MsgPsynergyBlocked);
    } else {
      BattlePlayback_Push(PLAYBACK_MESSAGE, (u32)&MsgCastLacksPp);
    }
    BattlePlayback_RunAll();
    BattleBackground_RestorePalette();
  } else {
    // The word after the plan in BattleWork is skipPlayback.
    if (((s32 *)plan)[25] == 0)
      Task_Add(BattlePlayback_Run, TASK_PASS_VBLANK);
    if (work.kind) {
      if (saved)
        BattleMotion_ReleaseBuffer(saved);
      if (plan->battleEffect & EFFECT_BY_ID_RANGE)
        BattleEffect_RunByIdRangeFar(&work);
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
    Actor_SelectAnimationFar(object, POSE_STANDING);
    for (targetIndex = 0; targetIndex != work.count; targetIndex++)
      BattleMotion_ReturnToSpot(work.actors[targetIndex]);
  }
  return 0;
}

/// Flashes a unit as it is hit, white and back twice in its damage pose.
void BattleMotion_FlashHit(s32 id, s32 unusedA, s32 unusedB) {
  u16 selection[2];
  u16 *selected;
  s32 target;
  s32 remaining;

  Unit_Get(id);
  Actor_SelectAnimationFar(BattleObject_Get(id)->object, POSE_DAMAGE);

  selected = selection;
  remaining = 1;
  do {
    target = 0xff;
    selected[1] = target;
    selected[0] = id;
    PartyStatus_HighlightMembersFar(selected);
    BattleObject_SetColorMode(BattleObject_Get(id)->object, SPRITE_COLOR_WHITE);
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
  Unit_Get(id)->side = BATTLE_SIDE_NONE;
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
  for (j = 0; j < BATTLE_ACTION_CAPACITY; j++) {
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
  struct Unit *unit;
  struct Sprite *rec;
  struct SpriteAnimation *ent;
  s32 n;

  unit = Unit_Get(id);
  n = 0;
  while ((rec = GetMotionRecord(BattleObject_Get(id)->object, n)) != 0) {
    if (unit->side != BATTLE_SIDE_ENEMY)
      Sprite_StartAnimationFar(rec, POSE_PARTY_HELD);
    else
      Sprite_StartAnimationFar(rec, POSE_ENEMY_DOWN);
    n++;
  }

  if (unit->side == BATTLE_SIDE_ENEMY) {
    n = 0;
    while ((rec = GetMotionRecord(BattleObject_Get(id)->object, n)) != 0) {
      ent = rec->entries[0];
      recs[n] = rec;
      ent->colorMode = SPRITE_COLOR_GREY;
      ent->picture = 0xff;
      n++;
    }
    wait(4);
    BattleObject_RemoveFromLists(id);
    SpriteDissolve_RunManyFar(recs, n); // Erase pixel by pixel.
    BattleObject_MarkForRelease(id);
  }
}
