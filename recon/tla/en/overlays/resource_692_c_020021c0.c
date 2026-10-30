// Matches: differ 0 of 138 (tla-en, 2026-10-30); the object's bytes and
// relocations equal the listing's, literal pool included. The last row was the
// prepared actor's copy into r7: the three insns left after the call (that copy,
// the motionFlags store, i = 0) tie in sched2 and go in RTL order, and the copy
// is emitted right after the call. A temporary assigned to actor after the
// store is merged back by copy propagation (2); an empty asm with "+l" on it
// still allocates it to r7 (10). Holding it in r0 (a pin, tagged below) emits
// the copy after the store.
// Under -mthumb-slow-loads (2026-10-26): differ 2 of 138; left: the effect
// pointer's copy (adds r7, r0) comes before the motionFlags store here, after
// it in the native; assigning it after the store changes nothing.
// Near miss of Func_020021c0 of overlay 692 (138 instructions, 2 differ): one
// scheduling row. After OverlayObject_PrepareObject the native clears the
// motion flags byte before it copies the returned actor into r7; the draft
// copies it first, the same row as Func_020028ac's draft. Tried for that: i = 0
// before the store (15), Value4 for the prepare call (2), the store through a
// u8 pointer (2), an empty asm after the store (12).
#include "../../../../games/tla/src/scene/magma_rock_inside/local.h"
#include "io_reg.h"
#include "call.h"

struct FieldActor;

void Func_02004f30(s32 cue);
void Func_02004d40(void);
void Func_02004d48(void);
void Func_02004d10(s32 a, s32 b, s32 c);
void Func_02004e20(void);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
struct FieldActor *OverlayObject_PrepareObject(s32 x, s32 y, s32 z, s32 type);
void Func_02004cb0(struct FieldActor *actor);
s32 Func_02000694(u32 layer, s32 x, s32 y, s32 value);

void Func_020021c0(void) {
  u8 *work = Ram_HeapSlots->eventWork;
  struct Actor *object;
  struct FieldActor *actor;
  struct Actor *other;
  s32 i;

  object = Actor_GetById(((s8 *)work)[0xd30]);
  Actor_SetPartAttribute(object, 0);
  Func_02004f30(136);
  i = 0;
  do {
    object->scaleY = object->scaleX += 0x1000;
    wait(5);
    i++;
  } while (i != 16);
  GameFlag_SetBit(((s8 *)work)[0xd30] + 0x2ee);
  if (GameFlag_Test(0x303) && GameFlag_Test(0x304)) {
    other = Actor_GetById(11);
    Func_02004d40();
    Func_02004e90(0);
    Call3((void (*)())Func_02004d10, 0x20000, 0x10000, 0x10000);
    Func_02004f30(141);
    FieldEvent_Wait(30);
    Camera_MoveWithinMapBounds(0x2680000, -1, 0x1280000, 1);
    Func_02004e20();
    FieldEvent_Wait(30);
    Func_02004f30(0x22d);
    {
      register u16 alpha __asm__("r3") =
          0x1000; /* FAKEMATCH: the blend value pinned to r3 in a one-pass
                     block, as the native stores it (differ 20 as a plain store,
                     37 without the pins, 20 without the blocks). */

      do {
      } while (0); /* FAKEMATCH: with the pin above. */
      REG_BLDALPHA = alpha;
    }
    REG_BLDCNT = 0x3f10;
    {
      /* FAKEMATCH(pin): the prepared actor held in r0, the register it is
       * returned in, until the motion flags are cleared; as actor =
       * OverlayObject_PrepareObject(...) ahead of the store, 2 of 138
       * instructions differ in tla-en. */
      register struct FieldActor *prepared __asm__("r0") =
          OverlayObject_PrepareObject(other->x, other->terrainHeight,
                                      other->z + 0x300000, 312);

      other->motionFlags = 0;
      actor = prepared;
    }
    i = 0;
    do {
      other->y -= 0x10000;
      FieldEvent_Wait(5);
      i++;
    } while (i != 32);
    Func_02004f30(0x22a);
    Func_02004c98(other, 4);
    Func_02004cb0(actor);
    FieldEvent_Wait(1);
    {
      register u16 off __asm__("r3") =
          0; /* FAKEMATCH: the zero pinned to r3 in a one-pass block (differ 4
                as a plain store). */

      do {
      } while (0); /* FAKEMATCH: with the pin above. */
      REG_BLDCNT = off;
    }
    Func_02000694(0, 38, 18, 0);
    Func_02004d48();
  }
}
