#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "call.h"

extern s32 gData_02003274;
extern s32 gData_02003278;
extern const u8 gData_02002f70[];
extern const u8 gData_02002f94[];

struct Actor *Actor_GetById(s32 id);
void GameFlag_SetBit(s32 flag);
void wait(s32 frames);
void FieldEvent_Wait(s32 frames);
void Func_020029a8(s32 value);
void Func_02002940(s32 id, s32 angle);
void Func_02002a40(s32 sound);
void Actor_SetAnimationSpeed(struct Actor *object, s32 speed);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Func_02001718(const s32 *from, const s32 *to);
void Task_Add(void (*func)(void), s32 priority);
void Task_RemoveFar(void (*func)(void));
void Func_02002870(s32 a, s32 b, s32 c);
void Actor_SetSpritePriority(u32 id, s32 variant);
void Actor_SetPartAttribute(struct Actor *object, s32 value);
void Actor_SetMovementTypeById(s32 id, const void *script);
void Func_02002980(s32 a, s32 b);
void Func_02002978(s32 a, s32 b);
void Func_02002988(s32 frames);
void Func_02002860(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02000d80(s32 value);
void Actor_RefreshSelectorById(u32 id);
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
/// leader (Func_02002940), plays its hooks and sounds 220 and 178, starts the
/// spark ring from above the leader to (680, 0, 574) (Func_02001718) and a
/// pulse callback (Func_02000294) that quickens over 20 steps, sounds 390 and
/// 144 with the screen shaking, objects 23 and 24 brought in on their action
/// scripts, flashes, the room's cells copied open (Func_02002860) and
/// Func_02000d80(2); then the callbacks stop, objects 13 to 16 rise 2 pixels a
/// frame for 16 frames, take their next state (Func_02000e04) and sound 80
/// plays.
void Func_020002b8(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  u32 pulseState;
  struct Actor *a = Actor_GetById(13);
  struct Actor *b = Actor_GetById(14);
  struct Actor *c = Actor_GetById(15);
  struct Actor *d = Actor_GetById(16);
  s32 from[3];
  s32 to[3];
  s32 i;

  if (leader->x >> 20 != 42 || leader->z >> 20 != 41)
    return;
  GameFlag_SetBit(2560);
  Func_020029a8(0);
  Func_02002940(gMainRoutineWork.currentOwner, 0xc000);
  /* FAKEMATCH(temporary): the pulse state 3 held from the turn on, so its
   * register spans the quickening loop and the pulse's address and zero take
   * r9 and r11; as a plain 3, 30 of 395 instructions differ in tla-en. */
  pulseState = 3;
  FieldEvent_Wait(30);
  leader->update = Func_02000120;
  FieldEvent_Wait(30);
  Func_02002a40(220);
  Actor_SetAnimationSpeed(leader, 64);
  FieldEvent_Wait(50);
  Func_02002a40(178);
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
  Task_Add(Func_02000294, 0x480);
  i = 0;
quicken:
  gData_02003274 = 0;
  wait((20 - i) / 2);
  gData_02003274 = 1;
  wait(1);
  if (++i <= 19)
    goto quicken;
  Func_02002a40(390);
  Call3((void (*)())Func_02002870, 0x10000, 0x10000, 0x10000);
  gData_02003274 = 1;
  FieldEvent_Wait(90);
  Call3((void (*)())Func_02002870, 0x30000, 0x30000, 0x10000);
  gData_02003274 = pulseState;
  gData_02003278 = 0;
  FieldEvent_Wait(40);
  Bring(23);
  Bring(24);
  Task_Add(Func_020001a4, 0x480);
  Func_02002a40(144);
  Call3((void (*)())Func_02002870, 0x60000, 0x60000, 0x10000);
  Call2((void (*)())Func_02002980, 0x10000, 0);
  Call2((void (*)())Func_02002978, 0x4063ff, 0);
  Func_02002988(60);
  wait(60);
  Func_02002a40(144);
  wait(30);
  Func_02002a40(144);
  wait(30);
  Func_02002a40(144);
  wait(30);
  Call6((void (*)())Func_02002860, 58, 99, 3, 3, 41, 99);
  Call6((void (*)())Func_02002860, 123, 32, 1, 5, 106, 32);
  Func_02000d80(2);
  Func_02002a40(144);
  wait(30);
  leader->action = 3;
  wait(1);
  leader->update = 0;
  Actor_SetAnimationSpeed(leader, 16);
  Actor_SetSingleChildFlags(Actor_GetById(gMainRoutineWork.currentOwner),
                                     1);
  Actor_SetMovementTypeById(23, gData_02002f94);
  Actor_SetMovementTypeById(24, gData_02002f94);
  gData_02003274 = 2;
  FieldEvent_Wait(30);
  gData_02003274 = 1;
  FieldEvent_Wait(30);
  gData_02003274 = 0;
  Actor_RefreshSelectorById(23);
  Actor_RefreshSelectorById(24);
  Actor_GetById(23)->scaleX = 0;
  Actor_GetById(24)->scaleX = 0;
  Call2((void (*)())Func_02002978, 0x10000, 0);
  Func_02002988(30);
  Call3((void (*)())Func_02002870, 0x20000, 0x20000, 0x10000);
  FieldEvent_Wait(30);
  Call3((void (*)())Func_02002870, 0, 0, 0x10000);
  Task_RemoveFar(Func_02000294);
  Func_0200185c();
  FieldEvent_Wait(30);
  a->motionFlags = 0;
  b->motionFlags = 0;
  c->motionFlags = 0;
  d->motionFlags = 0;
  i = 15;
rise:
  a->y += 0x20000;
  b->y += 0x20000;
  c->y += 0x20000;
  d->y += 0x20000;
  wait(1);
  if (--i >= 0)
    goto rise;
  Func_02000e04(13);
  Func_02000e04(14);
  Func_02000e04(15);
  Func_02000e04(16);
  Func_02002a40(80);
  Sound_WaitForJingle();
}
