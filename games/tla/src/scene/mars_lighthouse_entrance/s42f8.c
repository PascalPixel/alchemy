/// @file
/// Scene code of overlay 0x6aa, in ROM order.

#include "../shared/tables.h"
#include "../shared/teleporter.h"
#include "../shared/map_cells.h"
#include "../shared/actors.h"
#include "../shared/effects.h"
#include "../shared/mars_lighthouse_entrance_room.h"
#include "field_effect.h"
#include "module/field/field_effect.h"
#include "module/scene/mars_lighthouse_entrance.h"
#include "module/scene/scene.h"

void Func_02003b30(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags, struct EffectOptions *options);

void Func_020042f8(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  struct EffectOptions options;
  struct Vector3 velocity;
  u32 i;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Camera_SetBounds(-1, -1, -1, 0);
  Map_CenterOnFollowedPoint();
  System_Wait(1);
  leader->y = 0x820000;
  leader->gravity = 0x8000;
  leader->bounce = 0;
  leader->motionFlags = 0;
  SCENE_WORK->unk1ac = 0x100;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_PlaySound(204);
  leader->motionFlags = 3;
  FieldEvent_Wait(24);
  options.palette = 7;
  options.update = Effect_AdvanceMotion;
  options.startScaleX = 0xa3d7;
  options.startScaleY = 0xa3d7;
  i = 0;
next:
  {
    s32 angle = i << 12;

    velocity.x = Math_Cosine(angle);
    velocity.y = 0;
    velocity.z = Math_Sin(angle);
    velocity.x += velocity.x >> 1;
    velocity.x += ((u32)(Random16Far() * 3 << 14) >> 16) - 0x6000;
    velocity.z += ((u32)(Random16Far() * 3 << 13) >> 16) - 0x3000;
    Func_02003b30(leader->x, leader->y, leader->z, velocity.x, velocity.y,
                  velocity.z, 0x1090001, &options);
  }
  if (++i <= 16)
    goto next;
  Event_PlaySound(188);
  Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 0x101);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 49);
  MapGrid_SetShake(0x50000, 0x50000, 0x10000);
  MapGrid_SetShake(-1, -1, 0xe666);
  MapGrid_WaitForShakeToSettle();
  Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 0x100);
  leader->gravity = 0x10000;
  leader->bounce = 0x4000;
  FieldEvent_Wait(10);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 1);
  Event_End();
}

#include "types.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "actor.h"
#include "event_inline.h"
#include "cell_target.h"

void GameFlag_SetByte(s32 flag, s32 value);
void Func_02004160(struct CellTarget *work,
                   struct CellTargetEntry *entry);

DEFINE_TELEPORTERS_STEP(Func_02004490, Data_020068b4, Func_020040f0, Func_02004160)

DEFINE_TELEPORTER_PLACE_ARRIVAL(Func_02004540, Data_020068b4, Func_020042f8)

#include "dma.h"
#include "io_reg.h"

extern u16 *gData_0200e88c;
extern u16 *gData_02006890;

/// Records the four palette lists, writes each listed palette entry from its
/// red, green and blue, then copies two palettes from block32 and starts the
/// blend.
DEFINE_PALETTE_SET_COLORS_FADE(Func_020045f8, gData_0200e88c, gData_02006890,
    Data_02006894, Data_02006880, Func_020048e8)

#include "task.h"

extern u16 gData_02006898;

DEFINE_PALETTE_CYCLE_START(Func_020046b4, gData_0200689c, gData_02006898,
    Func_02004794, gData_0200e88c, gData_0200e888, 15, gData_02006884, 16,
    gData_0200687c, Func_020047b8)

void Func_02004718(void) {
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 428) = 256;
  FieldTask_Remove(Func_020047b8);
}

DEFINE_PALETTE_CYCLE_START(Func_02004738, gData_0200689c, gData_02006898,
    Func_02004794, gData_0200e88c, gData_0200e888, 2, gData_02006884, 2,
    gData_0200687c, Func_020047b8)

DEFINE_LIST_COUNT_ENTRIES(Func_02004794)

DEFINE_PALETTE_CYCLE_FADE(Func_020047b8, gData_02006884, gData_0200e888,
    Data_02006894, gData_02006898, Data_02006880, gData_0200689c,
    gData_0200e88c, gData_0200687c, gData_02006890)

void Func_020048e8(void) {
  struct MarsLighthouseEntranceEventWork *work = Ram_HeapSlots->eventWork;
  s32 saved;

  saved = work->value1ac;
  PaletteFade_ApplyTarget(0x202108, 1);
  PaletteFade_ApplyTarget(0x10000, 2);
  ((struct MarsLighthouseEntranceEventWork *)Ram_HeapSlots->eventWork)->value1ac = 0x100;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Func_02004738();
  Func_02004dc8();
  System_Wait(40);
  Func_02004718();
  PaletteFade_ApplyTarget(0x10000, 1);
  PaletteFade_Interpolate(16);
  System_Wait(16);
  {
    struct MainRoutineWork *party = &gMainRoutineWork;

    party->unk2c2 = 0;
  }
  work->value1ac = saved;
}

