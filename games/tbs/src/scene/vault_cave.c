#include "game_state.h"
#include "engine_call.h"
#include "scene_ids.h"
#include "unit/import.h"
#include "unit/scene/vault_cave.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/runtime/runtime.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "global_cells.h"
#include "types.h"
#include "text/msg_ids.h"
#include "field_event.h"
#include "field_scene.h"
#include "callback_scheduler.h"
#include "call.h"
#include "object_runtime.h"
#include "ability_ids.h"
#include "objcmd.h"

// 前方宣言
static void SceneState_ApplyThreeRects(void);
static void VaultCave_RaiseActorPriorities(void);
static void VaultCave_EnableGroupAutomaticPriority(void);
static void VaultCave_SpawnPuffs(struct FieldActor *source);

s32 Math_RemainderUnsigned();

struct MapShake {
  u8 unk00[0x18];
  s32 directionX;
  s32 directionZ;
  u8 unk20[4];
  s32 scroll;
};

void QueueIoWriteDelay2(u32 address, s32 value);

#define TILE(position) ((position) / 0x100000)

const struct SceneEntrance *Scene_GetEntrances(void) {
  s16 scene = gGameState.scene;

  if (scene == (s32)&SceneId_VaultCave1) {
    return gVaultCaveEntrances1;
  }
  if (scene == (s32)&SceneId_VaultCave2) {
    return gVaultCaveEntrances2;
  }
  if (scene == (s32)&SceneId_VaultCave3) {
    return gVaultCaveEntrances3;
  }
  return gVaultCaveEntrancesOther;
}

u8 *VaultCave_GetRegions(void) { return (u8 *)gVaultCaveRegions; }

const u32 *Scene_GetExits(void) {
  s16 scene = gGameState.scene;

  if (scene == (s32)&SceneId_VaultCave1) {
    return gVaultCaveExits1;
  }
  if (scene == (s32)&SceneId_VaultCave2) {
    return gVaultCaveExits2;
  }
  if (scene == (s32)&SceneId_VaultCave3) {
    return gVaultCaveExits3;
  }
  return gVaultCaveExitsOther;
}

const struct ScenePlacement *Scene_GetPlacements(void) {
  s16 scene = gGameState.scene;

  if (scene == (s32)&SceneId_VaultCave1) {
    return gVaultCavePlacements1;
  }
  if (scene == (s32)&SceneId_VaultCave2) {
    return gVaultCavePlacements2;
  }
  if (scene == (s32)&SceneId_VaultCave3) {
    return gVaultCavePlacements3;
  }
  return gVaultCavePlacementsOther;
}

static s32 *SceneData_FindActiveSlotAtCell(s32 cx, s32 cz) {
  s32 **slots = (s32 **)((u8 *)gEventWork + 0x14);
  u32 i;

  for (i = 8; i <= 65; i++) {
    s32 *p = slots[i];

    if (cx == (p[2] >> 20) && cz == (p[4] >> 20) && *((u8 *)p + 0x59) != 0) {
      return p;
    }
  }
  return 0;
}

struct DirectionSteps {
  s32 xz[16];
};

#define DIRECTION_STEP(heading)                                                \
  (((struct DirectionSteps *)gVaultCave_DirectionSteps)->xz[heading])

static void VaultCave_PushBlockAhead(void) {
  /* FAKEMATCH: DIRECTION_STEP reads the s16-pair table (x high, z low) as one
   * word per heading through an array reference, as native; through a plain s32
   * pointer cast the address is summed once into a register and no longer
   * matches, the table as const s32 words indexed directly differs 79 (143
   * instructions for 146), a const s32 local pointer 94. */
  struct FieldActor *leader = Object_GetById(0);
  s32 direction = leader->facing >> 12;
  struct FieldActor *block;
  struct FieldActor *beyond;
  union FieldCoordinate pos[3];
  s32 i;
  s32 cx;
  s32 cz;

  cx = (leader->x.part.pixel + (DIRECTION_STEP(direction) >> 16)) >> 4;
  cz = (leader->z.part.pixel + (s16)DIRECTION_STEP(direction)) >> 4;
  block = (struct FieldActor *)SceneData_FindActiveSlotAtCell(cx, cz);
  if (block->collisionFlags == 0 || block == 0) {
    return;
  }
  for (i = 0; i <= 3; i++) {
    if (block == Object_GetById(i + 11)) {
      return;
    }
  }
  cx = (block->x.part.pixel + (DIRECTION_STEP(direction) >> 16)) >> 4;
  cz = (block->z.part.pixel + (s16)DIRECTION_STEP(direction)) >> 4;
  beyond = (struct FieldActor *)SceneData_FindActiveSlotAtCell(cx, cz);
  if (beyond != 0 && (beyond->collisionFlags & 1)) {
    return;
  }
  block->terrainId = 2;
  pos[0].fixed = block->x.fixed + (DIRECTION_STEP(direction) & 0xffff0000);
  pos[1].fixed = block->y.fixed;
  pos[2].fixed = block->z.fixed + (DIRECTION_STEP(direction) << 16);
  if (FieldObject_CheckMovementCollision(block, pos) > 0) {
    return;
  }
  Object_SetMode(leader, 8);
  Engine_TaskWait(15);
  Engine_AudioPlayCue(238);
  block->speed = 0x3333;
  block->acceleration = 0x3333;
  Engine_ObjectSetPosition(block, pos[0].fixed, pos[1].fixed, pos[2].fixed);
  leader->speed = 0x3333;
  leader->acceleration = 0x3333;
  Engine_ObjectSetPosition(leader, pos[0].fixed, pos[1].fixed, pos[2].fixed);
  Engine_ObjectCommitPosition(block);
  Engine_AudioPlayCue(288);
  block->x.fixed = pos[0].fixed;
  block->z.fixed = pos[2].fixed;
  block->velocityX = 0;
  block->velocityZ = 0;
  leader->velocityX = 0;
  leader->velocityZ = 0;
  leader->targetX = ACTOR_NO_TARGET;
  leader->targetZ = ACTOR_NO_TARGET;
  Object_SetMode(leader, 1);
}

