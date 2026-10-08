//--------------------------------------------------------------------------
// MARKERS.C
// 戦闘エフェクト 8 番：選択中のオブジェクトを中心に画面を切りかえ、印を出す
//--------------------------------------------------------------------------

#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "GAME_STATE.H"
#include "TYPES.H"
#include "CALLBACK_SCHEDULER.H"
#include "DISPTRAN.H"

#define Data_03001ebc (*(struct EffectGlobals *)&Data_03001ebc)
#include "OBJECT_LOOKUP.H"
#define ObjectTable_Get ((struct EffectTarget * (*)(s32)) ObjectTable_Get)

#define BattleFx_RunEventAction ((void (*)(s32, s32, s32))BattleFx_RunEventAction)

// イベントのワークと画面切りかえのワークを兼ねる
struct EffectState
{
	u8	unknown_000[0x52a];
	u16 frame;	// DisplayTransitionState の value
	s32 x;		// 同じく centre_x (16.16 固定小数)
	s32 height; // 同じく centre_y
	u8	unknown_534[0xcb8 - 0x534];
	u16 active;
	u16 duration;
	s16 center_x; // 中心 (ドット)
	s16 center_y;
	u16 step;
};
struct EffectGlobals
{
	struct EffectState *scene; // イベントのワーク
	u8					unknown_04[12];
	struct EffectState *work; // 画面切りかえのワーク
};
struct EffectTarget
{
	u8	unknown_00[8];
	s32 x; // 16.16 固定小数
	s32 y;
	s32 z;
};
void BattleFx_ApplyColorToSourceBuffer(s32, s32);
void WaitFrames(s32);
s32	 BattleFx_FindMatchingEvent(s32, s32, s32 *);
void Audio_PlayCue(s32);

//--------------------------------------------------------------------------
// 戦闘エフェクト 8 番
// 選択中のオブジェクトの位置を中心に画面切りかえを始め、
// 合うイベントがあれば動かしてから、まわりに印を出す
//--------------------------------------------------------------------------
void RunBattleEffect08(void)
{
	struct EffectState	*scene;
	struct EffectState	*work;
	struct EffectTarget *target;
	s32					 arg;
	s32					 ev;
	s32					 frame;

	scene = Data_03001ebc.scene;
	ObjectDispatch_ApplyValueToKind200Children(6);
	DisplayTransition_InitializeBattleEffectState(8);
	work = Data_03001ebc.work;
	target = ObjectTable_Get(gGameState.selected_actor);
	work->x = target->x;
	work->height = target->z - target->y;
	BattleFx_ApplyColorToSourceBuffer(0x10000, 0);
	BattleFx_ApplyColorToTargetBuffer(0x10001, 1);
	BattleFx_StartBufferInterpolation(1);
#if !defined(TBS_EDITION_EN)
	scene->active = 0; // 英語版にはない
#endif
	WaitFrames(1);
	ev = BattleFx_FindMatchingEvent(0x50000005, 8, &arg);
	if (ev != 0)
		BattleFx_RunEventAction(ev, gGameState.selected_actor, arg);
	Audio_PlayCue(131);

	// イベントのワークに中心 (ドット) と時間を入れ、近くに印を出す
	scene->active = 1;
	scene->center_x = work->x / 0x10000;
	scene->center_y = work->height / 0x10000;
	scene->duration = 600;
	scene->step = 1;
	FieldEffect_SpawnNearbyMarkers();
	frame = 0;
	do
	{
		WaitFrames(1);
		work->frame = frame;
		frame++;
	} while (frame <= 18);
	Scheduler_AddOrUpdateCallback(FieldEffect_WatchLeaderDistance, 0xc80);
}
