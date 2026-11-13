/// @file
/// A wave that ripples the screen while its colour cycles, sending particles
/// from the target back to the user.
// Part of f0dcb44.c.

#include "../f0dcb44_internal.h"
#include "event.h"
#include "module/field/event_commands.h"
#include "module/field/f0dcb44.h"
#include "module/system/system.h"

/* VIEW(f0dcb44): routine BattleEffect_ApplyColorToTargetBuffer as void BattleEffect_ApplyColorToTargetBuffer(u32 color, s32 mode); event.h has void BattleEffect_ApplyColorToTargetBuffer(s32 value, s32 mode). */
extern void BattleEffect_ApplyColorToTargetBuffer__f0dcb44(u32 color, s32 mode) __asm__("BattleEffect_ApplyColorToTargetBuffer");
struct Actor *Actor_GetById(u32 objectId);
/* VIEW(f0dcb44): routine Scene_FindActorPlacement as struct WaveFxDescriptor *Scene_FindActorPlacement(s32 id); battle.h has struct ActionDescriptor *Scene_FindActorPlacement(s32 id). */
extern struct WaveFxDescriptor *Scene_FindActorPlacement__f0dcb44(s32 id) __asm__("Scene_FindActorPlacement");
/* VIEW(f0dcb44): routine Resource_GetMetadataRecordFarFar as struct WaveFxMetadata *Resource_GetMetadataRecordFarFar(s32 sprite); sprite_metadata.h has struct AnimationMetadata *Resource_GetMetadataRecordFarFar(u32 number). */
extern struct WaveFxMetadata *Resource_GetMetadataRecordFarFar__f0dcb44(s32 sprite) __asm__("Resource_GetMetadataRecordFarFar");
struct Actor *Func_080200c0(s32 kind, s32 x, s32 y, s32 z);

// Each frame after the delay: rewrites the 160-line wave into the other
// page, cycles the colour every other frame, and at counts 0, 8 and 16 of
// its 61-frame cycle sends a particle from the target back to the user.
void Func_080dcb44(void) {
  struct WaveFxWork *work = Ram_HeapSlots->unk4c[3];
  struct WaveFxVector from;
  struct WaveFxVector to;
  struct WaveFxDescriptor *descriptor;
  s32 spriteId;
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
    BattleEffect_ApplyColorToTargetBuffer__f0dcb44(
        (work->blue << 10) | (work->green << 5) | work->red | 0x200000, 1);
    BattleEffect_StartBufferInterpolation(1);
  }
  BattleEffect_ApplyFlickerValue(Actor_GetById(work->sourceId), 0);

  if (work->counter == 0 || work->counter == 8 || work->counter == 16) {
    source = Actor_GetById(work->sourceId);
    target = Actor_GetById(work->targetId);
    if (source != NULL && target != NULL) {
      descriptor = Scene_FindActorPlacement__f0dcb44(work->sourceId);
      spriteId = descriptor->sprite;
      from.x = source->x;
      metadata = Resource_GetMetadataRecordFarFar__f0dcb44(spriteId);
      from.y = source->y + (metadata->boxX << 16) - 0x20000;
      from.z = source->z;
      sprite = target->sprite;
      descriptor = Scene_FindActorPlacement__f0dcb44(work->targetId);
      spriteId = descriptor->sprite;
      to.x = target->x;
      metadata = Resource_GetMetadataRecordFarFar__f0dcb44(spriteId);
      {
        s32 box = metadata->boxX;
        s32 y = target->y;

        y += box << 16;
        to.y = y - 0x20000;
      }
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
        sprite = object->sprite;
        object->motionFlags = 0;
        object->speedLimit = 0xa3d7;
        object->acceleration = 0xa3d7;
        object->facing = ArcTan2(from.z - to.z, from.x - to.x);
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
