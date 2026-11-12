/// @file
/// Auto battle: the actions it builds and plays without the command menu, the
/// shoulder-key camera and the escape.
// Part of battle.c.

#include "../battle_internal.h"

struct BattleSortedUnitEntry {
  u16 unitId;
  u16 unk02;
  s16 value;
  s16 width;
  s16 mode;
  s16 priority;
  u8 unk0c[4];
};

s32 AutoBattle_BuildActions(
    struct BattleSortedUnitEntry *entries) {
  struct BattleSortedUnitEntry swap;
  u16 unitIds[14];
  s32 firstCount;
  s32 count = 0;
  s32 secondCount;
  s32 priorityRange;
  s32 index;
  struct BattleSortedUnitEntry *entry;
  struct BattleUnit *unit;

  firstCount = BattleParty_ListLivingUnits(BATTLE_SIDE_PARTY, unitIds);
  for (index = 0; index != 4; index++)
    Unit_Get(index);
  for (index = 0; index < firstCount; index++) {
    s32 unitId = unitIds[index];

    unit = Unit_Get(unitId);
    entry = &entries[index];
    entry->unitId = unitId;
    entry->value = unit->agi;
    entry->width = 0;
    entry->mode = 0;
    entry->priority = 0x80;
    count++;
  }
  secondCount = BattleParty_ListLivingUnits(BATTLE_SIDE_ENEMIES, unitIds);
  entry = &entries[firstCount];
  priorityRange = BattleParty_ListLivingUnits(BATTLE_SIDE_PARTY, NULL);
  for (index = 0; index < secondCount; index++) {
    s32 unitId = unitIds[index];

    unit = Unit_Get(unitId);
    entry->unitId = unitId;
    entry->value = unit->agi >> 1;
    if (entry->value != 0)
      entry->value += (u32)(Random16() * unit->agi) >> 16;
    entry->width = 0;
    entry->mode = 0;
    entry->priority = (u32)(Random16() * priorityRange) >> 16;
    count++;
    entry++;
  }
  for (index = count - 2; index > 0; index--) {
    s32 swaps = 0;
    s32 pos;

    for (pos = count - 1; pos > 0; pos--) {
      if (entries[pos].value > entries[pos - 1].value) {
        Dma_Set(&entries[pos], &swap, DMA_COPY_WORDS(4), REG_DMA3);
        Dma_Set(&entries[pos - 1], &entries[pos], DMA_COPY_WORDS(4), REG_DMA3);
        Dma_Set(&swap, &entries[pos - 1], DMA_COPY_WORDS(4), REG_DMA3);
        swaps++;
      }
    }
    if (swaps == 0)
      break;
  }
  return count;
}

s32 AutoBattle_ShowDefend(s16 *);
s32 BattleStaging_RunUnitAction(s16 *);

void BattleCamera_AdjustByShoulderKeys(void) {
  struct BattleCamera *cam = Ram_HeapSlots->cameraWork;
  struct BattlePresentationTransition *trans = Ram_HeapSlots->transitionWork;

  if ((gInput.held & 512) != 0) {
    cam->yaw += 512;
  }
  // The held keys reread through a volatile pointer, a load apart from the L
  // test's; as gInput.held, 18 of 41 instructions differ in tla-en.
  if ((*(volatile u32 *)&gInput.held & 256) != 0) {
    cam->yaw -= 512;
  }
  if (trans->flag == 0) {
    BattleCamera_SetRange(0x780000, 0x780000, 0, 0, 0x10000);
  }
}

extern u8 gData_03001ae8[];
s32 Func_0811ca54(struct BattleActionRecord *action);
s32 Func_0811c710(struct BattleActionRecord *);

s32 AutoBattle_RunAction(s16 *action) {
  struct BattlePresentationTransition *transition;
  s32 actorId;
  s32 battleMode;
  u8 *actor;

  actorId = action[0];
  actor = Unit_Get(actorId);
  if (*(s16 *)(actor + 0x38) == 0)
    return -1;

  action[5] =
      BattleTarget_ReplaceDefeated((const struct BattleActionRecord *)action);
  transition = Ram_HeapSlots->transitionWork;
  if (action[0] > 4)
    battleMode = -0x2000;
  else
    battleMode = 0x2000;
  transition->targetYaw = battleMode;
  transition->frames = 60;
  MessageArgument_ClearAllFar();

  switch (action[3]) {
  case 99:
    Message_ShowAndWaitCoreFar((s32)&MsgPartyFlees);
    if (BattleEscape_PlayRun((struct BattleActionRecord *)action) != 0)
      return 1;
    break;
  case 3:
    wait(45);
    AutoBattle_ShowDefend(action);
    break;
  case 2:
    wait(45);
    Func_0811ca54((struct BattleActionRecord *)action);
    break;
  case 0:
  default: {
    struct BattlePresentationTransition *tr = Ram_HeapSlots->transitionWork;
    tr->flag = 0;
    Func_0811ca54((struct BattleActionRecord *)action);
    tr->flag = 0;
    break;
  }
  case 1:
    Func_0811c710((struct BattleActionRecord *)action);
    break;
  }

  Message_ResetIdleFar();
  return 0;
}

