#include "types.h"
#include "actor.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "io_reg.h"
#include "io_write_queue.h"
#include "iwram_call.h"
#include "heap_release.h"
#include "bg_scroll.h"
#include "input.h"
#include "call.h"
#include "map.h"
#define ACTOR(id) ((struct Actor *)Value1((s32 (*)())Actor_GetById, id))
#define FIELD(p, off) (*(s32 *)((u8 *)(p) + (off)))

struct EventWorkView {
  u8 unk000[0x1ac];
  s32 unk1ac;
};

void Camera_MoveWithinMapBounds(s32 x, s32 a, s32 z, s32 b);
void Event_FinishScreenTransition(void);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Event_Wait(s32 frames);
void Field_StartPaletteTransition(s32 a);
void Func_020002a4(u16 *colors, u16 *target, s32 count);
void Func_0200032c(u16 *colors, s32 count, s32 delta);
void Func_02000394(void);
void Func_02000478(s32 radius, u32 count);
void Func_02001454(void);
void Map_SetScreenShake(s32 a, s32 b, s32 c);
void Func_0803f9c0(s32 a, s32 b);
void Actor_SetHorizontalPositionWithTerrain(s32 a, s32 b, s32 c);
void Actor_ConfigureSpriteColor(s32 a, s32 b);
void Event_SetMessageId(const void *message);
void Event_ShowMessageThenWait(s32 a, s32 b, s32 c);
void Event_ShowMessageAndWait(s32 a, s32 b);
s32 Func_080d47b4(s32 a, s32 b, s32 c);
void Func_080d489c(s32 a, s32 b);
void Camera_SetSpeedParameters(s32 a, s32 b);
void Func_0200152c(void);
void Func_02001534(s32 scene, s32 entrance);
void BattleEffect_ApplyColorToTargetBuffer(s32 a, s32 b);
void BattleEffect_StartBufferInterpolation(s32 a);
void Func_02001564(s32 a, s32 b);
void Func_02001574(void);
void Func_0200157c(void);
void Func_02001584(void);
void Func_0200158c(struct Actor *object, s32 a);
void Sound_Play(s32 a);
void Motion_SetVarCbAndRefresh(s32 id, s32 variant);
void Actor_SetSingleChildFlags(struct Actor *object, s32 flags);
void Actor_ArmCallback(s32 id, s32 angle, s32 flags);
void Actor_ResetAndSetPositionInMode2(s32 id, s32 x, s32 z);
void Actor_SetSpritePriority(u32 id, s32 variant);
void Actor_SetPositionAndReset(s32 actor, s32 x, s32 z);
void Actor_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
struct Actor *Actor_GetById(s32 id);
void Actor_SetActionById(s32 id, s32 action);
void Actor_SetModeById(u32 id, s32 mode);
void Task_Add(void *func, s32 priority);
void Task_RemoveFar(void *func);
void wait(s32 frames);
extern u8 MsgGoldenSunEternalLifeAndPower[];
extern u8 MsgGoldenSunFormation[];
extern char SceneId_Ov64c;
extern char SceneId_Ov6b3;

static __inline__ void CopyToPalette(void *from, void *to, s32 size) {
  /* FAKEMATCH(wrapper): the copy forwarded inline; as Call3, 4 of 1526
   * instructions differ in tla-en. */
  Iwram_CopyWords(from, to, size);
}

