/// @file
/// A whole battle, Func_081197f0 (The Broken Seal's Battle_Run), its tutorial
/// helpers, and the lists of the units in it.
// Part of battle.c.

#include "../battle_internal.h"

s32 Func_0811a24c(s32 sideMask, u16 *unitIds);

void Func_081195ec(void) {
  u16 actors[6];
  u16 reserves[6];
  s32 actorCount;
  s32 reserveCount;
  s32 count;
  struct BattleWork *work = Ram_HeapSlots->battleWork;
  s32 i;

  if (Func_0811a24c(1, 0) == 0)
    return;
  count = actorCount = BattleParty_ListActorIds(1, actors);
  reserveCount = BattleParty_PrepareReserveOwners(reserves);
  if (count > reserveCount)
    count = reserveCount;
  BattleHighlight_SetModes(actors, 4);
  wait(32);
  for (i = 0; i < count; i++) {
    s32 unit = work->partyUnits[i];
    s32 id = reserves[i];
    struct BattleObj *from = BattleObject_Get(unit);
    struct BattleObj *to = BattleObject_Get(id);
    struct MotionObject *object = to->object;
    struct MotionObject *place = from->object;

    to->anchorX = from->anchorX;
    to->anchorZ = from->anchorZ;
    Actor_SetPositionAndResetMotionFar((void *)object, place->x, place->y,
                                        place->z);
    gMainRoutineWork.activeOwners[i] = id;
    work->partyUnits[i] = id;
  }
  for (i = 0; i < count; i++)
    gMainRoutineWork.activeOwners[actorCount + i] = actors[i];
  PartyStatus_DrawFar(Ram_HeapSlots->battleWork->partyStatus);
  BattleObject_Put();
  BattleHighlight_SetModes(reserves, 6);
  wait(32);
}

/// Allocates the battle camera (heap slot 48) and sets its starting view.
// The values are those The Broken Seal's battle start sets inline after its
// own allocation.
void BattleCamera_Create(void) {
  struct BattleCamera *camera =
      HeapSlot_AllocEwram(48, sizeof(struct BattleCamera));
  s32 *focus = camera->pos;

  SET_VECTOR(focus, 0, 0x400000, 0);
  SET_VECTOR(camera->eye, 0, 0xb40000, 0x400000);
  camera->yaw = 0x2800;
  camera->pitch = 0x5000;
  camera->distance = 0x1000000;
}

struct BattleFxRequest {
  s32 field00;
  s32 field04;
  s32 field08;
  s32 field0c;
  s32 field10;
  s32 field14;
  s32 field18;
  u8 unk1c[8];
  s16 field24;
  u8 unk26[0x32];
};

void Func_08138050(struct BattleFxRequest *request, s32 value);

s32 BattleUnit_SetFar(s32 slot, s32 unit, s32 mask);

void Func_08119734(s32 id) {
  struct BattleWork *work = Ram_HeapSlots->battleWork;
  s32 unit;
  s32 *object;

  if (id == 78)
    unit = 373;
  else
    unit = 376;
  BattleObject_MarkForRelease(128);
  BattleMotion_DestroyAllSlotObjects();
  BattleEnemy_ClaimNameLetter(unit, 1);
  BattleUnit_SetFar(128, unit, 255);
  work->enemyIds[0] = 128;
  work->enemyIds[1] = 255;
  Enemy_Update();
  object = (s32 *)BattleObject_Get(128);
  BattleObject_Make((struct BattleObj *)object, 128, object[3] / Q16_ONE,
                    object[4] / Q16_ONE);
  BattleObject_Put();
}

void Func_081197a8(s32 value) {
  struct BattleFxRequest request;

  request.field10 = 0;
  request.field08 = 128;
  request.field0c = 0;
  request.field14 = 1;
  request.field24 = 128;
  request.field04 = 0;
  request.field00 = 0;
  request.field18 = 0;
  Func_08138050(&request, value);
}

