#include "types.h"
#include "edition.h"
#include "adventure.h"
#include "object_runtime.h"

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))

struct ObjectRuntime *Object_GetById(s32 id);
void Vector_AddPolarOffset(s32 length, s32 angle, s32 *position);
struct ObjectRuntime *Func_080eaf28(s32 *position, s32 mode);
s32 Func_08020210(struct ObjectRuntime *object, s32 *position);

/// Whether the object 16 pixels in front of the current owner, along its
/// facing rounded to a quarter turn, can be pushed on by 16 pixels: 1 when
/// nothing solid stands on the cell beyond it or above it and its terrain
/// test passes there, else 0. Func_080cdd80, after it, makes the push.
// The Japanese edition has no test against the paused object and stays on
// its scaffold: with the branch below it differs by one reordered row (the
// angle's argument copy issued after the next position load, differ 4).
s32 Func_080cdc74(void) {
  s32 position[3];
  s32 above[3];
  struct ObjectRuntime *owner = Object_GetById(gAdventure.currentOwner);
  struct ObjectRuntime *target;
  struct ObjectRuntime *hit;
  s32 id;
  s32 terrain;
  s32 result;
  /* FAKEMATCH(pin): the half cell (128 << 11) started in r2, as the native
   * does; as an ordinary local, 19 of 122 instructions differ in tla-en. */
  register s32 half asm("r2");
  s32 angle = owner->angle >> 12;

  angle = ((angle + 2) & 12) << 12;
  position[0] = owner->x;
  half = 128;
  /* FAKEMATCH(barrier): the half cell started before the owner's height is
   * loaded, as the native does; without the barrier, 2 of 122 instructions
   * differ in tla-en (as plain `owner->y + 0x40000`, also 2). */
  __asm__ volatile("" : "+r"(half));
  position[1] = (owner->y + (half << 11)) & 0xfff80000;
  position[2] = owner->z;
  Vector_AddPolarOffset(0x100000, angle, position);
  target = Func_080eaf28(position, 1);
  if (target == 0)
    return 0;
  if (BYTE_AT(target, 98) != 0)
    return 0;
#if LANGUAGE != LANGUAGE_JAPANESE
  id = gAdventure.pausedObject;
  if (id != 1 && target == Object_GetById(id))
    return 0;
#endif
  position[0] = target->x;
  position[1] = target->y;
  position[2] = target->z;
  Vector_AddPolarOffset(0x100000, angle, position);
  hit = Func_080eaf28(position, 1);
  if (hit != 0 && (hit->collisionFlags & 1))
    return 0;
  above[0] = target->x;
  above[1] = target->y + 0x100000;
  above[2] = target->z;
  hit = Func_080eaf28(above, 1);
  if (hit != 0 && (hit->collisionFlags & 1))
    return 0;
  terrain = target->terrainId;
  target->terrainId = 2;
  result = Func_08020210(target, position);
  target->terrainId = terrain;
  if (result > 0)
    return 0;
  return 1;
}
