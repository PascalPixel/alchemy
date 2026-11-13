/// @file
/// Magma Rock's inside: its crumbling pillars and traps.

#include "../shared/map_cells.h"
#include "../magma_rock_inside.h"
#include "io_reg.h"
#include "call.h"
#include "event_work.h"
#include "field.h"
#include "module/field/object.h"
#include "module/scene/magma_rock_inside.h"
#include "module/scene/scene.h"
#include "module/tla.h"

void Tightrope_Initialize(void);

extern const u8 gMagmaRockInside_PlatformPath1[];
extern const u8 gMagmaRockInside_PlatformPath2[];
extern const u8 gMagmaRockInside_PlatformPath3[];
extern const u8 gMagmaRockInside_PlatformPath4[];
extern const u8 gMagmaRockInside_PlatformPath5[];
extern const u8 gMagmaRockInside_PlatformPath10[];
extern const u8 gMagmaRockInside_PlatformPath11[];
extern const u8 gMagmaRockInside_PlatformPath12[];
extern u16 gMagmaRockInside_Statues[];
extern u16 gMagmaRockInside_Room4Statues[];
extern const u8 gMagmaRockInside_Room6Pillars[];

void MagmaRockInside_StartLavaPlatform(s32 id, const u8 *script, s32 flag);
void Actor_PlaceBlocks(const u8 *table);
void Func_02004ec8(s32 a, s32 b, s32 c);
s32 MagmaRockInside_SwapCells(u32 layer, s32 x, s32 y, s32 value);
void MagmaRockInside_LayerWithLeader(void);

#define CLOSED_BLEND()                                                         \
  {                                                                            \
    REG_BLDALPHA = 0x1000;                                                     \
    REG_BLDCNT = 0x3f44;                                                       \
  }

static __inline__ void Raise(s32 id) {
  struct Actor *object = Actor_GetById(id);

  object->motionFlags = 2;
  object->y = object->terrainHeight = 20 << 16;
  object->priorityFlags |= 128;
}

static __inline__ void Lower(s32 id, s32 height) {
  Actor_GetById(id)->motionFlags = 0;
  Actor_GetById(id)->y = Actor_GetById(id)->terrainHeight = height;
}