s32 GameFlag_Test(s32);

s32 Tutorial_GetDjinnGiveMessage(void) {
  if (GameFlag_Test(0x175))
    return (s32)MsgDjinnGiveTutorialB;
  return (s32)MsgDjinnGiveTutorialA;
}

struct EncounterWork {
  s32 enemyGroup;
  u8 unk004[0x0c];
  u16 enemyClasses[22];
  u16 firstDefeated;
  u16 defeatState;
  u8 unk040;
  u8 partyStatus;
  u8 battleType;
  u8 challengeFlag;
  u8 twoSided;
  u8 encounterMode;
  u8 unk046;
  u8 actionEnded;
  u8 ownerSlots[8];
  u8 linkSide;
  u8 linkMisses;
  u8 linkPaused;
  u8 unk053;
  s32 promptSlot;
  s16 partyUnits[7];
  s16 enemyIds[7];
  u8 placement[16];
  u8 slots[14][44];
  struct BattleActionRecord actions[20];
  u8 unk42c[0x12c];
  s32 defeated;
  u8 unk55c[0x114];
  u16 background;
  u8 unk672[0x1e6];
  s32 pendingUnit;
  s32 pendingTag;
  u16 extraQueue;
  u16 nextExtraQueue;
  u8 extraCount;
  u8 nextExtraCount;
  u8 unk866[0x16];
};
extern u8 gCpuLoadDisplayEnabled;
/// The actor sprite slot buffers, 0x4000 bytes a slot.
extern u8 Data_02018000[];

// The ROM's call here keeps the whole return register, as an s32 return
// does; system/system/flags.c defines it returning u8.
s32 GameFlag_GetByte(s32 flag);
s32 Event_GetSpecialValueFar(void);
void ObjectSystem_InitializeFar(s32 mode);
void WindowSystem_InitBattleFar(s32 mode);
s32 Func_08127cd4(s32 enemyGroup);
s32 Task_Add(void (*func)(void), s32 order);
void Sound_LoadPresetParameters(s32 preset);
void BattleUnit_RefreshPlacement(void);
void PartyStatus_CreateWindowFar(s32 layout);
void Func_081263c4(void);
void Func_081263fc(s32 mode, s32 background, s32 value);
void BattleSpoils_Clear(void);
s32 Func_0811a39c(s32 groups, u16 *dst);
void Func_08125d74(s32 enemyCount, s32 enemyGroup);
void Func_0811b9fc(void);
void Func_0811d7e4(void);
s32 Battle_GatherActions(struct BattleActionRecord *actions);
struct BattleSortedUnitEntry;
s32 AutoBattle_BuildActions(struct BattleSortedUnitEntry *entries);
s32 Func_0811d9cc(struct BattleActionRecord *action, s32 delay);
s32 AutoBattle_RunAction(s16 *action);
s32 Battle_FindTaggedSlotByValue(u32 id);
void Func_08124cc0(void);
void Func_08124e20(void);
s32 Message_AllTypedFar(void);
void Window_CloseFar(s32 window, s32 mode);
void Func_08120060(s32 x, s32 y);
void Battle_AwardSpoils(void);
void Fade_ToBlack(s32 vblanks);
void Fade_Wait(void);
void Func_081263f0(void);

