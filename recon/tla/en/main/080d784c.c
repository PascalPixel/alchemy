// Matches: differ 0 (tla-en, 2026-11-08)
// Other editions: tla-ja at 080d7808, tla-de at 080e484c, tla-es at 080e0840,
// tla-fr at 080dc840 and tla-it at 080d7840 hold the same 0xf0 bytes as
// tla-en but for the seven calls' bl offsets (pool words equal), so the one
// form should serve all six; diff cannot measure them yet, as their scaffold
// sections carry no symbol ("unknown ISA"). Confirm with make compare-editions
// when adopting.
// 2026-11-08, how: the head left was reload, not the C. With 0xfff set after
// the 0xf000 and (the native's order), the 0xfff is an output reload into
// scene's sl through r3 (`*thumb_movsi_gba_const` takes only a low register),
// so the and inherits r3 (`ands r3, r2; mov sl, r3`) where the native reloads
// sl through r0. An asm with scene as an in-out operand stores into scene
// between the two, which ends that inheritance and leaves the native's
// sequence; an `r3` clobber in its place matches too. The Broken Seal's twin
// gets the native's head from its one-expression masks because its 0xfff is
// a pool constant, an input reload that nothing inherits; under -mtune=gba
// the same masks give 57 here. Milder devices all stayed at 57 or worse: a
// self-assignment, a redundant or both-arms-alike store (folded before
// combine), a mask local or a copy of scene (combine merges the copy into the
// and), a taken address or pointer to scene (65 to 70), inline helpers for
// the and or the mask (57 to 102), and a label kept by an unused static
// address (deleted with it); an empty asm without the operand stays at 57.
// Earlier, differ 6 of 107 (2026-11-07 and 08): 0xfff set before the 0xf000
// and, so that the 0xf000's r3 ended the inheritance, with an empty once-only
// block putting the ldrsh first; left was the 0xf000 mask built after the
// 0xfff where the native builds it first.
// Earlier, differ 8 of 107 (2026-11-06), 53 (2026-11-05) and 57 (2026-11-01).
// Twin of The Broken Seal's Actor_PlaceMapActors (the same pending-event test,
// flag 0x109, range 300 to 380, placement match and 32-pixel lift); ⚓️ places
// ids 8 to 79 and calls Actor_SetMode. Both natives share the head's
// "mov sl, r3; mov r0, sl; ands r0, r2; mov sl, r0".
#include "types.h"
#include "actor.h"
#include "main_routine_work.h"

struct ActorDescriptor {
  s16 action;
  s16 scene;
  u8 unk04[4];
  s32 x;
  s32 y;
  s32 z;
};

s32 GameFlag_Test(s32 flag);
struct ActorDescriptor *Scene_FindActorPlacement(s32 id);
void Actor_SetPositionAndResetMotionFar(struct Actor *object, s32 x,
                                         s32 y, s32 z);

void Func_080d784c(void) {
  struct MainRoutineWork *party = &gMainRoutineWork;
  u32 raw = (u16)party->pendingDjinnEvent;
  s32 flags = party->pendingDjinnEvent & 0xf000;
  u32 scene = 0xfff;
  s32 id;

  /* FAKEMATCH(barrier): the form kept; as scene &= raw without the asm, 57 of 107 instructions differ in tla-en. */
  asm("" : "+r"(scene));
  scene &= raw;
  if (GameFlag_Test(0x109) != 0 && flags == 0) {
    flags = scene & 0x800;
    scene &= 0x7ff;
    if (scene - 300 <= 80 && party->sequenceResult > 0) {
      s32 *owner;

      for (id = 8, owner = &party->currentOwner; id <= 79; id++) {
        struct ActorDescriptor *desc = Scene_FindActorPlacement(id);
        struct Actor *obj;

        if (desc == NULL || desc->scene - 48 != scene - 300)
          continue;
        obj = ObjectTable_Get(id);
        if (obj == NULL)
          continue;
        if (flags == 0) {
          obj->terrainHeight = flags;
          obj->motionFlags = 3;
          Actor_SetPositionAndResetMotionFar(obj, desc->x, desc->y, desc->z);
        } else {
          struct Actor *selection = ObjectTable_Get(*owner);

          Actor_SetPositionAndResetMotionFar(obj, selection->x, selection->y,
                                              selection->z - (32 << 16));
        }
        Actor_SetMode(obj, 1);
      }
    }
  }
}
