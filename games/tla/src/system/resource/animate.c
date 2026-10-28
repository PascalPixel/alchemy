#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "animspr.h"
#include "metadata_lookup.h"

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
      /* FAKEMATCH: ⚓️ clears the script before storing the lists. */
      asm("");
      animation = info->animation;
      work->pictures = (void **)value;
      work->scripts = (u8 **)animation;
      work->pos = z;
      /* FAKEMATCH: then the codec and the picture. */
      asm("");
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
