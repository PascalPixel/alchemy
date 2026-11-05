// Draft, differ 13 of 50 (tla-en, 2026-11-05). Older notes below may predate this figure.
// Measured under -Os, 5 November 2026: differ 15 (tla-en); notes below may predate it.
// Draft, differ 35 of 50: the native takes both scene state addresses before
// the flag test, reloads the table each pass beside an i * 4 index and a
// reduced second offset, and sign-extends the mode inside the loop; this draft
// shares one pointer. Tried: a single index (51), chained stores, pointer
// locals (32), an s32 mode cast at the call, a minute of alchemy permute.
#include "../../../../games/tla/include/types.h"
#include "../../../../games/tla/include/actor.h"

extern u16 gSceneState[];
extern const s16 gData_0200533c[][2];

s32 GameFlag_Test(s32 flag);
s32 Func_02004fe4(void);
struct Actor *Actor_GetById(u32 id);
void Func_02004ebc(s32 id, s32 x, s32 z);
void Func_02002b04(u32 id, s8 mode);
void Func_02002b8c(void);
void Task_Add(void (*func)(void), s32 priority);

void Func_02002cb4(u8 mode) {
  if (!GameFlag_Test(265)) {
    s32 i;
    s32 id;

    /* FAKEMATCH(one-pass): the statement in a once-only block, whose end holds the
     * order around it; as the plain statement, 15 of 50 instructions differ
     * in tla-en. */
    do {
      gSceneState[0] = 0;
    } while (0);
    gSceneState[1] = 0;
    Actor_GetById(Func_02004fe4())->priorityFlags |= 0x40;
    for (i = 0, id = 8; id <= 16; i++, id++) {
      Func_02004ebc(id, gData_0200533c[i][0] << 19, gData_0200533c[i][1] << 19);
      Func_02002b04(id, mode);
    }
  }
  Task_Add(Func_02002b8c, 0x480);
}