static void EV22_PSYNERGY(void) { Engine_GameFlagSet(0x300); }

static void EV99_SEARCH(void) {
  Engine_MessageShowCentered(MSGNO(MsgFieldDoorTightlyLocked), 1);
}

const struct SceneEvent *Scene_GetEvents(void) {
  s16 scene = gGameState.scene;

  if (scene == (s32)&SceneId_VaultCave1) {
    return gVaultCaveEvents1;
  }
  if (scene == (s32)&SceneId_VaultCave2) {
    return gVaultCaveEvents2;
  }
  if (scene == (s32)&SceneId_VaultCave3) {
    return gVaultCaveEvents3;
  }
  return gVaultCaveEventsOther;
}

static s32 IsActor9AtTile15x54(void) {
  struct FieldActor *actor = Object_GetById(9);
  s32 z = actor->z.fixed;
  s32 x;
  s32 z_tile;
  s32 x_tile;

  if (z < 0) {
    z += 0x000FFFFF;
  }
  x = actor->x.fixed;
  z_tile = z >> 20;
  if (x < 0) {
    x += 0x000FFFFF;
  }
  x_tile = x >> 20;
  if (x_tile == 15 && z_tile == 54) {
    return 1;
  }
  return 0;
}

static void EV60_SCRIPT(void) {
  if (GameFlag_IsSet(0x9a9) == 0) {
    VaultCave_PushBlockAhead();
    if (IsActor9AtTile15x54() != 0) {
      GameFlag_Set(0x9a9);
      Event_PlayCue(80);
      SceneState_ApplyThreeRects();
    }
  }
}

static void SceneState_ApplyThreeRects(void) {

  {

    Map_CopyCells(87, 50, 2, 4, 80, 50);
  }
  Map_CopyCells(23, 52, 1, 2, 16, 52);
  Map_CopyCellAttributes(16, 52, 1, 1, 16, 53);
}

static void EV09_PSYNERGY_2(void) {
  if (GameFlag_IsSet(0x9a9) == 0) {
    if (IsActor9AtTile15x54() != 0) {
      GameFlag_Set(0x9a9);
      Event_PlayCue(80);
      SceneState_ApplyThreeRects();
    }
  }
}

static void FieldScene_NoOp(void) {}

static void SceneState_ApplyThreeRectsRows9And10(void) {

  {

    Map_CopyCells(90, 9, 2, 3, 80, 9);
  }
  Map_CopyCells(27, 10, 1, 2, 17, 10);
  Map_CopyCellAttributes(17, 10, 1, 1, 17, 11);
}

static s32 SceneActor_IsActor10AtTile16x12(void) {
  struct FieldActor *p = Object_GetById(10);
  s32 z = p->z.fixed;
  s32 x;
  s32 cz;
  s32 cx;

  if (z < 0) {
    z += 0x000FFFFF;
  }
  x = p->x.fixed;
  cz = z >> 20;
  if (x < 0) {
    x += 0x000FFFFF;
  }
  cx = x >> 20;
  if (cx == 16 && cz == 12) {
    return 1;
  }
  return 0;
}

static void EV50_SCRIPT(void) {
  VaultCave_PushBlockAhead();
  if (GameFlag_IsSet(0x9aa) == 0) {
    if (SceneActor_IsActor10AtTile16x12() != 0) {
      if (GameFlag_IsSet(0x207) == 0) {
        Event_PlayCue(80);
        SceneState_ApplyThreeRectsRows9And10();
        GameFlag_Set(0x9aa);
      }
    }
  }
}

static void EV09_PSYNERGY_3(void) {}

static void EV10_PSYNERGY(void) {
  if (GameFlag_IsSet(0x9aa) == 0) {
    if (SceneActor_IsActor10AtTile16x12() != 0) {
      if (GameFlag_IsSet(0x207) == 0) {
        Event_PlayCue(80);
        SceneState_ApplyThreeRectsRows9And10();
        GameFlag_Set(0x9aa);
      }
    }
  }
}

static void EV16_PSYNERGY(void) {
  struct FieldActor *rec = Object_GetById(16);

  Map_CopyCellAttributes(26, 30, 1, 1, 23, 32);

  if (rec != 0) {

    Object_GetById(16)->motionFlags = 0;
    rec->priorityFlags = 1;
  }

  Engine_GameFlagSet(0x200);
}

