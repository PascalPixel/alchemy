/// @file
/// A whole battle, Battle_Run, its tutorial helpers, and the lists of
/// the units in it.
// Part of battle.c.

#include "battle_internal.h"
#include "flags.h"
#include "fixed_math.h"
#include "sounds.h"
#include "field/flag_call.h"
#include "system/frame.h"
#include "gamepak/far_call/window.h"
#include "battle/battle.h"
#include "gamepak/far_call/battle_effect.h"
#include "gamepak/far_call/field_sequence.h"
#include "battle/presentation.h"
#include "system/system.h"
#include "battle/range_falloff.h"
#include "menu/djinn_menu.h"
#include "sound/sound_driver_symbols.h"

/// The six enemy units, which Unit_Get reads for unit ids 128 to 133 while the
/// battle lasts (heap slot HEAP_SLOT_ENEMY_UNITS).
#define ENEMY_UNITS_SIZE (ENEMY_BATTLE_SIZE * BATTLE_UNIT_SIZE)

/// The camera's zoom, in percent, from the battle's setup until the opening
/// sets its own: the curtain's a frame later, or the Doom Dragon's shattering
/// opening's, so no frame shows it.
#define BATTLE_ZOOM_BEFORE_OPENING 190

/// With the front line down and someone living, sends the reserve members in
/// to take the fallen ones' places, as many as there are of both.
void BattleParty_SwapInReserves(void) {
  u16 actors[6];
  u16 reserves[6];
  s32 actorCount;
  s32 reserveCount;
  s32 count;
  struct BattleWork *battle = Ram_HeapSlots->battleWork;
  s32 i;

  if (BattleUnit_ListLivingWithReserve(BATTLE_SIDE_MASK_PARTY, 0) == 0)
    return;
  count = actorCount = BattleUnit_ListIds(BATTLE_SIDE_MASK_PARTY, actors);
  reserveCount = BattleParty_PrepareReserveOwners(reserves);
  if (count > reserveCount)
    count = reserveCount;
  BattleHighlight_SetModes(actors, HIGHLIGHT_FADE_OUT);
  wait(32);
  for (i = 0; i < count; i++) {
    s32 unit = battle->partyUnits[i];
    s32 id = reserves[i];
    struct BattleObject *from = BattleObject_Get(unit);
    struct BattleObject *to = BattleObject_Get(id);
    struct MotionObject *object = to->object;
    struct MotionObject *place = from->object;

    to->anchorX = from->anchorX;
    to->anchorZ = from->anchorZ;
    Actor_SetPositionAndResetMotionFar((void *)object, place->x, place->y,
                                       place->z);
    gMainRoutineWork.party[i] = id;
    battle->partyUnits[i] = id;
  }
  for (i = 0; i < count; i++)
    gMainRoutineWork.party[actorCount + i] = actors[i];
  PartyStatus_DrawFar(Ram_HeapSlots->battleWork->partyStatus);
  BattleObject_Put();
  BattleHighlight_SetModes(reserves, HIGHLIGHT_FADE_IN);
  wait(32);
}

/// Allocates the battle camera and sets its starting view.
void BattleCamera_Create(void) {
  struct BattleCamera *camera =
      HeapSlot_AllocEwram(HEAP_SLOT_CAMERA, sizeof(struct BattleCamera));
  s32 *focus = camera->focus;

  SET_VECTOR(focus, 0, toFIXED(64), 0);
  SET_VECTOR(camera->eye, 0, toFIXED(180), toFIXED(64));
  camera->yaw = TRIG_TURN * 5 / 32;
  camera->pitch = TRIG_TURN * 5 / 16;
  camera->distance = toFIXED(256);
}

/// Name letters past every edition's last, which leave an enemy's name bare:
/// 255, and NAME_LETTER_ALPHABET, past Z, the last letter the Japanese edition
/// gives.
#define NAME_LETTER_NONE 255
#define NAME_LETTER_PAST_ALL NAME_LETTER_ALPHABET

