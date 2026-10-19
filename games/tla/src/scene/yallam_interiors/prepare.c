#include "types.h"
#include "field_scene.h"
#include "call.h"
#include "flag_call.h"
#include "yallam_interiors.h"
#include "edition.h"

void Func_02000a60(s32 id, s32 a);

/// The scene's start entry: the screen effect 0x209, then the actors of
/// rooms 1 and 2.
s32 Func_0200086c(void) {
  s32 scene;

  ((struct YallamInteriorsEventWork *)Ram_HeapSlots->eventWork)->screenEffect =
      0x209;
  {
    /* FAKEMATCH: the party address pinned to r3 for the first scene read.
     * Unpinned, gcse gives both scene reads one pseudo and this read takes r1,
     * the store's offset r0 (differ 26); plain forms measured against it: a
     * scene local, a work local, a party local, an inline read, a one-pass
     * block or an empty asm after the store (all 26), a volatile read (15 or
     * 18). */
    register struct PartyState *party asm("r3") = &gPartyState;

    scene = party->scene;
  }
  if (scene == (s32)&SceneId_Ov677Room1) {
    if (Flag_IsSet(0x301)) {
      Call3((void (*)())Func_020009f8, 14, 0x2580000, 0x1680000);
      Call3((void (*)())Func_020009f8, 16, 0x2780000, 0x15a0000);
      Call3((void (*)())ObjectMotion_ArmCallback, 16, 0x4000, 0);
#if (LANGUAGE != LANGUAGE_JAPANESE)
      Call3((void (*)())Func_020009f8, 17, 0, 0);
#endif
    } else {
      Call2((void (*)())Func_02000a60, 14, 0);
    }
    ObjectDispatch_SetSingleChildFlags(Object_GetById(17), 0);
    Call2((void (*)())ObjectMotion_SetActionVariant, 11, 2);
  }
  if (gPartyState.scene == (s32)&SceneId_Ov677Room2) {
    Call1((void (*)())Func_02000a48, 0);
    Call2((void (*)())ObjectMotion_SetActionVariant, 12, 2);
    Object_GetById(12)->collisionFlags |= 4;
    Call2((void (*)())ObjectMotion_SetActionVariant, 10, 2);
    Object_GetById(10)->collisionFlags |= 4;
    Call2((void (*)())ObjectMotion_SetActionVariant, 11, 2);
    Object_GetById(11)->collisionFlags |= 4;
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }
