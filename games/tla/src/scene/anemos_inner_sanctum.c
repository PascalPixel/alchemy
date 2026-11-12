#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "field_effect.h"
#include "event_work.h"
#include "event_inline.h"

void Func_02003398(void);
void Func_02003308(void);
s32 Math_Cosine(s32 angle);
u32 Random16Far(void);
void Effect_AdvanceMotion(union FieldObject *object);
void Func_02002780(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags, struct EffectOptions *options);
void Func_020033a0(void);

void Func_02002f48(void) {
  struct Actor *actor = Actor_Get(gMainRoutineWork.currentOwner);
  u32 i;
  struct EffectOptions options;
  s32 spawn = 1;

  Func_02003398();
  FieldEvent_ResolvePendingActions(0);
  /* FAKEMATCH(wrapper): the call forwarded through Call4, which builds each -1
   * in its own register; as a direct call, 8 of 169 instructions differ in
   * tla-en. */
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Func_02003308();
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
  options.update = Effect_AdvanceMotion;
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
      Func_02002780(actor->x, actor->y, actor->z, velocity[0], velocity[1],
                    velocity[2], 0x1090001, &options);
    }
    i++;
  }
  if (i <= 16)
    goto loop;
  Event_PlaySound(188);
  Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 0x101);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 49);
  /* FAKEMATCH(wrapper): the call forwarded through Call3, which builds each
   * constant in its own register; as a direct call, 26 of 169 instructions
   * differ in tla-en. */
  MapGrid_SetShake(0x50000, 0x50000, 0x10000);
  /* FAKEMATCH(wrapper): the call forwarded through Call3, which builds each -1
   * in its own register; as a direct call, 7 of 169 instructions differ in
   * tla-en. */
  MapGrid_SetShake(-1, -1, 0xe666);
  MapGrid_WaitForShakeToSettle();
  Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 0x100);
  actor->gravity = 0x10000;
  actor->bounce = 0x4000;
  FieldEvent_Wait(10);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 1);
  Func_020033a0();
}

#include "cell_target.h"

extern struct CellTarget Data_02004458;

void GameFlag_SetByte(s32 flag, s32 value);
void Func_02002d40(struct CellTarget *work, struct CellTargetEntry *entry);
void Func_02002db0(struct CellTarget *work, struct CellTargetEntry *entry);