s32 Func_02000564(void) {
  u8 *block;
  void *buffer;
  s32 mode;
  s32 i, j, scale, r, g, b, value;
  s32 c;
  u16 *color, *out;
  u8 *message;
  struct Actor *object;
  volatile u32 *held;
  struct HeapSlots *slots;

  buffer = Heap_AllocEwram(0x200);
  Func_02001454();
  Call2((void (*)())Func_0803f9c0, gMainRoutineWork.paletteGlow[0],
        gMainRoutineWork.paletteGlow[1]);
  Call3((void (*)())Actor_SetHorizontalPositionWithTerrain, 4, 0, 0);
  mode = gMainRoutineWork.entrance;
  if (mode == 10) {
    Func_02001534((s32)&SceneId_Ov64c, 72);
  } else if (mode == 2) {
    Call4((void (*)())Camera_MoveWithinMapBounds, 0x1000000, -1, 0x800000, 0);
    Call2((void (*)())Camera_SetSpeedParameters, 0x10000, 0x2000);
    Call2((void (*)())Actor_SetSingleChildFlags,
          (s32)Value1((s32 (*)())Actor_GetById, 8), 0);
    Call2((void (*)())Actor_SetModeById, 8, 12);
    Call2((void (*)())Actor_SetActionById, 8, 8);
    ((struct EventWorkView *)Ram_HeapSlots->eventWork)->unk1ac = 0;
    REG_DISPCNT &= 0xfdff;
    REG_BG1CNT &= 0xfffc;
    REG_BG1CNT |= 3;
    REG_BG3CNT &= 0xfffc;
    REG_BG3CNT |= 2;
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Call4((void (*)())Camera_MoveWithinMapBounds, 0x1000000, -1, 0x13e0000, 1);
    Func_0200152c();
    Call4((void (*)())Camera_MoveWithinMapBounds, 0x13c0000, -1, 0x13e0000, 1);
    Func_0200152c();
    i = 0;
  L1:
    if (i == 3) {
      Call2((void (*)())Camera_SetSpeedParameters, 0x1999, 0x333);
      Call4((void (*)())Camera_MoveWithinMapBounds, 0x13c0000, -1, 0x12c0000,
            1);
    }
    ACTOR(8)->z -= 0x10000;
    Call1((void (*)())wait, 30);
    i++;
    if (i != 10)
      goto L1;
    Call2((void (*)())Actor_SetActionById, 8, 0);
    Call3((void (*)())Iwram_CopyWords, (s32)buffer, 0x5000008, 14);
    j = 0;
  L2:
    out = (u16 *)0x500000e;
    color = out;
    i = 0;
  L3:
    c = *color;
    r = c & 31;
    g = ((u16)c >> 5) & 31;
    b = ((u16)c >> 10) & 31;
    r += j;
    g += j / 16;
    b -= j;
    if (r > 31)
      r = 31;
    if (g > 31)
      g = 31;
    if (b < 8)
      b = 8;
    if (b > 31)
      b = 31;
    *out = (b << 10) | (g << 5) | r;
    color++;
    out++;
    i++;
    if (i != 8)
      goto L3;
    Call1((void (*)())wait, 4);
    j++;
    if (j != 10)
      goto L2;
    Call3((void (*)())Func_080d47b4, 8, 0x102, 0);
    Call1((void (*)())wait, 30);
    Call1((void (*)())Event_SetMessageId, MsgGoldenSunFormation);
    Call2((void (*)())Event_ShowMessageAndWait, 0x4008, 0);
    Call1((void (*)())wait, 30);
    Call2((void (*)())Motion_SetVarCbAndRefresh, 8, 2);
    Call2((void (*)())Event_ShowMessageAndWait, 0x4008, 0);
    Call2((void (*)())Actor_SetActionById, 8, 32);
    i = 0;
  L4:
    if (i == 16) {
      Call4((void (*)())Camera_MoveWithinMapBounds, 0x13c0000, -1, 0x800000, 1);
      Call2((void (*)())Camera_SetSpeedParameters, 0x20000, 0x4000);
    }
    if (i == 19)
      Event_FinishScreenTransition();
    ACTOR(8)->z -= 0x20000;
    Call1((void (*)())wait, 8);
    i++;
    if (i != 20)
      goto L4;
    Func_02001534((s32)&SceneId_Ov64c, 1);
  } else if (mode == 3) {
    slots = Ram_HeapSlots;
    block = slots->block32;
    CopyToPalette(buffer, (void *)0x5000000, 0x1c0);
    Call4((void (*)())Camera_MoveWithinMapBounds, 0x1000000, -1, 0x900000, 0);
    Call2((void (*)())Camera_SetSpeedParameters, 0x10000, 0x2000);
    Call2((void (*)())Actor_SetModeById, 8, 14);
    object = ACTOR(8);
    scale = 0;
    object->angle = scale;
    ACTOR(8)->x = 0xf80000;
    ACTOR(8)->z = 0x980000;
    IO_WRITE_QUEUE_PUSH((u32)&REG_BLDCNT, 0x27f, 0x20000);
    IO_WRITE_QUEUE_PUSH((u32)&REG_BLDALPHA, 0x1000, 0x20000);
    REG_DISPCNT &= 0xfdff;
    REG_BG1CNT &= 0xfffc;
    REG_BG1CNT |= 3;
    REG_BG3CNT &= 0xfffc;
    REG_BG3CNT |= 2;
    Call1((void (*)())wait, 1);
    ((struct EventWorkView *)slots->eventWork)->unk1ac = scale;
    REG_DISPCNT &= 0xfdff;
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Call3((void (*)())Map_SetScreenShake, 0x40000, 0x40000, 0x10000);
    i = 0;
  L5:
    gBgScroll[2].y += 8;
    Call1((void (*)())Event_Wait, 5);
    i++;
    if (i != 30)
      goto L5;
    Call2((void (*)())Actor_SetModeById, 8, 15);
    Call1((void (*)())wait, 60);
    Call1((void (*)())Sound_Play, 212);
    REG_BG1CNT &= 0xfffc;
    REG_BG1CNT |= 1;
    REG_BLDCNT = 0x3f42;
    IO_WRITE_QUEUE_PUSH((u32)&REG_BLDALPHA, 0x1002, 0x20000);
    j = 0;
  L6:
    if (j == 16) {
      Call1((void (*)())Sound_Play, 140);
      Call2((void (*)())Actor_SetModeById, 8, 16);
    }
    if (j == 51) {
      Call1((void (*)())Sound_Play, 107);
      Iwram_CopyWords(buffer, (void *)0x5000000, 0x1c0);
      Iwram_CopyWords(block + 0xf80, block + 0x1040, 96);
    }
    if (j >= 16 && j <= 28) {
      value = (j - 16) / 2 + 2;
      IO_WRITE_QUEUE_PUSH((u32)&REG_BLDALPHA, value | 0x1000, 0x20000);
    }
    if (j >= 34 && j <= 50) {
      value = (j - 34) / 2 + 8;
      IO_WRITE_QUEUE_PUSH((u32)&REG_BLDALPHA, value | 0x1000, 0x20000);
    }
    if (j >= 20 && j <= 51)
      Call3((void (*)())Func_020002a4, (u16 *)0x5000080, (u16 *)0x50000c0, 16);
    if (j > 49)
      Call3((void (*)())Func_0200032c, (u16 *)0x5000000, 224, 1);
    Call1((void (*)())wait, 4);
    j++;
    if (j != 84)
      goto L6;
    Call2((void (*)())Actor_SetSpritePriority, 8, 3);
    Call3((void (*)())Map_SetScreenShake, -1, -1, 0xe666);
    Call1((void (*)())Sound_Play, 0x186);
    Call4((void (*)())Camera_MoveWithinMapBounds, 0x1000000, -1, 0x13e0000, 0);
    IO_WRITE_QUEUE_PUSH((u32)&REG_BG1CNT, (REG_BG1CNT & ~3) | 3, 0x20000);
    Call1((void (*)())Sound_Play, 212);
    Call1((void (*)())wait, 1);
    Call2((void (*)())Task_Add, Func_02000394, 0x480);
    j = 0;
  L7:
    Call3((void (*)())Func_020002a4, (u16 *)0x5000000, buffer, 224);
    Call1((void (*)())wait, 4);
    j++;
    if (j != 32)
      goto L7;
    Call2((void (*)())Actor_SetSpritePriority, 8, 1);
    Call2((void (*)())Actor_SetModeById, 8, 1);
    Call4((void (*)())Camera_MoveWithinMapBounds, 0x1000000, -1, 0x800000, 1);
    Call2((void (*)())Camera_SetSpeedParameters, 0x10000, 0x2000);
    Func_0200152c();
    Call3((void (*)())Actor_SetSpeedParameters, 8, 0x4ccc, 0x2666);
    Call3((void (*)())Actor_SetPositionAndReset, 8, 248, 128);
    Call2((void (*)())Motion_SetVarCbAndRefresh, 8, 2);
    Call1((void (*)())Event_Wait, 5);
    Call1((void (*)())Sound_Play, 78);
    Call1((void (*)())Event_SetMessageId, MsgGoldenSunEternalLifeAndPower);
    Call3((void (*)())Event_ShowMessageThenWait, 0x2008, 0, 5);
    Call3((void (*)())Actor_ArmCallback, 8, 0x4000, 0);
    Call1((void (*)())Event_Wait, 30);
    Call2((void (*)())Actor_SetModeById, 8, 14);
    Call3((void (*)())Event_ShowMessageThenWait, 0x2008, 0, 5);
    Call2((void (*)())Actor_SetModeById, 8, 15);
    Call3((void (*)())Event_ShowMessageThenWait, 0x2008, 0, 150);
    Func_080d47b4(8, 0x101, 30);
    Call2((void (*)())Actor_SetModeById, 8, 1);
    Call3((void (*)())Actor_ArmCallback, 8, 0x6000, 0);
    Call1((void (*)())Event_Wait, 10);
    Call3((void (*)())Actor_ArmCallback, 8, 0x2000, 0);
    Call1((void (*)())Event_Wait, 10);
    Call3((void (*)())Actor_ArmCallback, 8, 0x6000, 0);
    Call1((void (*)())Event_Wait, 10);
    Call3((void (*)())Actor_ArmCallback, 8, 0x2000, 0);
    Call1((void (*)())Event_Wait, 10);
    Call3((void (*)())Actor_ArmCallback, 8, 0x4000, 0);
    Call1((void (*)())Event_Wait, 50);
    Call2((void (*)())Actor_SetModeById, 8, 15);
    Call1((void (*)())Event_Wait, 50);
    Call3((void (*)())Event_ShowMessageThenWait, 0x2008, 0, 5);
    Call1((void (*)())Event_Wait, 50);
    Call2((void (*)())Motion_SetVarCbAndRefresh, 8, 2);
    Call1((void (*)())Event_Wait, 5);
    Call2((void (*)())Actor_SetSingleChildFlags,
          (s32)Value1((s32 (*)())Actor_GetById, 9), 0);
    Call3((void (*)())Actor_SetPositionAndReset, 9, 216, 96);
    Call3((void (*)())Actor_ArmCallback, 8, 0x2000, 0);
    Call3((void (*)())Func_080d47b4, 8, 0x102, 30);
    Call1((void (*)())Event_SetMessageId,
          (MsgGoldenSunEternalLifeAndPower + 4));
    Call3((void (*)())Event_ShowMessageThenWait, 0x2008, 0, 5);
    Call1((void (*)())Sound_Play, 0x2d2);
    Call3((void (*)())Event_ShowMessageThenWait, 9, 0, 5);
    Call2((void (*)())Actor_SetModeById, 8, 1);
    Call3((void (*)())Actor_ArmCallback, 8, 0xa000, 0);
    Call3((void (*)())Func_080d47b4, 8, 0x100, 30);
    ACTOR(8)->actionFlags &= ~1;
    Call3((void (*)())Actor_ResetAndSetPositionInMode2, 8, 0x100, 136);
    Call3((void (*)())Actor_SetSpeedParameters, 8, 0x9999, 0x4ccc);
    Call1((void (*)())Event_Wait, 20);
    Call2((void (*)())Actor_SetModeById, 8, 1);
    Call3((void (*)())Event_ShowMessageThenWait, 0x2008, 0, 5);
    Call2((void (*)())Actor_SetModeById, 9, 4);
    Call3((void (*)())Event_ShowMessageThenWait, 9, 0, 5);
    Call2((void (*)())Motion_SetVarCbAndRefresh, 8, 2);
    Call1((void (*)())Event_Wait, 5);
    Call3((void (*)())Event_ShowMessageThenWait, 0x2008, 0, 5);
    message = MsgGoldenSunEternalLifeAndPower;
    Call2((void (*)())Actor_SetModeById, 9, 5);
    Call3((void (*)())Event_ShowMessageThenWait, 9, 0, 5);
    Call2((void (*)())Func_080d489c, 8, 0x102);
    Call1((void (*)())Event_Wait, 20);
    Call3((void (*)())Event_ShowMessageThenWait, 0x2008, 0, 5);
    Call2((void (*)())Actor_SetModeById, 9, 6);
    Call3((void (*)())Event_ShowMessageThenWait, 9, 0, 5);
    Call3((void (*)())Func_080d47b4, 8, 0x107, 30);
    message += 12;
    Call1((void (*)())Event_SetMessageId, message);
    Call3((void (*)())Event_ShowMessageThenWait, 0x2008, 0, 5);
    Call2((void (*)())Actor_SetModeById, 8, 13);
    Func_02001574();
    Call2((void (*)())Func_0200158c, (s32)Value1((s32 (*)())Actor_GetById, 8),
          2);
    Call1((void (*)())Sound_Play, 178);
    Call2((void (*)())Func_02001564, 8, 0x4084);
    Call1((void (*)())Event_Wait, 20);
    Call2((void (*)())Actor_SetModeById, 9, 3);
    Call3((void (*)())Actor_SetSpeedParameters, 9, 0x1999, 0xccc);
    j = 0;
  L8:
    if (j & 1) {
      Call3((void (*)())Actor_SetPositionAndReset, 9, 216, 96);
    } else {
      ACTOR(9)->actionFlags &= ~1;
      Call3((void (*)())Actor_ResetAndSetPositionInMode2, 9, 208, 88);
    }
    i = 0;
  L9:
    Call2((void (*)())Actor_ConfigureSpriteColor, 9, 0x100);
    Call1((void (*)())Event_Wait, 3);
    Call2((void (*)())Actor_ConfigureSpriteColor, 9, 0);
    Call1((void (*)())Event_Wait, 3);
    i++;
    if (i != 10)
      goto L9;
    j++;
    if (j != 4)
      goto L8;
    Func_080d47b4(8, 0x108, 30);
    Call3((void (*)())Event_ShowMessageThenWait, 0x2008, 0, 5);
    Call1((void (*)())Event_Wait, 10);
    Call2((void (*)())Func_0200158c, (s32)Value1((s32 (*)())Actor_GetById, 8),
          0);
    Func_02001584();
    Func_0200157c();
    Call2((void (*)())Actor_SetModeById, 9, 6);
    Func_02001574();
    Call2((void (*)())Func_0200158c, (s32)Value1((s32 (*)())Actor_GetById, 9),
          2);
    Call1((void (*)())wait, 30);
    Call1((void (*)())Sound_Play, 212);
    Call3((void (*)())Map_SetScreenShake, 0x100000, 0x100000, 0x10000);
    i = 0;
  L10:
    Call2((void (*)())BattleEffect_ApplyColorToTargetBuffer, 0x10005, 1);
    Call1((void (*)())BattleEffect_StartBufferInterpolation, 1);
    Call1((void (*)())Event_Wait, 2);
    Call2((void (*)())BattleEffect_ApplyColorToTargetBuffer, 0x10000, 1);
    Call1((void (*)())BattleEffect_StartBufferInterpolation, 1);
    Call1((void (*)())Event_Wait, 2);
    i++;
    if (i != 4)
      goto L10;
    Call3((void (*)())Map_SetScreenShake, 0, 0, 0);
    Call2((void (*)())Motion_SetVarCbAndRefresh, 8, 2);
    Call1((void (*)())Field_StartPaletteTransition, 1);
    Call2((void (*)())Func_02001564, 8, 0x1110);
    Call1((void (*)())Event_Wait, 20);
    Call3((void (*)())Actor_SetSpeedParameters, 8, 0x80000, 0x40000);
    Call2((void (*)())Func_02000478, 128, 8);
    ACTOR(8)->actionFlags &= ~1;
    Call3((void (*)())Actor_ResetAndSetPositionInMode2, 8, 0x118, 160);
    Call2((void (*)())Actor_SetModeById, 8, 17);
    Call1((void (*)())Sound_Play, 134);
    Call1((void (*)())Event_Wait, 2);
    Call2((void (*)())Func_02000478, 128, 8);
    Call1((void (*)())Event_Wait, 2);
    Call2((void (*)())Func_02000478, 128, 8);
    Call1((void (*)())Event_Wait, 56);
    Call1((void (*)())Sound_Play, 178);
    i = 0;
    ACTOR(8)->motionFlags = i;
  L11:
    if (i == 16)
      Call2((void (*)())Actor_SetModeById, 8, 18);
    ACTOR(8)->y += 0x2666;
    Call1((void (*)())Event_Wait, 1);
    i++;
    if (i != 64)
      goto L11;
    Call2((void (*)())Motion_SetVarCbAndRefresh, 8, 2);
    Call1((void (*)())Event_Wait, 5);
    Call3((void (*)())Event_ShowMessageThenWait, 0x2008, 0, 5);
    i = 0;
  L12:
    ACTOR(8)->y += 0x10000;
    Call1((void (*)())Event_Wait, 1);
    i++;
    if (i != 32)
      goto L12;
    ACTOR(8)->motionFlags = 1;
    i = 0;
  L13:
    ACTOR(8)->y -= 0x851eb;
    Call1((void (*)())Event_Wait, 1);
    i++;
    if (i != 5)
      goto L13;
    Call2((void (*)())Func_02000478, 0x100, 16);
    Call2((void (*)())Actor_SetModeById, 8, 17);
    Call1((void (*)())Sound_Play, 145);
    Call3((void (*)())Map_SetScreenShake, 0x100000, 0x100000, 0x10000);
    Call1((void (*)())Event_Wait, 10);
    Call3((void (*)())Map_SetScreenShake, -1, -1, 0xe666);
    Call2((void (*)())Func_0200158c, (s32)Value1((s32 (*)())Actor_GetById, 9),
          0);
    Func_02001584();
    Func_0200157c();
    Call1((void (*)())Sound_Play, 0x186);
    Call2((void (*)())Actor_SetModeById, 9, 4);
    Call1((void (*)())Field_StartPaletteTransition, 32);
    Call1((void (*)())Event_Wait, 10);
    Call3((void (*)())Event_ShowMessageThenWait, 9, 0, 5);
    Call2((void (*)())Motion_SetVarCbAndRefresh, 8, 2);
    Call1((void (*)())Event_Wait, 5);
    Call3((void (*)())Event_ShowMessageThenWait, 0x2008, 0, 5);
    Call2((void (*)())Actor_SetModeById, 9, 3);
    Call3((void (*)())Event_ShowMessageThenWait, 9, 0, 5);
    Call2((void (*)())Actor_SetModeById, 8, 18);
    Call1((void (*)())Event_Wait, 10);
    Call3((void (*)())Event_ShowMessageThenWait, 0x2008, 0, 5);
    Call1((void (*)())Event_Wait, 120);
    Call1((void (*)())Sound_Play, 107);
    i = 0;
  L14:
    Call3((void (*)())Map_SetScreenShake, (i << 16) + 0x10000,
          (i << 16) + 0x10000, 0x10000);
    Call1((void (*)())Event_Wait, 8);
    i++;
    if (i != 2)
      goto L14;
    Call2((void (*)())Actor_SetModeById, 9, 8);
    Call1((void (*)())Event_Wait, 60);
    Call2((void (*)())Actor_SetModeById, 9, 7);
    Call3((void (*)())Event_ShowMessageThenWait, 9, 0, 5);
    Func_080d47b4(8, 0x101, 30);
    Call3((void (*)())Event_ShowMessageThenWait, 0x2008, 0, 5);
    Call2((void (*)())Actor_SetModeById, 9, 8);
    Call3((void (*)())Event_ShowMessageThenWait, 9, 0, 5);
    for (i = 0, j = 0x20000; i != 4; i++, j += 0x10000) {
      Call3((void (*)())Map_SetScreenShake, j, j, 0x10000);
      Call1((void (*)())Event_Wait, 8);
    }
    Call2((void (*)())Motion_SetVarCbAndRefresh, 8, 2);
    Call1((void (*)())Event_Wait, 5);
    Call3((void (*)())Event_ShowMessageThenWait, 0x2008, 0, 5);
    Call2((void (*)())Actor_SetModeById, 9, 3);
    Call3((void (*)())Event_ShowMessageThenWait, 9, 0, 5);
    Call2((void (*)())Motion_SetVarCbAndRefresh, 8, 2);
    Call1((void (*)())Event_Wait, 5);
    Call2((void (*)())Actor_SetModeById, 9, 4);
    Call3((void (*)())Event_ShowMessageThenWait, 9, 0, 5);
    Call1((void (*)())Event_Wait, 10);
    slots = (struct HeapSlots *)Ram_HeapSlots->mapWork;
    Call3((void (*)())Map_SetScreenShake, 0x100000, 0x100000, 0x10000);
    j = 0;
    scale = -0x1e000;
  L16:
    if (j == 20)
      Call2((void (*)())Actor_SetModeById, 9, 7);
    if (j >= 30 && j <= 46) {
      ACTOR(9)->scaleX = 0x10000 - scale;
      ACTOR(9)->scaleY = scale + 0x10000;
      ACTOR(9)->z -= 0x20000;
    }
    if (j == 47)
      Call3((void (*)())Actor_SetHorizontalPositionWithTerrain, 9, 0, 0);
    if (j == 64) {
      Call2((void (*)())BattleEffect_ApplyColorToTargetBuffer, 0x7fff, 0);
      Call1((void (*)())BattleEffect_StartBufferInterpolation, 64);
    }
    FIELD(slots, 332) -= 0x10000;
    ACTOR(8)->z += 0x10000;
    Call1((void (*)())wait, 1);
    j++;
    scale += 0x1000;
    if (j != 128)
      goto L16;
    Call1((void (*)())Sound_Play, 78);
    Call1((void (*)())Event_Wait, 32);
    Call1((void (*)())Sound_Play, 208);
    Call1((void (*)())Event_Wait, 120);
    Call1((void (*)())Task_RemoveFar, Func_02000394);
    Call2((void (*)())BattleEffect_ApplyColorToTargetBuffer, 0x7fff, 0);
    Call1((void (*)())BattleEffect_StartBufferInterpolation, 1);
    Call1((void (*)())wait, 1);
    Call1((void (*)())Sound_Play, 0x186);
    Call2((void (*)())BattleEffect_ApplyColorToTargetBuffer, 0, 0);
    Call1((void (*)())BattleEffect_StartBufferInterpolation, 120);
    Call1((void (*)())wait, 0x12c);
    Func_02001534((s32)&SceneId_Ov6b3, 1);
  } else {
    ((struct EventWorkView *)Ram_HeapSlots->eventWork)->unk1ac = 0;
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    for (j = 0; j != 0x140; j++) {
      held = &gInput.held;
      if (*held & 2)
        j = 0;
      if (*held & 1)
        break;
      if (j == 0) {
        Call2((void (*)())Actor_SetSingleChildFlags,
              (s32)Value1((s32 (*)())Actor_GetById, 8), 0);
        Call2((void (*)())Actor_SetModeById, 8, 12);
      }
    }
    Func_02001534((s32)&SceneId_Ov64c, 0);
  }
  Heap_Release(buffer);
  return 0;
}
