#ifndef ALCHEMY_MODULE_SCENE_MARS_LIGHTHOUSE_ENTRANCE_H
#define ALCHEMY_MODULE_SCENE_MARS_LIGHTHOUSE_ENTRANCE_H

/// @file
/// What The Lost Age's code declares for the files that use it, written once.

#include "types.h"
#include "field/cell_target.h"

struct Actor;
struct CellTarget;
struct CellTargetEntry;
struct MapCell;
struct Position;

struct MarsLighthouseEntranceAnimation {
  u8 unk00[9];
  u8 unused09 : 2;
  u8 layer : 2;
  u8 unk0a[6];
  u8 value10;
  u8 flag11 : 1;
  u16 angle;
};

struct MarsLighthouseEntranceRideHead {
  s32 counter;
  s16 active;
  u8 unk06[6];
  u16 value0c;
  s16 value0e;
  u8 unk10[0x60];
  struct Actor *handle;
};

struct MarsLighthouseEntranceEventWork {
  u8 unk000[0x1ac];
  s32 value1ac;
};

void Leader_KnockBack(void);

void Func_02001c74();

s32 MapCell_Swap(u32 layer, s32 x, s32 y, s32 value);

void Func_02000798(void);

void Func_020019c4(void);

void Func_020046b4(void);
void Func_02004718(void);

void Func_02001a00(void);

void Func_020019e0(void);

void Func_02000fe0(const struct MapCell *cell);

void Func_020012e4(struct Actor *object);

void Actor_Attach(s32 id, s32 a1, s32 a2, s32 a3, const void *script);

void Leader_SlideOnIceFromEdge(void);

s32 Func_020029a0(void);

void Leader_SlideOnIceAhead(void);

void Func_02000d18(void);

s32 Func_020029bc(void);

void Leader_SetSlideStopped(s32 value);

void Func_02000414(s32 value);

void Func_02001988(void);
void Func_020019a4(void);

s32 Actor_IsTouching(struct Actor *a, struct Actor *b);

s32 Actor_IsBeside(struct Actor *a, struct Actor *b);
void Func_02001e60();
void Func_02001ea8();
void Func_02001ef0();

void Actor_RollAndCrumble(struct Actor *actor);

s32 Func_02004c78(s32 *a, s32 radiusA, struct Position *b, s32 radiusB);

extern struct CellTarget Data_020068b4;

void CrackedTile_Crack(struct CellTarget *table, struct CellTargetEntry *entry);

extern u16 *Data_02006894;
extern u16 *Data_02006880;

void Func_020048e8(void);

extern u16 gData_0200687c;
extern u16 gData_02006884;
extern u16 gData_0200689c;
extern u16 gData_0200e888;

s32 List_CountEntries(u16 *list);
void PaletteCycle_Fade(void);
void Actor_SetCellOpen(s32 id, u32 kind, s32 flag, s32 layer);

s32 Func_02004db8(struct Actor *object);

void Func_02000808(void);

void Func_02000a24(void);

void Func_02000c1c(void);

void Func_02000c68(void);

void MarsLighthouseEntrance_StartPoundIceCone(void);

void MarsLighthouseEntrance_CheckIce(void);

void MarsLighthouseEntrance_Teleport(void);

void Func_02000710(void);

void Func_02000930(void);

void Func_0200093c(void);

void Func_020009c0(void);

void Func_020009e8(void);

void Func_02000a74(void);

void Func_02000a7c(void);

void Func_02000af4(void);

void Func_02000b54(void);

void Func_02000c84(void);

void Func_02000cb4(void);

void Func_02000fa4(void);

void Func_02000fd4(void);

int MarsLighthouseEntrance_SlideOnIce(void);
void Leader_SlideOnIce(void);

void Leader_StopIfPending(void);

void Func_02000b24(void);

#endif
