#ifndef SETTLEMENTS_LOCAL_H
#define SETTLEMENTS_LOCAL_H

#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "scene_tables.h"
#include "iwram_call.h"
#include "scene_ids.h"
#include "event_inline.h"

s32 Math_Cosine(s32 angle);

extern u8 MsgSettlements01[];

#include "frame.h"

extern s16 gData_0200226c[];
extern u16 gData_02002ce8;

void Func_02000758(s32 x, s32 y, s32 z);
void Func_02000d14(void);
void Func_02000ed0(void);
void Func_02000f2c(void);
void Func_02000fd0(void);
void Func_0200122c(void);
void Func_0200138c(void);
void Func_02001770(void);
void Func_02002118(void);
void Func_02002148(void);
void Func_02002168(s32 a);
void Func_02002178(s32 a, s32 b, s32 c);
void Func_02002190(const void *data);
void Func_02002198(void);
void Func_0200052c(void);
void Func_02000714(struct Actor *object);
void Func_02001fe0(struct Actor *object, s32 a);
void Func_02002000(struct Actor *object);
void Animation_SetStateFlags(struct Actor *object, u32 value);
extern s32 gData_02002ce0;
extern u16 gData_02002ce4;
extern u16 gData_02002ce6;
void Func_02000e30(void);
void Func_02000e90(void);
void Func_02001fa8(s32 a, s32 b, void (*callback)(void));
void Func_02002068(void);
void Func_02002070(void);
void Func_02002130(s32 a, s32 b);
void Func_020021a0(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g);

extern u32 gData_02002380[];
extern u32 gData_020023a0[];

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void CommitPos(s32 id, s32 x, s32 z) {
  Actor_CommitPositionAndActivate(id, x, z);
}

static __inline__ void Window(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g) {
  Func_020021a0(a, b, c, d, e, f, g);
}

static __inline__ void CopyCells(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f) {
  Map_CopyCellPictureIndicesRect(a, b, c, d, e, f);
}

static __inline__ void CopyCells2(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f) {
  Map_CopyCellAttributeRect(a, b, c, d, e, f);
}

static __inline__ void SetPosCommit(s32 id, s32 x, s32 z) {
  Actor_SetPositionAndCommit(id, x, z);
}

static __inline__ s32 Flag_Test(s32 flag) { return GameFlag_Test(flag); }

static __inline__ void Flag_Set(s32 flag) { GameFlag_SetBit(flag); }

#endif
