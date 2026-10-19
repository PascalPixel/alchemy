/// @file
/// Part of battle.c, the battle object, compiled only through it.
/// A whole battle, Battle_RunEncounter, and the lists of the units in it.

#include "../battle_internal.h"

/// BattleWork.partyStatus: how the party status window draws, with the
/// Summon command offered or not.
#define PARTY_STATUS_NO_SUMMONS 1
#define PARTY_STATUS_SUMMONS 3

/// Battle_RunEncounter's results besides the number of enemies defeated.
#define ENCOUNTER_PARTY_LOST -1
#define ENCOUNTER_FLED 999

/// BattleWork.defeatState: whether the last enemy fell alone, or with others
/// before it ("has been downed!" or "'s party goes down!"); 3 when a party
/// member's action ended the battle with a single defeat.
#define DEFEAT_ONE 0
#define DEFEAT_SEVERAL 1
#define DEFEAT_BY_PARTY_MEMBER 3

/// The size of the block a link battle copies its field state into.
#define LINK_OWNER_STATES_SIZE 0x7c8

/// Fights the battle against the enemy formation enemyGroup, from the
/// opening transition to the end, and returns how it ended: the number of
/// enemies defeated when the party won (or the link broke), ENCOUNTER_FLED
/// when the party ran, or ENCOUNTER_PARTY_LOST.
///
/// Each round, the party's commands are chosen (or chosen automatically
/// under FLAG_AUTO_BATTLE), every unit acts in turn order, and the round ends
/// with the turn-end effects; the Djinn tutorial adds its two messages
/// after each round.
s32 Battle_RunEncounter(s32 enemyGroup) {
  struct BattleCamera *camera;
  s32 *focus;
  struct BattleWork *battle;
  u8 *ownerStates;
  struct BattlePresentationTransition *transition;
  struct BattleDisplayWork *display;
  s32 enemyCount;
  u8 *ownerStateDestination;
  s32 actionCount;
  s32 i;
  s32 result;
  struct UiWindow *tutorialWindow;
  s32 linkWaitFrames;
  u32 copyIndex;
  u32 actingUnit;
  s16 cue;
  u8 unused[48]; // never used: the native frame keeps these 48 bytes
  volatile u32 fill;

  // The battle's work areas.
  camera = Runtime_AllocateBlock(HEAP_SLOT_CAMERA, sizeof(struct BattleCamera));
  battle = Runtime_AllocateBlock(HEAP_SLOT_BATTLE, sizeof(struct BattleWork));
  ownerStates = Runtime_AllocateBlock(54, LINK_OWNER_STATES_SIZE);
  transition = Runtime_AllocateBlock(HEAP_SLOT_BATTLE_BACKGROUND, 32);
  Runtime_AllocateBlock(11, 0x280);
  focus = camera->pos;
  /* FAKEMATCH(dead): result set to 0, a store no read sees, whose register
   * CSE takes the setup's zeros from; without it, 122 of 645 instructions
   * differ in tbs-en. */
  result = 0;
  Iwram_ClearWords(ownerStates, LINK_OWNER_STATES_SIZE);
  Scheduler_ResetTaskTable();
  transition->frames = 0;
  transition->targetYaw = 0x2000;
  transition->flag = 1;
  transition->unk18 = 0;
  transition->unk1c = 0;
  REG_DISPCNT = DISPCNT_MODE_1;
  GameFlag_SetBitFar(FLAG_IN_BATTLE);
  GameFlag_SetBitFar(0x169);
  Render_ResetTransformState();

  Dma_Fill32Cell(fill, 0, camera, sizeof(struct BattleCamera));
  Dma_Fill32Cell(fill, 0, battle, sizeof(struct BattleWork));
  battle->promptSlot = -1;
  battle->enemyGroup = enemyGroup;
  display = Runtime_AllocateBlock(37, sizeof(struct BattleDisplayWork));
  Dma_Fill32Cell(fill, 0, display, sizeof(struct BattleDisplayWork));
  battle->background = Event_GetSpecialValueFar();
  Runtime_AllocateBlock(4, 0xe00);
  Runtime_AllocateBlock(3, 0x600);
  ObjectSystem_InitializeFar(4);
  if (GameFlag_TestFar(FLAG_DJINN_TUTORIAL) != 0)
    UiWork_InitializeFar(1);
  else
    UiWork_InitializeFar(0);

  // The camera's starting view, and the enemies.
  SET_VECTOR(focus, 0, 0x400000, 0);
  SET_VECTOR(camera->eye, 0, 0xb40000, 0x400000);
  camera->yaw = 0x2800;
  camera->pitch = 0x5000;
  camera->distance = 0x1000000;
  enemyCount = BattleFormation_BuildEnemyList(battle->enemyGroup);

  // A link battle: wait for the connection, take this console's side, keep
  // a copy of the field state, and fight as the formation's first type.
  if (GameFlag_TestFar(FLAG_LINK_BATTLE) != 0) {
    battle->twoSided = 1;
    Battle_SetStart(4);
  }
  if (battle->twoSided != 0) {
    /* FAKEMATCH(one-pass): the seed's store in a one-pass block, which keeps
     * it ahead of linkWaitFrames' zero; as a plain assignment, 7 of 645
     * instructions differ in tbs-en. */
    do {
      gBattleRandomSeed = 0;
    } while (0);
    for (linkWaitFrames = 0;;) {
      if ((gLinkStatus & LINK_BOTH_CONNECTED) == LINK_BOTH_CONNECTED)
        break;
      WaitFrames(1);
      linkWaitFrames++;
      if (linkWaitFrames > LINK_MAX_MISSED_FRAMES) {
        battle->linkPaused = 1;
        break;
      }
    }
    battle->linkSide = SIO_MULTI_PLAYER_ID();
    ownerStateDestination = gBattleOwnerStates;
    for (copyIndex = 0; copyIndex < LINK_OWNER_STATES_SIZE; copyIndex++)
      *ownerStateDestination++ = gActorSpriteSlots[copyIndex];
    enemyCount = GameFlag_GetByteFar(FLAG_LINK_PARTY_COUNT);
    BattleParty_AssignMemberSlots();
    battle->battleType = FORMATION_SHUFFLED;
  }
  Scheduler_AddOrUpdateCallback(BattlePresentation_UpdateCamera, 0xc7f);

  // The battle music: the scene's own cue, or the usual battle songs.
  cue = gGameState.sceneCue;
  if (cue != 0) {
    AudioCommand_PlayFar(cue);
    if (GameFlag_TestFar(FLAG_LINK_BATTLE) != 0) {
      AudioCommand_PlayFar(MUS_055);
      Sound_LoadPresetParameters(4);
    }
  } else {
    AudioCommand_PlayFar(MUS_051);
    AudioCommand_PlayFar(MUS_076);
  }

  // The units, the windows and the background.
  BattleParty_CollectUnitList();
  BattleUnit_RefreshPlacement();
  BattleParty_ReconcileDjinnRecovery();
  BattleSummon_UpdateAvailability();
  if (((struct BattleSummonState *)Djinn_GetRecoveryTableFar(
           DJINN_RECOVERY_PARTY))
          ->availableMask != 0)
    battle->partyStatus = PARTY_STATUS_SUMMONS;
  else
    battle->partyStatus = PARTY_STATUS_NO_SUMMONS;
  UiWindow_CreateWithLayoutBoundsFar(9);
  Camera_InitDefaultTransform();
  BattleObjPut();
  BattlePresentation_InitializeWorkAndResetState();
  BattleBackground_Load(1, battle->background, 0);
  BattleCamera_SetRange(0xa00000, 0x500000, 0, 0, 0x20000);
  BattlePres_SetupTransitionScene(0, 0, 0, 190);
  Party_ReservedNoOp5B14(1);
  REG_BLDCNT = 0;
  BattleSpoils_Clear();
  battle->promptSlot = VramCache_AllocEntry(128);

  // A random encounter may open with a first strike (1 in 16) or a
  // surprise (1 in 32 of the rest); the Djinn tutorial always strikes first.
  battle->encounterMode = BATTLE_ENCOUNTER_NORMAL;
  if (GameFlag_TestFar(FLAG_DJINN_TUTORIAL) != 0) {
    battle->encounterMode = BATTLE_ENCOUNTER_PARTY_FIRST;
  } else if (gGameState.battleStart == 0) {
    if ((BattleRandomFar() & 15) == 0)
      battle->encounterMode = BATTLE_ENCOUNTER_PARTY_FIRST;
    else if ((BattleRandomFar() & 31) == 0)
      battle->encounterMode = BATTLE_ENCOUNTER_ENEMIES_FIRST;
  }
  BattlePres_RunEntryTransition(enemyCount, enemyGroup);
  transition->flag = 0;
  gCpuLoadDisplayEnabled = 0;
  Scheduler_AddOrUpdateCallback(BattleActor_UpdateSpritePriorities, 0xc80);

  // The rounds.
  for (;;) {
    Battle_ReservedNoOp9B2C();
    BattleSummon_UpdateAvailability();
    if (((struct BattleSummonState *)Djinn_GetRecoveryTableFar(
             DJINN_RECOVERY_PARTY))
            ->availableMask != 0)
      battle->partyStatus = PARTY_STATUS_SUMMONS;
    else
      battle->partyStatus = PARTY_STATUS_NO_SUMMONS;
    transition->targetYaw = 0x2800;
    transition->frames = 60;
    PartyStatusDrawFar(battle->partyStatus);

    // Every unit's action for the round, in turn order.
    Iwram_ClearWords(battle->actions, sizeof(battle->actions));
    Resource_ResetEntry(battle->promptSlot);
    if (GameFlag_TestFar(FLAG_AUTO_BATTLE) == 0) {
      Runtime_GetRemainingIwram();
      Runtime_GetRemainingEwram();
      actionCount = BattlePresentation_BuildActions(battle->actions);
      Runtime_GetRemainingIwram();
      Runtime_GetRemainingEwram();
    } else {
      actionCount = BattlePresentation_BuildSortedUnitEntries(battle->actions);
    }
    battle->promptSlot = VramCache_AllocEntry(128);
    PartyStatusDrawFar(battle->partyStatus);
    if (actionCount < 0)
      goto aborted;

    // Each action, until one side is down or the party runs.
    for (i = 0; i < actionCount; i++) {
      actingUnit = battle->actions[i].unitId;
      Runtime_GetRemainingIwram();
      Runtime_GetRemainingEwram();
      if (GameFlag_TestFar(FLAG_AUTO_BATTLE) == 0) {
        if (BattlePresentation_DispatchAction(&battle->actions[i],
                                              i == 0 ? 10 : 0) == 1)
          goto fled;
      } else {
        if (BattlePres_RunAction(&battle->actions[i]) == 1)
          goto fled;
      }
      Runtime_GetRemainingIwram();
      Runtime_GetRemainingEwram();
      if (BattleParty_ListLivingUnits(BATTLE_SIDE_PARTY, 0) == 0)
        goto party_lost;
      if (BattleParty_ListLivingUnits(BATTLE_SIDE_ENEMIES, 0) == 0) {
        if (actingUnit <= 7 && battle->spoils.defeated == 1)
          battle->defeatState = DEFEAT_BY_PARTY_MEMBER;
        goto won;
      }
      if (BattlePres_SyncTurn() < 0)
        goto aborted;
    }

    // The end of the round.
    battle->encounterMode = BATTLE_ENCOUNTER_NORMAL;
    Battle_ReservedNoOpF674();
    Battle_ProcessRoundEnd();
    BattleMotion_DestroyAllSlotObjects();
    if (battle->twoSided != 0) {
      if (BattlePres_SyncTurn() < 0)
        goto aborted;
    } else {
      WaitFrames(20);
    }
    if (GameFlag_TestFar(FLAG_DJINN_TUTORIAL) == 0)
      continue;

    // The Djinn tutorial explains unleashing and standby.
    tutorialWindow = UiText_OpenMessageWindowFar(MsgTutorialUnleashed, 0, 4, 1);
    while (UiWork_IsCompleteFar() == 0)
      WaitFrames(1);
    UiWork_FinalizeFar(tutorialWindow, 1);
    WaitFrames(1);
    tutorialWindow = UiText_OpenMessageWindowFar(MsgTutorialStandby, 10, 4, 1);
    BattlePresentation_WaitForPromptAt(92, 24);
    UiWork_FinalizeFar(tutorialWindow, 1);
    WaitFrames(1);
  }

won:
  // Name the defeat ("X has been downed!" or "X's party goes down!") after
  // the first enemy class to fall, then hand out the spoils.
  Battle_ClearSerialTransferPayload();
  if (GameFlag_TestFar(FLAG_DJINN_TUTORIAL) == 0) {
    if (battle->twoSided != 0)
      AudioCommand_PlayFar(MUS_058);
    if (battle->spoils.defeated != 0) {
      AudioCommand_PlayFar(MUS_058);
      if (battle->defeatState <= DEFEAT_SEVERAL) {
        BattleUnitSetFar(FIRST_ENEMY_UNIT,
                         battle->enemyClasses[battle->firstDefeated], 26);
        UiWork_ClearValueNameTablesFar();
        UiWork_PushValueSlotFar(FIRST_ENEMY_UNIT, VALUE_UNIT_NAME);
        UiText_ShowMessageAndWaitCoreFar(battle->defeatState + MsgDowned);
        BattlePresentation_WaitForAdvance();
      }
    }
    Battle_AwardSpoils();
  }
  AudioCommand_PlayFar(MUS_017);
  Fade_ToBlack(30);
  Fade_Wait();
  result = battle->spoils.defeated;
  goto finished;

aborted:
  // The link broke, or no actions could be gathered.
  Battle_ClearSerialTransferPayload();
  Scheduler_SuspendTasks(0);
  REG_DISPCNT = DISPCNT_MODE_1;
  result = battle->spoils.defeated;
  GameFlag_SetBitFar(0x3e8);
  goto finished;

party_lost:
  // "X goes down in defeat..." for a lone member, else "X's party has been
  // defeated!".
  Battle_ClearSerialTransferPayload();
  AudioCommand_PlayFar(MUS_059);
  UiWork_ClearValueNameTablesFar();
  UiWork_PushValueSlotFar(gGameState.activeOwners[0], VALUE_UNIT_NAME);
  if (BattleParty_PrepareActiveOwners(0) == 1)
    UiText_ShowMessageAndWaitCoreFar(MsgGoesDownInDefeat);
  else
    UiText_ShowMessageAndWaitCoreFar(MsgPartyDefeated);
  BattlePresentation_WaitForAdvance();
  AudioCommand_PlayFar(MUS_017);
  Fade_ToBlack(30);
  result = ENCOUNTER_PARTY_LOST;
  Fade_Wait();
  goto finished;

fled:
  AudioCommand_PlayFar(MUS_017);
  Fade_ToBlack(30);
  Fade_Wait();
  result = ENCOUNTER_FLED;

finished:
  BattleParty_ResetActiveRuntimeFields();
  Battle_ReservedNoOpF674();
  DjinnRecovery_Tick();
  Battle_SetStart(0);
  Scheduler_RemoveCallback(BattleActor_UpdateSpritePriorities);
  Runtime_ReleaseHeapBlock10();
  return result;
}

