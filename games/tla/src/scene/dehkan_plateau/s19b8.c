#include "types.h"

extern u16 gData_02003fea[];
extern u16 gData_020041b6[];
extern u16 gData_020041b8[];

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void Func_02003be0(s32 srcX, s32 srcY, s32 destX, s32 destY, s32 width,
                   s32 height);

void Func_020019b8(void) {
  u16 *record = gData_020041b6;
  s32 flag = 0x240;
  u32 i;

  for (i = 0; i <= 15; flag++, i++) {
    if (GameFlag_Test(0x109)) {
      if (GameFlag_Test(flag)) {
        s32 x;
        s32 y;

        record = &gData_020041b8[i * 6];
        x = *record++;
        y = *record++;
        Func_02003be0(gData_02003fea[0], gData_02003fea[1], x, y, 1, 1);
      }
    } else {
      if (*record != 0)
        GameFlag_SetBit(flag);
      record += 6;
    }
  }
}
