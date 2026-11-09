#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_inline.h"

extern const u8 gData_02003f44[];
extern const u8 gData_02003f52[];
extern const u8 gData_02003f80[];
extern const u8 gData_02003f92[];
extern const u8 gData_02003f98[];
extern const u8 gData_02003fa0[];
extern const u8 gData_02003fa8[];
extern const u8 gData_02003fb0[];
extern const u8 gData_02003fb8[];
extern const u8 gData_02003fc0[];
extern const u8 gData_02003fc4[];

void Func_020000a4(const void *data, s32 a, s32 b);
void Func_020002f8(const void *data);
void Func_02000708(const void *data);
void Func_02001474(s32 show);
void Func_02002180(void);
void Func_02003c28(void);
void Func_02003c30(void);
void Func_02003dc0(const void *data);
void Func_02003dc8(void);
void Func_02003de8(void);
void Func_020015b8(struct Actor *object);
void Func_0200172c(void);
void Func_02001804(void);

struct CellRecord {
  u16 present;
  u16 x;
  u16 y;
  u16 unk06[3];
};

extern const u16 gData_02003fda[];
extern const struct CellRecord gData_0200410e[];

void Func_020018a8(void) {
  const u16 *cell = &gData_0200410e[0].present;
  s32 flag = 0x240;
  u32 i;
  const u16 *dest;

  for (i = 0; i <= 7; i++) {
    if (GameFlag_Test(0x109) != 0) {
      if (GameFlag_Test(flag) != 0) {
        s32 x;
        s32 y;

        cell = &gData_0200410e[i].x;
        x = *cell++;
        y = *cell++;
        /* FAKEMATCH(temporary): the destination table through a local set here,
         * which loads its first halfword before the 1 is built; as
         * gData_02003fda[0], 2 of 56 instructions differ in tla-en. */
        dest = gData_02003fda;
        Map_CopyCellPictureIndicesRect(dest[0], dest[1], x, y, 1, 1);
      }
    } else {
      if (*cell != 0)
        GameFlag_SetBit(flag);
      cell += 6;
    }
    flag++;
  }
  if (GameFlag_Test(0x109) == 0)
    Func_02001804();
}
