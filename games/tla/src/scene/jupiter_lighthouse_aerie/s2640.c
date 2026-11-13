/// @file
/// Scene code of overlay 0x6a4, in ROM order.

#include "../shared/tables.h"
#include "../shared/routines.h"
#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "field_effect.h"
#include "event_inline.h"
#include "../shared/jupiter_lighthouse_aerie.h"
#include "event_work.h"
#include "module/field/field_effect.h"
#include "module/scene/jupiter_lighthouse_aerie.h"
#include "module/scene/scene.h"

static __inline__ void CamBounds(s32 x, s32 y, s32 z, s32 mode) {
  Camera_MoveWithinMapBounds(x, y, z, mode);
}

static __inline__ void SetWorkTriple(s32 a, s32 b, s32 c) {
  Map_SetScreenShake(a, b, c);
}

DEFINE_FUNC_02001E24(Func_02002640, Func_02001e78)

DEFINE_FUNC_0200180C(Func_020027d8, gData_02003488, Func_02002438,
    Func_020024a8)

void Func_02002888(s32 dx, s32 dz, s32 value) {
  struct Actor *object = Actor_Get(gMainRoutineWork.currentOwner);
  u8 *event = Ram_HeapSlots->eventWork;
  struct CellTarget *work =
      &gData_02003488;
  s32 x = FieldGameFlag_GetByte(work->flagByte);
  s32 z = FieldGameFlag_GetByte(work->flagByte + 8);

  if (x != 0 || z != 0) {
    struct Actor *shadow;

    GameFlag_SetByte(work->flagByte, 0);
    GameFlag_SetByte(work->flagByte + 8, 0);
    object->x = ((x + dx) << 20) + 0x80000;
    object->z = ((z + dz) << 20) + 0x80000;
    shadow = *(struct Actor **)(event + 460);
    shadow->x = object->x;
    shadow->z = object->z;
    Map_CenterOnFollowedPoint();
    Func_02002640();
    *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 428) = value;
  }
}

#include "scene_tables.h"
#include "objcmd.h"

void Func_02000314(void);
void Func_0200032c(void);

extern u32 gData_02002fe4[];

// clang-format off
DEFINE_G_DATA_020012A4(gData_02002fe4, 22)

DEFINE_G_DATA_020012A4(gData_02003020, 44)

DEFINE_G_DATA_020012A4(gData_0200305c, 126)

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 560, 96, 264, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

DEFINE_G_SCENE_EXITS_2(264, 259, 1, 259, 2, 259, 3, 262, 4)

struct ActorPlacement gActorPlacements[] = {
    {489, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_EAST, 2, 1},
    {339, CONDITION_ALWAYS, 1, PIXELS(472), 0, PIXELS(256), FACING_WEST, 2, 0},
    {339, CONDITION_ALWAYS, 1, PIXELS(552), 0, PIXELS(256), FACING_EAST, 2, 0},
    {340, CONDITION_ALWAYS, 1, PIXELS(136), 0, PIXELS(200), FACING_EAST, 2, 0},
    {340, CONDITION_ALWAYS, 1, PIXELS(888), 0, PIXELS(200), FACING_WEST, 2, 0},
    {428, CONDITION_ALWAYS, 1, PIXELS(120), 0, PIXELS(168), FACING_SOUTH, 2, 0},
    {428, CONDITION_ALWAYS, 1, PIXELS(904), 0, PIXELS(168), FACING_SOUTH, 2, 0},
    {373, CONDITION_ALWAYS, 1, PIXELS(456), 0, PIXELS(424), FACING_SOUTH, 2, 0},
    {373, CONDITION_ALWAYS, 1, PIXELS(568), 0, PIXELS(424), FACING_SOUTH, 2, 0},
    {302, CONDITION_ALWAYS, 7, PIXELS(312), PIXELS(16), PIXELS(312), FACING_SOUTH, 2, 0},
    {302, CONDITION_ALWAYS, 7, PIXELS(344), PIXELS(16), PIXELS(312), FACING_SOUTH, 2, 1},
    {302, CONDITION_ALWAYS, 7, PIXELS(376), PIXELS(16), PIXELS(312), FACING_SOUTH, 2, 1},
    {302, CONDITION_ALWAYS, 7, PIXELS(648), PIXELS(16), PIXELS(312), FACING_SOUTH, 2, 1},
    {302, CONDITION_ALWAYS, 7, PIXELS(680), PIXELS(16), PIXELS(312), FACING_SOUTH, 2, 1},
    {302, CONDITION_ALWAYS, 7, PIXELS(712), PIXELS(16), PIXELS(312), FACING_SOUTH, 2, 1},
    {425, CONDITION_ALWAYS, 11, PIXELS(512), 0, PIXELS(248), FACING_EAST, 2, 0},
    {425, CONDITION_ALWAYS, 11, PIXELS(512), 0, PIXELS(248), FACING_EAST, 2, 1},
    {SCENE_TABLE_END},
};

struct Trigger gSceneEvents[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 13, CONDITION_ALWAYS, EVENT(Func_02000314)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 13, CONDITION_ALWAYS, EVENT(Func_0200032c)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 14, CONDITION_ALWAYS, EVENT(Func_02000314)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 14, CONDITION_ALWAYS, EVENT(Func_0200032c)},
    {8, -1, CONDITION_ALWAYS, EVENT(Func_02000314)},
    {9, -1, CONDITION_ALWAYS, EVENT(Func_0200032c)},
    {TRIGGERS_END},
};

s32 gData_02003314 = -1;

s32 gData_02003318 = 1;

u32 gData_0200331c[] = {
    39, 5, 0, 12, 38,
};
// clang-format on

s32 gData_02003330[] = {
    (s32)gData_02002fe4,
    (s32)gData_02003020,
    (s32)gData_0200305c,
};
