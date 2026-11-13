/// @file
/// Overlay 0x6b4's code after field/field_effect/advance_motion, in ROM order.

#include "types.h"
#include "system.h"
#include "actor.h"
#include "field_sprite.h"
#include "field_effect.h"
#include "vector3.h"
#include "event_inline.h"
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

#include "scene_tables.h"
#include "field.h"
#include "event_work.h"
#include "module/field/event_commands.h"
#include "module/field/field_effect.h"
#include "module/scene/contigo.h"
#include "module/scene/mt_aleph_summit.h"
#include "module/scene/scene.h"
#include "module/system/system.h"
#include "text/msg_ids.h"

void Func_020000b8(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   const struct EffectOptions *options);

void Func_02000478(s32 radius, u32 count) {
  struct Actor *object;
  struct EffectOptions options;
  struct Vector3 dir;
  u32 i;
  s32 angle;

  object = Actor_Get(8);
  options.update = Effect_AdvanceMotion;
  options.startScaleX = 0xcccc;
  options.startScaleY = 0xcccc;
  options.priority = ((struct FieldSprite *)object->sprite)->priority;
  i = 0;
next:
  angle = i * (0x10000 / (s32)count);
  dir.x = Math_Cosine(angle) * radius / 256;
  dir.y = 0;
  dir.z = Math_Sin(angle) * radius / 256;
  dir.x += dir.x >> 1;
  dir.x += ((Random16Far() * 3) << 14 >> 16) - 0x6000;
  dir.z += ((Random16Far() * 3) << 13 >> 16) - 0x3000;
  Func_020000b8(object->x, object->y, object->z, dir.x, dir.y, dir.z,
                EFFECT_USE_UPDATE | EFFECT_USE_START_SCALE |
                    EFFECT_USE_PRIORITY | 1,
                &options);
  i++;
  if (i <= count)
    goto next;
}

#define ACTOR(id) (Actor_Get(id))
#define FIELD(p, off) (*(s32 *)((u8 *)(p) + (off)))

/* VIEW(s0478): tag struct EventWorkView as struct EventWorkView__s0478; module/tla.h has another definition; tried with the definition the rest of the object has, it does not compile in tla-ja: games/tla/src/scene/mt_aleph_summit/s0478.c:108: dereferencing pointer to incomplete type. */
struct EventWorkView__s0478 {
  u8 unk000[0x1ac];
  s32 unk1ac;
};

void Func_020002a4(u16 *colors, u16 *target, s32 count);
void Func_0200032c(u16 *colors, s32 count, s32 delta);
void Func_02000394(void);
void Func_02001564(s32 a, s32 b);

static __inline__ void CopyToPalette(void *from, void *to, s32 size) {
  /* FAKEMATCH(wrapper): the copy forwarded inline; as Call3, 4 of 1526
   * instructions differ in tla-en. */
  Iwram_CopyWords(from, to, size);
}

