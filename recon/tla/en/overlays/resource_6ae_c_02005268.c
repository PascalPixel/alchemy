// Draft, differ 66 of 174, skeleton gap 24 (tla-en, 2026-11-01)
// The update hook Func_020053e4 installs: animates the object every other
// frame, starts one of two rising sprite pairs every 16 frames (sound 0x256),
// and in mode 1 frees itself once both have passed 31.
// Left: allocation only, and everything follows from it. The listing has
// object r7, the unka0 cursor r6, sprite r5, work r8, j r10, z r9, i r11, with
// x, sine and the i * 80 giv on the stack (sp+8, sp+4, sp+0). This draft has
// sine r8, j r10, z r9, x r11, and work and i on the stack.
// Priorities here (.lreg, floor_log2(refs) * refs / live): sine .51, j .48,
// z .35, x .33, work .23, i .12. No plain scalar order gives the listing, which
// needs work above j and z and i above x and sine. So two separate questions:
// 1. Why x and sine lose. Best lead: struct { s32 sine; s32 x; } v, a DImode
// pseudo. Partial sets keep it live for 118 insns, and Thumb allows DImode
// only below r7 (HARD_REGNO_MODE_OK), so it goes to the stack once the cursor
// holds r6. That needs it after the cursor (.737), so refs 14 or fewer; it has
// 17 (each partial set 4, three reads 3 each), and takes r6:r7 (differ 171).
// Dropping one sine read, as a diagnostic with unk18 stored 0, gives every
// slot, r5 to r7, i r11 and a skeleton gap of 8. No C form found drops that
// read: first-pass CSE puts the subreg back through a temporary or
// sprite->unk18 = sprite->unk14. The pair's meaning is also unexplained.
// 2. Why work beats j and z. Even with the pair out, the order is j r8, z r10,
// work r9. work is 9 refs over about 118 insns, and every ref is in the final
// code; j cannot fall below .48 (see next note). This one has no lead.
// Forms that earned their keep: j = 0 before the Func_0200579c call. j has 14
// refs, so crossing only 3 calls makes it caller-save profitable and it takes
// r4 (differ 133); crossing 4 it does not, and sched2 moves the set below the
// bl. z stored before x (72 to 66). Bit view of the counter keeps its second
// ldrb (85 without). One pointer for position and sprite stops loop.c hoisting
// add rN, sp (158 without it); that shared variable is `temporary` under S2 and
// needs its tag if this form is adopted.
// Tried: a 12-byte memory struct for the giv, sine and x (153; hoists a frame
// base, as s32 values[3] did at 104); work->sprites[i * 2 + j] as the inner
// pointer (161) or spelled at each use (207); volatile x and sine (152); height
// as a pointer biv with a goto outer loop; declaration orders and inner scopes;
// do-while outer loop; j = 0 at the top of the outer loop (78) or after the
// position stores (137).
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
        /* FAKEMATCH(barrier): the empty asm on sprite changes its allocation rank; as no
         * asm, 39 of 174 instructions differ in tla-en. */
        asm("" : : "r"(sprite));
        for (; j < 2; j++) {
          /* FAKEMATCH(one-pass): the statement in a once-only block, whose end holds the
           * order around it; as the plain statement, 41 of 174 instructions differ
           * in tla-en. */
          do {
            sprite->unk10 = z;
          } while (0);
          sprite->unk0c = x;
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
