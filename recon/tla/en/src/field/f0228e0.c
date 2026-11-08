#include "types.h"
#include "sprite.h"
#include "sprite_metadata.h"

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
s32 Animation_LookupValueByKey(s32 key);
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
