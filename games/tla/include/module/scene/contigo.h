#ifndef ALCHEMY_MODULE_SCENE_CONTIGO_H
#define ALCHEMY_MODULE_SCENE_CONTIGO_H

/// @file
/// What The Lost Age's code declares for the files that use it, written once.

#include "types.h"
#include "scene_tables.h"

extern s32 gData_0200474c[];
extern struct ActorPlacement gData_02004c88[];
extern struct ActorPlacement gData_02004d90[];
extern struct ActorPlacement gData_02005018[];
extern s16 gData_02005228[];
extern struct ActorPlacement gData_02005318[];
extern struct ActorPlacement gData_02005390[];
extern struct Trigger gData_020054ec[];
extern struct Trigger gData_02005a44[];
extern struct Trigger gData_02005e70[];

void Contigo_StartSlowPaletteCycle(void);

void Contigo_StopPaletteCycle(void);

extern u32 gData_02004630[];
extern s32 gData_0200466c[];
extern u32 gData_02004b18[];
void Func_02003720(void);
void Func_02003880(void);
void Func_020039d4(void);
void Func_02003a8c(void);
extern s16 gData_0200469c[];
extern s16 gData_020046ac[];
extern s16 gData_020046d8[];
extern u16 gData_02004704[];
extern s32 gData_02004a1c[];
extern s32 gData_020042bc[];
void Func_02000608(void);
void Func_020008ec(void);
void Func_0200090c(void);

void Func_0200092c(void);

void Func_0200094c(void);
void Scene_ShowEntranceLocked(void);
void Func_020009ac(void);
void Func_02000af4(void);
void Contigo_TalkToWeaponMerchant(void);
void Contigo_TalkToArmorMerchant(void);

void Contigo_TalkToInnkeeper(void);

void Func_02000eb8(void);
void Func_02000edc(void);
void Func_02000f40(void);
void Func_02000f7c(void);
void Func_02000fc8(void);
void Func_02000fe0(void);
void Func_0200102c(void);
void Func_02001070(void);
void Func_02001094(void);
void Func_0200118c(void);
void Func_02001284(void);
void Func_020012a4(void);

void Func_020012c4(void);

void Func_02001354(void);
void Func_02001374(void);
void Func_02001394(void);
void Func_020013b4(void);
void Func_020013d4(void);
void Func_020013f4(void);
void Func_02001414(void);
void Func_02001434(void);
void Func_02001454(void);
void Func_02001474(void);
void Func_02001494(void);
void Func_020014b4(void);
void Func_020014d4(void);
void Func_0200152c(void);

void Func_020015ac(void);

void Func_02001abc(void);
void Func_02001b84(void);
void Func_02001d08(void);
void Func_02001d38(void);
void Func_02002e18(void);
extern u32 gData_0200457c[];
extern u32 gData_020045b8[];
extern u32 gData_020045f4[];
extern u32 gData_02004758[];
extern u32 gData_020047fc[];
extern s32 gData_02004b54[];

extern u16 *gPaletteSlots;
extern u16 *gPaletteCycleColors;

void Func_02000328(void);

extern u16 gPaletteCycleBright;
extern u16 gPaletteCycleTimer;
extern u16 gPaletteCycleDelay;
extern u16 gPaletteCycleLength;
extern u16 gPaletteCycleIndex;

s32 List_CountEntries(u16 *list);

void PaletteCycle_Fade(void);

#endif
