/// @file
/// Scene code of overlay 0x6a2, in ROM order, from s17e0's offset up to the next object the overlay links, and the overlay's data.

#include "types.h"
#include "system.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "field_effect.h"
#include "event_inline.h"
#include "cell_target.h"

void Func_02001be8(void);
void Func_02001b90(void);
s32 Math_Cosine(s32 angle);
void Effect_AdvanceMotion(union FieldObject *object);
void Func_02001018(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags, struct EffectOptions *options);
void Func_02001bf0(void);

void Func_020017e0(void) {
  struct Actor *actor = Actor_Get(gMainRoutineWork.currentOwner);
  u32 i;
  struct EffectOptions options;

  Func_02001be8();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Func_02001b90();
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
    Func_02001018(actor->x, actor->y, actor->z, velocity[0], velocity[1],
                  velocity[2], 0x1090001, &options);
    i++;
  } while (i <= 16);
  Event_PlaySound(188);
  Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 0x101);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 49);
  MapGrid_SetShake(0x50000, 0x50000, 0x10000);
  MapGrid_SetShake(-1, -1, 0xe666);
  MapGrid_WaitForShakeToSettle();
  Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 0x100);
  actor->gravity = 0x10000;
  actor->bounce = 0x4000;
  FieldEvent_Wait(10);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 1);
  Func_02001bf0();
}

extern struct CellTarget Data_02002764;

void GameFlag_SetByte(s32 flag, s32 value);
void Func_020015d8(struct CellTarget *work, struct CellTargetEntry *entry);
void Func_02001648(struct CellTarget *work, struct CellTargetEntry *entry);