s32 Func_081197f0(s32 enemyGroup) {
  struct BattleCamera *camera;
  struct EncounterWork *battle;
  struct BattlePresentationTransition *transition;
  s32 enemyCount;
  u8 *ownerStateDestination;
  s32 round;
  s32 actionCount;
  s32 i;
  s32 result;
  s32 window;
  s32 linkWaitFrames;
  u32 copyIndex;
  u32 actingUnit;
  s16 cue;
  s32 scripted;

  camera = HeapSlot_AllocEwram(48, sizeof(struct BattleCamera));
  battle = HeapSlot_AllocEwram(36, sizeof(struct EncounterWork));
  HeapSlot_AllocEwram(216, 0x7c8);
  transition = HeapSlot_AllocEwram(176, 32);
  HeapSlot_AllocEwram(44, 0x280);
  result = 0;
  Iwram_ClearWords(Ram_HeapSlots->unkd0[2], 0x7c8);
  Task_ResetAll();
  transition->targetYaw = 0x2000;
  transition->frames = 0;
  transition->flag = 1;
  transition->unk18 = 0;
  transition->unk1c = 0;
  *(volatile u16 *)0x04000000 = 1;
  GameFlag_SetBit(0x103);
  GameFlag_SetBit(0x169);
  Render_ResetTransformState();

  // The camera's fill in a once-only block keeps its entry's load after the
  // transform reset; as the plain call, 12 of 885 instructions differ in
  // tla-en.
  do {
    Iwram_FillWords(camera, sizeof(struct BattleCamera), 0);
  } while (0);
  Iwram_FillWords(battle, sizeof(struct EncounterWork), 0);
  battle->promptSlot = -1;
  battle->enemyGroup = enemyGroup;
  HeapSlot_AllocEwram(148, 12);
  // The slot's fill in a once-only block keeps its address after the
  // allocation; as the plain call, 6 of 885 instructions differ in tla-en.
  do {
    Iwram_FillWords(Ram_HeapSlots->unk84[4], 12, 0);
  } while (0);
  battle->background = Event_GetSpecialValueFar();
  HeapSlot_AllocEwram(16, 0xe00);
  HeapSlot_AllocEwram(12, 0x600);
  ObjectSystem_InitializeFar(4);
  if (GameFlag_Test(0x16e) != 0)
    WindowSystem_InitBattleFar(1);
  else
    WindowSystem_InitBattleFar(0);

  BattleCamera_Create();
  enemyCount = Func_08127cd4(battle->enemyGroup);

  if (GameFlag_Test(0x16c) != 0) {
    battle->twoSided = 1;
    /* FAKEMATCH(dead): an empty once-only block after the twoSided store, so
     * battleStart's address is started after it; without it, 4 of 885
     * instructions differ in tla-en. */
    do {
    } while (0);
    gMainRoutineWork.battleRule24b = 4;
  }
  if (battle->twoSided != 0) {
    // The seed's store in a once-only block keeps it ahead of the wait loop's
    // setup; as the plain store, 4 of 885 instructions differ in tla-en.
    do {
      gBattleRandomSeed = 0;
    } while (0);
    for (linkWaitFrames = 0;;) {
      if ((gLinkStatus & 3) == 3)
        break;
      wait(1);
      linkWaitFrames++;
      if (linkWaitFrames > 24) {
        battle->linkPaused = 1;
        break;
      }
    }
    battle->linkSide = (*(volatile u32 *)0x04000128 << 26) >> 30;
    ownerStateDestination = Ram_HeapSlots->unkd0[2];
    for (copyIndex = 0; copyIndex < 0x7c8; copyIndex++)
      *ownerStateDestination++ = Data_02018000[copyIndex];
    enemyCount = GameFlag_GetByte(0x3f0);
    BattleParty_AssignMemberSlots();
    battle->battleType = 0;
  }
  Task_Add(Func_08118adc, 0x47f);

  cue = gMainRoutineWork.sceneCue;
  if (cue != 0) {
    Sound_PlayFar(cue);
    if (GameFlag_Test(0x16c) != 0) {
      Sound_PlayFar(55);
      Sound_LoadPresetParameters(4);
    }
  } else {
    Sound_PlayFar(50);
  }

  Func_08118c68();
  BattleUnit_RefreshPlacement();
  Func_08118eb0();
  Func_08118f6c();
  if (*(u16 *)Djinn_GetRecoveryTableFar(0) != 0)
    battle->partyStatus = 3;
  else
    battle->partyStatus = 1;
  if (enemyGroup == 75 || enemyGroup == 76)
    Func_081185c4();
  PartyStatus_CreateWindowFar(9);
  Camera_InitializeDefaultTransform();
  BattleObject_Put();
  Func_081263c4();
  Func_081263fc(1, battle->background, 0);
  BattleCamera_SetRange(0xa00000, 0x500000, 0, 0, 0x20000);
  BattleCamera_SetView(0, 0, 0, 190);
  Party_ReservedNoOp5B14(1);
  *(volatile u16 *)0x04000050 = 0;
  BattleSpoils_Clear();
  battle->promptSlot = VramCache_AllocEntry(128);

  battle->encounterMode = 0;
  if (GameFlag_Test(0x16e) != 0) {
    battle->encounterMode = 1;
  } else if (gMainRoutineWork.battleRule24b == 0) {
    if ((Battle_RandomFar() & 15) == 0)
      battle->encounterMode = 1;
    else if ((Battle_RandomFar() & 31) == 0)
      battle->encounterMode = 2;
  } else if (gMainRoutineWork.battleRule24b == 6) {
    battle->encounterMode = 2;
  }
  Func_08125d74(enemyCount, enemyGroup);
  transition->flag = 0;
  gCpuLoadDisplayEnabled = 0;
  Task_Add(Func_0811b9fc, 0x480);

  for (round = 1;; round++) {
    if (Func_0811a39c(1, 0) == 0)
      Func_081195ec();
    Func_0811d7e4();
    Func_08118f6c();
    if (*(u16 *)Djinn_GetRecoveryTableFar(0) != 0)
      battle->partyStatus = 3;
    else
      battle->partyStatus = 1;
    transition->targetYaw = 0x2800;
    transition->frames = 60;
    PartyStatus_DrawFar(battle->partyStatus);

    Iwram_ClearWords(battle->actions, sizeof(battle->actions));
    Resource_ResetEntry(battle->promptSlot);
    if (GameFlag_Test(0x16a) == 0)
      actionCount = Battle_GatherActions(battle->actions);
    else
      actionCount = AutoBattle_BuildActions(
          (struct BattleSortedUnitEntry *)battle->actions);
    battle->promptSlot = VramCache_AllocEntry(128);
    PartyStatus_DrawFar(battle->partyStatus);
    if (actionCount < 0)
      goto aborted;

    for (i = 0; i < actionCount; i++) {
      actingUnit = battle->actions[i].unitId;
      battle->pendingUnit = -1;
      battle->actionEnded = 0;
      if (GameFlag_Test(0x16a) == 0) {
        if (Func_0811d9cc(&battle->actions[i], i == 0 ? 10 : 0) == 1)
          goto fled;
      } else {
        if (AutoBattle_RunAction((s16 *)&battle->actions[i]) == 1)
          goto fled;
      }
      if (enemyGroup == 638)
        goto party_lost;
      if (Func_0811a24c(1, 0) == 0)
        goto party_lost;
      if (BattleParty_ListLivingUnits(1, 0) == 0)
        break;
      if (BattleParty_ListLivingUnits(2, 0) == 0) {
        if (enemyGroup == 78 || enemyGroup == 79) {
          Func_081197a8(enemyGroup++);
          break;
        }
        if (enemyGroup == 80)
          Func_081197a8(80);
        if (actingUnit <= 7 && battle->defeated == 1) {
          /* FAKEMATCH(barrier): an empty asm before the defeatState store,
           * so battle's copy (mov r5, r9) comes before the 3; without it, 2
           * of 885 instructions differ in tla-en. */
          asm("");
          battle->defeatState = 3;
        }
        goto won;
      }
      if (battle->pendingUnit != -1 &&
          Battle_FindTaggedSlotByValue(battle->pendingTag) != -1) {
        battle->actions[i].unitId = battle->pendingUnit;
        battle->actions[i].command = 9;
        battle->actions[i].parameter = 0;
        battle->actions[i].target =
            Battle_FindTaggedSlotByValue(battle->pendingTag);
        battle->actions[i].range = 1;
        i--;
      }
      if (LinkBattle_SyncTurn() < 0)
        goto aborted;
      if (i == actionCount - 1 && battle->extraCount != 0) {
        u32 queue = battle->extraQueue;

        battle->extraQueue = queue >> 1;
        queue &= 1;
        battle->extraCount--;
        battle->actions[i].unitId = queue << 7;
        battle->actions[i].command = 10;
        battle->actions[i].parameter = 0;
        battle->actions[i].target = queue != 0 ? 0 : 0x80;
        battle->actions[i].range = 255;
        i--;
      }
      if (battle->actionEnded != 0)
        break;
    }

    Func_08118e64();
    battle->extraQueue = battle->nextExtraQueue;
#if TLA_PATCH_1
    // Japanese leaves the next turn's queue in place.
    battle->nextExtraQueue = 0;
#endif
    battle->extraCount = battle->nextExtraCount;
    battle->nextExtraCount = 0;
    battle->encounterMode = 0;
    Func_08124cc0();
    Func_08124e20();
    BattleMotion_DestroyAllSlotObjects();
    scripted = enemyGroup - 74;
    if ((u32)scripted <= 1 && round == 2)
      goto won_scripted;
    if (battle->twoSided != 0) {
      if (LinkBattle_SyncTurn() < 0)
        goto aborted_scripted;
    } else {
      wait(20);
    }
    if (GameFlag_Test(0x16e) == 0)
      continue;

    window = Message_OpenWindowFar(Tutorial_GetDjinnGiveMessage() + 7, 0, 4, 1);
    while (Message_AllTypedFar() == 0)
      wait(1);
    Window_CloseFar(window, 1);
    wait(1);
    window =
        Message_OpenWindowFar(Tutorial_GetDjinnGiveMessage() + 8, 10, 4, 1);
    Func_08120060(44, 24);
    Window_CloseFar(window, 1);
    wait(1);
  }

won:
  scripted = enemyGroup - 74;
won_scripted:
  BattleStaging_ClearLinkPayload();
  if ((u32)scripted > 1) {
    if (enemyGroup != 80 && GameFlag_Test(0x16e) == 0) {
      if (battle->twoSided != 0)
        Sound_PlayFar(58);
      if (battle->defeated != 0) {
        Sound_PlayFar(58);
        if (battle->defeatState <= 1) {
          BattleUnit_SetFar(128, battle->enemyClasses[battle->firstDefeated],
                            26);
          MessageArgument_ClearAllFar();
          MessageArgument_PushFar(128, 1);
          Message_ShowAndWaitCoreFar(battle->defeatState + (s32)MsgDowned);
          BattleMessage_WaitForAdvance();
        }
      }
      Battle_AwardSpoils();
    }
    Sound_PlayFar(78);
  }
  Fade_ToBlack(30);
  Fade_Wait();
  result = battle->defeated;
  goto finished;

aborted:
  scripted = enemyGroup - 74;
aborted_scripted:
  BattleStaging_ClearLinkPayload();
  Task_SuspendAll(0);
  *(volatile u16 *)0x04000000 = 1;
  result = battle->defeated;
  GameFlag_SetBit(1000);
  goto finished;

party_lost:
  BattleStaging_ClearLinkPayload();
  scripted = enemyGroup - 74;
  if ((u32)scripted > 1 && enemyGroup != 638) {
    if (enemyGroup != 76) {
      Sound_PlayFar(59);
      MessageArgument_ClearAllFar();
      if (BattleParty_PrepareActiveOwners(0) == 1)
        Message_ShowAndWaitCoreFar((s32)MsgGoesDownInDefeat);
      else
        Message_ShowAndWaitCoreFar((s32)MsgPartyDefeated);
      BattleMessage_WaitForAdvance();
    }
    Sound_PlayFar(78);
  }
  Fade_ToBlack(30);
  result = -1;
  Fade_Wait();
  goto finished;

fled:
  Sound_PlayFar(78);
  Fade_ToBlack(30);
  Fade_Wait();
  result = 999;
  scripted = enemyGroup - 74;

finished:
  if ((u32)scripted <= 1)
    Func_0811843c();
  BattleParty_ResetActiveRuntimeFields();
  Func_08124cc0();
  DjinnRecovery_CountDown();
  gMainRoutineWork.battleRule24b = 0;
  Task_Remove(Func_0811b9fc);
  Func_081263f0();
  return result;
}

