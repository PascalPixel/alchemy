#include "../../../../games/tla/src/scene/jupiter_lighthouse_room_m/jupiter_lighthouse_room_m.h"
#include "field_sprite.h"
#include "frame.h"
#include "event_inline.h"


static __inline__ void I_Func_020048d4(s32 actor, s32 a) {
  /* FAKEMATCH(wrapper): forwarding keeps each call's own constants; as
   * direct calls at its one site, 2 of 37 instructions differ in tla-en. */
  Func_080d489c(actor, a);
}

s32 Func_02000a78(void) {
  struct Actor *object = Actor_Get(1);

  if (object != NULL) {
    struct FieldSprite *sprite = object->animation;

    if ((gFrameCount & 127) == 0)
      I_Func_020048d4(29, 258);
    if (sprite != NULL) {
      // As sprite->unk14[n], the frame count's reload passes these stores.
      *(sprite->unk14 + 3) = 30;
      *(sprite->unk14 + 2) = 252;
      sprite->rotation = Math_Sin(gFrameCount * 728) / 128 - 0x800;
    }
  }
  return 1;
}
