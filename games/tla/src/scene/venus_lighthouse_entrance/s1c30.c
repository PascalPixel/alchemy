/// @file
/// Scene code of overlay 0x64d, in ROM order, from s1c30's offset up to the next object the overlay links.

#include "types.h"
#include "ram_buffer.h"
#include "scene_tables.h"
#include "event_inline.h"
#include "actor.h"
#include "call.h"
#include "iwram_call.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

s32 Func_020028b4();
s32 Actor_SetSingleChildFlags();
s32 Func_02002994();
s32 Event_StartScreenTransition();
s32 Event_WaitTransitionFrames();
void Func_020028bc(void);
extern u8 MsgAreAreYou[];

void Func_02001c30(void) {
  s32 t96;

  Func_020028b4();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetPosition(5, PIXELS(364), PIXELS(156));
  Actor_SetPosition(8, PIXELS(362), PIXELS(142));
  Actor_SetPosition(9, PIXELS(390), PIXELS(144));
  Actor_FaceDirection(5, FACING_SOUTH, 0);
  Actor_FaceDirection(8, 0x3000, 0);
  Actor_FaceDirection(9, FACING_SOUTH, 0);
  FieldActor_SetSpritePriority(8, 2);
  Actor_SetSpriteRotationById(5, 0x4000);
  Actor_SetSingleChildFlags(((s32)Actor_Get(5)), 0);
  Func_02002994(5, 0);
  FIELD_AT(((s32)Ram_HeapSlots->eventWork + 0x1b4), s32, 0) = 0x3c;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_Wait(20);
  FieldEvent_SetMessageId((s32)MsgAreAreYou);
  Actor_ShowEmote(8, 0x102, 40);
  FieldEvent_ShowMessageThenWait(8, 0, 0xa);
  Actor_RunRepeatedMotion(5, 2);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(9, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(9, 0, 0xa);
  Actor_RunRepeatedMotion(8, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(8, 0, 0xa);
  Actor_RunRepeatedMotion(5, 2);
  FieldEvent_Wait(30);
  Actor_SetPosition(5, PIXELS(376), PIXELS(164));
  Actor_SetSpriteRotationById(5, 0);
  Actor_SetSingleChildFlags(((s32)Actor_Get(5)), 1);
  FieldActor_Jump(5, 6, 50);
  Actor_SetAnimationAndWait(5, 4);
  FieldEvent_Wait(20);
  Actor_WalkByAndWait(5, -14, -2);
  Actor_FaceDirection(5, FACING_NORTH, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(5, 3);
  FieldEvent_Wait(20);
  FieldEvent_Wait(10);
  Actor_FaceDirection(9, FACING_SOUTHWEST, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(9, 0, 0xa);
  FieldEvent_Wait(10);
  Actor_FaceDirection(8, 0, 0);
  Actor_FaceDirection(5, 0xf000, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(8, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(8, 0, 0xa);
  Actor_SetAnimationAndWait(9, 4);
  FieldEvent_ShowMessageThenWait(9, 0, 0xa);
  Actor_SetSpeed(9, 0xcccc, 0x6666);
  Actor_WalkByAndWait(9, 0, 0x10);
  Actor_FaceDirection(9, FACING_WEST, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(9, 0, 0xa);
  Actor_RunRepeatedMotion(9, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(9, 0, 0xa);
  Actor_SetAnimationAndWait(9, 3);
  FieldEvent_Wait(20);
  Actor_SetAnimation(5, 3);
  Actor_SetAnimationAndWait(8, 3);
  FieldEvent_Wait(20);
  FieldActor_WatchAndSetScript(5, 9);
  Actor_SetSpeed(9, 0xcccc, 0x6666);
  Actor_WalkBy(9, 0, 0x78);
  FieldEvent_Wait(40);
  Actor_SetSpeed(8, 0xcccc, 0x6666);
  Actor_WalkByAndWait(8, 0x18, 0);
  Actor_WalkByAndWait(8, 0, 0x14);
  Actor_FaceDirection(5, 0x3000, 0);
  FieldActor_WaitForMoveById(9);
  Actor_SetPosition(9, 0, 0);
  FieldEvent_Wait(10);
  Actor_ShowEmote(8, 0x105, 30);
  FieldEvent_ShowMessageThenWait(8, 0, 0xa);
  Actor_SetAnimationAndWait(5, 3);
  FieldEvent_Wait(30);
  Actor_RunRepeatedMotion(8, 2);
  FieldEvent_Wait(20);
  FieldActor_Jump(8, 4, 0);
  Actor_FaceDirection(8, FACING_WEST, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(8, 0, 0xa);
  Actor_FaceDirection(5, 0, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(5, 3);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(8, 3);
  FieldEvent_Wait(20);
  Actor_SetSpeed(8, 0x13333, 0x9999);
  Actor_SetAnimation(8, 2);
  t96 = ((s32)Actor_Get(5));
  if (t96 != 0) {
    Actor_SetDestination(8, FIELD_AT(t96, s16, 10),
                              FIELD_AT(t96, s16, 18));
  }
  FieldActor_WaitForMoveById(8);
  Actor_SetPosition(8, 0, 0);
  FIELD_AT(((s32)Ram_HeapSlots->eventWork + 0x1b4), s32, 0) = 8;
  Func_020028bc();
}

struct GlowState {
  s32 x; ///< 16.16 pixels
  s32 y; ///< 16.16 pixels
  s32 z; ///< 16.16 pixels
  s32 velocityX;
  s32 velocityY;
  s32 velocityZ;
  s32 frame;
};

struct GlowSprite {
  u8 unk00[4];
  u16 attr0 : 13;
  u16 fullColor : 1;
  u16 shape : 2;
  u16 attr1;
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u8 unk0a[20];
  u16 unk1e;
  u8 unk20[8];
};

struct GlowWork {
  u8 unk000[0x400];
  struct GlowSprite sprites[16];
  struct GlowState states[16];
  struct Actor *object;
  struct Actor *effect;
  s32 x;
  s32 y;
  s32 z;
  s16 stage;
  s16 timer;
  u16 tileTop;
  u16 tile;
  s16 height;
  s8 follow;
};

void Func_02002a14(struct GlowSprite *sprite, struct GlowState *state);

void Func_02001fd0(void) {
  struct GlowWork *work = Ram_HeapSlots->menuRuntime;
  struct Actor *object = work->object;
  struct Actor *effect = work->effect;
  s32 i, y;
  s32 scale;
  s32 frameStep;
  s32 mask;
  struct GlowState *state;
  struct GlowSprite *sprite;

  switch (work->stage) {
  case 0:
    if (work->timer == 0) {
      work->stage++;
      work->timer = -1;
    }
    break;
  case 1:
    work->height += 4;
    if (work->height > 31) {
      work->stage++;
      work->timer = -1;
    }
    break;
  case 2:
    if (work->timer == 90) {
      work->stage++;
      work->timer = -1;
    }
    break;
  case 3:
    if (work->timer == 0) {
      object->motionFlags = 3;
      object->velocityY = 0x180000;
      work->follow = 0;
      Sound_Play(134);
    }
    work->height += 8;
    if (work->height > 79) {
      work->stage++;
      work->timer = -1;
    }
    break;
  case 4: {
    s16 *timer = &work->timer;

    if (work->timer == 10) {
      object->z = 0;
      object->x = 0;
    }
    if (*timer == 30) {
      work->stage++;
      work->timer = -1;
    }
    break;
  }
  case 5:
    work->height -= 8;
    if (work->height <= 0) {
      work->stage++;
      work->timer = -1;
    }
    break;
  case 6: {
    s16 *timer;

    scale = effect->scaleX;
    if (scale > 0)
      scale -= 0x51e;
    else
      scale = 0;
    effect->scaleX = scale;
    effect->scaleY = scale;
    /* FAKEMATCH(dead): an empty once-only block after the scale's stores, which
     * the timer's address follows; as no block, 2 of 449 instructions differ in
     * tla-en. */
    do {
    } while (0);
    timer = &work->timer;
    if (work->timer == 10)
      Sound_Play(310);
    if (*timer == 90) {
      work->stage++;
      work->timer = -1;
    }
    break;
  }
  case 7: {
    s16 *timer = &work->timer;

    if (work->timer == 0) {
      Sound_Play(144);
      MapGrid_SetShake(0x30000, 0x30000, 0x10000);
    }
    if (*timer == 30) {
      MapGrid_SetShake(-1, -1, 0xe666);
      if (work->timer == 30) {
        work->stage = 999;
        work->timer = -1;
      }
    }
    break;
  }
  }
  object->angle += 0x1000;
  if (work->height > 0) {
    for (i = 0, y = 0; i < 16 && y < work->height - 8; i++, y += 16) {
      state = &work->states[i];
      state->x = work->x;
      state->y = work->y + (y << 16) + 0x80000;
      state->z = work->z;
      sprite = &work->sprites[i];
      state->frame++;
      frameStep = state->frame >> 2;
      mask = 3;
      sprite->tile = work->tileTop + (frameStep & mask) * 8;
      Func_02002a14(sprite, state);
    }
    if (i <= 15) {
      state = &work->states[i];
      state->x = work->x;
      state->y = work->y + (work->height << 16);
      state->z = work->z;
      state->frame++;
      state->y += Value2((s32 (*)())Iwram_MulQ16, 0xfffe0000,
                         Math_Sin(state->frame << 12)) +
                  0xfffe0000;
      sprite = &work->sprites[i];
      sprite->tile = work->tile + ((state->frame >> 2) & 3) * 8;
      Func_02002a14(sprite, state);
      if (work->follow != 0)
        object->y = state->y;
    }
  }
  work->timer++;
}