s32 BattleParty_PrepareActiveOwners(u16 *owners) {
  s32 limit;
  s32 count;
  s32 index;

  limit = 4;
  if (Ram_HeapSlots->battleWork->twoSided != 0)
    limit = 3;

  count = Party_CountActiveOwnersFar();
  if (count > limit)
    count = limit;

  for (index = 0; index < count; index++) {
    s32 owner = gMainRoutineWork.activeOwners[index];

    if (owners != 0)
      *owners++ = owner;
    ((struct BattleUnit *)Unit_Get(owner))->side = 2;
  }

  if (owners != 0)
    *owners = 0xff;
  return count;
}

s32 BattleParty_PrepareReserveOwners(u16 *owners) {
  s32 count;
  s32 index;

  count = Party_CountActiveOwnersFar();
  if (count > 4)
    count -= 4;
  else
    count = 0;
  if (Ram_HeapSlots->battleWork->twoSided != 0)
    count = 0;

  for (index = 0; index < count; index++) {
    s32 owner = gMainRoutineWork.activeOwners[index + 4];

    if (owners != 0)
      *owners++ = owner;
    ((struct BattleUnit *)Unit_Get(owner))->side = 2;
  }

  if (owners != 0)
    *owners = 0xff;
  return count;
}

s32 BattleEnemy_List(s16 *unit_ids) {
  s16 *output;
  s32 count;
  s32 id;
  s32 limit;
  s32 listed;

  output = unit_ids;
  listed = 0;
  limit = 6;
  count = 0;
  if (output != NULL) {
    if (GameFlag_Test(0x16c) != 0)
      limit = 3;
    id = 0x80;
    limit += 0x80;
    for (; id < limit; id++) {
      if (((struct BattleUnit *)Unit_Get(id))->side != 0) {
        *output = id;
        listed++;
        output++;
      }
    }
    count = listed;
    *output = 0xff;
  }
  return count;
}

s32 BattleParty_ListLivingUnits(s32 sideMask, u16 *unitIds) {
  u16 activeMembers[8];
  u16 *base;
  s32 remaining;
  s32 enemyLimit;
  u16 *output;
  s32 livingCount;
  s32 enemyCapacity;
  u16 *member;
  s32 unitId;
  s16 hp;
  s32 activeCount;
  struct BattleUnit *unit;

  output = unitIds;
  livingCount = 0;
  enemyCapacity = 6;
  if (GameFlag_Test(0x16C) != 0) {
    enemyCapacity = 3;
  }
  if (sideMask & BATTLE_SIDE_PARTY) {
    base = activeMembers;
    activeCount = BattleParty_PrepareActiveOwners(base);
    if (livingCount < activeCount) {
      member = base;
      remaining = activeCount;
      do {
        unitId = *member;
        member += 1;
        hp = ((struct BattleUnit *)Unit_Get(unitId))->hp;
        if (hp > 0) {
          if (output != NULL) {
            *output = unitId;
            output += 1;
          }
          livingCount += 1;
        }
        remaining -= 1;
      } while (remaining != 0);
    }
  }
  if (sideMask & BATTLE_SIDE_ENEMIES) {
    remaining = 0x80;
    enemyLimit = enemyCapacity + 0x80;
    if (remaining < enemyLimit) {
      do {
        unit = Unit_Get(remaining);
        if (unit->side != 0 && unit->hp > 0) {
          if (output != NULL) {
            *output = remaining;
            output += 1;
          }
          livingCount += 1;
        }
        remaining += 1;
      } while (remaining < enemyLimit);
    }
  }
  if (output != NULL) {
    *output = 0xFF;
  }
  return livingCount;
}

