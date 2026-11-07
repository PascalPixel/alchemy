#include "types.h"
#include "scene_tables.h"
#include "call.h"

void GameFlag_SetBit(s32 flag);
void Func_02002bbc();
void Func_02002aa4();

void Func_0200060c(void) {
  s32 angle;

  Call2(Func_02002bbc, 31, 415);
  /* FAKEMATCH: the local starts the facing before the other arguments, as the
   * native has (one reordered row as a plain argument). */
  angle = 0xc000;
  Call4(Func_02002aa4, 31, PIXELS(312), PIXELS(593), angle);
  GameFlag_SetBit(0xa74);
}