/// Lists the party members who fight (the first four of the party, three in
/// a link battle) and marks each unit as a party member. owners, if not
/// null, receives their ids ended by BATTLE_UNIT_LIST_END. Returns how many
/// fight.
s32 BattleParty_PrepareActiveOwners(u16 *owners) {
  s32 limit;
  s32 count;
  s32 index;

  limit = PARTY_BATTLE_SIZE;
  if (gBattleWork->twoSided != 0)
    limit = LINK_PARTY_SIZE;

  count = Party_CountActiveOwnersFar();
  if (count > limit)
    count = limit;

  for (index = 0; index < count; index++) {
    s32 owner = gGameState.activeOwners[index];

    if (owners != 0)
      *owners++ = owner;
    UnitGetFar(owner)->side = BATTLE_UNIT_PARTY;
  }

  if (owners != 0)
    *owners = BATTLE_UNIT_LIST_END;
  return count;
}

/// Lists the enemy units present, ended by BATTLE_UNIT_LIST_END, and returns
/// how many there are (0 when unitIds is null).
s32 BattleEnemyList(s16 *unitIds) {
  s16 *output;
  s32 count;
  s32 id;
  s32 lastId;
  s32 listed;

  output = unitIds;
  listed = 0;
  lastId = ENEMY_BATTLE_SIZE;
  count = 0;
  if (output != NULL) {
    if (GameFlag_TestFar(FLAG_LINK_BATTLE) != 0) {
      lastId = LINK_PARTY_SIZE;
    }
    id = FIRST_ENEMY_UNIT;
    lastId += FIRST_ENEMY_UNIT;
    for (; id < lastId; id += 1) {
      if (UnitGetFar(id)->side != BATTLE_UNIT_ABSENT) {
        *output = id;
        listed += 1;
        output += 1;
      }
    }
    *output = BATTLE_UNIT_LIST_END;
    count = listed;
  }
  return count;
}