#include "field.h"

DEFINE_ACTORS_SET_CELLS_OPEN(Func_02004970, Func_02004a60)

// Finds the current target's table row and shrinks it.
void Func_020049dc(u16 *list, s32 mode, s32 count) {
  struct GabombaStatueEffectWork *work = Ram_HeapSlots->effectWork;

  /* FAKEMATCH(dead): the entry jump in a once-only block; as the plain
   * goto, 5 of 61 instructions differ in tla-en. */
  do {
    goto test;
  } while (0);
  do {
    u32 entry = *list;
    s32 id = work->objectId;

    if (id == entry) {
      struct Actor *object = Actor_Get(id);
      s32 a;
      s32 flag;

      list++;
      a = list[0];
      flag = list[1];
      if (count < 8)
        object->scaleY -= 0x1f00;
      if (mode == 1) {
        Event_SetFlag(flag);
        Func_02004a60(id, a, flag, object->terrainId);
        object->scaleY = 0x10000;
      }
      break;
    }
    list += 3;
  test:;
  } while (*list != 0xffff);
}

#include "field_event.h"
#include "map.h"

DEFINE_ACTOR_SET_CELL_OPEN(Func_02004a60, Func_02004db8, Func_02004bc0)

#include "scene_tables.h"
#include "version.h"
#include "objcmd.h"

void Func_02000038(void);
void Func_02000070(void);
void Func_0200308c(void);
void Func_02003138(void);
void Func_020031dc(void);

// clang-format off
DEFINE_G_DATA_02004DE8(gData_02004de8, 200, 104)

DEFINE_G_DATA_02004DE8(gData_02004e50, 232, 296)

DEFINE_G_DATA_02004EB8(gData_02004eb8, 48, 904, 528, 840)

u32 gData_02004f44[] = {
    SC_SET(SW_STEERING, 0),
    SC_SET(SW_MOTION, 2),
    SC_SET(SW_Y, PIXELS(48)),
    SC_SET(SW_GROUND, PIXELS(48)),
    SC_SET(SW_SPEED, 0x8000),
    SC_SET(SW_ACCEL, 0x800),
    SC_WAIT(10),
    SC_MOVE(PIXELS(88), 0, PIXELS(688)),
    SC_SYNC,
    SC_WAIT(30),
    SC_LABEL(1),
    SC_MOVE(PIXELS(152), 0, PIXELS(688)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(88), 0, PIXELS(688)),
    SC_SYNC,
    SC_LOOP(SC_FOREVER, SC_TO(1)),
};

DEFINE_G_DATA_02004FF0(gData_02004ff0, 0)

s32 gData_0200503c[] = {
    SC_SET(SW_STEERING, 0),
    SC_SET(SW_MOTION, 0),
    SC_SET(SW_Y, PIXELS(32)),
    SC_SET(SW_GROUND, PIXELS(32)),
    SC_SET(SW_SPEED, 0x8000),
    SC_SET(SW_ACCEL, 0x800),
    SC_MOVE(PIXELS(808), PIXELS(32), PIXELS(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(808), PIXELS(48), PIXELS(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(728), PIXELS(48), PIXELS(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(728), PIXELS(32), PIXELS(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(712), PIXELS(32), PIXELS(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(712), PIXELS(48), PIXELS(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(616), PIXELS(48), PIXELS(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(616), PIXELS(32), PIXELS(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(552), PIXELS(32), PIXELS(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(552), PIXELS(48), PIXELS(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(536), PIXELS(48), PIXELS(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(536), PIXELS(32), PIXELS(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(520), PIXELS(32), PIXELS(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(520), PIXELS(48), PIXELS(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(296), PIXELS(48), PIXELS(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(296), PIXELS(32), PIXELS(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(264), PIXELS(32), PIXELS(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(264), PIXELS(48), PIXELS(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(136), PIXELS(48), PIXELS(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(136), PIXELS(32), PIXELS(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(120), PIXELS(32), PIXELS(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(120), PIXELS(48), PIXELS(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(56), PIXELS(48), PIXELS(128)),
    SC_SYNC,
    SC_WAIT(60),
    SC_CALL(Func_02000070),
    SC_END,
};

u32 gData_02005314[] = {
    SC_SET(SW_STEERING, 0),
    SC_SET(SW_MOTION, 0),
    SC_SET(SW_X, PIXELS(56)),
    SC_SET(SW_Z, PIXELS(128)),
    SC_SET(SW_Y, PIXELS(48)),
    SC_SET(SW_GROUND, PIXELS(48)),
    SC_SET(SW_SPEED, 0x8000),
    SC_SET(SW_ACCEL, 0x800),
    SC_MOVE(PIXELS(56), PIXELS(48), PIXELS(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_END,
};

u32 gData_02005394[] = {
    0, 40, 0x80010000, 22, 25, 0,
    0, 1, 22, 25, 1, 0,
    1, 13, 10, 0xc0010000, 38,
};

s16 gData_020053d8[] = {
    8, 7, 512, 9, 7, 513, -1,
};

u16 gData_020053e6[] = {
    9, 3, 10, 3, 11, 3, 12, 3,
    0,
};

u16 gData_020053f8[] = {
    8, 0, 0,
};

s16 gData_020053fe[] = {
    16, 7, 514, 17, 7, 515, -1,
};

u16 gData_0200540c[] = {
    8, 3, 0,
};

u16 gData_02005412[] = {
    9, 1, 0,
};

s16 gData_02005418[] = {
    12, 7, 516, 13, 7, 517, 14, 7,
    518, 15, 7, 519, 16, 7, 520, -1,
};

u16 gData_02005438[] = {
    9, 3, 0,
};

u16 gData_0200543e[] = {
    8, 0, 10, 1, 0,
};

u16 gData_02005448[] = {
    11, 67, 0,
};

u16 gData_0200544e[] = {
    12, 1, 0,
};

s16 gData_02005454[] = {
    13, 7, 528, 14, 7, 529, 15, 7,
    530, 16, 7, 531, 17, 7, 532, 18,
    7, 533, -1, 0,
};

DEFINE_G_DATA_0200547C(gData_0200547c, Func_0200308c, 112, 728, 22, 128, 728,
    Func_020031dc)

DEFINE_G_DATA_020054CC(gData_020054cc, Func_0200308c, 0x500000, 0x200000,
    0xa80000, 0x160000, 0x600000, 0xa80000, Func_02000c68, Func_020031dc)

DEFINE_G_DATA_0200547C(gData_02005528, Func_0200308c, 872, 576, 30, 872, 584,
    Func_020031dc)

DEFINE_G_DATA_020054CC(gData_02005578, Func_02003138, 0x600000, 0x200000,
    0x2a80000, 0x160000, 0x700000, 0x2a80000, Func_02000a24, Func_020031dc)

s32 gData_020055d4[] = {
    46,
    (s32)Func_02003138,
    4,
    0x3580000,
    0x200000,
    0x880000,
    1,
    4,
    0x3680000,
    0x200000,
    0x880000,
    1,
    4,
    0x3680000,
    0x160000,
    0x880000,
    1,
    4,
    0x3680000,
    0x160000,
    0x980000,
    1,
    22,
    37,
    (s32)Func_02000c1c,
    46,
    (s32)Func_020031dc,
    17,
};

DEFINE_G_DATA_020054CC(gData_02005644, Func_02003138, 0x700000, 0x200000,
    0x2a80000, 0x160000, 0x800000, 0x2a80000, Func_02000808, Func_020031dc)

DEFINE_G_DATA_02001540(gData_020056a0, 30)

u32 gData_020056d0[] = {
    0, 20, 0x80010000, 22, 25, 0,
    0, 1, 22, 25, 1, 0,
    1, 13, 10, 0xc0010000, 38,
};

u32 gData_02005714[] = {
    0, 20, 0x80010000, 22, 25, 0,
    0, 1, 22, 25, 1, 0,
    1, 13, 10, 0xc0010000, 38,
};

s32 gData_02005758[] = {
    0xffff, -0x8000, (s32)0xe000c000, (s32)0xc000a000, 0x20004000, 0x40006000,
    0xffff, -0x8000, 0xffff, -0x8000, -0x4000, (s32)0xc000ffff,
    -0xc000, 0x4000ffff, 0xffff, -0x8000,
};

s32 gData_02005798[] = {
    0xffff, -0x8000, 0xc000, (s32)0xc0008000, 0x4000, 0x40008000,
    0xffff, -0x8000,
};

DEFINE_G_DATA_020012A4(gData_020057b8, 22)

DEFINE_G_DATA_020012A4(gData_020057f4, 44)

DEFINE_G_DATA_020012A4(gData_02005830, 126)

s16 gData_0200586c[] = {
    137, 138, 139, 140, 141, 142, 143, -1,
};

s16 gData_0200587c[] = {
    23, 25, 22, 23, 25, 22, 23, 25,
    22, 23, 25, 22, 23, 25, 22, 23,
    25, 22, 23, 25, 22, -1,
};

s16 gData_020058a8[] = {
    30, 27, 31, 22, 21, 31, 20, 18,
    31, 13, 11, 27, 11, 8, 24, 8,
    5, 21, 31, 31, 31, -1,
};

u32 gData_020058d4[] = {
    0x89008e, 0x8b008a, 0x8d008c, 0x8e008d, 0x8a0089, 0x8c008b,
    0x8d008c, 0x89008e, 0x8b008a, 0x8c008b, 0x8e008d, 0x8a0089,
    0x8b008a, 0x8d008c, 0x89008e, 0x8a0089, 0x8c008b, 0x8e008d,
};

s32 gData_0200591c[] = {
    SC_CALL(Func_02000038),
    SC_END,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 192, 0, 192, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(279),
    SCENE_EXIT(1, 2, 84),
    SCENE_EXIT(2, 281, 2),
    SCENE_EXITS(280),
    SCENE_EXIT(1, 281, 1),
    SCENE_EXIT(2, 287, 2),
    SCENE_EXIT(3, 280, 4),
    SCENE_EXIT(4, 280, 3),
    SCENE_EXIT(5, 282, 5),
    SCENE_EXIT(6, 282, 6),
    SCENE_EXIT(7, 280, 8),
    SCENE_EXIT(8, 280, 7),
    SCENE_EXIT(9, 280, 10),
    SCENE_EXIT(10, 280, 9),
    SCENE_EXIT(20, 281, 20),
    SCENE_EXITS(281),
    SCENE_EXIT(1, 280, 1),
    SCENE_EXIT(2, 279, 2),
    SCENE_EXIT(3, 282, 3),
    SCENE_EXIT(4, 282, 4),
    SCENE_EXIT(5, 283, 5),
    SCENE_EXIT(6, 281, 7),
    SCENE_EXIT(7, 281, 6),
    SCENE_EXIT(8, 281, 9),
    SCENE_EXIT(9, 281, 8),
    SCENE_EXIT(10, 281, 11),
    SCENE_EXIT(11, 281, 10),
    SCENE_EXIT(12, 281, 13),
    SCENE_EXIT(13, 281, 12),
    SCENE_EXIT(14, 281, 15),
    SCENE_EXIT(15, 281, 14),
    SCENE_EXIT(16, 282, 16),
    SCENE_EXIT(17, 281, 18),
    SCENE_EXIT(18, 281, 17),
    SCENE_EXIT(20, 280, 3),
    SCENE_EXITS(282),
    SCENE_EXIT(1, 283, 1),
    SCENE_EXIT(2, 283, 2),
    SCENE_EXIT(3, 281, 3),
    SCENE_EXIT(4, 281, 4),
    SCENE_EXIT(5, 280, 5),
    SCENE_EXIT(6, 280, 6),
    SCENE_EXIT(7, 284, 7),
    SCENE_EXIT(8, 282, 9),
    SCENE_EXIT(9, 282, 8),
    SCENE_EXIT(10, 282, 11),
    SCENE_EXIT(11, 282, 10),
    SCENE_EXIT(14, 283, 14),
    SCENE_EXIT(16, 281, 16),
    SCENE_EXITS(283),
    SCENE_EXIT(1, 282, 1),
    SCENE_EXIT(2, 282, 2),
    SCENE_EXIT(3, 283, 4),
    SCENE_EXIT(4, 283, 3),
    SCENE_EXIT(5, 281, 5),
    SCENE_EXIT(6, 283, 7),
    SCENE_EXIT(7, 283, 6),
    SCENE_EXIT(8, 283, 9),
    SCENE_EXIT(9, 283, 8),
    SCENE_EXIT(10, 283, 11),
    SCENE_EXIT(11, 283, 10),
    SCENE_EXIT(12, 283, 13),
    SCENE_EXIT(13, 283, 12),
    SCENE_EXIT(14, 282, 14),
    SCENE_EXITS(284),
    SCENE_EXIT(1, 285, 1),
    SCENE_EXIT(2, 284, 3),
    SCENE_EXIT(3, 284, 2),
    SCENE_EXIT(4, 284, 5),
    SCENE_EXIT(5, 284, 4),
    SCENE_EXIT(6, 286, 6),
    SCENE_EXIT(7, 282, 7),
    SCENE_EXIT(8, 286, 8),
    SCENE_EXIT(9, 286, 9),
    SCENE_EXIT(10, 284, 11),
    SCENE_EXIT(11, 284, 10),
    SCENE_EXIT(12, 284, 13),
    SCENE_EXIT(13, 284, 12),
    SCENE_EXIT(14, 284, 15),
    SCENE_EXIT(15, 284, 14),
    SCENE_EXIT(16, 286, 10),
    SCENE_EXITS(285),
    SCENE_EXIT(1, 284, 1),
    SCENE_EXIT(2, 285, 3),
    SCENE_EXIT(3, 285, 2),
    SCENE_EXIT(4, 285, 6),
    SCENE_EXIT(5, 286, 5),
    SCENE_EXIT(6, 285, 4),
    SCENE_EXIT(7, 286, 7),
    SCENE_EXIT(11, 286, 11),
    SCENE_EXITS(286),
    SCENE_EXIT(1, 286, 2),
    SCENE_EXIT(2, 286, 1),
    SCENE_EXIT(3, 286, 4),
    SCENE_EXIT(4, 286, 3),
    SCENE_EXIT(5, 285, 5),
    SCENE_EXIT(6, 284, 6),
    SCENE_EXIT(7, 285, 7),
    SCENE_EXIT(8, 284, 8),
    SCENE_EXITS_END,
};

u32 gData_02005ae8[] = {
    39, 7, 17,
};

struct ActorPlacement gMarsLighthouseEntrance_Placements[] = {
    {SCENE_TABLE_END},
};

struct ActorPlacement gMarsLighthouseEntrance_Room1Placements[] = {
    {413, CONDITION_ALWAYS, 1, PIXELS(232), 0, PIXELS(280), FACING_EAST, 2, 0},
    {413, CONDITION_ALWAYS, 1, PIXELS(296), 0, PIXELS(280), FACING_WEST, 2, 0},
    {SCENE_TABLE_END},
};
// clang-format on

struct ActorPlacement gMarsLighthouseEntrance_Room2Placements[] = {
    {406, CONDITION_ALWAYS, (s32)gData_02005ae8, 0, 0, 0, FACING_SOUTH, 2, 0},
    {406, CONDITION_ALWAYS, (s32)gData_02005ae8, PIXELS(136), 0, PIXELS(424),
     FACING_SOUTH, 2, 0},
    {489, 800, 1, PIXELS(648), 0, PIXELS(376), FACING_SOUTH, 2, 0},
#if !TLA_PATCH_1
    {245, 105, 1, PIXELS(664), 0, PIXELS(424), FACING_SOUTH, 0, 0},
#else
    {245, 105, 1, PIXELS(728), 0, PIXELS(408), FACING_NORTHWEST, 0, 0},
#endif
    {SCENE_TABLE_END},
};

void Func_02000044(void);
void Func_02000050(void);
void MarsLighthouseEntrance_GrindIce(void);
void Func_02000648(void);
void MarsLighthouseEntrance_BurstIceSheet(void);
void Func_02000718(void);
void Func_0200085c(void);
void MarsLighthouseEntrance_PushStatueIntoFlames(void);
void Func_0200096c(void);
void Func_02000b84(void);
void Func_02000ce4(void);
void Func_02000e3c(void);
void Func_0200112c(void);
void Func_02001138(void);

// clang-format off
struct ActorPlacement gMarsLighthouseEntrance_Room3Placements[] = {
    {413, CONDITION_ALWAYS, 1, PIXELS(56), 0, PIXELS(120), FACING_SOUTH, 2, 0},
    {414, CONDITION_ALWAYS, (s32)gData_02004de8, PIXELS(104), 0, PIXELS(112), FACING_SOUTH, 2, 0},
    {414, CONDITION_ALWAYS, (s32)gData_02004e50, PIXELS(280), 0, PIXELS(112), FACING_SOUTH, 2, 0},
    {414, CONDITION_ALWAYS, 1, PIXELS(328), 0, PIXELS(112), FACING_SOUTH, 2, 0},
    {414, CONDITION_ALWAYS, 1, PIXELS(72), 0, PIXELS(112), FACING_SOUTH, 2, 0},
    {419, CONDITION_ALWAYS, (s32)gData_0200591c, PIXELS(88), 0, PIXELS(680), FACING_SOUTH, 2, 0},
    {419, CONDITION_ALWAYS, (s32)gData_0200591c, PIXELS(552), 0, PIXELS(680), FACING_SOUTH, 2, 0},
    {418, CONDITION_ALWAYS, (s32)gData_0200591c, PIXELS(88), 0, PIXELS(728), FACING_SOUTH, 2, 0},
    {406, CONDITION_ALWAYS, (s32)gData_02005ae8, 0, 0, 0, FACING_SOUTH, 2, 0},
    {406, CONDITION_ALWAYS, (s32)gData_02005ae8, PIXELS(552), 0, PIXELS(776), FACING_SOUTH, 2, 0},
    {291, CONDITION_ALWAYS, 7, PIXELS(840), 0, PIXELS(672), FACING_SOUTH, 2, 0},
    {419, CONDITION_ALWAYS, (s32)gData_0200591c, PIXELS(88), 0, PIXELS(728), FACING_SOUTH, 2, 0},
    {291, CONDITION_ALWAYS, 7, PIXELS(480), 0, PIXELS(704), FACING_SOUTH, 2, 0},
    {673, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {412, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};
// clang-format on

// clang-format off
struct ActorPlacement gMarsLighthouseEntrance_Room4Placements[] = {
    {414, CONDITION_ALWAYS, (s32)gData_02004eb8, PIXELS(840), 0, PIXELS(528), FACING_SOUTH, 2, 0},
    {419, CONDITION_ALWAYS, (s32)gData_0200591c, PIXELS(872), 0, PIXELS(568), FACING_SOUTH, 2, 0},
    {413, CONDITION_ALWAYS, 1, PIXELS(200), 0, PIXELS(792), FACING_EAST, 2, 0},
    {413, CONDITION_ALWAYS, 1, PIXELS(264), 0, PIXELS(792), FACING_WEST, 2, 0},
    {406, CONDITION_ALWAYS, (s32)gData_02005ae8, PIXELS(840), 0, PIXELS(680), FACING_SOUTH, 2, 0},
    {406, CONDITION_ALWAYS, (s32)gData_02005ae8, PIXELS(904), 0, PIXELS(744), FACING_SOUTH, 2, 1},
    {406, CONDITION_ALWAYS, (s32)gData_02005ae8, PIXELS(840), 0, PIXELS(808), FACING_SOUTH, 2, 1},
    {406, CONDITION_ALWAYS, (s32)gData_02005ae8, PIXELS(936), 0, PIXELS(824), FACING_SOUTH, 2, 1},
    {406, CONDITION_ALWAYS, (s32)gData_02005ae8, PIXELS(904), 0, PIXELS(872), FACING_SOUTH, 2, 1},
    {244, 0x54, 1, PIXELS(88), 0, PIXELS(904), FACING_SOUTH, 0, 0},
    {673, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {673, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {412, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {412, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};
// clang-format on

// clang-format off
struct ActorPlacement gMarsLighthouseEntrance_Room5Placements[] = {
    {418, CONDITION_ALWAYS, (s32)gData_0200591c, PIXELS(72), 0, PIXELS(632), FACING_SOUTH, 2, 0},
    {419, CONDITION_ALWAYS, (s32)gData_0200591c, PIXELS(72), 0, PIXELS(680), FACING_SOUTH, 2, 0},
    {418, CONDITION_ALWAYS, (s32)gData_0200591c, PIXELS(904), 0, PIXELS(120), FACING_SOUTH, 2, 0},
    {419, CONDITION_ALWAYS, (s32)gData_0200591c, PIXELS(856), 0, PIXELS(120), FACING_SOUTH, 2, 0},
    {291, CONDITION_ALWAYS, 7, PIXELS(240), 0, PIXELS(688), FACING_SOUTH, 2, 0},
    {673, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {412, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};
// clang-format on

// clang-format off
struct ActorPlacement gMarsLighthouseEntrance_Room6Placements[] = {
    {413, CONDITION_ALWAYS, 1, PIXELS(72), 0, PIXELS(696), FACING_SOUTH, 2, 0},
    {414, CONDITION_ALWAYS, (s32)gData_02004f44, PIXELS(120), 0, PIXELS(688), FACING_SOUTH, 2, 0},
    {419, CONDITION_ALWAYS, (s32)gData_0200591c, PIXELS(88), 0, PIXELS(728), FACING_SOUTH, 2, 0},
    {414, CONDITION_ALWAYS, (s32)gData_02004ff0, PIXELS(792), 0, PIXELS(128), FACING_SOUTH, 2, 0},
    {419, CONDITION_ALWAYS, (s32)gData_0200591c, PIXELS(56), 0, PIXELS(168), FACING_SOUTH, 2, 0},
    {406, CONDITION_ALWAYS, (s32)gData_02005ae8, PIXELS(200), 0, PIXELS(168), FACING_SOUTH, 2, 0},
    {406, CONDITION_ALWAYS, (s32)gData_02005ae8, PIXELS(328), 0, PIXELS(184), FACING_SOUTH, 2, 0},
    {406, CONDITION_ALWAYS, (s32)gData_02005ae8, PIXELS(360), 0, PIXELS(152), FACING_SOUTH, 2, 0},
    {406, CONDITION_ALWAYS, (s32)gData_02005ae8, PIXELS(424), 0, PIXELS(152), FACING_SOUTH, 2, 0},
    {406, CONDITION_ALWAYS, (s32)gData_02005ae8, PIXELS(664), 0, PIXELS(152), FACING_SOUTH, 2, 0},
    {406, CONDITION_ALWAYS, (s32)gData_02005ae8, PIXELS(760), 0, PIXELS(152), FACING_SOUTH, 2, 0},
    {673, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {412, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};
// clang-format on

// clang-format off
struct ActorPlacement gMarsLighthouseEntrance_Room7Placements[] = {
    {413, CONDITION_ALWAYS, 1, PIXELS(88), 0, PIXELS(152), FACING_EAST, 2, 0},
    {413, CONDITION_ALWAYS, 1, PIXELS(152), 0, PIXELS(152), FACING_WEST, 2, 0},
    {SCENE_TABLE_END},
};

struct Trigger gMarsLighthouseEntrance_Triggers[] = {
    {TRIGGERS_END},
};

struct Trigger gMarsLighthouseEntrance_Room1Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGERS_END},
};
// clang-format on

struct Trigger gMarsLighthouseEntrance_Room2Triggers[] = {
    {0x31, 1, CONDITION_ALWAYS, 1},
    {0x21, 2, CONDITION_ALWAYS, 2},
    {0x51, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {0x31, 5, CONDITION_ALWAYS, 5},
    {0x31, 6, CONDITION_ALWAYS, 6},
    {0x51, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {0x51, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_EXIT, 13, CONDITION_ALWAYS, 13},
    {TRIGGER_EXIT, 14, CONDITION_ALWAYS, 14},
    {TRIGGER_EXIT, 15, CONDITION_ALWAYS, 15},
    {TRIGGER_EXIT, 16, CONDITION_ALWAYS, 16},
    {0x8400, 11, CONDITION_ALWAYS, EVENT(Func_02000044)},
    {0xc400, 11, CONDITION_ALWAYS, EVENT(Func_02000044)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 8, 0x200, EVENT(MarsLighthouseEntrance_StartPoundIceCone)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 9, 0x201, EVENT(MarsLighthouseEntrance_StartPoundIceCone)},
    {PSYNERGY_ON_ACTOR(ABILITY_GRIND), 10, 0xa3e, EVENT(MarsLighthouseEntrance_GrindIce)},
    {TRIGGER_TALK, 10, 0xa3e, EVENT(MarsLighthouseEntrance_CheckIce)},
    {TRIGGERS_END},
};

struct Trigger gMarsLighthouseEntrance_Room3Triggers[] = {
    {0x21, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {0x51, 3, CONDITION_ALWAYS, 3},
    {0x51, 4, CONDITION_ALWAYS, 4},
    {0x31, 5, CONDITION_ALWAYS, 5},
    {0x51, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {0x51, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_EXIT, 13, CONDITION_ALWAYS, 13},
    {0x51, 14, CONDITION_ALWAYS, 14},
    {TRIGGER_EXIT, 15, CONDITION_ALWAYS, 15},
    {0x31, 16, CONDITION_ALWAYS, 16},
    {TRIGGER_STEP, 88, CONDITION_ALWAYS, EVENT(Func_0200263c)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 16, 0x202, EVENT(MarsLighthouseEntrance_StartPoundIceCone)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 17, 0x203, EVENT(MarsLighthouseEntrance_StartPoundIceCone)},
    {0x9c05, 17, CONDITION_ALWAYS, EVENT(Func_02000648)},
    {0x9c05, 18, CONDITION_ALWAYS, EVENT(MarsLighthouseEntrance_Teleport)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 8, 0xa4f, EVENT(MarsLighthouseEntrance_PushStatueIntoFlames)},
    {9, 0, CONDITION_ALWAYS, EVENT(MarsLighthouseEntrance_PushStatueIntoFlames)},
    {0x50009705, 80, 0x1230, EVENT(MarsLighthouseEntrance_BurstIceSheet)},
    {0x50009705, 81, 0x1231, EVENT(Func_0200085c)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST), 15, CONDITION_ALWAYS, 0},
    {0x60009a15, 13, CONDITION_ALWAYS, EVENT(Func_02000710)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_AFTER_END), 13, CONDITION_ALWAYS, EVENT(Func_02000718)},
    {TRIGGER_TALK, 64, CONDITION_ALWAYS, EVENT(Func_02000930)},
    {TRIGGERS_END},
};

struct Trigger gMarsLighthouseEntrance_Room4Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {0x21, 5, CONDITION_ALWAYS, 5},
    {0x21, 6, CONDITION_ALWAYS, 6},
    {0x31, 7, CONDITION_ALWAYS, 7},
    {0x51, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {0x51, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_EXIT, 13, CONDITION_ALWAYS, 13},
    {TRIGGER_EXIT, 15, CONDITION_ALWAYS, 15},
    {0x21, 16, CONDITION_ALWAYS, 16},
    {TRIGGER_STEP, 30, CONDITION_ALWAYS, EVENT(Func_020032b8)},
    {TRIGGER_STEP, 90, CONDITION_ALWAYS, EVENT(MarsLighthouseEntrance_SlideOnIce)},
    {TRIGGER_TALK, 17, CONDITION_ALWAYS, EVENT(Func_02000050)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_ACTS), 10, CONDITION_ALWAYS, EVENT(Func_0200096c)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 11, CONDITION_ALWAYS, 0},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 12, 0x204, EVENT(MarsLighthouseEntrance_StartPoundIceCone)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 13, 0x205, EVENT(MarsLighthouseEntrance_StartPoundIceCone)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 14, 0x206, EVENT(MarsLighthouseEntrance_StartPoundIceCone)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 15, 0x207, EVENT(MarsLighthouseEntrance_StartPoundIceCone)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 16, 0x208, EVENT(MarsLighthouseEntrance_StartPoundIceCone)},
    {0x9c05, 14, CONDITION_ALWAYS, EVENT(Func_0200093c)},
    {TRIGGERS_END},
};

struct Trigger gMarsLighthouseEntrance_Room5Triggers[] = {
    {0x51, 1, CONDITION_ALWAYS, 1},
    {0x51, 2, CONDITION_ALWAYS, 2},
    {0x51, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {0x21, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {0x51, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {0x51, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {0x51, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_EXIT, 13, CONDITION_ALWAYS, 13},
    {TRIGGER_STEP, 30, CONDITION_ALWAYS, EVENT(Func_020032b8)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST), 8, CONDITION_ALWAYS, 0},
    {0x60009a15, 9, CONDITION_ALWAYS, EVENT(Func_020009c0)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_AFTER_END), 9, CONDITION_ALWAYS, EVENT(Func_020009e8)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST), 10, CONDITION_ALWAYS, 0},
    {0x60009a15, 11, CONDITION_ALWAYS, EVENT(Func_02000a74)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_AFTER_END), 11, CONDITION_ALWAYS, EVENT(Func_02000a7c)},
    {0x50009705, 80, 0x1232, EVENT(Func_02000b84)},
    {0x9c05, 14, CONDITION_ALWAYS, EVENT(Func_02000af4)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, EVENT(Func_02000b24)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(Func_02000b54)},
    {TRIGGERS_END},
};

struct Trigger gMarsLighthouseEntrance_Room6Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {0x51, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {0x51, 5, CONDITION_ALWAYS, 5},
    {0x31, 6, CONDITION_ALWAYS, 6},
    {0x21, 7, CONDITION_ALWAYS, 7},
    {0x31, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {0x51, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 14, CONDITION_ALWAYS, 14},
    {0x51, 15, CONDITION_ALWAYS, 15},
    {TRIGGER_STEP, 9, CONDITION_ALWAYS, EVENT(Func_02000e3c)},
    {TRIGGER_STEP, 16, CONDITION_ALWAYS, EVENT(Func_02000e3c)},
    {TRIGGER_STEP, 35, 0x234, EVENT(Func_02000fa4)},
    {TRIGGER_STEP, 30, CONDITION_ALWAYS, EVENT(Func_02000fd4)},
    {TRIGGER_STEP, 90, CONDITION_ALWAYS, EVENT(MarsLighthouseEntrance_SlideOnIce)},
    {TRIGGER_STEP, 13, CONDITION_ALWAYS, EVENT(Func_020029a0)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 13, 0x210, EVENT(MarsLighthouseEntrance_StartPoundIceCone)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 14, 0x211, EVENT(MarsLighthouseEntrance_StartPoundIceCone)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 15, 0x212, EVENT(MarsLighthouseEntrance_StartPoundIceCone)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 16, 0x213, EVENT(MarsLighthouseEntrance_StartPoundIceCone)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 17, 0x214, EVENT(MarsLighthouseEntrance_StartPoundIceCone)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 18, 0x215, EVENT(MarsLighthouseEntrance_StartPoundIceCone)},
    {0x9c05, 12, CONDITION_ALWAYS, EVENT(Func_02000c84)},
    {0x9c05, 13, CONDITION_ALWAYS, EVENT(Func_02000cb4)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 8, 0xa4e, EVENT(Func_02000ce4)},
    {TRIGGERS_END},
};

struct Trigger gMarsLighthouseEntrance_Room7Triggers[] = {
    {0x51, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {0x51, 3, CONDITION_ALWAYS, 3},
    {0x51, 4, CONDITION_ALWAYS, 4},
    {0x31, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {0x31, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_EXIT, 13, CONDITION_ALWAYS, 13},
    {TRIGGER_EXIT, 14, CONDITION_ALWAYS, 14},
    {TRIGGER_EXIT, 15, CONDITION_ALWAYS, 15},
    {TRIGGER_EXIT, 16, CONDITION_ALWAYS, 16},
    {TRIGGER_STEP, 30, CONDITION_ALWAYS, EVENT(Func_020032b8)},
    {TRIGGER_STEP, 31, CONDITION_ALWAYS, EVENT(Func_02001138)},
    {0x202, 35, CONDITION_ALWAYS, EVENT(Func_0200112c)},
    {TRIGGERS_END},
};

u32 gData_02006868[] = {
    0,
};

s32 gData_0200686c[] = {
    (s32)gData_020057b8,
    (s32)gData_020057f4,
    (s32)gData_02005830,
};
