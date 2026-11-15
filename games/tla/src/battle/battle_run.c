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
#include "system/system_bank.h"
#include "battle/range_falloff.h"

/// With the front line down and someone living, sends the reserve members in
/// to take the fallen ones' places, as many as there are of both.
void BattleParty_SwapInReserves(void) {
  u16 actors[6];
  u16 reserves[6];
  s32 actorCount;
  s32 reserveCount;
  s32 count;
  struct BattleWork *work = Ram_HeapSlots->battleWork;
  s32 i;

  if (BattleUnit_ListLivingWithReserve(1, 0) == 0)
    return;
  count = actorCount = BattleUnit_ListIds(1, actors);
  reserveCount = BattleParty_PrepareReserveOwners(reserves);
  if (count > reserveCount)
    count = reserveCount;
  BattleHighlight_SetModes(actors, HIGHLIGHT_FADE_OUT);
  wait(32);
  for (i = 0; i < count; i++) {
    s32 unit = work->partyUnits[i];
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
    work->partyUnits[i] = id;
  }
  for (i = 0; i < count; i++)
    gMainRoutineWork.party[actorCount + i] = actors[i];
  PartyStatus_DrawFar(Ram_HeapSlots->battleWork->partyStatus);
  BattleObject_Put();
  BattleHighlight_SetModes(reserves, HIGHLIGHT_FADE_IN);
  wait(32);
}

/// Allocates the battle camera (heap slot 48) and sets its starting view.
void BattleCamera_Create(void) {
  struct BattleCamera *camera =
      HeapSlot_AllocEwram(HEAP_SLOT_CAMERA, sizeof(struct BattleCamera));
  s32 *focus = camera->focus;

  SET_VECTOR(focus, 0, px(64), 0);
  SET_VECTOR(camera->eye, 0, px(180), px(64));
  camera->yaw = TRIG_TURN * 5 / 32;
  camera->pitch = TRIG_TURN * 5 / 16;
  camera->distance = 256 << 16;
}

/// Replaces enemy 128 with the next form of the three-part battle of enemy
/// groups 78 to 80: unit 373 after group 78, else 376. The form change's
/// battle effect calls it through its far call.
void Battle_ChangeEnemyForm(s32 id) {
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
  work->enemyIds[1] = BATTLE_UNIT_LIST_END;
  Enemy_Update();
  object = (s32 *)BattleObject_Get(128);
  BattleObject_Make((struct BattleObject *)object, 128, object[3] / Q16_ONE,
                    object[4] / Q16_ONE);
  BattleObject_Put();
}

/// Plays the form change effect of enemy group value (78 to 80) on the first
/// enemy.
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
  BattleEffect_RunFormChangeFar((struct BattleFxRequest *)&request, value);
}

/// Returns the first of the Djinn tutorial's messages: the second set's
/// (MsgDjinnGiveTutorialB) while flag 0x175 is set, as Up held when Echo's
/// tutorial begins sets it.
s32 Tutorial_GetDjinnGiveMessage(void) {
  if (GameFlag_Get(0x175))
    return (s32)MsgDjinnGiveTutorialB;
  return (s32)MsgDjinnGiveTutorialA;
}

/* VIEW(battle): tag struct EncounterWork as struct EncounterWork__battle;
 * scene/shared.h has another definition; tried with the definition the rest of
 * the object has, it does not compile in tla-ja:
 * games/tla/include/scene/shared.h:98: redefinition of `struct EncounterWork'.
 */
/// The battle work as Battle_Run reads it.
#define EncounterWork EncounterWork__battle
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
/// The actor sprite slot buffers, 0x4000 bytes a slot.
/* VIEW(battle): variable Data_02018000 as u8 Data_02018000[]; scene/shared.h
 * has u32 Data_02018000[]; tried as the rest of the object declares it, it does
 * not compile in tla-ja: games/tla/src/battle/battle_run.c:255: `Data_02018000'
 * undeclared (first use in this function). */
extern u8 Data_02018000__battle[] __asm__("Data_02018000");
#define Data_02018000 Data_02018000__battle

