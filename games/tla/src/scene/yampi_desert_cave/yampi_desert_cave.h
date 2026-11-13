#ifndef TLA_YAMPI_DESERT_CAVE_H
#define TLA_YAMPI_DESERT_CAVE_H

#include "types.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "event_inline.h"

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

s32 Func_020001d4(s32 *position);
void Func_02000458(struct YampiDesertCaveRect rect);
void Func_02000898(void);
void Func_020008fc(void);
s32 Func_02000978(u16 *list);
void Func_0200099c(void);
void Func_02000bc0(u16 *list, s32 mode, s32 count);
void Event_SetDestination(s32 scene, s32 entrance);
void Actor_ReturnHome(s32 a, s32 b, s32 c);
void Func_02001958(void);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static inline void YampiDesertCave_Call1920(s32 a, s32 b, s32 c) {
  Actor_ReturnHome(a, b, c);
}

#endif
