/// @file
/// Scene code of overlay 0x6b0, in ROM order.

#include "../shared/tables.h"
#include "../shared/teleporter.h"
#include "types.h"
#include "system.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "field_effect.h"
#include "event_work.h"
#include "event_inline.h"
#include "module/field/field_effect.h"
#include "module/scene/anemos_inner_sanctum.h"
#include "module/scene/scene.h"

void FieldEffect_SpawnLocal(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                            s32 velocityZ, u32 flags,
                            struct EffectOptions *options);

void Teleporter_DropIn(void) {
  struct Actor *actor = Actor_Get(gMainRoutineWork.currentOwner);
  u32 i;
  struct EffectOptions options;
  s32 spawn = 1;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Map_CenterOnFollowedPoint();
  System_Wait(1);
  actor->y = 0x820000;
  actor->gravity = 0x8000;
  actor->bounce = 0;
  actor->motionFlags = 0;
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x100;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_PlaySound(204);
  actor->motionFlags = 3;
  FieldEvent_Wait(24);
  options.palette = 7;
  options.update = FieldEffect_AdvanceMotion;
  options.startScaleX = 0xa3d7;
  options.startScaleY = 0xa3d7;
  i = 0;
  // A goto loop carries no loop notes: as a do-while loop, loop.c moves the
  // velocity array's address out of the loop and 4 of 169 instructions differ
  // in tla-en (10 as a for or while loop).
loop:
  {
    s32 angle;
    s32 velocity[3];
    s32 cosine;

    angle = i << 12;
    cosine = Trig_Cos(angle);
    /* FAKEMATCH(dead): spawn is always 1, a constant GCSE only folds after its
     * partial redundancy pass, so that pass leaves the velocity array's
     * address in the loop, built after the cosine call as the native has it;
     * as the plain body, 4 of 169 instructions differ in tla-en. */
    if (spawn) {
      velocity[0] = cosine;
      velocity[1] = 0;
      velocity[2] = Math_Sin(angle);
      velocity[0] += velocity[0] >> 1;
      velocity[0] += (Random16() * 0xc000 >> 16) - 0x6000;
      velocity[2] += (Random16() * 0x6000 >> 16) - 0x3000;
      FieldEffect_SpawnLocal(actor->x, actor->y, actor->z, velocity[0],
                             velocity[1], velocity[2], 0x1090001, &options);
    }
    i++;
  }
  if (i <= 16)
    goto loop;
  Event_PlaySound(188);
  Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 0x101);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 49);
  MapGrid_SetShake(0x50000, 0x50000, 0x10000);
  MapGrid_SetShake(-1, -1, 0xe666);
  FieldMapGrid_WaitForShakeToSettle();
  Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 0x100);
  actor->gravity = 0x10000;
  actor->bounce = 0x4000;
  FieldEvent_Wait(10);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 1);
  Event_End();
}

#include "cell_target.h"

void GameFlag_SetByte(s32 flag, s32 value);
void Teleporter_Sink(struct CellTarget *work, struct CellTargetEntry *entry);

DEFINE_TELEPORTERS_STEP(Teleporters_Step, gTeleporters, Teleporter_Uncover,
                        Teleporter_Sink)

DEFINE_TELEPORTER_PLACE_ARRIVAL(Teleporter_PlaceArrival, gTeleporters,
                                Teleporter_DropIn)

#include "scene_tables.h"

void Func_02000654(void);
void Func_02000660(void);
void Func_020006f8(void);
void Func_02000794(void);

extern u32 gData_0200378c[];
extern u32 gData_020037c8[];

