#include "local.h"
#include "edition.h"

void Func_02003c54(void) {
  if (GameFlag_Test_(0x210) == 0) {
    GameFlag_SetBit_(0x210);
    Func_020053a8_(1, 0x210, 10, 11);
  }
  {
    /* FAKEMATCH: a temporary builds the angle ahead of the other arguments
     * (three reordered rows as a plain argument). */
    s32 angle = 0x3000;

    Func_02005238_(12, 0x1880000, 0x1680000, angle);
  }
}

void Func_02001c8c(void);
void Func_02003e8c(void);
void Func_020044d4(void);
void Func_020046d8(void);
void Func_0200482c(void);
void Func_02004fb4(void);

void Func_02003c8c(void) {
  ((struct EventWork1ac *)Ram_HeapSlots->eventWork)->value = 521;
  Func_02005390();
  Func_020053a8_(0, 527, 8, 9);
  Func_020053a8_(1, 528, 10, 11);
  if (GameFlag_Test_(0x849) == 0) {
    Actor_SetModeById_(13, 5);
    Actor_SetModeById_(15, 5);
  }
  Actor_SetModeById_(14, 5);
  Task_Add(Func_02001c8c, 0x480);
  if ((gAdventure.entrance >= 4 && gAdventure.entrance <= 6) ||
      gAdventure.entrance == 14) {
    if (GameFlag_Test_(0x84a)) {
      /* FAKEMATCH: a temporary builds the angle ahead of the other arguments
       * (one reordered row as a plain argument). */
      s32 angle = 0x5000;

      Func_02005238_(12, 0x1b00000, 0x18a0000, angle);
    } else {
      Func_02004fb4();
      if (GameFlag_Test_(0x849))
        Func_02003c54();
    }
  }
  /* FAKEMATCH: an empty asm clobbering memory stops GCSE from keeping the party
   * state's address in r5 across the tests; the native loads it again for each
   * (differ 16 without). */
  asm("" ::: "memory");
  if ((gAdventure.entrance >= 1 && gAdventure.entrance <= 3) ||
      gAdventure.entrance == 9) {
    if (GameFlag_Test_(0x212))
      Func_02001f3c();
    if (GameFlag_Test_(0x849)) {
      /* FAKEMATCH: a temporary set just before its call builds the angle ahead
       * of the other arguments, as above. */
      s32 angle;

      Func_02005230_(12, 0, 0);
      angle = 0xb000;
      Func_02005238_(15, 0x1980000, 0x2980000, angle);
    } else if (GameFlag_Test_(0x848)) {
      Func_02005230_(12, 0, 0);
      Func_02005230_(15, 0, 0);
    } else {
      Func_02003e8c();
    }
  } else if (gAdventure.entrance == 6) {
    if (GameFlag_Test_(0x849) == 0)
      Func_020044d4();
  } else if (gAdventure.entrance == 13) {
    Func_020046d8();
  } else if (gAdventure.entrance == 14) {
    if (GameFlag_Test_(0x849) == 0)
      Func_0200482c();
  }
#if (LANGUAGE != LANGUAGE_JAPANESE) /* not in the Japanese edition */
  /* FAKEMATCH: as above, the barrier makes this test load the adventure's
   * address again. */
  asm("" ::: "memory");
  if (gAdventure.entrance >= 8 && gAdventure.entrance <= 9)
    GameFlag_ClearBit_(0x12f);
#endif
}
