// Matches: differ 0 (tla-ja, 2026-11-08); tla-en, tla-de, tla-es, tla-fr and
// tla-it link the patch-1 form (236 instructions), against which this is 208.
// Literal pool words checked in order.
// The Japanese release's own Func_0200263c of overlay 6aa: the patch-1 C in
// games/tla/src/scene/mars_lighthouse_entrance/s263c.c without its overlap
// scan, as overlay 6ab's Japanese draft of Func_02002148. Adopting it means a
// non-patch-1 form in s263c.c, linked by resource_6aa.ld for Japanese only.
// What closed it (8 November): Func_02004c38's far routine Func_08020210
// takes (object, position), so the call passes &position (r1 = sp at both
// calls); position is a plain struct Vector3 again, its union replaced by
// once-only blocks around position.z (and, in the step branch, the call with
// it, or the call's &position is rebuilt: one more mov r1, sp); actor->z is
// loaded into off inside a once-only block before z's sum, so the loop notes
// hold the load between x's store and the work read with no filler between;
// the function returns s32 (tagged), whose epilogue use of r0 ranks work's
// copy ahead of the final zero. Four tagged devices remain: off's copy of
// actor->z (temporary), Func_020024c0 called as returning s32 (signature),
// the velocityX union (the work's s16 read must wait on that s32 store with
// i's init filling the gap, which only an alias does; a barrier would hold
// i's init back too, volatile casts make the read ldrh, 21) and the s32
// return (asm operands after or before the last store: 2 or 17). The work
// at gData_020023c4 + 0x188 is ride.h's RideWork (unk04 is its active).
// Earlier rounds, for the record: z = actor->z += (6), z's sum swapped (8),
// the last store in a block, through a zero local (6) or a u16 pointer (20),
// z read before x's sum (24), 14 minutes of permute.
#include "edition.h"
#include "types.h"
#include "fixed_math.h"
#include "actor.h"
#include "main_routine_work.h"
#include "vector3.h"

struct Work020023c4 {
  u8 unk00[4];
  u16 unk04;
  u8 unk06[0x68 - 6];
  s16 unk68;
  s16 unk6a;
};

void Func_020024c0();
struct Actor *Actor_GetById(s32 id);
void Func_02004ca8(void);
void Func_02004da0(s32 a);
void Func_02004bc0(struct Actor *actor, s32 a);
s32 Func_02004c18(s32 terrain, s32 x, s32 z);
s32 Func_02004c38(struct Actor *actor, struct Vector3 *position);
void Func_02004ce8();
void wait(s32 frames);
void Animation_ApplyChildValues(struct Actor *actor, s32 a);
void Func_02002614(void);
extern u8 gData_020023c4[];

s32 Func_0200263c(void) {
  struct MainRoutineWork *main = &gMainRoutineWork;
  struct Work020023c4 *work;
  struct Vector3 position;
  /* FAKEMATCH(signature): the function returns s32 and returns nothing, so
   * the epilogue's use of r0 ranks work's copy ahead of the last store's
   * zero; as void, 2 of 186 instructions differ in tla-ja. */

#ifdef UBFIX
  work = (struct Work020023c4 *)(gData_020023c4 + 0x188);
#endif
  if (main->renderMode == 0) {
    struct Actor *actor;

    /* FAKEMATCH(signature): Func_020024c0 called as returning s32; as the
     * void call, 2 of 186 instructions differ in tla-ja. */
    ((s32 (*)())Func_020024c0)(-10, 0);
    actor = Actor_GetById(main->currentOwner);
    work = (struct Work020023c4 *)(gData_020023c4 + 0x188);
    Func_02004ca8();
    Func_02004da0(0);
    actor->update = Func_02002614;
    actor->angle = TRIG_TURN / 4;
    Func_02004bc0(actor, 49);
    for (;;) {
      if (Func_02004c18(actor->terrainId, actor->x, actor->z) == 7) {
        s32 i;

        /* FAKEMATCH(signature): velocityX stored through a union with a u16
         * member, so the store aliases the work's s16 read after it; as the
         * plain member store, 2 of 186 instructions differ in tla-ja. */
        ((union {
           s32 value;
           u16 half;
         } *)&actor->velocityX)
            ->value = work->unk68 << 17;
        actor->velocityZ = work->unk6a << 17;
        actor->acceleration = Q16_ONE / 16;
        for (i = 0; i <= 9; i++) {
          position.x = actor->x + (work->unk68 << 19);
          position.y = actor->y;
          // A once-only block: its start holds position's first two stores
          // ahead of the work read (as the plain statement, 8 of 186
          // instructions differ in tla-ja).
          do {
            position.z = actor->z + (work->unk6a << 19);
          } while (0);
          if (Func_02004c38(actor, &position) != 0) {
            actor->velocityX = 0;
            actor->velocityZ = 0;
            break;
          }
          wait(1);
        }
        break;
      } else {
        s32 result;
        s32 x, z, off;

        position.x = actor->x + (work->unk68 << 19);
        position.y = actor->y;
        // A once-only block, as in the loop above, with the call inside so
        // the call reuses position's address (as plain statements, 8 of 186
        // instructions differ in tla-ja).
        do {
          position.z = actor->z + (work->unk6a << 19);
          result = Func_02004c38(actor, &position);
        } while (0);
        if (result > 0)
          break;
        if (result < 0) {
          actor->angle = TRIG_TURN / 4;
          // gMainRoutineWork.currentOwner is main->currentOwner read afresh.
          Func_02004ce8(gMainRoutineWork.currentOwner, 6, -1);
          break;
        }
        x = actor->x += work->unk68 << 17;
        /* FAKEMATCH(temporary): off copies actor->z inside a once-only block
         * whose ends hold the load between x's store and the work read; as
         * z = actor->z + (work->unk6a << 17), 8 of 186 instructions differ in
         * tla-ja. */
        do {
          off = actor->z;
        } while (0);
        z = off + (work->unk6a << 17);
        actor->z = z;
        off = (x & 0xfffff) + 0xfff80000;
        if (off > Q16_ONE)
          off = Q16_ONE;
        if (off < -Q16_ONE)
          off = -Q16_ONE;
        actor->x = x - off;
        off = (z & 0xfffff) + 0xfff80000;
        if (off > Q16_ONE)
          off = Q16_ONE;
        if (off < -Q16_ONE)
          off = -Q16_ONE;
        actor->z = z - off;
        work->unk04 = 0;
        wait(1);
      }
    }
    actor->update = 0;
    Animation_ApplyChildValues(actor, 0);
  }
  // UB: when renderMode is not 0, work is never assigned and the store goes
  // through whatever sl held on entry.
  work->unk04 = 0;
}