/// As BattleParty_ListLivingUnits, with the reserve owners listed after the
/// active ones.
s32 Func_0811a24c(s32 sideMask, u16 *unitIds) {
  u16 members[10];
  u16 *base;
  s32 remaining;
  s32 enemyLimit;
  u16 *output;
  s32 livingCount;
  s32 enemyCapacity;
  u16 *member;
  s32 unitId;
  s16 hp;
  s32 count;
  struct BattleUnit *unit;

  output = unitIds;
  livingCount = 0;
  enemyCapacity = 6;
  if (GameFlag_Test(0x16C) != 0) {
    enemyCapacity = 3;
  }
  if (sideMask & BATTLE_SIDE_PARTY) {
    base = members;
    count = BattleParty_PrepareActiveOwners(base);
    count += BattleParty_PrepareReserveOwners(base + count);
    if (livingCount < count) {
      member = base;
      remaining = count;
      do {
        unitId = *member;
        member += 1;
        hp = ((struct BattleUnit *)Unit_Get(unitId))->hp;
        if (hp > 0) {
          if (output != NULL) {
            *output = unitId;
            output += 1;
          }
          livingCount += 1;
        }
        remaining -= 1;
      } while (remaining != 0);
    }
  }
  if (sideMask & BATTLE_SIDE_ENEMIES) {
    remaining = 0x80;
    enemyLimit = enemyCapacity + 0x80;
    if (remaining < enemyLimit) {
      do {
        unit = Unit_Get(remaining);
        if (unit->side != 0 && unit->hp > 0) {
          if (output != NULL) {
            *output = remaining;
            output += 1;
          }
          livingCount += 1;
        }
        remaining += 1;
      } while (remaining < enemyLimit);
    }
  }
  if (output != NULL) {
    *output = 0xFF;
  }
  return livingCount;
}

