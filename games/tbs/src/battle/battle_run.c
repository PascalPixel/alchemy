/// @file
/// A whole battle, Battle_Run, and the lists of the units in it.
// Part of battle.c.

#include "battle_internal.h"

/// Battle_Run's results besides the number of enemies defeated.
#define BATTLE_OUTCOME_LOST -1
#define BATTLE_OUTCOME_FLED 999

/// The six enemy units, gEnemyUnits, which Unit_Get reads for unit ids 128 to
/// 133 while the battle lasts.
#define ENEMY_UNITS_SIZE (6 * BATTLE_UNIT_SIZE)

/// Fights the battle against the enemy formation enemyGroup, from the opening
/// to the end. Returns the number of enemies defeated when the party won or the
/// link broke, BATTLE_OUTCOME_FLED or BATTLE_OUTCOME_LOST.
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
  /* LEFTOVER(frame): an unused local the ROM's frame reserves and never
   * touches: of the 68 bytes it reserves (sub sp, #68), no instruction
   * addresses the 48 at sp+20 to sp+67; as without it, 4 of 645 instructions
   * differ in tbs-en. */
  UNUSED u8 unused[48];
  volatile u32 fill;

  // The battle's work areas, all in EWRAM.
  camera = HeapSlot_AllocEwram(HEAP_SLOT_CAMERA, sizeof(struct BattleCamera));
  battle = HeapSlot_AllocEwram(HEAP_SLOT_BATTLE, sizeof(struct BattleWork));
  enemyUnits = HeapSlot_AllocEwram(HEAP_SLOT_ENEMY_UNITS, ENEMY_UNITS_SIZE);
  view = HeapSlot_AllocEwram(HEAP_SLOT_BATTLE_BACKGROUND,
                             sizeof(struct BattleBackgroundWork));
  HeapSlot_AllocEwram(HEAP_SLOT_BATTLE_UNUSED, 640);
  focus = camera->focus;
  result = 0;
  Iwram_ClearWords(enemyUnits, ENEMY_UNITS_SIZE);
  Task_ResetAll();
  // The opening transition owns the background until it is over.
  view->frames = 0;
  view->targetYaw = YAW_PARTY_ACTION;
  view->flag = 1;
  view->unusedAfterFlag[0] = 0;
  view->unusedAfterFlag[1] = 0;
  REG_DISPCNT = DISPCNT_MODE_1;
  GameFlag_SetFar(FLAG_IN_BATTLE);
  // XXX (no evidence): what flag 0x169 marks; it is set as every battle starts
  // and nothing reads it.
  GameFlag_SetFar(0x169);
  Transform_Reset();

  Dma_Fill32Cell(fill, 0, camera, sizeof(struct BattleCamera));
  Dma_Fill32Cell(fill, 0, battle, sizeof(struct BattleWork));
  battle->arrowTiles = -1;
  battle->formation = enemyGroup;
  display = HeapSlot_AllocEwram(HEAP_SLOT_BATTLE_MESSAGE,
                                sizeof(struct BattleMessageWork));
  Dma_Fill32Cell(fill, 0, display, sizeof(struct BattleMessageWork));
  battle->background = BattleBackground_GetSelectedFar();
  HeapSlot_AllocEwram(HEAP_SLOT_SPRITES, 3584);
  HeapSlot_AllocEwram(HEAP_SLOT_SPRITE_ANIMATIONS, 1536);
  ActorSystem_InitializeFar(ACTOR_SYSTEM_BATTLE);
  if (GameFlag_GetFar(FLAG_DJINN_LESSON) != 0)
    WindowSystem_InitBattleFar(1);
  else
    WindowSystem_InitBattleFar(0);

  // The camera's starting view, then the enemies.
  SET_VECTOR(focus, 0, px(64), 0);
  SET_VECTOR(camera->eye, 0, px(180), px(64));
  camera->yaw = TRIG_TURN * 5 / 32;
  camera->pitch = TRIG_TURN * 5 / 16;
  camera->distance = 256 << 16;
  enemyCount = BattleFormation_BuildEnemyList(battle->formation);

  // A link battle: the opponent's party, which the Battle Arena left in
  // gActorSpriteSlots, becomes the enemies, and both consoles start from seed 0
  // so that their rolls agree.
  if (GameFlag_GetFar(FLAG_LINK_BATTLE) != 0) {
    battle->isLink = 1;
    Battle_SetType(BATTLE_TYPE_LINK);
  }
  if (battle->isLink != 0) {
    /* FAKEMATCH(dead): the seed's store in a once-only block; as the plain
     * store, 4 of 645 instructions differ in tbs-en. */
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
  cue = gMainRoutineWork.battleMusic;
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
  PartyStatus_CreateWindowFar(PARTY_STATUS_CLOSED);
  BattleCamera_SetDefaultView();
  BattleObject_Put();
  BattleBackground_Init();
  BattleBackground_Load(BATTLE_BACKGROUND_FIXED, battle->background, 0);
  // The camera on the centre at 190 percent until the opening zooms.
  BattleCamera_SetRange(px(160), px(80), 0, 0, 2 << 16);
  BattleCamera_SetView(0, 0, 0, 190);
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
  } else if (gMainRoutineWork.battleType == BATTLE_TYPE_NORMAL) {
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
    view->targetYaw = TRIG_TURN * 5 / 32;
    view->frames = 60;
    PartyStatus_DrawFar(battle->partyStatus);

    // Every unit's action for the turn, in action order.
    Iwram_ClearWords(battle->actions, sizeof(battle->actions));
    VramCache_FreeEntry(battle->arrowTiles);
    if (GameFlag_GetFar(FLAG_AUTO_BATTLE) == 0) {
      // XXX (no evidence): who read the free memory around the command menu.
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

    // The Djinn tutorial explains unleashing, then standby.
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
  // "X has been downed!" or "X's party goes down!", named after the first
  // enemy to fall, then the spoils. Letter 26 gives the name with no letter.
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
  Sound_PlayFar(MUS_DEFEAT);
  MessageArgument_ClearAllFar();
  MessageArgument_PushFar(gMainRoutineWork.party[0], MESSAGE_ARGUMENT_ACTOR);
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
  BattleParty_ResetActiveRuntimeFields();
  Battle_ReservedNoOpF674();
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
  if (gBattleWork->isLink != 0)
    limit = LINK_PARTY_SIZE;

  count = Party_CountMembersFar();
  if (count > limit)
    count = limit;

  for (index = 0; index < count; index++) {
    s32 unitId = gMainRoutineWork.party[index];

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

/// Lists the units in the battle on the sides in sideMask, standing or not, and
/// returns how many.
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
