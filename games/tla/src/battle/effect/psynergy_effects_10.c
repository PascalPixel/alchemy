#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "iwram_call.h"

struct EffectSprite {
  u8 unk00[4];
  u16 attr0_0 : 13;
  u16 color : 1;
  u16 attr0_14 : 2;
  u16 attr1;
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u8 unk0a[2];
  s32 x;
  s32 y;
  s32 scaleX;
  s32 scaleY;
  u8 unk1c[2];
  u16 depth;
  u8 unk20[8];
};

struct EffectParticle {
  s32 x;
  s32 y;
  s32 z;
  s32 velocityX;
  s32 velocityY;
  s32 velocityZ;
  s32 timer;
};

struct RiseWork {
  u8 unk0000[0x18a8];
  struct ObjectRuntime *object;
  s16 done;
  s16 timer;
};

struct AnchorWork {
  u8 unk0000[0x1930];
  struct ObjectRuntime *anchor;
};

struct ModeEvent {
  u8 unk000[352];
  u16 cue;
};

void Func_080eb01c(struct EffectSprite *sprite);
void Camera_WorldToScreen(s32 *position);
void BattleFx_IntegrateVector3(struct EffectParticle *particle, s32 frames,
                               s32 flags);
struct ObjectRuntime *Object_GetById(s32 id);
void WaitFrames(s32 frames);
s32 Trig_Sin(s32 angle);

/// Per-frame callback: until done, moves the effect object down 2.5 pixels
/// and shrinks it by 1/64 each frame, marking done at frame 8.
void BattleFx_SinkAndShrinkObject(void) {
  struct RiseWork *work = Ram_HeapSlots->battleFxWork[0];

  /* FAKEMATCH: ⚓️ loads the effect work before it builds the flag offset */
  asm volatile("");
  if (work->done == 0) {
    struct ObjectRuntime *object = work->object;

    object->y += -0x28000;
    object->scaleX += -0x400;
    object->scaleY += -0x400;
    if (work->timer == 8)
      work->done = 999;
  }
  work->timer++;
}