/// Lists the units still standing (HP above 0) on the sides in sideMask
/// (BATTLE_SIDE_PARTY, BATTLE_SIDE_ENEMIES or both), party first. unitIds,
/// if not null, receives them ended by BATTLE_UNIT_LIST_END. Returns how
/// many there are.
s32 BattleParty_ListLivingUnits(s32 sideMask, u16 *unitIds) {
  u16 activeMembers[8];
  u16 *base;
  s32 remaining;
  s32 lastEnemy;
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
  enemyCapacity = ENEMY_BATTLE_SIZE;
  if (GameFlag_TestFar(FLAG_LINK_BATTLE) != 0) {
    enemyCapacity = LINK_PARTY_SIZE;
  }
  // Count the living party members before scanning the enemy unit-id range.
  if (sideMask & BATTLE_SIDE_PARTY) {
    base = activeMembers;
    activeCount = BattleParty_PrepareActiveOwners(base);
    if (livingCount < activeCount) {
      member = base;
      remaining = activeCount;
      do {
        unitId = *member;
        member += 1;
        hp = UnitGetFar(unitId)->hp;
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
    // remaining walks the enemy unit ids here.
    remaining = FIRST_ENEMY_UNIT;
    lastEnemy = enemyCapacity + FIRST_ENEMY_UNIT;
    if (remaining < lastEnemy) {
      do {
        unit = UnitGetFar(remaining);
        if ((unit->side != BATTLE_UNIT_ABSENT) && (unit->hp > 0)) {
          if (output != NULL) {
            *output = remaining;
            output += 1;
          }
          livingCount += 1;
        }
        remaining += 1;
      } while (remaining < lastEnemy);
    }
  }
  if (output != NULL) {
    *output = BATTLE_UNIT_LIST_END;
  }
  return livingCount;
}

/// Lists the units in the battle's unit lists on the sides in sideMask,
/// party first, skipping those removed from the battle. unitIds, if not
/// null, receives them ended by BATTLE_UNIT_LIST_END. Returns how many
/// there are.
s32 BattleParty_ListActorIds(s32 sideMask, u16 *unitIds) {
  struct BattleWork *battle;
  s32 count;
  s32 i;

  battle = gBattleWork;
  count = 0;
  if (sideMask & BATTLE_SIDE_PARTY) {
    for (i = 0; battle->partyUnits[i] != BATTLE_UNIT_LIST_END; i++) {
      if (battle->partyUnits[i] != BATTLE_UNIT_REMOVED) {
        if (unitIds != NULL)
          *unitIds++ = battle->partyUnits[i];
        count++;
      }
    }
  }
  if (sideMask & BATTLE_SIDE_ENEMIES) {
    for (i = 0; battle->enemyIds[i] != BATTLE_UNIT_LIST_END; i++) {
      if (battle->enemyIds[i] != BATTLE_UNIT_REMOVED) {
        if (unitIds != NULL)
          *unitIds++ = battle->enemyIds[i];
        count++;
      }
    }
  }
  if (unitIds != NULL)
    *unitIds = BATTLE_UNIT_LIST_END;
  return count;
}
