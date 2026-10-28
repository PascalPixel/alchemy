// Draft, differ 195 of 529: the scene where person 8 tests the party: after
// their talk it throws person 10 three times, each throw flashing five colours
// of the object palette; then it raises the glow over the spot, the party is
// pushed back, the glow flares and person 10 drops to the floor with a
// shake.
#include "types.h"
#include "call.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "text/msg_ids.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

struct ThrowAnimations {
  s32 animations[3];
};

extern const struct ThrowAnimations gData_02003634;

struct Actor *Actor_GetById(s32 id);
void Func_0200345c(void);
void Func_020035b4(s32 mode);
void Func_0200350c(s32 message);
void Func_020005e8(s32 value);
void Func_02003544(s32 id, s32 animation, s32 frames);
void Func_0200351c(s32 speaker, s32 a, s32 b);
void Func_0200067c(s32 mode);
void Actor_SetPositionAndReset(s32 actor, s32 x, s32 z);
void Func_02003534(s32 id, s32 angle);
void FieldEvent_Wait(s32 frames);
void FieldMessage_ShowAtActor(s32 speaker, s32 flags);
s32 Event_AskYesNoWithActorAnimation(s32 actor, s32 flags);
void Actor_ArmCallback(s32 actor, s32 angle, s32 frames);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 follow);
void Func_02003564(void);
void Actor_SetModeById(u32 id, s32 mode);
void Func_0200361c(s32 id, s32 animation);
void Func_020034d4(s32 id, s32 x, s32 z);
void Actor_Jump(s32 actor, s32 speed, s32 frames);
void Actor_SetSpritePriority(u32 id, s32 variant);
void Actor_SetSpeedParameters(u32 objectId, s32 speedLimit,
                                     s32 acceleration);
void Actor_OffsetPositionAndResetMotion(u32 objectId, s32 xOffset,
                                               s32 zOffset);
void Func_0200362c(s32 cue);
void Func_02000734(const u16 *from, const u16 *to, s32 frames);
void Actor_CommitPositionAndActivate(u32 objectId, s32 xOffset,
                                            s32 zOffset);
void Actor_StartRepeatedMotion(u32 objectId, s32 variant);
void Func_0200354c(s32 id, s32 animation);
void Func_0200360c(s32 a, s32 b);
void Func_02003434(s32 x, s32 y, s32 z);
void Func_02003164(s32 *position);
void Func_0200320c(s32 *position);
void Field_StartPaletteTransition(s32 mode);
void Func_020032a4(void);
void wait(s32 frames);
void Func_020035bc(void);
void Motion_SetVarCbAndRefresh(u32 objectId, s32 variant);
s32 Func_02003464(void);
void Func_02000038(void);

