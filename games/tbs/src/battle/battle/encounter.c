/// @file
/// A whole battle, Battle_Run, and the lists of the units in it.
// Part of battle.c.

#include "../battle_internal.h"

/// Battle_Run's results besides the number of enemies defeated.
#define BATTLE_OUTCOME_LOST -1
#define BATTLE_OUTCOME_FLED 999

/// The six enemy units, BATTLE_UNIT_SIZE bytes each: the
/// HEAP_SLOT_ENEMY_UNITS heap slot, gEnemyUnits, which Unit_Get
/// reads for unit ids 128 to 133 while it is allocated (game/game/units.c). The
/// Battle Arena passes the opponent's party to the battle in a copy of this
/// block (menu/battle_arena.c).
#define ENEMY_UNITS_SIZE 0x7c8

/// Fights the battle against the enemy formation enemyGroup, from the
/// opening transition to the end, and returns how it ended: the number of
/// enemies defeated when the party won (or the link broke), BATTLE_OUTCOME_FLED
/// when the party ran, or BATTLE_OUTCOME_LOST.
///
/// Each turn, the party's commands are chosen (or chosen automatically
/// under FLAG_AUTO_BATTLE), every unit acts in action order, and the turn ends
/// with the turn-end effects; the Djinn tutorial adds its two messages
/// after each turn.
///
/// Reached through its far stub from ParentSequence_Run's switch on the
/// sequence (SEQUENCE_BATTLE, with the entrance as enemyGroup), and from
/// DebugBattle_RunMenu. Allocates the battle's heap slots (the camera,
/// gBattleWork, the enemy units, the background transition, the battle message
/// window and others), sets FLAG_IN_BATTLE, and on its way out resets the
/// party's battle state, sets gAdventure.battleType back to
/// BATTLE_TYPE_NORMAL and releases heap block 10. A broken link also sets flag
/// FLAG_AREA_TEMP_E8, which the Battle Arena reads
/// (BattleArena_RunRoundResult).
s32 Battle_Run(s32 enemyGroup) {
  struct BattleCamera *camera;
  s32 *focus;
  struct BattleWork *battle;
  u8 *enemyUnits;
  struct BattleBackgroundWork *view;
  struct BattleMessageWork *display;
  s32 enemyCount;
  u8 *enemyUnitDestination;
  s32 actionCount;
  s32 i;
  s32 result;
  struct Window *tutorialWindow;
  s32 linkWaitFrames;
  u32 copyIndex;
  u32 actingUnit;
  s16 cue;
  UNUSED u8
      unused[48]; // never used: the native stack frame keeps these 48 bytes
  // The fill word Dma_Fill32Cell reads its zero from.
  volatile u32 fill;

  // The battle's work areas. The actor system's sprite and layer pools
  // are taken here from EWRAM first: ActorSystem_InitializeFar's own requests
  // for them then get these blocks, as an allocated slot is returned as it
  // is (field/field/animation.c, runtime/runtime/heap.c).
  camera = HeapSlot_AllocEwram(HEAP_SLOT_CAMERA, sizeof(struct BattleCamera));
  battle = HeapSlot_AllocEwram(HEAP_SLOT_BATTLE, sizeof(struct BattleWork));
  enemyUnits = HeapSlot_AllocEwram(HEAP_SLOT_ENEMY_UNITS, ENEMY_UNITS_SIZE);
  view = HeapSlot_AllocEwram(HEAP_SLOT_BATTLE_BACKGROUND, 32);
  HeapSlot_AllocEwram(11, 0x280);
  focus = camera->focus;
  /* FAKEMATCH(dead): result set to 0, a store no read sees, whose register
   * CSE takes the setup's zeros from; without it, 122 of 645 instructions
   * differ in tbs-en. */
  result = 0;
  Iwram_ClearWords(enemyUnits, ENEMY_UNITS_SIZE);
  Task_ResetAll();
  // The transition owns the background (flag) until the entry transition
  // is over, the camera toward the party's view.
  view->frames = 0;
  view->targetYaw = YAW_PARTY_ACTION;
  view->flag = 1;
  view->unusedAfterFlag[0] = 0;
  view->unusedAfterFlag[1] = 0;
  REG_DISPCNT = DISPCNT_MODE_1;
  GameFlag_SetFar(FLAG_IN_BATTLE);
  // Flag 0x169 is set as every battle starts and never cleared; no code,
  // event, flag range or byte read in this game reads it.
  // XXX (no evidence): what it marks; with no reader in the ROM, nothing the
  // game does shows its use.
  GameFlag_SetFar(0x169);
  Transform_Reset();

  Dma_Fill32Cell(fill, 0, camera, sizeof(struct BattleCamera));
  Dma_Fill32Cell(fill, 0, battle, sizeof(struct BattleWork));
  battle->arrowTiles = -1;
  battle->formation = enemyGroup;
  display = HeapSlot_AllocEwram(HEAP_SLOT_BATTLE_MESSAGE,
                                sizeof(struct BattleMessageWork));
  Dma_Fill32Cell(fill, 0, display, sizeof(struct BattleMessageWork));
  battle->background = Event_GetSpecialValueFar();
  HeapSlot_AllocEwram(HEAP_SLOT_SPRITES, 0xe00);
  HeapSlot_AllocEwram(HEAP_SLOT_SPRITE_ANIMATIONS, 0x600);
  ActorSystem_InitializeFar(ACTOR_SYSTEM_BATTLE);
  if (GameFlag_GetFar(FLAG_DJINN_LESSON) != 0)
    WindowSystem_InitBattleFar(1);
  else
    WindowSystem_InitBattleFar(0);

  // The camera's starting view, in 16.16 pixels and 1/65536 turns: the focus
  // 64 pixels up, the eye at (0, 180, 64), a yaw of 0x2800 and a distance of
  // 256 pixels. Then the enemies.
  SET_VECTOR(focus, 0, 0x400000, 0);
  SET_VECTOR(camera->eye, 0, 0xb40000, 0x400000);
  camera->yaw = 0x2800;
  camera->pitch = 0x5000;
  camera->distance = 0x1000000;
  enemyCount = BattleFormation_BuildEnemyList(battle->formation);

  // A link battle: wait for the connection, take this console's side, put
  // the opponent's party into the enemy units from the copy the Battle Arena
  // left at gActorSpriteSlots (menu/battle_arena.c), and lay it out left to
  // right (FORMATION_LAYOUT_SHUFFLED). Both consoles start from seed 0 so their
  // rolls agree; the opponent's party size is the enemy count.
  if (GameFlag_GetFar(FLAG_LINK_BATTLE) != 0) {
    battle->isLink = 1;
    Battle_SetType(BATTLE_TYPE_LINK);
  }
  if (battle->isLink != 0) {
    /* FAKEMATCH(one-pass): the seed's store in a one-pass block, which keeps
     * it ahead of linkWaitFrames' zero; as a plain assignment, 7 of 645
     * instructions differ in tbs-en. */
    do {
      gBattleRandomSeed = 0;
    } while (0);
    for (linkWaitFrames = 0;;) {
      if ((gLinkStatus & LINK_BOTH_CONNECTED) == LINK_BOTH_CONNECTED)
        break;
      wait(1);
      linkWaitFrames++;
      if (linkWaitFrames > LINK_MAX_MISSED_FRAMES) {
        battle->linkPaused = 1;
        break;
      }
    }
    battle->linkSide = SIO_MULTI_PLAYER_ID();
    enemyUnitDestination = gEnemyUnits;
    for (copyIndex = 0; copyIndex < ENEMY_UNITS_SIZE; copyIndex++)
      *enemyUnitDestination++ = gActorSpriteSlots[copyIndex];
    enemyCount = GameFlag_GetByteFar(FLAG_LINK_PARTY_COUNT);
    BattleParty_AssignOpponentSlots();
    battle->layout = FORMATION_LAYOUT_SHUFFLED;
  }
  Task_Add(BattleCamera_Update, TASK_PASS_FRAME - 1);

  // The battle music: the song an event set for this battle, or the usual
  // battle songs.
  cue = gAdventure.battleMusic;
  if (cue != 0) {
    Sound_PlayFar(cue);
    if (GameFlag_GetFar(FLAG_LINK_BATTLE) != 0) {
      Sound_PlayFar(MUS_BOULDER_CHASE);
      Sound_LoadReverbCode(SOUND_REVERB_NONE);
    }
  } else {
    Sound_PlayFar(MUS_SATUROS_BATTLE);
    Sound_PlayFar(MUS_COLOSSO_BATTLE);
  }

  // The units, the windows and the background.
  BattleUnit_Collect();
  BattleUnit_RefreshPlacement();
  DjinnRecovery_Reconcile();
  BattleSummon_UpdateAvailability();
  if (((struct BattleSummonState *)Djinn_GetRecoveryTableFar(
           DJINN_RECOVERY_PARTY))
          ->availableMask != 0)
    battle->partyStatus = PARTY_STATUS_SUMMONS;
  else
    battle->partyStatus = PARTY_STATUS_NO_SUMMONS;
  // The party status window, opened PARTY_STATUS_CLOSED: laid out, but erased
  // until it is drawn (window/screens/party_status.c).
  PartyStatus_CreateWindowFar(PARTY_STATUS_CLOSED);
  BattleCamera_SetDefaultView();
  BattleObject_Put();
  BattleBackground_Init();
  BattleBackground_Load(BATTLE_BACKGROUND_FIXED, battle->background, 0);
  // The camera on the centre at 190 percent, until the opening sets its own
  // zoom (BattleTransition_Run). With the background
  // BATTLE_BACKGROUND_FIXED, BattleCamera_SetRange builds no plane page; it
  // sets at most BG1's horizontal scroll from the camera's yaw.
  BattleCamera_SetRange(0xa00000, 0x500000, 0, 0, 0x20000);
  BattleCamera_SetView(0, 0, 0, 190);
  Party_BattleSetupNoOp(1);
  REG_BLDCNT = 0;
  BattleSpoils_Clear();
  // 128 bytes of VRAM cache for the advance arrow's tiles, which
  // playback shows after a message (PLAY_ADVANCE); taken again each turn.
  battle->arrowTiles = VramCache_AllocEntry(128);

  // A random encounter may open with a first strike (1 in 16) or a
  // surprise (1 in 32 of the rest); the Djinn tutorial always strikes first.
  battle->firstStrike = FIRST_STRIKE_NONE;
  if (GameFlag_GetFar(FLAG_DJINN_LESSON) != 0) {
    battle->firstStrike = FIRST_STRIKE_PARTY;
  } else if (gAdventure.battleType == BATTLE_TYPE_NORMAL) {
    if ((Battle_RandomFar() & 15) == 0)
      battle->firstStrike = FIRST_STRIKE_PARTY;
    else if ((Battle_RandomFar() & 31) == 0)
      battle->firstStrike = FIRST_STRIKE_ENEMIES;
  }
  // After the entry transition the camera task owns the background
  // again, and the sprite priorities are sorted every frame.
  BattleTransition_Run(enemyCount, enemyGroup);
  view->flag = 0;
  gCpuLoadDisplayEnabled = 0;
  Task_Add(BattleObject_UpdateSpritePriorities, TASK_PASS_FRAME);

  // The turns.
  for (;;) {
    Battle_TurnStartNoOp();
    BattleSummon_UpdateAvailability();
    if (((struct BattleSummonState *)Djinn_GetRecoveryTableFar(
             DJINN_RECOVERY_PARTY))
            ->availableMask != 0)
      battle->partyStatus = PARTY_STATUS_SUMMONS;
    else
      battle->partyStatus = PARTY_STATUS_NO_SUMMONS;
    // The camera eases back to its starting yaw over 60 frames, and the
    // party status window is drawn with or without its Djinn column.
    view->targetYaw = 0x2800;
    view->frames = 60;
    PartyStatus_DrawFar(battle->partyStatus);

    // Every unit's action for the turn, in action order.
    Iwram_ClearWords(battle->actions, sizeof(battle->actions));
    VramCache_FreeEntry(battle->arrowTiles);
    if (GameFlag_GetFar(FLAG_AUTO_BATTLE) == 0) {
      // The free-memory reads around the command menu change nothing: each
      // only subtracts, and its result is dropped.
      // XXX (no evidence): who read them; a debug build might have shown the
      // figures, but nothing in the ROM keeps them.
      Heap_GetRemainingIwram();
      Heap_GetRemainingEwram();
      actionCount = Battle_GatherActions(battle->actions);
      Heap_GetRemainingIwram();
      Heap_GetRemainingEwram();
    } else {
      actionCount = AutoBattle_BuildActions(battle->actions);
    }
    battle->arrowTiles = VramCache_AllocEntry(128);
    PartyStatus_DrawFar(battle->partyStatus);
    if (actionCount < 0)
      goto aborted;

    // Each action, until one side is down or the party runs. The turn's
    // first action waits 10 frames facing the party first.
    for (i = 0; i < actionCount; i++) {
      actingUnit = battle->actions[i].unitId;
      Heap_GetRemainingIwram();
      Heap_GetRemainingEwram();
      if (GameFlag_GetFar(FLAG_AUTO_BATTLE) == 0) {
        if (BattleStaging_DispatchAction(&battle->actions[i],
                                         i == 0 ? 10 : 0) == 1)
          goto fled;
      } else {
        if (AutoBattle_RunAction(&battle->actions[i]) == 1)
          goto fled;
      }
      Heap_GetRemainingIwram();
      Heap_GetRemainingEwram();
      if (BattleUnit_ListLiving(BATTLE_SIDE_MASK_PARTY, 0) == 0)
        goto party_lost;
      if (BattleUnit_ListLiving(BATTLE_SIDE_MASK_ENEMIES, 0) == 0) {
        // Unit ids 0 to 7 are party members.
        if (actingUnit <= ALLY_MAX && battle->spoils.defeated == 1)
          battle->defeatState = DEFEAT_BY_PARTY_MEMBER;
        goto won;
      }
      if (LinkBattle_SyncTurn() < 0)
        goto aborted;
    }

    // The end of the turn.
    battle->firstStrike = FIRST_STRIKE_NONE;
    Battle_ReservedNoOpF674();
    Battle_ProcessTurnEnd();
    BattleObject_DestroyAll();
    if (battle->isLink != 0) {
      if (LinkBattle_SyncTurn() < 0)
        goto aborted;
    } else {
      wait(20);
    }
    if (GameFlag_GetFar(FLAG_DJINN_LESSON) == 0)
      continue;

    // The Djinn tutorial explains unleashing and standby: two windows that
    // zoom open at tiles (0, 4) and (10, 4), the second closed by a key at
    // the cursor, at pixel (92, 24).
    tutorialWindow = Message_OpenWindowFar(MsgTutorialUnleashed, 0, 4, 1);
    while (Message_AllTypedFar() == 0)
      wait(1);
    Window_CloseFar(tutorialWindow, 1);
    wait(1);
    tutorialWindow = Message_OpenWindowFar(MsgTutorialStandby, 10, 4, 1);
    BattleMessage_WaitForAdvanceAt(92, 24);
    Window_CloseFar(tutorialWindow, 1);
    wait(1);
  }

won:
  // Name the defeat ("X has been downed!" or "X's party goes down!") after
  // the first enemy to fall, then hand out the spoils. Unit 128 is
  // rewritten as that class to carry the name, bare: letter 26 is past every
  // edition's last (BattleUnit_Set). DEFEAT_BY_PARTY_MEMBER shows no line.
  Battle_ClearSerialTransferPayload();
  if (GameFlag_GetFar(FLAG_DJINN_LESSON) == 0) {
    if (battle->isLink != 0)
      Sound_PlayFar(MUS_VICTORY);
    if (battle->spoils.defeated != 0) {
      Sound_PlayFar(MUS_VICTORY);
      if (battle->defeatState <= DEFEAT_SEVERAL) {
        BattleUnit_SetFar(FIRST_ENEMY_UNIT,
                          battle->enemyKinds[battle->firstDefeated], 26);
        MessageArgument_ClearAllFar();
        MessageArgument_PushFar(FIRST_ENEMY_UNIT, MESSAGE_ARGUMENT_ACTOR);
        BattleMessage_ShowAndWaitFar(battle->defeatState + MsgDowned);
        BattleMessage_WaitForAdvance();
      }
    }
    Battle_AwardSpoils();
  }
  Sound_PlayFar(SOUND_FADE_OUT_BGM);
  ScreenFade_ToBlack(30);
  ScreenFade_Wait();
  result = battle->spoils.defeated;
  goto finished;

aborted:
  // The link broke, or no actions could be gathered. Flag FLAG_AREA_TEMP_E8
  // tells the Battle Arena (BattleArena_RunRoundResult) the battle did not
  // finish.
  Battle_ClearSerialTransferPayload();
  Task_SuspendAll(0);
  REG_DISPCNT = DISPCNT_MODE_1;
  result = battle->spoils.defeated;
  GameFlag_SetFar(FLAG_AREA_TEMP_E8);
  goto finished;

party_lost:
  // "X goes down in defeat..." for a lone party member, else "X's party has
  // been defeated!".
  Battle_ClearSerialTransferPayload();
  Sound_PlayFar(MUS_DEFEAT);
  MessageArgument_ClearAllFar();
  MessageArgument_PushFar(gAdventure.party[0], MESSAGE_ARGUMENT_ACTOR);
  if (BattleParty_PrepareMembers(0) == 1)
    BattleMessage_ShowAndWaitFar(MsgGoesDownInDefeat);
  else
    BattleMessage_ShowAndWaitFar(MsgPartyDefeated);
  BattleMessage_WaitForAdvance();
  Sound_PlayFar(SOUND_FADE_OUT_BGM);
  ScreenFade_ToBlack(30);
  result = BATTLE_OUTCOME_LOST;
  ScreenFade_Wait();
  goto finished;

fled:
  Sound_PlayFar(SOUND_FADE_OUT_BGM);
  ScreenFade_ToBlack(30);
  ScreenFade_Wait();
  result = BATTLE_OUTCOME_FLED;

finished:
  // Every way out: Djinn recover one step, the next battle starts normally,
  // and the per-frame sorting and the affine scanlines (block 10) go.
  BattleParty_ResetActiveRuntimeFields();
  Battle_ReservedNoOpF674();
  DjinnRecovery_CountDown();
  Battle_SetType(BATTLE_TYPE_NORMAL);
  Task_Remove(BattleObject_UpdateSpritePriorities);
  BattleBackground_Free();
  return result;
}

