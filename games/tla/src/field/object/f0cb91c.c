#include "types.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "input.h"
#include "actor.h"
#include "iwram_call.h"
#include "scene_ids.h"
#include "macros.h"
#include "version.h"
#include "io_reg.h"

struct FieldWork {
  u8 unk000[16];
  const void *events;
  u8 unk014[0x140];
  s16 coordEvent;
  s16 pushCell;
  s16 warp;
  s16 menu;
  s16 examine;
  s16 start;
  s16 talk;
  s16 searchCell;
  s16 battle;
  s16 psynergy;
  s16 item;
  s16 raisedEvent;
  s16 fallenCount;
  s16 survivorCount;
  s16 fallenOwners[9];
  s16 itemArgument;
  u8 unk184[2];
  u16 keys;
  u8 unk188[2];
  s8 eventMode;
  u8 unk18b[9];
  s32 unk194;
  u8 unk198[4];
  s32 walkingScale;
  s32 walkingDistance;
  u8 unk1a4[4];
  void *previousCell;
  s32 transition;
  u8 unk1b0[2];
  u16 transitionActive;
  s32 transitionFrames;
  u8 unk1b8[0xaea];
  s16 busy;
  u8 unkca4;
  u8 effectTimerRunning;
  u8 unkca6[0x12];
  s16 queuedSound;
  u8 unkcba[0x7c];
  u8 sceneAttributeLow;
  u8 sceneAttributeHigh;
  u8 unkd38[0x18];
};

struct FieldUnit {
  u8 unk00[20];
  s16 hpGauge;
  s16 ppGauge;
  u8 unk18[28];
  s16 maxHp;
  s16 maxPp;
  s16 hp;
  s16 pp;
};

struct OverlayTable {
  u32 words[14];
};

extern struct OverlayTable gOverlayArea;
extern u8 gDebugMode;
extern char MsgFieldStrengthExhausted[];
extern char MsgFieldPartyAnnihilated[];
extern char MsgFieldPauseUnavailable[];
extern u8 gMapCellBuffer[];
extern u8 gData_02001000[];
extern u8 gResourceBlockOwners[];

