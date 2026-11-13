/// @file
/// The field sequence: running a scene until the party leaves it, and its
/// entrance.
// Part of field_sequence.c.

#include "../field_sequence_internal.h"
#include "field_work.h"
#include "gamepak/far_call/field.h"
#include "gamepak/far_call/sound.h"
#include "gamepak/far_call/window.h"
#include "module/field/event_commands.h"
#include "module/field/field.h"
#include "module/field/field_sequence.h"
#include "module/field/object.h"
#include "module/tla.h"

extern u8 gMapCellBuffer[];

void BattleEffect_ApplyColorToTargetBuffer(s32 color, s32 mode);
s32 Field_GetSceneAttributes(s32 scene);
s32 PartyInventory_GiveItem(s32 item);
void Func_080d5720(void);
void Func_080d57e4(void);
void Func_080d59dc(void);
void Func_080d5aa0(void);

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

#define HAZARD (*(struct HazardView *)&gMainRoutineWork)
#define ARRIVAL (*(struct ArrivalView *)&gMainRoutineWork)

#define OVERLAY_CALL(index) ((s32 (*)(void))gOverlayArea[index])()

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
  Iwram_ClearWordsEntry(work, 0xd50);
  GameFlag_ClearBit(259);
  gMainRoutineWork.primary = gMainRoutineWork.scene;
  gMainRoutineWork.secondary = gMainRoutineWork.entrance;
  gMainRoutineWork.pair1ee[0] = -1;
  gMainRoutineWork.pair1ee[1] = -1;
  gMainRoutineWork.pair1f2[0] = -1;
  gMainRoutineWork.pair1f2[1] = -1;
  gMainRoutineWork.unk2c3 = 0;
  Task_ResetAll();
  Djinn_ResolvePending(0);
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
    Djinn_RestoreActorAfterBattle();
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
    Scene_SetRestartPoint();
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
        // (s16): the id compared as the field's own width; without it, 79 of
        // 1300 instructions differ in tla-en.
        if (ARRIVAL.scene != (s16)(s32)&SceneId_WorldMap)
          goto clearOwner;
        /* FAKEMATCH(barrier): an empty asm clobbering r0, so the entrance's
         * offset 0x2c0 is built afresh, not added to the scene's 0x2be in r0;
         * without it, 21 of 1300 instructions differ in tla-en. */
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
      Djinn_ResolvePending(gMainRoutineWork.pendingDjinnEvent);