static void EV17_PSYNERGY(void) {
  struct FieldActor *rec = Object_GetById(17);

  Map_CopyCellAttributes(26, 30, 1, 1, 23, 34);

  if (rec != 0) {

    Object_GetById(17)->motionFlags = 0;
    rec->priorityFlags = 1;
  }

  Engine_GameFlagSet(0x201);
}

static void EV18_PSYNERGY(void) {
  struct FieldActor *record = Object_GetById(18);

  Map_CopyCellAttributes(26, 30, 1, 1, 24, 34);

  if (record != 0) {

    Object_GetById(18)->motionFlags = 0;
    record->priorityFlags = 1;
  }

  Engine_GameFlagSet(0x202);
}

static void EV19_PSYNERGY(void) {
  struct FieldActor *p = Object_GetById(19);

  Map_CopyCellAttributes(26, 30, 1, 1, 26, 32);

  if (p != 0) {
    Engine_ActorSetSpriteFlags(p, 0);

    Object_GetById(19)->motionFlags = 0;
    p->priorityFlags = 1;
  }

  Engine_GameFlagSet(0x203);
}

static void EV20_PSYNERGY(void) {
  struct FieldActor *rec = Object_GetById(20);

  Map_CopyCellAttributes(26, 30, 1, 1, 26, 34);

  if (rec != 0) {
    Engine_ActorSetSpriteFlags(rec, 0);

    Object_GetById(20)->motionFlags = 0;
    rec->priorityFlags = 1;
  }

  Engine_GameFlagSet(0x204);
}

static void EV21_PSYNERGY(void) {
  struct FieldActor *record = Object_GetById(21);

  Map_CopyCellAttributes(26, 30, 1, 1, 28, 33);

  if (record != 0) {
    Engine_ActorSetSpriteFlags(record, 0);

    Object_GetById(21)->motionFlags = 0;
    record->priorityFlags = 1;
  }

  Engine_GameFlagSet(0x205);
}

static void SceneState_DispatchByActorZeroDepth(void) {
  struct FieldActor *p = Object_GetById(ACTOR_PARTY_LEADER);

  if (p->y.fixed >= 0x100000) {
    VaultCave_RaiseActorPriorities();
  } else {
    VaultCave_EnableGroupAutomaticPriority();
  }
}

static void VaultCave_RaiseActorPriorities(void) {
  s32 i;
  s32 id;

  id = 16;
  for (i = 0; i < 6; i++) {
    Object_GetById(id)->priorityFlags |= 2;
    id++;
  }
}

static void VaultCave_EnableGroupAutomaticPriority(void) {
  s32 index = 16;
  s32 flag = 1;
  s32 remaining = 5;

  do {
    struct FieldActor *entry = Object_GetById(index);

    remaining--;
    entry->priorityFlags = flag;
    index++;
  } while (remaining >= 0);
}

static void VaultCave_RunRumble(void) {
  struct MapShake *shake = (struct MapShake *)(((u8 *)gMapWork[0]) + 0x164);
  s32 frames;
  s32 eva;
  s32 evb;
  s32 top;

  Engine_EventBegin();
  if (gFrameCount & 1) {
    shake->directionX = 1;
    shake->directionZ = 1;
  } else {
    shake->directionX = -1;
    shake->directionZ = -1;
  }
  Work_SetValuesIfNonNegative(0x30000, 0x30000, 0x10000);
  Work_SetValuesIfNonNegative(-1, -1, 0xe666);
  Event_PlayCue(163);
  for (frames = 0x1df; frames >= 0; frames--) {
    u32 drift = (u32)(Random_Next() << 11) >> 16;

    shake->scroll = (double)shake->scroll - (4718.592 - (double)drift);
    Engine_EventWait(1);
  }
  eva = 6;
  evb = 6;
  frames = 0;
  top = eva << 10;
  do {
    QueueIoWriteDelay2(0x4000052, top | (eva << 5) | evb);
    Engine_EventWait(1);
    if (frames % 20 == 0) {
      evb--;
      eva--;
    }
    frames++;
  } while (frames <= 69);
  Map_CopyCells(19, 83, 15, 8, 19, 91);
  Event_PlayCue(0x120);
  Engine_MapRedraw();
  Engine_MapRenderWaitForValues();
  Engine_EventEnd();
}

static void EV20_SEARCH(void) {
  s32 v5;
  s32 v6;

  if (GameFlag_IsSet(0x9a8) == 0) {
    Engine_MessageShowCentered(MSGNO(MsgFieldFlippedSwitch), 1);
    GameFlag_Set(0x9a8);
    v5 = 27;
    v6 = 92;
    Event_PlayCue(155);
    Map_CopyCells(107, 27, 1, 1, v6, v5);
    Engine_EventWait(39);
    Map_CopyCells(108, 27, 1, 1, v6, v5);
    Engine_EventWait(50);
    v6 = 25;
    Event_PlayCue(156);
    Map_CopyCells(1, 24, 1, 2, v6, v5);
    Engine_EventWait(40);
    Map_CopyCells(2, 24, 1, 2, v6, v5);
    Engine_EventWait(40);
    VaultCave_RunRumble();
  }
}

