#ifndef ALCHEMY_DISPLAY_TRANSITION_H
#define ALCHEMY_DISPLAY_TRANSITION_H

#include "edition.h"
#include "types.h"
#include "layout_guard.h"

struct DisplayTransitionFrame
{
	u8	unk000[0x508];
	u8	paletteNibbles[0x22];
	u16 value;
	u8	unk52c[13];
	u8	ditherToggle;
	s8	start;
	s8	end;
	s8	duration;
	s8	step;
};

struct DisplayTransitionWindow
{
	u32 unk000[5];
	u16 dispcnt;
	u8	unk016[0xea];
	u16 firstLine;
	u16 secondLine;
	u16 irqFirstLine;
	u16 irqSecondLine;
	u16 irqPhase;
};

struct DisplayTransitionState
{
	u16 pages[2][322];
	u32 solidTile[8];
	u16 mode;
	u16 value;
	s32 centreX;
	s32 centreY;
	s16 mask;
	s16 active;
	u8	unk538;
	u8	page;
	s8	start;
	s8	end;
	s8	duration;
	s8	step;
	s8	nextMode;
};

LAYOUT_OFFSET_GUARD(DisplayTransitionFrame_Value, struct DisplayTransitionFrame, value, 0x52a);
LAYOUT_OFFSET_GUARD(DisplayTransitionFrame_Start, struct DisplayTransitionFrame, start, 0x53a);
LAYOUT_SIZE_GUARD(DisplayTransitionFrame_Size, struct DisplayTransitionFrame, 0x540);
LAYOUT_OFFSET_GUARD(DisplayTransitionWindow_Control, struct DisplayTransitionWindow, dispcnt, 0x14);
LAYOUT_OFFSET_GUARD(
	DisplayTransitionWindow_FirstLine, struct DisplayTransitionWindow, firstLine, 0x100);
LAYOUT_OFFSET_GUARD(
	DisplayTransitionWindow_IrqPhase, struct DisplayTransitionWindow, irqPhase, 0x108);
LAYOUT_SIZE_GUARD(DisplayTransitionWindow_PrefixSize, struct DisplayTransitionWindow, 0x10c);
LAYOUT_OFFSET_GUARD(DisplayTransitionState_Pages, struct DisplayTransitionState, pages, 0);
LAYOUT_OFFSET_GUARD(
	DisplayTransitionState_SolidTile, struct DisplayTransitionState, solidTile, 0x508);
LAYOUT_OFFSET_GUARD(DisplayTransitionState_Page, struct DisplayTransitionState, page, 0x539);
LAYOUT_OFFSET_GUARD(DisplayTransitionState_Mode, struct DisplayTransitionState, mode, 0x528);
LAYOUT_OFFSET_GUARD(DisplayTransitionState_Centre, struct DisplayTransitionState, centreX, 0x52c);
LAYOUT_OFFSET_GUARD(DisplayTransitionState_Mask, struct DisplayTransitionState, mask, 0x534);
LAYOUT_OFFSET_GUARD(
	DisplayTransitionState_NextMode, struct DisplayTransitionState, nextMode, 0x53e);
LAYOUT_OFFSET_GUARD(
	DisplayTransitionState_Duration, struct DisplayTransitionState, duration, 0x53c);
LAYOUT_SIZE_GUARD(DisplayTransitionState_Size, struct DisplayTransitionState, 0x540);

#if GAME_TLA
void						   DisplayTransition_InitializeBattleEffectState(s32 mode);
void						   BattleFx_EnableTwoCallbacks(void);
struct DisplayTransitionState *DisplayTransition_AllocateAndClearState(void);
void						   DisplayTransition_Start(s32 mode, s32 frames);
void						   DisplayTransition_Finish(s32 mode, s32 frames);
void DisplayState_ClearFlags(s32 clear_0800, s32 clear_0400, s32 clear_0200);
void DisplayTransition_Update(void);
void DisplayTransition_UpdateFromCentre(void);
void DisplayTransition_UpdateScanline(void);
void DisplayTransition_UpdateFrame(void);
void DisplayTransition_FillTilemapAndSolidTile(s32 color);
void DisplayTransition_InitializeState(s32 mode);
void DisplayTransition_UpdateScanlineTable(void);
void BattleFx_StartWindowHBlankDma(void);
#endif

#endif
