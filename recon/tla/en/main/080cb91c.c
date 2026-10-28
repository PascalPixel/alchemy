// Draft, differ 940 of 1300: SceneSequence_Run, this game's form of ☀️'s
// SceneSequence_Run (games/tbs/src/overworld/overworld/scene_sequence.c):
// runs the current scene until the party leaves it and returns how (the
// warp, or 999 after the party falls). It sets the scene up in the event
// work (heap slot 108, 0xd50 bytes, cleared), the world map (scene map 1,
// event mode 3) or a layout (mode 2), places objects and the camera, fades
// in (with the ship's arrival wait when the adventure's 0x2c2 is 1) and
// gives what was left pending, then loops: the leader walks (Func_080cb2b8
// each frame) until a request is made, and the requests are handled until none
// is left: a raised event (the party fell, the world map, its zoom, the
// four ship commands), a touch or exit, a push, a battle, examining, the
// menu, talking, searching, Psynergy, an item or the pause menu.
// The same calls, offsets and control flow; left: register and stack
// allocation throughout (the native stack frame is 12 bytes against 16, with the
// adventure at sp 8, the event work in r8 and the transition fields in
// r9 to r11), which shifts most rows.
#include "types.h"
#include "ram_buffer.h"
#include "adventure.h"
#include "input.h"
#include "object_runtime.h"
#include "iwram_call.h"
#include "scene_ids.h"

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
extern u8 gMapCellBuffer[];
extern u8 gData_02001000[];
extern u8 gResourceBlockOwners[];
extern u16 Data_0200044c;
extern s32 Data_02000454;
extern u16 Data_02000422;
extern s32 Data_02000498;
extern s8 Data_020004ac;

void *Runtime_AllocateBlock(s32 slot, s32 size);
void *Runtime_AllocateHeapBlock(s32 slot, s32 size);
void Runtime_ReleaseHeapBlock(s32 slot);
void GameFlag_ClearBit(s32 flag);
void GameFlag_SetBit(s32 flag);
s32 GameFlag_Test(s32 flag);
void Task_ResetAll(void);
void Func_080d793c();
void Func_08020110(void);
void Func_08020108(void);
void Func_080c9e48(s32 mode);
void Func_080e2718(void);
void UiWork_InitializeWithResourceCountersFar(void);
void ObjectSystem_InitializeFar(s32 mode);
void BattleEffect_ResetCounters(void);
void BattleMap_ApplySceneEntranceView(void);
void Func_080caa4c(void);
void Func_080cec34(void);
void Func_080cae5c(void);
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
s32 EventRuntime_GetControlledOwner(void);
struct ObjectRuntime *Object_GetById(s32 id);
void ObjectDispatch_SetSingleChildFlagsFar(struct ObjectRuntime *object,
                                           s32 flags);
void Func_080cb05c(void);
void WaitFrames(s32 frames);
s32 BattleEffect_SumCounters(void);
void ScreenTransition_Start(s32 mode, s32 frames);
void ScreenTransition_Finish(s32 mode, s32 frames);
s32 ObjectEffect_RunPendingFlagEvent(void);
void Func_080e7804(void);
s32 Func_080c9e1c(s32 scene);
void EventRuntime_SetDestination(s32 scene, s32 entrance);
void Func_080e7238(void);
void Func_08038200(s32 scene);
void Func_080cb82c(void);
void Func_080cb8a4(void);
s32 Func_080d25c8(s32 item);
void PartyInventory_GiveItem(s32 item, s32 mode);
struct ObjectRuntime *Func_080cb8e8(void);
void EventRuntime_PrepareCurrentObject(void);
void UiText_DrawQuantity(s32 value, s32 kind);
void UiText_ShowPositionedMessageAndWaitFar(s32 message, s32 mode);
void Sound_PlayFar(s32 cue);
struct FieldUnit *Unit_Get(s32 unitId);
void Func_080ca3f4(s32 scene, s32 entrance);
s32 Func_080ed2a4(s32 mode);
void BattleEffect_RunVisibilityTransition(void);
void Func_080d5720(void);
void Func_080d57e4(void);
void Func_080d59dc(void);
void Func_080d5aa0(void);
void Scene_RunRaisedEvent(void);
void Scene_RunTouchOrExit(s32 event);
void Func_080cd680(s32 cell);
void ObjectTable_Snapshot(void);
void Func_080d5c70(s32 battle);
void UiTimedNotice_CloseIfActiveFar(void);
s32 Field_FindNearbyTalkTarget(s32 owner);
s32 Func_080cce94(s32 target);
s32 Func_080cc54c(void);
void Menu_RunSelectionWithCursorObjectFar(void);
void Menu_RunTopSelectionFar(void);
void GameFlag_RefreshLureCapFar(void);
s32 Func_080ccec8(s32 id);
void Func_080cd808(s32 cell);
s32 Func_080ce61c(u32 code);
void Func_080ce0ac(s32 item, s32 argument);
void Menu_RunSelectionFar(void);
void Func_080cc9fc(void);
void EventRuntime_ResolveAllPendingActions(void);
s32 Sound_GetCurrentBgmFar(void);
void Func_08038358(s32 mode);
void Func_080200b0(void);
void Func_080cb2b8(s32 terrain, s32 x, s32 y, s32 z);