/* VIEW(battle): routine GameFlag_GetByte as s32 GameFlag_GetByte(s32 flag);
 * game/game_flags.h has u8 GameFlag_GetByte(s32 flag); tried as the
 * definition's u8 GameFlag_GetByte(s32 flag), which the object already
 * declares, it does not compile in tla-ja (make modern):
 * games/tla/include/gamepak/far_call/window.h:19:42: note: expected 'struct
 * RenderInput *' but argument is of type 's32' {aka 'int'}. */
s32 GameFlag_GetByte__battle(s32 flag) __asm__("GameFlag_GetByte");
#define GameFlag_GetByte GameFlag_GetByte__battle
void ActorSystem_InitializeFar(s32 mode);
void Sound_LoadReverbCode(s32 preset);

/// Fights the battle against the enemy formation enemyGroup, from the opening
/// to the end. Returns the number of enemies defeated when the party won or the
/// link broke, BATTLE_OUTCOME_FLED or BATTLE_OUTCOME_LOST.
s32 Battle_Run(s32 enemyGroup) {
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

  camera = HeapSlot_AllocEwram(HEAP_SLOT_CAMERA, sizeof(struct BattleCamera));
  battle = HeapSlot_AllocEwram(HEAP_SLOT_BATTLE, sizeof(struct EncounterWork));
  HeapSlot_AllocEwram(HEAP_SLOT_ENEMY_UNITS, 0x7c8);
  transition = HeapSlot_AllocEwram(HEAP_SLOT_BATTLE_BACKGROUND, 32);
  HeapSlot_AllocEwram(HEAP_SLOT_BATTLE_UNUSED, 0x280);
  result = 0;
  Iwram_ClearWordsEntry(Ram_HeapSlots->enemyUnits, 0x7c8);
  Task_ResetAll();
  transition->targetYaw = YAW_PARTY_ACTION;
  transition->frames = 0;
  transition->flag = 1;
  transition->unk18 = 0;
  transition->unk1c = 0;
  *(volatile u16 *)0x04000000 = 1;
  GameFlag_Set(FLAG_IN_BATTLE);
  GameFlag_Set(0x169);
  Transform_Reset();

  IWRAM_FILL_WORDS(camera, sizeof(struct BattleCamera), 0);
  Iwram_FillWordsEntry(battle, sizeof(struct EncounterWork), 0);
  battle->promptSlot = -1;
  battle->enemyGroup = enemyGroup;
  HeapSlot_AllocEwram(HEAP_SLOT_BATTLE_MESSAGE, 12);
  IWRAM_FILL_WORDS(Ram_HeapSlots->battleMessage, 12, 0);
  battle->background = BattleBackground_GetSelectedFar();
  HeapSlot_AllocEwram(HEAP_SLOT_SPRITES, 0xe00);
  HeapSlot_AllocEwram(HEAP_SLOT_SPRITE_ANIMATIONS, 0x600);
  ActorSystem_InitializeFar(4);
  if (GameFlag_Get(FLAG_DJINN_LESSON) != 0)
    WindowSystem_InitBattleFar(1);
  else
    WindowSystem_InitBattleFar(0);

  BattleCamera_Create();
  enemyCount = BattleFormation_BuildEnemyList(battle->enemyGroup);

  if (GameFlag_Get(FLAG_LINK_BATTLE) != 0) {
    battle->twoSided = 1;
    /* FAKEMATCH(dead): an empty once-only block after the twoSided store. */
    do {
    } while (0);
    gMainRoutineWork.battleType = BATTLE_TYPE_LINK;
  }
  if (battle->twoSided != 0) {
    /* FAKEMATCH(dead): the seed's store in a once-only block. */
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
    ownerStateDestination = Ram_HeapSlots->enemyUnits;
    for (copyIndex = 0; copyIndex < 0x7c8; copyIndex++)
      *ownerStateDestination++ = Data_02018000[copyIndex];
    enemyCount = GameFlag_GetByte(0x3f0);
    BattleParty_AssignOpponentSlots();
    battle->battleType = 0;
  }
  Task_Add(BattleCamera_Update, 0x47f);

  cue = gMainRoutineWork.battleMusic;
  if (cue != 0) {
    Sound_PlayFar(cue);
    if (GameFlag_Get(FLAG_LINK_BATTLE) != 0) {
      Sound_PlayFar(MUS_055);
      Sound_LoadReverbCode(4);
    }
  } else {
    Sound_PlayFar(MUS_BATTLE);
  }

  BattleUnit_Collect();
  BattleUnit_RefreshPlacement();
  DjinnRecovery_Reconcile();
  BattleSummon_UpdateAvailability();
  if (*(u16 *)Djinn_GetRecoveryTableFar(0) != 0)
    battle->partyStatus = 3;
  else
    battle->partyStatus = 1;
  if (enemyGroup == 75 || enemyGroup == 76)
    ScriptedBattle_RestoreUnits();
  PartyStatus_CreateWindowFar(9);
  BattleCamera_SetDefaultView();
  BattleObject_Put();
  BattleBackground_Init();
  BattleBackground_Load(1, battle->background, 0);
  BattleCamera_SetRange(px(160), px(80), 0, 0, Q16_ONE * 2);
  BattleCamera_SetView(0, 0, 0, 190);
  Party_ReservedNoOp5B14(1);
  *(volatile u16 *)0x04000050 = 0;
  BattleSpoils_Clear();
  battle->promptSlot = VramCache_AllocEntry(128);

  battle->encounterMode = 0;
  if (GameFlag_Get(FLAG_DJINN_LESSON) != 0) {
    battle->encounterMode = 1;
  } else if (gMainRoutineWork.battleType == BATTLE_TYPE_NORMAL) {
    if ((Battle_RandomFar() & 15) == 0)
      battle->encounterMode = 1;
    else if ((Battle_RandomFar() & 31) == 0)
      battle->encounterMode = 2;
  } else if (gMainRoutineWork.battleType == BATTLE_TYPE_LOST) {
    battle->encounterMode = 2;
  }
  BattleTransition_Run(enemyCount, enemyGroup);
  transition->flag = 0;
  gCpuLoadDisplayEnabled = 0;
  Task_Add(BattleObject_UpdateSpritePriorities, 0x480);

  for (round = 1;; round++) {
    if (BattleUnit_ListLivingIds(1, 0) == 0)
      BattleParty_SwapInReserves();
    Battle_TurnStartNoOp();
    BattleSummon_UpdateAvailability();
    if (*(u16 *)Djinn_GetRecoveryTableFar(0) != 0)
      battle->partyStatus = 3;
    else
      battle->partyStatus = 1;
    transition->targetYaw = 0x2800;
    transition->frames = 60;
    PartyStatus_DrawFar(battle->partyStatus);

    Iwram_ClearWordsEntry(battle->actions, sizeof(battle->actions));
    VramCache_FreeEntry(battle->promptSlot);
    if (GameFlag_Get(FLAG_AUTO_BATTLE) == 0)
      actionCount = Battle_GatherActions(battle->actions);
    else
      actionCount = AutoBattle_BuildActions(battle->actions);
    battle->promptSlot = VramCache_AllocEntry(128);
    PartyStatus_DrawFar(battle->partyStatus);
    if (actionCount < 0)
      goto aborted;

    for (i = 0; i < actionCount; i++) {
      actingUnit = battle->actions[i].unitId;
      battle->pendingUnit = -1;
      battle->actionEnded = 0;
      if (GameFlag_Get(FLAG_AUTO_BATTLE) == 0) {
        if (BattleStaging_DispatchAction(&battle->actions[i],
                                         i == 0 ? 10 : 0) == 1)
          goto fled;
      } else {
        if (AutoBattle_RunAction(&battle->actions[i]) == 1)
          goto fled;
      }
      if (enemyGroup == 638)
        goto party_lost;
      if (BattleUnit_ListLivingWithReserve(1, 0) == 0)
        goto party_lost;
      if (BattleUnit_ListLiving(1, 0) == 0)
        break;
      if (BattleUnit_ListLiving(2, 0) == 0) {
        if (enemyGroup == 78 || enemyGroup == 79) {
          Battle_PlayFormChange(enemyGroup++);
          break;
        }
        if (enemyGroup == 80)
          Battle_PlayFormChange(80);
        if (actingUnit <= ALLY_MAX && battle->defeated == 1) {
          /* FAKEMATCH(barrier): an empty asm before the defeatState store. */
          asm("");
          battle->defeatState = 3;
        }
        goto won;
      }
      if (battle->pendingUnit != -1 &&
          Battle_FindUnitPlace(battle->pendingTag) != -1) {
        battle->actions[i].unitId = battle->pendingUnit;
        battle->actions[i].command = COMMAND_DJINN_COUNTER;
        battle->actions[i].parameter = 0;
        battle->actions[i].target = Battle_FindUnitPlace(battle->pendingTag);
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
        battle->actions[i].command = COMMAND_DAEDALUS;
        battle->actions[i].parameter = 0;
        battle->actions[i].target = queue != 0 ? 0 : 0x80;
        battle->actions[i].range = 255;
        i--;
      }
      if (battle->actionEnded != 0)
        break;
    }

    BattleParty_ClearGuards();
    battle->extraQueue = battle->nextExtraQueue;
#if TLA_PATCH_1
    // Japanese leaves the next turn's queue in place.
    battle->nextExtraQueue = 0;
#endif
    battle->extraCount = battle->nextExtraCount;
    battle->nextExtraCount = 0;
    battle->encounterMode = 0;
    Battle_TurnEndNoOp();
    Battle_ProcessTurnEnd();
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
    if (GameFlag_Get(FLAG_DJINN_LESSON) == 0)
      continue;

    window = Message_OpenWindowFar(Tutorial_GetDjinnGiveMessage() + 7, 0, 4, 1);
    while (Message_AllTypedFar() == 0)
      wait(1);
    Window_CloseFar(window, 1);
    wait(1);
    window =
        Message_OpenWindowFar(Tutorial_GetDjinnGiveMessage() + 8, 10, 4, 1);
    BattleMessage_WaitForAdvanceAt(44, 24);
    Window_CloseFar(window, 1);
    wait(1);
  }

won:
  scripted = enemyGroup - 74;
won_scripted:
  Battle_ClearSerialTransferPayload();
  if ((u32)scripted > 1) {
    if (enemyGroup != 80 && GameFlag_Get(FLAG_DJINN_LESSON) == 0) {
      if (battle->twoSided != 0)
        Sound_PlayFar(MUS_VICTORY);
      if (battle->defeated != 0) {
        Sound_PlayFar(MUS_VICTORY);
        if (battle->defeatState <= 1) {
          BattleUnit_SetFar(128, battle->enemyClasses[battle->firstDefeated],
                            26);
          MessageArgument_ClearAllFar();
          MessageArgument_PushFar(128, MESSAGE_ARGUMENT_ACTOR);
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
  result = battle->defeated;
  goto finished;

aborted:
  scripted = enemyGroup - 74;
aborted_scripted:
  Battle_ClearSerialTransferPayload();
  Task_SuspendAll(0);
  *(volatile u16 *)0x04000000 = 1;
  result = battle->defeated;
  GameFlag_Set(FLAG_AREA_TEMP_E8);
  goto finished;

party_lost:
  Battle_ClearSerialTransferPayload();
  scripted = enemyGroup - 74;
  if ((u32)scripted > 1 && enemyGroup != 638) {
    if (enemyGroup != 76) {
      Sound_PlayFar(MUS_DEFEAT);
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
  result = -1;
  ScreenFade_Wait();
  goto finished;

fled:
  Sound_PlayFar(SOUND_FADE_OUT_BGM);
  ScreenFade_ToBlack(30);
  ScreenFade_Wait();
  result = 999;
  scripted = enemyGroup - 74;

finished:
  if ((u32)scripted <= 1)
    ScriptedBattle_SaveUnits();
  BattleParty_ResetActiveRuntimeFields();
  Battle_TurnEndNoOp();
  DjinnRecovery_CountDown();
  gMainRoutineWork.battleType = BATTLE_TYPE_NORMAL;
  Task_Remove(BattleObject_UpdateSpritePriorities);
  BattleBackground_Free();
  return result;
}

/// Lists the party members who fight, three in a link battle, into unitIds when
/// it is not null, and returns how many.
s32 BattleParty_PrepareMembers(u16 *owners) {
  s32 limit;
  s32 count;
  s32 index;

  limit = 4;
  if (Ram_HeapSlots->battleWork->isLink != 0)
    limit = 3;

  count = Party_CountMembersFar();
  if (count > limit)
    count = limit;

  for (index = 0; index < count; index++) {
    s32 owner = gMainRoutineWork.party[index];

    if (owners != 0)
      *owners++ = owner;
    Unit_Get(owner)->side = 2;
  }

  if (owners != 0)
    *owners = 0xff;
  return count;
}

/// Lists the party's reserve members, those past the fourth (none in a link
/// battle), into owners when it is not null, puts each on the party's side and
/// returns how many there are.
s32 BattleParty_PrepareReserveOwners(u16 *owners) {
  s32 count;
  s32 index;

  count = Party_CountMembersFar();
  if (count > 4)
    count -= 4;
  else
    count = 0;
  if (Ram_HeapSlots->battleWork->isLink != 0)
    count = 0;

  for (index = 0; index < count; index++) {
    s32 owner = gMainRoutineWork.party[index + 4];

    if (owners != 0)
      *owners++ = owner;
    Unit_Get(owner)->side = 2;
  }

  if (owners != 0)
    *owners = 0xff;
  return count;
}

/// Lists the enemy units present and returns how many, 0 when unitIds is null.
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
    if (GameFlag_Get(FLAG_LINK_BATTLE) != 0)
      limit = 3;
    id = 0x80;
    limit += 0x80;
    for (; id < limit; id++) {
      if (Unit_Get(id)->side != 0) {
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

/// Lists the units still standing on the sides in sideMask, party first, into
/// unitIds when it is not null, and returns how many.
s32 BattleUnit_ListLiving(s32 sideMask, u16 *unitIds) {
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
  struct Unit *unit;

  output = unitIds;
  livingCount = 0;
  enemyCapacity = 6;
  if (GameFlag_Get(FLAG_LINK_BATTLE) != 0) {
    enemyCapacity = 3;
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

/// As BattleUnit_ListLiving, with the reserve owners listed after the
/// active ones.
s32 BattleUnit_ListLivingWithReserve(s32 sideMask, u16 *unitIds) {
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
  struct Unit *unit;

  output = unitIds;
  livingCount = 0;
  enemyCapacity = 6;
  if (GameFlag_Get(FLAG_LINK_BATTLE) != 0) {
    enemyCapacity = 3;
  }
  if (sideMask & BATTLE_SIDE_MASK_PARTY) {
    base = members;
    count = BattleParty_PrepareMembers(base);
    count += BattleParty_PrepareReserveOwners(base + count);
    if (livingCount < count) {
      member = base;
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

/// Lists the units in the battle on the sides in sideMask, standing or not, and
/// returns how many.
s32 BattleUnit_ListIds(s32 groups, u16 *dst) {
  struct BattleWork *order;
  s32 count;
  s32 i;

  order = Ram_HeapSlots->battleWork;
  count = 0;
  if (groups & 1) {
    for (i = 0; order->partyUnits[i] != BATTLE_UNIT_LIST_END; i++) {
      if (order->partyUnits[i] != BATTLE_UNIT_REMOVED) {
        if (dst != NULL)
          *dst++ = order->partyUnits[i];
        count++;
      }
    }
  }
  if (groups & 2) {
    for (i = 0; order->enemyIds[i] != BATTLE_UNIT_LIST_END; i++) {
      if (order->enemyIds[i] != BATTLE_UNIT_REMOVED) {
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

/// As BattleUnit_ListIds, listing only the actors whose HP is not 0.
s32 BattleUnit_ListLivingIds(s32 groups, u16 *dst) {
  struct BattleWork *order;
  s32 count;
  s32 i;

  order = Ram_HeapSlots->battleWork;
  count = 0;
  if (groups & 1) {
    for (i = 0; order->partyUnits[i] != BATTLE_UNIT_LIST_END; i++) {
      if (order->partyUnits[i] != BATTLE_UNIT_REMOVED &&
          Unit_Get(order->partyUnits[i])->hp != 0) {
        if (dst != NULL)
          *dst++ = order->partyUnits[i];
        count++;
      }
    }
  }
  if (groups & 2) {
    for (i = 0; order->enemyIds[i] != BATTLE_UNIT_LIST_END; i++) {
      if (order->enemyIds[i] != BATTLE_UNIT_REMOVED &&
          Unit_Get(order->enemyIds[i])->hp != 0) {
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