void *HeapSlot_AllocEwram(s32 slot, s32 size);
void *HeapSlot_Alloc(s32 slot, s32 size);
void HeapSlot_Release(s32 slot);
void GameFlag_ClearBit(s32 flag);
void GameFlag_SetBit(s32 flag);
s32 GameFlag_Test(s32 flag);
void Task_ResetAll(void);
void Func_080d793c(s32 capture);
void Func_08020110(void);
void Func_08020108(s32 map);
void Func_080c9e48(s32 mode);
void Func_080e2718(void);
void WindowSystem_InitFieldFar(void);
void ObjectSystem_InitializeFar(s32 mode);
void BattleEffect_ResetCounters(void);
void BattleMap_ApplySceneEntranceView(void);
void Func_080caa4c(s32 table);
void Func_080cec34(void);
void ObjectTable_Restore(void);
void Func_080d784c(void);
void Map_UpdateCurrentTileBlockFar(void);
void WorldMap_LoadGraphicsFar(s32 x, s32 z);
void Func_08020180(void);
void Map_CenterOnFollowedPointFar(void);
void BattleEffect_InitializeBuffers(void);
void BattleEffect_ApplyColorToTargetBuffer(s32 color, s32 mode);
void Func_080ca2fc(void);
void FieldEffect_ResumeCloakCheck(void);
s32 Field_GetSceneAttributes(s32 scene);
s32 Party_GetLeaderActor(void);
struct Actor *Actor_GetById(s32 id);
void Actor_SetSingleChildFlagsFar(struct Actor *object, s32 flags);
void Func_080cb05c(void);
void wait(s32 frames);
s32 BattleEffect_SumCounters(void);
void ScreenTransition_Start(s32 mode, s32 frames);
void ScreenTransition_Finish(s32 mode, s32 frames);
s32 ObjectEffect_RunPendingFlagEvent(void);
void Func_080e7804(void);
s32 Func_080c9e1c(s32 scene);
void Event_SetDestination(s32 scene, s32 entrance);
void Func_080e7238(void);
void Func_08038200(s32 scene);
void Func_080cb82c(void);
void Func_080cb8a4(void);
s32 Func_080d25c8(s32 item);
s32 PartyInventory_GiveItem(s32 item);
struct Actor *Func_080cb8e8(void);
void Leader_StopMoving(void);
void MessageArgument_PushFar(s32 value, s32 kind);
void FieldMessage_ShowFar(s32 message, s32 mode);
void Sound_PlayFar(s32 cue);
struct FieldUnit *Unit_Get(s32 unitId);
void Func_080ca3f4(s32 scene, s32 entrance);
s32 Func_080ed2a4(s32 mode);
void BattleEffect_RunVisibilityTransition(void);
void Func_080d5720(void);
void Func_080d57e4(void);
void Func_080d59dc(void);
void Func_080d5aa0(void);
void Scene_RunRaisedEvent(s32 event);
void Scene_RunTouchOrExit(s32 event);
void Func_080cd680(s32 cell);
void ObjectTable_Snapshot(void);
void Func_080d5c70(s32 battle);
void UiTimedNotice_CloseIfActiveFar(void);
s32 Field_FindNearbyTalkTarget(s32 owner);
void *Func_080cce94(s32 id);
s32 Func_080cc54c(void);
void Menu_RunSelectionWithCursorObjectFar(void);
void Menu_RunTopSelectionFar(void);
void GameFlag_RefreshLureCapFar(void);
s32 Func_080ccec8(s32 id);
void Func_080cd808(s32 cell);
s32 Func_080ce61c(u32 code);
void Func_080ce0ac(s32 item, s32 argument);
void Menu_RunSelectionFar(void);
void Debug_RunPaletteEditor(void);
void Event_ResolveAllPendingActions(void);
s32 Sound_GetCurrentBgmFar(void);
void Func_08038358(s32 mode);
void Func_080200b0(void);
void Func_080cb2b8(s32 terrain, s32 x, s32 y, s32 z);

static __inline__ s32 Unit_ClampGauge(s32 ratio) {
  s32 clamped;

  if (ratio > 0x4000)
    clamped = 0x4000;
  else if (ratio < 0)
    clamped = 0;
  else
    clamped = ratio;
  return clamped;
}

#define Unit_UpdateGauges(unit)                                                \
  do {                                                                         \
    (unit)->hpGauge = Unit_ClampGauge(((unit)->hp << 14) / (unit)->maxHp);     \
    if ((unit)->hpGauge == 0 && (unit)->hp != 0)                               \
      (unit)->hpGauge = 1;                                                     \
    (unit)->ppGauge = Unit_ClampGauge(((unit)->pp << 14) / (unit)->maxPp);     \
    if ((unit)->ppGauge == 0 && (unit)->pp != 0)                               \
      (unit)->ppGauge = 1;                                                     \
  } while (0)

struct ArrivalView {
  u8 unk000[0x2be];
  s16 scene;
  s16 entrance;
  s8 state;
};

struct HazardView {
  u8 unk000[0x24c];
  u16 limit;
  u16 mode;
  u16 damage;
};

#define HAZARD (*(struct HazardView *)&gMainRoutineWork)
#define ARRIVAL (*(struct ArrivalView *)&gMainRoutineWork)

#define OVERLAY_CALL(index) ((s32 (*)(void))gOverlayArea.words[index])()