// clang-format off
u32 gData_020034c0[] = {
    0, 1, 13, 22, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gData_020034fc[] = {
    0, 1, 13, 44, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gData_02003538[] = {
    0, 1, 13, 126, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gData_02003574[] = {
    0, 20, 38, 17,
};

DEFINE_G_DATA_02003584(gData_02003584, Func_02000660, 38, 17)

u32 gData_02003594[] = {
    0x80010000, 23, 9, 0xffffe667, 23, 10,
    0xffffe667, 0, 1, 13, 8, 0xc0010000,
    38, 17,
};

u32 gData_020035cc[] = {
    0, 50, 0x80010000, 23, 9, 0x1999,
    23, 10, 0x1999, 23, 6, 0x30000,
    0, 2, 13, 10, 0xc0010000, 22,
    26, 4, 17,
};

s32 gData_02003620[] = {
    0, 30, 22, 26, 0, (s32)0x80010000,
    23, 9, (s32)0xffffe667, 23, 10, (s32)0xffffe667,
    23, 6, -0x30000, 0, 2, 13,
    10, (s32)0xc0010000, 17,
};

u32 gData_02003674[] = {
    0, 3, 38, 17,
};

s32 gData_02003684[] = {
    22,
    9,
    0x20000,
    22,
    10,
    0x20000,
    (s32)0x80010000,
    23,
    9,
    (s32)0xffffe667,
    23,
    10,
    (s32)0xffffe667,
    0,
    2,
    13,
    10,
    (s32)0xc0010000,
    46,
    (s32)Func_020006f8,
    46,
    (s32)Func_02000794,
    38,
    17,
};

s16 gData_020036e4[] = {
    8, 10, -1,
};

s16 gData_020036ea[] = {
    8, 10, 11, 12, 13, -1,
};

s16 gData_020036f6[] = {
    9, 10, -1,
};

s32 gData_020036fc[] = {
    -48, -32, 48, -32, -64, -16,
    64, -16, -32, -16, 32, -16,
    -48, 0, 48, 0, -32, 0,
    32, 0, -64, 16, 64, 16,
    -32, 16, 32, 16, -48, 32,
    48, 32, -16, 32, 16, 32,
};

u32 gData_0200378c[] = {
    0, 1, 13, 22, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gData_020037c8[] = {
    0, 1, 13, 44, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gData_02003804[] = {
    0, 1, 13, 126, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

s32 gData_02003840[] = {
    (s32)gData_020034c0,
    (s32)gData_020034fc,
    (s32)gData_02003538,
};

s16 gData_0200384c[] = {
    89, 90, 91, 92, 93, 94, 95, -1,
};

s16 gData_0200385c[] = {
    23, 25, 22, 23, 25, 22, 23, 25,
    22, 23, 25, 22, 23, 25, 22, 23,
    25, 22, 23, 25, 22, -1,
};

s16 gData_02003888[] = {
    30, 27, 31, 22, 21, 31, 20, 18,
    31, 13, 11, 27, 11, 8, 24, 8,
    5, 21, 31, 31, 31, -1,
};

DEFINE_G_DATA_020013B4(gData_020038b4, 94, 89, 90, 91, 92, 93)

s32 gData_020038fc[] = {
    46,
    (s32)Func_02000654,
    17,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(303),
    SCENE_EXIT_IF(1, 308, 2, CONDITION_ALWAYS),
    SCENE_EXIT_IF(2, 307, 10, CONDITION_ALWAYS),
    SCENE_EXIT_IF(99, 236, 99, CONDITION_ALWAYS),
    SCENE_EXITS(304),
    SCENE_EXIT_IF(1, 304, 5, CONDITION_ALWAYS),
    SCENE_EXIT_IF(2, 304, 12, CONDITION_ALWAYS),
    SCENE_EXIT_IF(3, 304, 17, CONDITION_ALWAYS),
    SCENE_EXIT_IF(4, 305, 8, CONDITION_ALWAYS),
    SCENE_EXIT_IF(5, 304, 1, CONDITION_ALWAYS),
    SCENE_EXIT_IF(6, 307, 2, CONDITION_ALWAYS),
    SCENE_EXIT_IF(7, 304, 13, CONDITION_ALWAYS),
    SCENE_EXIT_IF(8, 307, 1, CONDITION_ALWAYS),
    SCENE_EXIT_IF(9, 304, 15, CONDITION_ALWAYS),
    SCENE_EXIT_IF(10, 304, 14, CONDITION_ALWAYS),
    SCENE_EXIT_IF(11, 304, 18, CONDITION_ALWAYS),
    SCENE_EXIT_IF(12, 304, 2, CONDITION_ALWAYS),
    SCENE_EXIT_IF(13, 304, 7, CONDITION_ALWAYS),
    SCENE_EXIT_IF(14, 304, 10, CONDITION_ALWAYS),
    SCENE_EXIT_IF(15, 304, 9, CONDITION_ALWAYS),
    SCENE_EXIT_IF(16, 307, 9, CONDITION_ALWAYS),
    SCENE_EXIT_IF(17, 304, 3, CONDITION_ALWAYS),
    SCENE_EXITS(305),
    SCENE_EXIT_IF(1, 305, 13, CONDITION_ALWAYS),
    SCENE_EXIT_IF(2, 306, 7, CONDITION_ALWAYS),
    SCENE_EXIT_IF(3, 305, 14, CONDITION_ALWAYS),
    SCENE_EXIT_IF(4, 305, 15, CONDITION_ALWAYS),
    SCENE_EXIT_IF(5, 305, 10, CONDITION_ALWAYS),
    SCENE_EXIT_IF(6, 307, 7, CONDITION_ALWAYS),
    SCENE_EXIT_IF(7, 306, 2, CONDITION_ALWAYS),
    SCENE_EXIT_IF(8, 304, 4, CONDITION_ALWAYS),
    SCENE_EXIT_IF(9, 306, 1, CONDITION_ALWAYS),
    SCENE_EXIT_IF(10, 305, 5, CONDITION_ALWAYS),
    SCENE_EXIT_IF(11, 305, 17, CONDITION_ALWAYS),
    SCENE_EXIT_IF(12, 307, 4, CONDITION_ALWAYS),
    SCENE_EXIT_IF(13, 305, 1, CONDITION_ALWAYS),
    SCENE_EXIT_IF(14, 305, 3, CONDITION_ALWAYS),
    SCENE_EXIT_IF(15, 305, 4, CONDITION_ALWAYS),
    SCENE_EXIT_IF(16, 307, 3, CONDITION_ALWAYS),
    SCENE_EXIT_IF(17, 305, 11, CONDITION_ALWAYS),
    SCENE_EXITS(306),
    SCENE_EXIT_IF(1, 305, 9, CONDITION_ALWAYS),
    SCENE_EXIT_IF(2, 305, 7, CONDITION_ALWAYS),
    SCENE_EXIT_IF(3, 306, 9, CONDITION_ALWAYS),
    SCENE_EXIT_IF(4, 306, 8, CONDITION_ALWAYS),
    SCENE_EXIT_IF(5, 308, 5, CONDITION_ALWAYS),
    SCENE_EXIT_IF(6, 307, 5, CONDITION_ALWAYS),
    SCENE_EXIT_IF(7, 305, 2, CONDITION_ALWAYS),
    SCENE_EXIT_IF(8, 306, 4, CONDITION_ALWAYS),
    SCENE_EXIT_IF(9, 306, 3, CONDITION_ALWAYS),
    SCENE_EXIT_IF(10, 307, 6, CONDITION_ALWAYS),
    SCENE_EXIT_IF(11, 306, 11, CONDITION_ALWAYS),
    SCENE_EXIT_IF(12, 306, 12, CONDITION_ALWAYS),
    SCENE_EXIT_IF(13, 307, 12, CONDITION_ALWAYS),
    SCENE_EXITS(307),
    SCENE_EXIT_IF(1, 304, 8, CONDITION_ALWAYS),
    SCENE_EXIT_IF(2, 304, 6, CONDITION_ALWAYS),
    SCENE_EXIT_IF(3, 305, 16, CONDITION_ALWAYS),
    SCENE_EXIT_IF(4, 305, 12, CONDITION_ALWAYS),
    SCENE_EXIT_IF(5, 306, 6, CONDITION_ALWAYS),
    SCENE_EXIT_IF(6, 306, 10, CONDITION_ALWAYS),
    SCENE_EXIT_IF(7, 305, 6, CONDITION_ALWAYS),
    SCENE_EXIT_IF(8, 307, 11, CONDITION_ALWAYS),
    SCENE_EXIT_IF(9, 304, 16, CONDITION_ALWAYS),
    SCENE_EXIT_IF(10, 303, 2, CONDITION_ALWAYS),
    SCENE_EXIT_IF(11, 307, 8, CONDITION_ALWAYS),
    SCENE_EXITS_END,
};

struct ActorPlacement gData_02003b38[] = {
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_02003b50[] = {
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_02003b68[] = {
    {272, CONDITION_ALWAYS, 7, 0x1, 0, 0, FACING_NORTH, 2, 2},
    {432, CONDITION_ALWAYS, (s32)gData_020038fc, 0x2, 0, 0, FACING_NORTH, 2, 2},
    {272, CONDITION_ALWAYS, 7, 0x3, 0, 0, FACING_NORTH, 2, 2},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_02003bc8[] = {
    {272, CONDITION_ALWAYS, 7, 0x1, 0, 0, FACING_NORTH, 2, 2},
    {329, CONDITION_ALWAYS, 1, 0x2, 0, 0, FACING_NORTH, 2, 2},
    {272, CONDITION_ALWAYS, 7, 0x3, 0, 0, FACING_NORTH, 2, 2},
    {272, CONDITION_ALWAYS, 7, 0x4, 0, 0, FACING_NORTH, 2, 2},
    {272, CONDITION_ALWAYS, 7, 0x5, 0, 0, FACING_NORTH, 2, 2},
    {272, CONDITION_ALWAYS, 7, 0x6, 0, 0, FACING_NORTH, 2, 2},
    {489, CONDITION_ALWAYS, 7, 0x7, 0, 0, FACING_NORTH, 2, 2},
    {SCENE_TABLE_END},
};

DEFINE_G_DATA_02003C88(gData_02003c88, 0xffff01b0, gData_020038fc, 3, 0x202c000,
    7, 4, 0xffff0110, 5, 0x202c000)

struct ActorPlacement gData_02003ce8[] = {
    {431, CONDITION_ALWAYS, 7, 0x1, 0, 0, FACING_NORTH, 2, 2},
    {431, CONDITION_ALWAYS, 7, 0x2, 0, 0, FACING_NORTH, 2, 2},
    {431, CONDITION_ALWAYS, 7, 0x3, 0, 0, FACING_NORTH, 2, 2},
    {432, CONDITION_ALWAYS, (s32)gData_020038fc, 0x4, 0, 0, FACING_NORTH, 2, 2},
    {429, CONDITION_ALWAYS, (s32)gData_020038fc, PIXELS(584), 0, PIXELS(582), FACING_WEST, 1, 0},
    {429, CONDITION_ALWAYS, (s32)gData_020038fc, PIXELS(584), 0, PIXELS(630), FACING_WEST, 1, 1},
    {429, CONDITION_ALWAYS, (s32)gData_020038fc, PIXELS(680), 0, PIXELS(582), FACING_EAST, 1, 1},
    {429, CONDITION_ALWAYS, (s32)gData_020038fc, PIXELS(680), 0, PIXELS(630), FACING_EAST, 1, 1},
    {SCENE_TABLE_END},
};

struct Trigger gData_02003dc0[] = {
    {TRIGGER_TABLE_END},
};

struct Trigger gData_02003dcc[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_STEP, 54, CONDITION_ALWAYS, EVENT(Func_02001090)},
    {TRIGGER_STEP, 50, 0x200, EVENT(Func_02000ac8)},
    {TRIGGER_STEP, 51, 0x200, EVENT(Func_02000ac8)},
    {TRIGGER_STEP, 52, 0x200, EVENT(Func_02000ac8)},
    {TRIGGER_STEP, 53, 0x200, EVENT(Func_02000ac8)},
    {0x9c05, 30, CONDITION_ALWAYS, EVENT(Func_02001c60)},
    {TRIGGER_TABLE_END},
};

struct Trigger gData_02003e38[] = {
    {0x31, 1, CONDITION_ALWAYS, 1},
    {0x31, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {0x31, 4, CONDITION_ALWAYS, 4},
    {0x21, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {0x21, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_EXIT, 13, CONDITION_ALWAYS, 13},
    {TRIGGER_EXIT, 14, CONDITION_ALWAYS, 14},
    {TRIGGER_EXIT, 15, CONDITION_ALWAYS, 15},
    {0x21, 16, CONDITION_ALWAYS, 16},
    {TRIGGER_EXIT, 17, CONDITION_ALWAYS, 17},
    {TRIGGER_STEP, 11, IF_FLAG_SET(0x207), EVENT(Func_02001518)},
    {TRIGGER_STEP, 50, 0x200, EVENT(Func_02001398)},
    {TRIGGER_STEP, 51, 0x201, EVENT(Func_02001398)},
    {TRIGGER_STEP, 52, 0x202, EVENT(Func_02001398)},
    {TRIGGER_STEP, 53, 0x203, EVENT(Func_02001398)},
    {TRIGGER_STEP, 54, 0x204, EVENT(Func_02001398)},
    {TRIGGER_STEP, 55, 0x205, EVENT(Func_02001398)},
    {TRIGGER_STEP, 56, 0x206, EVENT(Func_02001398)},
    {0x8, 0, CONDITION_ALWAYS, EVENT(Func_0200109c)},
    {0x9, 0, CONDITION_ALWAYS, EVENT(Func_02001160)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, 1), 8, 0xa80, EVENT(Func_0200109c)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 8, 0xa80, EVENT(Func_02001160)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, 1), 10, 0xa81, EVENT(Func_0200109c)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 10, 0xa81, EVENT(Func_02001160)},
    {TRIGGER_TABLE_END},
};

struct Trigger gData_02003fac[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {0x31, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {0x21, 6, CONDITION_ALWAYS, 6},
    {0x31, 7, CONDITION_ALWAYS, 7},
    {0x21, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_EXIT, 13, CONDITION_ALWAYS, 13},
    {TRIGGER_EXIT, 14, CONDITION_ALWAYS, 14},
    {TRIGGER_EXIT, 15, CONDITION_ALWAYS, 15},
    {TRIGGER_EXIT, 16, CONDITION_ALWAYS, 16},
    {TRIGGER_EXIT, 17, CONDITION_ALWAYS, 17},
    {0x8b85, 0, CONDITION_ALWAYS, EVENT(Func_02001614)},
    {0x40008b85, 0, CONDITION_ALWAYS, EVENT(Leader_SetLayerByte)},
    {TRIGGER_STEP, 50, 0x200, EVENT(Func_02001660)},
    {TRIGGER_STEP, 51, IF_FLAG_SET(0x200), EVENT(Func_020016fc)},
    {PSYNERGY_ON_ACTOR(ABILITY_LIFT), 9, CONDITION_ALWAYS, 0},
    {0x8, 0, CONDITION_ALWAYS, EVENT(Func_0200109c)},
    {0x9, 0, CONDITION_ALWAYS, EVENT(Func_02001160)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, 1), 8, 0xa82, EVENT(Func_0200109c)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 8, 0xa82, EVENT(Func_02001160)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, 1), 10, 0xa83, EVENT(Func_0200109c)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 10, 0xa83, EVENT(Func_02001160)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, 1), 11, CONDITION_ALWAYS, EVENT(Func_0200109c)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 11, CONDITION_ALWAYS, EVENT(Func_02001160)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, 1), 12, 0xa85, EVENT(Func_0200109c)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 12, 0xa85, EVENT(Func_02001160)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, 1), 13, 0xa84, EVENT(Func_0200109c)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 13, 0xa84, EVENT(Func_02001160)},
    {TRIGGER_TABLE_END},
};

struct Trigger gData_02004150[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {0x21, 2, CONDITION_ALWAYS, 2},
    {0x21, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {0x21, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {0x31, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_STEP, 11, IF_FLAG_SET(0x207), EVENT(Func_02001518)},
    {TRIGGER_STEP, 50, 0x200, EVENT(Func_02001398)},
    {TRIGGER_STEP, 51, 0x201, EVENT(Func_02001398)},
    {TRIGGER_STEP, 52, 0x202, EVENT(Func_02001398)},
    {TRIGGER_STEP, 53, 0x203, EVENT(Func_02001398)},
    {TRIGGER_STEP, 54, 0x204, EVENT(Func_02001398)},
    {TRIGGER_STEP, 55, 0x205, EVENT(Func_02001398)},
    {TRIGGER_STEP, 56, 0x206, EVENT(Func_02001398)},
    {TRIGGER_STEP, 33, CONDITION_ALWAYS, EVENT(Func_0200178c)},
    {TRIGGER_STEP, 34, CONDITION_ALWAYS, EVENT(Func_0200178c)},
    {TRIGGER_STEP, 35, CONDITION_ALWAYS, EVENT(Func_020017a0)},
    {TRIGGER_STEP, 36, CONDITION_ALWAYS, EVENT(Func_020017a0)},
    {0x8b85, 0, CONDITION_ALWAYS, EVENT(Func_02001614)},
    {0x40008b85, 0, CONDITION_ALWAYS, EVENT(Leader_SetLayerByte)},
    {0x8, 0, CONDITION_ALWAYS, EVENT(Func_0200109c)},
    {0x9, 0, CONDITION_ALWAYS, EVENT(Func_02001160)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, 1), 9, 0xa86, EVENT(Func_0200109c)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 9, 0xa86, EVENT(Func_02001160)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, 1), 10, CONDITION_ALWAYS, EVENT(Func_0200109c)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 10, CONDITION_ALWAYS, EVENT(Func_02001160)},
    {TRIGGER_TABLE_END},
};

struct Trigger gData_020042c4[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {0x31, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {0x31, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_STEP, 50, 0x200, EVENT(Func_02001938)},
    {TRIGGER_STEP, 51, 0x200, EVENT(Func_02001938)},
    {TRIGGER_STEP, 52, 0x200, EVENT(Func_02001938)},
    {TRIGGER_STEP, 30, 0xa8c, EVENT(Func_02001b88)},
    {TRIGGER_RAISED, 200, CONDITION_ALWAYS, EVENT(Func_020019a8)},
    {TRIGGER_RAISED, 201, CONDITION_ALWAYS, EVENT(Func_02001a94)},
    {0xffffffff, 0, 0x0, 0},
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_TABLE_END},
};

s16 gData_020043e4[] = {
    52, 7, 1, 2, 2, 54, 7, 1,
    2, 2, -1,
};

s16 gData_020043fa[] = {
    18, 4, 1, 2, 2, 18, 7, 1,
    2, 2, -1,
};
// clang-format on

s32 gData_02004410[] = {
    (s32)gData_020043fa, 0x4000b, (s32)gData_020043fa, 0x4001a,
    (s32)gData_020043fa, 0x40035,
};

s32 gData_02004428[] = {
    (s32)gData_0200378c,
    (s32)gData_020037c8,
    (s32)gData_02003804,
};
