// Under -mthumb-slow-loads (2026-10-26): differ 4 of 68 in tla-en, instruction
// order only. No empty asm before any one statement lowers it.
// Near miss: differ 6 of 68 (three reordered rows; differ 8 before). The empty
// asm on the two addresses and the one-pass block on the source fix the table
// load and the position add. Left: the packed-field store masks the matrix
// index (movs r3, #31) before it reads the byte at 23, where the native reads
// first, as in Animation_SetStateFlags (the bitfield store emits the mask of
// the value before the read; an explicit byte expression uses another mask, 193
// for -63); and with the one-pass block in the same basic block two more pairs
// move (ands r3, r4 / lsrs r1; lsls r0 / ands r3, r2).
#include "types.h"
#include "iwram_call.h"
#include "scene.h"
void UiWindow_SetTilemapEntry(s32, s32, s32, s32, s32);

struct Effect {
  unsigned x : 16;
  unsigned y : 16;
  unsigned z : 16;
  unsigned unk : 16;
};

struct Object {
  u8 filler0[6];
  u16 src6;
  u8 src8;
  u8 filler9[6];
  u8 out15;
  u8 filler16[4];
  u8 out20;
  u8 mode21 : 2;
  u8 rest21 : 6;
  u16 pos22 : 9;
  u16 affine22 : 5;
  u16 rest22 : 2;
};

extern u32 gVBlankCount;
extern s32 gUi_ObjectPulseScales[];
s32 AffineMatrix_BuildForEffect(struct Effect *effect);

void Ui_ApplyTableScaleToObject(struct Object *obj) {
  s32 v;
  u32 *vblankCount = &gVBlankCount;
  u32 source;
  struct Effect effect;

  v = (s32)gUi_ObjectPulseScales;
  /* FAKEMATCH: the empty asm holds both addresses: the native loads the
   * count's address and the table before it reads the count and builds the mask
   * 7 (two reordered rows). */
  __asm__("" : "+l"(vblankCount), "+l"(v));
  v = *(s32 *)(v + (((*vblankCount >> 1) & 7) << 2));

  if (v < 0)
    v += 255;
  v >>= 8;

  if (obj != 0) {
    effect.x = v;
    effect.y = v;
    effect.z = 0;
    obj->affine22 = AffineMatrix_BuildForEffect(&effect);
    obj->mode21 = 3;
    /* FAKEMATCH: the one-pass block ends the scheduling block: the source is
     * read before 0xfff0 is built (one reordered row). */
    do {
      source = obj->src6;
    } while (0);
    obj->pos22 = source + 0xfff0;
    obj->out20 = obj->src8 + 0xf0;
    obj->out15 = 0xfc;
  }
}