#else
      ((void (*)(s32, s32))Djinn_ResolvePending)(gMainRoutineWork.pendingDjinnEvent,
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
    party->heading = leader->facing;
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
        Field_StartEncounterTransition(work->battle);
        work->unk194 = 0;
        work->battle = 0;
        *steps = 0;
      } else if (work->examine != 0) {
        AreaNameWindow_HideFar();
        talk = 0;
        if (gMainRoutineWork.ownerOverride == 0)
          value = Field_FindNearbyTalkTarget(*leaderId);
        else
          value = Field_FindNearbyTalkTarget(8);
        if (value != -1) {
          talk = 1;
          if ((s32)value <= 63)
            talk = Scene_FindActorTalkTrigger(value) != 0;
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
        AreaNameWindow_HideFar();
        Leader_StopMoving();
        Sound_PlayFar(111);
        Func_080cb82c();
        GameFlag_SetBit(262);
        if (gDebugMode && (*(volatile u32 *)&gInput.held & 2) != 0 &&
            ((*(volatile u32 *)&gInput.held & 4) != 0)) {
          DebugFlagEditor_RunFar();
        } else if (GameFlag_Test(263)) {
          work->raisedEvent = 250;
        } else {
          work->effectTimerRunning = 0;
          FieldMenu_RunFar();
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
        AreaNameWindow_HideFar();
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
        Field_UseItem(work->item, work->itemArgument);
        Func_080cb8a4();
        work->item = 0;
      } else if (work->start != 0) {
        Sound_PlayFar(111);
        Leader_StopMoving();
        Func_080cb82c();
        GameFlag_SetBit(262);
        if (*(volatile u8 *)&gDebugMode && (gInput.held & 2)) {
          DebugSceneSelect_RunFar();
        } else if (*(volatile u8 *)&gDebugMode && (gInput.held & 0x200)) {
          Debug_RunPaletteEditor();
        } else if (GameFlag_Test(263)) {
          work->raisedEvent = 250;
        } else {
          Event_ResolveAllPendingActions();
          AreaNameWindow_HideFar();
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
        // unk66 read signed (ldrsh), as its other readers read it; as
        // leader->unk66 == 0, 39 of 1300 instructions differ in tla-en.
        if (*pending == 0 ||
            (leader->timer == 0 && *(s16 *)&leader->unk66 == 0))
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

/* VIEW(field_sequence): tag struct Object as struct Object__field_sequence; module/tla.h has another definition. */
struct Object__field_sequence {
  u8 pad0[6];
  u16 angle;
  struct EffectVector pos;
  u8 pad1[14];
  u8 kind;
};

s32 Func_080cc54c(void) {
  u8 *workBytes;
  u8 *mapBytes;
  struct Object__field_sequence *leader;
  union PolarOffsetPosition probePosition;
  u8 *cell;
  s32 collisionCode;
  s32 terrainHeight;
  s32 result;

  result = 0;
  leader = Func_080cb8e8();
  workBytes = Ram_HeapSlots->eventWork;
  mapBytes = (u8 *)Ram_HeapSlots->mapWork;
  if (leader != 0) {
    probePosition.vector.x = leader->pos.x;
    probePosition.vector.y = leader->pos.y;
    probePosition.vector.z = leader->pos.z;
    {
      s32 *position = probePosition.coordinates;

      Vector_AddPolarOffset(0x100000, leader->angle, position);
    }
    if ((s8)workBytes[0x18a] == 3) {
      s32 x, z;

      x = probePosition.vector.x;
      z = probePosition.vector.z;
      cell = Ram_MapBlocks +
             ((((x / 0x200000) & 31) + (((z / 0x200000) & 31) << 5)) << 2);
    } else {
      s32 x, z;

      cell = *(u8 **)(mapBytes + 0x138);
      x = probePosition.vector.x;
      z = probePosition.vector.z;
      cell += (((x / 0x100000) + ((z / 0x100000) << 7)) << 2);
    }
    collisionCode = cell[2];
    if (collisionCode >= 242 && collisionCode <= 247) {
      terrainHeight = MapGrid_GetTerrainHeightFar(
          leader->kind, probePosition.vector.x, probePosition.vector.z);
      if (terrainHeight >= leader->pos.y &&
          terrainHeight <= leader->pos.y + 0x100000) {
        result = collisionCode;
      }
    } else if (Func_080ccd78(3, collisionCode) != 0) {
      result = collisionCode;
    }
    if (result == 0 && (s8)workBytes[0x18a] == 3) {
      if (gMainRoutineWork.ownerOverride != 0 &&
          GetWorldMapCollisionFar(probePosition.coordinates) == 3)
        result = 99;
    }
  }
  return result;
}

/// The twin of The Broken Seal's Scene_ApplyEntrance: places the party at
/// its entrance from the area overlay's entrance list and clamps the camera
/// to that entrance's bounds. With findEntrance it looks the entrance up and
/// returns 0 when there is none; without, it takes the first row.
s32 Func_080cc67c(s32 findEntrance) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 entrance = gMainRoutineWork.entrance;
  // The overlay table's entry 3 returns the scene's entrances.
  const struct SceneEntrance *entry =
      ((const struct SceneEntrance *(*)(void))gOverlayArea[3])();
  s32 found = 0;
  s32 left;

  if (findEntrance) {
    while (entry->entrance != -1) {
      if (entry->entrance == entrance &&
          (entry->requiredFlag == -1 || GameFlag_Test(entry->requiredFlag))) {
        found = 1;
        break;
      }
      entry++;
    }
    if (!found)
      return 0;
  }

  if (!GameFlag_Test(0x109)) {
    gMainRoutineWork.x = entry->x << 16;
    gMainRoutineWork.y = entry->y << 16;
    gMainRoutineWork.z = entry->z << 16;
    gMainRoutineWork.heading = entry->facing;
    gMainRoutineWork.turn = 0;
  }

  left = entry->cameraLeft;
  if (left != -1)
    map->minX = left << 16;
  if (entry->cameraTop != -1)
    map->minZ = entry->cameraTop << 16;
  if (entry->cameraRight != -1)
    map->maxX = entry->cameraRight << 16;
  if (entry->cameraBottom != -1)
    map->maxZ = entry->cameraBottom << 16;

  if (map->minX + (240 << 16) > map->maxX)
    map->minX = map->maxX - (240 << 16);
  if (map->minZ + (160 << 16) > map->maxZ)
    map->minZ = map->maxZ - (160 << 16);
  return 1;
}

/// Reads the map's entrance stream (gData_0202e000) for the current entrance,
/// places the party there unless flag 0x109 keeps its position, then sets the
/// camera bounds from the first box that holds it.
s32 Func_080cc7c4(void) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct MainRoutineWork *work = &gMainRoutineWork;
  s32 xFixed;
  u8 *cursor = gData_0202e000;
  s32 *angle;
  u16 *turn;
  s32 entrance = work->entrance;
  s32 height;
  s32 x;
  s32 z;
  s32 zFixed;
  s32 base;
  s32 flags;
  s32 left;
  s32 top;
  s32 bottom;
  s32 right;
  s32 leftEdge, topEdge, rightEdge, bottomEdge;

  cursor = gData_0202e000 + (u16)Stream_ReadU16(&cursor);
  angle = &Data_02000448;
  turn = &Data_0200044c;
  for (;; cursor += 6) {
    if (*(s8 *)cursor == -1) {
      *angle = 0x4000;
      {
        // A halfword constant through a plain pointer comes from the pool.
        s32 zero = 0;
        *turn = zero;
      }
      return 0;
    }
    if (*(s8 *)cursor == entrance) {
      /* FAKEMATCH(barrier): an empty asm reading work keeps it live across the
       * loop in r6, so entrance takes r7; without it, 14 of 211 instructions
       * differ in tla-en. */
      asm("" : : "r"(work));
      cursor++;
      x = Stream_ReadS16(&cursor);
      z = Stream_ReadS16(&cursor);
      xFixed = x << 16;
      zFixed = z << 16;
      height = MapGrid_GetTerrainHeightFar(0, xFixed, zFixed) >> 16;
      flags = *cursor;
      base = (s32)gData_0202e000;
      if (!GameFlag_Test(0x109)) {
        gMainRoutineWork.x = xFixed;
        gMainRoutineWork.z = zFixed;
        *angle = (flags & 15) << 12;
        *turn = 0;
        if (flags & 16)
          gMainRoutineWork.y = 0x80000;
      }
      if (flags & 32)
        return 1;

      cursor = Data_0202e004;
      cursor = (u8 *)((u16)Stream_ReadU16(&cursor) + base);
      while (*(s8 *)cursor != -1) {
        {
          left = *(s8 *)cursor;
          leftEdge = left << 4;
          cursor++;
          top = *(s8 *)cursor;
          topEdge = top << 4;
          cursor++;
          right = *(s8 *)cursor;
          rightEdge = right << 4;
          cursor++;
          bottom = *(s8 *)cursor;
          bottomEdge = bottom << 4;
          cursor++;
          if (x >= leftEdge && x <= rightEdge && z - height >= topEdge &&
              z - height <= bottomEdge) {
            map->minX = left << 20;
            map->minZ = top << 20;
            map->maxX = right << 20;
            map->maxZ = bottom << 20;
            if (map->minX + (240 << 16) > map->maxX)
              map->minX = map->maxX - (240 << 16);
            if (map->minZ + (160 << 16) > map->maxZ)
              map->minZ = map->maxZ - (160 << 16);
            break;
          }
        }
      }
      return 1;
    }
  }
}

u32 Stream_ReadU16(u8 **cursor) {
  u32 low = *(*cursor)++;
  u32 high = *(*cursor)++;

  return low | (high << 8);
}

s16 Stream_ReadS16(u8 **cursor) {
  u32 value;

  value = *(*cursor)++;
  value |= *(*cursor)++ << 8;
  return value;
}

void BattleMap_ApplySceneEntranceView(void) {
  struct EventWork *event = Ram_HeapSlots->eventWork;
  s32 done;

  done = Func_080cc67c(1);
  if (event->controlMode == 2 && done == 0)
    done = Func_080cc7c4();
  if (done == 0)
    Func_080cc67c(0);
}