s32 Scene_Initialize(void) {
  s32 i;

  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 516;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov692Room1) {
    Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 64;
    MagmaRockInside_StartLavaPlatform(8, gMagmaRockInside_PlatformPath1, 768);
    if (!Event_GetFlag(768)) {
      CLOSED_BLEND();
      MapGrid_CopyCellsTo(71, 64, 47, 98, 1, 1);
    }
  } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov692Room2) {
    s32 j;

    FieldEffect_StartBlankOverlay(0);
    Actor_GetById(15)->update = MagmaRockInside_LayerWithLeader;
    j = 0;
    do {
      {
        struct Actor *object = Actor_GetById(j + 9);

        object->motionFlags = 0;
        object->y = object->terrainHeight = 0;
      }
    } while (++j != 6);
    if (Event_GetFlag(2544))
      Actor_GetById(15)->z = 132 << 17;
    Actor_PlaceBlocks(gMagmaRockInside_Room2Pillars);
    MagmaRockInside_RaiseActorCells(gMagmaRockInside_Statues);
    Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 64;
    Raise(16);
    if (Event_GetFlag(768))
      MagmaRockInside_PressGateSwitch1();
    else
      CLOSED_BLEND();
  } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov692Room3) {
    FieldEffect_StartBlankOverlay(0);
    if (Event_GetFlag(2545))
      Actor_GetById(11)->z = 166 << 18;
    Actor_PlaceBlocks(gMagmaRockInside_Room3Pillars);
    Tightrope_Initialize();
    Func_02004ec8(0, 13, 14);
    MagmaRockInside_RaiseActorCells(gMagmaRockInside_Statues);
    Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 64;
    Raise(9);
    MagmaRockInside_StartLavaPlatform(10, gMagmaRockInside_PlatformPath2, 769);
    MagmaRockInside_StartLavaPlatform(12, gMagmaRockInside_PlatformPath3, 769);
    if (Event_GetFlag(769))
      MagmaRockInside_PressGateSwitch2();
    else
      CLOSED_BLEND();
  } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov692Room4) {
    FieldEffect_StartBlankOverlay(0);
    Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 64;
    MagmaRockInside_RaiseActorCells(gMagmaRockInside_Room4Statues);
    if (gMainRoutineWork.entrance <= 7) {
      MagmaRockInside_StartLavaPlatform(10, gMagmaRockInside_PlatformPath4,
                                        769);
      MagmaRockInside_StartLavaPlatform(12, gMagmaRockInside_PlatformPath5,
                                        769);
      if (Event_GetFlag(769)) {
        MagmaRockInside_PressGateSwitch2();
      } else {
        CLOSED_BLEND();
        MapGrid_CopyCellsTo(71, 64, 35, 82, 1, 1);
      }
    } else {
      Actor_GetById(23)->priorityFlags |= 32;
      Actor_GetById(24)->priorityFlags |= 32;
      Actor_GetById(25)->priorityFlags |= 32;
      Actor_SetSpritePriority(23, 3);
      Actor_SetSpritePriority(24, 3);
      Actor_SetSpritePriority(25, 3);
      if (Event_GetFlag(2546))
        Actor_GetById(23)->x = 172 << 17;
      if (Event_GetFlag(2547))
        Actor_GetById(24)->x = 172 << 17;
      if (Event_GetFlag(2548))
        Actor_GetById(25)->x = 172 << 17;
      Actor_PlaceBlocks(gMagmaRockInside_Room4Pillars);
      Lower(26, 0);
      Actor_GetById(15)->unk5c[0] = 3;
      Actor_GetById(16)->unk5c[0] = 3;
      Actor_GetById(17)->unk5c[0] = 3;
      Actor_GetById(18)->unk5c[0] = 3;
      Actor_GetById(19)->unk5c[0] = 3;
      Actor_GetById(20)->unk5c[0] = 3;
      Actor_GetById(21)->unk5c[0] = 3;
      Actor_GetById(22)->unk5c[0] = 3;
      MagmaRockInside_StartLavaPlatform(15, gMagmaRockInside_PlatformPath6,
                                        770);
      MagmaRockInside_StartLavaPlatform(16, gMagmaRockInside_PlatformPath6,
                                        770);
      MagmaRockInside_StartLavaPlatform(17, gMagmaRockInside_PlatformPath7,
                                        770);
      MagmaRockInside_StartLavaPlatform(18, gMagmaRockInside_PlatformPath7,
                                        770);
      MagmaRockInside_StartLavaPlatform(19, gMagmaRockInside_PlatformPath8,
                                        770);
      MagmaRockInside_StartLavaPlatform(20, gMagmaRockInside_PlatformPath8,
                                        770);
      MagmaRockInside_StartLavaPlatform(21, gMagmaRockInside_PlatformPath9,
                                        770);
      MagmaRockInside_StartLavaPlatform(22, gMagmaRockInside_PlatformPath9,
                                        770);
      if (Event_GetFlag(770))
        MagmaRockInside_PressGateSwitch3();
      else
        CLOSED_BLEND();
    }
  } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov692Room5) {
    FieldEffect_StartBlankOverlay(0);
    Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 64;
    HangingVine_Place(10, 1);
    HangingVine_Place(11, 1);
    Raise(8);
    Raise(9);
    if (Event_GetFlag(770)) {
      MagmaRockInside_SwapCells(0, 22, 26, 0);
      MagmaRockInside_SwapCells(0, 25, 26, 0);
      Actor_SetSpritePriority(64, 3);
    } else {
      CLOSED_BLEND();
    }
  } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov692Room6) {
    FieldEffect_StartBlankOverlay(0);
    Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 64;
    if (Event_GetFlag(2549)) {
      MapGrid_CopyCellAttributes(46, 74, 1, 1, 46, 73);
      Actor_GetById(9)->x = 190 << 18;
    }
    if (Event_GetFlag(2550))
      Actor_SetPosition(12, 230 << 18, 154 << 18);
    Actor_PlaceBlocks(gMagmaRockInside_Room6Pillars);
    MagmaRockInside_StartLavaPlatform(8, gMagmaRockInside_PlatformPath10, 770);
    MagmaRockInside_StartLavaPlatform(10, gMagmaRockInside_PlatformPath11, 770);
    MagmaRockInside_StartLavaPlatform(11, gMagmaRockInside_PlatformPath12, 770);
    if (Event_GetFlag(770)) {
      Actor_SetAnimation(9, 4);
      if (Event_GetFlag(2550))
        Actor_SetAnimation(12, 4);
      if (Event_GetFlag(2549))
        MagmaRockInside_SwapCells(2, 46, 8, 0xff00);
      MagmaRockInside_SwapCells(0, 10, 45, 0);
      MagmaRockInside_SwapCells(0, 53, 40, 0);
    } else {
      CLOSED_BLEND();
      MapGrid_CopyCellsTo(71, 64, 50, 102, 1, 1);
    }
  } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov692Room7) {
    i = 0;
  room7:
    {
      struct Actor *object = Actor_GetById(i + 8);

      Func_02004c98(object, 4);
      object->motionFlags = 0;
      object->y = object->terrainHeight = -0x200000;
    }
    if (++i != 3)
      goto room7;
    if (Event_GetFlag(2551)) {
      Actor_SetAnimation(23, 4);
      Actor_SetPosition(23, 174 << 18, 162 << 18);
    }
    Actor_PlaceBlocks(gMagmaRockInside_Room7Pillars);
    FieldEffect_StartBlankOverlay(0);
    if (!Event_GetFlag(771))
      Actor_SetPosition(21, 0, 0);
    if (!Event_GetFlag(772))
      Actor_SetPosition(22, 0, 0);
    if (Event_GetFlag(771) && Event_GetFlag(772)) {
      Lower(11, -0x200000);
      Actor_SetAnimation(11, 4);
    } else {
      MagmaRockInside_SwapCells(0, 38, 18, 0xff00);
    }
    if (Event_GetFlag(2553))
      Actor_SetAnimation(12, 0);
    for (i = 0; i != 4; i++)
      Actor_SetAnimation(i + 13, 2);
    for (i = 0; i != 11; i++)
      Actor_SetSpriteFlags(Actor_GetById(i + 12), 0);
    if (!Event_GetFlag(2552)) {
      for (i = 0; i != 9; i++)
        Actor_SetPosition(i + 12, 0, 0);
    }
  } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov692Room8) {
    MagmaRockInside_StartStepCells();
    Event_SetFlag(324);
    i = 0;
  room8:
    if (Event_GetFlag(i + 514)) {
      s32 y = 24 - i * 2;

      MapGrid_CopyCellsTo(65, 1, 80, y, 1, 2);
      Call4((void (*)())MagmaRockInside_SwapCells, 0, 16, y, 0xff00);
    }
    if (++i != 6)
      goto room8;
    Actor_SetSpriteFlags(Actor_GetById(10), 0);
    Actor_SetSpriteFlags(Actor_GetById(11), 0);
    if (Event_GetFlag(773)) {
      Lower(9, -0x200000);
      Actor_SetAnimation(9, 4);
    } else {
      MagmaRockInside_SwapCells(0, 7, 21, 0xff00);
      Actor_SetPosition(11, 0, 0);
    }
    Lower(8, 0x100000);
    Actor_SetActionCallback(Actor_GetById(8), 9);
  }
  return 0;
}

