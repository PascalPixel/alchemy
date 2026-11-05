// Draft, differ 25: overlay 694's copy of the function drafted in
// resource_6ae_c_02005268.c, whose header says what it is and what still differs.
#include "types.h"
#include "call.h"
#include "actor.h"
#include "system/vector3.h"

struct SparkleSprite {
  u8 unk00[12];
  s32 unk0c;
  s32 unk10;
  s32 unk14;
  s32 unk18;
  u8 unk1c[2];
  u16 unk1e;
  u8 unk20[8];
};

struct SparkleWork {
  struct SparkleSprite sprites[4];
  s16 unka0[2];
  s16 vramEntry;
  s16 unka6;
  struct Actor *object;
};

struct CounterBits {
  u8 low : 1;
  u8 rest : 7;
};

void Animation_ApplyChildValues(struct Actor *, u32);
s32 Resource_ResetEntry(u32 entry);
s32 Math_Sine(s32);
void Func_0200421c(struct Vector3 *v);
void Func_0200424c(struct SparkleSprite *sprite);
s32 Func_0200425c(struct Actor *object);
void Func_020042b4(s32 sound);

void Func_02003cd8(struct Actor *object) {
  struct Vector3 position;
  s32 x, z, sine;
  s32 i, j;
  struct SparkleWork *work = (struct SparkleWork *)object->watchedObject;
  u8 *count = (u8 *)object + 99;
  struct SparkleSprite *sprite;

  if (*count & 1)
    Animation_ApplyChildValues(
        object, (u8)((u32)((struct CounterBits *)count)->rest % 6));
  if (((u8 *)object)[98] == 0) {
    if ((*count & 15) == 0) {
      Func_020042b4(0x256);
      work->unka0[work->unka6] = 0;
      work->unka6 ^= 1;
    }
  } else if (((u8 *)object)[98] == 1) {
    if (work->unka0[0] > 31 && work->unka0[1] > 31) {
      Animation_ApplyChildValues(object, 0);
      Resource_ResetEntry(work->vramEntry);
      object->update = 0;
      return;
    }
  }
  (*count)++;
  for (i = 0; i < 2; i++) {
    sine = Math_Sine(work->unka0[i] << 10);
    if (work->unka0[i] >= 0) {
      if (work->unka0[i] <= 31) {
        j = 0;
        sprite = (struct SparkleSprite *)&position;
        ((struct Vector3 *)sprite)->x = object->x;
        ((struct Vector3 *)sprite)->y = object->y + (work->unka0[i] << 16);
        ((struct Vector3 *)sprite)->z = object->z;
        Func_0200421c((struct Vector3 *)sprite);
        x = ((struct Vector3 *)sprite)->x;
        z = ((struct Vector3 *)sprite)->z;
        sprite = &work->sprites[i * 2];
        /* FAKEMATCH(barrier): the empty asm on sprite changes its allocation rank; as no
         * asm, 39 of 174 instructions differ in tla-en. */
        asm("" : : "r"(sprite));
        for (; j < 2; j++) {
          /* FAKEMATCH(one-pass): the statement in a once-only block, whose end holds the
           * order around it; as the plain statement, 41 of 174 instructions differ
           * in tla-en. */
          {
            sprite->unk10 = z;
          }
          sprite->unk0c = x;
          sprite->unk14 = sprite->unk18 = sine;
          if (j == 0) {
            sprite->unk1e = Func_0200425c(object) - 1;
          } else {
            sprite->unk1e = Func_0200425c(object) + 1;
            sprite->unk10 -= 0x10000;
            sprite->unk18 = -sprite->unk18;
          }
          Func_0200424c(sprite);
          sprite++;
        }
        work->unka0[i]++;
      }
    }
  }
}
