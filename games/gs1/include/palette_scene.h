#ifndef ALCHEMY_PALETTE_SCENE_H
#define ALCHEMY_PALETTE_SCENE_H

#include "types.h"

u8 *Func_02000030(void);
u8 *Func_02000038(void);
u8 *Func_02000040(void);
u8 *Func_02000048(void);
void Func_02000050(void);
u8 *Func_02000080(void);
void Func_02000248(void);
void Func_02000284(void);
void Func_020002c0(void);
s32 Func_020011e8();
void Func_02001218(void);
void Func_020012b4(void);
void Func_0200152c();
void Func_0200172c(s32 key, s32 value);
void Func_02001768(s32 adjustment);
u16 Func_020017d0(u16 color, s32 adjustment);

#define PaletteScene_GetScriptData       Func_02000030
#define PaletteScene_GetMessageData      Func_02000038
#define PaletteScene_GetActorData        Func_02000040
#define PaletteScene_GetEffectData       Func_02000048
#define PaletteScene_Initialize          Func_02000050
#define PaletteScene_GetState            Func_02000080
#define PaletteScene_RunActorNineBranch  Func_02000248
#define PaletteScene_RunActorEightBranch Func_02000284
#define PaletteScene_RunFlaggedBranch    Func_020002c0
#define PaletteScene_AdvanceEffectFrame  Func_020011e8
#define PaletteScene_SpawnEffect         Func_02001218
#define PaletteScene_AdvanceTransition   Func_020012b4
#define PaletteScene_AdvanceOrbit        Func_0200152c
#define PaletteScene_SetRecordValue      Func_0200172c
#define PaletteScene_AdjustPaletteWindow Func_02001768
#define PaletteScene_AdjustColor         Func_020017d0

#endif
