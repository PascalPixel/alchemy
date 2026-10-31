// Near miss, differ 8 of 197 (tla-en, 2026-10-31): two reordered pairs, both
// where -mthumb-slow-loads only approximates the ROM's wait after a store.
// Shared by thirteen overlays (698 to 6a4): the update of an object that
// carries the leader. While no event runs it animates, plays a sound every 32
// frames, takes the leader's sprite priority and moves along its angle; when
// the leader stands close enough (and flag 260 is clear) it links the leader
// to itself. Off its track it stops, takes its rest script and clears the
// ride.
// Settled 2026-10-31 (16 to 8): the multiply is called through IwramIrqMain
// plus its offset, the route the Broken Seal's item trade takes to its word
// copy: a linked address is a pool word that waits for the cosine call, so
// the speed loads first as in the native (at the fixed Iwram_MulQ16 the
// constant is built first: 10). The 200 is stored through EventWork's
// unk16a, which builds it with movs (as *(u16 *) it loads from the pool: 34;
// a local s32 also matched, a temporary). The stop path clears the byte
// before the update from one word zero (tagged below).
// Left: (1) the cell lookup's 2 after its x and z loads. The slow-loads rule
// delays a load only after a store it may read, and FieldSprite, which
// declares no s32, cannot alias Actor.x, so x and z go first. With bytes
// 0x0c to 0x0f declared as the s32 16.16 scale the projection draft reads
// there, the same C puts the 2 first (a stand-in type in a scratch copy:
// differ 4); that is field_sprite.h's to decide. (2) the leader's link: the
// native stores watchedObject and the angle before r1 = r7 (the flag's
// address); sched2 ranks that copy 16 and the stores 15, the flag load being
// exempt as a read-modify-write, and the watchedObject store depends on the
// angle load (the halfword reads Actor's alias set): no C order moves it.
// Tried: every order of the three leader stores (12, 34, 76, 86 before the
// type note), the angle through an s32 or u16 local (8, 31),
// *(u16 *)&leader->angle (36), the layer through a local and an old-style
// declaration of the lookup (no change), the priority as a byte
// read-modify-write (fixes (1), 33 overall), work->unk15a unsigned (17).
// Permute 15 minutes on the earlier form and 5 minutes (10,484 candidates)
// with the stand-in type found nothing better. An empty asm after the
// priority store gets x before z (14 on the earlier form).
#include "types.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "irq.h"
#include "event_work.h"
#include "actor.h"
#include "main_routine_work.h"
#include "field_sprite.h"

extern const s32 gData_0200219c[];
extern s32 gData_020021bc;

struct Actor *Actor_GetById(s32 id);
void Actor_SetAnimationSpeed(struct Actor *object, s32 speed);
void Func_02001b44(s32 sound);
s32 Func_02001b2c(s32 x, s32 z, s32 layer);
void Func_02001b14(s32 *position);
s32 Func_020005b8(s32 *from, s32 *to);
s32 GameFlag_Test(s32 flag);
s32 Math_Cosine(s32 angle);
s32 Math_Sine(s32 angle);
void Func_02001a0c(struct Actor *object, const s32 *script);

void Func_020005e4(struct Actor *self) {
  struct EventWork *work = Ram_HeapSlots->eventWork;
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 position[3];
  s32 distance;
  s32 near;

  if (work->unk166 != 0 || *(s16 *)&work->unk15a != 0 ||
      *(s16 *)&work->unk15c[2] != 0 || *(s16 *)work->unk168 != 0) {
    Actor_SetAnimationSpeed(self, 0);
    return;
  }
  Actor_SetAnimationSpeed(self, 16);
  if ((++*(u16 *)&self->action & 31) == 31)
    Func_02001b44(231);
  ((struct FieldSprite *)self->animation)->priority =
      ((struct FieldSprite *)leader->animation)->priority;
  if (Func_02001b2c(self->x, self->z, 2) != 255) {
    position[0] = self->x;
    position[1] = self->y;
    position[2] = self->z;
    Func_02001b14(position);
    distance = position[0];
    if (distance > 0x1100000)
      goto stop;
    if (distance < -0x200000)
      goto stop;
    {
      if (*((u8 *)self + 0x62) == 0) {
        distance = leader->y - self->y;
        if (distance < 0)
          distance = self->y - leader->y;
        near = 0;
        if (Func_020005b8(&leader->x, &self->x) <= 12 && distance < 0xc0000)
          near = 1;
        if (near && !GameFlag_Test(260)) {
          leader->angle = self->angle;
          leader->watchedObject = self;
          leader->priorityFlags &= ~1;
          work->unk16a = 200;
          gMainRoutineWork.renderMode = 0;
          *((u8 *)self + 0x62) = 1;
          *((u8 *)self + 0x63) = 1;
        }
      }
      // IwramMulQ16 reached from IwramIrqMain rather than at its fixed address:
      // the address is a linked word read after the cosine, as the native
      // reads it after the speed (at Iwram_MulQ16 it is built before).
      self->x += ((s32 (*)(s32, s32))(
          IwramIrqMain + FIXED_OFFSET(0x11c, IwramMulQ16, IwramIrqMain)))(
          Math_Cosine(self->angle), self->speedLimit);
      self->z += ((s32 (*)(s32, s32))(
          IwramIrqMain + FIXED_OFFSET(0x11c, IwramMulQ16, IwramIrqMain)))(
          Math_Sine(self->angle), self->speedLimit);
      return;
    }
  }
stop:
  {
    s32 zero = 0;

    /* FAKEMATCH(temporary): one word zero clears the ride byte, then the
     * update; as self->update = 0; *((u8 *)self + 0x63) = 0;, 12 of 197
     * instructions differ in tla-en (byte first, 23). */
    *((u8 *)self + 0x63) = zero;
    self->update = (void *)zero;
  }
  Func_02001a0c(self, gData_0200219c);
  Func_02001b44(228);
  gData_020021bc = 0;
}