/// Replaces the first enemy with the Doom Dragon's next form:
/// ENEMY_DOOM_DRAGON_E after FORMATION_DOOM_DRAGON_A, else ENEMY_DOOM_DRAGON_H.
/// The form change's battle effect calls it through its far call.
void Battle_ChangeEnemyForm(s32 id) {
  struct BattleWork *battle = Ram_HeapSlots->battleWork;
  s32 unit;
  struct BattleObject *battleObject;

  if (id == FORMATION_DOOM_DRAGON_A)
    unit = ENEMY_DOOM_DRAGON_E;
  else
    unit = ENEMY_DOOM_DRAGON_H;
  BattleObject_MarkForRelease(FIRST_ENEMY_UNIT);
  BattleMotion_DestroyAllSlotObjects();
  BattleEnemy_ClaimNameLetter(unit, 1);
  BattleUnit_SetFar(FIRST_ENEMY_UNIT, unit, NAME_LETTER_NONE);
  battle->enemyIds[0] = FIRST_ENEMY_UNIT;
  battle->enemyIds[1] = BATTLE_UNIT_LIST_END;
  BattlePlacement_RelayoutEnemies();
  battleObject = BattleObject_Get(FIRST_ENEMY_UNIT);
  BattleObject_Make(battleObject, FIRST_ENEMY_UNIT,
                    battleObject->anchorX / FIXED_ONE,
                    battleObject->anchorZ / FIXED_ONE);
  BattleObject_Put();
}

/// Plays the Doom Dragon's form change effect for its formation value
/// (FORMATION_DOOM_DRAGON_A to _C) on the first enemy.
void Battle_PlayFormChange(s32 value) {
  struct BattlePresentationWork request;

  request.initialValue = 0;
  request.actor = FIRST_ENEMY_UNIT;
  request.firstTarget = 0;
  request.count = 1;
  request.actors[0] = FIRST_ENEMY_UNIT;
  request.side = 0;
  request.kind = 0;
  request.variant = 0;
  BattleEffect_RunFormChangeFar((struct BattlePresentationWork *)&request,
                                value);
}

/// Returns the first of the Djinn tutorial's messages: the second set's
/// (MsgDjinnGiveTutorialB) while FLAG_ALREADY_KNOWS_DJINN is set, as Up held
/// when Echo's tutorial begins sets it.
s32 Tutorial_GetDjinnGiveMessage(void) {
  if (GameFlag_Get(FLAG_ALREADY_KNOWS_DJINN))
    return (s32)MsgDjinnGiveTutorialB;
  return (s32)MsgDjinnGiveTutorialA;
}

void ActorSystem_InitializeFar(s32 mode);
void Sound_LoadReverbCode(s32 preset);

