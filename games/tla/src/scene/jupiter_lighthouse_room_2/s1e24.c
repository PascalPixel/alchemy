/// @file
/// Scene code of overlay 0x69b, in ROM order, from s1e24's offset up to the next object the overlay links, and the overlay's data.

#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "field_effect.h"
#include "event_inline.h"
#include "../jupiter_lighthouse_aerie/jupiter_lighthouse_aerie.h"

void Event_Start(void);
void Map_CenterOnFollowedPoint(void);
s32 Math_Cosine(s32 angle);
void Effect_AdvanceMotion(union FieldObject *object);
void Func_0200165c(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags, struct EffectOptions *options);
void Event_End(void);

static __inline__ void CamBounds(s32 x, s32 y, s32 z, s32 mode) {
  Camera_MoveWithinMapBounds(x, y, z, mode);
}

static __inline__ void SetWorkTriple(s32 a, s32 b, s32 c) {
  Map_SetScreenShake(a, b, c);
}

void Func_02001e24(void) {
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
    Func_0200165c(actor->x, actor->y, actor->z, velocity[0], velocity[1],
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

extern struct CellTarget gData_02002ce0;
void Func_02001c1c();
void Func_02001c8c();

s32 Func_02001fbc(void) {
  struct Actor *object = Actor_Get(gMainRoutineWork.currentOwner);
  s32 x = object->x >> 20;
  s32 z = object->z >> 20;
  struct CellTarget *work = &gData_02002ce0;
  struct CellTargetEntry *entry = work->entries;
  s32 i;

  for (i = 0; i < work->count; i++, entry++) {
    if (entry->x == x && entry->z == z) {
      if (Event_GetFlag(entry->flag) == 0) {
        Func_02001c1c(work, entry);
        Event_SetFlag(entry->flag);
        work->current = i;
        break;
      }
      if (i == work->current)
        break;
      work->current = i;
      Func_02001c8c(work, entry);
      GameFlag_SetByte(work->flagByte, x);
      GameFlag_SetByte(work->flagByte + 8, z);
      return 1;
    }
  }
  return 0;
}

#include "cell_target.h"

struct JupiterLighthouseRoom8EventWork {
  u8 unk000[0x1ac];
  s32 screenEffect;
  u8 unk1b0[0x1c];
  struct Actor *marker;
};

void Func_0200206c(s32 x, s32 z, s32 effect) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  struct JupiterLighthouseRoom8EventWork *work = Ram_HeapSlots->eventWork;
  s32 dx = FieldGameFlag_GetByte(gData_02002ce0.flagByte);
  s32 dz = FieldGameFlag_GetByte(gData_02002ce0.flagByte + 8);

  if (dx != 0 || dz != 0) {
    struct Actor *marker;

    GameFlag_SetByte(gData_02002ce0.flagByte, 0);
    GameFlag_SetByte(gData_02002ce0.flagByte + 8, 0);
    leader->x = ((dx + x) << 20) + 0x80000;
    leader->z = ((dz + z) << 20) + 0x80000;
    marker = work->marker;
    marker->x = leader->x;
    marker->z = leader->z;
    Map_CenterOnFollowedPoint();
    Func_02001e24();
    ((struct JupiterLighthouseRoom8EventWork *)Ram_HeapSlots->eventWork)
        ->screenEffect = effect;
  }
}

#include "scene_tables.h"
#include "objcmd.h"

void Func_020000d0(void);
void Func_02000230(void);
void Func_020003f4(void);
void Func_02000410(void);
void Func_0200042c(void);
void Func_02000440(void);
void Func_02000454(void);
void Func_020004d0(void);
void Func_02000500(void);
void Func_02000674(void);
void Func_02000680(void);
void Func_020006dc(void);
void Func_02000704(void);

extern u32 gData_020027a8[];
extern u32 gData_020027e4[];
extern u32 gData_02002820[];

// clang-format off
u32 gData_020027a8[] = {
    SC_WAIT(1),
    SC_LOOP(22, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
    SC_END,
};

u32 gData_020027e4[] = {
    SC_WAIT(1),
    SC_LOOP(44, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
    SC_END,
};

u32 gData_02002820[] = {
    SC_WAIT(1),
    SC_LOOP(126, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
    SC_END,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(255),
    SCENE_EXIT(1, 254, 1),
    SCENE_EXIT(2, 255, 3),
    SCENE_EXIT(3, 255, 2),
    SCENE_EXIT(4, 255, 5),
    SCENE_EXIT(5, 255, 4),
    SCENE_EXIT(6, 256, 6),
    SCENE_EXIT(7, 256, 7),
    SCENE_EXIT(8, 256, 8),
    SCENE_EXIT(9, 256, 9),
    SCENE_EXIT(10, 256, 10),
    SCENE_EXIT(11, 256, 11),
    SCENE_EXIT(12, 257, 11),
    SCENE_EXITS_END,
};

struct ActorPlacement gActorPlacements[] = {
    {316, CONDITION_ALWAYS, 1, PIXELS(168), 0, PIXELS(104), FACING_EAST, 2, 0},
    {316, CONDITION_ALWAYS, 1, PIXELS(200), 0, PIXELS(104), FACING_EAST, 2, 1},
    {316, CONDITION_ALWAYS, 1, PIXELS(232), 0, PIXELS(104), FACING_EAST, 2, 1},
    {316, CONDITION_ALWAYS, 1, PIXELS(264), 0, PIXELS(104), FACING_EAST, 2, 1},
    {316, CONDITION_ALWAYS, 1, PIXELS(312), 0, PIXELS(104), FACING_EAST, 2, 1},
    {341, CONDITION_ALWAYS, 1, PIXELS(184), 0, PIXELS(600), FACING_SOUTHWEST, 2, 0},
    {341, CONDITION_ALWAYS, 1, PIXELS(248), 0, PIXELS(600), FACING_SOUTHEAST, 2, 1},
    {341, CONDITION_ALWAYS, 1, PIXELS(184), 0, PIXELS(728), FACING_SOUTHWEST, 2, 1},
    {341, CONDITION_ALWAYS, 1, PIXELS(248), 0, PIXELS(728), FACING_SOUTHEAST, 2, 1},
    {428, CONDITION_ALWAYS, 1, PIXELS(376), 0, PIXELS(648), FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

struct Trigger gSceneEvents[] = {
    {1, 1, CONDITION_ALWAYS, 1},
    {1, 2, CONDITION_ALWAYS, 2},
    {1, 3, CONDITION_ALWAYS, 3},
    {1, 4, CONDITION_ALWAYS, 4},
    {1, 5, CONDITION_ALWAYS, 5},
    {33, 6, CONDITION_ALWAYS, 6},
    {33, 7, CONDITION_ALWAYS, 7},
    {1, 8, CONDITION_ALWAYS, 8},
    {1, 9, CONDITION_ALWAYS, 9},
    {1, 10, CONDITION_ALWAYS, 10},
    {1, 11, CONDITION_ALWAYS, 11},
    {1, 12, CONDITION_ALWAYS, 12},
    {2, 20, CONDITION_ALWAYS, EVENT(Func_02000454)},
    {0x50008905, 30, CONDITION_ALWAYS, EVENT(Func_020003f4)},
    {6, 30, CONDITION_ALWAYS, EVENT(Func_02000230)},
    {0x50008905, 31, CONDITION_ALWAYS, EVENT(Func_02000410)},
    {6, 31, CONDITION_ALWAYS, EVENT(Func_02000230)},
    {0x50008905, 32, CONDITION_ALWAYS, EVENT(Func_0200042c)},
    {6, 32, CONDITION_ALWAYS, EVENT(Func_020000d0)},
    {0x50008905, 33, CONDITION_ALWAYS, EVENT(Func_02000440)},
    {6, 33, CONDITION_ALWAYS, EVENT(Func_020000d0)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 17, CONDITION_ALWAYS, EVENT(Func_020004d0)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 17, CONDITION_ALWAYS, EVENT(Func_02000500)},
    {8, 35, CONDITION_ALWAYS, EVENT(Func_020004d0)},
    {9, 35, CONDITION_ALWAYS, EVENT(Func_02000500)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 12, CONDITION_ALWAYS, EVENT(Func_02000674)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 12, CONDITION_ALWAYS, EVENT(Func_02000680)},
    {PSYNERGY_ANYWHERE(ABILITY_HOVER, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(Func_020006dc)},
    {PSYNERGY_ANYWHERE(ABILITY_HOVER, PSYNERGY_PHASE_AFTER_END), 0, CONDITION_ALWAYS, EVENT(Func_02000704)},
    {TRIGGERS_END},
};

s32 gData_02002b34[] = {
    -1,
};

u32 gData_02002b38[] = {
    1,
};

u32 gData_02002b3c[] = {
    39, 5, 0, 12, 38,
};
// clang-format on

s32 gData_02002b50[] = {
    (s32)gData_020027a8,
    (s32)gData_020027e4,
    (s32)gData_02002820,
};
