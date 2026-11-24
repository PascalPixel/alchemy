/// @file
/// A whole battle, Battle_Run, and the lists of the units in it.
// Part of battle.c.

#include "battle_internal.h"

/// A name letter past every edition's last (Z, the Japanese edition's 26th),
/// which leaves an enemy's name bare, as The Lost Age names it.
#define NAME_LETTER_PAST_ALL 26

/// The six enemy units, gEnemyUnits, which Unit_Get reads for unit ids 128 to
/// 133 while the battle lasts.
#define ENEMY_UNITS_SIZE (ENEMY_BATTLE_SIZE * BATTLE_UNIT_SIZE)

/// The camera's zoom, in percent, from the battle's setup until the opening
/// sets its own: the curtain's a frame later, or the Fusion Dragon's
/// shattering opening's, so no frame shows it.
#define BATTLE_ZOOM_BEFORE_OPENING 190

/// The camera's yaw as a battle starts, and as each turn's commands are
/// chosen: about 56 degrees, a little past YAW_PARTY_ACTION.
#define YAW_TURN_START (TRIG_TURN * 5 / 32)

/// Fights the battle against the enemy formation enemyGroup, from the opening
/// to the end. Returns the number of enemies defeated when the party won or the
/// link broke, BATTLE_OUTCOME_FLED or BATTLE_OUTCOME_LOST.
s32 Battle_Run(s32 enemyGroup) {
  struct BattleCamera *camera;
  s32 *focus;
  struct BattleState *battle;
  u8 *enemyUnits;
  struct BattleBackgroundState *view;
  struct BattleMessageState *display;
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
  /* LEFTOVER(frame): an unused local the ROM's frame reserves and never
   * touches. */
  UNUSED u8 unused[48];
  volatile u32 fill;

  // The battle's state, all in EWRAM.
  camera = HeapSlot_AllocEwram(HEAP_SLOT_CAMERA, sizeof(struct BattleCamera));
  battle = HeapSlot_AllocEwram(HEAP_SLOT_BATTLE, sizeof(struct BattleState));
  enemyUnits = HeapSlot_AllocEwram(HEAP_SLOT_ENEMY_UNITS, ENEMY_UNITS_SIZE);
  view = HeapSlot_AllocEwram(HEAP_SLOT_BATTLE_BACKGROUND,
                             sizeof(struct BattleBackgroundState));
  HeapSlot_AllocEwram(HEAP_SLOT_BATTLE_UNUSED, 640);
  focus = camera->focus;
  result = 0;
  Iwram_ClearWords(enemyUnits, ENEMY_UNITS_SIZE);
  Task_ResetAll();
  // The opening transition owns the background until it is over.
  view->frames = 0;
  view->targetYaw = YAW_PARTY_ACTION;
  view->keepRange = 1;
  view->unusedAfterKeepRange[0] = 0;
  view->unusedAfterKeepRange[1] = 0;
  REG_DISPCNT = DISPCNT_MODE_1;
  GameFlag_SetFar(FLAG_IN_BATTLE);
  // XXX (no evidence): what flag 0x169 marks; it is set as every battle
  // starts, as The Lost Age's is, and no code of either game reads or clears
  // it, so nothing the game does shows what it means.
  GameFlag_SetFar(0x169);
  Matrix_Initialize();

  Dma_Fill32Cell(fill, 0, camera, sizeof(struct BattleCamera));
  Dma_Fill32Cell(fill, 0, battle, sizeof(struct BattleState));
  battle->arrowTiles = -1;
  battle->formation = enemyGroup;
  display = HeapSlot_AllocEwram(HEAP_SLOT_BATTLE_MESSAGE,
                                sizeof(struct BattleMessageState));
  Dma_Fill32Cell(fill, 0, display, sizeof(struct BattleMessageState));
  battle->background = BattleBackground_GetSelectedFar();
  HeapSlot_AllocEwram(HEAP_SLOT_SPRITES, 3584);
  HeapSlot_AllocEwram(HEAP_SLOT_SPRITE_ANIMATIONS, 1536);
  ActorSystem_InitializeFar(ACTOR_SYSTEM_BATTLE);
  if (GameFlag_GetFar(FLAG_DJINN_LESSON) != 0)
    WindowSystem_InitBattleFar(1);
  else
    WindowSystem_InitBattleFar(0);

  // The camera's starting view, then the enemies.
  SET_VECTOR(focus, 0, toFIXED(64), 0);
  SET_VECTOR(camera->eye, 0, toFIXED(180), toFIXED(64));
  camera->yaw = YAW_TURN_START;
  camera->pitch = TRIG_TURN * 5 / 16;
  camera->distance = toFIXED(256);
  enemyCount = BattleFormation_BuildEnemyList(battle->formation);

  // A link battle: the opponent's party, which the Battle Arena left in
  // gActorSpriteSlots, becomes the enemies, and both consoles start from seed 0
  // so that their rolls agree.
  if (GameFlag_GetFar(FLAG_LINK_BATTLE) != 0) {
    battle->isLink = 1;
    Battle_SetType(BATTLE_TYPE_LINK);
  }
  if (battle->isLink != 0) {
    Battle_SetRandomSeed(0);
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

  // The battle music: the music an event or encounter set for this battle,
  // and in a link battle Test Your Skills after it; with none set, two other
  // pieces in turn.
  // XXX: which piece is heard when two are started one after the other; a
  // recording of a link battle and of a battle with no music of its own would
  // settle it.
  cue = gMainRoutineState.battleMusic;
  if (cue != 0) {
    Sound_PlayFar(cue);
    if (GameFlag_GetFar(FLAG_LINK_BATTLE) != 0) {
      Sound_PlayFar(MUS_TEST_YOUR_SKILLS);
      Sound_LoadReverbCode(SOUND_REVERB_NONE);
    }
  } else {
    Sound_PlayFar(MUS_SWORN_ENEMIES_STAND_IN_YOUR_WAY);
    Sound_PlayFar(MUS_COLOSSEUM_FINALS);
  }

  // The units, the windows and the background.
  BattleUnit_Collect();
  BattleUnit_RefreshPlacement();
  DjinnRecovery_Reconcile();
  BattleSummon_UpdateAvailability();
  if (Djinn_GetRecoveryTableFar(DJINN_RECOVERY_PARTY)->summonMask != 0)
    battle->partyStatus = PARTY_STATUS_SUMMONS;
  else
    battle->partyStatus = PARTY_STATUS_NO_SUMMONS;
  PartyStatus_CreateWindowFar(PARTY_STATUS_CLOSED);
  BattleCamera_SetDefaultView();
  BattleObject_Put();
  BattleBackground_Init();
  BattleBackground_Load(BATTLE_BACKGROUND_FIXED, battle->background, 0);
  BattleCamera_SetRange(toFIXED(160), toFIXED(80), 0, 0, 2 * FIXED_ONE);
  BattleCamera_SetView(0, 0, 0, BATTLE_ZOOM_BEFORE_OPENING);
  Party_BattleSetupNoOp(1);
  REG_BLDCNT = 0;
  BattleSpoils_Clear();
  // The advance arrow's tiles, taken again each turn.
  battle->arrowTiles = VramCache_AllocEntry(128);

  // A random encounter may open with a first strike (1 in 16) or a
  // surprise (1 in 32 of the rest); the Djinn tutorial always strikes first.
  battle->firstStrike = FIRST_STRIKE_NONE;
  if (GameFlag_GetFar(FLAG_DJINN_LESSON) != 0) {
    battle->firstStrike = FIRST_STRIKE_PARTY;
  } else if (gMainRoutineState.battleType == BATTLE_TYPE_NORMAL) {
    if ((Battle_RandomFar() & 15) == 0)
      battle->firstStrike = FIRST_STRIKE_PARTY;
    else if ((Battle_RandomFar() & 31) == 0)
      battle->firstStrike = FIRST_STRIKE_ENEMIES;
  }
  // After the entry transition the camera task owns the background
  // again, and the sprite priorities are sorted every frame.
  BattleTransition_Run(enemyCount, enemyGroup);
  view->keepRange = 0;
  gCpuLoadDisplayEnabled = 0;
  Task_Add(BattleObject_UpdateSpritePriorities, TASK_PASS_FRAME);

  // The turns.
  for (;;) {
    Battle_TurnStartNoOp();
    BattleSummon_UpdateAvailability();
    if (Djinn_GetRecoveryTableFar(DJINN_RECOVERY_PARTY)->summonMask != 0)
      battle->partyStatus = PARTY_STATUS_SUMMONS;
    else
      battle->partyStatus = PARTY_STATUS_NO_SUMMONS;
    // The camera eases back to its starting yaw over 60 frames, and the
    // party status window is drawn with or without its Djinn column.
    view->targetYaw = YAW_TURN_START;
    view->frames = 60;
    PartyStatus_DrawFar(battle->partyStatus);

    // Every unit's action for the turn, in action order.
    Iwram_ClearWords(battle->actions, sizeof(battle->actions));
    VramCache_FreeEntry(battle->arrowTiles);
    if (GameFlag_GetFar(FLAG_AUTO_BATTLE) == 0) {
      // DEAD: the heap's free IWRAM and EWRAM measured and dropped, before and
      // after the commands are chosen and around each action; the ROM keeps
      // the calls.
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
        if (actingUnit <= ALLY_MAX && battle->spoils.defeated == 1)
          battle->defeatState = DEFEAT_BY_PARTY_MEMBER;
        goto won;
      }
      if (LinkBattle_SyncTurn() < 0)
        goto aborted;
    }

    // The end of the turn.
    battle->firstStrike = FIRST_STRIKE_NONE;
    Battle_TurnEndNoOp();
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

    // The Djinn lesson's two lines after each turn.
    // "See! When you unleashed me, I attacked with my mighty stone-cleaver!"
    tutorialWindow =
        Message_OpenWindowFar(MsgTutorialUnleashed, 0, 4, MESSAGE_WINDOW_ZOOM);
    while (Message_AllTypedFar() == 0)
      wait(1);
    Window_CloseFar(tutorialWindow, 1);
    wait(1);
    // "After being unleashed, we Djinn will then stand by to be summoned..."
    tutorialWindow =
        Message_OpenWindowFar(MsgTutorialStandby, 10, 4, MESSAGE_WINDOW_ZOOM);
    BattleMessage_WaitForAdvanceAt(92, 24);
    Window_CloseFar(tutorialWindow, 1);
    wait(1);
  }

won:
  // "X has been downed!" or "X's party goes down!", named after the first
  // enemy to fall, with no letter, then the spoils.
  Battle_ClearSerialTransferPayload();
  if (GameFlag_GetFar(FLAG_DJINN_LESSON) == 0) {
    if (battle->isLink != 0)
      Sound_PlayFar(MUS_VICTORY);
    if (battle->spoils.defeated != 0) {
      Sound_PlayFar(MUS_VICTORY);
      if (battle->defeatState <= DEFEAT_SEVERAL) {
        BattleUnit_SetFar(FIRST_ENEMY_UNIT,
                          battle->enemyKinds[battle->firstDefeated],
                          NAME_LETTER_PAST_ALL);
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
  // The link broke, or no actions could be gathered.
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
  Sound_PlayFar(MUS_DEFEATED);
  MessageArgument_ClearAllFar();
  MessageArgument_PushFar(gMainRoutineState.party[0], MESSAGE_ARGUMENT_ACTOR);
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
  // Every way out: Djinn recover one step and the next battle starts normally.
  BattleParty_ClearBattleStatuses();
  Battle_TurnEndNoOp();
  DjinnRecovery_CountDown();
  Battle_SetType(BATTLE_TYPE_NORMAL);
  Task_Remove(BattleObject_UpdateSpritePriorities);
  BattleBackground_Free();
  return result;
}

/// Lists the party members who fight, three in a link battle, into unitIds when
/// it is not null, and returns how many.
s32 BattleParty_PrepareMembers(u16 *unitIds) {
  s32 limit;
  s32 count;
  s32 index;

  limit = PARTY_BATTLE_SIZE;
  if (gBattleState->isLink != 0)
    limit = LINK_PARTY_SIZE;

  count = Party_CountMembersFar();
  if (count > limit)
    count = limit;

  for (index = 0; index < count; index++) {
    s32 unitId = gMainRoutineState.party[index];

    if (unitIds != 0)
      *unitIds++ = unitId;
    Unit_GetFar(unitId)->side = BATTLE_SIDE_PARTY;
  }

  if (unitIds != 0)
    *unitIds = BATTLE_UNIT_LIST_END;
  return count;
}

/// Lists the enemy units present and returns how many, 0 when unitIds is null.
s32 BattleEnemy_List(s16 *unitIds) {
  s16 *output = unitIds;
  s32 listed = 0;
  s32 enemyCapacity = ENEMY_BATTLE_SIZE;
  s32 count = 0;
  s32 unitId;

  if (output != NULL) {
    if (GameFlag_GetFar(FLAG_LINK_BATTLE) != 0)
      enemyCapacity = LINK_PARTY_SIZE;
    for (unitId = FIRST_ENEMY_UNIT; unitId < FIRST_ENEMY_UNIT + enemyCapacity;
         unitId++) {
      if (Unit_GetFar(unitId)->side != BATTLE_SIDE_NONE) {
        *output++ = unitId;
        listed++;
      }
    }
    *output = BATTLE_UNIT_LIST_END;
    count = listed;
  }
  return count;
}

/// Lists the units still standing on the sides in sideMask, party first, into
/// unitIds when it is not null, and returns how many.
s32 BattleUnit_ListLiving(s32 sideMask, u16 *unitIds) {
  u16 activeMembers[8];
  s32 i;
  u16 *output;
  s32 livingCount;
  s32 enemyCapacity;
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
  if (sideMask & BATTLE_SIDE_MASK_PARTY) {
    activeCount = BattleParty_PrepareMembers(activeMembers);
    for (i = 0; i < activeCount; i++) {
      unitId = activeMembers[i];
      hp = Unit_GetFar(unitId)->hp;
      if (hp > 0) {
        if (output != NULL) {
          *output = unitId;
          output++;
        }
        livingCount++;
      }
    }
  }
  if (sideMask & BATTLE_SIDE_MASK_ENEMIES) {
    for (i = FIRST_ENEMY_UNIT; i < enemyCapacity + FIRST_ENEMY_UNIT; i++) {
      unit = Unit_GetFar(i);
      if ((unit->side != BATTLE_SIDE_NONE) && (unit->hp > 0)) {
        if (output != NULL) {
          *output = i;
          output++;
        }
        livingCount++;
      }
    }
  }
  if (output != NULL) {
    *output = BATTLE_UNIT_LIST_END;
  }
  return livingCount;
}

/// Lists the units in the battle on the sides in sideMask, standing or not, and
/// returns how many.
s32 BattleUnit_ListIds(s32 sideMask, u16 *unitIds) {
  struct BattleState *battle;
  s32 count;
  s32 i;

  battle = gBattleState;
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