static s32 SceneActor_LiftLowActorOnSubjectTile(s32 subjectActor) {
  struct FieldActor *subject = Object_GetById(subjectActor);
  s32 index = 0;

  do {
    struct FieldActor *actor = Object_GetById(index + 11);

    if ((u32)(actor->y.fixed - 1) <= 0x000ffffe) {
      s32 actorZ = TILE(actor->z.fixed);
      s32 actorX = TILE(actor->x.fixed);
      s32 subjectZ = TILE(subject->z.fixed);
      s32 subjectX = TILE(subject->x.fixed);
      s32 zDelta = subjectZ - actorZ;
      s32 xDelta = subjectX - actorX;

      if (xDelta == 0 && zDelta == 0) {
        actor->y.fixed = 0x00ff0000;
        actor->gravity = 0;
        actor->velocityY = 0;
        return 1;
      }
    }

    index++;
  } while (index <= 3);

  return 0;
}

static void FieldScene_RunOpeningAuxiliarySequence(void) {
  s32 i;
  struct FieldActor *rec7;
  s32 flag;
  s32 index;

  flag = Object_GetById(10)->paused;
  if (flag == 0) {
    if (++gVaultCave_ScheduleTimer > 190) {
      gVaultCave_ScheduleTimer = 0;
    }
    index = gVaultCave_ScheduleIndex;
    if (gVaultCave_ScheduleFrames[index] == gVaultCave_ScheduleTimer) {
      rec7 = Actor_Get((index + 11));
      rec7->gravity = 0xa3d;
      if (++gVaultCave_ScheduleIndex > 3) {
        gVaultCave_ScheduleIndex = 0;
      }
    }
    for (i = 0; i <= 3; i++) {
      rec7 = Actor_Get((i + 11));
      if (rec7->velocityY >= 0) {
        if (rec7->y.fixed <= 0xffff) {
          VaultCave_SpawnPuffs(rec7);
          rec7->y.fixed = 0xff0000;
          rec7->gravity = 0;
          rec7->velocityY = 0;
          rec7->paused = 0;
          Event_PlayCue(106);
        }
      }
    }
    if (SceneActor_LiftLowActorOnSubjectTile(10) != 0) {
      Engine_ActorSetAnimation(10, 1);
      if (GameFlag_IsSet(0x207) == 0) {
        GameFlag_Set(0x207);
        Event_PlayCue(204);
      } else {
        Event_PlayCue(106);
      }
    }
    if (SceneActor_LiftLowActorOnSubjectTile(9) != 0) {
      Event_PlayCue(106);
    }
  }
}

static void FieldScene_RunScene3a7SequenceD(void) {

  s32 i;
  struct FieldActor *rec7;
  s32 record;
  s32 *selected;

  rec7 = Object_GetById(10);
  if (rec7->paused == 0) {
    if ((++gVaultCave_TickCounter & 63) == 0) {
      selected = &gVaultCave_TickValue;
      record = Engine_RandomNext();
      record = Math_RemainderUnsigned(record, 6);
      *selected = record;
      rec7 = Object_GetById((record + 10));
      rec7->gravity = 0xa3d;
    }
    for (i = 0; i <= 5; i++) {
      rec7 = Object_GetById((i + 10));
      record = Engine_GameFlagIsSet((i + 0x200));
      if (record != 0) {
        if (rec7->velocityY <= 0) {
          if (rec7->y.fixed > 0x20ffff) {
            continue;
          }
        }
        rec7->y.fixed = 0xff0000;
        rec7->gravity = 0;
        rec7->velocityY = 0;
        Engine_AudioPlayCue(106);
      } else {
        if (rec7->velocityY <= 0) {
          if (rec7->y.fixed > 0xffff) {
            continue;
          }
        }
        rec7->gravity = record;
        rec7->velocityY = record;
        rec7->y.fixed = 0xff0000;
        Engine_AudioPlayCue(106);
      }
    }
  }
}

static void SceneActor_TransformAndApplyRecordPosition(s32 *rec, s32 v0,
                                                       s32 v1) {
  s32 pos[3];

  if (rec == 0) {
    return;
  }
  pos[0] = rec[2];
  pos[1] = rec[3];
  pos[2] = rec[4];
  Vector_AddPolarOffset(v0, v1, pos);
  Engine_ObjectSetPosition(rec, pos[0], pos[1], pos[2]);
}

static void VaultCave_SpawnPuffs(struct FieldActor *source) {
  struct FieldActor *puff;
  s32 i;

  for (i = 0; i < 4; i++) {
    puff = Engine_ObjectCreate(240, source->x.fixed, source->y.fixed,
                               source->z.fixed);
    if (puff == 0)
      break;
    puff->scaleY = 0x8ccc;
    puff->scaleX = 0x8ccc;
    puff->motionFlags = 2;
    puff->velocityY = -0x10000;
    puff->speed = Engine_RandomNext() + 0xcccc;
    puff->collisionFlags = 1;
    SceneActor_TransformAndApplyRecordPosition(puff, 0x200000,
                                               Engine_RandomNext());
    {
      u16 *timer = (u16 *)&puff->unk5d[1];
      s32 frames = 8;

      *timer = frames;
    }
    Engine_ObjectSetScript(puff, gVaultCave_PuffScript);
  }
}

