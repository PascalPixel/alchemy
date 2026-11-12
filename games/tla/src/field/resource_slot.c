#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "resource.h"
#include "sprite.h"
#include "sprite_metadata.h"
#include "ram_buffer.h"

enum { RESOURCE_SLOT_COUNT = 8 };

struct ResourceSlotNumber {
  u16 resource;
  s16 number;
};

extern struct ResourceSlotNumber gResourceSlot_NumberTable[];

s32 ResourceSlot_Load(u32 slot, u32 *buffer, s32 number, u32 variant) {
  struct AnimationMetadata *metadata;
  struct ObjectSystemWork *work;
  struct ResourceSlot *record;
  struct ResourceSlotNumber *entry;
  u32 *offset;
  u8 *text;
  u8 *end;
  u8 *table;
  u32 size;
  u32 count;
  u32 index;
  u16 entryNumber;
  s32 resource;

  if (slot >= RESOURCE_SLOT_COUNT)
    return 0;
  work = Ram_HeapSlots->objectSystem;
  record = &work->slots[slot];
  metadata = Resource_GetMetadataRecordFar(number);
  record->tag = (slot << 12) | number;
  record->buffer = buffer;

  entry = gResourceSlot_NumberTable;
  for (count = 0; count < 256; count++) {
    s16 *numbers = &entry->number;

    resource = entry->resource;
    entryNumber = *numbers;
    entry++;
    if (entryNumber == 0)
      return 0;
    if (entryNumber == number)
      break;
  }
  size = Resource_DecodeType01(Resource_GetTableEntry(resource), buffer);

  offset = buffer;
  for (count = 0; count < 256; count++) {
    if (*offset == 0)
      break;
    *offset += (u32)buffer;
    offset++;
  }
  if (variant != 0) {
    index = variant - 1;
    text = (u8 *)(offset + 1);
    end = (u8 *)buffer + size;
    if (index > 4)
      index = 0;
    table = gResourceSlot_ConversionTables[index];
    for (; text < end; text++) {
      u32 c = *text;

      if (c <= 0xdf) {
        c = table[c];
        *text = c;
      }
    }
  }
  return metadata->width * metadata->height;
}

s32 Animation_LookupValueByKey(s32 key) {
  struct ObjectSystemWork *work = Ram_HeapSlots->objectSystem;
  u32 no;
  struct ResourceSlot *p;

  no = 0;
  for (p = work->slots; no < RESOURCE_SLOT_COUNT; no++, p++) {
    if (p->tag == key)
      return (s32)p->buffer;
  }
  return 0;
}

// actor_view.h's ActorSpriteView, with the bytes this function writes named
// from the projection draft's layout (field_sprite.h); its other members as
// the tree has them.
struct ActorSpriteView {
  u8 unk00[5];
  u8 unk05Low : 2;
  u8 stateFlags : 2;
  u8 unk05High : 4;
  u8 unk06[6];
  s32 scale;
  u8 unk10;
  u8 unk11Low : 1;
  u8 displayFlag : 1;
  u8 unk11High : 6;
  s16 value12;
  u8 width;
  u8 height;
  s8 offsetX;
  s8 offsetY;
  u8 unk18;
  u8 dirty;
  u8 value1a;
  u8 count;
  u8 unk1c[0x0c];
  struct AnimationEntry *entries[4];
};
s32 InitializeAnimationObjects(struct ActorSpriteView *state) {
  s32 index;
  struct AnimationEntry **slot;

  index = 0;
  if (index < state->count) {
    slot = state->entries;
    do {
      struct AnimationEntry *object = *slot++;
      struct AnimationMetadata *metadata =
          Resource_GetMetadataRecordFar(object->animId);
      void **pictures;
      u8 **animation;

      if (metadata->width == 0)
        continue;

      if (index == 0) {
        state->width = metadata->width;
        state->height = metadata->height;
        state->scale = metadata->scale << 8;
        state->offsetY = metadata->adjustY;
        state->offsetX = metadata->adjustX;
      }

      pictures = metadata->pictures;
      if (pictures == 0)
        pictures = (void **)Animation_LookupValueByKey(object->animId);

      object->kind = metadata->drawKind;
      animation = metadata->animation;
      // UB: the pictures are stored through a u8 ** view of the field, the
      // scripts' type, which keeps the store after the scripts' load; stored
      // as void **, it passes the load and the drawing kind's store.
#ifdef UBFIX
      object->pictures = pictures;
#else
      *(u8 ***)&object->pictures = (u8 **)pictures;
#endif
      object->scripts = animation;
      object->mode = metadata->pictureCodec;
      object->picture = 0xff;
      object->script = 0;
      object->pos = 0;
    } while (++index < state->count);
  }

  return 0;
}

#include "debug.h"

void Animation_InitializeWorkFromMetadata(struct AnimationEntry *work) {
  s32 value;
  s32 z;
  s32 animation;
  struct AnimationMetadata *info;

  if (work != NULL) {
    info = Resource_GetMetadataRecordFar(work->animId);
    if (info->width != 0) {
      value = info->pictures;
      if (value == 0)
        value = Animation_LookupValueByKey(work->animId);
      z = 0;
      work->kind = info->drawKind;
      work->script = (u8 *)z;
      /* FAKEMATCH(dead): an empty once-only block keeps the kind's store and
       * the script's clear ahead of the lists' stores; without it, 6 of 30
       * instructions differ in tla-en. */
      do {
      } while (0);
      animation = info->animation;
      work->pictures = (void **)value;
      work->scripts = (u8 **)animation;
      work->pos = z;
      work->mode = info->pictureCodec;
      work->picture = 0xff;
    }
  }
}

