// Under -mthumb-slow-loads (26 October 2026): differ 50 of 885 in tla-en.
// Tried, one edit at a time: an empty asm barrier after each statement, each
// adjacent pair of statements swapped, each call argument copied into its
// argument register (pinned) at the call or up to three statements before it;
// none reaches 0. Greedy single barriers and a swap reach 28 after six
// rounds, with five barriers.
// Draft, differ 84 of 885, from The Broken Seal's Battle_RunEncounter
// (games/tbs/src/battle/battle/encounter.c): this game allocates by heap
// byte offset and fills with IwramFillWords, creates the camera through
// BattleCamera_Create, plays one battle song, runs a reserve swap each
// round, lets a pending tagged action or a queued extra action repeat the
// current action slot, plays the Djinn tutorial's messages through
// Tutorial_GetDjinnGiveMessage, and treats enemy groups 74 to 80 and 638 as
// scripted battles. The control flow, frame and registers match; left is
// the order of about forty rows, nearly all a constant or pool load the
// native issues one or two rows away from here (the transition's stores,
// the fill calls' arguments, the heap offsets 0x24b and 0x20e, the round
// end's 0x860 and 0x864), and the pool offsets those moves shift. The
// queued action's side reuses the queue's register (queue &= 1): 198 to 84.
#include "types.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "battle_session.h"
#include "battle_display.h"

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

struct AdventureView {
  u8 unk000[0x20e];
  s16 sceneCue;
  u8 unk210[0x3b];
  u8 battleStart;
};

extern struct AdventureView gAdventure;
extern s32 gBattleRandomSeed;
extern volatile u16 gLinkStatus;
extern u8 gCpuLoadDisplayEnabled;
extern u8 Data_02018000[];
extern u8 MsgDowned[];
// Messages 3203 and 3209, not yet named in this game's catalog.
extern u8 MsgPartyDefeated[];
extern u8 MsgGoesDownInDefeat[];

