// Near miss, differ 4 of 641 (tla-en, 2026-10-30): the native copies the
// motion flags' address into r8 (actor 13) and r10 (actor 14) after storing 0
// through it (strb, then mov r8, r3); the draft copies first. In sched2 the
// copy and the strb are both leaves of priority 1 after ldr r6 (the sprite);
// the strb is anti-dependent on that load (the animation read and the flags
// store share alias sets), which puts it in haifa's second class and the
// copy, independent, ahead. Read through another type the sprite load loses
// that dependence but then issues earlier (8); swapping the two statements,
// reading the sprite as u32, storing the flags as u8 or s8 change nothing (4);
// a flags pointer kept for the later = 3 stores is far worse (45 to 53).
// Earlier (26 October): alchemy permute (5 to 6 minutes) and an empty asm or
// one-pass block after any one statement do not move it.
// Draft, differ 12 of 641: actors 13, 14 and 15 rise in turn after
// MsgKandoreanTemple04. Calls go through call.h except those whose
// constants the native shares. Left: each rise loop's y load is issued
// before its constant in the native, and one pool load.
#include "../../../../games/tla/src/scene/kandorean_temple/local.h"
#include "call.h"

extern u8 MsgKandoreanTemple04;

void Func_020052e8(void);
void Func_02005270(s32 id, s32 value);
void Func_02005290(s32 id, s32 a, s32 b);
void Func_020053d8(s32 cue);
void Func_02003e34(void *sprite, s32 x, s32 y);
void Func_02003e60(struct Actor *object, s32 a, s32 b);
void Func_02001f3c(void);
void Func_02001c8c(void);
void Actor_Jump(s32 id, s32 height, s32 frames);
void Motion_SetVarCbAndRefresh(s32 id, s32 value);
s32 Task_Add(void (*func)(void), s32 priority);
void Task_RemoveFar(void (*func)(void));

void Func_02003e8c(void) {
  struct Actor *object;
  struct FieldSprite *sprite;
  u32 i;

  Func_020051c0();
  Call1((void (*)())Func_02005370, 0);
  Call4((void (*)())Camera_MoveWithinMapBounds, -1, -1, -1, 0);
  Func_020052f0()->motionFlags = 0;
  Call1((void (*)())wait, 1);
  Call3((void (*)())Func_02005230, gMainRoutineWork.currentOwner, 0x1900000,
        0x31c0000);
  Call1((void (*)())wait, 1);
  Event_StartScreenTransition();
  Call3((void (*)())Actor_SetPositionAndReset, gMainRoutineWork.currentOwner,
        400, 752);
  Call2((void (*)())Func_020052d8, 0x4cccc, 0x9999);
  Call4((void (*)())Camera_MoveWithinMapBounds, 0x1b40000, 0x480000, 0x2980000,
        1);
  Func_020052e8();
  Call3((void (*)())Actor_Jump, 12, 4, 0);
  Func_02005280((s32)&MsgKandoreanTemple04);
  Call2((void (*)())Func_02005298, 12, 0);
  Call4((void (*)())Camera_MoveWithinMapBounds, 0x1c80000, 0, 0x2b00000, 1);
  Func_020052e8();
  Call2((void (*)())Motion_SetVarCbAndRefresh, 13, 2);
  Call1((void (*)())Func_020053d8, 131);
  Call2((void (*)())Func_02005270, 13, 0x100);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call2((void (*)())Func_02005270, 13, 0);
  Call1((void (*)())FieldEvent_Wait, 20);
  object = Actor_GetById(13);
  sprite = object->animation;
  object->motionFlags = 0;
  i = 0;
rise13:
  object->y += 0xccc;
  FieldEvent_Wait(1);
  i++;
  if (i <= 63)
    goto rise13;
  Func_02003e34(sprite, 32, 32);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call2((void (*)())Func_020052d0, 13, 0x102);
  Func_02003e34(sprite, 32, -64);
  Func_02003e34(sprite, 32, 64);
  Func_02003e34(sprite, 64, 32);
  Call2((void (*)())Func_020052d0, 13, 0x102);
  Call1((void (*)())FieldEvent_Wait, 60);
  object->motionFlags = 3;
  Call1((void (*)())FieldEvent_Wait, 5);
  Call1((void (*)())Func_020053d8, 159);
  object->velocityY = 0x30000;
  sprite->rotation = 0;
  Call1((void (*)())FieldEvent_Wait, 60);
  Call2((void (*)())Func_02005298, 13, 0);
  Call4((void (*)())Camera_MoveWithinMapBounds, 0x1b40000, 0x480000, 0x2980000,
        1);
  Func_020052e8();
  Call3((void (*)())Actor_SetSpeedParameters, 12, 0xcccc, 0x6666);
  Call3((void (*)())Actor_SetPositionAndReset, 12, 476, 664);
  Call3((void (*)())Actor_ArmCallback, 12, 0x3000, 0);
  Call2((void (*)())Actor_SetModeById, 12, 4);
  Call2((void (*)())Func_02005298, 0x100c, 0);
  Call3((void (*)())Actor_SetPositionAndReset, 12, 474, 664);
  Call3((void (*)())Actor_SetPositionAndReset, 12, 472, 648);
  Call3((void (*)())Func_02005230, 12, 0, 0);
  Call2((void (*)())Func_020052d8, 0x13333, 0x2666);
  Call4((void (*)())Camera_MoveWithinMapBounds, 0x16c0000, 0x480000, 0x2980000,
        1);
  Func_020052e8();
  Call3((void (*)())Func_02005230, 12, 0x1480000, 0x2880000);
  Call1((void (*)())wait, 1);
  Call3((void (*)())Actor_SetPositionAndReset, 12, 326, 664);
  Call3((void (*)())Actor_SetPositionAndReset, 12, 320, 664);
  Call3((void (*)())Actor_ArmCallback, 12, 0x5000, 0);
  Call2((void (*)())Func_02005298, 0x200c, 0);
  Call2((void (*)())Func_020052d8, 0x4cccc, 0x9999);
  Call4((void (*)())Camera_MoveWithinMapBounds, 0x1560000, 0, 0x2b00000, 1);
  Func_020052e8();
  Call2((void (*)())Motion_SetVarCbAndRefresh, 14, 2);
  Call1((void (*)())Func_020053d8, 131);
  Call2((void (*)())Func_02005270, 14, 0x100);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call2((void (*)())Func_02005270, 14, 0);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call2((void (*)())Func_020052d0, 14, 0x101);
  object = Actor_GetById(14);
  object->motionFlags = 0;
  i = 0;
rise14:
  object->y += 0x2666;
  FieldEvent_Wait(1);
  i++;
  if (i <= 63)
    goto rise14;
  Func_02003e60(object, 64, 0x1000);
  Func_02003e60(object, 128, -0x1000);
  Func_02003e60(object, 128, 0x1000);
  Func_02003e60(object, 64, -0x1000);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call2((void (*)())Func_020052d0, 14, 0x100);
  object->motionFlags = 3;
  Call1((void (*)())FieldEvent_Wait, 5);
  Call1((void (*)())Func_020053d8, 159);
  object->velocityY = 0x30000;
  Call1((void (*)())FieldEvent_Wait, 60);
  Call2((void (*)())Func_02005298, 14, 0);
  Call4((void (*)())Camera_MoveWithinMapBounds, 0x16c0000, 0x480000, 0x2980000,
        1);
  Func_020052e8();
  Call2((void (*)())Func_02005298, 0x200c, 0);
  Call3((void (*)())Actor_ArmCallback, 12, 0x3000, 10);
  Call2((void (*)())Func_02005298, 12, 0);
  Call4((void (*)())Camera_MoveWithinMapBounds, 0x1900000, 0, 0x2b00000, 1);
  Func_020052e8();
  Call2((void (*)())Motion_SetVarCbAndRefresh, 15, 2);
  Call1((void (*)())Func_020053d8, 131);
  Call2((void (*)())Func_02005270, 15, 0x100);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call2((void (*)())Func_02005270, 15, 0);
  Call1((void (*)())FieldEvent_Wait, 20);
  object = Actor_GetById(15);
  sprite = object->animation;
  object->motionFlags = 0;
  i = 0;
rise15:
  object->y += 0x3333;
  FieldEvent_Wait(1);
  i++;
  if (i <= 63)
    goto rise15;
  Func_02003e34(sprite, 32, 64);
  sprite->rotation = 0;
  Call2((void (*)())Motion_SetVarCbAndRefresh, 15, 1);
  Call1((void (*)())FieldEvent_Wait, 60);
  Func_02003e34(sprite, 32, -64);
  sprite->rotation = 0;
  Call2((void (*)())Motion_SetVarCbAndRefresh, 15, 1);
  Call1((void (*)())FieldEvent_Wait, 60);
  i = 0;
fall15:
  object->y += -0x3333;
  FieldEvent_Wait(1);
  i++;
  if (i <= 63)
    goto fall15;
  Call1((void (*)())FieldEvent_Wait, 20);
  Value3((s32 (*)())Func_020052c8, 15, 0x108, 0);
  Call3((void (*)())Func_02005290, 15, 0, 20);
  Call2((void (*)())Func_02005298, 0x100c, 0);
  Value3((s32 (*)())Func_020052c8, 15, 0x103, 20);
  Call2((void (*)())Actor_SetModeById, 15, 1);
  Call3((void (*)())Actor_Jump, 15, 4, 0);
  Call3((void (*)())Actor_ArmCallback, 15, 0xb000, 20);
  Call2((void (*)())Func_02005298, 0x400f, 0);
  Call4((void (*)())Camera_MoveWithinMapBounds, 0x16c0000, 0x480000, 0x2980000,
        1);
  Func_020052e8();
  Value3((s32 (*)())Func_020052c8, 12, 0x102, 20);
  Call2((void (*)())Func_02005298, 0x800c, 0);
  Call2((void (*)())Func_02005298, 15, 0);
  Call3((void (*)())Actor_ArmCallback, 12, 0xb000, 40);
  Call2((void (*)())Actor_SetModeById, 12, 4);
  Call2((void (*)())Func_02005298, 0x800c, 0);
  Call2((void (*)())Func_02005298, 15, 0);
  Value3((s32 (*)())Func_020052c8, 12, 0x105, 80);
  Call2((void (*)())Func_02005298, 0x800c, 0);
  Call3((void (*)())Actor_ArmCallback, 12, 0x3000, 20);
  Call2((void (*)())Actor_SetModeById, 12, 3);
  Call2((void (*)())Func_02005298, 0x800c, 0);
  Call3((void (*)())Actor_SetPositionAndReset, 12, 326, 664);
  Call3((void (*)())Actor_SetPositionAndReset, 12, 328, 648);
  Actor_SetSingleChildFlags(Actor_GetById(12), 0);
  Actor_SetPartAttribute(Actor_GetById(12), 15);
  Call3((void (*)())Func_02005230, 12, 0x1b80000, 0x2580000);
  Call4((void (*)())Camera_MoveWithinMapBounds, 0x1900000, 0, 0x2b00000, 1);
  Call3((void (*)())Actor_SetSpeedParameters, 15, 0xcccc, 0x6666);
  Call3((void (*)())Actor_SetPositionAndReset, 15, 400, 644);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call1((void (*)())Func_020053d8, 188);
  Func_02001f3c();
  Call2((void (*)())Func_02005298, 12, 0);
  Task_RemoveFar(Func_02001c8c);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 15, 3);
  Call2((void (*)())Actor_SetSpritePriority, 15, 3);
  Call3((void (*)())Actor_SetPositionAndReset, 15, 400, 640);
  Call3((void (*)())Func_02005230, 15, 0, 0);
  Call3((void (*)())Func_02005230, 12, 0, 0);
  Call4((void (*)())Camera_MoveWithinMapBounds, 0x1900000, 0, 0x2f00000, 1);
  Func_020052e8();
  Task_Add(Func_02001c8c, 0x480);
  Func_020051c8();
  Call1((void (*)())GameFlag_SetBit, 0x848);
}