/// Lists the party members who fight (the first four of the party, three in
/// a link battle) and marks each unit as a party member. unitIds, if not
/// null, receives their ids ended by BATTLE_UNIT_LIST_END. Returns how many
/// fight. Called across the battle parts whenever they need the party, and
/// through its far stub by the party status window (party_status.c). Sets
/// each fighting party member's side to BATTLE_SIDE_PARTY.
s32 BattleParty_PrepareMembers(u16 *unitIds) {
  s32 limit;
  s32 count;
  s32 index;

  limit = PARTY_BATTLE_SIZE;
  if (gBattleWork->isLink != 0)
    limit = LINK_PARTY_SIZE;

  count = Party_CountMembersFar();
  if (count > limit)
    count = limit;

  for (index = 0; index < count; index++) {
    s32 unitId = gAdventure.party[index];

    if (unitIds != 0)
      *unitIds++ = unitId;
    Unit_GetFar(unitId)->side = BATTLE_SIDE_PARTY;
  }

  if (unitIds != 0)
    *unitIds = BATTLE_UNIT_LIST_END;
  return count;
}

/// Lists the enemy units present, ended by BATTLE_UNIT_LIST_END, and returns
/// how many there are (0 when unitIds is null). A link battle looks only at
/// the opponent's three places. Called by BattleIntro_AnnounceEncounter,
/// BattleUnit_Collect and BattleAbility_ApplyToTarget.
s32 BattleEnemy_List(s16 *unitIds) {
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
    if (GameFlag_GetFar(FLAG_LINK_BATTLE) != 0) {
      lastId = LINK_PARTY_SIZE;
    }
    id = FIRST_ENEMY_UNIT;
    lastId += FIRST_ENEMY_UNIT;
    for (; id < lastId; id += 1) {
      if (Unit_GetFar(id)->side != BATTLE_SIDE_NONE) {
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
/// (BATTLE_SIDE_MASK_PARTY, BATTLE_SIDE_MASK_ENEMIES or both), party first.
/// unitIds, if not null, receives them ended by BATTLE_UNIT_LIST_END. Returns
/// how many there are. Called across the battle parts (the turn, staging, auto
/// battle, the turn's end and spoils) to test whether a side still stands and
/// to pick targets.
s32 BattleUnit_ListLiving(s32 sideMask, u16 *unitIds) {
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
  struct Unit *unit;

  output = unitIds;
  livingCount = 0;
  enemyCapacity = ENEMY_BATTLE_SIZE;
  if (GameFlag_GetFar(FLAG_LINK_BATTLE) != 0) {
    enemyCapacity = LINK_PARTY_SIZE;
  }
  // The party, in its battle order; then every enemy unit id that is
  // present, which a link battle limits to the opponent's three.
  if (sideMask & BATTLE_SIDE_MASK_PARTY) {
    base = activeMembers;
    activeCount = BattleParty_PrepareMembers(base);
    if (livingCount < activeCount) {
      member = base;
      remaining = activeCount;
      do {
        unitId = *member;
        member += 1;
        hp = Unit_GetFar(unitId)->hp;
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
  if (sideMask & BATTLE_SIDE_MASK_ENEMIES) {
    // remaining walks the enemy unit ids here.
    remaining = FIRST_ENEMY_UNIT;
    lastEnemy = enemyCapacity + FIRST_ENEMY_UNIT;
    if (remaining < lastEnemy) {
      do {
        unit = Unit_GetFar(remaining);
        if ((unit->side != BATTLE_SIDE_NONE) && (unit->hp > 0)) {
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
/// there are. Unlike BattleUnit_ListLiving, it does not test HP.
/// Called across the battle parts that draw or target units, and through
/// its far stub by the command windows (command_row.c, party_commands.c).
s32 BattleUnit_ListIds(s32 sideMask, u16 *unitIds) {
  struct BattleWork *battle;
  s32 count;
  s32 i;

  battle = gBattleWork;
  count = 0;
  if (sideMask & BATTLE_SIDE_MASK_PARTY) {
    for (i = 0; battle->partyUnits[i] != BATTLE_UNIT_LIST_END; i++) {
      if (battle->partyUnits[i] != BATTLE_UNIT_REMOVED) {
        if (unitIds != NULL)
          *unitIds++ = battle->partyUnits[i];
        count++;
      }
    }
  }
  if (sideMask & BATTLE_SIDE_MASK_ENEMIES) {
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