s32 Func_02000a34(s32 value) {
  struct Actor *thrown = Actor_GetById(10);
  s32 glow[3];
  u16 dim[5];
  u16 bright[5];
  struct ThrowAnimations animations;
  s32 landing[3];
  s32 i;
  struct Actor *object;

  glow[0] = 0x2960000;
  glow[1] = 0;
  glow[2] = 0xee0000;
  bright[0] = ((volatile u16 *)0x050001e4)[0];
  bright[1] = ((volatile u16 *)0x050001e4)[1];
  bright[2] = ((volatile u16 *)0x050001e4)[2];
  bright[3] = ((volatile u16 *)0x050001e4)[3];
  bright[4] = ((volatile u16 *)0x050001e4)[4];
  dim[0] = ((volatile u16 *)0x050001c4)[0];
  dim[1] = ((volatile u16 *)0x050001c4)[1];
  dim[2] = ((volatile u16 *)0x050001c4)[2];
  dim[3] = ((volatile u16 *)0x050001c4)[3];
  dim[4] = ((volatile u16 *)0x050001c4)[4];
  Func_0200345c();
  Call1((void (*)())Func_020035b4, 0);
  Func_0200350c((s32)MsgWellDoneYou);
  Func_020005e8(value);
  Call3((void (*)())Func_02003544, 8, 0x100, 30);
  Call3((void (*)())Func_0200351c, 8, 0, 5);
  Call3((void (*)())Func_02003544, 8, 0x108, 30);
  Call3((void (*)())Func_0200351c, 8, 0, 5);
  Call1((void (*)())Func_0200067c, 1);
  Call3((void (*)())Actor_SetPositionAndReset, 8, 0x298, 200);
  Call2((void (*)())Func_02003534, 8, 0x3000);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call2((void (*)())Func_02003534, 8, 0x8000);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call2((void (*)())FieldMessage_ShowAtActor, 8, 0);
  if (Event_AskYesNoWithActorAnimation(4, 0) == 0) {
    Call1((void (*)())FieldEvent_Wait, 20);
    Call3((void (*)())Func_0200351c, 8, 0, 5);
    FIELD_AT(Ram_HeapSlots->eventWork, u16, 452)++;
  } else {
    Call1((void (*)())FieldEvent_Wait, 40);
    FIELD_AT(Ram_HeapSlots->eventWork, u16, 452)++;
    Call3((void (*)())Func_0200351c, 8, 0, 5);
  }
  Call3((void (*)())Actor_ArmCallback, gMainRoutineWork.currentOwner, 0x2000,
        0);
  Call2((void (*)())Func_02003534, 8, 0x5000);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call4((void (*)())Camera_MoveWithinMapBounds, 0x2980000, -1, 0xe80000, 1);
  Func_02003564();
  Call1((void (*)())FieldEvent_Wait, 10);
  Call3((void (*)())Func_02003544, 8, 0x107, 40);
  Call3((void (*)())Func_0200351c, 0x2008, 0, 5);
  for (i = 0; i <= 2; i++) {
    animations = gData_02003634;
    Call2((void (*)())Actor_SetModeById, 8, 9);
    Call1((void (*)())FieldEvent_Wait, 3);
    Call2((void (*)())Func_0200361c, 10, animations.animations[i]);
    Call3((void (*)())Func_020034d4, 10, 0x29c0000, 0xc80000);
    thrown->gravity = 0x4000;
    Call3((void (*)())Actor_Jump, 10, 4, 2);
    Call2((void (*)())Actor_SetSpritePriority, 10, 1);
    Call3((void (*)())Actor_SetSpeedParameters, 10, 0x10000, 0x8000);
    Call3((void (*)())Actor_OffsetPositionAndResetMotion, 10, 0, 40);
    Call1((void (*)())Func_0200362c, 0x136);
    Call1((void (*)())FieldEvent_Wait, 34);
    landing[0] = thrown->x;
    landing[1] = thrown->y;
    landing[2] = thrown->z;
    Call3((void (*)())Func_020034d4, 10, 0, 0);
    Call1((void (*)())FieldEvent_Wait, 20);
    Call1((void (*)())Func_0200362c, 134);
    Call3((void (*)())Func_02000734, bright, dim, 12);
    Call3((void (*)())Func_02000734, dim, bright, 48);
    Call1((void (*)())FieldEvent_Wait, 10);
  }
  object = Actor_GetById(gMainRoutineWork.currentOwner);
  object->actionFlags &= ~1;
  Call3((void (*)())Actor_CommitPositionAndActivate,
        gMainRoutineWork.currentOwner, 12, 12);
  Call1((void (*)())FieldEvent_Wait, 1);
  Actor_GetById(gMainRoutineWork.currentOwner)->actionFlags |= 1;
  Call2((void (*)())Func_02003534, gMainRoutineWork.currentOwner, 0x2000);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call2((void (*)())Actor_SetModeById, gMainRoutineWork.currentOwner, 22);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call2((void (*)())Actor_SetModeById, 8, 7);
  Call1((void (*)())FieldEvent_Wait, 60);
  Call1((void (*)())Func_0200362c, 78);
  Call3((void (*)())Func_02003544, 8, 0x101, 60);
  Call3((void (*)())Func_0200351c, 0x2008, 0, 5);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call1((void (*)())Func_0200362c, 220);
  Call3((void (*)())Func_02000734, bright, dim, 48);
  Call2((void (*)())Actor_StartRepeatedMotion, gMainRoutineWork.currentOwner,
        3);
  Call2((void (*)())Func_0200354c, gMainRoutineWork.currentOwner, 0x102);
  Call1((void (*)())FieldEvent_Wait, 50);
  Call3((void (*)())Func_02003544, 8, 0x101, 60);
  Call2((void (*)())Func_0200360c, 16, 0x1090);
  Call3((void (*)())Func_02003434, 0x20000, 0x20000, 0x10000);
  Func_02003164(glow);
  Call2((void (*)())Actor_SetModeById, 8, 1);
  Call3((void (*)())Actor_Jump, 4, 6, 0);
  Call1((void (*)())Func_0200362c, 194);
  Actor_GetById(4)->actionFlags &= ~1;
  Call3((void (*)())Actor_CommitPositionAndActivate, 4, -12, -12);
  Call1((void (*)())FieldEvent_Wait, 1);
  Actor_GetById(4)->actionFlags |= 1;
  Call1((void (*)())FieldEvent_Wait, 10);
  Call2((void (*)())Actor_StartRepeatedMotion, 8, 3);
  Call2((void (*)())Func_0200354c, 8, 0x102);
  Call1((void (*)())FieldEvent_Wait, 30);
  Call3((void (*)())Func_0200351c, 0x2008, 0, 5);
  Call3((void (*)())Func_02003434, 0x40000, 0x40000, 0x10000);
  Func_0200320c(glow);
  Call1((void (*)())Func_0200362c, 194);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call3((void (*)())Actor_Jump, 8, 6, 15);
  Call3((void (*)())Actor_Jump, 8, 6, 23);
  Call1((void (*)())Func_0200362c, 194);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call3((void (*)())Func_0200351c, 0x2008, 0, 5);
  Call2((void (*)())Func_0200361c, 10, 0x146);
  Call3((void (*)())Func_020034d4, 10, 0x2980000, 0xe80000);
  Call2((void (*)())Actor_SetSpritePriority, 10, 1);
  thrown->motionFlags = 2;
  thrown->y = 0x180000;
  thrown->update = Func_02000038;
  thrown->terrainHeight = 0;
  thrown->scaleX = 0;
  thrown->scaleY = 0;
  Call3((void (*)())Actor_Jump, 10, 5, 0);
  Call1((void (*)())Field_StartPaletteTransition, 8);
  Func_020032a4();
  Call3((void (*)())Func_02003434, -1, -1, 0xe666);
  while (thrown->y > 0)
    Call1((void (*)())wait, 1);
  thrown->gravity = 0x10000;
  thrown->y = 0;
  thrown->motionFlags = 4;
  Func_020035bc();
  Call1((void (*)())FieldEvent_Wait, 20);
  Call2((void (*)())Motion_SetVarCbAndRefresh, 8, 2);
  Call1((void (*)())FieldEvent_Wait, 5);
  Call3((void (*)())Func_0200351c, 0x2008, 0, 5);
  Call2((void (*)())Func_02003534, 8, 0x8000);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call3((void (*)())Func_0200351c, 0x2008, 0, 5);
  Call2((void (*)())Func_02003534, 8, 0x5000);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call2((void (*)())Actor_SetSpritePriority, 10, 2);
  return Func_02003464();
}