s32 BattleObject_IsValidId(u32 objectId) {
  if (objectId <= 7) {
    return 0;
  }
  if (objectId - 128 <= 5) {
    return 0;
  }
  return -1;
}

s32 BattleEscape_PlayRun(struct BattleActionRecord *action) {
  s16 party_members[14];
  s32 party_size;
  s32 animated;
  s32 member_slot;

  (void)action;
  if (((u32)(Random16() << 4) >> 16) != 0) {
    party_size =
        BattleParty_ListLivingUnits(BATTLE_SIDE_PARTY, (u16 *)party_members);
    animated = 0;
    if (party_size != 0) {
      member_slot = 0;
      do {
        BattleMotion_SetupEscapeObject(party_members[member_slot]);
        animated++;
        wait(8);
        member_slot++;
      } while (animated != party_size);
    }
    wait(22);
    return 1;
  }
  Message_ShowAndWaitCoreFar((s32)MsgCannotEscape);
  return 0;
}

s32 AutoBattle_ShowDefend(s16 *script) {
  s32 objectId;
  s32 result;
  struct BattleUnit *unit;

  objectId = *script;
  unit = Unit_Get(objectId);
  if (BattleObject_IsValidId(objectId) < 0) {
    return -1;
  }
  result = 0;
  if (unit->hp <= 0) {
    return result;
  }
  MessageArgument_ClearAllFar();
  MessageArgument_PushFar(objectId, 1);
  Message_ShowAndWaitCoreFar((s32)MsgActorDefends);
  return 0;
}

s32 Func_0811c710(struct BattleActionRecord *action) {
  u16 visibleUnits[14];
  struct BattlePresentationWork context;
  s32 actorId;
  s32 targetId;
  u32 sideStart;
  u32 sideCount;
  u32 livingCount;
  struct MotionObject *actorRecord;
  u32 index;

  livingCount = 0;
  actorId = action->unitId;
  if (BattleObject_IsValidId(actorId) < 0)
    return -1;
  targetId = action->target;
  if (BattleObject_IsValidId(targetId) < 0)
    return -1;

  {
    struct BattlePresentationTransition *transition =
        Ram_HeapSlots->transitionWork;
    transition->targetYaw = action->unitId > 4 ? 0x5000 : 0x2000;
    transition->frames = 60;
  }
  wait(10);
  Random16();
  actorRecord = BattleObject_Get(actorId)->object;

  if (targetId >= 0 && targetId <= 7) {
    sideCount = BattleParty_ListLivingUnits(2, visibleUnits);
    sideStart = 0x80;
  } else {
    sideCount = BattleParty_ListLivingUnits(1, visibleUnits);
    sideStart = 0;
  }

  for (index = 0; index != sideCount; index++) {
    if (index + sideStart == actorId)
      Actor_SetMode((struct Actor *)actorRecord, 3);
  }

  wait(30);
  REG_BLDCNT = 0x3f40;
  for (index = 0; index != sideCount; index++)
    BattleHighlight_SetSpriteMode(index + sideStart, 1);
  for (index = 0; index != 16; index++) {
    REG_BLDALPHA = (16 - index) | 0x1000;
    wait(1);
  }
  PartyStatus_DrawFar(9);

  if (targetId > 0x7f) {
    u32 count = BattleParty_ListLivingUnits(2, visibleUnits);
    for (index = 0; livingCount != count; index++) {
      u32 unit = index + 0x80;
      if (((struct BattleUnit *)Unit_Get(unit))->hp > 0)
        visibleUnits[livingCount++] = unit;
    }
  } else {
    u32 count = BattleParty_ListLivingUnits(1, visibleUnits);
    for (index = 0; livingCount != count; index++) {
      if (((struct BattleUnit *)Unit_Get(index))->hp > 0)
        visibleUnits[livingCount++] = index;
    }
  }
  visibleUnits[livingCount] = 0xff;
  BattleActor_SpawnObjectsForList((s16 *)visibleUnits, 0);

  context.kind = action->parameter;
  context.actor = actorId;
  for (index = 0; index != livingCount; index++)
    context.actors[index] = visibleUnits[index];
  context.count = livingCount;
  if (targetId >= 0 && targetId <= 7)
    context.side = 1;
  else
    context.side = 0;
  BattleEffect_RunSceneEffectFar((s32 *)&context);

  wait(10);
  BattleObject_Put();
  REG_BLDCNT = 0x3f40;
  for (index = 0; index != sideCount; index++)
    BattleHighlight_SetSpriteMode(index + sideStart, 1);
  for (index = 0; index != 16; index++) {
    REG_BLDALPHA = index | 0x1000;
    wait(1);
  }
  for (index = 0; index != sideCount; index++)
    BattleHighlight_SetSpriteMode(index + sideStart, 0);
  BattleCamera_SetView(0, 0, 0, 0x64);
  wait(3);
  return 0;
}

