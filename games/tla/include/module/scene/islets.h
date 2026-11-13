#ifndef ALCHEMY_MODULE_SCENE_ISLETS_H
#define ALCHEMY_MODULE_SCENE_ISLETS_H

/// @file
/// What The Lost Age's code declares for the files that use it, written once.

#include "types.h"

struct Actor;

void Func_020000d8(void);

void Func_02000210(void);
void Func_020002ac(void);
void Func_02000380(void);
void Func_02000440(void);

void Func_02000568(void);

void Func_020014e0(void);
void Func_02001a68(void);

void Func_02001bf4(void);

void Func_02001c38(void);
void Func_0200208c(void);
void Func_020020b4(void);
void Func_020020d0(void);
void Func_02002144(void);
void Func_020024c8(void);
void Func_020024f0(void);

void Func_0200253c(void);

void Func_020025a4(void);
void Func_020025b0(void);

void Func_020025bc(void);
void Func_020025dc(void);
void Func_0200261c(void);
void Func_0200265c(void);

void Func_0200267c(void);
void Func_0200274c(void);

void Func_02003620(void);
void Func_0200367c(void);

extern u32 gData_020040d4[];
extern u32 gData_02004198[];
extern u32 gData_02004640[];
extern s16 gData_020046a4[];
extern s16 gData_020046ba[];

void Func_020034fc(void);

void Field_TryJumpForward();

void Func_02003790(s32 layer, s32 x, s32 z, u32 width, u32 height, s32 value);
struct Actor *Func_02003940(s32 *direction, s32 *number, s32 *kind);

void Func_02001c4c(void);

struct Actor;

struct ActionScript {
  u32 words[9];
};

struct IsletsMapWork {
  u8 unk000[0x190];
  s32 value190;
  s32 value194;
};

void Import_ActorDestroy(struct Actor *object);
void Import_FieldPsynergyGreyMindReadPalette(void);
void Import_FieldPsynergyRestoreMindReadPalette(void);
void Import_FieldCameraWaitForArrival(void);
void Func_02003fec();

s32 Func_02003684(s32 id);

void Func_02003f4c(s32 actor);

#endif
