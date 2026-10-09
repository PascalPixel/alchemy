#ifndef ALCHEMY_BATTLE_EFFECT_WORK_H
#define ALCHEMY_BATTLE_EFFECT_WORK_H

#include "effect_step.h"
#include "layout_guard.h"
#include "types.h"

struct BattleEffectArgument;

struct BattleEffectWork
{
	u8							 sheet[0x6980];
	s32							 bg2_x[160];
	u8							 unknown_6c00[0x480];
	struct EffectStep			 particles[64];
	s32							 transfer_mode;
	s32							 transfer_value;
	s32							 unknown_7788;
	s32							 frame;
	s32							 scroll_timer;
	s32							 scroll_interval;
	s32							 scroll_step_x;
	s32							 scroll_step_y;
	s32							 saved_bg1_x;
	s32							 saved_bg1_y;
	s32							 shake_frames;
	s32							 camera_delta;
	s32							 camera_phase;
	s32							 fade_frames;
	s32							 fade_step;
	u16							 win0h;
	u16							 win0v;
	u16							 win1h;
	u16							 win1v;
	u16							 winin;
	u16							 winout;
	u16							 dispcnt;
	u16							 bldcnt;
	u16							 bldalpha;
	u16							 padding_77ce;
	s32							 bg2x;
	s32							 bg2y;
	void						*objects[16];
	u8							 actor_timers[8];
	s32							 frames_since_transfer;
	s32							 transfer_pending;
	struct BattleEffectArgument *effect;
};

struct BattleEffectArgument
{
	s32 kind;
	s32 side;
	s32 actor;
	s32 unknown_000c;
	s32 unknown_0010;
	s32 count;
	s32 variant;
	s32 prelude;
	s32 unknown_0020;
	s16 actors[8];
};

LAYOUT_OFFSET_GUARD(BattleEffectWork_Bg2X, struct BattleEffectWork, bg2_x, 0x6980);
LAYOUT_OFFSET_GUARD(BattleEffectWork_Particles, struct BattleEffectWork, particles, 0x7080);
LAYOUT_OFFSET_GUARD(BattleEffectWork_TransferMode, struct BattleEffectWork, transfer_mode, 0x7780);
LAYOUT_OFFSET_GUARD(
	BattleEffectWork_TransferValue, struct BattleEffectWork, transfer_value, 0x7784);
LAYOUT_OFFSET_GUARD(BattleEffectWork_Frame, struct BattleEffectWork, frame, 0x778c);
LAYOUT_OFFSET_GUARD(BattleEffectWork_ScrollTimer, struct BattleEffectWork, scroll_timer, 0x7790);
LAYOUT_OFFSET_GUARD(BattleEffectWork_ScrollStepX, struct BattleEffectWork, scroll_step_x, 0x7798);
LAYOUT_OFFSET_GUARD(BattleEffectWork_SavedBg1X, struct BattleEffectWork, saved_bg1_x, 0x77a0);
LAYOUT_OFFSET_GUARD(BattleEffectWork_ShakeFrames, struct BattleEffectWork, shake_frames, 0x77a8);
LAYOUT_OFFSET_GUARD(BattleEffectWork_CameraDelta, struct BattleEffectWork, camera_delta, 0x77ac);
LAYOUT_OFFSET_GUARD(BattleEffectWork_FadeFrames, struct BattleEffectWork, fade_frames, 0x77b4);
LAYOUT_OFFSET_GUARD(BattleEffectWork_FadeStep, struct BattleEffectWork, fade_step, 0x77b8);
LAYOUT_OFFSET_GUARD(BattleEffectWork_Win0H, struct BattleEffectWork, win0h, 0x77bc);
LAYOUT_OFFSET_GUARD(BattleEffectWork_Bg2RefX, struct BattleEffectWork, bg2x, 0x77d0);
LAYOUT_OFFSET_GUARD(BattleEffectWork_Objects, struct BattleEffectWork, objects, 0x77d8);
LAYOUT_OFFSET_GUARD(BattleEffectWork_ActorTimers, struct BattleEffectWork, actor_timers, 0x7818);
LAYOUT_OFFSET_GUARD(
	BattleEffectWork_FramesSinceTransfer, struct BattleEffectWork, frames_since_transfer, 0x7820);
LAYOUT_OFFSET_GUARD(
	BattleEffectWork_TransferPending, struct BattleEffectWork, transfer_pending, 0x7824);
LAYOUT_OFFSET_GUARD(BattleEffectWork_Effect, struct BattleEffectWork, effect, 0x7828);
LAYOUT_SIZE_GUARD(BattleEffectWork_Size, struct BattleEffectWork, 0x782c);
LAYOUT_OFFSET_GUARD(BattleEffectArgument_Count, struct BattleEffectArgument, count, 0x14);
LAYOUT_OFFSET_GUARD(BattleEffectArgument_Actors, struct BattleEffectArgument, actors, 0x24);

#endif
