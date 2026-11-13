#ifndef ALCHEMY_MODULE_SCENE_SHRINE_OF_THE_SEA_GOD_H
#define ALCHEMY_MODULE_SCENE_SHRINE_OF_THE_SEA_GOD_H

/// @file
/// What The Lost Age's code declares for the files that use it, written once.

#include "types.h"

struct ShrineOfTheSeaGodObject;

/// A word of the object written through a one-member union (see the
/// FAKEMATCH(signature) tag in Func_02001d24).
union ObjectWord {
  s32 value;
};

struct ShrineOfTheSeaGodSpot {
  s32 x;
  s32 z;
  s32 field18;
  s32 rotation;
};

struct ShrineOfTheSeaGodEffectParams {
  u8 unk00[0x18];
  u16 kind;
  u8 unk1a[0xe];
};

struct ShrineOfTheSeaGodPartyExtra {
  u8 unk000[0x256];
  s16 value;
};

void Func_020025fc(void);
void Func_02003854(s32 actor);

void Func_020004e0(void);

void Func_02000554(void);

void Func_02000688(void);

void Func_02000e38(void);

void Func_02000f34(void);

void Func_02000f58(void);

void Func_02000f80(void);

void Func_02000fa8(void);

void Func_02000fb8(void);

void Func_02000fec(void);

void Func_02001290(void);

void Func_020012b4(void);

void Func_02001474(void);

void Func_020016d8(void);

void Func_02001a0c(void);
void Func_02001a3c(void);
void Func_02001d24(void);
void Func_02002384(void);

void Func_02002430(void);

void Func_020024e4(void);

void Func_020026f8(void);
void Func_02002d50(void);
void Func_02002e8c(void);
void Func_02002ea8(void);

extern u32 gData_02003b8c[];

void Func_02001658(struct ShrineOfTheSeaGodObject *object, s32 base);
void Func_020016c8(void);

void Func_020018ac(void);

void Func_02003b00(s32 value);
void Func_02003b18(void);

void Func_0200123c(void);
void Func_020010e8(void);
void Func_02001088(void);

void Func_020011a0(void);

void Func_02000f18(s32 actor);

void Func_02003094(void);
void Func_02003134(void);

void Func_0200320c(void);

void Func_02003354(void);
void Func_0200338c(void);

void Func_02003504(void);

void Func_02003598(void);
void Func_02003740(void);

void Func_0200377c(void);

void Func_0200380c(void);

void Func_02000994(void);

void Func_02000ce8(void);

#endif
