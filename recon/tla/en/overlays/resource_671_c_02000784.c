// Draft, differ 280 of 230: turns the controlled character with a turntable
// (Func_020005b4 finds the one under it, kind 357): unless it stands below
// the table's top, it is rotated about the table's centre by the turn in
// gData_02005af0 (reversed when the table faces 0x8000), the turn and two
// spots a sixth of a turn either side checked for room (Func_02005684)
// before it moves; it always turns by the same amount. Left: a rough reading: the native keeps the offsets, the turn and both candidate positions on the stack, the table in r9 and the test vector's address in r10, and calls the multiply through a fresh pool load each time; the alignment is poor.
#include "types.h"
#include "actor.h"
#include "iwram_call.h"
#include "system/vector3.h"

extern s32 gData_02005af0;
s32 Func_0200584c(void);
struct Actor *Actor_GetById(u32 id);
struct Actor *Func_020005b4(struct Actor *object, s32 kind);
s32 Func_02005684(struct Actor *object, struct Vector3 *position);
s32 Math_Sine(s32 angle);
s32 Math_Cosine(s32 angle);

void Func_02000784(void) {
  struct Actor *actor, *table;
  struct Vector3 test;
  s32 turn, back, dx, dz, s, c, x, z, angle;

  if (gData_02005af0 == 0)
    return;
  actor = Actor_GetById(Func_0200584c());
  table = Func_020005b4(actor, 357);
  turn = gData_02005af0 * -364;
  if (table != 0) {
    if (table->y + 0x100000 > actor->y)
      return;
    if (table->angle == 0x8000)
      turn = -turn;
    dx = actor->x - table->x;
    dz = actor->z - table->z;
    back = -turn;
    s = Math_Sine(back);
    c = Math_Cosine(back);
    x = table->x + Iwram_MulQ16(c, dx) + Iwram_MulQ16(s, dz);
    z = table->z - Iwram_MulQ16(s, dx) + Iwram_MulQ16(c, dz);
    test.x = x + actor->velocityX;
    test.y = table->y;
    test.z = z + actor->velocityZ;
    if (Func_02005684(actor, &test) == 0) {
      angle = 0x1555 - turn;
      s = Math_Sine(angle);
      c = Math_Cosine(angle);
      test.x = table->x + Iwram_MulQ16(c, dx) + Iwram_MulQ16(s, dz) +
               actor->velocityX;
      test.z = table->z - Iwram_MulQ16(s, dx) + Iwram_MulQ16(c, dz) +
               actor->velocityZ;
      if (Func_02005684(actor, &test) == 0) {
        angle = back - 0x1555;
        s = Math_Sine(angle);
        c = Math_Cosine(angle);
        test.x = table->x + Iwram_MulQ16(c, dx) + Iwram_MulQ16(s, dz) +
                 actor->velocityX;
        test.z = table->z - Iwram_MulQ16(s, dx) + Iwram_MulQ16(c, dz) +
                 actor->velocityZ;
        if (Func_02005684(actor, &test) == 0) {
          actor->x = x;
          actor->z = z;
        }
      }
    }
  }
  actor->angle += turn;
}
