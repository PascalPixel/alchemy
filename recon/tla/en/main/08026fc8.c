// Draft, differ 2 of 69 in tla-en under -mthumb-slow-loads (2026-10-26). No
// single empty asm, one-pass block, wrapping block or swap of neighbouring
// statements brings it closer, nor any pair of empty asm statements or
// one-pass blocks (device search).
// Near miss, differ 4 of 69, instruction order only. Creates object kind 14
// above this one, plays cue 623 unless bit 0 of unk66 is set, and gives the
// new sprite this one's layer bits; mode 2 also applies child values 13 and 8.
// Left: the native stores value1a before it copies the parent sprite pointer
// to r2, and loads the child's flags byte right after the parent's (here after
// the first and). A second empty asm after the value1a store fixes the first
// but swaps the next call's two argument moves (differ 4 again). Tried before:
// explicit masks on a u8 or s8 field (differ 14 to 47), u8, s8, u16 or u32
// bitfields (no change), the layer through a local or a volatile pointer (no
// change), the height in a local with an empty asm on the sprite (registers
// change, differ 38), permute 4 minutes from here (no better).
#include "types.h"
#include "object_runtime.h"
#include "object_dispatch.h"

struct ObjectRuntime *Func_08023220(s32 kind, s32 x, s32 y, s32 z);
extern const s32 gData_0802ec88[];

struct Child {
  u8 unk00[9];
  u8 low9 : 2;
  u8 layer : 2;
  u8 high9 : 4;
  u8 unk0a[0x10];
  u8 value1a;
};

void Func_08026fc8(struct ObjectRuntime *object, s32 mode) {
  struct ObjectRuntime *created;
  struct Child *sprite;

  created = Func_08023220(14, object->x, object->y + 0x40000, object->z);
  sprite = object->animation;
  if (!(object->unk66 & 1))
    Audio_PlayCue(623);
  if (created != NULL) {
    struct Child *child = created->animation;

    created->terrainHeight = object->terrainHeight;
    /* FAKEMATCH(barrier): an empty asm after the height's store, so the
     * sprite loads between the height's load and store and the call's
     * arguments follow; as plain statements, 10 of 69 instructions differ in
     * tla-en. */
    asm("");
    ObjectDispatch_Initialize((struct DispatchObject *)created, gData_0802ec88);
    created->motionFlags = 0;
    if (child != NULL) {
      Animation_ApplyChildArgument((struct DispatchChild *)child, 1);
      child->value1a = 0;
      child->layer = sprite->layer;
      if (mode == 2) {
        Animation_ApplyChildValuesToRecord((struct DispatchChild *)child, 13);
        Animation_ApplyChildValue((struct DispatchChild *)child, 8);
      }
    }
  }
}
