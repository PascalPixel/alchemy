/// @file
/// A whole battle, Battle_RunEncounter, and the lists of the units in it.
// Part of battle.c.

#include "../battle_internal.h"

/// Battle_RunEncounter's results besides the number of enemies defeated.
#define ENCOUNTER_PARTY_LOST -1
#define ENCOUNTER_FLED 999

/// The six enemy units, BATTLE_UNIT_SIZE bytes each: the
/// HEAP_SLOT_LINK_OWNER_STATES work slot, gBattleOwnerStates, which UnitGet
/// reads for unit ids 128 to 133 while it is allocated (game/game/units.c). The
/// link lobby passes the opponent's party to the battle in a copy of this block
/// (menu/lobby.c).
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
///
/// Reached through its far stub from Game_Run's scene switch
/// (SCENE_BATTLE, with the entrance as enemyGroup), and from
/// DebugBattle_RunMenu. Allocates the battle's work slots (the camera,
/// gBattleWork, the enemy units, the background transition, the display and
/// others), sets FLAG_IN_BATTLE, and on its way out resets the party's
/// battle state, sets gGameState.battleStart back to BATTLE_START_NORMAL and
/// releases heap block 10. A broken link also sets flag FLAG_PLACE_TEMP_E8,
/// which the link lobby reads (LinkLobby_RunRoundResult).
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
  UNUSED u8 unused[48]; // never used: the native frame keeps these 48 bytes
  // The fill word Dma_Fill32Cell reads its zero from.
  volatile u32 fill;

  // The battle's work areas. The object system's sprite and layer pools
  // are taken here from EWRAM first: ObjectSystem_InitializeFar's own requests
  // for them then get these blocks, as an allocated slot is returned as it
  // is (field/field/animation.c, runtime/runtime/heap.c).
  camera = Runtime_AllocateBlock(HEAP_SLOT_CAMERA, sizeof(struct BattleCamera));
  battle = Runtime_AllocateBlock(HEAP_SLOT_BATTLE, sizeof(struct BattleWork));
  ownerStates = Runtime_AllocateBlock(HEAP_SLOT_LINK_OWNER_STATES,
                                      LINK_OWNER_STATES_SIZE);
  transition = Runtime_AllocateBlock(HEAP_SLOT_BATTLE_BACKGROUND, 32);
  Runtime_AllocateBlock(11, 0x280);
  focus = camera->pos;
  /* FAKEMATCH(dead): result set to 0, a store no read sees, whose register
   * CSE takes the setup's zeros from; without it, 122 of 645 instructions
   * differ in tbs-en. */
  result = 0;
  Iwram_ClearWords(ownerStates, LINK_OWNER_STATES_SIZE);
  Scheduler_ResetTaskTable();
  // The transition owns the background (flag) until the entry transition
  // is over, the camera toward the party's view.
  transition->frames = 0;
  transition->targetYaw = YAW_PARTY_TURN;
  transition->flag = 1;
  transition->unk18 = 0;
  transition->unk1c = 0;
  REG_DISPCNT = DISPCNT_MODE_1;
  GameFlag_SetBitFar(FLAG_IN_BATTLE);
  // XXX: what flag 0x169 marks; nothing in the source reads it by number,
  // so a reader through another name or a script would settle it.
  GameFlag_SetBitFar(0x169);
  Render_ResetTransformState();

  Dma_Fill32Cell(fill, 0, camera, sizeof(struct BattleCamera));
  Dma_Fill32Cell(fill, 0, battle, sizeof(struct BattleWork));
  battle->promptSlot = -1;
  battle->enemyGroup = enemyGroup;
  display = Runtime_AllocateBlock(HEAP_SLOT_BATTLE_DISPLAY,
                                  sizeof(struct BattleDisplayWork));
  Dma_Fill32Cell(fill, 0, display, sizeof(struct BattleDisplayWork));
  battle->background = Event_GetSpecialValueFar();
  Runtime_AllocateBlock(HEAP_SLOT_ANIMATION_SPRITES, 0xe00);
  Runtime_AllocateBlock(HEAP_SLOT_ANIMATION_LAYERS, 0x600);
  ObjectSystem_InitializeFar(MODE_BATTLE);
  if (GameFlag_TestFar(FLAG_DJINN_LESSON) != 0)
    UiWork_InitializeFar(1);
  else
    UiWork_InitializeFar(0);

  // The camera's starting view, in 16.16 pixels and 1/65536 turns: the focus
  // 64 pixels up, the eye at (0, 180, 64), a yaw of 0x2800 and a distance of
  // 256 pixels. Then the enemies.
  SET_VECTOR(focus, 0, 0x400000, 0);
  SET_VECTOR(camera->eye, 0, 0xb40000, 0x400000);
  camera->yaw = 0x2800;
  camera->pitch = 0x5000;
  camera->distance = 0x1000000;
  enemyCount = BattleFormation_BuildEnemyList(battle->enemyGroup);

  // A link battle: wait for the connection, take this console's side, put
  // the opponent's party into the enemy units from the copy the link lobby
  // left at gActorSpriteSlots (menu/lobby.c), and lay it out left to right
  // (FORMATION_SHUFFLED). Both consoles start from seed 0 so their rolls
  // agree; the opponent's party size is the enemy count.
  if (GameFlag_TestFar(FLAG_LINK_BATTLE) != 0) {
    battle->twoSided = 1;
    Battle_SetStart(BATTLE_START_LINK);
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
  // The party status window, opened PARTY_STATUS_CLOSED: laid out, but erased
  // until it is drawn (window/battle_command/party_status.c).
  UiWindow_CreateWithLayoutBoundsFar(PARTY_STATUS_CLOSED);
  Camera_InitDefaultTransform();
  BattleObjPut();
  BattlePresentation_InitializeWorkAndResetState();
  BattleBackground_Load(BATTLE_BACKGROUND_FLAT, battle->background, 0);
  // The camera on the centre at 190 percent, until the opening sets its own
  // zoom (BattlePresentation_RunEntryTransition). With the background
  // BATTLE_BACKGROUND_FLAT, BattleCamera_SetRange builds no plane page; it sets
  // at most BG1's horizontal scroll from the camera's yaw.
  BattleCamera_SetRange(0xa00000, 0x500000, 0, 0, 0x20000);
  BattlePresentation_SetupTransitionScene(0, 0, 0, 190);
  Party_ReservedNoOp5B14(1);
  REG_BLDCNT = 0;
  BattleSpoils_Clear();
  // 128 bytes of VRAM cache for the advance arrow's tiles, which event
  // playback shows after a message (PLAY_ADVANCE); taken again each round.
  battle->promptSlot = VramCache_AllocEntry(128);

  // A random encounter may open with a first strike (1 in 16) or a
  // surprise (1 in 32 of the rest); the Djinn tutorial always strikes first.
  battle->encounterMode = BATTLE_ENCOUNTER_NORMAL;
  if (GameFlag_TestFar(FLAG_DJINN_LESSON) != 0) {
    battle->encounterMode = BATTLE_ENCOUNTER_PARTY_FIRST;
  } else if (gGameState.battleStart == BATTLE_START_NORMAL) {
    if ((BattleRandomFar() & 15) == 0)
      battle->encounterMode = BATTLE_ENCOUNTER_PARTY_FIRST;
    else if ((BattleRandomFar() & 31) == 0)
      battle->encounterMode = BATTLE_ENCOUNTER_ENEMIES_FIRST;
  }
  // After the entry transition the camera callback owns the background
  // again, and the sprite priorities are sorted every frame.
  BattlePresentation_RunEntryTransition(enemyCount, enemyGroup);
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
    // The camera eases back to its starting yaw over 60 frames, and the
    // party status window is drawn with or without its Djinn column.
    transition->targetYaw = 0x2800;
    transition->frames = 60;
    PartyStatusDrawFar(battle->partyStatus);

    // Every unit's action for the round, in turn order.
    Iwram_ClearWords(battle->actions, sizeof(battle->actions));
    Resource_ResetEntry(battle->promptSlot);
    if (GameFlag_TestFar(FLAG_AUTO_BATTLE) == 0) {
      // XXX: likely free-memory probes left from debugging, because no
      // caller here reads their results.
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

    // Each action, until one side is down or the party runs. The round's
    // first action waits 10 frames facing the party first.
    for (i = 0; i < actionCount; i++) {
      actingUnit = battle->actions[i].unitId;
      Runtime_GetRemainingIwram();
      Runtime_GetRemainingEwram();
      if (GameFlag_TestFar(FLAG_AUTO_BATTLE) == 0) {
        if (BattlePresentation_DispatchAction(&battle->actions[i],
                                              i == 0 ? 10 : 0) == 1)
          goto fled;
      } else {
        if (BattlePresentation_RunAction(&battle->actions[i]) == 1)
          goto fled;
      }
      Runtime_GetRemainingIwram();
      Runtime_GetRemainingEwram();
      if (BattleParty_ListLivingUnits(BATTLE_SIDE_PARTY, 0) == 0)
        goto party_lost;
      if (BattleParty_ListLivingUnits(BATTLE_SIDE_ENEMIES, 0) == 0) {
        // Unit ids 0 to 7 are party members.
        if (actingUnit <= ALLY_MAX && battle->spoils.defeated == 1)
          battle->defeatState = DEFEAT_BY_PARTY_MEMBER;
        goto won;
      }
      if (BattlePresentation_SyncTurn() < 0)
        goto aborted;
    }

    // The end of the round.
    battle->encounterMode = BATTLE_ENCOUNTER_NORMAL;
    Battle_ReservedNoOpF674();
    Battle_ProcessRoundEnd();
    BattleMotion_DestroyAllSlotObjects();
    if (battle->twoSided != 0) {
      if (BattlePresentation_SyncTurn() < 0)
        goto aborted;
    } else {
      WaitFrames(20);
    }
    if (GameFlag_TestFar(FLAG_DJINN_LESSON) == 0)
      continue;

    // The Djinn tutorial explains unleashing and standby: two windows that
    // zoom open at tiles (0, 4) and (10, 4), the second closed by a key at
    // the cursor, at pixel (92, 24).
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
  // the first enemy class to fall, then hand out the spoils. Unit 128 is
  // rewritten as that class to carry the name, bare: letter 26 is past every
  // edition's last (BattleUnitSet). DEFEAT_BY_PARTY_MEMBER shows no line.
  Battle_ClearSerialTransferPayload();
  if (GameFlag_TestFar(FLAG_DJINN_LESSON) == 0) {
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
  // The link broke, or no actions could be gathered. Flag FLAG_PLACE_TEMP_E8
  // tells the link lobby (LinkLobby_RunRoundResult) the battle did not finish.
  Battle_ClearSerialTransferPayload();
  Scheduler_SuspendTasks(0);
  REG_DISPCNT = DISPCNT_MODE_1;
  result = battle->spoils.defeated;
  GameFlag_SetBitFar(FLAG_PLACE_TEMP_E8);
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
  // Every way out: Djinn recover one step, the next battle starts normally,
  // and the per-frame sorting and the affine scanlines (block 10) go.
  BattleParty_ResetActiveRuntimeFields();
  Battle_ReservedNoOpF674();
  DjinnRecovery_Tick();
  Battle_SetStart(BATTLE_START_NORMAL);
  Scheduler_RemoveCallback(BattleActor_UpdateSpritePriorities);
  Runtime_ReleaseHeapBlock10();
  return result;
}

/// Lists the party members who fight (the first four of the party, three in
/// a link battle) and marks each unit as a party member. owners, if not
/// null, receives their ids ended by BATTLE_UNIT_LIST_END. Returns how many
/// fight. Called across the battle parts whenever they need the party, and
/// through its far stub by the party status window (party_status.c). Sets
/// each fighting member's side to BATTLE_UNIT_PARTY.
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
/// how many there are (0 when unitIds is null). A link battle looks only at
/// the opponent's three places. Called by BattleIntro_AnnounceEncounter,
/// BattleParty_CollectUnitList and BattleTargetExec.
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
/// many there are. Called across the battle parts (the round, staging, auto
/// battle, turn end and spoils) to test whether a side still stands and to
/// pick targets.
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
  // The party, in its battle order; then every enemy unit id that is
  // present, which a link battle limits to the opponent's three.
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
/// there are. Unlike BattleParty_ListLivingUnits, it does not test HP.
/// Called across the battle parts that draw or target units, and through
/// its far stub by the command windows (command_row.c, party_commands.c).
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
