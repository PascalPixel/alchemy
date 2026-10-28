// Near miss, differ 91 (105 before statements reordered 2026-10-26) of 290 (2026-10-15): the wave scroll's per-frame
// callback, which wavefx.c removes. The delay, the wave lines and the
// colour match but for one pair (this builds the page field's offset
// before the phase's store, the native after it); from the two
// Scene_FindActorPlacement lookups on, the registers differ: this
// builds &from in sl first and copies it back (one instruction more, 291
// against 290), and the orientation tests take r1 and r2 where the native
// takes r0 and r1. Belongs between Func_080dcadc and hue.c's
// BattleEffect_ArmBg0HBlankDma.
#include "types.h"
#include "ram_buffer.h"
#include "actor.h"

struct WaveFxWork {
  u16 lines[2][162];
  u16 phase;
  u8 page;
  s8 red;
  s8 green;
  s8 blue;
  u16 hue;
  u16 sourceId;
  u16 targetId;
  u8 delay;
  u8 counter;
};

struct WaveFxSprite {
  u32 unk00[2];
  u8 unk08;
  u32 unk09_0 : 2;
  u32 mode : 2;
  u32 unk09_4 : 4;
  u8 unk0a[8];
  u16 orientation;
  u8 unk14[6];
  u8 palette;
};

struct WaveFxVector {
  s32 x;
  s32 y;
  s32 z;
};

struct WaveFxDescriptor {
  s16 sprite;
};

struct WaveFxMetadata {
  u8 unk00[8];
  s8 boxX;
};

s32 Trig_Sin(s32 angle);
void BattleEffect_AdvanceHueCycle(void);
void BattleEffect_ApplyColorToTargetBuffer(u32 color, s32 mode);
void BattleEffect_StartBufferInterpolation(s32 frames);
struct Actor *Actor_GetById(s32 id);
void BattleEffect_ApplyFlickerValue(struct Actor *object, s32 value);
struct WaveFxDescriptor *Scene_FindActorPlacement(s32 id);
struct WaveFxMetadata *Resource_GetMetadataRecordFarFar(s32 sprite);
struct Actor *Func_080200c0(s32 kind, s32 x, s32 y, s32 z);
s32 ArcTan2(s32 y, s32 x);
void BattleEffect_SetCallbackWhenTargetUnset(struct Actor *object);

// Each frame after the delay: rewrites the 160-line wave into the other
// page, cycles the colour every other frame, and at counts 0, 8 and 16 of
// its 61-frame cycle sends a particle from the target back to the user.
void Func_080dcb44(void) {
  struct WaveFxWork *work = Ram_HeapSlots->unk4c[3];
  struct WaveFxVector to;
  struct WaveFxVector from;
  struct WaveFxDescriptor *descriptor;
  struct WaveFxMetadata *metadata;
  struct WaveFxSprite *sprite;
  struct Actor *source;
  struct Actor *target;
  struct Actor *object;
  u16 *line;
  s32 i;

  if (work->delay != 0) {
    work->delay--;
    return;
  }

  line = work->lines[work->page ^ 1];
  for (i = 0; i < 160; i++)
    *line++ = Trig_Sin(((work->phase + i * 8) << 16) / 160) >> 14;
  work->phase += 4;
  work->page ^= 1;
  if (work->page != 0) {
    BattleEffect_AdvanceHueCycle();
    BattleEffect_ApplyColorToTargetBuffer(
        (work->blue << 10) | (work->green << 5) | work->red | 0x200000, 1);
    BattleEffect_StartBufferInterpolation(1);
  }
  BattleEffect_ApplyFlickerValue(Actor_GetById(work->sourceId), 0);

  if (work->counter == 0 || work->counter == 8 || work->counter == 16) {
    source = Actor_GetById(work->sourceId);
    target = Actor_GetById(work->targetId);
    if (source != NULL && target != NULL) {
      descriptor = Scene_FindActorPlacement(work->sourceId);
      from.x = source->x;
      metadata = Resource_GetMetadataRecordFarFar(descriptor->sprite);
      sprite = target->animation;
      from.y = source->y + (metadata->boxX << 16) - 0x20000;
      from.z = source->z;
      descriptor = Scene_FindActorPlacement(work->targetId);
      to.x = target->x;
      metadata = Resource_GetMetadataRecordFarFar(descriptor->sprite);
      to.y = target->y + (metadata->boxX << 16) - 0x20000;
      to.z = target->z;
      if (sprite->orientation == 0x8000) {
        to.y -= 0x180000;
      } else if (sprite->orientation == 0x4000) {
        to.x += 0x60000;
        to.y -= 0xc0000;
      } else if (sprite->orientation == 0xc000) {
        to.x -= 0x60000;
        to.y -= 0xc0000;
      }
      object = Func_080200c0(677, to.x, to.y, to.z);
      if (object != NULL) {
        sprite = object->animation;
        object->motionFlags = 0;
        object->speedLimit = 0xa3d7;
        object->acceleration = 0xa3d7;
        object->angle = ArcTan2(from.z - to.z, from.x - to.x);
        object->update = (void (*)(void))BattleEffect_SetCallbackWhenTargetUnset;
        sprite->palette = 0;
        sprite->mode = 1;
        Actor_SetTargetPosition(object, from.x, from.y, from.z);
      }
    }
  }

  if (work->counter != 99) {
    if (work->counter == 0)
      Sound_PlayFar(130);
    if (++work->counter > 60)
      work->counter = 0;
  }
}