/// Fights the battle against the enemy formation enemyGroup, from the opening
/// to the end. Returns the number of enemies defeated when the party won or the
/// link broke, BATTLE_OUTCOME_FLED or BATTLE_OUTCOME_LOST.
s32 Battle_Run(s32 enemyGroup) {
  struct BattleCamera *camera;
  struct BattleWork *battle;
  struct BattleBackgroundWork *background;
  s32 enemyCount;
  u8 *enemyUnitDestination;
  s32 turn;
  s32 actionCount;
  s32 i;
  s32 result;
  s32 window;
  s32 linkWaitFrames;
  u32 copyIndex;
  u32 actingUnit;
  s16 cue;

  camera = HeapSlot_AllocEwram(HEAP_SLOT_CAMERA, sizeof(struct BattleCamera));
  battle = HeapSlot_AllocEwram(HEAP_SLOT_BATTLE, sizeof(struct BattleWork));
  HeapSlot_AllocEwram(HEAP_SLOT_ENEMY_UNITS, ENEMY_UNITS_SIZE);
  background = HeapSlot_AllocEwram(HEAP_SLOT_BATTLE_BACKGROUND,
                                   sizeof(struct BattleBackgroundWork));
  HeapSlot_AllocEwram(HEAP_SLOT_BATTLE_UNUSED, 640);
  result = 0;
  Iwram_ClearWordsEntry(Ram_HeapSlots->enemyUnits, ENEMY_UNITS_SIZE);
  Task_ResetAll();
  background->targetYaw = YAW_PARTY_ACTION;
  background->frames = 0;
  background->flag = 1;
  background->unusedAfterFlag[0] = 0;
  background->unusedAfterFlag[1] = 0;
  REG_DISPCNT = DISPCNT_MODE_1;
  GameFlag_Set(FLAG_IN_BATTLE);
  // XXX (no evidence): what flag 0x169 marks; it is set as every battle
  // starts, as The Broken Seal's, and no code of either game reads it, so
  // nothing the game does shows what it means.
  GameFlag_Set(0x169);
  Matrix_Initialize();

  IWRAM_FILL_WORDS(camera, sizeof(struct BattleCamera), 0);
  Iwram_FillWordsEntry(battle, sizeof(struct BattleWork), 0);
  battle->arrowTiles = -1;
  battle->formation = enemyGroup;
  HeapSlot_AllocEwram(HEAP_SLOT_BATTLE_MESSAGE,
                      sizeof(struct BattleMessageWork));
  IWRAM_FILL_WORDS(Ram_HeapSlots->battleMessage,
                   sizeof(struct BattleMessageWork), 0);
  battle->background = BattleBackground_GetSelectedFar();
  HeapSlot_AllocEwram(HEAP_SLOT_SPRITES, 3584);
  HeapSlot_AllocEwram(HEAP_SLOT_SPRITE_ANIMATIONS, 1536);
  ActorSystem_InitializeFar(4);
  if (GameFlag_Get(FLAG_DJINN_LESSON) != 0)
    WindowSystem_InitBattleFar(1);
  else
    WindowSystem_InitBattleFar(0);

  BattleCamera_Create();
  enemyCount = BattleFormation_BuildEnemyList(battle->formation);

  if (GameFlag_Get(FLAG_LINK_BATTLE) != 0) {
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
    battle->linkSide = SIO_ID_OF(REG_SIOCNT);
    enemyUnitDestination = Ram_HeapSlots->enemyUnits;
    for (copyIndex = 0; copyIndex < ENEMY_UNITS_SIZE; copyIndex++)
      *enemyUnitDestination++ = gActorSpriteSlots[copyIndex];
    enemyCount = GameFlag_GetByte(FLAG_AREA_TEMP_F0);
    BattleParty_AssignOpponentSlots();
    battle->layout = FORMATION_LAYOUT_SHUFFLED;
  }
  Task_Add(BattleCamera_Update, TASK_PASS_FRAME - 1);

  cue = gMainRoutineWork.battleMusic;
  if (cue != 0) {
    Sound_PlayFar(cue);
    if (GameFlag_Get(FLAG_LINK_BATTLE) != 0) {
      Sound_PlayFar(MUS_TEST_YOUR_SKILLS);
      Sound_LoadReverbCode(SOUND_REVERB_NONE);
    }
  } else {
    Sound_PlayFar(MUS_BATTLE_THE_LOST_AGE);
  }

  BattleUnit_Collect();
  BattleUnit_RefreshPlacement();
  DjinnRecovery_Reconcile();
  BattleSummon_UpdateAvailability();
  if ((u16)Djinn_GetRecoveryTableFar(DJINN_RECOVERY_PARTY)->summonMask != 0)
    battle->partyStatus = PARTY_STATUS_SUMMONS;
  else
    battle->partyStatus = PARTY_STATUS_NO_SUMMONS;
  if (enemyGroup == FORMATION_KARST_AND_AGATIO_B ||
      enemyGroup == FORMATION_KARST_AND_AGATIO_C)
    ScriptedBattle_RestoreUnits();
  PartyStatus_CreateWindowFar(PARTY_STATUS_CLOSED);
  BattleCamera_SetDefaultView();
  BattleObject_Put();
  BattleBackground_Init();
  BattleBackground_Load(BATTLE_BACKGROUND_FIXED, battle->background, 0);
  BattleCamera_SetRange(toFIXED(160), toFIXED(80), 0, 0, FIXED_ONE * 2);
  BattleCamera_SetView(0, 0, 0, BATTLE_ZOOM_BEFORE_OPENING);
  Party_BattleSetupNoOp(1);
  REG_BLDCNT = 0;
  BattleSpoils_Clear();
  battle->arrowTiles = VramCache_AllocEntry(128);

  battle->firstStrike = FIRST_STRIKE_NONE;
  if (GameFlag_Get(FLAG_DJINN_LESSON) != 0) {
    battle->firstStrike = FIRST_STRIKE_PARTY;
  } else if (gMainRoutineWork.battleType == BATTLE_TYPE_NORMAL) {
    if ((Battle_RandomFar() & 15) == 0)
      battle->firstStrike = FIRST_STRIKE_PARTY;
    else if ((Battle_RandomFar() & 31) == 0)
      battle->firstStrike = FIRST_STRIKE_ENEMIES;
  } else if (gMainRoutineWork.battleType == BATTLE_TYPE_LOST) {
    battle->firstStrike = FIRST_STRIKE_ENEMIES;
  }
  BattleTransition_Run(enemyCount, enemyGroup);
  background->flag = 0;
  gCpuLoadDisplayEnabled = 0;
  Task_Add(BattleObject_UpdateSpritePriorities, TASK_PASS_FRAME);

  for (turn = 1;; turn++) {
    if (BattleUnit_ListLivingIds(BATTLE_SIDE_MASK_PARTY, 0) == 0)
      BattleParty_SwapInReserves();
    Battle_TurnStartNoOp();
    BattleSummon_UpdateAvailability();
    if ((u16)Djinn_GetRecoveryTableFar(DJINN_RECOVERY_PARTY)->summonMask != 0)
      battle->partyStatus = PARTY_STATUS_SUMMONS;
    else
      battle->partyStatus = PARTY_STATUS_NO_SUMMONS;
    background->targetYaw = TRIG_TURN * 5 / 32;
    background->frames = 60;
    PartyStatus_DrawFar(battle->partyStatus);

    Iwram_ClearWordsEntry(battle->actions, sizeof(battle->actions));
    VramCache_FreeEntry(battle->arrowTiles);
    if (GameFlag_Get(FLAG_AUTO_BATTLE) == 0)
      actionCount = Battle_GatherActions(battle->actions);
    else
      actionCount = AutoBattle_BuildActions(battle->actions);
    battle->arrowTiles = VramCache_AllocEntry(128);
    PartyStatus_DrawFar(battle->partyStatus);
    if (actionCount < 0)
      goto aborted;

    for (i = 0; i < actionCount; i++) {
      actingUnit = battle->actions[i].unitId;
      battle->counterUnitId = -1;
      battle->ceaseFire = 0;
      if (GameFlag_Get(FLAG_AUTO_BATTLE) == 0) {
        if (BattleStaging_DispatchAction(&battle->actions[i],
                                         i == 0 ? 10 : 0) == 1)
          goto fled;
      } else {
        if (AutoBattle_RunAction(&battle->actions[i]) == 1)
          goto fled;
      }
      if (enemyGroup == FORMATION_KARST_AND_AGATIO_VS_ISAAC)
        goto party_lost;
      if (BattleUnit_ListLivingWithReserve(BATTLE_SIDE_MASK_PARTY, 0) == 0)
        goto party_lost;
      if (BattleUnit_ListLiving(BATTLE_SIDE_MASK_PARTY, 0) == 0)
        break;
      if (BattleUnit_ListLiving(BATTLE_SIDE_MASK_ENEMIES, 0) == 0) {
        if (enemyGroup == FORMATION_DOOM_DRAGON_A ||
            enemyGroup == FORMATION_DOOM_DRAGON_B) {
          Battle_PlayFormChange(enemyGroup++);
          break;
        }
        if (enemyGroup == FORMATION_DOOM_DRAGON_C)
          Battle_PlayFormChange(FORMATION_DOOM_DRAGON_C);
        if (actingUnit <= ALLY_MAX && battle->spoils.defeated == 1) {
          battle->defeatState = DEFEAT_BY_PARTY_MEMBER;
        }
        goto won;
      }
      if (battle->counterUnitId != -1 &&
          Battle_FindUnitPlace(battle->counterTargetId) != -1) {
        battle->actions[i].unitId = battle->counterUnitId;
        battle->actions[i].command = COMMAND_DJINN_COUNTER;
        battle->actions[i].parameter = 0;
        battle->actions[i].target =
            Battle_FindUnitPlace(battle->counterTargetId);
        battle->actions[i].range = 1;
        i--;
      }
      if (LinkBattle_SyncTurn() < 0)
        goto aborted;
      if (i == actionCount - 1 && battle->daedalusStrikes != 0) {
        u32 queue = battle->daedalusFromEnemies;

        battle->daedalusFromEnemies = queue >> 1;
        queue &= 1;
        battle->daedalusStrikes--;
        // The strike is the summoner's side's: unit 0's or the first enemy's.
        battle->actions[i].unitId = queue << 7;
        battle->actions[i].command = COMMAND_DAEDALUS;
        battle->actions[i].parameter = 0;
        battle->actions[i].target = queue != 0 ? 0 : FIRST_ENEMY_UNIT;
        battle->actions[i].range = ABILITY_RANGE_ALL;
        i--;
      }
      if (battle->ceaseFire != 0)
        break;
    }

    BattleParty_ClearGuards();
    battle->daedalusFromEnemies = battle->nextDaedalusFromEnemies;
#if TLA_PATCH_1
    // Japanese leaves the next turn's queue in place.
    battle->nextDaedalusFromEnemies = 0;
#endif
    battle->daedalusStrikes = battle->nextDaedalusStrikes;
    battle->nextDaedalusStrikes = 0;
    battle->firstStrike = FIRST_STRIKE_NONE;
    Battle_TurnEndNoOp();
    Battle_ProcessTurnEnd();
    BattleMotion_DestroyAllSlotObjects();
    if ((enemyGroup == FORMATION_KARST_AND_AGATIO_A ||
         enemyGroup == FORMATION_KARST_AND_AGATIO_B) &&
        turn == 2)
      goto won;
    if (battle->isLink != 0) {
      if (LinkBattle_SyncTurn() < 0)
        goto aborted;
    } else {
      wait(20);
    }
    if (GameFlag_Get(FLAG_DJINN_LESSON) == 0)
      continue;

    // "See! What did I tell you? When you unleashed me, I used my special
    // attack ability!"
    window = Message_OpenWindowFar(Tutorial_GetDjinnGiveMessage() +
                                       DJINN_TUTORIAL_LINE_UNLEASHED,
                                   0, 4, MESSAGE_WINDOW_ZOOM);
    while (Message_AllTypedFar() == 0)
      wait(1);
    Window_CloseFar(window, 1);
    wait(1);
    // "After being unleashed, we Djinn will then stand by, ready to be
    // summoned back again..."
    window = Message_OpenWindowFar(Tutorial_GetDjinnGiveMessage() +
                                       DJINN_TUTORIAL_LINE_STANDBY,
                                   10, 4, MESSAGE_WINDOW_ZOOM);
    BattleMessage_WaitForAdvanceAt(44, 24);
    Window_CloseFar(window, 1);
    wait(1);
  }

won:
  Battle_ClearSerialTransferPayload();
  if ((enemyGroup != FORMATION_KARST_AND_AGATIO_A &&
       enemyGroup != FORMATION_KARST_AND_AGATIO_B)) {
    if (enemyGroup != FORMATION_DOOM_DRAGON_C &&
        GameFlag_Get(FLAG_DJINN_LESSON) == 0) {
      if (battle->isLink != 0)
        Sound_PlayFar(MUS_VICTORY);
      if (battle->spoils.defeated != 0) {
        Sound_PlayFar(MUS_VICTORY);
        if (battle->defeatState <= DEFEAT_SEVERAL) {
          // The first defeated enemy's name for the message, with no letter.
          BattleUnit_SetFar(FIRST_ENEMY_UNIT,
                            battle->enemyKinds[battle->firstDefeated],
                            NAME_LETTER_PAST_ALL);
          MessageArgument_ClearAllFar();
          MessageArgument_PushFar(FIRST_ENEMY_UNIT, MESSAGE_ARGUMENT_ACTOR);
          BattleMessage_ShowAndWaitFar(battle->defeatState + (s32)MsgDowned);
          BattleMessage_WaitForAdvance();
        }
      }
      Battle_AwardSpoils();
    }
    Sound_PlayFar(SOUND_FADE_OUT_BGM);
  }
  ScreenFade_ToBlack(30);
  ScreenFade_Wait();
  result = battle->spoils.defeated;
  goto finished;

aborted:
  Battle_ClearSerialTransferPayload();
  Task_SuspendAll(0);
  REG_DISPCNT = DISPCNT_MODE_1;
  result = battle->spoils.defeated;
  GameFlag_Set(FLAG_AREA_TEMP_E8);
  goto finished;

party_lost:
  Battle_ClearSerialTransferPayload();
  if ((enemyGroup != FORMATION_KARST_AND_AGATIO_A &&
       enemyGroup != FORMATION_KARST_AND_AGATIO_B) &&
      enemyGroup != FORMATION_KARST_AND_AGATIO_VS_ISAAC) {
    if (enemyGroup != FORMATION_KARST_AND_AGATIO_C) {
      Sound_PlayFar(MUS_DEFEATED);
      MessageArgument_ClearAllFar();
      if (BattleParty_PrepareMembers(0) == 1)
        BattleMessage_ShowAndWaitFar((s32)MsgGoesDownInDefeat);
      else
        BattleMessage_ShowAndWaitFar((s32)MsgPartyDefeated);
      BattleMessage_WaitForAdvance();
    }
    Sound_PlayFar(SOUND_FADE_OUT_BGM);
  }
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
  if ((enemyGroup == FORMATION_KARST_AND_AGATIO_A ||
       enemyGroup == FORMATION_KARST_AND_AGATIO_B))
    ScriptedBattle_SaveUnits();
  BattleParty_ResetActiveRuntimeFields();
  Battle_TurnEndNoOp();
  DjinnRecovery_CountDown();
  Battle_SetType(BATTLE_TYPE_NORMAL);
  Task_Remove(BattleObject_UpdateSpritePriorities);
  BattleBackground_Free();
  return result;
}

/// Lists the party members who fight, three in a link battle, into owners when
/// it is not null, and returns how many.
s32 BattleParty_PrepareMembers(u16 *owners) {
  s32 limit;
  s32 count;
  s32 index;

  limit = PARTY_BATTLE_SIZE;
  if (Ram_HeapSlots->battleWork->isLink != 0)
    limit = LINK_PARTY_SIZE;

  count = Party_CountMembersFar();
  if (count > limit)
    count = limit;

  for (index = 0; index < count; index++) {
    s32 owner = gMainRoutineWork.party[index];

    if (owners != 0)
      *owners++ = owner;
    Unit_Get(owner)->side = BATTLE_SIDE_PARTY;
  }

  if (owners != 0)
    *owners = BATTLE_UNIT_LIST_END;
  return count;
}

/// Lists the party's reserve members, those past the fourth (none in a link
/// battle), into owners when it is not null, puts each on the party's side and
/// returns how many there are.
s32 BattleParty_PrepareReserveOwners(u16 *owners) {
  s32 count;
  s32 index;

  count = Party_CountMembersFar();
  if (count > PARTY_BATTLE_SIZE)
    count -= PARTY_BATTLE_SIZE;
  else
    count = 0;
  if (Ram_HeapSlots->battleWork->isLink != 0)
    count = 0;

  for (index = 0; index < count; index++) {
    s32 owner = gMainRoutineWork.party[index + PARTY_BATTLE_SIZE];

    if (owners != 0)
      *owners++ = owner;
    Unit_Get(owner)->side = BATTLE_SIDE_PARTY;
  }

  if (owners != 0)
    *owners = BATTLE_UNIT_LIST_END;
  return count;
}

/// Lists the enemy units present and returns how many, 0 when unitIds is null.
s32 BattleEnemy_List(s16 *unitIds) {
  s16 *output;
  s32 count;
  s32 id;
  s32 limit;
  s32 listed;

  output = unitIds;
  listed = 0;
  limit = ENEMY_BATTLE_SIZE;
  count = 0;
  if (output != NULL) {
    if (GameFlag_Get(FLAG_LINK_BATTLE) != 0)
      limit = LINK_PARTY_SIZE;
    id = FIRST_ENEMY_UNIT;
    limit += FIRST_ENEMY_UNIT;
    for (; id < limit; id++) {
      if (Unit_Get(id)->side != BATTLE_SIDE_NONE) {
        *output = id;
        listed++;
        output++;
      }
    }
    count = listed;
    *output = BATTLE_UNIT_LIST_END;
  }
  return count;
}

/// Lists the units still standing on the sides in sideMask, party first, into
/// unitIds when it is not null, and returns how many.
s32 BattleUnit_ListLiving(s32 sideMask, u16 *unitIds) {
  u16 activeMembers[8];
  s32 remaining;
  s32 enemyLimit;
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
  if (GameFlag_Get(FLAG_LINK_BATTLE) != 0) {
    enemyCapacity = LINK_PARTY_SIZE;
  }
  if (sideMask & BATTLE_SIDE_MASK_PARTY) {
    activeCount = BattleParty_PrepareMembers(activeMembers);
    if (livingCount < activeCount) {
      member = activeMembers;
      remaining = activeCount;
      do {
        unitId = *member;
        member += 1;
        hp = Unit_Get(unitId)->hp;
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
    enemyLimit = enemyCapacity + FIRST_ENEMY_UNIT;
    if (remaining < enemyLimit) {
      for (; remaining < enemyLimit; remaining += 1) {
        unit = Unit_Get(remaining);
        if (unit->side != BATTLE_SIDE_NONE && unit->hp > 0) {
          if (output != NULL) {
            *output = remaining;
            output += 1;
          }
          livingCount += 1;
        }
      }
    }
  }
  if (output != NULL) {
    *output = BATTLE_UNIT_LIST_END;
  }
  return livingCount;
}

/// As BattleUnit_ListLiving, with the reserve owners listed after the
/// active ones.
s32 BattleUnit_ListLivingWithReserve(s32 sideMask, u16 *unitIds) {
  u16 members[10];
  s32 remaining;
  s32 enemyLimit;
  u16 *output;
  s32 livingCount;
  s32 enemyCapacity;
  u16 *member;
  s32 unitId;
  s16 hp;
  s32 count;
  struct Unit *unit;

  output = unitIds;
  livingCount = 0;
  enemyCapacity = ENEMY_BATTLE_SIZE;
  if (GameFlag_Get(FLAG_LINK_BATTLE) != 0) {
    enemyCapacity = LINK_PARTY_SIZE;
  }
  if (sideMask & BATTLE_SIDE_MASK_PARTY) {
    count = BattleParty_PrepareMembers(members);
    count += BattleParty_PrepareReserveOwners(members + count);
    if (livingCount < count) {
      member = members;
      remaining = count;
      do {
        unitId = *member;
        member += 1;
        hp = Unit_Get(unitId)->hp;
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
    enemyLimit = enemyCapacity + FIRST_ENEMY_UNIT;
    if (remaining < enemyLimit) {
      for (; remaining < enemyLimit; remaining += 1) {
        unit = Unit_Get(remaining);
        if (unit->side != BATTLE_SIDE_NONE && unit->hp > 0) {
          if (output != NULL) {
            *output = remaining;
            output += 1;
          }
          livingCount += 1;
        }
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
  struct BattleWork *battle;
  s32 count;
  s32 i;

  battle = Ram_HeapSlots->battleWork;
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

/// As BattleUnit_ListIds, listing only the units whose HP is not 0.
s32 BattleUnit_ListLivingIds(s32 sideMask, u16 *unitIds) {
  struct BattleWork *battle;
  s32 count;
  s32 i;

  battle = Ram_HeapSlots->battleWork;
  count = 0;
  if (sideMask & BATTLE_SIDE_MASK_PARTY) {
    for (i = 0; battle->partyUnits[i] != BATTLE_UNIT_LIST_END; i++) {
      if (battle->partyUnits[i] != BATTLE_UNIT_REMOVED &&
          Unit_Get(battle->partyUnits[i])->hp != 0) {
        if (unitIds != NULL)
          *unitIds++ = battle->partyUnits[i];
        count++;
      }
    }
  }
  if (sideMask & BATTLE_SIDE_MASK_ENEMIES) {
    for (i = 0; battle->enemyIds[i] != BATTLE_UNIT_LIST_END; i++) {
      if (battle->enemyIds[i] != BATTLE_UNIT_REMOVED &&
          Unit_Get(battle->enemyIds[i])->hp != 0) {
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
