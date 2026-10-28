#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"

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

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void wait(s32 frames);
struct Actor *Actor_GetById(s32 id);
void Func_020000a4(const void *data, s32 a, s32 b);
void Func_020002f8(const void *data);
void Func_02000708(const void *data);
void Func_02001474(s32 show);
void Func_02002180(void);
void Func_02003bf0(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02003c28(void);
void Func_02003c30(void);
void Func_02003d78(s32 skip);
void Func_02003dc0(const void *data);
void Func_02003dc8(void);
void Func_02003df8(s32 cue);
void Func_02003c68(s32 id, s32 x, s32 z);
void Func_02003de8(void);
void Func_02003be0(s32 srcX, s32 srcY, s32 destX, s32 destY, s32 width,
                   s32 height);
void Func_020015b8(struct Actor *object);
void Func_0200172c(void);
void Func_02001804(void);

struct CellRecord {
  u16 present;
  u16 x;
  u16 y;
  u16 unk06[3];
};

extern const u16 gData_02003fd2[];
extern const struct CellRecord gData_02004066[];

static __inline__ s32 Flag_Test(s32 flag) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  return GameFlag_Test(flag);
}

static __inline__ void Flag_Set(s32 flag) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  GameFlag_SetBit(flag);
}

static __inline__ void Flag_Clear(s32 flag) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  GameFlag_ClearBit(flag);
}

static __inline__ void Map_CopyAttributes(s32 a, s32 b, s32 c, s32 d, s32 e,
                                          s32 f) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  Func_02003bf0(a, b, c, d, e, f);
}

static __inline__ void Wait(s32 frames) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  wait(frames);
}

void Func_02001820(void) {
  const u16 *cell = &gData_02004066[0].present;
  s32 flag = 0x240;
  u32 i;
  const u16 *dest;

  for (i = 0; i <= 13; i++) {
    if (Flag_Test(0x109) != 0) {
      if (Flag_Test(flag) != 0) {
        s32 x;
        s32 y;

        cell = &gData_02004066[i].x;
        x = *cell++;
        y = *cell++;
        /* FAKEMATCH(temporary): the destination table through a local set here,
         * which loads its first halfword before the 1 is built; as
         * gData_02003fd2[0], 2 of 56 instructions differ in tla-en. */
        dest = gData_02003fd2;
        Func_02003be0(dest[0], dest[1], x, y, 1, 1);
      }
    } else {
      if (*cell != 0)
        Flag_Set(flag);
      cell += 6;
    }
    flag++;
  }
  if (Flag_Test(0x109) == 0)
    Func_02001804();
}
