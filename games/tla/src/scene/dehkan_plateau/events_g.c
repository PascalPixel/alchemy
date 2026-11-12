/// @file
/// Scene code of overlay 0x655, in ROM order, from events_g's offset up to the next object the overlay links.

#include "types.h"
#include "scene_tables.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "field.h"

extern const u8 gData_02003f44[];
extern const u8 gData_02003f52[];
extern const u8 gData_02003f80[];
extern const u8 gData_02003f92[];
extern const u8 gData_02003f98[];
extern const u8 gData_02003fa0[];
extern const u8 gData_02003fa8[];
extern const u8 gData_02003fb0[];
extern const u8 gData_02003fb8[];
extern const u8 gData_02003fc0[];
extern const u8 gData_02003fc4[];

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void wait(s32 frames);
struct Actor *Actor_GetById(s32 id);
void Func_020000a4(const void *data, s32 a, s32 b);
void Func_020002f8(const void *data);
void Func_02000708(const void *data);
void Func_02001474(s32 show);
void Func_02002180(void);
void Map_CopyCellAttributeRect(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02003c28(void);
void Func_02003c30(void);
void Event_ResolvePendingActions(s32 skip);
void Func_02003dc0(const void *data);
void Func_02003dc8(void);
void Sound_Play(s32 cue);
void Actor_SetHorizontalPositionWithTerrain(s32 id, s32 x, s32 z);
void Func_02003de8(void);
void Map_CopyCellPictureIndicesRect(s32 srcX, s32 srcY, s32 destX, s32 destY, s32 width,
                   s32 height);
void Func_020015b8(struct Actor *object);
void Func_0200172c(void);
void Func_02001804(void);
void Func_020021b4(void);
void Func_02002604(void);

struct CellRecord {
  u16 present;
  u16 x;
  u16 y;
  u16 unk06[3];
};

extern const u16 gData_02003fd2[];
extern const struct CellRecord gData_02004066[];

void Actor_SetHorizontalPositionAndAngleWithTerrain(u32 id, s32 x, s32 z, s16 angle);
struct Actor *Func_080d47b4(s32 id, s32 a, s32 b);
void Func_02003cf0(void);
struct Actor *Func_080d4568(void);
void Func_02003d50(s32 a, s32 b);
void Func_02003d58(s32 a, s32 b);
void Func_02003d60(void);
void Func_02003d68(void);
void Func_02003d70(void);
void Camera_MoveWithinMapBounds(s32 a, s32 b, s32 c, s32 d);
void Actor_SetAngleToward(s32 id, s32 target, s32 mode);
void Actor_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void Actor_SetPositionAndReset(s32 id, s32 x, s32 z);
void Actor_Jump(s32 id, s32 a, s32 b);
void Actor_ArmCallback(s32 id, s32 a, s32 b);
void Actor_WatchAndSetScript(s32 id, s32 target);
void Event_Wait(s32 frames);
void Task_Add(void (*func)(void), s32 priority);
void Task_RemoveFar(void (*func)(void));
void Field_DispatchTypeHandler(s32 type);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void Camera_SetBounds(s32 a, s32 b, s32 c, s32 d) {
  Camera_MoveWithinMapBounds(a, b, c, d);
}

static __inline__ void Actor_SetSpeed(s32 id, s32 speed, s32 acceleration) {
  Actor_SetSpeedParameters(id, speed, acceleration);
}

static __inline__ void Actor_MoveTo(s32 id, s32 x, s32 z) {
  Actor_SetPositionAndReset(id, x, z);
}

static __inline__ void Actor_PlaceFacing(s32 id, s32 x, s32 z, s32 angle) {
  Actor_SetHorizontalPositionAndAngleWithTerrain(id, x, z, angle);
}

void Func_020021e8(void) {
  Func_02003c28();
  Event_ResolvePendingActions(0);
  Func_080d4568()->motionFlags = 0;
  Camera_MoveWithinMapBounds(658 << 16, -1, 164 << 16, 1);
  Func_02003cf0();
  Actor_SetAngleToward(24, gMainRoutineWork.currentOwner, 0);
  Func_080d47b4(24, 0x100, 40);
  Actor_SetSpeed(24, 0x19999, 0xcccc);
  Actor_MoveTo(24, 664, 136);
  Map_CopyCellPictureIndicesRect(68, 33, 105, 6, 1, 1);
  Sound_Play(161);
  Func_020021b4();
  GameFlag_SetBit(0x247);
  Actor_MoveTo(24, 664, 104);
  Map_CopyCellPictureIndicesRect(69, 33, 105, 4, 1, 1);
  Sound_Play(229);
  Func_020021b4();
  Event_Wait(10);
  Actor_Jump(24, 6, 16);
  Actor_PlaceFacing(24, 222 << 18, 168 << 16, FACING_SOUTH);
  Event_Wait(20);
  GameFlag_SetBit(0x86c);
  Func_02003c30();
}

void Func_020022e0(void) {
  Func_02003c28();
  Event_ResolvePendingActions(0);
  Camera_MoveWithinMapBounds(245 << 18, -1, 168 << 16, 1);
  Func_02003cf0();
  Actor_SetAngleToward(24, gMainRoutineWork.currentOwner, 0);
  Func_080d47b4(24, 0x100, 40);
  Actor_GetById(24)->motionFlags = 0;
  Actor_SetSpritePriority(24, 1);
  Actor_SetSpeed(24, 0x19999, 0xcccc);
  Actor_MoveTo(24, 940, 168);
  Actor_MoveTo(24, 984, 152);
  Actor_MoveTo(24, 1040, 152);
  Actor_SetHorizontalPositionWithTerrain(24, 0, 0);
  Event_Wait(20);
  GameFlag_SetBit(0x86d);
  Func_02003c30();
}

void Func_02002390(void) {
  Func_02003c28();
  Event_ResolvePendingActions(0);
  Camera_MoveWithinMapBounds(144 << 16, -1, 214 << 15, 1);
  Func_02003cf0();
  Actor_SetAngleToward(25, gMainRoutineWork.currentOwner, 0);
  Func_080d47b4(25, 0x100, 40);
  Actor_SetSpeed(25, 0x19999, 0xcccc);
  Actor_SetPositionAndReset(25, 168, 104);
  Event_Wait(10);
  Sound_Play(153);
  Actor_GetById(25)->velocityY = 0x80000;
  Actor_SetSingleChildFlags(Actor_GetById(25), 0);
  Actor_SetPositionAndReset(25, 200, 104);
  Actor_SetSingleChildFlags(Actor_GetById(25), 1);
  Actor_MoveTo(25, 294, 104);
  Actor_PlaceFacing(25, 212 << 17, 208 << 15, FACING_SOUTH);
  GameFlag_SetBit(0x86e);
  Func_02003c30();
}

void Func_02002458(void) {
  Func_02003c28();
  Event_ResolvePendingActions(0);
  Actor_WatchAndSetScript(gMainRoutineWork.currentOwner, 25);
  Func_080d4568()->motionFlags = 0;
  Camera_SetBounds(212 << 17, -1, 208 << 15, 1);
  Func_02003cf0();
  Actor_SetAngleToward(25, gMainRoutineWork.currentOwner, 0);
  Func_080d47b4(25, 0x100, 20);
  Camera_MoveWithinMapBounds(212 << 17, -1, 152 << 16, 1);
  Actor_SetSpeed(25, 0x19999, 0xcccc);
  Actor_MoveTo(25, 424, 152);
  Actor_ArmCallback(25, FACING_EAST, 20);
  Func_02002604();
  Actor_MoveTo(25, 440, 152);
  Actor_MoveTo(25, 456, 168);
  Actor_MoveTo(25, 456, 184);
  GameFlag_SetBit(0x241);
  Map_CopyCellPictureIndicesRect(69, 51, 92, 11, 1, 1);
  Sound_Play(161);
  Func_020021b4();
  Actor_MoveTo(25, 456, 216);
  Actor_MoveTo(25, 584, 216);
  Actor_PlaceFacing(25, 154 << 18, 140 << 17, FACING_SOUTH);
  GameFlag_SetBit(0x86f);
  Func_02003c30();
}

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void Map_CopyAttributes(s32 a, s32 b, s32 c, s32 d, s32 e,
                                          s32 f) {
  Map_CopyCellAttributeRect(a, b, c, d, e, f);
}

struct EffectWork7c4 {
  u8 unk000[0x7c4];
  s16 active;
  s16 timer;
};

void Func_02002574(void) {
  struct EffectWork7c4 *work = Ram_HeapSlots->effectWork;
  struct Actor *object = Actor_GetById(18);

  if (work->timer != -1 && work->timer <= 7)
    object->scaleY += -0x1f00;
  if (work->active != 0) {
    Map_CopyAttributes(27, 8, 1, 1, 27, 9);
    Actor_SetModeById(18, 4);
    object->priorityFlags |= 2;
    ((u8 *)object)[89] &= 0xfe;
    object->scaleY = 0x10000;
    GameFlag_SetBit(0x205);
  }
}

void Func_02002604(void) {
  Func_02003d50(134, 1);
  Task_Add(Func_02002574, 0x480);
  Func_02003d58(25, 18);
  Func_02003d70();
  Field_DispatchTypeHandler(1);
  Func_02003d60();
  Task_RemoveFar(Func_02002574);
  Func_02003d68();
}
