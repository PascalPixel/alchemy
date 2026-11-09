#include "types.h"
#include "actor.h"
#include "iwram_call.h"
#include "system/vector3.h"
#include "event_inline.h"

extern s32 gData_02005af0;
struct Actor *Func_020005b4(struct Actor *object, s32 kind);
s32 Func_02005684(struct Actor *object, struct Vector3 *position);
s32 Math_Cosine(s32 angle);

void Func_02000784(void) {
  struct Actor *actor, *table;
  struct Vector3 test;
  /* FAKEMATCH(temporary): the step kept in a local set at the top; as
   * gData_02005af0 * -364, 43 of 230 instructions differ in tla-en. */
  s32 step = -364;
  s32 turn, x, z, dx, dz, back, tx, tz;

  if (gData_02005af0 == 0)
    return;
  actor = Actor_GetById(Party_GetLeaderActor());
  table = Func_020005b4(actor, 357);
  turn = gData_02005af0 * step;
  if (table != 0) {
    if (table->y + 0x100000 > actor->y)
      return;
    if (table->angle == 0x8000)
      turn = -turn;
    dx = actor->x - table->x;
    dz = actor->z - table->z;
    back = -turn;
    {
      s32 s = Math_Sin(back);
      s32 c = Math_Cosine(back);
      x = table->x + Iwram_MulQ16(c, dx) + Iwram_MulQ16(s, dz);
      z = table->z - Iwram_MulQ16(s, dx) + Iwram_MulQ16(c, dz);
    }
    test.x = x + actor->velocityX;
    test.y = table->y;
    test.z = z + actor->velocityZ;
    if (Func_02005684(actor, &test) == 0) {
      s32 s, c, angle;
      angle = 0x1555 - turn;
      s = Math_Sin(angle);
      c = Math_Cosine(angle);
      tx = table->x + Iwram_MulQ16(c, dx) + Iwram_MulQ16(s, dz);
      tz = table->z - Iwram_MulQ16(s, dx) + Iwram_MulQ16(c, dz);
      test.x = tx + actor->velocityX;
      test.z = tz + actor->velocityZ;
      if (Func_02005684(actor, &test) == 0) {
        s32 s, c, angle;
        angle = back - 0x1555;
        s = Math_Sin(angle);
        c = Math_Cosine(angle);
        tx = table->x + Iwram_MulQ16(c, dx) + Iwram_MulQ16(s, dz);
        tz = table->z - Iwram_MulQ16(s, dx) + Iwram_MulQ16(c, dz);
        test.x = tx + actor->velocityX;
        test.z = tz + actor->velocityZ;
        if (Func_02005684(actor, &test) == 0) {
          actor->x = x;
          actor->z = z;
        }
      }
    }
  }
  actor->angle += turn;
}
