#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "iwram_call.h"
#include "event_inline.h"

struct WarpTarget {
  s32 scene;
  u16 x;
  u16 z;
};

s32 ArcTan2(s32, s32);
void Animation_SetIndexAndInitializeObjects(struct Actor *object, s32 index);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02001b7c(struct Actor *object, s32 mode);
void Func_02001ba4(struct Actor *object, s32 value);
void Func_02001bcc(void);
void Func_02001bd4(void);
void Func_02001cf4(s32 a, s32 b, s32 c);
void Func_02001d44(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g);
s32 Func_02001d54(struct Actor *object);
s32 Func_02001d5c(s32 x, s32 z, s32 layer);
void Func_02001d64(s32 x, s32 z, s32 layer, s32 value);

extern const struct WarpTarget gData_020021bc;
extern const struct WarpTarget gData_020021c4;
extern const struct WarpTarget gData_020021cc;
extern const struct WarpTarget gData_020021d4;
extern const struct WarpTarget gData_020021dc;
extern const struct WarpTarget gData_020021e4;
extern const struct WarpTarget gData_020021ec;

extern u8 MsgTheDoorIs;
extern u8 MsgItHasBeen;
extern u8 MsgEveryoneSpoilsMe;
extern u8 MsgWeAreThe;
extern u8 MsgAreThereOther;

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void Func_02001cf4_(s32 a, s32 b, s32 c) {
  Func_02001cf4(a, b, c);
}

void Func_020005ac(s32 a) { Func_02001cf4_(a, 64, 0x300); }

void Func_020005bc(s32 a) { Func_02001cf4_(a, 65, 0x301); }

void Func_020005cc(s32 a) { Func_02001cf4_(a, 66, 0x302); }

void Func_020005dc(s32 a) { Func_02001cf4_(a, 67, 0x303); }

void Func_020005ec(s32 a) { Func_02001cf4_(a, 68, 0x304); }

void Func_020005fc(s32 a) { Func_02001cf4_(a, 70, 0x306); }

void Func_0200060c(s32 a) { Func_02001cf4_(a, 71, 0x307); }

void Func_0200061c(s32 a) { Func_02001cf4_(a, 72, 0x308); }
