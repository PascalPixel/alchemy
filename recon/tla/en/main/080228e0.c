// Draft, differ 2 of 68 in tla-en under -mthumb-slow-loads (2026-10-26). No
// single empty asm, one-pass block, wrapping block or swap of neighbouring
// statements brings it closer, nor any pair of empty asm statements or
// one-pass blocks (device search).
// Near miss, differ 2 of 68, one reordered pair in the loop's setup: the native
// stores the slot pointer to its stack home before it copies the zero index to
// r8 (str r2, [sp, #4]; mov r8, r1), this form copies first. The two tagged
// empty asm statements keep the frames store ahead of the animation store;
// without them the frames are stored two rows later and that pair differs as
// well (differ 4).
#include "types.h"
#include "animspr.h"
#include "metadata_lookup.h"

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
  struct AnimationEntry *entries[4];
};
s32 Animation_LookupValueByKey(s32 key);
s32 InitializeAnimationObjects(struct MetadataSlotState *state) {
  s32 index;

  for (index = 0; index < state->count; index++) {
    struct AnimationEntry *object = state->entries[index];
    struct AnimationMetadata *metadata =
        Resource_GetMetadataRecordFar(object->animId);
    s32 frames;
    s32 animation;

    if (metadata->width == 0)
      continue;

    if (index == 0) {
      state->width = metadata->width;
      state->height = metadata->height;
      state->scale = metadata->scale << 8;
      state->offsetY = metadata->adjustY;
      state->offsetX = metadata->adjustX;
    }

    frames = metadata->frames;
    if (frames == 0)
      frames = Animation_LookupValueByKey(object->animId);

    object->kind = metadata->drawKind;
    animation = metadata->animation;
    asm volatile(""); /* FAKEMATCH: first fence of the pair below */
    *(s32 *)&object->frames = frames;
    asm volatile(""); /* FAKEMATCH: the listing stores the frames before the
                         animation; unfenced, the frames are stored two rows
                         later (one reordered row) */
    *(s32 *)&object->scripts = animation;
    object->mode = metadata->pictureCodec;
    object->frame = 0xff;
    object->script = 0;
    object->pos = 0;
  }

  return 0;
}
