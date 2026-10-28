#include "types.h"
#include "actor.h"
#include "sequence_parent_work.h"
#include "edition.h"

extern char SceneId_Ov671Room7, SceneId_Ov672Room1;
void Func_02001e80(s32 id);
struct Actor *Actor_GetById(u32 id);
s32 GameFlag_Test(s32 flag);
void Func_02001094(void);
void Func_02001e10(s32 id, s32 x, s32 z);
void Func_02000f88(void);

/// Gives actors 15 to 32 action variant 2, runs Func_02001e80 for actor 8 and
/// grounds it, then Func_02001094 with actor 10 placed when flag 0x8ff is set,
/// else Func_02000f88; arriving by entrance 2 it sets the retreat to
/// Ov671Room7's entrance 1 (flag 0xa27) or Ov672Room1's entrance 3 (flag
/// 0xa28).
void Func_02001c44(void) {
  u32 i;
  char *scene;

  for (i = 0; i <= 17; i++)
    Actor_SetSpritePriority(i + 15, 2);
  Func_02001e80(8);
  Actor_GetById(8)->y = 0;
  if (GameFlag_Test(0x8ff)) {
    Func_02001094();
    Func_02001e10(10, 0, 0);
  } else {
    Func_02000f88();
  }
#if (LANGUAGE != LANGUAGE_JAPANESE) /* The Japanese edition does not set the retreat. */
  if (gSequenceParentWork.entrance == 2) {
    if (GameFlag_Test(0xa27)) {
      /* FAKEMATCH(one-pass): the scene id through a temporary and a once-only
       * block, which issues its literal load first; as plain stores, 4 of 71
       * instructions differ in tla-en. */
      scene = &SceneId_Ov671Room7;
      do {
        gSequenceParentWork.retreatScene = (s32)scene;
        gSequenceParentWork.retreatEntrance = 1;
      } while (0);
    } else if (GameFlag_Test(0xa28)) {
      /* FAKEMATCH(one-pass): the scene id through a temporary and a once-only
       * block, as the first branch; as plain stores, 4 of 71 instructions
       * differ in tla-en. */
      scene = &SceneId_Ov672Room1;
      do {
        gSequenceParentWork.retreatScene = (s32)scene;
        gSequenceParentWork.retreatEntrance = 3;
      } while (0);
    }
  }
#endif
}