void BattleMotion_ApproachTarget(s32 actorId, s32 targetId, s32 travelDivisor,
                                 s32 initialVelocityY);
void Func_08138008(struct BattlePresentationWork *work);

/// The actor announces its attack, walks up to its target and plays the
/// attack's effect on it. Returns -1 when either id is not a battle object.
s32 AutoBattle_RunApproachAttack(struct BattleActionRecord *input) {
  struct BattlePresentationWork work;

  if (Ram_HeapSlots->transitionWork->targetYaw == 0x2000) {
    Ram_HeapSlots->transitionWork->targetYaw = 0x2000;
    wait(10);
  } else {
    Ram_HeapSlots->transitionWork->targetYaw = 0x2000;
    wait(30);
  }

  work.actor = input->unitId;
  if (BattleObject_IsValidId(work.actor) < 0)
    return -1;

  work.actors[0] = input->target;
  if (BattleObject_IsValidId(work.actors[0]) < 0)
    return -1;

  Unit_Get(work.actor);
  Unit_Get(work.actors[0]);
  Random16();
  MessageArgument_PushFar(work.actor, 1);
  Message_ShowAndWaitCoreFar((s32)MsgActorAttacks);
  BattleMotion_ApproachTarget(work.actor, work.actors[0], 13, 0);
  Actor_SetAnimationSpeedFar(
      (struct ActorView *)BattleObject_Get(work.actor)->object, 16);
  BattleObject_Get(work.actors[0]);

  work.count = 1;
  if ((u16)work.actors[0] <= 7)
    work.side = 1;
  else
    work.side = 0;
  work.flags = 0;

  wait(4);
  Func_08138008(&work);
  ObjMotRst(work.actors[0]);
  ObjMotRst(work.actor);
  return 0;
}

void Func_08138018(struct BattlePresentationWork *work);
void Func_08138020(struct BattlePresentationWork *work);

s32 Func_0811ca54(struct BattleActionRecord *action) {
  struct BattlePresentationWork work;
  struct BattlePresentationTransition *transition =
      Ram_HeapSlots->transitionWork;
  struct MotionObject *object;
  s32 facing;
  s32 i;

  if (action->unitId > 4)
    facing = -0x2000;
  else
    facing = 0x2000;
  if (transition->targetYaw == facing) {
    transition->frames = 40;
    wait(40);
  } else {
    transition->targetYaw = facing;
    transition->frames = 40;
    wait(40);
  }
  work.kind = action->parameter;
  work.initialValue = action->range;
  work.actor = action->unitId;
  work.firstTarget = action->target;
  if (BattleObject_IsValidId(work.actor) < 0)
    return -1;
  if (work.firstTarget > 127)
    work.count = BattleParty_ListLivingUnits(2, (u16 *)work.actors);
  else
    work.count = BattleParty_ListLivingUnits(1, (u16 *)work.actors);
  PartyStatus_DrawFar(Ram_HeapSlots->battleWork->partyStatus & ~1);
  object = BattleObject_Get(work.actor)->object;
  Actor_SetMode((struct Actor *)object, 3);
  Actor_SetAnimationSpeedFar((struct ActorView *)object, 16);
  if ((u16)action->target <= 7) {
    work.side = 1;
    BattleParty_ListLivingUnits(1, (u16 *)work.actors);
    work.count = 1;
  } else {
    work.side = 0;
    BattleParty_ListLivingUnits(2, (u16 *)work.actors);
    work.count = 1;
  }
  for (i = 0; i != work.count; i++) {
    struct ActorSpriteView *animation =
        BattleObject_Get(work.actors[i])->object->records;
    s32 j;

    for (j = 0; j != animation->count - 1; j++)
      work.params[i][j] = animation->entries[j]->param;
  }
  work.kind = 0;
  work.variant = 0;
  Func_08138020(&work);
  work.kind = 1;
  Func_08138020(&work);
  work.kind = 2;
  Func_08138020(&work);
  work.kind = 3;
  Func_08138020(&work);
  work.kind = 0;
  Func_08138018(&work);
  Actor_SetMode((struct Actor *)object, 1);
  // An empty once-only loop ends CSE's knowledge of work's address, so the
  // return loop's exit test addresses work afresh; without it, 103 of 180
  // instructions differ in tla-en.
  do {
  } while (0);
  for (i = 0; i != work.count; i++)
    ObjMotRst(work.actors[i]);
  ObjMotRst(work.actor);
  return 0;
}