s32 Scene_PrepareMap(void) {
  s32 scene = gMainRoutineWork.scene;
  s32 entrance = gMainRoutineWork.entrance;

  if (!Event_GetFlag(0x109)) {
    if ((scene == (s32)&SceneId_Ov692Room2 && entrance == 8) ||
        (scene == (s32)&SceneId_Ov692Room7 && entrance == 11)) {
      Event_ClearFlag(0x300);
      Event_ClearFlag(0x301);
      Event_ClearFlag(0x302);
    }
  }
  return 0;
}

#include "types.h"
#include "ram_buffer.h"
#include "map.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_inline.h"

s32 MapGrid_GetTerrainKind(s32 layer, s32 x, s32 z);
s32 MapCollision_FindOrAddTag(s32 unused, s32 key, s32 value);

/// On entering, sets each listed pillar from its two flags, then lifts the
/// leader onto the ground under it.
DEFINE_PILLARS_RESTORE_RAISED(MagmaRockInside_RestoreCrumblingPillars,
                              Func_02004ed0, MapGrid_GetTerrainKind,
                              Actor_DisableById, MapCollision_FindOrAddTag, Func_02004c98)

#include "../shared/trial_road.h"

DEFINE_TRAPS_CHECK(MagmaRockInside_CheckCrumblingStep, Field_TryJumpForward,
                   Actor_FindAtCell, Func_02004c98, MagmaRockInside_SpawnRubble,
                   MagmaRockInside_SetLeaderCellShape,
                   MagmaRockInside_SpawnDustRing)

