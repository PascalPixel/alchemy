// Near miss, differ 6 of 179 (tla-en, 2026-10-31): the listing issues
// `movs r0, #128` (offset 586 of the auto-sleep store) right after the build
// stamp's store and before `ldr r1, [r3]` of gFrameWaitCount; here that load
// goes first. The sched2 dump gives the load only a 1-cycle wait after the
// store (priority 47 against the constant's 42) because the
// -mthumb-slow-loads read-modify-write test sees the later
// `str r1, [r3]` to gData_02001000 through the reused r3; worked by hand, a
// 2-cycle wait gives the listing's order. A scheduler cost the C cannot move.
// No single empty asm, one-pass block, wrapping block or swap of neighbouring
// statements brings it closer (device search); a 4-minute permuter run found
// nothing below 6.
#include "types.h"
#include "ram_buffer.h"
#include "save.h"
#include "main_routine_work.h"
#include "battle_unit.h"
#include "owner_state.h"
#include "frame.h"
#include "scene.h"
#include "menu.h"

struct SaveGameWords {
  u32 buildStamp;
  u32 playTime;
};

#define SAVE_GAME_WORDS ((struct SaveGameWords *)&gMainRoutineWork)

extern u8 gData_02000000[];
extern u32 gData_02001000;
extern u32 gGameFlagBytes[];

u32 BuildStamp_GetTimeFar(s32);
s32 GameFlag_Test(s32 flag);

// Builds the save header.
s32 Save_BuildFileSummary(void) {
  u16 owners[14];
  struct SaveSummary *summary;
  struct BattleUnit *owner;
  u8 *source;
  u8 *destination;
  u32 *word;
  s32 i;
  u32 sum = 0;

  summary = (struct SaveSummary *)(gData_02000000 - 16);
  SAVE_GAME_WORDS->buildStamp = BuildStamp_GetTimeFar(0);
  SAVE_GAME_WORDS->playTime = gFrameWaitCount;
  gData_02001000 = gFrameWaitCount;
  gMainRoutineWork.unk244[6] = gAutoSleepEnabled;
  owner = Unit_Get(gMainRoutineWork.currentOwner);
  destination = summary->name;
  source = owner->name;
  for (i = 11; i >= 0; i--)
    *destination++ = *source++;
  summary->level = owner->level;
  summary->playTime = SAVE_GAME_WORDS->playTime;
  summary->area =
      Scene_GetAreaNameIndexFar(gMainRoutineWork.scene, gMainRoutineWork.entrance);
  summary->classId = owner->classIndex;
  summary->coins = gMainRoutineWork.coins;
  summary->djinn[0] = Party_SumDjinnCountsFar(0);
  summary->djinn[1] = Party_SumDjinnCountsFar(1);
  summary->djinn[2] = Party_SumDjinnCountsFar(2);
  summary->djinn[3] = Party_SumDjinnCountsFar(3);
  Party_ListActiveOwnersFar((s16 *)owners);
  for (i = 0; i <= 3 && owners[i] != 255; i++)
    summary->party[i] = owners[i];
  summary->party[i] = -1;
  summary->paletteGlow = gMainRoutineWork.unk220[5];
  summary->paletteGlowSub = gMainRoutineWork.unk220[6];
  summary->sendFlag = gMainRoutineWork.unk22d[2];
  summary->flagCount = 0;
  for (i = 48; i <= 127; i++) {
    if (GameFlag_Test(i))
      summary->flagCount++;
  }
  summary->hasFlag34 = GameFlag_Test(34) != 0;
  {
    s32 n;
    u32 frames;

    frames = SAVE_GAME_WORDS->buildStamp;
    word = gGameFlagBytes;
    /* FAKEMATCH(barrier): an empty asm reading frames and word, so both load
     * before the bound; as the same statements without it, 10 of 179
     * instructions differ in tla-en (a one-pass block around the two loads
     * leaves 8). */
    __asm__("" : : "r"(frames), "r"(word));
    n = 242;
    i = 0;
    n <<= 2;
    summary->frames = frames;
    // Entered at its test: as a while loop, 32 of 179 instructions differ (it
    // counts down and loses the entry jump).
    goto test;
    do {
      sum += *word++;
      i++;
    test:;
    } while (i < n);
  }
  summary->checksum = sum;
  return 0;
}
