#ifndef ALCHEMY_MODULE_SCENE_GABOMBA_STATUE_H
#define ALCHEMY_MODULE_SCENE_GABOMBA_STATUE_H

/// @file
/// What The Lost Age's code declares for the files that use it, written once.

#include "types.h"

struct Actor;
struct Sprite;

struct Unk02004d48 {
  u8 unk00[8];
  s32 x;
  u8 unk0c[76 - 12];
  u32 unk4c;
  u8 unk50[98 - 80];
  u8 unk62;
  u8 unk63;
  u16 unk64;
  u16 unk66;
};

void Func_020020f0(s32 value);

void Func_0200533c(void);
s32 Func_02004d48(s32 a, s32 b);

void Func_02000740(void);

void Func_02000978(void);

void Func_02000ad4(void);

void Func_02000c38(void);

void Func_02000c8c(void);

void Func_02000fc0(void);

void Func_02001284(void);

void Func_020012ec(void);

void Func_02001364(void);

void Func_02001430(void);
void Func_02001498(void);

void Func_02001500(void);

void Func_020015b8(void);

void Func_02001630(void);

void Func_0200176c(void);
void Func_020019d0(void);

void Func_02004530(void);
void Func_020045d8(void);
void Func_02004620(void);
void Func_020046c8(void);

extern s32 gData_02005fbc[];
extern s32 gData_02005fc8[];
extern u32 gData_02005b1c[];
extern u32 gData_02005b60[];
extern u32 gData_02005ba4[];
extern u32 gData_02005be8[];
extern s32 gData_02005c74[];
extern s32 gData_02005ca0[];

extern u32 gData_02005944[];

extern u32 gData_02005ccc[];
extern u32 gData_02005d24[];

void Func_02005364(void);

void Func_02001b38(void);
void Func_02001b68(void);

s32 Func_02003604(void);
s32 Func_020048a4(s32 x, s32 z, s32 value, s32 id);
void Func_02004f40(u32 *x_out, u32 *z_out);

void Func_02005644(struct Actor *object);

extern u32 gData_02005c40[];

void Func_02001c10(void);

void Func_020034ec(void);
void Func_020034f4(void);
void Func_0200359c(void);
void Func_020035f0(void);
void Func_02003634(void);
void Func_02003868(void);
void Func_020052a4();
void Func_020053ac();
s32 Func_02003fc0(s32 x0, s32 z0, s32 x1, s32 z1, s32 a, s32 b, s32 c, s32 d);
void Func_02004270(s32 col, s32 row, s32 a, s32 b);
s32 abs(s32);
s32 Func_02004224(s32 from, s32 to, s32 kind);
void Func_02004288(s32 id, s32 value);

void Func_020055ec(struct Sprite *oam, s32 depth);

void Func_020038ac(void);
void Func_02003974(void);
s32 Func_02003a64(s32 value);

void Func_02005654();

void Func_02005210(void);
void Func_0200525c(void);
extern char ResourceId_ClockDigits;
void Func_020054ac(s32 id, u32 mode, s32 flag, s32 layer);

s32 Func_02005844(struct Actor *object);

void Func_02002074(void);

#endif
