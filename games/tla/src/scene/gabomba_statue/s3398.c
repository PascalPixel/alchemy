#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"
#include "scene_ids.h"

struct EventWork1ac {
  u8 unk000[0x1ac];
  s32 screenEffect;
};

s32 GameFlag_Test(s32 flag);
struct ObjectRuntime *Object_GetById(s32 id);
s32 Func_0200584c(void);
void Func_02001c10(void);
void Func_020020f0(s32 value);
void Func_020034ec(void);
void Func_020034f4(void);
void Func_0200359c(void);
void Func_020035f0(void);
void Func_02003634(void);
void Func_02003868(void);
void Func_02005674(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);

static __inline__ void Map_CopyBlock(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  Func_02005674(a, b, c, d, e, f);
}

/// Prepare the current room.
s32 Func_02003398(void) {
  s16 scene;
  struct PartyState *party;

  ((struct EventWork1ac *)Ram_HeapSlots->eventWork)->screenEffect = 521;
  if (gPartyState.scene != (s32)&SceneId_Ov671Room1) {
    if (GameFlag_Test(515))
      Func_020020f0(0);
    else if (GameFlag_Test(0x8cd))
      Func_020020f0(-1);
    else
      Func_020020f0(1);
    Func_02001c10();
  }
  Object_GetById(Func_0200584c())->priorityFlags |= 0x20;
  party = &gPartyState;
  /* FAKEMATCH: the native loads the party state's address before it builds the
   * scene's offset; the plain read builds the offset first (one reordered pair,
   * differ 2). */
  asm volatile("" : "+l"(party));
  scene = party->scene;
  if (scene == (s32)&SceneId_Ov671Room1) {
    Func_020034ec();
  } else if (scene == (s32)&SceneId_Ov671Room2) {
    Map_CopyBlock(15, 68, 8, 3, 15, 4);
    Func_020034f4();
  } else if (scene == (s32)&SceneId_Ov671Room3) {
    Func_0200359c();
  } else if (scene == (s32)&SceneId_Ov671Room5) {
    Func_020035f0();
  } else if (scene == (s32)&SceneId_Ov671Room6) {
    Func_02003634();
  } else if (scene == (s32)&SceneId_Ov671Room7) {
    Func_02003868();
  }
  return 0;
}
