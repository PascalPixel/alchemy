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

extern const u16 gData_02003fd2[];
extern const struct CellRecord gData_02004066[];

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void Map_CopyAttributes(s32 a, s32 b, s32 c, s32 d, s32 e,
                                          s32 f) {
  Map_CopyCellAttributeRect(a, b, c, d, e, f);
}

void Func_02002140(void) {
  s32 set = GameFlag_Test(0x207);

  if (set == 0) {
    Actor_GetById(8)->motionFlags = set;
    Map_CopyAttributes(51, 12, 1, 1, 47, 12);
    GameFlag_SetBit(0x202);
  }
}

void Func_02002180(void) {
  if (GameFlag_Test(0x207) == 0) {
    Map_CopyAttributes(48, 11, 1, 1, 47, 12);
    GameFlag_ClearBit(0x202);
  }
}

void Func_020021b4(void) {
  struct Actor *object = Func_080d4568();
  u32 i;

  object->y += -0x80000;
  wait(1);
  for (i = 0; i <= 7; i++) {
    object->y += 0x10000;
    wait(1);
  }
}