void *HeapSlot_AllocEwram(s32 slot, s32 size);
void Task_ResetAll(void);
void GameFlag_SetBit(s32 flag);
s32 GameFlag_Test(s32 flag);
s32 GameFlag_GetByte(s32 flag);
void Render_ResetTransformState(void);
s32 Event_GetSpecialValueFar(void);
void ObjectSystem_InitializeFar(s32 mode);
void WindowSystem_InitBattleFar(s32 mode);
void BattleCamera_Create(void);
s32 Func_08127cd4(s32 enemyGroup);
void WaitFrames(s32 frames);
void BattleParty_AssignMemberSlots(void);
s32 Task_Add(void (*func)(void), s32 order);
void Task_Remove(void (*func)(void));
void Task_SuspendAll(s32 mode);
void Sound_PlayFar(s32 cue);
void Sound_LoadPresetParameters(s32 preset);
void Func_08118c68(void);
void BattleUnit_RefreshPlacement(void);
void Func_08118eb0(void);
void Func_08118f6c(void);
u16 *Djinn_GetRecoveryTableFar(s32 owner);
void Func_081185c4(void);
void PartyStatus_CreateWindowFar(s32 layout);
void Camera_InitializeDefaultTransform(void);
void BattleObject_Put(void);
void Func_081263c4(void);
void Func_081263fc(s32 mode, s32 background, s32 value);
void BattleCamera_SetRange(s32 cx, s32 cy, s32 ox, s32 oy, s32 scale);
void BattleCamera_SetView(s32 a, s32 b, s32 c, s32 d);
void Party_ReservedNoOp5B14(s32 value);
void BattleSpoils_Clear(void);
s32 VramCache_AllocEntry(s32 size);
u32 Battle_RandomFar(void);
void Func_08125d74(s32 enemyCount, s32 enemyGroup);
void BattleStaging_UpdateCamera(void);
void Func_0811b9fc(void);
s32 Func_0811a39c(s32 side, s32 value);
void Func_081195ec(void);
void Func_0811d7e4(void);
void PartyStatus_DrawFar(s32 mode);
void Resource_ResetEntry(s32 slot);
s32 Battle_GatherActions(struct BattleActionRecord *actions);
s32 AutoBattle_BuildActions(struct BattleActionRecord *actions);
s32 Func_0811d9cc(struct BattleActionRecord *action, s32 delay);
s32 AutoBattle_RunAction(struct BattleActionRecord *action);
s32 Func_0811a24c(s32 side, s32 value);
s32 BattleParty_ListLivingUnits(s32 side, u16 *unitIds);
void Func_081197a8(s32 enemyGroup);
s32 Battle_FindTaggedSlotByValue(s32 tag);
s32 LinkBattle_SyncTurn(void);
void Func_08118e64(void);
void Func_08124cc0(void);
void Func_08124e20(void);
void BattleMotion_DestroyAllSlotObjects(void);
s32 Tutorial_GetDjinnGiveMessage(void);
s32 Message_OpenWindowFar(s32 message, s32 x, s32 y, s32 mode);
s32 Message_AllTypedFar(void);
void Window_CloseFar(s32 window, s32 mode);
void Func_08120060(s32 x, s32 y);
void BattleStaging_ClearLinkPayload(void);
s32 BattleUnit_SetFar(s32 slot, s32 unit, s32 mask);
void MessageArgument_ClearAllFar(void);
void MessageArgument_PushFar(s32 slot, s32 kind);
void Message_ShowAndWaitCoreFar(s32 message);
void BattleMessage_WaitForAdvance(void);
void Func_081284c0(void);
void Fade_ToBlack(s32 vblanks);
void Fade_Wait(void);
s32 BattleParty_PrepareActiveOwners(u16 *owners);
void Func_0811843c(void);
void BattleParty_ResetActiveRuntimeFields(void);
void DjinnRecovery_CountDown(void);
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

  Iwram_FillWords(camera, sizeof(struct BattleCamera), 0);
  Iwram_FillWords(battle, sizeof(struct EncounterWork), 0);
  battle->promptSlot = -1;
  battle->enemyGroup = enemyGroup;
  HeapSlot_AllocEwram(148, 12);
  Iwram_FillWords(Ram_HeapSlots->unk84[4], 12, 0);
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
    gAdventure.battleStart = 4;
  }
  if (battle->twoSided != 0) {
    gBattleRandomSeed = 0;
    for (linkWaitFrames = 0;;) {
      if ((gLinkStatus & 3) == 3)
        break;
      linkWaitFrames++;
      WaitFrames(1);
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
  Task_Add(BattleStaging_UpdateCamera, 0x47f);

  cue = gAdventure.sceneCue;
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
  if (*Djinn_GetRecoveryTableFar(0) != 0)
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
  } else if (gAdventure.battleStart == 0) {
    if ((Battle_RandomFar() & 15) == 0)
      battle->encounterMode = 1;
    else if ((Battle_RandomFar() & 31) == 0)
      battle->encounterMode = 2;
  } else if (gAdventure.battleStart == 6) {
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
    if (*Djinn_GetRecoveryTableFar(0) != 0)
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
      actionCount = AutoBattle_BuildActions(battle->actions);
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
        if (AutoBattle_RunAction(&battle->actions[i]) == 1)
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
        if (actingUnit <= 7 && battle->defeated == 1)
          battle->defeatState = 3;
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
    battle->nextExtraQueue = 0;
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
      WaitFrames(20);
    }
    if (GameFlag_Test(0x16e) == 0)
      continue;

    window = Message_OpenWindowFar(Tutorial_GetDjinnGiveMessage() + 7,
                                         0, 4, 1);
    while (Message_AllTypedFar() == 0)
      WaitFrames(1);
    Window_CloseFar(window, 1);
    WaitFrames(1);
    window = Message_OpenWindowFar(Tutorial_GetDjinnGiveMessage() + 8,
                                         10, 4, 1);
    Func_08120060(44, 24);
    Window_CloseFar(window, 1);
    WaitFrames(1);
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
      Func_081284c0();
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
  scripted = enemyGroup - 74;
  result = 999;

finished:
  if ((u32)scripted <= 1)
    Func_0811843c();
  BattleParty_ResetActiveRuntimeFields();
  Func_08124cc0();
  DjinnRecovery_CountDown();
  gAdventure.battleStart = 0;
  Task_Remove(Func_0811b9fc);
  Func_081263f0();
  return result;
}
