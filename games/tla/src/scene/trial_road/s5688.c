#include "trial_road.h"

struct TrialRoadControl {
  s16 state;
  s16 field02;
  s16 field04;
  s16 field06;
  s16 field08;
  s16 field0a;
};

extern u8 gSceneState[];

void Func_02004ddc(void);

void Func_02005688(s32 a) {
  u8 *work = Ram_HeapSlots->unkec[0];
  struct TrialRoadControl *control = (struct TrialRoadControl *)gSceneState;

  if (GameFlag_Test(0x109) == 0) {
    /* FAKEMATCH: a temporary reads the work value before the three stores
     * (differ 4 without it). */
    s32 value = *(u16 *)(work + 228);

    control->field02 = a;
    control->field04 = value;
    control->field08 = 0;
  }
  {
    /* FAKEMATCH: a temporary keeps the priority constant ahead of the task function
     * load (differ 2 without it). */
    s32 priority = 0x485;

    Task_Add(Func_02004ddc, priority);
  }
}

void Func_020056c4(s32 a) {
  u8 *work = Ram_HeapSlots->unkec[0];
  struct TrialRoadControl *control = (struct TrialRoadControl *)gSceneState;
  /* FAKEMATCH: a word temporary builds 0 with movs for both halfword stores,
   * where a halfword constant loads from the pool. */
  s32 zero = 0;

  *(u16 *)(work + 234) = zero;
  control->field0a = a;
  control->field06 = zero;
  control->state = 1;
  Func_02004ddc();
}

s32 Func_020056e8(void) {
  s32 state = *(s16 *)gSceneState;

  return IsZero(state ^ 9);
}

void Func_02005704(void) {}
