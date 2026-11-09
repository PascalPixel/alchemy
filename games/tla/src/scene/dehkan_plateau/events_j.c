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

void Func_02003cf0(void);
void Func_02003d50(s32 a, s32 b);
void Func_02003d58(s32 a, s32 b);
void Func_02003d60(void);
void Func_02003d68(void);
void Func_02003d70(void);
void Func_02003cc0(s32 id);
void Func_02003d40(struct Actor *object, s32 mode);
void Func_02003bb8(struct Actor *object);
void Func_02002b68(void);
void Func_02002f08(void);

struct EffectWork7c4 {
  u8 unk000[0x7c4];
  s16 active;
  s16 timer;
};
void Field_DispatchTypeHandler(s32 type);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);

void Func_02002dd4(void);

void Func_02002f74(void) {
  if (GameFlag_Test(0x207) != 0)
    Func_02002f08();
  else
    Func_02002dd4();
  GameFlag_SetBit(0x209);
}

extern struct Actor *gData_02004420;

void Func_02002f9c(void) {
  Func_02003c28();
  Event_ResolvePendingActions(0);
  if (gData_02004420 != 0)
    Func_02003d40(gData_02004420, 3);
  PartyInventory_GiveItem(199, 0);
  if (gData_02004420 != 0)
    Func_02003bb8(gData_02004420);
  Map_CopyAttributes(1, 0, 1, 1, 15, 5);
  GameFlag_SetBit(0x878);
  Func_02003c30();
}
