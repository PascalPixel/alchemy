#ifndef TLA_YAMPI_DESERT_CAVE_H
#define TLA_YAMPI_DESERT_CAVE_H

#include "types.h"
#include "adventure.h"
#include "ram_buffer.h"

extern char SceneId_Ov662Room1;
extern char SceneId_Ov662Room2;
extern char SceneId_Ov662Room3;

struct YampiDesertCaveRect {
  s32 word[6];
};

extern u16 gData_02001fc8;
extern u16 gData_02001fd0;
extern u16 gData_02001fd4;
extern u16 *gData_02001fd8;
extern u16 gData_02001fe4;
extern u16 gData_02001fe8;
extern u32 gData_02001b58[];
extern s32 gData_02001bd0[];
extern s32 gData_02001c18[];
extern const u8 gData_02001d20[];
extern const u8 gData_02001d28[];
extern const u8 gData_02001ddc[];
extern const u8 gData_02001e78[];

s32 Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 priority);
void FieldEvent_WaitFrames(s32 frames);
s32 Func_020001d4(struct YampiDesertCaveRect *rect);
void Func_02000458(struct YampiDesertCaveRect rect);
void Func_02000898(void);
void Func_020008fc(void);
s32 Func_02000978(u16 *list);
void Func_0200099c(void);
void Func_02000bc0(const u8 *a, s32 b, s32 c);
void Func_02001848(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_020018d8(s32 scene, s32 entrance);
void Func_02001920(s32 a, s32 b, s32 c);
void Func_02001958(void);

static inline void YampiDesertCave_Call1848(s32 a, s32 b, s32 c, s32 d, s32 e,
                                            s32 f) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02001848(a, b, c, d, e, f);
}

static inline void YampiDesertCave_Call1920(s32 a, s32 b, s32 c) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02001920(a, b, c);
}

#endif
