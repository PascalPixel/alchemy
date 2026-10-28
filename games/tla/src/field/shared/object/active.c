#include "types.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "object_runtime.h"
#include "field_sprite.h"
#include "dma.h"
#include "mem.h"
#include "resource.h"
#include "edition.h"

struct ObjectRuntime *Object_GetById(u32);
void Func_08038248(s32 resource, u8 *buffer);
void ResourceMetadata_ClearRecordFar(void *record);

/// Gives an actor's sprite a still picture: resource, drawn by Func_08038248
/// into a cleared 128-byte buffer, or a blank one when bit 15 is set, loaded
/// into the sprite's VRAM block as its tile.
void Func_080d3b28(u32 id, s32 resource) {
  struct ObjectRuntime *object = Object_GetById(id);
  struct FieldSprite *sprite;
  u8 *buffer;
  /* FAKEMATCH(temporary): the zero set between the allocation and its 0x400
   * offset, which builds it in r3 and the offset in r2 as the native does; as
   * plain zeros, 14 of 86 instructions differ in tla-en. */
  s32 zero;
  s32 tile;
  u8 kind;

  if (object != NULL) {
    kind = object->animationKind;
    if (kind == 1) {
      sprite = object->animation;
      if (resource & 0x8000)
        resource = 0;
      buffer = Runtime_AllocateHeapBlock(68, 0x608);
      zero = 0;
      buffer += 0x400;
      Dma_Fill32(zero, buffer, 0x80);
      Func_08038248(resource, buffer);
      tile = VramCache_Load(sprite->vramBlock, 0x80, buffer);
      Runtime_ReleaseHeapBlock(68);
      object->unk5c[0] = kind;
      ResourceMetadata_ClearRecordFar(sprite->record);
      sprite->record = NULL;
      sprite->unk1b[0] = zero;
      sprite->fullColor = zero;
      sprite->tile = tile;
      sprite->unk19 = zero;
      sprite->nextFrame = zero;
    }
  }
}

void EventRuntime_SetMessage(s32 message) {
  struct EventWork *event = Ram_HeapSlots->eventWork;

  event->messageId = message;
}

s32 ObjectTable_ReadActiveValue(s32 key) {
  struct EventWork *event = Ram_HeapSlots->eventWork;
  s32 result = -1;
  struct ObjectRuntime *object = event->objects[key & 0x0fff];

  if (object != NULL && object->animationKind == 1)
    result = *((struct ObjectAnimationState *)object->animation)->value28;
  return result;
}

s32 ObjectTable_FindActiveByValue(s32 value) {
  struct EventWork *event = Ram_HeapSlots->eventWork;
  s32 result = -1;
  s32 index = 8;
  struct ObjectRuntime *object = event->objects[index];

  if (object != NULL && object->animationKind == 1 &&
      *((struct ObjectAnimationState *)object->animation)->value28 == value) {
    result = index;
  } else {
  next:
    index++;
    if (index <= 79) {
      object = event->objects[index];
      if (object == NULL || object->animationKind != 1 ||
          *((struct ObjectAnimationState *)object->animation)->value28 != value)
        goto next;
      result = index;
    }
  }
  return result;
}