#define OVERLAY_CALL(index) ((s32 (*)(void))gOverlayArea.words[index])()

s32 SceneSequence_Run(void) {
  struct FieldWork *work;
  struct Adventure *party;
  struct ObjectRuntime *leader;
  struct FieldUnit *unit;
  s32 result;
  s32 mode;
  u32 attributes;
  s32 flags;
  s32 target;
  s32 talk;
  s32 i;
  s32 gauge;
  s32 packed;
  s32 freeBlocks;
  s32 n;
  u8 *owner;
  u8 *objects;
  s8 *arrival;
  s16 zero;

  work = Runtime_AllocateBlock(108, 0xd50);
  Iwram_ClearWords(work, 0xd50);
  GameFlag_ClearBit(259);
  gAdventure.primary = gAdventure.scene;
  gAdventure.secondary = gAdventure.entrance;
  gAdventure.pair1ee[0] = -1;
  gAdventure.pair1ee[1] = -1;
  gAdventure.pair1f2[0] = -1;
  gAdventure.pair1f2[1] = -1;
  gAdventure.unk2c3 = 0;
  Task_ResetAll();
  Func_080d793c(0);
  if (gAdventure.map == (s32)&SceneId_WorldMap) {
    Func_08020110();
    gAdventure.unk244[0] = 1;
    mode = 3;
    Func_080c9e48(0);
  } else {
    Func_08020108();
    mode = 2;
    Func_080c9e48(1);
  }
  work->eventMode = mode;
  if (!GameFlag_Test(265))
    gData_02001000[0x194c] = 0;
  else
    Func_080e2718();
  UiWork_InitializeWithResourceCountersFar();
  ObjectSystem_InitializeFar(mode);
  BattleEffect_ResetCounters();
  work->events = (const void *)OVERLAY_CALL(9);
  BattleMap_ApplySceneEntranceView();
  OVERLAY_CALL(7);
  Func_080caa4c();
  Func_080cec34();
  if (GameFlag_Test(265))
    Func_080cae5c();
  if (gAdventure.pendingDjinnEvent != 0)
    Func_080d784c();
  work->queuedSound = -1;
  if (mode == 3) {
    Map_UpdateCurrentTileBlockFar();
    WorldMap_LoadGraphicsFar(gAdventure.x, gAdventure.z);
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
  zero = 0;
  work->transitionActive = zero;
  work->walkingScale = 409;
  work->walkingDistance = zero;
  if (gAdventure.sceneVariant == 2) {
    work->walkingScale = zero;
    GameFlag_SetBit(324);
  }
  work->previousCell = gMapCellBuffer;
  if (!GameFlag_Test(265)) {
    Func_080ca2fc();
    *(u16 *)gAdventure.unk24c = 16;
    *(u16 *)&gAdventure.unk24c[2] = zero;
    *(u16 *)&gAdventure.unk24c[4] = 1;
    gAdventure.pausedObject = -1;
    gAdventure.cloaked = 0;
  }
  FieldEffect_ResumeCloakCheck();
  attributes = Field_GetSceneAttributes(gAdventure.scene);
  work->sceneAttributeHigh = (attributes >> 4) & 15;
  work->sceneAttributeLow = attributes & 15;
  arrival = (s8 *)&gAdventure.unk2c2;
  if (*arrival == 2) {
    Object_GetById(EventRuntime_GetControlledOwner())->scaleX = zero;
    ObjectDispatch_SetSingleChildFlagsFar(
        Object_GetById(EventRuntime_GetControlledOwner()), 0);
  }
  Func_080cb05c();
  WaitFrames(2);
  OVERLAY_CALL(1);
  result = work->warp;
  if (result != 0) {
    work->warp = zero;
    goto done;
  }
  GameFlag_ClearBit(265);
  if (BattleEffect_SumCounters() == 0) {
    n = work->transitionActive;
    if (n == 0) {
      if (*arrival == 1) {
        Runtime_AllocateHeapBlock(92, 0x1938);
        WaitFrames(20);
        Runtime_ReleaseHeapBlock(92);
      }
      ScreenTransition_Start(work->transition, work->transitionFrames);
      work->transitionActive = 1;
      if (*arrival == 1) {
        Runtime_AllocateHeapBlock(92, 0x1938);
        WaitFrames(20);
        Runtime_ReleaseHeapBlock(92);
      }
      *(volatile u16 *)0x05000000 = n;
      if (!ObjectEffect_RunPendingFlagEvent())
        WaitFrames((work->transitionFrames + 1) / 2);
    }
    n = *(s8 *)&gAdventure.unk2c2;
    if (n == 1) {
      gAdventure.unk2c2 = 2;
      Func_080e7804();
      if (*(s16 *)gAdventure.unk2be != (s32)&SceneId_Ov64a ||
          ((flags = Func_080c9e1c(*(s16 *)&gAdventure.unk2be[2])) & 16))
        gAdventure.ownerOverride = 0;
      else if (flags & 32)
        gAdventure.ownerOverride = 1;
      EventRuntime_SetDestination(*(s16 *)gAdventure.unk2be,
                                  *(s16 *)&gAdventure.unk2be[2]);
    } else if (n == 2) {
      gAdventure.unk2c2 = 0;
      Func_080e7238();
    }
    if (GameFlag_Test(303)) {
      GameFlag_ClearBit(303);
      Func_08038200(gAdventure.scene);
    }
    if (gAdventure.pendingDjinnEvent != 0) {
      Func_080d793c(gAdventure.pendingDjinnEvent, 1);
      gAdventure.pendingDjinnEvent = 0;
    }
    if (gAdventure.tally25c != 0) {
      Func_080cb82c();
      if (Func_080d25c8(gAdventure.tally25c) == 0)
        PartyInventory_GiveItem(gAdventure.tally25c, 0);
      Func_080cb8a4();
      gAdventure.tally25c = 0;
    }
  }

  party = &gAdventure;
  for (;;) {
    GameFlag_SetBit(260);
    leader = Func_080cb8e8();
    Object_ResetMotion(leader);
    party->x = leader->x;
    party->y = 0;
    party->z = leader->z;
    party->heading = leader->angle;
    Data_0200044c = leader->terrainId;
    while (BattleEffect_SumCounters() != 0) {
      if (work->warp != 0) {
        result = work->warp;
        work->warp = 0;
        goto done;
      }
      if (work->raisedEvent != 0) {
        work->busy = 1;
        if (work->raisedEvent == -1) {
          EventRuntime_PrepareCurrentObject();
          Func_080cb82c();
          for (i = 0; i < work->fallenCount; i++) {
            Object_SetMode(leader, 22);
            UiText_DrawQuantity(work->fallenOwners[i], 1);
            UiText_ShowPositionedMessageAndWaitFar(0xd92, 1);
          }
          if (work->survivorCount == 0) {
            if (Data_02000454 == 4)
              Object_SetMode(leader, 36);
            else
              Object_SetMode(leader, 19);
            Sound_PlayFar(59);
            UiText_ShowPositionedMessageAndWaitFar(0xd93, 1);
            unit = Unit_Get(Data_02000454);
            unit->hp = 1;
            gauge = (unit->hp << 14) / unit->maxHp;
            if (gauge > 0x4000)
              gauge = 0x4000;
            else if (gauge < 0)
              gauge = 0;
            unit->hpGauge = gauge;
            if ((s16)gauge == 0 && unit->hp != 0)
              unit->hpGauge = 1;
            gauge = (unit->pp << 14) / unit->maxPp;
            if (gauge > 0x4000)
              gauge = 0x4000;
            else if (gauge < 0)
              gauge = 0;
            unit->ppGauge = gauge;
            if ((s16)gauge == 0 && unit->pp != 0)
              unit->ppGauge = 1;
            party->scene = party->savedScene;
            Data_02000422 = party->savedEntrance;
            Func_080ca3f4(party->scene, Data_02000422);
            result = 999;
            Func_080cb8a4();
            goto done;
          }
          Func_080cb8a4();
        } else if (work->raisedEvent == -0x377) {
          EventRuntime_PrepareCurrentObject();
          Func_080ed2a4(1);
          if (*(s16 *)gAdventure.unk2be != -1)
            gAdventure.unk2c2 = 1;
        } else if (work->raisedEvent == -0x378) {
          EventRuntime_PrepareCurrentObject();
          n = 0;
          while (Func_080ed2a4(n))
            n ^= 1;
        } else if (work->raisedEvent == -0x379) {
          EventRuntime_PrepareCurrentObject();
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
          Scene_RunRaisedEvent();
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
        EventRuntime_PrepareCurrentObject();
        ObjectTable_Snapshot();
        party->returnCue = -1;
        party->scene = 510;
        Data_02000422 = work->battle;
        work->warp = 999;
        Func_080d5c70(work->battle);
        work->unk194 = 0;
        work->battle = 0;
        Data_02000498 = 0;
      } else if (work->examine != 0) {
        UiTimedNotice_CloseIfActiveFar();
        talk = 0;
        target = Field_FindNearbyTalkTarget(
            gAdventure.ownerOverride == 0 ? Data_02000454 : 8);
        if (target != -1) {
          talk = 1;
          if (target <= 63)
            talk = Func_080cce94(target) != 0;
        }
        if (talk) {
          work->talk = target | 0x4000;
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
        EventRuntime_PrepareCurrentObject();
        Sound_PlayFar(111);
        Func_080cb82c();
        GameFlag_SetBit(262);
        if (gDebugMode && (gInput.held & 2) && (gInput.held & 4)) {
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
        packed = (s16)(work->psynergy & 0x2000);
        UiTimedNotice_CloseIfActiveFar();
        if (packed == 0) {
          Func_080cb82c();
          WaitFrames(1);
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
        EventRuntime_PrepareCurrentObject();
        Func_080cb82c();
        GameFlag_SetBit(262);
        if (gDebugMode && (gInput.held & 2)) {
          Menu_RunSelectionFar();
        } else if (gDebugMode && (gInput.held & 0x200)) {
          Func_080cc9fc();
        } else if (GameFlag_Test(263)) {
          work->raisedEvent = 250;
        } else {
          EventRuntime_ResolveAllPendingActions();
          UiTimedNotice_CloseIfActiveFar();
          party->returnCue = Sound_GetCurrentBgmFar();
          if (!GameFlag_Test(382)) {
            objects = (u8 *)Ram_HeapSlots->objectSystem;
            owner = gResourceBlockOwners;
            freeBlocks = 0;
            n = 512;
            do {
              if (*owner++ == 0xff)
                freeBlocks++;
            } while (--n != 0);
            if (freeBlocks - 136 < 0) {
              *(u16 *)(objects + 4) = 1;
              WaitFrames(1);
            }
            Func_08038358(0);
            *(u16 *)(objects + 4) = 0;
          } else {
            UiText_ShowPositionedMessageAndWaitFar(0x1167, 1);
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
    do {
      WaitFrames(1);
      leader = Func_080cb8e8();
      if (gDebugMode == 0 || !GameFlag_Test(355)) {
        if (Data_020004ac == 0 || (leader->action == 0 && leader->unk66 == 0))
          Func_080cb2b8(leader->terrainId, leader->x, leader->y, leader->z);
      }
    } while (BattleEffect_SumCounters() == 0);
  }

done:
  if (work->transitionActive != 0) {
    ScreenTransition_Finish(work->transition, work->transitionFrames);
    work->transitionActive = 0;
    WaitFrames(work->transitionFrames);
  }
  WaitFrames(1);
  Runtime_ReleaseHeapBlock(108);
  return result;
}
