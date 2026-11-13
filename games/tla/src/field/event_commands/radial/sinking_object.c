/// @file
/// An effect object that sinks and shrinks.
// Part of radial.c.

#include "../radial_internal.h"

struct RiseWork {
  u8 unk0000[0x18a8];
  struct Actor *object;
  s16 done;
  s16 timer;
};

struct AnchorWork {
  u8 unk0000[0x1930];
  struct Actor *anchor;
};

struct ModeEvent {
  u8 unk000[352];
  u16 cue;
};

void Camera_WorldToScreen(s32 *position);
struct Actor *Actor_GetById(u32 objectId);

/// Per-frame callback: until done, moves the effect object down 2.5 pixels
/// and shrinks it by 1/64 each frame, marking done at frame 8.
void BattleEffect_SinkAndShrinkObject(void) {
  struct RiseWork *work = Ram_HeapSlots->battleFxWork[0];

  if (work->done == 0) {
    struct Actor *object = work->object;

    object->y += -0x28000;
    object->scaleX += -0x400;
    object->scaleY += -0x400;
    if (work->timer == 8)
      work->done = 999;
  }
  work->timer++;
}

void FieldPsynergy_Blaze(void);
void FieldPsynergy_Sand(void);
void FieldPsynergy_Burst(void);
void FieldPsynergy_Pound(void);
void FieldPsynergy_Scoop(void);
void FieldPsynergy_Parch(void);
void FieldPsynergy_Hover(void);

/// Runs field effect 18 (FieldPsynergy_Pound) for the request dispatcher.
void FieldPsynergy_PoundOnTarget(void) { FieldPsynergy_Pound(); }
