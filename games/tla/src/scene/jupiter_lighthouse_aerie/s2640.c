/// @file
/// Scene code of overlay 0x6a4, in ROM order, from s2640's offset up to the next object the overlay links, and the overlay's data.

#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "field_effect.h"
#include "event_inline.h"
#include "jupiter_lighthouse_aerie.h"
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

void Func_02002640(void) {
  struct Actor *actor = Actor_Get(gMainRoutineWork.currentOwner);
  u32 i;
  struct EffectOptions options;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  CamBounds(-1, -1, -1, 0);
  Map_CenterOnFollowedPoint();
  System_Wait(1);
  actor->y = 0x820000;
  actor->gravity = 0x8000;
  actor->bounce = 0;
  actor->motionFlags = 0;
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x100;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_PlaySound(204);
  actor->motionFlags = 3;
  FieldEvent_Wait(24);
  options.palette = 7;
  options.update = Effect_AdvanceMotion;
  options.startScaleX = 0xa3d7;
  options.startScaleY = 0xa3d7;
  i = 0;
  do {
    s32 angle;
    s32 velocity[3];

    angle = i << 12;
    velocity[0] = Math_Cosine(angle);
    velocity[1] = 0;
    velocity[2] = Math_Sin(angle);
    velocity[0] += velocity[0] >> 1;
    velocity[0] += (Random16Far() * 0xc000 >> 16) - 0x6000;
    velocity[2] += (Random16Far() * 0x6000 >> 16) - 0x3000;
    Func_02001e78(actor->x, actor->y, actor->z, velocity[0], velocity[1],
                  velocity[2], 0x1090001, &options);
    i++;
  } while (i <= 16);
  Event_PlaySound(188);
  Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 0x101);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 49);
  SetWorkTriple(0x50000, 0x50000, 0x10000);
  SetWorkTriple(-1, -1, 0xe666);
  MapGrid_WaitForShakeToSettle();
  Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 0x100);
  actor->gravity = 0x10000;
  actor->bounce = 0x4000;
  FieldEvent_Wait(10);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 1);
  Event_End();
}

s32 Func_020027d8(void) {
  struct Actor *object = Actor_Get(gMainRoutineWork.currentOwner);
  s32 x = object->x >> 20;
  s32 z = object->z >> 20;
  struct CellTarget *work = &gData_02003488;
  struct CellTargetEntry *entry = work->entries;
  s32 i;

  for (i = 0; i < work->count; i++, entry++) {
    if (entry->x == x && entry->z == z) {
      if (Event_GetFlag(entry->flag) == 0) {
        Func_02002438(work, entry);
        Event_SetFlag(entry->flag);
        work->current = i;
        break;
      }
      if (i == work->current)
        break;
      work->current = i;
      Func_020024a8(work, entry);
      GameFlag_SetByte(work->flagByte, x);
      GameFlag_SetByte(work->flagByte + 8, z);
      return 1;
    }
  }
  return 0;
}

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
u32 gData_02002fe4[] = {
    SC_WAIT(1),
    SC_LOOP(22, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
    SC_END,
};

u32 gData_02003020[] = {
    SC_WAIT(1),
    SC_LOOP(44, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
    SC_END,
};

u32 gData_0200305c[] = {
    SC_WAIT(1),
    SC_LOOP(126, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
    SC_END,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 560, 96, 264, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(264),
    SCENE_EXIT(1, 259, 1),
    SCENE_EXIT(2, 259, 2),
    SCENE_EXIT(3, 259, 3),
    SCENE_EXIT(4, 262, 4),
    SCENE_EXITS_END,
};

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