s32 FieldSequence_Run(void) {
  struct FieldWork *work;
  struct MainRoutineWork *party;
  struct Actor *leader;
  struct FieldUnit *unit;
  s32 result;
  s32 arrivalState;
  s32 *leaderId;
  s32 *steps;
  s16 *entrance;
  u16 *turn;
  s8 *pending;
  s32 code;
  s32 mode;
  u32 value;
  s32 talk;
  s32 packed;

  work = HeapSlot_AllocEwram(108, 0xd50);
  Iwram_ClearWords(work, 0xd50);
  GameFlag_ClearBit(259);
  gMainRoutineWork.primary = gMainRoutineWork.scene;
  gMainRoutineWork.secondary = gMainRoutineWork.entrance;
  gMainRoutineWork.pair1ee[0] = -1;
  gMainRoutineWork.pair1ee[1] = -1;
  gMainRoutineWork.pair1f2[0] = -1;
  gMainRoutineWork.pair1f2[1] = -1;
  gMainRoutineWork.unk2c3 = 0;
  Task_ResetAll();
  Func_080d793c(0);
  if (gMainRoutineWork.map == (s32)&SceneId_Ov64a) {
    Func_08020110();
    gMainRoutineWork.unk244[0] = 1;
    mode = 3;
    Func_080c9e48(0);
  } else {
    Func_08020108(gMainRoutineWork.map);
    mode = 2;
    Func_080c9e48(1);
  }
  work->eventMode = mode;
  if (!GameFlag_Test(265))
    gData_02001000[0x194c] = 0;
  else
    Func_080e2718();
  WindowSystem_InitFieldFar();
  ObjectSystem_InitializeFar(mode);
  BattleEffect_ResetCounters();
  work->events = (const void *)OVERLAY_CALL(9);
  BattleMap_ApplySceneEntranceView();
  Func_080caa4c(OVERLAY_CALL(7));
  Func_080cec34();
  if (GameFlag_Test(265))
    ObjectTable_Restore();
  if (gMainRoutineWork.pendingDjinnEvent != 0)
    Func_080d784c();
  work->queuedSound = -1;
  if (mode == 3) {
    Map_UpdateCurrentTileBlockFar();
    WorldMap_LoadGraphicsFar(gMainRoutineWork.x, gMainRoutineWork.z);
  } else {
    Func_08020180();
    OVERLAY_CALL(13);
    Map_CenterOnFollowedPointFar();
  }
  GameFlag_ClearBit(287);
  BattleEffect_InitializeBuffers();
  BattleEffect_ApplyColorToTargetBuffer(0x10000, 0);
  work->transition = 0x100;
  work->transitionFrames = 16;
  work->transitionActive = 0;
  work->walkingScale = 409;
  work->walkingDistance = 0;
  if (gMainRoutineWork.sceneVariant == 2) {
    work->walkingScale = 0;
    GameFlag_SetBit(324);
  }
  work->previousCell = gMapCellBuffer;
  if (!GameFlag_Test(265)) {
    Func_080ca2fc();
    HAZARD.limit = 16;
    HAZARD.mode = 0;
    HAZARD.damage = 1;
    gMainRoutineWork.pausedObject = -1;
    gMainRoutineWork.cloaked = 0;
  }
  FieldEffect_ResumeCloakCheck();
  /* FAKEMATCH(signature): Field_GetSceneAttributes declared returning s32,
   * though it returns s16; as s16, 17 of 1300 instructions differ in
   * tla-en. */
  value = Field_GetSceneAttributes(gMainRoutineWork.scene);
  work->sceneAttributeHigh = (value >> 4) & 15;
  work->sceneAttributeLow = value & 15;

  if (ARRIVAL.state == 2) {
    Actor_GetById(Party_GetLeaderActor())->scaleX = 0;
    Actor_SetSingleChildFlagsFar(Actor_GetById(Party_GetLeaderActor()), 0);
  }
  Func_080cb05c();
  wait(2);
  OVERLAY_CALL(1);
  code = work->warp;
  if (code != 0) {
    result = code;
    work->warp = 0;
    goto done;
  }
  GameFlag_ClearBit(265);
  if (BattleEffect_SumCounters() == 0) {
    value = work->transitionActive;
    if (value == 0) {
      if (ARRIVAL.state == 1) {
        HeapSlot_Alloc(92, 0x19b8);
        wait(20);
        HeapSlot_Release(92);
      }
      ScreenTransition_Start(work->transition, work->transitionFrames);
      work->transitionActive = 1;
      if (ARRIVAL.state == 1) {
        HeapSlot_Alloc(92, 0x19b8);
        wait(20);
        HeapSlot_Release(92);
      }
      PLTT_BACKDROP = 0;
      if (!ObjectEffect_RunPendingFlagEvent())
        wait((work->transitionFrames + 1) / 2);
    }
    arrivalState = ARRIVAL.state;
    if (arrivalState == 1) {
      // Through a pointer: stored as ARRIVAL.state, the byte is read and masked
      // with a 0 made before the store, which then serves ownerOverride.
      *(s8 *)&ARRIVAL.state = 2;
      // Once-only: inside the block the 0's one use counts at loop depth, so
      // it stays in a register across Func_080e7804.
      do {
        s32 zero = 0;

        Func_080e7804();
        // (s16): the id compared as the field's own width (79 differ without).
        if (ARRIVAL.scene != (s16)(s32)&SceneId_WorldMap)
          goto clearOwner;
        /* FAKEMATCH(barrier): an r0 clobber, so reload forgets r0 between the
         * 0x2be and 0x2c0 addresses; as no asm, 21 of 1300 instructions differ
         * in tla-en. */
        asm("" : : : "r0");
        if ((value = Func_080c9e1c(ARRIVAL.entrance)) & 16)
        clearOwner:
          gMainRoutineWork.ownerOverride = zero;
        else if (value & 32)
          gMainRoutineWork.ownerOverride = 1;
      } while (0);
      Event_SetDestination(ARRIVAL.scene, ARRIVAL.entrance);
    } else if (arrivalState == 2) {
      ARRIVAL.state = 0;
      Func_080e7238();
    }
    if (GameFlag_Test(303)) {
      GameFlag_ClearBit(303);
      Func_08038200(gMainRoutineWork.scene);
    }
    if (gMainRoutineWork.pendingDjinnEvent != 0) {
      // UB: called with a second argument (1) its definition does not take.
#ifdef UBFIX
      Func_080d793c(gMainRoutineWork.pendingDjinnEvent);
#else
      ((void (*)(s32, s32))Func_080d793c)(gMainRoutineWork.pendingDjinnEvent,
                                          1);
#endif
      gMainRoutineWork.pendingDjinnEvent = 0;
    }
    if (gMainRoutineWork.tally25c != 0) {
      Func_080cb82c();
      if (Func_080d25c8(gMainRoutineWork.tally25c) == 0) {
        // UB: called with a second argument (0) its definition does not take.
#ifdef UBFIX
        PartyInventory_GiveItem(gMainRoutineWork.tally25c);
#else
        ((s32 (*)(s32, s32))PartyInventory_GiveItem)(gMainRoutineWork.tally25c,
                                                     0);
#endif
      }
      Func_080cb8a4();
      gMainRoutineWork.tally25c = 0;
    }
  }

  party = &gMainRoutineWork;
  leaderId = &gMainRoutineWork.currentOwner;
  steps = &gMainRoutineWork.encounterSteps;
  entrance = &gMainRoutineWork.entrance;
  turn = &gMainRoutineWork.turn;
  for (;;) {
    GameFlag_SetBit(260);
    leader = Func_080cb8e8();
    Actor_ResetMotion(leader);
    party->x = leader->x;
    party->y = 0;
    party->z = leader->z;
    party->heading = leader->angle;
    *turn = leader->terrainId;
    while (BattleEffect_SumCounters() != 0) {
      if (work->warp != 0) {
        result = work->warp;
        work->warp = 0;
        goto done;
      }
      if (work->raisedEvent != 0) {
        work->busy = 1;
        if (work->raisedEvent == -1) {
          Leader_StopMoving();
          Func_080cb82c();
          for (value = 0; value < work->fallenCount; value++) {
            Actor_SetMode(leader, 22);
            MessageArgument_PushFar(work->fallenOwners[value], 1);
            FieldMessage_ShowFar((s32)MsgFieldStrengthExhausted, 1);
          }
          if (work->survivorCount == 0) {
            if (*leaderId == 4)
              Actor_SetMode(leader, 36);
            else
              Actor_SetMode(leader, 19);
            Sound_PlayFar(59);
            FieldMessage_ShowFar((s32)MsgFieldPartyAnnihilated, 1);
            unit = Unit_Get(*leaderId);
            unit->hp = 1;
            Unit_UpdateGauges(unit);
            {
              s32 restartScene = party->savedScene;
              s32 restartEntrance;

              party->scene = restartScene;
              restartEntrance = party->savedEntrance;
              *entrance = restartEntrance;
              Func_080ca3f4((s16)restartScene, (s16)restartEntrance);
            }
            result = 999;
            Func_080cb8a4();
            goto done;
          }
          Func_080cb8a4();
        } else if (work->raisedEvent == -0x377) {
          Leader_StopMoving();
          Func_080ed2a4(1);
          if (ARRIVAL.scene != -1)
            ARRIVAL.state = 1;
        } else if (work->raisedEvent == -0x378) {
          Leader_StopMoving();
          value = 0;
          while (Func_080ed2a4(value))
            value ^= 1;
        } else if (work->raisedEvent == -0x379) {
          Leader_StopMoving();
          BattleEffect_RunVisibilityTransition();
        } else if (work->raisedEvent == -0x37b) {
          if (work->keys == gInput.held) {
            Func_080d5720();
            work->menu = 0;
            work->start = 0;
            work->examine = 0;
            work->battle = 0;
            work->psynergy = 0;
          }
        } else if (work->raisedEvent == -0x37a) {
          if (work->keys == gInput.held) {
            Func_080d57e4();
            work->menu = 0;
            work->start = 0;
            work->examine = 0;
            work->battle = 0;
            work->psynergy = 0;
          }
        } else if (work->raisedEvent == -0x37d) {
          if (work->keys == gInput.held) {
            Func_080d59dc();
            work->menu = 0;
            work->start = 0;
            work->examine = 0;
            work->battle = 0;
            work->psynergy = 0;
          }
        } else if (work->raisedEvent == -0x37c) {
          if (work->keys == gInput.held) {
            Func_080d5aa0();
            work->menu = 0;
            work->start = 0;
            work->examine = 0;
            work->battle = 0;
            work->psynergy = 0;
          }
        } else {
          Scene_RunRaisedEvent(work->raisedEvent);
        }
        work->busy = 0;
        work->raisedEvent = 0;
      } else if (work->coordEvent != 0) {
        work->busy = 1;
        Scene_RunTouchOrExit(work->coordEvent);
        work->busy = 0;
        work->coordEvent = 0;
      } else if (work->pushCell != 0) {
        Func_080cd680(work->pushCell);
        work->pushCell = 0;
      } else if (work->battle != 0) {
        Leader_StopMoving();
        ObjectTable_Snapshot();
        party->returnCue = -1;
        party->scene = 510;
        *entrance = work->battle;
        work->warp = 999;
        Func_080d5c70(work->battle);
        work->unk194 = 0;
        work->battle = 0;
        *steps = 0;
      } else if (work->examine != 0) {
        UiTimedNotice_CloseIfActiveFar();
        talk = 0;
        if (gMainRoutineWork.ownerOverride == 0)
          value = Field_FindNearbyTalkTarget(*leaderId);
        else
          value = Field_FindNearbyTalkTarget(8);
        if (value != -1) {
          talk = 1;
          if ((s32)value <= 63)
            talk = Func_080cce94(value) != 0;
        }
        if (talk) {
          work->talk = value | 0x4000;
          work->menu = 0;
        } else if ((talk = Func_080cc54c()) != 0) {
          work->searchCell = talk;
          work->menu = 0;
        } else {
          work->menu = 1;
        }
        work->examine = 0;
      } else if (work->menu != 0) {
        UiTimedNotice_CloseIfActiveFar();
        Leader_StopMoving();
        Sound_PlayFar(111);
        Func_080cb82c();
        GameFlag_SetBit(262);
        if (gDebugMode && (*(volatile u32 *)&gInput.held & 2) != 0 &&
            ((*(volatile u32 *)&gInput.held & 4) != 0)) {
          Menu_RunSelectionWithCursorObjectFar();
        } else if (GameFlag_Test(263)) {
          work->raisedEvent = 250;
        } else {
          work->effectTimerRunning = 0;
          Menu_RunTopSelectionFar();
          work->effectTimerRunning = 1;
          Func_080cb05c();
        }
        Func_080cb8a4();
        GameFlag_ClearBit(262);
        GameFlag_RefreshLureCapFar();
        work->menu = 0;
      } else if (work->talk != 0) {
        Func_080cb82c();
        Func_080ccec8(work->talk & 0x3fff);
        Func_080cb8a4();
        work->talk = 0;
      } else if (work->searchCell != 0) {
        Func_080cb82c();
        Func_080cd808(work->searchCell);
        Func_080cb8a4();
        work->searchCell = 0;
      } else if (work->psynergy != 0) {
        packed = work->psynergy & 0x2000;
        UiTimedNotice_CloseIfActiveFar();
        if (packed == 0) {
          Func_080cb82c();
          if (TLA_PATCH_1)
            wait(1);
        }
        Func_080ce61c(work->psynergy);
        if (packed == 0)
          Func_080cb8a4();
        work->psynergy = 0;
      } else if (work->item != 0) {
        Func_080cb82c();
        Func_080ce0ac(work->item, work->itemArgument);
        Func_080cb8a4();
        work->item = 0;
      } else if (work->start != 0) {
        Sound_PlayFar(111);
        Leader_StopMoving();
        Func_080cb82c();
        GameFlag_SetBit(262);
        if (*(volatile u8 *)&gDebugMode && (gInput.held & 2)) {
          Menu_RunSelectionFar();
        } else if (*(volatile u8 *)&gDebugMode && (gInput.held & 0x200)) {
          Debug_RunPaletteEditor();
        } else if (GameFlag_Test(263)) {
          work->raisedEvent = 250;
        } else {
          Event_ResolveAllPendingActions();
          UiTimedNotice_CloseIfActiveFar();
          party->returnCue = Sound_GetCurrentBgmFar();
          if (!GameFlag_Test(382)) {
            u8 *objects = (u8 *)Ram_HeapSlots->objectSystem;
            u8 *owner = gResourceBlockOwners;
            s32 freeBlocks = 0;
            s32 n = 512;

            do {
              if (*owner++ == 0xff)
                freeBlocks++;
            } while (--n != 0);
            if (freeBlocks - 136 < 0) {
              *(u16 *)(objects + 4) = 1;
              wait(1);
            }
            Func_08038358(0);
            *(u16 *)(objects + 4) = 0;
          } else {
            FieldMessage_ShowFar((s32)MsgFieldPauseUnavailable, 1);
          }
        }
        Func_080cb8a4();
        GameFlag_ClearBit(262);
        work->start = 0;
      }
    }
    GameFlag_ClearBit(260);
    if (Func_080cb8e8() != NULL)
      Func_080200b0();
    pending = &gMainRoutineWork.pendingThird;
    do {
      wait(1);
      leader = Func_080cb8e8();
      if (gDebugMode == 0 || !GameFlag_Test(355)) {
        /* FAKEMATCH(signature): the u16 unk66 read as s16 (an ldrsh); as
         * leader->unk66 == 0, 39 of 1300 instructions differ in tla-en. */
        if (*pending == 0 ||
            (leader->action == 0 && *(s16 *)&leader->unk66 == 0))
          Func_080cb2b8(leader->terrainId, leader->x, leader->y, leader->z);
      }
    } while (BattleEffect_SumCounters() == 0);
  }

done:
  if (work->transitionActive != 0) {
    ScreenTransition_Finish(work->transition, work->transitionFrames);
    work->transitionActive = 0;
    wait(work->transitionFrames);
  }
  wait(1);
  HeapSlot_Release(108);
  return result;
}