s32 Func_020030e0(void) {
  struct Actor *object = Actor_Get(gMainRoutineWork.currentOwner);
  s32 x = object->x >> 20;
  s32 z = object->z >> 20;
  struct CellTarget *work = &Data_02004458;
  struct CellTargetEntry *entry = work->entries;
  s32 i;

  for (i = 0; i < work->count; i++, entry++) {
    if (entry->x == x && entry->z == z) {
      if (Event_GetFlag(entry->flag) == 0) {
        Func_02002d40(work, entry);
        Event_SetFlag(entry->flag);
        work->current = i;
        break;
      }
      if (i == work->current)
        break;
      work->current = i;
      Func_02002db0(work, entry);
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

void Func_02003190(s32 x, s32 z, s32 effect) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  struct JupiterLighthouseRoom8EventWork *work = Ram_HeapSlots->eventWork;
  s32 dx = FieldGameFlag_GetByte(Data_02004458.flagByte);
  s32 dz = FieldGameFlag_GetByte(Data_02004458.flagByte + 8);

  if (dx != 0 || dz != 0) {
    struct Actor *marker;

    GameFlag_SetByte(Data_02004458.flagByte, 0);
    GameFlag_SetByte(Data_02004458.flagByte + 8, 0);
    leader->x = ((dx + x) << 20) + 0x80000;
    leader->z = ((dz + z) << 20) + 0x80000;
    marker = work->marker;
    marker->x = leader->x;
    marker->z = leader->z;
    Func_02003308();
    Func_02002f48();
    ((struct JupiterLighthouseRoom8EventWork *)Ram_HeapSlots->eventWork)
        ->screenEffect = effect;
  }
}

#include "scene_tables.h"

void Func_02000654(void);
void Func_02000660(void);
void Func_020006f8(void);
void Func_02000794(void);
void Func_02000ac8(void);
void Func_02001090(void);
void Func_0200109c(void);
void Func_02001160(void);
void Func_02001398(void);
void Func_02001518(void);
void Func_02001614(void);
void Func_02001644(void);
void Func_02001660(void);
void Func_020016fc(void);
void Func_0200178c(void);
void Func_020017a0(void);
void Func_02001938(void);
void Func_020019a8(void);
void Func_02001a94(void);
void Func_02001b88(void);
void Func_02001c60(void);

extern u32 gData_020034c0[];
extern u32 gData_020034fc[];
extern u32 gData_02003538[];
extern u32 gData_0200378c[];
extern u32 gData_020037c8[];
extern u32 gData_02003804[];
extern s32 gData_020038fc[];
extern s16 gData_020043fa[];

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

s32 gData_02003584[] = {
    46,
    (s32)Func_02000660,
    38,
    17,
};

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

u16 gData_020038b4[] = {
    94, 89, 90, 91, 92, 93, 93, 94,
    89, 90, 91, 92, 92, 93, 94, 89,
    90, 91, 91, 92, 93, 94, 89, 90,
    90, 91, 92, 93, 94, 89, 89, 90,
    91, 92, 93, 94,
};

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

u32 gData_02003b38[] = {
    0xffff, 0, 0, 0, 0, 0,
};

u32 gData_02003b50[] = {
    0xffff, 0, 0, 0, 0, 0,
};

s32 gData_02003b68[] = {
    (s32)0xffff0110,
    7,
    1,
    0,
    0,
    0x202c000,
    (s32)0xffff01b0,
    (s32)gData_020038fc,
    2,
    0,
    0,
    0x202c000,
    (s32)0xffff0110,
    7,
    3,
    0,
    0,
    0x202c000,
    0xffff,
    0,
    0,
    0,
    0,
    0,
};

u32 gData_02003bc8[] = {
    0xffff0110, 7, 1, 0, 0, 0x202c000,
    0xffff0149, 1, 2, 0, 0, 0x202c000,
    0xffff0110, 7, 3, 0, 0, 0x202c000,
    0xffff0110, 7, 4, 0, 0, 0x202c000,
    0xffff0110, 7, 5, 0, 0, 0x202c000,
    0xffff0110, 7, 6, 0, 0, 0x202c000,
    0xffff01e9, 7, 7, 0, 0, 0x202c000,
    0xffff, 0, 0, 0, 0, 0,
};

s32 gData_02003c88[] = {
    (s32)0xffff01b0,
    (s32)gData_020038fc,
    3,
    0,
    0,
    0x202c000,
    (s32)0xffff0110,
    7,
    4,
    0,
    0,
    0x202c000,
    (s32)0xffff0110,
    7,
    5,
    0,
    0,
    0x202c000,
    0xffff,
    0,
    0,
    0,
    0,
    0,
};

s32 gData_02003ce8[] = {
    (s32)0xffff01af,
    7,
    1,
    0,
    0,
    0x202c000,
    (s32)0xffff01af,
    7,
    2,
    0,
    0,
    0x202c000,
    (s32)0xffff01af,
    7,
    3,
    0,
    0,
    0x202c000,
    (s32)0xffff01b0,
    (s32)gData_020038fc,
    4,
    0,
    0,
    0x202c000,
    (s32)0xffff01ad,
    (s32)gData_020038fc,
    0x2480000,
    0,
    0x2460000,
    0x18000,
    (s32)0xffff01ad,
    (s32)gData_020038fc,
    0x2480000,
    0,
    0x2760000,
    0x1018000,
    (s32)0xffff01ad,
    (s32)gData_020038fc,
    0x2a80000,
    0,
    0x2460000,
    0x1010000,
    (s32)0xffff01ad,
    (s32)gData_020038fc,
    0x2a80000,
    0,
    0x2760000,
    0x1010000,
    0xffff,
    0,
    0,
    0,
    0,
    0,
};

s32 gData_02003dc0[] = {
    -1, 0, 0,
};

s32 gData_02003dcc[] = {
    1,
    (s32)0xffff0001,
    1,
    1,
    (s32)0xffff0002,
    2,
    2,
    (s32)0xffff0036,
    (s32)Func_02001090,
    2,
    0x2000032,
    (s32)Func_02000ac8,
    2,
    0x2000033,
    (s32)Func_02000ac8,
    2,
    0x2000034,
    (s32)Func_02000ac8,
    2,
    0x2000035,
    (s32)Func_02000ac8,
    0x9c05,
    (s32)0xffff001e,
    (s32)Func_02001c60,
    -1,
    0,
    0,
};

s32 gData_02003e38[] = {
    49,
    (s32)0xffff0001,
    1,
    49,
    (s32)0xffff0002,
    2,
    1,
    (s32)0xffff0003,
    3,
    49,
    (s32)0xffff0004,
    4,
    33,
    (s32)0xffff0005,
    5,
    1,
    (s32)0xffff0006,
    6,
    1,
    (s32)0xffff0007,
    7,
    1,
    (s32)0xffff0008,
    8,
    1,
    (s32)0xffff0009,
    9,
    1,
    (s32)0xffff000a,
    10,
    33,
    (s32)0xffff000c,
    12,
    1,
    (s32)0xffff000d,
    13,
    1,
    (s32)0xffff000e,
    14,
    1,
    (s32)0xffff000f,
    15,
    33,
    (s32)0xffff0010,
    16,
    1,
    (s32)0xffff0011,
    17,
    2,
    0x1207000b,
    (s32)Func_02001518,
    2,
    0x2000032,
    (s32)Func_02001398,
    2,
    0x2010033,
    (s32)Func_02001398,
    2,
    0x2020034,
    (s32)Func_02001398,
    2,
    0x2030035,
    (s32)Func_02001398,
    2,
    0x2040036,
    (s32)Func_02001398,
    2,
    0x2050037,
    (s32)Func_02001398,
    2,
    0x2060038,
    (s32)Func_02001398,
    8,
    -0x10000,
    (s32)Func_0200109c,
    9,
    -0x10000,
    (s32)Func_02001160,
    0x10008c15,
    0xa800008,
    (s32)Func_0200109c,
    0x8c15,
    0xa800008,
    (s32)Func_02001160,
    0x10008c15,
    0xa81000a,
    (s32)Func_0200109c,
    0x8c15,
    0xa81000a,
    (s32)Func_02001160,
    -1,
    0,
    0,
};

s32 gData_02003fac[] = {
    1,
    (s32)0xffff0001,
    1,
    49,
    (s32)0xffff0002,
    2,
    1,
    (s32)0xffff0003,
    3,
    1,
    (s32)0xffff0004,
    4,
    1,
    (s32)0xffff0005,
    5,
    33,
    (s32)0xffff0006,
    6,
    49,
    (s32)0xffff0007,
    7,
    33,
    (s32)0xffff0008,
    8,
    1,
    (s32)0xffff0009,
    9,
    1,
    (s32)0xffff000a,
    10,
    1,
    (s32)0xffff000b,
    11,
    1,
    (s32)0xffff000c,
    12,
    1,
    (s32)0xffff000d,
    13,
    1,
    (s32)0xffff000e,
    14,
    1,
    (s32)0xffff000f,
    15,
    1,
    (s32)0xffff0010,
    16,
    1,
    (s32)0xffff0011,
    17,
    0x8b85,
    -0x10000,
    (s32)Func_02001614,
    0x40008b85,
    -0x10000,
    (s32)Func_02001644,
    2,
    0x2000032,
    (s32)Func_02001660,
    2,
    0x12000033,
    (s32)Func_020016fc,
    0x8f15,
    (s32)0xffff0009,
    0,
    8,
    -0x10000,
    (s32)Func_0200109c,
    9,
    -0x10000,
    (s32)Func_02001160,
    0x10008c15,
    0xa820008,
    (s32)Func_0200109c,
    0x8c15,
    0xa820008,
    (s32)Func_02001160,
    0x10008c15,
    0xa83000a,
    (s32)Func_0200109c,
    0x8c15,
    0xa83000a,
    (s32)Func_02001160,
    0x10008c15,
    (s32)0xffff000b,
    (s32)Func_0200109c,
    0x8c15,
    (s32)0xffff000b,
    (s32)Func_02001160,
    0x10008c15,
    0xa85000c,
    (s32)Func_0200109c,
    0x8c15,
    0xa85000c,
    (s32)Func_02001160,
    0x10008c15,
    0xa84000d,
    (s32)Func_0200109c,
    0x8c15,
    0xa84000d,
    (s32)Func_02001160,
    -1,
    0,
    0,
};

s32 gData_02004150[] = {
    1,
    (s32)0xffff0001,
    1,
    33,
    (s32)0xffff0002,
    2,
    33,
    (s32)0xffff0003,
    3,
    1,
    (s32)0xffff0004,
    4,
    1,
    (s32)0xffff0005,
    5,
    1,
    (s32)0xffff0006,
    6,
    33,
    (s32)0xffff0007,
    7,
    1,
    (s32)0xffff0008,
    8,
    49,
    (s32)0xffff0009,
    9,
    1,
    (s32)0xffff000a,
    10,
    2,
    0x1207000b,
    (s32)Func_02001518,
    2,
    0x2000032,
    (s32)Func_02001398,
    2,
    0x2010033,
    (s32)Func_02001398,
    2,
    0x2020034,
    (s32)Func_02001398,
    2,
    0x2030035,
    (s32)Func_02001398,
    2,
    0x2040036,
    (s32)Func_02001398,
    2,
    0x2050037,
    (s32)Func_02001398,
    2,
    0x2060038,
    (s32)Func_02001398,
    2,
    (s32)0xffff0021,
    (s32)Func_0200178c,
    2,
    (s32)0xffff0022,
    (s32)Func_0200178c,
    2,
    (s32)0xffff0023,
    (s32)Func_020017a0,
    2,
    (s32)0xffff0024,
    (s32)Func_020017a0,
    0x8b85,
    -0x10000,
    (s32)Func_02001614,
    0x40008b85,
    -0x10000,
    (s32)Func_02001644,
    8,
    -0x10000,
    (s32)Func_0200109c,
    9,
    -0x10000,
    (s32)Func_02001160,
    0x10008c15,
    0xa860009,
    (s32)Func_0200109c,
    0x8c15,
    0xa860009,
    (s32)Func_02001160,
    0x10008c15,
    (s32)0xffff000a,
    (s32)Func_0200109c,
    0x8c15,
    (s32)0xffff000a,
    (s32)Func_02001160,
    -1,
    0,
    0,
};

s32 gData_020042c4[] = {
    1,
    (s32)0xffff0001,
    1,
    1,
    (s32)0xffff0002,
    2,
    1,
    (s32)0xffff0003,
    3,
    1,
    (s32)0xffff0004,
    4,
    1,
    (s32)0xffff0005,
    5,
    1,
    (s32)0xffff0006,
    6,
    49,
    (s32)0xffff0007,
    7,
    1,
    (s32)0xffff0008,
    8,
    49,
    (s32)0xffff0009,
    9,
    1,
    (s32)0xffff000a,
    10,
    1,
    (s32)0xffff000b,
    11,
    2,
    0x2000032,
    (s32)Func_02001938,
    2,
    0x2000033,
    (s32)Func_02001938,
    2,
    0x2000034,
    (s32)Func_02001938,
    2,
    0xa8c001e,
    (s32)Func_02001b88,
    6,
    (s32)0xffff00c8,
    (s32)Func_020019a8,
    6,
    (s32)0xffff00c9,
    (s32)Func_02001a94,
    -1,
    0,
    0,
    1,
    (s32)0xffff0001,
    1,
    1,
    (s32)0xffff0002,
    2,
    1,
    (s32)0xffff0003,
    3,
    1,
    (s32)0xffff0004,
    4,
    1,
    (s32)0xffff0005,
    5,
    -1,
    0,
    0,
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
