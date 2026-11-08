// Matches: differ 0 (tla-ja, 2026-11-08)
// Func_020024e4 of overlay 6ac: the Japanese release's own form, still on its
// scaffold in tla-ja. The other five editions link patch 1's form from
// games/tla/src/scene/mars_lighthouse_room_m/s24e4.c, so a default (tla-en)
// measure compares this draft with that function (differ 208), not this one.
// Written as overlay 6aa's Japanese draft of Func_0200263c with this
// overlay's symbols; Func_02003180 takes the position (its far target
// Func_08020210 is declared (object, position) elsewhere, and r1 holds it at
// both calls). Plain C apart from two tagged devices:
// - the step pair read through a union: an access through a union member
//   aliases every store, so the step loads wait for the velocityX and
//   position stores as in the listing. This one union replaces the earlier
//   two (position, velocityX) and the once-only blocks; around one step
//   alone it is 28.
// - an empty asm reading work before the last store: without it sched2
//   breaks the tie between the zero and reload's copy of work by RTL order
//   and sets the zero first. Returning work also matches, but is the worse
//   signature device (and breaks patch 1's form, 44).
// Tried at the tail without gain: volatile on the field (18) or through a
// cast (18), a copy of work, both arms alike, an empty test, the store in
// both branches (145), asm with "l" operands or clobbers (17 to 19, an
// extra copy), "+l" through a copy (6, registers swapped).
#include "edition.h"
#include "types.h"
#include "call.h"
#include "actor.h"
#include "main_routine_work.h"
#include "vector3.h"

struct Work020023c4 {
  u8 unk00[4];
  u16 unk04;
  u8 unk06[0x68 - 6];
  // The union is the device tagged in Func_020024e4.
  union {
    struct {
      s16 unk68;
      s16 unk6a;
    } s;
    s32 word;
  } u;
};

void Func_02002368();
struct Actor *Actor_GetById(s32 id);
void Func_020031e0(void);
void Func_02003280(s32 a);
void Func_02003118(struct Actor *actor, s32 a);
s32 Func_02003168(s32 terrain, s32 x, s32 z);
s32 Func_02003180(struct Actor *actor, struct Vector3 *position);
void Func_02003200();
void wait(s32 frames);
void Animation_ApplyChildValues(struct Actor *actor, s32 a);
void Func_020024bc(void);
extern u8 gData_020023c4[];

void Func_020024e4(void) {
  struct MainRoutineWork *main = &gMainRoutineWork;
  struct Work020023c4 *work;
  struct Vector3 position;

  if (main->renderMode == 0) {
    struct Actor *actor;

    ((s32 (*)())Func_02002368)(-10, 0);
    actor = Actor_GetById(main->currentOwner);
    /* FAKEMATCH(temporary): the step pair read through a union beside a word;
     * as plain s16 fields, 26 of 186 instructions differ in tla-ja. */
    work = (struct Work020023c4 *)(gData_020023c4 + 0x188);
    Func_020031e0();
    Func_02003280(0);
    actor->update = Func_020024bc;
    actor->angle = 0x4000;
    Func_02003118(actor, 49);
    for (;;) {
      if (Func_02003168(actor->terrainId, actor->x, actor->z) == 7) {
        s32 i;

        actor->velocityX = work->u.s.unk68 << 17;
        actor->velocityZ = work->u.s.unk6a << 17;
        actor->acceleration = 0x1000;
        for (i = 0; i <= 9; i++) {
          position.x = actor->x + (work->u.s.unk68 << 19);
          position.y = actor->y;
          position.z = actor->z + (work->u.s.unk6a << 19);
          if (Func_02003180(actor, &position) != 0) {
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

        position.x = actor->x + (work->u.s.unk68 << 19);
        position.y = actor->y;
        position.z = actor->z + (work->u.s.unk6a << 19);
        result = Func_02003180(actor, &position);
        if (result > 0)
          break;
        if (result < 0) {
          actor->angle = 0x4000;
          Call3((void (*)())Func_02003200, gMainRoutineWork.currentOwner, 6,
                -1);
          break;
        }
        x = actor->x += work->u.s.unk68 << 17;
        z = actor->z + (work->u.s.unk6a << 17);
        actor->z = z;
        off = (x & 0xfffff) + 0xfff80000;
        if (off > 0x10000)
          off = 0x10000;
        if (off < (s32)0xffff0000)
          off = 0xffff0000;
        actor->x = x - off;
        off = (z & 0xfffff) + 0xfff80000;
        if (off > 0x10000)
          off = 0x10000;
        if (off < (s32)0xffff0000)
          off = 0xffff0000;
        actor->z = z - off;
        work->unk04 = 0;
        wait(1);
      }
    }
    actor->update = 0;
    Animation_ApplyChildValues(actor, 0);
  }
  /* FAKEMATCH(barrier): an empty asm reading work before the last store; as
   * the plain store, 2 of 186 instructions differ in tla-ja. */
  asm("" : : "r"(work));
  work->unk04 = 0;
}