void AutoBattle_ApplyDamage(u32 unitId, s32 damage, s32 showMessage,
                                        u8 *context) {
  u8 localContext[4];
  struct BattleUnit *character;
  struct BattleObj *slot;

  if (context == 0) {
    context = localContext;
    context[0] = 0;
    context[1] = 0;
    context[2] = 0;
    context[3] = 0;
  }

  character = Unit_Get(unitId);
  character->hp -= damage;
  if (character->hp < 0)
    character->hp = 0;

  slot = BattleObject_Get(unitId);
  Actor_SetMode((struct Actor *)slot->object, 5);
  PartyStatus_DrawFar(0);
  MessageArgument_ClearAllFar();

  if (unitId <= 7) {
    if (showMessage != 0)
      Message_ShowAndWaitCoreFar((s32)MsgBitterBlow);
    MessageArgument_PushFar(damage, 5);
    MessageArgument_PushFar(unitId, 1);
    Message_ShowAndWaitCoreFar((s32)MsgDmgP);
  } else {
    if (showMessage != 0)
      Message_ShowAndWaitCoreFar((s32)MsgCritical);
    MessageArgument_PushFar(damage, 5);
    MessageArgument_PushFar(unitId, 1);
    Message_ShowAndWaitCoreFar((s32)MsgDmgE);
    MessageArgument_PushFar(unitId, 1);
  }

  BattleMotion_SetMode5AndActivateSlot(unitId);
  if (unitId <= 7) {
    if (character->hp <= 0) {
      MessageArgument_PushFar(unitId, 1);
      Message_ShowAndWaitCoreFar((s32)MsgGoesDown);
    }
  } else if (character->hp <= 0) {
    MessageArgument_PushFar(unitId, 1);
    Message_ShowAndWaitCoreFar((s32)MsgDowned);
  }

  slot = BattleObject_Get(unitId);
  Actor_SetMode((struct Actor *)slot->object, 1);
}

struct BattleObjectAnimation {
  u8 unk00[0x28];
  struct AnimationEntry *entry;
};

s32 Animation_ApplyChildArgumentFar(void *group, s32 flags);

void BattleMotion_SetMode5AndActivateSlot(s32 unitId) {
  struct BattleObjectAnimation *animation;
  struct AnimationEntry *entry;

  if (((struct BattleUnit *)Unit_Get(unitId))->hp <= 0) {
    animation = BattleObject_Get(unitId)->object->records;
    Animation_ApplyChildArgumentFar(animation, 5);
    entry = animation->entry;
    entry->param = 6;
    entry->picture = 0xff;
    wait(4);
    Map_RenderAnimatedTilePicturesForObjectFar(animation);
    BattleObject_MarkForRelease(unitId);
  }
}

s32 BattleTarget_ReplaceDefeated(const struct BattleActionRecord *action) {
  s16 livingUnits[14];
  s32 targetId;
  s32 livingCount;

  targetId = action->target;
  if (((struct BattleUnit *)Unit_Get(targetId))->hp != 0) {
    return targetId;
  }

  if (targetId > 0x7F) {
    livingCount = BattleParty_ListLivingUnits(BATTLE_SIDE_ENEMIES, livingUnits);
  } else {
    livingCount = BattleParty_ListLivingUnits(BATTLE_SIDE_PARTY, livingUnits);
  }

  if (livingCount == 0) {
    return 0x100;
  }

  return livingUnits[(u32)(Random16() * livingCount) >> 16];
}
