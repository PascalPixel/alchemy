#include "types.h"
#include "ram_buffer.h"
#include "adventure.h"
#include "edition.h"

/// The world map's event work, as far as the encounter events read it.
struct EncounterWork {
  u8 unk000[0x194];
  s32 bias;
  s32 threshold;
};

u32 Random16Far(void);
void Func_02004d50(s32 a, s32 b);

// Once the party has walked nine tenths of the encounter threshold, a coin
// toss either requests the event's battle now or makes the party walk the
// whole threshold first. The Japanese release has no render mode test.
#if (LANGUAGE == LANGUAGE_JAPANESE)
#define WORLD_MAP_TRY_ENCOUNTER(a, b)                                          \
  {                                                                            \
    struct EncounterWork *work;                                                \
    struct Adventure *party;                                                  \
                                                                               \
    work = Ram_HeapSlots->eventWork;                                           \
    /* FAKEMATCH: an empty asm after the work load keeps it ahead of           \
       gAdventure's address, which the native loads before it builds          \
       the 0x258 offset; plain, the offset starts first (differ 2). */         \
    asm("");                                                                   \
    party = &gAdventure;                                                      \
    if (party->encounterSteps >= work->threshold * 9 / 10) {                   \
      if (Random16Far() < 0x8000) {                                            \
        Func_02004d50(a, b);                                                   \
        work->bias = 0;                                                        \
      } else {                                                                 \
        party->encounterSteps = work->threshold;                               \
      }                                                                        \
    }                                                                          \
  }
#else
#define WORLD_MAP_TRY_ENCOUNTER(a, b)                                          \
  {                                                                            \
    struct Adventure *party;                                                  \
    struct EncounterWork *work;                                                \
                                                                               \
    party = &gAdventure;                                                      \
    /* FAKEMATCH: an empty asm after gAdventure's address keeps it            \
       ahead of the 0x212 offset and the work load; as no asm, 2               \
       instructions differ in tla-en. */                                       \
    asm("");                                                                   \
    work = Ram_HeapSlots->eventWork;                                           \
    if (party->renderMode == 1) {                                              \
      if (party->encounterSteps >= work->threshold * 9 / 10) {                 \
        if (Random16Far() < 0x8000) {                                          \
          Func_02004d50(a, b);                                                 \
          work->bias = 0;                                                      \
        } else {                                                               \
          party->encounterSteps = work->threshold;                             \
        }                                                                      \
      }                                                                        \
    }                                                                          \
  }
#endif

void Func_02000038(void) { WORLD_MAP_TRY_ENCOUNTER(0x80a, 8) }

void Func_0200009c(void) { WORLD_MAP_TRY_ENCOUNTER(0x80b, 28) }

void Func_02000100(void) { WORLD_MAP_TRY_ENCOUNTER(0x80c, 31) }

void Func_02000164(void) { WORLD_MAP_TRY_ENCOUNTER(0x80d, 72) }

void Func_020001c8(void) { WORLD_MAP_TRY_ENCOUNTER(0x80e, 13) }

void Func_0200022c(void) { WORLD_MAP_TRY_ENCOUNTER(0x80f, 53) }
