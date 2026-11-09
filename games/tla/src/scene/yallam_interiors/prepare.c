#include "types.h"
#include "scene_tables.h"
#include "yallam_interiors.h"
#include "version.h"
#include "event_inline.h"

void Func_02000a60(s32 id, s32 a);

/// The scene's start entry: the screen effect 0x209, then the actors of
/// rooms 1 and 2.
s32 Func_0200086c(void) {
  s32 scene;

  ((struct YallamInteriorsEventWork *)Ram_HeapSlots->eventWork)->screenEffect =
      0x209;
  {
    /* FAKEMATCH: the party address pinned to r3 for the first scene read;
     * unpinned, gcse merges both scene reads and 26 instructions differ in
     * tla-en. */
    register struct MainRoutineWork *party asm("r3") = &gMainRoutineWork;

    scene = party->scene;
  }
  if (scene == (s32)&SceneId_Ov677Room1) {
    if (Event_GetFlag(0x301)) {
      Actor_SetPosition(14, PIXELS(600), PIXELS(360));
      Actor_SetPosition(16, PIXELS(632), PIXELS(346));
      Actor_FaceDirection(16, FACING_SOUTH, 0);
#if TLA_PATCH_1
      Actor_SetHorizontalPositionWithTerrain(17, 0, 0);
#endif
    } else {
      Func_02000a60(14, 0);
    }
    Actor_SetSingleChildFlags(
        ((struct YallamInteriorsObject *)Actor_GetById(17)), 0);
    Actor_SetSpritePriority(11, 2);
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov677Room2) {
    Func_02000a48(0);
    Actor_SetSpritePriority(12, 2);
    ((struct YallamInteriorsObject *)Actor_GetById(12))->collisionFlags |= 4;
    Actor_SetSpritePriority(10, 2);
    ((struct YallamInteriorsObject *)Actor_GetById(10))->collisionFlags |= 4;
    Actor_SetSpritePriority(11, 2);
    ((struct YallamInteriorsObject *)Actor_GetById(11))->collisionFlags |= 4;
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }
