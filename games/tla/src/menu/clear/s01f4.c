#include "types.h"
#include "input.h"

/// The two secret button codes: unlocked, held and progress of each.
extern s16 gData_020024f6;
extern s16 gData_020024f8;
extern s16 gData_020024fa;
extern s16 gData_020024fc;
extern s16 gData_020024fe;
extern s16 gData_02002500;
extern const u16 gData_02002410[];
extern const u16 gData_02002418[];

void Func_02002408(s32 cue);

/// Follows the two secret button codes: each held key that matches the next
/// key of a sequence advances it, a wrong key starts it again, and the end of
/// the sequence unlocks the code with a sound.
void Func_020001f4(void) {
  if (gData_020024f6 == 0) {
    /* FAKEMATCH: the held keys are read through a volatile pointer pinned to
     * r4, and the key that clears the held flag is pinned to r3; unpinned, the
     * pointer takes r0 and the progress value r4 (differ 24), and with the
     * pointer pinned alone the key read takes r4 (differ 12). The one-pass
     * block keeps the held flag's store beside the progress store (differ 32
     * without it). */
    register volatile u32 *held asm("r4") = &gInput.held;

    if (gData_020024fe != 0) {
      register u32 keys asm("r3") = *held;

      if (keys == 0)
        gData_020024fe = keys;
    } else if (*held != 0) {
      if (*held == gData_02002410[gData_020024fa]) {
        do {
          gData_020024fa++;
          gData_020024fe = 1;
        } while (0);
        if (gData_02002410[gData_020024fa] == 0) {
          gData_020024f6 = 1;
          Func_02002408(110);
        }
      } else {
        gData_020024fa = 0;
      }
    }
  }
  if (gData_020024f8 == 0) {
    /* FAKEMATCH: the same pins and one-pass block as the first code, for the
     * same differences. */
    register volatile u32 *held asm("r4") = &gInput.held;

    if (gData_02002500 != 0) {
      register u32 keys asm("r3") = *held;

      if (keys == 0)
        gData_02002500 = keys;
    } else if (*held != 0) {
      if (*held == gData_02002418[gData_020024fc]) {
        do {
          gData_020024fc++;
          gData_02002500 = 1;
        } while (0);
        if (gData_02002418[gData_020024fc] == 0) {
          gData_020024f8 = 1;
          Func_02002408(110);
        }
      } else {
        gData_020024fc = 0;
      }
    }
  }
}