struct MetadataSlotState {
  u8 unk00[0x0c];
  s32 scale;
  u8 unk10[4];
  u8 width;
  u8 height;
  s8 offsetX;
  s8 offsetY;
  u8 unk18[3];
  u8 count;
  u8 unk1c[0x0c];
  s32 slots[4];
};

s32 ResourceMetadata_Register(struct MetadataSlotState *state, s32 id) {
  s32 value = state->slots[0];
  s32 index = 0;
  s32 *slot;
  struct AnimationMetadata *metadata;

  if (value != 0) {
    slot = &state->slots[0];
    do {
      index++;
      if (index > 3)
        break;
      slot++;
      value = *slot;
    } while (value != 0);
  }
  if (index == 4)
    return -1;
  value = AnimationObject_Allocate(id);
  if (value == 0)
    return 0;
  state->slots[index] = value;
  metadata = Resource_GetMetadataRecordFar(id);
  if (state->count == 0) {
    state->width = metadata->width;
    state->height = metadata->height;
    state->scale = metadata->scale << 8;
    state->offsetY = metadata->adjustY;
    state->offsetX = metadata->adjustX;
  }
  if (index == state->count)
    state->count = index + 1;
  return value;
}

void ResourceMetadata_Unregister(struct MetadataSlotState *state, s32 handle) {
  s32 count;
  u32 slot;
  u32 i;

  if (state != NULL && handle != 0) {
    ResourceMetadata_ClearRecord((void *)handle);
    for (slot = 0; slot <= 3; slot++) {
      if (handle == state->slots[slot])
        break;
    }
    if (slot != 4) {
      state->slots[slot] = 0;
      count = 0;
      for (i = slot + 1; i <= 3; i++) {
        if (state->slots[i] != 0)
          count++;
      }
      if (count == 0)
        state->count = slot;
    }
  }
}

void ResourceMetadata_ReleaseSlot(struct MetadataSlotState *state, u32 no) {
  s32 slot;
  s32 count;
  u32 i;

  if (state != NULL && no <= 3) {
    slot = state->slots[no];
    if (slot != 0) {
      ResourceMetadata_ClearRecord((void *)slot);
      state->slots[no] = 0;
      count = 0;
      for (i = no + 1; i <= 3; i++) {
        if (state->slots[i] != 0)
          count++;
      }
      if (count == 0)
        state->count = no;
    }
  }
}

void Animation_SetWorkEntry(struct AnimationEntry *work, s32 no) {
  s32 hi;
  struct AnimationMetadata *info;
  s32 value;

  hi = 0x80 & no;
  if (work->scripts != 0) {
    info = Resource_GetMetadataRecordFar(work->animId);
    if (no < info->animationCount) {
      value = (s32)work->scripts[no];
      work->kind = info->drawKind;
      work->script = (u8 *)value;
      work->step = 0x10;
      if (hi == 0) {
        work->pos = hi;
        work->timer = hi;
      }
    }
  }
}

struct AnimationGroup {
  u8 unk00[0x16];
  s8 offsetX;
  s8 offsetY;
  u8 lastNo;
  u8 unk19[2];
  u8 count;
  u8 unk1c[0x0c];
  struct AnimationEntry *entries[4];
};

s32 Animation_ApplyChildArgument(struct AnimationGroup *state, s32 flags) {
  s32 highBit;
  s32 index;

  highBit = flags & 0x80;
  flags &= 0x7f;

  if (state->lastNo != flags) {
    index = 0;
    goto test_entry;
  entry_loop:
    {
      struct AnimationEntry *entry = state->entries[index];
      struct AnimationMetadata *metadata;
      s32 selectedAnimation;

      if (entry == 0)
        goto next_entry;
      if (entry->scripts == 0)
        goto next_entry;

      metadata = Resource_GetMetadataRecordFar(entry->animId);
      if (flags >= metadata->animationCount)
        goto next_entry;

      selectedAnimation = (s32)entry->scripts[flags];
      entry->kind = metadata->drawKind;
      entry->script = (u8 *)selectedAnimation;
      entry->step = 0x10;
      if (highBit == 0) {
        entry->pos = 0;
        entry->timer = 0;
      }
      if (index == 0) {
        state->offsetY = metadata->adjustY;
        state->offsetX = metadata->adjustX;
      }
      goto next_entry;
    }
  next_entry:
    index++;
  test_entry:
    if (index < state->count)
      goto entry_loop;
    state->lastNo = flags;
  }
  return 0;
}

struct ChildObject {
  u8 unk00[2];
  u16 value;
  u8 unk04[8];
  s32 active;
  u8 unk10[5];
  u8 mode;
};

struct ChildGroup {
  u8 unk00[27];
  u8 count;
  u8 unk1c[12];
  struct ChildObject *children[1];
};

s32 Func_08022b80(struct ChildGroup *group, s32 value) {
  s32 i;

  for (i = 0; i < group->count; i++) {
    struct ChildObject *child = group->children[i];

    if (child != NULL && child->active != 0) {
      child->value = value << 4;
    }
  }
  return 0;
}

void Animation_ApplyChildValue(struct ChildGroup *group, s32 value) {
  s32 i;

  for (i = 0; i < group->count; i++) {
    struct ChildObject *child = group->children[i];

    if (child != NULL && child->active != 0)
      *((u8 *)child + 21) = value;
  }
}