static void SceneActor_InitSlots10To15AndStartTask(void) {
  s32 selector = 10;
  s32 remaining = 5;

  do {
    struct FieldActor *record;

    Engine_ActorSetSpriteFlags(Object_GetById(selector), 0);
    record = Object_GetById(selector);
    record->bounce = 0x1999;
    record->gravity = 0;
    remaining--;
    record->y.fixed = 0x00ff0000;
    selector++;
  } while (remaining >= 0);

  FieldScheduler_AddCallback((s32)FieldScene_RunScene3a7SequenceD,
                             TASK_PRIORITY_SCENE);
}

static void SceneActor_SetupActors11To14AndInstallTask(void) {
  s32 no = 11;
  s32 i = 0;

  do {
    struct FieldActor *rec;

    Engine_ActorSetSpriteFlags(Object_GetById(no), 0);
    rec = Object_GetById(no);
    rec->bounce = 0x1999;
    rec->gravity = 0;
    rec->y.fixed = 0x00ff0000;
    Engine_ActorSetSpritePriority(i + 11, 1);
    i++;
    no++;
  } while (i <= 3);

  FieldScheduler_AddCallback((s32)FieldScene_RunOpeningAuxiliarySequence,
                             TASK_PRIORITY_SCENE);
}

s32 VaultCave_ApplyEntryState(void) {
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  if (gGameState.scene == (s32)&SceneId_VaultCave1) {
    switch (gGameState.entrance) {
    case 5:
    case 6:
    case 7:
    case 8:
    case 13:
      if (Engine_GameFlagIsSet(0x9a8) == 0) {
        Map_CopyCellAttributes(22, 29, 1, 1, 21, 29);
      } else {
        Map_CopyCells(108, 27, 1, 1, 92, 27);
        Engine_TaskWait(1);
        Map_CopyCells(19, 83, 15, 8, 19, 91);
        Engine_TaskWait(1);
        Map_CopyCells(2, 24, 1, 2, 25, 27);
      }
      Engine_MapRedraw();
      Engine_TaskWait(1);
      break;
    case 10:
      Engine_GameFlagClear(0x9a8);
      break;
    }
  }
  if (gGameState.scene == (s32)&SceneId_VaultCave2) {
    if (Engine_GameFlagIsSet(0x300) == 0) {
      Object_GetById(22)->scaleY = 0x18000;
    }
    switch (gGameState.entrance) {
    case 1:
    case 2:
    case 3:
    case 4:
      if (Engine_GameFlagIsSet(0x9a8) == 0) {
        Map_CopyCells(5, 81, 11, 7, 5, 73);
      } else {
        Map_CopyCellAttributes(5, 12, 1, 1, 6, 12);
        Map_CopyCellAttributes(12, 10, 1, 1, 12, 11);
      }
      break;
    case 8:
    case 9:
    case 14:
      SceneActor_InitSlots10To15AndStartTask();
      if (Engine_GameFlagIsSet(0x200) != 0) {
        Engine_ActorSetAnimation(16, 5);
        EV16_PSYNERGY();
      }
      if (Engine_GameFlagIsSet(0x201) != 0) {
        Engine_ActorSetAnimation(17, 5);
        EV17_PSYNERGY();
      }
      if (Engine_GameFlagIsSet(0x202) != 0) {
        Engine_ActorSetAnimation(18, 5);
        EV18_PSYNERGY();
      }
      if (Engine_GameFlagIsSet(0x203) != 0) {
        Engine_ActorSetAnimation(19, 5);
        EV19_PSYNERGY();
      }
      if (Engine_GameFlagIsSet(0x204) != 0) {
        Engine_ActorSetAnimation(20, 5);
        EV20_PSYNERGY();
      }
      if (Engine_GameFlagIsSet(0x205) != 0) {
        Engine_ActorSetAnimation(21, 5);
        EV21_PSYNERGY();
      }
      FieldScheduler_AddCallback((s32)SceneState_DispatchByActorZeroDepth,
                                 TASK_PRIORITY_SCENE);
      break;
    case 10:
    case 11:
      if (Engine_GameFlagIsSet(0x9a9) != 0) {
        SceneState_ApplyThreeRects();
        Actor_SetPosition(9, 0xf80000, 0x36c0000);
      }
      Object_GetById(8)->priorityFlags = 2;
      break;
    }
    Engine_ActorSetAnimation(8, 2);
    Engine_ActorSetAnimation(9, 2);
    Engine_ActorSetSpriteFlags(Object_GetById(8), 0);
    Engine_ActorSetSpriteFlags(Object_GetById(9), 0);
    Object_GetById(9)->collisionFlags = 1;
  }
  if (gGameState.scene == (s32)&SceneId_VaultCave3) {
    Engine_ActorSetAnimation(8, 2);
    if (Engine_GameFlagIsSet(0x207) == 0) {
      Engine_ActorSetAnimation(10, 2);
    }
    Engine_ActorSetSpriteFlags(Object_GetById(8), 0);
    Engine_ActorSetSpriteFlags(Object_GetById(10), 0);
    Engine_ActorSetSpriteFlags(Object_GetById(9), 0);
    Object_GetById(10)->collisionFlags |= 0x80;
    Object_GetById(9)->collisionFlags |= 0x80;
    switch (gGameState.entrance) {
    case 5:
    case 6:
      SceneActor_SetupActors11To14AndInstallTask();
      Object_GetById(11)->collisionFlags = 2;
      Object_GetById(12)->collisionFlags = 2;
      Object_GetById(13)->collisionFlags = 2;
      Object_GetById(14)->collisionFlags = 2;
      Object_GetById(8)->collisionFlags = 1;
      Object_GetById(10)->collisionFlags = 1;
      Object_GetById(9)->collisionFlags = 1;
      if (Engine_GameFlagIsSet(0x9aa) != 0) {
        SceneState_ApplyThreeRectsRows9And10();
        Actor_SetPosition(10, 0x1080000, 0xcc0000);
      }
      break;
    }
  }
  gGameState.retreatEntrance = 10;
  gGameState.retreatScene = (s32)&SceneId_VaultCave1;
  return 0;
}

