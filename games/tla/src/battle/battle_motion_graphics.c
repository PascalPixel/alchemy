/// @file
/// A unit's part in the battle stagings: a beast called up, an attack, an
/// item's use and a cast, the flash when a unit is hit, and a downed unit's
/// removal from the battle and its graphics.
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

void BattleObject_Reset(struct BattleObject *battleObject, s32 keep);

/// The sprite slot a called beast borrows, after the six enemy slots.
#define BEAST_SPRITE_SLOT 7

/// The frames a called beast takes to drop in, and to rise away.
#define BEAST_ARRIVAL_FRAMES 30
#define BEAST_DEPARTURE_FRAMES 16

/// A critical hit's zoom on the attacker and its target, in percent: from
/// CRITICAL_ZOOM_START, CRITICAL_ZOOM_GAIN more by the end of the run up.
#define CRITICAL_ZOOM_START 100
#define CRITICAL_ZOOM_GAIN 30

/// The beasts called up in battle (ABILITY_WILD_WOLF to ABILITY_CALL_DULLAHAN):
/// for a call that lands, saves the battle object it borrows and brings in the
/// beast's enemy (ENEMY_WILD_WOLF for Wild Wolf and so on) to play the attack;
/// returns the save, or NULL for any other ability. BattleMotion_DismissBeast
/// undoes it.
struct SavedBattleObject *
BattleMotion_CallBeast(struct BattlePlan *plan,
                       struct BattlePresentationState *state) {
  s32 single;
  struct BattleObject *battleObject;
  struct SavedBattleObject *saved;
  u16 ids[UNIT_MAX];
  struct MotionObject *object;
  struct MotionObject *actor;
  s32 classId;
  s32 count;
  s32 i;

  if ((u32)(plan->staging - STAGING_BEAST_ATTACK) >
          STAGING_BEAST_PSYNERGY - STAGING_BEAST_ATTACK ||
      plan->failure != 0)
    return NULL;
  {
    u8 sides = (plan->unitId ^ plan->targetIds[0]) & UNIT_ID_ENEMY_BIT;
    single = sides != 0;
  }
  saved = Heap_AllocEwram(sizeof(struct SavedBattleObject));
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
    // No beast of its own: the first enemy row after ENEMY_NONE.
    classId = ENEMY_VENUS_DJINNI_UNUSED_QUARTZ;
    break;
  }
  saved->classId = classId;
  if (!single) {
    saved->unitId = (plan->unitId ^ UNIT_ID_ENEMY_BIT) & UNIT_ID_ENEMY_BIT;
    if (BattleObject_Get(saved->unitId)->object == NULL) {
      saved->reactivate = 1;
#ifdef UBFIX
      BattleObject_Reset(BattleObject_Get(saved->unitId), FALSE);
#else
      // UB: called without keep, so the routine reads an argument it was never
      // given.
      ((void (*)())BattleObject_Reset)(BattleObject_Get(saved->unitId));
#endif
      BattleObject_Get(saved->unitId)->scale = FIXED_ONE * 6 / 5;
    }
  } else {
    saved->unitId = plan->unitId;
  }
  state->actor = saved->unitId;
  actor = BattleObject_Get(plan->unitId)->object;
  Actor_SelectAnimationFar(actor, POSE_CAST);
  Actor_SetAnimationSpeedFar((struct Actor *)actor, ANIM_STEP_NORMAL);
  IoWriteQueue_Store16((u32)&REG_BLDCNT, BLDCNT_TGT2_ALL | BLDCNT_EFFECT_BLEND);
  Sound_PlayFar(SE_CAST);
  wait(10);
  if (single) {
    BattleHighlight_SetSpriteMode(saved->unitId, SPRITE_SEMI_TRANSPARENT);
  } else {
    count = BattleUnit_ListIds((saved->unitId != 0) + 1, ids);
    for (i = 0; i < count; i++)
      BattleHighlight_SetSpriteMode(ids[i], SPRITE_SEMI_TRANSPARENT);
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
    saved->buffer = Heap_AllocEwram(32768);
  else
    saved->buffer = gActorSpriteSlots;
  i = SpritePictureCache_LoadFar(BEAST_SPRITE_SLOT, saved->buffer,
                                 BattleSprite_GetSprite(saved->classId),
                                 BattleSprite_GetPalette(saved->classId));
  if (i != 0) {
    battleObject->resource = BattleSprite_GetSprite(saved->classId) +
                             (BEAST_SPRITE_SLOT << RESOURCE_SLOT_SHIFT);
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
  BattleHighlight_SetSpriteMode(saved->unitId, SPRITE_SEMI_TRANSPARENT);
  for (i = 0; i < BEAST_ARRIVAL_FRAMES; i++) {
    object->y = toFIXED((BEAST_ARRIVAL_FRAMES - 1 - i) * 3);
    object->velocityY = 0;
    // Four turns as it drops.
    object->angle += TRIG_TURN * 4 / BEAST_ARRIVAL_FRAMES;
    if (i < BLEND_WEIGHT_MAX)
      REG_BLDALPHA = ((BLEND_WEIGHT_MAX - i) << 8) | i;
    wait(1);
  }
  BattleHighlight_SetSpriteMode(saved->unitId, SPRITE_OPAQUE);
  return saved;
}

/// Frees the borrowed buffer and releases the motion graphics slot.
void BattleMotion_ReleaseBuffer(struct SavedBattleObject *saved) {
  struct MotionObject *object = BattleObject_Get(saved->unitId)->object;

  if (saved->buffer != NULL) {
    Sprite_SkipToStopFar(GetMotionRecord(object, 0));
    wait(1);
    Heap_Release(saved->buffer);
    saved->buffer = NULL;
    {
      struct Sprite *record = GetMotionRecord(object, 0);

      record->flags = SPRITE_FLAG_KEEP_PICTURE | record->flags;
    }
    Sprite_RemoveAnimationAtFar(GetMotionRecord(object, 0), 0);
  }
}

/// Sends the called beast away and puts back the battle object
/// BattleMotion_CallBeast saved, then frees the save.
void BattleMotion_DismissBeast(struct SavedBattleObject *saved) {
  u16 ids[UNIT_MAX];
  struct MotionObject *object;
  s32 unitId;
  s32 count;
  s32 i;

  unitId = saved->unitId;
  object = BattleObject_Get(unitId)->object;
  IoWriteQueue_Store16((u32)&REG_BLDCNT, BLDCNT_TGT2_ALL | BLDCNT_EFFECT_BLEND);
  BattleHighlight_SetSpriteMode(unitId, SPRITE_SEMI_TRANSPARENT);
  for (i = 0; i != BEAST_DEPARTURE_FRAMES; i++) {
    s32 fade;

    // Two turns as it rises, fading out over the second half.
    object->y = toFIXED(i * 4);
    object->angle += DEGtoANG(45);
    fade = i * 2 - BEAST_DEPARTURE_FRAMES;
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
  SpritePictureCache_LoadFar(BEAST_SPRITE_SLOT, 0, 0, 0);
  if (saved->single) {
    ids[0] = unitId;
    ids[1] = BATTLE_UNIT_LIST_END;
    BattleObject_SpawnObjectsForList((s16 *)ids, 1, 0);
    BattleHighlight_SetSpriteMode(unitId, SPRITE_SEMI_TRANSPARENT);
  } else {
    count = BattleUnit_ListIds((saved->unitId != 0) + 1, ids);
    for (i = 0; i < count; i++) {
      s32 resource = EnemySprite_LoadIntoSlots(ids[i]);

      if (resource)
        BattleObject_Get(ids[i])->resource = resource;
    }
    BattleObject_SpawnObjectsForList((s16 *)ids, 1, 0);
    for (i = 0; i < count; i++)
      BattleHighlight_SetSpriteMode(ids[i], SPRITE_SEMI_TRANSPARENT);
  }
  BattleMotion_ReturnToSpot(unitId);
  SET_POSITION(object, object->targetX, object->targetY, object->targetZ);
  for (i = 0; i < BLEND_WEIGHT_MAX; i++) {
    object->y = 0;
    REG_BLDALPHA = ((BLEND_WEIGHT_MAX - i) << 8) | i;
    wait(1);
  }
  if (saved->single) {
    BattleHighlight_SetSpriteMode(unitId, SPRITE_OPAQUE);
  } else {
    s32 total = BattleUnit_ListIds((saved->unitId != 0) + 1, ids);

    for (i = 0; i < total; i++)
      BattleHighlight_SetSpriteMode(ids[i], SPRITE_OPAQUE);
  }
}

/// Plays an attack: the attacker runs up to its first target and the blow
/// lands. A miss makes the target hop back ("nimbly dodges the blow"); a
/// critical hit zooms in on the pair with only the two of them shown.
s32 BattleStaging_RunAttack(struct BattlePlan *plan, s32 mode) {
  struct BattlePresentationState state;
  void *object;
  struct Sprite *animation;
  struct Unit *unit;
  s16 criticalPair[3];
  s32 critical;
  struct BattleBackgroundState *background;
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

  background = Ram_HeapSlots->battleBackground;
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
  adjusted &= DEGtoANG(180) - 1;
  facingAngle = (adjusted - YAW_PARTY_ACTION) / 2 + YAW_PARTY_ACTION;
  if (background->targetYaw == facingAngle) {
    background->targetYaw = facingAngle;
    wait(1);
  } else {
    background->targetYaw = facingAngle;
    wait(1);
  }

  BattleHighlight_SetModes(0, HIGHLIGHT_OFF);
  BattleStaging_BuildTargetList(plan, &state);
  if (state.kind == EFFECT_ID_SEVERE_BLOW)
    PartyStatus_DrawFar(Ram_HeapSlots->battleState->partyStatus &
                        ~PARTY_STATUS_SHOW_PP);
  saved = BattleMotion_CallBeast(plan, &state);
  unit = Unit_Get(state.actor);
  // DEAD: Unit_Get's result is not used; the ROM keeps the call.
  Unit_Get(state.actors[0]);
  critical = plan->targetCritical[0];
  missed = plan->targetMultiplier[0] == 0;

  object = BattleObject_Get(plan->unitId)->object;
  animation = GetMotionRecord(object, 0);
  frames = SpriteAnimation_SumCommandLengthsFar(
      animation->entries[0]->animationId, 2, 1);
  // A class whose attack is the burst leaps where it stands.
  if (state.kind == ATTACK_EFFECT_BURST)
    Actor_SelectAnimationFar(object, POSE_LEAP);
  else
    BattleMotion_ApproachTarget(
        state.actor, state.actors[0], frames,
        INTtoFIXED(BattleSprite_GetAttackHop(unit->enemyId)));
  if ((mode & ATTACK_WITH_PARTNER) && plan->unitId != plan->partnerId &&
      plan->partnerId != (state.actors[0] & UNIT_ID_ENEMY_BIT)) {
    object = BattleObject_Get(plan->partnerId);

    animation = GetMotionRecord(((struct BattleObject *)object)->object, 0);
    frames = SpriteAnimation_SumCommandLengthsFar(
        animation->entries[0]->animationId, 2, 1);
    BattleMotion_ApproachTarget(plan->partnerId, state.actors[0], frames,
                                INTtoFIXED(BattleSprite_GetAttackHop(
                                    Unit_Get(plan->partnerId)->enemyId)));
    Actor_SetAnimationSpeedFar(
        (struct Actor *)((struct BattleObject *)object)->object,
        ANIM_STEP_NORMAL);
  }
  Actor_SetAnimationSpeedFar(
      (struct Actor *)BattleObject_Get(state.actor)->object, ANIM_STEP_NORMAL);

  if ((u16)state.actors[0] <= ALLY_MAX)
    state.side = EFFECT_SIDE_PARTY;
  else
    state.side = EFFECT_SIDE_ENEMIES;

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
    BattleMotion_HopBack(state.actors[0]);
    wait(2);
    wait(4);
    wait(10);
    BattlePlayback_Push(PLAYBACK_UNIT_NAME, plan->targetIds[0]);
    BattlePlayback_Push(PLAYBACK_MESSAGE, (u32)&MsgNimblyDodgesBlow);
    BattlePlayback_RunAll();
    BattleMotion_ReturnToSpot(state.actors[0]);
  } else {
    zoomOnCritical = 0;
    state.flags = 0;
    if (plan->battleEffect != 0)
      state.flags = 1;
    if (critical != 0)
      state.kind += EFFECT_ID_CRITICAL_OFFSET;
    if (state.kind != EFFECT_ID_CRITICAL_OFFSET + ATTACK_EFFECT_BURST &&
        critical != 0) {
      zoomOnCritical = 1;
      background->flag = 1;
      criticalPair[0] = state.actor;
      criticalPair[1] = state.firstTarget;
      criticalPair[2] = BATTLE_UNIT_LIST_END;
      BattleActor_SpawnObjectsForList(criticalPair, 0);
    }
    // The battle effect starts 8 frames before the run up ends, at least one
    // frame after it began; a critical hit zooms in on the pair meanwhile.
    frames -= 8;
    if (frames <= 0)
      frames = 1;
    {
      s32 first;
      s32 second;

      for (index = 0; index != frames; index++) {
        if (zoomOnCritical != 0) {
          first = state.actor;
          second = state.firstTarget;
          BattleCamera_SetViewAtPairMidpoint(
              first, second,
              index * CRITICAL_ZOOM_GAIN / frames + CRITICAL_ZOOM_START);
        }
        wait(1);
      }
    }
    Task_Add(BattlePlayback_Run, TASK_PASS_FRAME);
    if (state.kind != 0) {
      if (saved)
        BattleMotion_ReleaseBuffer(saved);
      if (plan->battleEffect & EFFECT_BY_ID_RANGE)
        BattleEffect_RunByIdRangeFar(&state);
      else
        BattleEffect_RunFromTableFar(&state);
    }
    if (saved) {
      BattleMotion_DismissBeast(saved);
      Heap_Release(saved);
    }
    BattlePlayback_WaitForReady();
    if (critical != 0) {
      background->flag = 0;
      BattleUnit_ListAllAndSubmit();
      BattleCamera_SetView(0, 0, 0, BATTLE_ZOOM_NORMAL);
    }
    BattleMotion_ReturnToSpot(state.actors[0]);
  }
  if (!saved)
    BattleMotion_ReturnToSpot(state.actor);
  return 0;
}

/// Marks the item in `slot` broken; returns -1 for an empty slot.
s32 Inventory_BreakFar(s32 owner, s32 slot);

/// Plays an item's use: the camera turns to the user's side, the user raises
/// the item and, for an item with a battle effect, the battle background dims
/// over 20 frames while the effect plays with its messages. Then the item is
/// spent as its use type says: used up, broken one time in eight, or the
/// Hermes' Water left an Empty Bottle. When a used-up item empties its slot,
/// the user's later item commands this turn (`selection` is this one) follow
/// their items to their new slots, or lose theirs (-1).
s32 BattleStaging_RunItem(struct BattlePlan *plan, s32 unused,
                          struct BattleActionRecord *selection) {
  struct BattlePresentationState state;
  struct MotionObject *object;
  s32 i;
  s32 inventoryEntry;
  struct Unit *unit;
  struct ItemDefinition *item;
  struct BattleBackgroundState *background = Ram_HeapSlots->battleBackground;
  s32 targetYaw;

  if (plan->unitId > FELIX)
    targetYaw = YAW_ENEMY_ACTION;
  else
    targetYaw = YAW_PARTY_ACTION;
  if (background->targetYaw != targetYaw)
    background->targetYaw = targetYaw;
  BattleStaging_BuildTargetList(plan, &state);
  BattleHighlight_SetModes(0, HIGHLIGHT_OFF);
  {
    struct BattleObject *slot = BattleObject_Get(state.actor);
    struct BattleState *battle;
    u8 *mode;

    battle = Ram_HeapSlots->battleState;
    mode = &battle->actingAnimation;
    object = slot->object;
    Actor_SelectAnimationFar(object, *mode);
  }
  Actor_SetAnimationSpeedFar((struct Actor *)object, ANIM_STEP_NORMAL);
  if (plan->targetIds[0] <= ALLY_MAX)
    state.side = EFFECT_SIDE_PARTY;
  else
    state.side = EFFECT_SIDE_ENEMIES;

  for (i = 0; i != state.count; i++) {
    struct Sprite *animation =
        GetMotionRecord(BattleObject_Get(state.actors[i])->object, 0);
    s32 entryIndex;

    for (entryIndex = 0; entryIndex != animation->count - 1; entryIndex++)
      state.params[i][entryIndex] = animation->entries[entryIndex]->colorMode;
  }

  Task_Add(BattlePlayback_Run, TASK_PASS_FRAME);
  if (state.kind != 0) {
    FIXED fade;
    struct HeapSlots *heap;

    i = 0;
    heap = Ram_HeapSlots;
    fade = 0;
    do {
      struct BattleState *battle = heap->battleState;

      if (i < 20) {
        u16 *palette = battle->palette;
        FIXED value = FIXED_ONE - fade;

        battle->brightness = value;
        ColorBuffer_Scale(palette, (u32)BATTLE_BG_PALETTE, value,
                          BATTLE_BACKGROUND_COLORS);
      }
      wait(1);
      i++;
      fade += BACKGROUND_LEVEL_STEP;
    } while (i < 20);
    if (plan->battleEffect & EFFECT_BY_ID_RANGE)
      BattleEffect_RunByIdRangeFar(&state);
    else
      BattleEffect_RunFromTableFar(&state);
  } else {
    wait(60);
  }
  BattlePlayback_WaitForReady();
  Actor_SelectAnimationFar(object, POSE_STANDING);
  for (i = 0; i != state.count; i++)
    BattleMotion_ReturnToSpot(state.actors[i]);

  unit = Unit_Get(selection->unitId);
  inventoryEntry = unit->inventory[selection->parameter];
  item = Item_GetDirectFar(inventoryEntry);
  if (item->useType == ITEM_USE_CONSUMED) {
    s32 result = Inventory_RemoveFar(selection->unitId, selection->parameter);
    s32 index = selection->parameter;
    if (result == INVENTORY_REMOVE_LAST) {
      struct BattleState *battle = Ram_HeapSlots->battleState;
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
  } else if (item->useType == ITEM_USE_MAY_BREAK) {
    if ((Battle_RandomFar() & 7) == 0) {
      BattlePlayback_Push(PLAYBACK_ITEM_NAME,
                          unit->inventory[selection->parameter]);
      BattlePlayback_Push(PLAYBACK_MESSAGE, (u32)&MsgItemBroke);
      Inventory_BreakFar(selection->unitId, selection->parameter);
      BattlePlayback_RunAll();
    }
  } else if (item->useType == ITEM_USE_EMPTIES) {
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
  s32 childIndex;
  s32 count;
  struct Sprite *record;
  struct SpriteAnimation *child;
  struct SpriteAnimation **children;
  u8 mask = SPRITE_PICTURE_NONE;
  u8 frame;

  for (; (record = GetMotionRecord(object, index)) != NULL; index++) {
    children = &record->entries[1];
    child = record->entries[0];
    frame = child->picture;
    count = record->count;
    child->colorMode = value;
    // DEAD: each picture is read and ORed with SPRITE_PICTURE_NONE, which
    // stores SPRITE_PICTURE_NONE whatever it was; the ROM keeps the loads and
    // the ORs.
    child->picture = frame | mask;
    for (childIndex = 1; childIndex < count; childIndex++) {
      child = *children++;
      child->colorMode = 0;
      child->picture |= SPRITE_PICTURE_NONE;
    }
  }
}

/// Plays Psynergy, a Djinni or an enemy's cast. The camera turns toward
/// the targets from the caster's side, the caster raises its hand and glows
/// in its element (no glow for a Djinni); then the cast fails ("blocked" or
/// too little PP) or its battle effect plays with its playback, and the targets
/// return to their spots. The party status window is drawn without its PP row
/// meanwhile. `mode` is an enum BattleCastKind: STAGING_CAST plays
/// CAST_PSYNERGY, STAGING_DJINNI CAST_DJINNI and STAGING_ENEMY_CAST CAST_ENEMY.
s32 BattleStaging_RunPsynergyCast(struct BattlePlan *plan, s32 mode) {
  struct BattlePresentationState state;
  struct BattleBackgroundState *background = Ram_HeapSlots->battleBackground;
  struct MotionObject *object;
  struct HeapSlots *heap;
  void *saved;
  s32 isDjinni;
  s32 targetIndex;

  if (plan->battleEffect & EFFECT_VIEW_ACROSS) {
    if (plan->unitId <= ALLY_MAX)
      background->targetYaw = YAW_ENEMY_ACTION;
    else
      background->targetYaw = YAW_BEHIND_ENEMIES;
    background->frames = 60;
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
    if (background->targetYaw != targetYaw)
      background->targetYaw = targetYaw;
  }
  if (plan->battleEffect & EFFECT_VIEW_REVERSED) {
    if (plan->unitId <= ALLY_MAX)
      background->targetYaw = YAW_ENEMY_ACTION;
    else
      background->targetYaw = YAW_PARTY_ACTION;
    background->frames = 60;
  }

  BattleStaging_BuildTargetList(plan, &state);
  isDjinni = mode & CAST_DJINNI;
  if (isDjinni)
    state.flags = 1;
  BattleHighlight_SetModes(0, HIGHLIGHT_OFF);
  heap = Ram_HeapSlots;
  PartyStatus_DrawFar(heap->battleState->partyStatus & ~PARTY_STATUS_SHOW_PP);
  saved = BattleMotion_CallBeast(plan, &state);
  object = BattleObject_Get(state.actor)->object;
  Actor_SelectAnimationFar(object, heap->battleState->actingAnimation);
  Actor_SetAnimationSpeedFar((struct Actor *)object, ANIM_STEP_NORMAL);
  Sound_PlayFar(SE_CAST);
  if (mode & CAST_ENEMY)
    ElementGlow_Play(state.actor, plan->element, GLOW_STREAKS, 0);
  else if (!isDjinni)
    ElementGlow_Play(state.actor, plan->element, GLOW_STAR_FIELD, 0);
  if (plan->targetIds[0] <= ALLY_MAX)
    state.side = EFFECT_SIDE_PARTY;
  else
    state.side = EFFECT_SIDE_ENEMIES;

  for (targetIndex = 0; targetIndex != state.count; targetIndex++) {
    struct Sprite *animation =
        GetMotionRecord(BattleObject_Get(state.actors[targetIndex])->object, 0);
    s32 entryIndex;

    for (entryIndex = 0; entryIndex != animation->count - 1; entryIndex++)
      state.params[targetIndex][entryIndex] =
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
    // The word after the plan in BattleState is skipPlayback.
    if (*(s32 *)(plan + 1) == 0)
      Task_Add(BattlePlayback_Run, TASK_PASS_FRAME);
    if (state.kind) {
      if (saved)
        BattleMotion_ReleaseBuffer(saved);
      if (plan->battleEffect & EFFECT_BY_ID_RANGE)
        BattleEffect_RunByIdRangeFar(&state);
      else
        BattleEffect_RunFromTableFar(&state);
    } else {
      BattleBackground_RestorePalette();
    }
    if (saved) {
      BattleMotion_DismissBeast(saved);
      Heap_Release(saved);
    }
    if (*(s32 *)(plan + 1) == 0)
      BattlePlayback_WaitForReady();
    Actor_SelectAnimationFar(object, POSE_STANDING);
    for (targetIndex = 0; targetIndex != state.count; targetIndex++)
      BattleMotion_ReturnToSpot(state.actors[targetIndex]);
  }
  return 0;
}

/// Flashes a unit as it is hit, white and back twice in its damage pose.
void BattleMotion_FlashHit(s32 id, s32 unusedA, s32 unusedB) {
  u16 selection[2];
  s32 target;
  s32 flash;

  // DEAD: Unit_Get's result is not used; the ROM keeps the call.
  Unit_Get(id);
  Actor_SelectAnimationFar(BattleObject_Get(id)->object, POSE_DAMAGE);
  for (flash = 0; flash < 2; flash++) {
    target = BATTLE_UNIT_LIST_END;
    selection[1] = target;
    selection[0] = id;
    PartyStatus_HighlightMembersFar(selection);
    BattleObject_SetColorMode(BattleObject_Get(id)->object, SPRITE_COLOR_WHITE);
    wait(2);

    selection[0] = id;
    PartyStatus_HighlightMembersFar(selection);
    BattleObject_SetColorMode(BattleObject_Get(id)->object,
                              BattleMotion_GetSlotPalette(id));
    wait(2);
  }

  PartyStatus_DrawFar(Ram_HeapSlots->battleState->partyStatus);
}

/// Takes unit id out of the battle: off the party's or the enemies' list, its
/// name suffix freed and its actions this turn struck off. Returns -1 when it
/// is on neither list.
s32 BattleObject_RemoveFromLists(s32 id) {
  struct BattleState *battle;
  s32 i;
  u32 j;
  s32 unit;

  battle = Ram_HeapSlots->battleState;
  Unit_Get(id)->side = BATTLE_SIDE_NONE;
  for (i = 0;; i++) {
    unit = battle->partyUnits[i];
    if (unit == id) {
      battle->partyUnits[i] = BATTLE_UNIT_REMOVED;
      goto removed;
    }
    if (unit == BATTLE_UNIT_LIST_END)
      break;
  }

  for (j = 0;; j++) {
    unit = battle->enemyIds[j];
    if (unit == id) {
      battle->enemyIds[j] = BATTLE_UNIT_REMOVED;
      goto removed;
    }
    if (unit == BATTLE_UNIT_LIST_END)
      break;
  }
  return -1;

removed:
  BattleEnemy_ReleaseNameLetter(id);
  for (j = 0; j < BATTLE_ACTION_CAPACITY; j++) {
    if (battle->actions[j].unitId == id)
      battle->actions[j].unitId = BATTLE_UNIT_LIST_END;
  }
  return 0;
}

/// Shows unit id downed: a party member takes POSE_PARTY_HELD; an enemy takes
/// POSE_ENEMY_DOWN, turns grey, and four frames later leaves the battle and
/// dissolves pixel by pixel.
void BattleMotion_ShowDowned(s32 id) {
  void *sprites[4];
  struct Unit *unit;
  struct Sprite *sprite;
  struct SpriteAnimation *entry;
  s32 count;

  unit = Unit_Get(id);
  for (count = 0;
       (sprite = GetMotionRecord(BattleObject_Get(id)->object, count)) != 0;
       count++) {
    if (unit->side != BATTLE_SIDE_ENEMY)
      Sprite_StartAnimationFar(sprite, POSE_PARTY_HELD);
    else
      Sprite_StartAnimationFar(sprite, POSE_ENEMY_DOWN);
  }

  if (unit->side == BATTLE_SIDE_ENEMY) {
    for (count = 0;
         (sprite = GetMotionRecord(BattleObject_Get(id)->object, count)) != 0;
         count++) {
      entry = sprite->entries[0];
      sprites[count] = sprite;
      entry->colorMode = SPRITE_COLOR_GREY;
      entry->picture = SPRITE_PICTURE_NONE;
    }
    wait(4);
    BattleObject_RemoveFromLists(id);
    SpriteDissolve_RunManyFar(sprites, count);
    BattleObject_MarkForRelease(id);
  }
}