s32 Scene_Initialize(void) {
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
  Window_LoadGraphics();
  WindowColor_Apply(gMainRoutineWork.paletteGlow[0], gMainRoutineWork.paletteGlow[1]);
  Actor_SetPosition(4, 0, 0);
  mode = gMainRoutineWork.entrance;
  if (mode == 10) {
    Event_SetDestination((s32)&SceneId_Ov64c, 72);
  } else if (mode == 2) {
    FieldCamera_MoveTo(0x1000000, -1, 0x800000, 0);
    FieldCamera_SetSpeed(0x10000, 0x2000);
    Call2((void (*)())Actor_SetSingleChildFlags,
          (s32)(s32)Actor_Get(8), 0);
    Actor_SetAnimation(8, 12);
    Actor_SetAnimationSpeedById(8, 8);
    ((struct EventWorkView__s0478 *)Ram_HeapSlots->eventWork)->unk1ac = 0;
    REG_DISPCNT &= 0xfdff;
    REG_BG1CNT &= 0xfffc;
    REG_BG1CNT |= 3;
    REG_BG3CNT &= 0xfffc;
    REG_BG3CNT |= 2;
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    FieldCamera_MoveTo(0x1000000, -1, 0x13e0000, 1);
    Func_0200152c();
    FieldCamera_MoveTo(0x13c0000, -1, 0x13e0000, 1);
    Func_0200152c();
    i = 0;
  L1:
    if (i == 3) {
      FieldCamera_SetSpeed(0x1999, 0x333);
      FieldCamera_MoveTo(0x13c0000, -1, 0x12c0000, 1);
    }
    ACTOR(8)->z -= 0x10000;
    System_Wait(30);
    i++;
    if (i != 10)
      goto L1;
    Actor_SetAnimationSpeedById(8, 0);
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
    System_Wait(4);
    j++;
    if (j != 10)
      goto L2;
    Actor_ShowEmote(8, 0x102, 0);
    System_Wait(30);
    FieldEvent_SetMessageId(MsgGoldenSunFormation);
    Event_ShowMessage(0x4008, 0);
    System_Wait(30);
    Actor_RunRepeatedMotion(8, 2);
    Event_ShowMessage(0x4008, 0);
    Actor_SetAnimationSpeedById(8, 32);
    i = 0;
  L4:
    if (i == 16) {
      FieldCamera_MoveTo(0x13c0000, -1, 0x800000, 1);
      FieldCamera_SetSpeed(0x20000, 0x4000);
    }
    if (i == 19)
      Event_FinishScreenTransition();
    ACTOR(8)->z -= 0x20000;
    System_Wait(8);
    i++;
    if (i != 20)
      goto L4;
    Event_SetDestination((s32)&SceneId_Ov64c, 1);
  } else if (mode == 3) {
    slots = Ram_HeapSlots;
    block = slots->block32;
    CopyToPalette(buffer, (void *)0x5000000, 0x1c0);
    FieldCamera_MoveTo(0x1000000, -1, 0x900000, 0);
    FieldCamera_SetSpeed(0x10000, 0x2000);
    Actor_SetAnimation(8, 14);
    object = ACTOR(8);
    scale = 0;
    object->facing = scale;
    ACTOR(8)->x = 0xf80000;
    ACTOR(8)->z = 0x980000;
    IO_WRITE_QUEUE_PUSH((u32)&REG_BLDCNT, 0x27f, 0x20000);
    IO_WRITE_QUEUE_PUSH((u32)&REG_BLDALPHA, 0x1000, 0x20000);
    REG_DISPCNT &= 0xfdff;
    REG_BG1CNT &= 0xfffc;
    REG_BG1CNT |= 3;
    REG_BG3CNT &= 0xfffc;
    REG_BG3CNT |= 2;
    System_Wait(1);
    ((struct EventWorkView__s0478 *)slots->eventWork)->unk1ac = scale;
    REG_DISPCNT &= 0xfdff;
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    MapGrid_SetShake(0x40000, 0x40000, 0x10000);
    i = 0;
  L5:
    gBgScroll[2].y += 8;
    FieldEvent_Wait(5);
    i++;
    if (i != 30)
      goto L5;
    Actor_SetAnimation(8, 15);
    System_Wait(60);
    Event_PlaySound(212);
    REG_BG1CNT &= 0xfffc;
    REG_BG1CNT |= 1;
    REG_BLDCNT = 0x3f42;
    IO_WRITE_QUEUE_PUSH((u32)&REG_BLDALPHA, 0x1002, 0x20000);
    j = 0;
  L6:
    if (j == 16) {
      Event_PlaySound(140);
      Actor_SetAnimation(8, 16);
    }
    if (j == 51) {
      Event_PlaySound(107);
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
      Func_020002a4((u16 *)0x5000080, (u16 *)0x50000c0, 16);
    if (j > 49)
      Call3((void (*)())Func_0200032c, (u16 *)0x5000000, 224, 1);
    System_Wait(4);
    j++;
    if (j != 84)
      goto L6;
    FieldActor_SetSpritePriority(8, 3);
    MapGrid_SetShake(-1, -1, 0xe666);
    Event_PlaySound(0x186);
    FieldCamera_MoveTo(0x1000000, -1, 0x13e0000, 0);
    IO_WRITE_QUEUE_PUSH((u32)&REG_BG1CNT, (REG_BG1CNT & ~3) | 3, 0x20000);
    Event_PlaySound(212);
    System_Wait(1);
    Task_AddFrame(Func_02000394, 0x480);
    j = 0;
  L7:
    Func_020002a4((u16 *)0x5000000, buffer, 224);
    System_Wait(4);
    j++;
    if (j != 32)
      goto L7;
    FieldActor_SetSpritePriority(8, 1);
    Actor_SetAnimation(8, 1);
    FieldCamera_MoveTo(0x1000000, -1, 0x800000, 1);
    FieldCamera_SetSpeed(0x10000, 0x2000);
    Func_0200152c();
    Actor_SetSpeed(8, 0x4ccc, 0x2666);
    Actor_WalkToAndWait(8, 248, 128);
    Actor_RunRepeatedMotion(8, 2);
    FieldEvent_Wait(5);
    Event_PlaySound(78);
    FieldEvent_SetMessageId(MsgGoldenSunEternalLifeAndPower);
    FieldEvent_ShowMessageThenWait(0x2008, 0, 5);
    Actor_FaceDirection(8, 0x4000, 0);
    FieldEvent_Wait(30);
    Actor_SetAnimation(8, 14);
    FieldEvent_ShowMessageThenWait(0x2008, 0, 5);
    Actor_SetAnimation(8, 15);
    FieldEvent_ShowMessageThenWait(0x2008, 0, 150);
    Actor_ShowEmote(8, 0x101, 30);
    Actor_SetAnimation(8, 1);
    Actor_FaceDirection(8, 0x6000, 0);
    FieldEvent_Wait(10);
    Actor_FaceDirection(8, 0x2000, 0);
    FieldEvent_Wait(10);
    Actor_FaceDirection(8, 0x6000, 0);
    FieldEvent_Wait(10);
    Actor_FaceDirection(8, 0x2000, 0);
    FieldEvent_Wait(10);
    Actor_FaceDirection(8, 0x4000, 0);
    FieldEvent_Wait(50);
    Actor_SetAnimation(8, 15);
    FieldEvent_Wait(50);
    FieldEvent_ShowMessageThenWait(0x2008, 0, 5);
    FieldEvent_Wait(50);
    Actor_RunRepeatedMotion(8, 2);
    FieldEvent_Wait(5);
    Call2((void (*)())Actor_SetSingleChildFlags,
          (s32)(s32)Actor_Get(9), 0);
    Actor_WalkToAndWait(9, 216, 96);
    Actor_FaceDirection(8, 0x2000, 0);
    Actor_ShowEmote(8, 0x102, 30);
    FieldEvent_SetMessageId((MsgGoldenSunEternalLifeAndPower + 4));
    FieldEvent_ShowMessageThenWait(0x2008, 0, 5);
    Event_PlaySound(0x2d2);
    FieldEvent_ShowMessageThenWait(9, 0, 5);
    Actor_SetAnimation(8, 1);
    Actor_FaceDirection(8, 0xa000, 0);
    Actor_ShowEmote(8, 0x100, 30);
    ACTOR(8)->actionFlags &= ~1;
    Actor_WalkTo(8, 0x100, 136);
    Actor_SetSpeed(8, 0x9999, 0x4ccc);
    FieldEvent_Wait(20);
    Actor_SetAnimation(8, 1);
    FieldEvent_ShowMessageThenWait(0x2008, 0, 5);
    Actor_SetAnimation(9, 4);
    FieldEvent_ShowMessageThenWait(9, 0, 5);
    Actor_RunRepeatedMotion(8, 2);
    FieldEvent_Wait(5);
    FieldEvent_ShowMessageThenWait(0x2008, 0, 5);
    message = MsgGoldenSunEternalLifeAndPower;
    Actor_SetAnimation(9, 5);
    FieldEvent_ShowMessageThenWait(9, 0, 5);
    Actor_SetAttachedEffect(8, 0x102);
    FieldEvent_Wait(20);
    FieldEvent_ShowMessageThenWait(0x2008, 0, 5);
    Actor_SetAnimation(9, 6);
    FieldEvent_ShowMessageThenWait(9, 0, 5);
    Actor_ShowEmote(8, 0x107, 30);
    message += 12;
    FieldEvent_SetMessageId(message);
    FieldEvent_ShowMessageThenWait(0x2008, 0, 5);
    Actor_SetAnimation(8, 13);
    BattleEffect_StartArcEffect();
    BattleEffect_SetObjectUpdateMode((s32)Actor_Get(8), 2);
    Event_PlaySound(178);
    Func_02001564(8, 0x4084);
    FieldEvent_Wait(20);
    Actor_SetAnimation(9, 3);
    Actor_SetSpeed(9, 0x1999, 0xccc);
    j = 0;
  L8:
    if (j & 1) {
      Actor_WalkToAndWait(9, 216, 96);
    } else {
      ACTOR(9)->actionFlags &= ~1;
      Actor_WalkTo(9, 208, 88);
    }
    i = 0;
  L9:
    Actor_SetSpriteColorById(9, 0x100);
    FieldEvent_Wait(3);
    Actor_SetSpriteColorById(9, 0);
    FieldEvent_Wait(3);
    i++;
    if (i != 10)
      goto L9;
    j++;
    if (j != 4)
      goto L8;
    Actor_ShowEmote(8, 0x108, 30);
    FieldEvent_ShowMessageThenWait(0x2008, 0, 5);
    FieldEvent_Wait(10);
    BattleEffect_SetObjectUpdateMode((s32)Actor_Get(8), 0);
    BattleEffect_WaitForArcGroups();
    BattleEffect_EndArcEffect();
    Actor_SetAnimation(9, 6);
    BattleEffect_StartArcEffect();
    BattleEffect_SetObjectUpdateMode((s32)Actor_Get(9), 2);
    System_Wait(30);
    Event_PlaySound(212);
    MapGrid_SetShake(0x100000, 0x100000, 0x10000);
    i = 0;
  L10:
    PaletteFade_ApplyTarget(0x10005, 1);
    PaletteFade_Interpolate(1);
    FieldEvent_Wait(2);
    PaletteFade_ApplyTarget(0x10000, 1);
    PaletteFade_Interpolate(1);
    FieldEvent_Wait(2);
    i++;
    if (i != 4)
      goto L10;
    MapGrid_SetShake(0, 0, 0);
    Actor_RunRepeatedMotion(8, 2);
    Field_StartPaletteTransition(1);
    Func_02001564(8, 0x1110);
    FieldEvent_Wait(20);
    Actor_SetSpeed(8, 0x80000, 0x40000);
    Func_02000478(128, 8);
    ACTOR(8)->actionFlags &= ~1;
    Actor_WalkTo(8, 0x118, 160);
    Actor_SetAnimation(8, 17);
    Event_PlaySound(134);
    FieldEvent_Wait(2);
    Func_02000478(128, 8);
    FieldEvent_Wait(2);
    Func_02000478(128, 8);
    FieldEvent_Wait(56);
    Event_PlaySound(178);
    i = 0;
    ACTOR(8)->motionFlags = i;
  L11:
    if (i == 16)
      Actor_SetAnimation(8, 18);
    ACTOR(8)->y += 0x2666;
    FieldEvent_Wait(1);
    i++;
    if (i != 64)
      goto L11;
    Actor_RunRepeatedMotion(8, 2);
    FieldEvent_Wait(5);
    FieldEvent_ShowMessageThenWait(0x2008, 0, 5);
    i = 0;
  L12:
    ACTOR(8)->y += 0x10000;
    FieldEvent_Wait(1);
    i++;
    if (i != 32)
      goto L12;
    ACTOR(8)->motionFlags = 1;
    i = 0;
  L13:
    ACTOR(8)->y -= 0x851eb;
    FieldEvent_Wait(1);
    i++;
    if (i != 5)
      goto L13;
    Func_02000478(0x100, 16);
    Actor_SetAnimation(8, 17);
    Event_PlaySound(145);
    MapGrid_SetShake(0x100000, 0x100000, 0x10000);
    FieldEvent_Wait(10);
    MapGrid_SetShake(-1, -1, 0xe666);
    BattleEffect_SetObjectUpdateMode((s32)Actor_Get(9), 0);
    BattleEffect_WaitForArcGroups();
    BattleEffect_EndArcEffect();
    Event_PlaySound(0x186);
    Actor_SetAnimation(9, 4);
    Field_StartPaletteTransition(32);
    FieldEvent_Wait(10);
    FieldEvent_ShowMessageThenWait(9, 0, 5);
    Actor_RunRepeatedMotion(8, 2);
    FieldEvent_Wait(5);
    FieldEvent_ShowMessageThenWait(0x2008, 0, 5);
    Actor_SetAnimation(9, 3);
    FieldEvent_ShowMessageThenWait(9, 0, 5);
    Actor_SetAnimation(8, 18);
    FieldEvent_Wait(10);
    FieldEvent_ShowMessageThenWait(0x2008, 0, 5);
    FieldEvent_Wait(120);
    Event_PlaySound(107);
    i = 0;
  L14:
    MapGrid_SetShake((i << 16) + 0x10000, (i << 16) + 0x10000, 0x10000);
    FieldEvent_Wait(8);
    i++;
    if (i != 2)
      goto L14;
    Actor_SetAnimation(9, 8);
    FieldEvent_Wait(60);
    Actor_SetAnimation(9, 7);
    FieldEvent_ShowMessageThenWait(9, 0, 5);
    Actor_ShowEmote(8, 0x101, 30);
    FieldEvent_ShowMessageThenWait(0x2008, 0, 5);
    Actor_SetAnimation(9, 8);
    FieldEvent_ShowMessageThenWait(9, 0, 5);
    for (i = 0, j = 0x20000; i != 4; i++, j += 0x10000) {
      MapGrid_SetShake(j, j, 0x10000);
      FieldEvent_Wait(8);
    }
    Actor_RunRepeatedMotion(8, 2);
    FieldEvent_Wait(5);
    FieldEvent_ShowMessageThenWait(0x2008, 0, 5);
    Actor_SetAnimation(9, 3);
    FieldEvent_ShowMessageThenWait(9, 0, 5);
    Actor_RunRepeatedMotion(8, 2);
    FieldEvent_Wait(5);
    Actor_SetAnimation(9, 4);
    FieldEvent_ShowMessageThenWait(9, 0, 5);
    FieldEvent_Wait(10);
    slots = (struct HeapSlots *)Ram_HeapSlots->mapWork;
    MapGrid_SetShake(0x100000, 0x100000, 0x10000);
    j = 0;
    scale = -0x1e000;
  L16:
    if (j == 20)
      Actor_SetAnimation(9, 7);
    if (j >= 30 && j <= 46) {
      ACTOR(9)->scaleX = 0x10000 - scale;
      ACTOR(9)->scaleY = scale + 0x10000;
      ACTOR(9)->z -= 0x20000;
    }
    if (j == 47)
      Actor_SetPosition(9, 0, 0);
    if (j == 64) {
      PaletteFade_ApplyTarget(0x7fff, 0);
      PaletteFade_Interpolate(64);
    }
    FIELD(slots, 332) -= 0x10000;
    ACTOR(8)->z += 0x10000;
    System_Wait(1);
    j++;
    scale += 0x1000;
    if (j != 128)
      goto L16;
    Event_PlaySound(78);
    FieldEvent_Wait(32);
    Event_PlaySound(208);
    FieldEvent_Wait(120);
    FieldTask_Remove(Func_02000394);
    PaletteFade_ApplyTarget(0x7fff, 0);
    PaletteFade_Interpolate(1);
    System_Wait(1);
    Event_PlaySound(0x186);
    PaletteFade_ApplyTarget(0, 0);
    PaletteFade_Interpolate(120);
    System_Wait(0x12c);
    Event_SetDestination((s32)&SceneId_Ov6b3, 1);
  } else {
    ((struct EventWorkView__s0478 *)Ram_HeapSlots->eventWork)->unk1ac = 0;
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
              (s32)(s32)Actor_Get(8), 0);
        Actor_SetAnimation(8, 12);
      }
    }
    Event_SetDestination((s32)&SceneId_Ov64c, 0);
  }
  Heap_Release(buffer);
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }
