#include "types.h"
#include "event_inline.h"

struct Sprite020003e8 {
  u8 unk00[18];
  s16 rotation;
  u8 unk14[6];
  u8 nextFrame;
};

struct Actor020003e8 {
  u8 unk00[0x50];
  struct Sprite020003e8 *sprite;
  u8 unk54;
  u8 motionFlags;
  u8 unk56[8];
  s16 unk5e;
};

extern const u8 gData_02003720[];

void Func_02003100(struct Actor020003e8 *object, s32 mode);
void Func_02003108(struct Actor020003e8 *object, const void *script);

void Func_020003e8(s32 x, s32 y, s32 z, s16 value, s32 fifth) {
  struct Actor020003e8 *object;
  struct Sprite020003e8 *sprite;

  object = ((struct Actor020003e8 *)Import_ActorCreate(0x1e8, x, y, z));
  if (object != 0) {
    sprite = object->sprite;
    Func_02003100(object, 1);
    Func_02003108(object, gData_02003720);
    object->motionFlags = 0;
    /* FAKEMATCH(barrier): an empty asm, so the fifth argument is read from the
     * stack before the zero for nextFrame, as the native does; as no asm, 2 of
     * 32 instructions differ in tla-en. */
    __asm__("");
    object->unk5e = fifth;
    sprite->nextFrame = 0;
    sprite->rotation = value;
  }
}
