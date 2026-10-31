// Near miss, differ 2 of 68 (tla-en, 2026-10-30): one reordered pair in the
// loop's setup. The native stores the slot pointer to its stack home before it
// copies the zero index to r8 (str r2, [sp, #4]; mov r8, r1); this form copies
// first. Both are ready together in sched2 with equal priority, so the tie goes
// to RTL order: loop.c hoists the stores' zero (r8) before strength reduction
// emits the slot pointer, and reload's spill of the pointer follows it. No
// index type (s32, u32, u8, s16), pointer walk, zero variable, NULL, while or
// goto loop form changes that order, nor a four-minute permuter run; the
// earlier device search (any single or pair of empty asm statements or one-pass
// blocks, wrapping blocks, swaps of neighbouring statements) found nothing
// closer either.
// The Broken Seal's twin is Sprite_ResetAnimations
// (games/tbs/src/field/field/sprite_animation.c), which matches in plain C.
// Here the pictures need the u8 ** view below (UBFIX gives the declared
// store); stored plainly, they pass the drawing kind's store (differ 4). It
// replaces the two empty asm statements of the 2026-10-26 draft.
#include "types.h"
#include "sprite.h"
#include "sprite_metadata.h"

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
    // scripts' type, which keeps the store after the scripts' load; stored as
    // void **, it passes the load and the drawing kind's store.
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
  }

  return 0;
}
