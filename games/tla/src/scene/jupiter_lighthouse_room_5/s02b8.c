#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "scene_tables.h"
#include "event_inline.h"

extern s32 gData_02003274;
extern s32 gData_02003278;
extern const u8 gData_02002f70[];
extern const u8 gData_02002f94[];

void Actor_SetAnimationSpeed(struct Actor *object, s32 speed);
void Actor_SetSingleChildFlags(struct Actor *object, s32 flags);
void Func_02001718(const s32 *from, const s32 *to);
void Func_02000d80(s32 value);
void Func_0200185c(void);
void Func_02000e04(s32 id);
void Sound_WaitForJingle(void);
void Func_02000120(void);
void Func_020000c8(void);
void Func_02000294(void);
void Func_020001a4(void);

static __inline__ void Bring(s32 id) {
  struct Actor *object;

  Actor_SetSpritePriority(id, 2);
  Actor_SetPartAttribute(Actor_GetById(id), 7);
  object = Actor_GetById(id);
  object->motionFlags = 0;
  object->priorityFlags |= 2;
  object->scaleY = 0xffff0000;
  object->scaleX = 0x1999;
  Actor_SetMovementTypeById(id, gData_02002f70);
}

/// The event when the leader stands on cell (42, 41): sets flag 2560, turns the
/// leader (Actor_ArmCallbackAndRefresh), plays its hooks and sounds 220 and
/// 178, starts the spark ring from above the leader to (680, 0, 574)
/// (Func_02001718) and a pulse callback (Func_02000294) that quickens over 20
/// steps, sounds 390 and 144 with the screen shaking, objects 23 and 24 brought
/// in on their action scripts, flashes, the room's cells copied open
/// (Func_0802b450) and Func_02000d80(2); then the callbacks stop, objects 13 to
/// 16 rise 2 pixels a frame for 16 frames, take their next state
/// (Func_02000e04) and sound 80 plays.
void Func_020002b8(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct Actor *a = Actor_GetById(13);
  struct Actor *b = Actor_GetById(14);
  struct Actor *c = Actor_GetById(15);
  struct Actor *d = Actor_GetById(16);
  s32 from[3];
  s32 to[3];
  s32 i;

  if (leader->x >> 20 != 42 || leader->z >> 20 != 41)
    return;
  Event_SetFlag(2560);
  FieldEvent_ResolvePendingActions(0);
  Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, FACING_NORTH);
  FieldEvent_Wait(30);
  leader->update = Func_02000120;
  FieldEvent_Wait(30);
  Event_PlaySound(220);
  Actor_SetAnimationSpeed(leader, 64);
  FieldEvent_Wait(50);
  Event_PlaySound(178);
  leader->action = 0;
  leader->update = Func_020000c8;
  FieldEvent_Wait(30);
  from[0] = leader->x;
  from[1] = leader->y + 0x140000;
  from[2] = leader->z;
  to[0] = 170 << 18;
  to[1] = 0;
  to[2] = 0x23e0000;
  Func_02001718(from, to);
  gData_02003274 = 0;
  gData_02003278 = 1;
  Task_AddFrame(Func_02000294, 0x480);
  for (i = 0; i < 20; i++) {
    gData_02003274 = 0;
    System_Wait((20 - i) / 2);
    gData_02003274 = 1;
    System_Wait(1);
  }
  Event_PlaySound(390);
  MapGrid_SetShake(0x10000, 0x10000, 0x10000);
  gData_02003274 = 1;
  FieldEvent_Wait(90);
  MapGrid_SetShake(0x30000, 0x30000, 0x10000);
  gData_02003274 = 3;
  gData_02003278 = 0;
  FieldEvent_Wait(40);
  Bring(23);
  Bring(24);
  Task_AddFrame(Func_020001a4, 0x480);
  Event_PlaySound(144);
  MapGrid_SetShake(0x60000, 0x60000, 0x10000);
  PaletteFade_ApplySource(0x10000, 0);
  PaletteFade_ApplyTarget(0x4063ff, 0);
  PaletteFade_Interpolate(60);
  System_Wait(60);
  Event_PlaySound(144);
  System_Wait(30);
  Event_PlaySound(144);
  System_Wait(30);
  Event_PlaySound(144);
  System_Wait(30);
  MapGrid_CopyCellsLayered(58, 99, 3, 3, 41, 99);
  MapGrid_CopyCellsLayered(123, 32, 1, 5, 106, 32);
  Func_02000d80(2);
  Event_PlaySound(144);
  System_Wait(30);
  leader->action = 3;
  System_Wait(1);
  leader->update = 0;
  Actor_SetAnimationSpeed(leader, 16);
  Actor_SetSingleChildFlags(Actor_GetById(gMainRoutineWork.currentOwner), 1);
  Actor_SetScript(23, gData_02002f94);
  Actor_SetScript(24, gData_02002f94);
  gData_02003274 = 2;
  FieldEvent_Wait(30);
  gData_02003274 = 1;
  FieldEvent_Wait(30);
  gData_02003274 = 0;
  FieldActor_RefreshSelectorById(23);
  FieldActor_RefreshSelectorById(24);
  Actor_GetById(23)->scaleX = 0;
  Actor_GetById(24)->scaleX = 0;
  PaletteFade_ApplyTarget(0x10000, 0);
  PaletteFade_Interpolate(30);
  MapGrid_SetShake(0x20000, 0x20000, 0x10000);
  FieldEvent_Wait(30);
  MapGrid_SetShake(0, 0, 0x10000);
  FieldTask_Remove(Func_02000294);
  Func_0200185c();
  FieldEvent_Wait(30);
  a->motionFlags = 0;
  b->motionFlags = 0;
  c->motionFlags = 0;
  d->motionFlags = 0;
  for (i = 0; i < 16; i++) {
    a->y += PIXELS(2);
    b->y += PIXELS(2);
    c->y += PIXELS(2);
    d->y += PIXELS(2);
    System_Wait(1);
  }
  Func_02000e04(13);
  Func_02000e04(14);
  Func_02000e04(15);
  Func_02000e04(16);
  Event_PlaySound(80);
  Sound_WaitForJingle();
}
