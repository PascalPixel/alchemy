// Draft, differ 104 of 174, skeleton gap 26 (tla-en, 2026-11-01)
// Each frame: on odd counts recolours the object, every 16 counts in mode 0
// restarts one of the two columns, in mode 1 frees the VRAM entry and stops
// once both columns pass 31; otherwise moves each live column's two sprites.
// Left: allocation. The listing gives object r7, work r8, the loop pointer r6,
// j r10, the second coordinate r9, i r11, and keeps the offset, the sine and
// the first coordinate on the stack; here work and i are on the stack and the
// sine and first coordinate in high registers (lreg: sine 8 refs over 46 insns
// ranks above work, 11 over 115). The listing's reloads of the counter after
// its & 1 test and of *slot after the range test show its pseudos lost their
// registers and came back from their REG_EQUIV memory, so they should follow
// the allocation; plain s16 and u8 locals did not change them.
// A volatile counter and column read makes it worse (differ 119, gap 38).
// The outer loop is a label and goto because a do or for loop hoists
// p = &position out of the loop (differ 182); p must stay in r5 in the block.
// slots and j = 0 before the position's y store are kept only because they
// rank work and j closer: without slots, differ 136 and gap 28, though the
// listing's strh matches that plain form.
#include "types.h"

struct SparkleSprite {
  u8 unk00[12];
  s32 unk0c;
  s32 unk10;
  s32 unk14;
  s32 unk18;
  u16 unk1c;
  u16 unk1e;
  u8 unk20[8];
};

struct SparkleWork {
  struct SparkleSprite sprites[4];
  s16 unkA0[2];
  s16 vramEntry;
  s16 unkA6;
};

struct SparkleObject {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[0x4e];
  u8 unk62;
  u8 unk63;
  u8 unk64[4];
  struct SparkleWork *work;
  void *update;
};

struct Vec3 {
  s32 x;
  s32 y;
  s32 z;
};

void Animation_ApplyChildValues(struct SparkleObject *object, u32 value);
s32 Math_Sine(s32 angle);
s32 Resource_ResetEntry(u32 entry);
void Func_02000ec4(struct Vec3 *position);
void Func_02000eec(struct SparkleSprite *sprite);
s32 Func_02000efc(struct SparkleObject *object);
void Func_02000f3c(s32 sound);

void Func_02000ad0(struct SparkleObject *object) {
  struct SparkleWork *work = object->work;
  struct Vec3 position;
  struct Vec3 *p;
  s32 sine, x, y;
  s32 i, j, offset;
  s16 *slot;
  s16 *slots;
  struct SparkleSprite *sprite;

  if (object->unk63 & 1)
    Animation_ApplyChildValues(object, (object->unk63 >> 1) % 6);
  if (object->unk62 == 0) {
    if ((object->unk63 & 15) == 0) {
      Func_02000f3c(0x256);
      slots = work->unkA0;
      slots[work->unkA6] = 0;
      work->unkA6 ^= 1;
    }
  } else if (object->unk62 == 1 && work->unkA0[0] > 31 &&
             work->unkA0[1] > 31) {
    Animation_ApplyChildValues(object, 0);
    Resource_ResetEntry(work->vramEntry);
    object->update = 0;
    return;
  }
  object->unk63++;
  offset = 0;
  i = 1;
  slot = work->unkA0;
OUTER:
  sine = Math_Sine(*slot << 10);
  if (*slot >= 0) {
    if (*slot <= 31) {
      p = &position;
      p->x = object->x;
      j = 0;
      p->y = object->y + (*slot << 16);
      p->z = object->z;
      Func_02000ec4(p);
      x = p->x;
      y = p->z;
      sprite = (struct SparkleSprite *)((u8 *)work + offset);
      do {
        sprite->unk10 = y;
        sprite->unk0c = x;
        sprite->unk14 = sine;
        sprite->unk18 = sine;
        if (j == 0) {
          sprite->unk1e = Func_02000efc(object) - 1;
        } else {
          s32 v = Func_02000efc(object);
          sprite->unk10 -= 0x10000;
          sprite->unk1e = v + 1;
          sprite->unk18 = -sprite->unk18;
        }
        Func_02000eec(sprite);
        j++;
        sprite++;
      } while (j <= 1);
      (*slot)++;
    }
  }
  offset += 80;
  i--;
  slot++;
  if (i >= 0)
    goto OUTER;
}
