/// @file
/// Scene code of overlay 0x69a, in ROM order, from s1704's offset up to the next object the overlay links, and the overlay's data.

#include "../shared/tables.h"
#include "../shared/routines.h"
#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "field_effect.h"
#include "event_inline.h"
#include "../jupiter_lighthouse_aerie/jupiter_lighthouse_aerie.h"
#include "event_work.h"
#include "module/field/field_effect.h"
#include "module/scene/jupiter_lighthouse_room_1.h"
#include "module/scene/scene.h"

void Func_02000f3c(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags, struct EffectOptions *options);

static __inline__ void CamBounds(s32 x, s32 y, s32 z, s32 mode) {
  Camera_MoveWithinMapBounds(x, y, z, mode);
}

static __inline__ void SetWorkTriple(s32 a, s32 b, s32 c) {
  Map_SetScreenShake(a, b, c);
}

void Func_02001704(void) {
  struct Actor *actor = Actor_Get(gMainRoutineWork.currentOwner);
  u32 i;
  struct EffectOptions options;
  s32 spawn = 1;

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
  // A goto loop carries no loop notes: as a do, for or while loop, loop.c
  // moves the velocity array's address out of the loop.
loop:
  {
    s32 angle;
    s32 velocity[3];
    s32 cosine;

    angle = i << 12;
    cosine = Math_Cosine(angle);
    /* FAKEMATCH(dead): spawn is always 1, a constant GCSE only folds after its
     * partial redundancy pass, so that pass leaves the velocity array's
     * address in the loop, built after the cosine call as the native has it;
     * as the plain body, 4 of 169 instructions differ in tla-en. */
    if (spawn) {
      velocity[0] = cosine;
      velocity[1] = 0;
      velocity[2] = Math_Sin(angle);
      velocity[0] += velocity[0] >> 1;
      velocity[0] += (Random16Far() * 0xc000 >> 16) - 0x6000;
      velocity[2] += (Random16Far() * 0x6000 >> 16) - 0x3000;
      Func_02000f3c(actor->x, actor->y, actor->z, velocity[0], velocity[1],
                    velocity[2], 0x1090001, &options);
    }
    i++;
  }
  if (i <= 16)
    goto loop;
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

extern struct CellTarget gData_020023a0;
void Func_020014fc();
void Func_0200156c();

DEFINE_FUNC_0200180C(Func_0200189c, gData_020023a0, Func_020014fc,
    Func_0200156c)

#include "cell_target.h"

DEFINE_FUNC_020018BC(Func_0200194c, gData_020023a0, Func_02001704)

#include "scene_tables.h"
#include "objcmd.h"

void Func_02000054(void);
void Func_020001f4(void);

// clang-format off
DEFINE_G_DATA_020012A4(gData_02002050, 22)

DEFINE_G_DATA_020012A4(gData_0200208c, 44)

DEFINE_G_DATA_020012A4(gData_020020c8, 126)

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(254),
    SCENE_EXIT(1, 255, 1),
    SCENE_EXIT(2, 251, 2),
    SCENE_EXIT(3, 251, 3),
    SCENE_EXIT(4, 254, 5),
    SCENE_EXIT(5, 254, 4),
    SCENE_EXIT(6, 252, 6),
    SCENE_EXITS_END,
};

struct ActorPlacement gActorPlacements[] = {
    {341, CONDITION_ALWAYS, 1, PIXELS(200), 0, PIXELS(744), FACING_SOUTHWEST, 2, 0},
    {341, CONDITION_ALWAYS, 1, PIXELS(296), 0, PIXELS(744), FACING_SOUTHEAST, 2, 1},
    {312, CONDITION_ALWAYS, 1, PIXELS(232), 0, PIXELS(536), FACING_EAST, 2, 0},
    {SCENE_TABLE_END},
};

struct Trigger gSceneEvents[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_STEP, 20, 0xa01, EVENT(Func_020001b4)},
    {TRIGGER_STEP, 21, 0xa01, EVENT(Func_020001d4)},
    {0x50008905, 30, CONDITION_ALWAYS, EVENT(Func_02000054)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 10, 0xa01, EVENT(Func_020001f4)},
    {TRIGGERS_END},
};

s32 gData_0200222c[] = {
    -1,
};

u32 gData_02002230[] = {
    1,
};

u32 gData_02002234[] = {
    39, 5, 0, 12, 38,
};
// clang-format on

s32 gData_02002248[] = {
    (s32)gData_02002050,
    (s32)gData_0200208c,
    (s32)gData_020020c8,
};