s32 Func_02001978(void) {
  struct Actor *object = Actor_Get(gMainRoutineWork.currentOwner);
  s32 x = object->x >> 20;
  s32 z = object->z >> 20;
  struct CellTarget *work = &Data_02002764;
  struct CellTargetEntry *entry = work->entries;
  s32 i;

  for (i = 0; i < work->count; i++, entry++) {
    if (entry->x == x && entry->z == z) {
      if (Event_GetFlag(entry->flag) == 0) {
        Func_020015d8(work, entry);
        Event_SetFlag(entry->flag);
        work->current = i;
        break;
      }
      if (i == work->current)
        break;
      work->current = i;
      Func_02001648(work, entry);
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

void Func_02001a28(s32 x, s32 z, s32 effect) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  struct JupiterLighthouseRoom8EventWork *work = Ram_HeapSlots->eventWork;
  s32 dx = ((s32)FieldGameFlag_GetByte(Data_02002764.flagByte));
  s32 dz = ((s32)FieldGameFlag_GetByte(Data_02002764.flagByte + 8));

  if (dx != 0 || dz != 0) {
    struct Actor *marker;

    GameFlag_SetByte(Data_02002764.flagByte, 0);
    GameFlag_SetByte(Data_02002764.flagByte + 8, 0);
    leader->x = ((dx + x) << 20) + 0x80000;
    leader->z = ((dz + z) << 20) + 0x80000;
    marker = work->marker;
    marker->x = leader->x;
    marker->z = leader->z;
    Func_02001b90();
    Func_020017e0();
    ((struct JupiterLighthouseRoom8EventWork *)Ram_HeapSlots->eventWork)
        ->screenEffect = effect;
  }
}

#include "scene_tables.h"
#include "objcmd.h"

void Func_02000054(void);
void Func_0200007c(void);
void Func_02000090(void);
void Func_020000e8(void);
void Func_0200018c(void);
void Func_020001bc(void);
void Func_02000228(void);
void Func_02000538(void);

extern u32 gData_0200211c[];
extern u32 gData_02002158[];
extern u32 gData_02002194[];

// clang-format off
u32 gData_0200211c[] = {
    SC_WAIT(1),
    SC_LOOP(22, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
    SC_END,
};

u32 gData_02002158[] = {
    SC_WAIT(1),
    SC_LOOP(44, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
    SC_END,
};

u32 gData_02002194[] = {
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
    SCENE_EXITS(262),
    SCENE_EXIT(1, 260, 1),
    SCENE_EXIT(2, 262, 3),
    SCENE_EXIT(3, 262, 2),
    SCENE_EXIT(4, 264, 4),
    SCENE_EXIT(5, 262, 6),
    SCENE_EXIT(6, 262, 5),
    SCENE_EXIT(7, 262, 8),
    SCENE_EXIT(8, 262, 7),
    SCENE_EXIT(13, 262, 13),
    SCENE_EXIT(14, 262, 14),
    SCENE_EXITS_END,
};

struct ActorPlacement gActorPlacements[] = {
    {489, CONDITION_ALWAYS, 11, -0x80000, 0, -0x80000, FACING_WEST, 2, 0},
    {317, CONDITION_ALWAYS, 11, PIXELS(424), 0, PIXELS(168), FACING_SOUTH, 2, 0},
    {317, CONDITION_ALWAYS, 11, PIXELS(440), 0, PIXELS(168), FACING_SOUTH, 2, 1},
    {317, CONDITION_ALWAYS, 11, PIXELS(664), 0, PIXELS(200), FACING_SOUTH, 2, 1},
    {317, CONDITION_ALWAYS, 11, PIXELS(680), 0, PIXELS(200), FACING_SOUTH, 2, 1},
    {317, CONDITION_ALWAYS, 11, PIXELS(440), 0, PIXELS(296), FACING_SOUTH, 2, 1},
    {317, CONDITION_ALWAYS, 11, PIXELS(408), 0, PIXELS(344), FACING_SOUTH, 2, 1},
    {317, CONDITION_ALWAYS, 11, PIXELS(424), 0, PIXELS(360), FACING_SOUTH, 2, 1},
    {317, CONDITION_ALWAYS, 11, PIXELS(440), 0, PIXELS(360), FACING_SOUTH, 2, 1},
    {317, CONDITION_ALWAYS, 11, PIXELS(456), 0, PIXELS(360), FACING_SOUTH, 2, 1},
    {317, CONDITION_ALWAYS, 11, PIXELS(472), 0, PIXELS(360), FACING_SOUTH, 2, 1},
    {317, CONDITION_ALWAYS, 11, PIXELS(488), 0, PIXELS(360), FACING_SOUTH, 2, 1},
    {317, CONDITION_ALWAYS, 11, PIXELS(504), 0, PIXELS(360), FACING_SOUTH, 2, 1},
    {317, CONDITION_ALWAYS, 11, PIXELS(520), 0, PIXELS(360), FACING_SOUTH, 2, 1},
    {317, CONDITION_ALWAYS, 11, PIXELS(536), 0, PIXELS(360), FACING_SOUTH, 2, 1},
    {317, CONDITION_ALWAYS, 11, PIXELS(552), 0, PIXELS(360), FACING_SOUTH, 2, 1},
    {317, CONDITION_ALWAYS, 11, PIXELS(568), 0, PIXELS(360), FACING_SOUTH, 2, 1},
    {317, CONDITION_ALWAYS, 11, PIXELS(584), 0, PIXELS(360), FACING_SOUTH, 2, 1},
    {317, CONDITION_ALWAYS, 11, PIXELS(600), 0, PIXELS(360), FACING_SOUTH, 2, 1},
    {317, CONDITION_ALWAYS, 11, PIXELS(616), 0, PIXELS(360), FACING_SOUTH, 2, 1},
    {317, CONDITION_ALWAYS, 11, PIXELS(632), 0, PIXELS(360), FACING_SOUTH, 2, 1},
    {317, CONDITION_ALWAYS, 11, PIXELS(648), 0, PIXELS(360), FACING_SOUTH, 2, 1},
    {317, CONDITION_ALWAYS, 11, PIXELS(664), 0, PIXELS(360), FACING_SOUTH, 2, 1},
    {317, CONDITION_ALWAYS, 11, PIXELS(680), 0, PIXELS(360), FACING_SOUTH, 2, 1},
    {317, CONDITION_ALWAYS, 11, PIXELS(696), 0, PIXELS(344), FACING_SOUTH, 2, 1},
    {SCENE_TABLE_END},
};

struct Trigger gSceneEvents[] = {
    {49, 1, CONDITION_ALWAYS, 1},
    {33, 2, CONDITION_ALWAYS, 2},
    {49, 3, CONDITION_ALWAYS, 3},
    {1, 4, CONDITION_ALWAYS, 4},
    {33, 5, CONDITION_ALWAYS, 5},
    {49, 6, CONDITION_ALWAYS, 6},
    {33, 7, CONDITION_ALWAYS, 7},
    {49, 8, CONDITION_ALWAYS, 8},
    {1, 9, CONDITION_ALWAYS, 9},
    {1, 10, CONDITION_ALWAYS, 10},
    {1, 11, CONDITION_ALWAYS, 11},
    {1, 12, CONDITION_ALWAYS, 12},
    {2, 30, CONDITION_ALWAYS, EVENT(Func_02000090)},
    {2, 31, CONDITION_ALWAYS, EVENT(Func_02000090)},
    {2, 50, CONDITION_ALWAYS, EVENT(Func_020000e8)},
    {2, 51, CONDITION_ALWAYS, EVENT(Func_020000e8)},
    {2, 52, CONDITION_ALWAYS, EVENT(Func_020000e8)},
    {2, 53, CONDITION_ALWAYS, EVENT(Func_020000e8)},
    {6, 200, CONDITION_ALWAYS, EVENT(Func_02000538)},
    {2, 33, CONDITION_ALWAYS, EVENT(Func_0200018c)},
    {2, 34, CONDITION_ALWAYS, EVENT(Func_02000228)},
    {2, 35, CONDITION_ALWAYS, EVENT(Func_0200018c)},
    {2, 43, CONDITION_ALWAYS, EVENT(Func_020001bc)},
    {2, 44, CONDITION_ALWAYS, EVENT(Func_020001bc)},
    {2, 45, CONDITION_ALWAYS, EVENT(Func_020001bc)},
    {0x9985, 0, CONDITION_ALWAYS, EVENT(Func_02000054)},
    {0x20009985, 0, CONDITION_ALWAYS, EVENT(Func_0200007c)},
    {TRIGGERS_END},
};

s32 gData_020025f0[] = {
    -1,
};

u32 gData_020025f4[] = {
    1,
};

u32 gData_020025f8[] = {
    39, 5, 0, 12, 38,
};
// clang-format on

s32 gData_0200260c[] = {
    (s32)gData_0200211c,
    (s32)gData_02002158,
    (s32)gData_02002194,
};
