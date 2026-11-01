// Draft, differ 76 of 174, skeleton gap 27 (tla-en, 2026-11-01)
// The update hook Func_020053e4 installs: animates the object every other
// frame, starts one of two rising sprite pairs every 16 frames (sound 0x256),
// and in mode 1 frees itself once both have passed 31.
// Left: allocation only, and everything follows from it. The listing has
// object r7, the unka0 cursor r6, sprite r5, work r8, j r10, z r9, i r11, with
// x, sine and the i * 80 giv on the stack (sp+8, sp+4, sp+0). This draft has
// work and i on the stack instead, sine in r8 and x in r11; the priorities are
// sine .51, x .33, work .23, i .12. Already right: frame size 24, the
// reversed loop, the reduced givs, and r5, r6, r7, r9, r10.
// What the listing needs: x and sine must lose to work and i. z and x read
// alike yet z keeps a register, so x and sine are not plain equals of z.
// Tried: x and sine as struct { s32, s32 } (a DImode pseudo; right slots, but
// 17 refs take r6:r7 ahead of the cursor; refs under 16 would flip it, and a
// single sine read or a copied store did not lower them); s32 values[2]
// (promoted to DImode too); s32 values[3] holding the giv, with for and goto
// inner loops (104 at best, hoists a frame base); an aggregate holding sine, x
// and position (185); volatile x and sine (152); height as a pointer biv and
// a goto outer loop (PRE of +160 into the arms); declaration orders and inner
// scopes (no change); j = 0 at the top of the outer loop (78).
// Forms that earned their keep: bit view of the counter keeps its second ldrb
// (CSE otherwise merges it); j = 0 above the pointer set lowers j below object
// (135 to 76), though the listing sets j after the call, so this may stand in
// for the real cause of the same mismatch; one pointer for position and sprite
// stops loop.c hoisting add rN, sp (158 without it). That shared variable is
// `temporary` under S2 and needs its tag if this form is adopted.
#include "types.h"
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
void Func_0200579c(struct Vector3 *v);
void Func_020057ac(struct SparkleSprite *sprite);
s32 Func_020057bc(struct Actor *object);
void Func_020057dc(s32 sound);

void Func_02005268(struct Actor *object) {
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
      Func_020057dc(0x256);
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
        Func_0200579c((struct Vector3 *)sprite);
        x = ((struct Vector3 *)sprite)->x;
        z = ((struct Vector3 *)sprite)->z;
        sprite = &work->sprites[i * 2];
        for (; j < 2; j++) {
          sprite->unk0c = x;
          sprite->unk10 = z;
          sprite->unk14 = sprite->unk18 = sine;
          if (j == 0) {
            sprite->unk1e = Func_020057bc(object) - 1;
          } else {
            sprite->unk1e = Func_020057bc(object) + 1;
            sprite->unk10 -= 0x10000;
            sprite->unk18 = -sprite->unk18;
          }
          Func_020057ac(sprite);
          sprite++;
        }
        work->unka0[i]++;
      }
    }
  }
}