s32 BattleParty_ListActorIds(s32 groups, u16 *dst) {
  struct BattleWork *order;
  s32 count;
  s32 i;

  order = Ram_HeapSlots->battleWork;
  count = 0;
  if (groups & 1) {
    for (i = 0; order->partyUnits[i] != 255; i++) {
      if (order->partyUnits[i] != 254) {
        if (dst != NULL)
          *dst++ = order->partyUnits[i];
        count++;
      }
    }
  }
  if (groups & 2) {
    for (i = 0; order->enemyIds[i] != 255; i++) {
      if (order->enemyIds[i] != 254) {
        if (dst != NULL)
          *dst++ = order->enemyIds[i];
        count++;
      }
    }
  }
  if (dst != NULL)
    *dst = 255;
  return count;
}

/// As BattleParty_ListActorIds, listing only the actors whose HP is not 0.
s32 Func_0811a39c(s32 groups, u16 *dst) {
  struct BattleWork *order;
  s32 count;
  s32 i;

  order = Ram_HeapSlots->battleWork;
  count = 0;
  if (groups & 1) {
    for (i = 0; order->partyUnits[i] != 255; i++) {
      if (order->partyUnits[i] != 254 &&
          ((struct BattleUnit *)Unit_Get(order->partyUnits[i]))->hp != 0) {
        if (dst != NULL)
          *dst++ = order->partyUnits[i];
        count++;
      }
    }
  }
  if (groups & 2) {
    for (i = 0; order->enemyIds[i] != 255; i++) {
      if (order->enemyIds[i] != 254 &&
          ((struct BattleUnit *)Unit_Get(order->enemyIds[i]))->hp != 0) {
        if (dst != NULL)
          *dst++ = order->enemyIds[i];
        count++;
      }
    }
  }
  if (dst != NULL)
    *dst = 255;
  return count;
}