void MagmaRockInside_CrumblePillar(const void *data, s32 id) {
  const struct TrapEntry *entry = data;
  struct Actor *object = Actor_Get(id);
  s32 flag;

  if (entry->actor != 0xffff) {
  next:
    if (entry->actor != id) {
      entry++;
      goto test;
    }
    flag = entry->flag;
    Event_Start();
    if (!Event_GetFlag(flag + 1)) {
      Event_PlaySound(185);
      MapGrid_SetShake(0x10000, 0x40000, 0x10000);
      MagmaRockInside_SpawnRubble(0);
      System_Wait(8);
      Func_02004c98(object, 2);
      MagmaRockInside_SetActorCellShape(-4, OBJECT_BYTE(object, 34), id);
      MagmaRockInside_SpawnRubble(1);
      System_Wait(16);
      MapGrid_SetShake(-1, -1, 0xe666);
      System_Wait(8);
      FieldActor_SetPositionAndResetMotion(object, 0, 0, 0);
      System_Wait(2);
      Event_PlaySound(188);
      MagmaRockInside_SpawnDustRing();
      System_Wait(20);
      Event_SetFlag(flag);
      Event_SetFlag(flag + 1);
    }
    Event_End();
    return;
  test:
    if (entry->actor != 0xffff)
      goto next;
  }
}

void MagmaRockInside_SetPillarMode3(s32 id) {
  Actor_Get(id)->unk5c[0] = 3;
  Actor_DisableById(id);
}

void Actor_MoveToObject(s32 id, s32 owner);
/// Places actors 8, 9 and 10, then walks a 0xffff-ended list of (actor, flag)
/// halfword pairs: flag + 1 set parks the actor at (0, 0); otherwise its cell
/// gets attributeB 128 and its shape-grid byte is rewritten, and flag set makes
/// it copy actor 9's visual attributes.
DEFINE_PILLARS_RESTORE(MagmaRockInside_RestorePillars,
                       MagmaRockInside_SetPillarMode3, Func_02004ed0,
                       MapGrid_GetTerrainKind, Actor_DisableById, MapCollision_FindOrAddTag)

void MagmaRockInside_TakePlaceOf(struct Actor *object, struct Actor *target) {
  FieldActor_SetPositionAndResetMotion(object, target->x, target->y, target->z);
  FieldActor_SetPositionAndResetMotion(target, 0, 0, 0);
}

void MagmaRockInside_StepOnTrap(const u16 *table) {
  struct Actor *leader;
  struct Actor *partner;
  struct Actor *trap;
  s32 position[3];
  s32 id;
  s32 flag;
  s32 second;

  if (Field_TryJumpForward() != 0)
    return;
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  partner = Actor_GetById(8);
  position[0] = leader->x;
  position[1] = leader->y;
  position[2] = leader->z;
  trap = Actor_FindAtCell(position, 0);
  if (trap != 0)
    goto test;
  return;
next:
  id = table[0];
  if (Actor_GetById(id) != trap) {
    table += 2;
    goto test;
  }
  {
    flag = table[1];
    Event_Start();
    if (!Event_GetFlag(flag)) {
      Func_02004c98(partner, 1);
      MagmaRockInside_TakePlaceOf(partner, trap);
      wait(1);
      Event_PlaySound(125);
      FieldActor_SetSpriteColor(Actor_GetById(8), 7);
      wait(2);
      Func_02004c98(partner, 0);
      Actor_CopyGraphics(id, 9);
      FieldActor_SetSpriteColor(Actor_GetById(8), 0);
      wait(2);
      FieldActor_SetSpriteColor(Actor_GetById(8), 7);
      wait(4);
      FieldActor_SetSpriteColor(Actor_GetById(8), 0);
      MagmaRockInside_SpawnRubble(0);
      MagmaRockInside_TakePlaceOf(trap, partner);
      wait(1);
      Event_SetFlag(flag);
    } else {
      second = flag + 1;
      if (!Event_GetFlag(second)) {
        Func_02004c98(partner, 0);
        MagmaRockInside_TakePlaceOf(partner, trap);
        wait(1);
        leader->motionFlags = 0;
        Event_PlaySound(185);
        MapGrid_SetShake(0x10000, 0x40000, 0x10000);
        MagmaRockInside_SpawnRubble(0);
        wait(8);
        Func_02004c98(partner, 2);
        MagmaRockInside_SetLeaderCellShape(-2, trap->terrainId);
        MagmaRockInside_SpawnRubble(1);
        wait(16);
        MagmaRockInside_SetLeaderCellShape(-2, trap->terrainId);
        wait(4);
        MapGrid_SetShake(-1, -1, 0xe666);
        wait(8);
        leader->motionFlags = 3;
        wait(5);
        Actor_SetPositionAndResetMotion(partner, 0, 0, 0);
        wait(2);
        Event_PlaySound(188);
        MagmaRockInside_SpawnDustRing();
        wait(20);
        Event_SetFlag(second);
      }
    }
    Event_End();
    return;
  }
test:
  if (*table != 0xffff)
    goto next;
}

#include "../../field/field_effect/advance_motion_priority.c"