// clang-format off
const s16 gVaultCave_DirectionSteps[] = {
    0, 16,
    0, 16,
    0, 16,
    16, 0,
    16, 0,
    16, 0,
    16, 0,
    0, -16,
    0, -16,
    0, -16,
    0, -16,
    -16, 0,
    -16, 0,
    -16, 0,
    -16, 0,
    0, 16,
};

const s32 gVaultCave_PuffScript[] = {
    SC_FREE,
};

struct SceneEntrance gVaultCaveEntrancesOther[] = {
    { 0, CONDITION_ALWAYS, 120, 0, 152, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gVaultCaveEntrances1[] = {
    { 0, CONDITION_ALWAYS, 120, 0, 152, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { 1, CONDITION_ALWAYS, 424, 0, 216, FACING_SOUTH, 0, 72, 8, 480, 320, 0 },
    { 2, CONDITION_ALWAYS, 136, 0, 104, FACING_SOUTH, 0, 72, 8, 480, 320, 0 },
    { 3, CONDITION_ALWAYS, 120, 0, 256, FACING_NORTH, 0, 72, 8, 480, 320, 0 },
    { 4, CONDITION_ALWAYS, 184, 0, 264, FACING_NORTH, 0, 72, 8, 480, 320, 0 },
    { 5, CONDITION_ALWAYS, 344, 0, 464, FACING_SOUTH, 0, 280, 360, 640, 640, 0 },
    { 6, CONDITION_ALWAYS, 488, 0, 488, FACING_SOUTH, 0, 280, 360, 640, 640, 0 },
    { 7, CONDITION_ALWAYS, 584, 0, 608, FACING_SOUTH, 0, 280, 360, 640, 640, 0 },
    { 8, CONDITION_ALWAYS, 344, 0, 632, FACING_NORTH, 0, 280, 360, 640, 640, 0 },
    { 13, CONDITION_ALWAYS, 408, 0, 472, FACING_SOUTH, 0, 280, 360, 640, 640, 0 },
    { 9, CONDITION_ALWAYS, 88, 0, 704, FACING_SOUTH, 0, 40, 632, 352, 912, 0 },
    { 12, CONDITION_ALWAYS, 184, 0, 864, FACING_SOUTH, 0, 40, 632, 352, 912, 0 },
    { 10, CONDITION_ALWAYS, 456, 0, 800, FACING_SOUTH, 0, 336, 696, 576, 856, 0 },
    { 11, CONDITION_ALWAYS, 456, 0, 840, FACING_NORTH, 0, 336, 696, 576, 856, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gVaultCaveEntrances2[] = {
    { 0, CONDITION_ALWAYS, 120, 0, 152, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { 1, CONDITION_ALWAYS, 136, 0, 104, FACING_SOUTH, 0, 40, 8, 400, 328, 0 },
    { 2, CONDITION_ALWAYS, 344, 0, 152, FACING_SOUTH, 0, 40, 8, 400, 328, 0 },
    { 3, CONDITION_ALWAYS, 344, 0, 184, FACING_NORTH, 0, 40, 8, 400, 328, 0 },
    { 4, CONDITION_ALWAYS, 168, 0, 328, FACING_NORTH, 0, 40, 8, 400, 328, 0 },
    { 5, CONDITION_ALWAYS, 56, 0, 480, FACING_SOUTH, 0, 8, 408, 248, 656, 0 },
    { 6, CONDITION_ALWAYS, 152, 0, 472, FACING_SOUTH, 0, 8, 408, 248, 656, 0 },
    { 7, CONDITION_ALWAYS, 120, 0, 576, FACING_SOUTH, 0, 8, 408, 248, 656, 0 },
    { 8, CONDITION_ALWAYS, 344, 0, 456, FACING_SOUTH, 0, 280, 344, 544, 672, 0 },
    { 9, CONDITION_ALWAYS, 424, 0, 664, FACING_NORTH, 0, 280, 344, 544, 672, 0 },
    { 14, CONDITION_ALWAYS, 456, 0, 472, FACING_SOUTH, 0, 280, 344, 544, 672, 0 },
    { 10, CONDITION_ALWAYS, 200, 0, 776, FACING_SOUTH, 0, 104, 696, 344, 944, 0 },
    { 11, CONDITION_ALWAYS, 264, 0, 808, FACING_SOUTH, 0, 104, 696, 344, 944, 0 },
    { 13, CONDITION_ALWAYS, 536, 0, 168, FACING_NORTH, 0, 416, 24, 656, 184, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gVaultCaveEntrances3[] = {
    { 0, CONDITION_ALWAYS, 120, 0, 152, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { 1, CONDITION_ALWAYS, 312, 0, 664, FACING_NORTH, 0, 200, 520, 440, 680, 0 },
    { 2, CONDITION_ALWAYS, 312, 0, 600, FACING_SOUTH, 0, 200, 520, 440, 680, 0 },
    { 3, CONDITION_ALWAYS, 88, 0, 568, FACING_SOUTH, 0, 8, 472, 248, 632, 0 },
    { 4, CONDITION_ALWAYS, 168, 0, 544, FACING_SOUTH, 0, 8, 472, 248, 632, 0 },
    { 5, CONDITION_ALWAYS, 248, 0, 336, FACING_NORTH, 0, 128, 40, 376, 384, 0 },
    { 6, CONDITION_ALWAYS, 280, 0, 104, FACING_SOUTH, 0, 128, 40, 376, 384, 0 },
    { SCENE_TABLE_END },
};

s16 gVaultCaveRegions[] = {
    447, 47, 783, 463, 63, 799, -1, 10,
    -1, 0, 0, 0, 0, 0, 0, 0,
};

u32 gVaultCaveExitsOther[] = {
    SCENE_EXITS(511),
};

u32 gVaultCaveExits1[] = {
    SCENE_EXITS(96),
    SCENE_EXIT(1, 98, 2),
    SCENE_EXIT(2, 97, 1),
    SCENE_EXIT(3, 96, 5),
    SCENE_EXIT(4, 96, 13),
    SCENE_EXIT(5, 96, 3),
    SCENE_EXIT(6, 97, 6),
    SCENE_EXIT(7, 97, 10),
    SCENE_EXIT(8, 96, 9),
    SCENE_EXIT(9, 96, 8),
    SCENE_EXIT(10, 23, 13),
    SCENE_EXIT(10, 96, 11),
    SCENE_EXIT(11, 96, 12),
    SCENE_EXIT(12, 96, 11),
    SCENE_EXIT(13, 96, 4),
    SCENE_EXITS_END,
};

u32 gVaultCaveExits2[] = {
    SCENE_EXITS(97),
    SCENE_EXIT(1, 96, 2),
    SCENE_EXIT(2, 98, 6),
    SCENE_EXIT(3, 97, 8),
    SCENE_EXIT(4, 97, 5),
    SCENE_EXIT(5, 97, 4),
    SCENE_EXIT(6, 96, 6),
    SCENE_EXIT(7, 98, 3),
    SCENE_EXIT(8, 97, 3),
    SCENE_EXIT(9, 97, 11),
    SCENE_EXIT(10, 96, 7),
    SCENE_EXIT(11, 97, 9),
    SCENE_EXIT(13, 97, 13),
    SCENE_EXIT(14, 97, 14),
    SCENE_EXITS_END,
};

u32 gVaultCaveExits3[] = {
    SCENE_EXITS(98),
    SCENE_EXIT(1, 23, 14),
    SCENE_EXIT(2, 96, 1),
    SCENE_EXIT(3, 97, 7),
    SCENE_EXIT(4, 98, 5),
    SCENE_EXIT(5, 98, 4),
    SCENE_EXIT(6, 97, 2),
    SCENE_EXITS_END,
};

struct ScenePlacement gVaultCavePlacementsOther[] = {
    { SCENE_TABLE_END },
};

struct ScenePlacement gVaultCavePlacements1[] = {
    { SCENE_TABLE_END },
};

struct ScenePlacement gVaultCavePlacements2[] = {
    { 0x12b, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(280), 0, PIXELS(876), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x12b, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(184), 0, PIXELS(892), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xf0, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(372), 0, PIXELS(512), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xf0, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(372), 0, PIXELS(544), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xf0, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(388), 0, PIXELS(544), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xf0, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(420), 0, PIXELS(512), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xf0, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(420), 0, PIXELS(544), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xf0, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(452), 0, PIXELS(528), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xe3, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(376), 0, PIXELS(520), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xe3, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(376), 0, PIXELS(552), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xe3, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(392), 0, PIXELS(552), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xe3, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(424), 0, PIXELS(520), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xe3, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(424), 0, PIXELS(552), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xe3, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(456), 0, PIXELS(536), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xf8, 0x300, ACTOR_STAND,
        PIXELS(456), 0, PIXELS(456), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gVaultCavePlacements3[] = {
    { 0x12b, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(296), 0, PIXELS(204), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x12b, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(264), 0, PIXELS(236), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x12b, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(264), 0, PIXELS(300), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xf0, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(248), 0, PIXELS(232), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xf0, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(264), 0, PIXELS(248), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xf0, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(264), 0, PIXELS(264), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xf0, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(280), 0, PIXELS(280), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct SceneEvent gVaultCaveEventsOther[] = {
    { SCENE_EVENTS_END },
};

struct SceneEvent gVaultCaveEvents1[] = {
    { 0x21, 1, CONDITION_ALWAYS, 1 },
    { 0x31, 2, CONDITION_ALWAYS, 2 },
    { EVENT_EXIT, 3, CONDITION_ALWAYS, 3 },
    { EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
    { EVENT_EXIT, 5, CONDITION_ALWAYS, 5 },
    { 0x31, 6, CONDITION_ALWAYS, 6 },
    { 0x31, 7, CONDITION_ALWAYS, 7 },
    { EVENT_EXIT, 8, CONDITION_ALWAYS, 8 },
    { EVENT_EXIT, 9, CONDITION_ALWAYS, 9 },
    { EVENT_EXIT, 10, CONDITION_ALWAYS, 10 },
    { EVENT_EXIT, 11, CONDITION_ALWAYS, 11 },
    { EVENT_EXIT, 12, CONDITION_ALWAYS, 12 },
    { EVENT_EXIT, 13, CONDITION_ALWAYS, 13 },
    { SEARCH(SEARCH_UNNAMED), 20, CONDITION_ALWAYS, EVENT_SCRIPT(EV20_SEARCH) },
    { SEARCH(SEARCH_UNNAMED), 99, CONDITION_ALWAYS, EVENT_SCRIPT(EV99_SEARCH) },
    { SEARCH(SEARCH_CHEST), 100, 0xef3, 0x500003 },
    { SCENE_EVENTS_END },
};

struct SceneEvent gVaultCaveEvents2[] = {
    { 0x21, 1, CONDITION_ALWAYS, 1 },
    { 0x31, 2, CONDITION_ALWAYS, 2 },
    { EVENT_EXIT, 3, CONDITION_ALWAYS, 3 },
    { EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
    { EVENT_EXIT, 5, CONDITION_ALWAYS, 5 },
    { 0x21, 6, CONDITION_ALWAYS, 6 },
    { 0x31, 7, CONDITION_ALWAYS, 7 },
    { EVENT_EXIT, 8, CONDITION_ALWAYS, 8 },
    { EVENT_EXIT, 9, CONDITION_ALWAYS, 9 },
    { 0x21, 10, CONDITION_ALWAYS, 10 },
    { EVENT_EXIT, 11, CONDITION_ALWAYS, 11 },
    { EVENT_EXIT, 13, CONDITION_ALWAYS, 13 },
    { EVENT_EXIT, 14, CONDITION_ALWAYS, 14 },
    { SEARCH(SEARCH_CHEST), 100, 0xfc4, FIND_ITEM(0x82) },
    { SEARCH(SEARCH_UNNAMED), 99, CONDITION_ALWAYS, EVENT_SCRIPT(EV99_SEARCH) },
    { PSYNERGY_ON_ACTOR(0x8c), 9, 0x9a9, EVENT_SCRIPT(EV09_PSYNERGY_2) },
    { 0x202, 60, CONDITION_ALWAYS, EVENT_SCRIPT(EV60_SCRIPT) },
    { PSYNERGY_ON_ACTOR(ABILITY_FROST), 16, 0x200, EVENT_SCRIPT(EV16_PSYNERGY) },
    { PSYNERGY_ON_ACTOR(ABILITY_FROST), 17, 0x201, EVENT_SCRIPT(EV17_PSYNERGY) },
    { PSYNERGY_ON_ACTOR(ABILITY_FROST), 18, 0x202, EVENT_SCRIPT(EV18_PSYNERGY) },
    { PSYNERGY_ON_ACTOR(ABILITY_FROST), 19, 0x203, EVENT_SCRIPT(EV19_PSYNERGY) },
    { PSYNERGY_ON_ACTOR(ABILITY_FROST), 20, 0x204, EVENT_SCRIPT(EV20_PSYNERGY) },
    { PSYNERGY_ON_ACTOR(ABILITY_FROST), 21, 0x205, EVENT_SCRIPT(EV21_PSYNERGY) },
    { PSYNERGY_ON_ACTOR(0x4e), 22, 0x300, EVENT_SCRIPT(EV22_PSYNERGY) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gVaultCaveEvents3[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { 0x31, 2, CONDITION_ALWAYS, 2 },
    { 0x21, 3, CONDITION_ALWAYS, 3 },
    { EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
    { EVENT_EXIT, 5, CONDITION_ALWAYS, 5 },
    { 0x21, 6, CONDITION_ALWAYS, 6 },
    { 0x202, 50, CONDITION_ALWAYS, EVENT_SCRIPT(EV50_SCRIPT) },
    { PSYNERGY_ON_ACTOR(0x8c), 10, 0x9aa, EVENT_SCRIPT(EV10_PSYNERGY) },
    { PSYNERGY_ON_ACTOR(0x8c), 9, CONDITION_ALWAYS, EVENT_SCRIPT(EV09_PSYNERGY_3) },
    { SEARCH(SEARCH_UNNAMED), 99, CONDITION_ALWAYS, EVENT_SCRIPT(EV99_SEARCH) },
    { SCENE_EVENTS_END },
};

u32 gVaultCave_ScheduleFrames[] = {
    20, 80, 115, 170,
};
// clang-format on

s32 gVaultCave_ScheduleTimer = 0;

s32 gVaultCave_ScheduleIndex = 0;

s32 gVaultCave_TickCounter = 0;

s32 gVaultCave_TickValue = 0;
