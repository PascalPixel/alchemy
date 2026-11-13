#include "types.h"
#include "system.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "field_effect.h"
#include "event_inline.h"

void Event_Start(void);
void Map_CenterOnFollowedPoint(void);
s32 Math_Cosine(s32 angle);
void Effect_AdvanceMotion(union FieldObject *object);
void Func_0200363c(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags, struct EffectOptions *options);
void Event_End(void);

static __inline__ void CamBounds(s32 x, s32 y, s32 z, s32 mode) {
  Camera_MoveWithinMapBounds(x, y, z, mode);
}

static __inline__ void SetWorkTriple(s32 a, s32 b, s32 c) {
  Map_SetScreenShake(a, b, c);
}

void Func_02003e04(void) {
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
    Func_0200363c(actor->x, actor->y, actor->z, velocity[0], velocity[1],
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

#include "cell_target.h"

extern struct CellTarget gData_02004cf4;

void GameFlag_SetByte(s32 flag, s32 value);
void Func_02003bfc(struct CellTarget *work,
                   struct CellTargetEntry *entry);
void Func_02003c6c(struct CellTarget *work,
                   struct CellTargetEntry *entry);

s32 Func_02003f9c(void) {
  struct Actor *object = Actor_Get(gMainRoutineWork.currentOwner);
  s32 x = object->x >> 20;
  s32 z = object->z >> 20;
  struct CellTarget *work = &gData_02004cf4;
  struct CellTargetEntry *entry = work->entries;
  s32 i;

  for (i = 0; i < work->count; i++, entry++) {
    if (entry->x == x && entry->z == z) {
      if (Event_GetFlag(entry->flag) == 0) {
        Func_02003bfc(work, entry);
        Event_SetFlag(entry->flag);
        work->current = i;
        break;
      }
      if (i == work->current)
        break;
      work->current = i;
      Func_02003c6c(work, entry);
      GameFlag_SetByte(work->flagByte, x);
      GameFlag_SetByte(work->flagByte + 8, z);
      return 1;
    }
  }
  return 0;
}

struct JupiterLighthouseRoom8EventWork {
  u8 unk000[0x1ac];
  s32 screenEffect;
  u8 unk1b0[0x1c];
  struct Actor *marker;
};

void Func_0200404c(s32 x, s32 z, s32 effect) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  struct JupiterLighthouseRoom8EventWork *work = Ram_HeapSlots->eventWork;
  s32 dx = FieldGameFlag_GetByte(gData_02004cf4.flagByte);
  s32 dz = FieldGameFlag_GetByte(gData_02004cf4.flagByte + 8);

  if (dx != 0 || dz != 0) {
    struct Actor *marker;

    GameFlag_SetByte(gData_02004cf4.flagByte, 0);
    GameFlag_SetByte(gData_02004cf4.flagByte + 8, 0);
    leader->x = ((dx + x) << 20) + 0x80000;
    leader->z = ((dz + z) << 20) + 0x80000;
    marker = work->marker;
    marker->x = leader->x;
    marker->z = leader->z;
    Map_CenterOnFollowedPoint();
    Func_02003e04();
    ((struct JupiterLighthouseRoom8EventWork *)Ram_HeapSlots->eventWork)
        ->screenEffect = effect;
  }
}

#include "scene_tables.h"
#include "text/msg_ids.h"
#include "objcmd.h"

void Func_02000038(void);
void Func_02000060(void);
void Func_02000074(void);
void Func_02000188(void);
void Func_020001e8(void);
void Func_0200024c(void);
void Func_020005b0(void);
void Func_02000ba4(void);
void Func_02000bd8(void);
void Func_02000e50(void);
void Func_02000e80(void);
void Func_0200236c(void);
void Func_02002c44(void);
void Func_02002ce8(void);

void Func_0200007c(void);

extern u32 gData_0200485c[];
extern u32 gData_02004898[];
extern u32 gData_020048d4[];
extern s32 gData_02004910[];

// clang-format off
u32 gData_02004474[] = {
    0, 40, 0x80010000, 22, 25, 0,
    0, 1, 22, 25, 1, 0,
    1, 13, 10, 0xc0010000, 38,
};

s32 gData_020044b8[] = {
    46,
    (s32)Func_02002c44,
    4,
    0x2680000,
    0,
    0x1080000,
    1,
    0,
    2,
    4,
    0x2680000,
    0,
    0xf80000,
    1,
    0,
    2,
    4,
    0x2380000,
    0,
    0xf00000,
    1,
    0,
    2,
    4,
    0x2380000,
    0x200000,
    0xf00000,
    1,
    0,
    2,
    4,
    0x2380000,
    0x200000,
    0xd80000,
    1,
    0,
    2,
    4,
    0x2400000,
    0x200000,
    0xd80000,
    1,
    0,
    2,
    4,
    0x2400000,
    0x160000,
    0xd80000,
    1,
    0,
    2,
    4,
    0x2500000,
    0x160000,
    0xd80000,
    1,
    0,
    2,
    22,
    37,
    (s32)Func_02000188,
    46,
    (s32)Func_02002ce8,
    17,
};

s32 gData_020045b8[] = {
    46,
    (s32)Func_02002c44,
    4,
    0x2a80000,
    0,
    0x1080000,
    1,
    0,
    2,
    4,
    0x2a80000,
    0,
    0x1280000,
    1,
    0,
    2,
    4,
    0x2d80000,
    0,
    0x1280000,
    1,
    0,
    2,
    4,
    0x2d80000,
    0,
    0x1080000,
    1,
    0,
    2,
    4,
    0x3280000,
    0,
    0x1080000,
    1,
    0,
    2,
    4,
    0x3280000,
    0,
    0xf00000,
    1,
    0,
    2,
    4,
    0x3280000,
    0x180000,
    0xf00000,
    1,
    0,
    2,
    4,
    0x3180000,
    0x180000,
    0xf00000,
    1,
    0,
    2,
    4,
    0x3180000,
    0x200000,
    0xf00000,
    1,
    0,
    2,
    4,
    0x3180000,
    0x200000,
    0xd80000,
    1,
    0,
    2,
    4,
    0x3100000,
    0x200000,
    0xd80000,
    1,
    0,
    2,
    4,
    0x3100000,
    0x160000,
    0xd80000,
    1,
    0,
    2,
    4,
    0x3000000,
    0x160000,
    0xd80000,
    1,
    0,
    2,
    22,
    37,
    (s32)Func_020001e8,
    46,
    (s32)Func_02002ce8,
    17,
};

u32 gData_02004744[] = {
    SC_WAIT(30),
    SC_SET(SW_HOOK, 0),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
};

u32 gData_02004774[] = {
    0, 20, 0x80010000, 22, 25, 0,
    0, 1, 22, 25, 1, 0,
    1, 13, 10, 0xc0010000, 38,
};

u32 gData_020047b8[] = {
    0, 20, 0x80010000, 22, 25, 0,
    0, 1, 22, 25, 1, 0,
    1, 13, 10, 0xc0010000, 38,
};

s32 gData_020047fc[] = {
    0xffff, -0x8000, (s32)0xe000c000, (s32)0xc000a000, 0x20004000, 0x40006000,
    0xffff, -0x8000, 0xffff, -0x8000, -0x4000, (s32)0xc000ffff,
    -0xc000, 0x4000ffff, 0xffff, -0x8000,
};

s32 gData_0200483c[] = {
    0xffff, -0x8000, 0xc000, (s32)0xc0008000, 0x4000, 0x40008000,
    0xffff, -0x8000,
};

u32 gData_0200485c[] = {
    SC_WAIT(1),
    SC_LOOP(22, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
    SC_END,
};

u32 gData_02004898[] = {
    SC_WAIT(1),
    SC_LOOP(44, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
    SC_END,
};

u32 gData_020048d4[] = {
    SC_WAIT(1),
    SC_LOOP(126, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
    SC_END,
};

s32 gData_02004910[] = {
    SC_CALL(Func_02000038),
    SC_END,
};

s32 gData_0200491c[] = {
    SC_ADD(SW_SCALE_X, 0x400),
    SC_ADD(SW_SCALE_Y, 0x400),
    SC_ADD(SW_Y, -0x4000),
    SC_WAIT(1),
    SC_LOOP(40, SC_TOP),
    SC_END,
};

s32 gData_02004958[] = {
    23, 9, -2048, 23, 10, -2048,
    23, 6, 0x4000, 0, 1, 13,
    24, 0, 3, 0, 1, 17,
};

s32 gData_020049a0[] = {
    23, 9, -1024, 23, 10, -1024,
    0, 1, 13, 40, 0, 3,
    0, 1, 17,
};

s32 gData_020049dc[] = {
    23, 9, 0x400, 23, 10, -1024,
    0, 1, 13, 40, 0, 3,
    0, 1, 17,
};
// clang-format on

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 192, 0, 192, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(286),      SCENE_EXIT(1, 286, 2), SCENE_EXIT(2, 286, 1),
    SCENE_EXIT(3, 286, 4), SCENE_EXIT(4, 286, 3), SCENE_EXIT(5, 285, 5),
    SCENE_EXIT(6, 284, 6), SCENE_EXIT(7, 285, 7), SCENE_EXIT(8, 284, 8),
    SCENE_EXITS_END,
};

struct ActorPlacement gActorPlacements[] = {
    {418, CONDITION_ALWAYS, (s32)gData_02004910, PIXELS(712), 0, PIXELS(264),
     FACING_SOUTH, 2, 0},
    {419, CONDITION_ALWAYS, (s32)gData_02004910, PIXELS(648), 0, PIXELS(264),
     FACING_SOUTH, 2, 0},
    {291, CONDITION_ALWAYS, 7, PIXELS(680), 0, PIXELS(240), FACING_SOUTH, 2, 0},
    {181, 0xa30, 7, PIXELS(664), 0, PIXELS(216), FACING_SOUTH, 2, 0},
    {181, 0xa30, 7, PIXELS(696), 0, PIXELS(216), FACING_SOUTH, 2, 1},
    {33, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {23, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {303, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {303, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 1},
    {673, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {673, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {412, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {412, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

struct Trigger gSceneEvents[] = {
    {1, 1, CONDITION_ALWAYS, 1},
    {81, 2, CONDITION_ALWAYS, 2},
    {81, 3, CONDITION_ALWAYS, 3},
    {1, 4, CONDITION_ALWAYS, 4},
    {33, 5, CONDITION_ALWAYS, 5},
    {33, 6, CONDITION_ALWAYS, 6},
    {33, 7, CONDITION_ALWAYS, 7},
    {33, 8, CONDITION_ALWAYS, 8},
    {33, 9, CONDITION_ALWAYS, 9},
    {1, 10, CONDITION_ALWAYS, 10},
    {1, 11, CONDITION_ALWAYS, 11},
    {2, 90, 0x1a4b, EVENT(Func_0200236c)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST), 8, CONDITION_ALWAYS, EVENT(Func_02000060)},
    {0x60009a15, 9, CONDITION_ALWAYS, EVENT(Func_02000074)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_AFTER_END), 9, CONDITION_ALWAYS, (u32)(Func_0200007c)},
    {6, 20, CONDITION_ALWAYS, EVENT(Func_020005b0)},
    {0x50009705, 20, 0x1233, EVENT(Func_0200024c)},
    {0, 14, CONDITION_ALWAYS, EVENT(Func_02000ba4)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgMarsStar05)},
    {0, 13, CONDITION_ALWAYS, EVENT(Func_02000bd8)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgMarsStar04)},
    {3, 21, CONDITION_ALWAYS, EVENT(Func_02000e50)},
    {0xde04, 1045, CONDITION_ALWAYS, EVENT(Func_02000e80)},
    {TRIGGERS_END},
};

s32 gData_02004ce0[] = {
    (s32)gData_0200485c,
    (s32)gData_02004898,
    (s32)gData_020048d4,
};
