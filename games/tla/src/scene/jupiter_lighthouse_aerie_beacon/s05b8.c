#include "types.h"
#include "edition.h"
#include "field_sprite.h"
#include "map.h"
#include "actor.h"
#include "ram_buffer.h"

/// The halves of a 16.16 coordinate. Read through an s16 pointer instead, the
/// high half's offset folds into the coordinate's (differ 11).
struct FixedHalves {
  u16 fraction;
  s16 whole;
};

/// What a field sprite's record points to.
struct SpriteRecord {
  u8 unk00[5];
  u8 unk05; ///< XXX: set to 1 here; the resource metadata code would say why.
};

extern u32 gFrameCount;

struct Actor *Actor_GetById(s32 id);
s32 Random16Far(void);
void Func_02005884(s32 id, s32 x, s32 z);

/// While the view's z is at most 177 pixels, sets object 36 at y 0x6f8000 and
/// full size on odd frames, or at y 0x688000 and 1.3 times the size on even
/// ones; beyond it, calls Func_02005884(36, 0, 0).
void Func_020005b8(void) {
  struct Actor *object = Actor_GetById(36);
  struct MapWork *map = Ram_HeapSlots->mapWork;

  Random16Far();
#if LANGUAGE != LANGUAGE_JAPANESE
  // Japanese goes on when object 36 is missing.
  if (object == NULL)
    return;
#endif
  if (((struct FixedHalves *)&map->viewZ)->whole <= 177) {
    ((struct SpriteRecord *)((struct FieldSprite *)Actor_GetById(36)
                                 ->animation)
         ->record)
        ->unk05 = 1;
    if (gFrameCount & 1) {
      Func_02005884(36, 0x2000000, 0x1080000);
      Actor_GetById(36)->y = 0x6f8000;
      Actor_GetById(36)->scaleX = 0x10000;
      Actor_GetById(36)->scaleY = 0x10000;
    } else {
      Func_02005884(36, 0x2000000, 0x1080000);
      Actor_GetById(36)->y = 0x688000;
      Actor_GetById(36)->scaleX = 0x14ccc;
      Actor_GetById(36)->scaleY = 0x14ccc;
    }
  } else {
    Func_02005884(36, 0, 0);
  }
}
