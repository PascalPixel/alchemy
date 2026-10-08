//--------------------------------------------------------------------------
// ACTOR_RESET_RUNTIME_FIELDS.C
// バトル：状態変化の消去と、メッセージ送りの待ち
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
#include "CALLBACK_SCHEDULER.H"
#include "TYPES.H"
#include "BATTLE_STATUS_ICON.H"
#include "SCENE.H"
#include "IO_REG.H"
#include "IO_WRITE_QUEUE.H"
#include "SYSTEM.H"
#include "RESOURCE.H"
#include "FIXED_MATH.H"
#include "UI.H"
#include "BTLSYS.H"
#include "BTLWORK.H"
#include "MENU_LIST.H"

//--------------------------------------------------------------------------
// 状態異常と能力の変化をすべて消す
// 戦闘不能になったときに呼ばれる
//--------------------------------------------------------------------------
s32 BattleActor_ResetRuntimeFields(s32 id)
{
	struct BtlUnit *unit;
	s8			   *lv;
	s32				n;
	u8				zero;

	unit = UnitGetFar(id);
	n = 3;
	zero = 0;
	lv = &unit->elm_lv[3];
	do
	{
		n--;
		*lv-- = zero;
	} while (n >= 0);

	unit->doku = 0;
	unit->atk_cnt = 0;
	unit->atk_lv = 0;
	unit->def_cnt = 0;
	unit->def_lv = 0;
	unit->res_cnt = 0;
	unit->res_lv = 0;
	unit->maboroshi = 0;
	unit->konran = 0;
	unit->charm = 0;
	unit->shibire = 0;
	unit->nemuri = 0;
	unit->fuuji = 0;
	unit->refrain = 0;
	unit->reflect = 0;
	unit->akuryo = 0;
	unit->shiryo = 0;
	unit->unknown_142[0] = 0;
	unit->unknown_142[1] = 0;
	unit->sonae = 0;
	unit->miugoki = 0;
	unit->agi_cnt = 0;
	unit->agi_lv = 0;
	unit->chouhatsu = 0;

	UnitCalcFar(id);
	return BattleUnit_BuildStatusFlags(id, BtlObjGet(id));
}

//--------------------------------------------------------------------------
// メッセージ送りの矢印
//--------------------------------------------------------------------------
#define KEY_OKURI (KEY_A | KEY_B | KEYS_SHOULDERS) // 送りのキー

// OAM の一枚
struct AdvanceSprite
{
	struct AdvanceSprite *next;
	u16					  attr0;
	u16					  attr1;
	u16					  attr2;
	u16					  attr3;
};

extern volatile u32 gFrameCount;
extern volatile u32 gKeysHeld;
void				Audio_PlayCue(s32 cue);

//--------------------------------------------------------------------------
// ウィンドウの位置に矢印を出し、送りのキーを待つ
// B を押し続けるか、送りのキーを押すと進む
// 16 フレームを過ぎれば、押し続けていても進む
//--------------------------------------------------------------------------
s32 BattlePresentation_WaitForAdvance(void)
{
	struct AdvanceSprite	   *spr;
	struct AdvanceSprite		sprite;
	s32							frame;
	s32							slot;
	s32							pat;
	struct UiWindow			   *window;
	struct BattleDisplayOffset *offset;
	s32							attr2;
	s32							attr1;
	s32							x;
	s16							tile;

	while (!UiWork_IsCompleteFar())
		WaitFrames(1);
	spr = &sprite;
	slot = Resource_LoadIntoFreeSlot(128);
	frame = 0;
loop:
	// 4 フレームごとに 8 枚のパターンを回す
	pat = ((gFrameCount >> 2) & 7) * 128 + (s32)BattlePres_AdvanceArrowTiles;
	window = gBattleDisplayWork->window;
	offset = gBattleDisplayWork->offset;
	QueueIoWriteDelay10((u32)&REG_WINOUT, 4);
	QueueIoWriteDelay6((u32)&REG_WINOUT, 16);
	/* FAKEMATCH: the one-pass loop keeps the BLDALPHA address load after the value. */
	do
	{
		u16 blend = 16;
		REG_BLDALPHA = blend;
	} while (0);
	*(u32 *)&spr->attr0 = 0xa400;
	*(u32 *)&spr->attr2 = 0;
	tile = Resource_GetBuffer(slot, pat) & 0x3ff;
	attr2 = spr->attr2;
	spr->attr2 = (attr2 & ~0x3ff) | tile;
	// ウィンドウの位置はキャラ単位、オフセットは 1/256 ドット
	x = (u16)((window->x * 8 + (offset->x >> 8) + 4) & 0x1ff);
	attr1 = spr->attr1;
	spr->attr1 = (attr1 & ~0x1ff) | x;
	// 上下に 2 ドットまでゆれる
	*(u8 *)&spr->attr0 = Trig_Sin(gFrameCount << 12) / 32768 + window->y * 8 + (offset->y >> 8) + 6;
	Runtime_PushSlotEntry(spr, 240);
	if (!(gKeysHeld & KEY_B) && !(gKeyState & KEY_OKURI)
		&& (frame <= 15 || !(gKeysHeld & KEY_OKURI)))
	{
		WaitFrames(1);
		frame++;
		goto loop;
	}
	Audio_PlayCue(111);
	Resource_ResetEntry(slot);
	WaitFrames(1);
}

//--------------------------------------------------------------------------
// 指定の位置でカーソルを動かし、送りのキーを待つ
// x, y : ドット
//--------------------------------------------------------------------------
s32 BattlePresentation_WaitForPromptAt(s32 x, s32 y)
{
	struct MenuPoint	 pos;
	s32					 pat;
	struct CursorSprite	 sprite;
	struct CursorSprite *spr;
	s32					 slot;

	pat = Ui_GetTableWordZeroFar(0);
	pos.x = x;
	pos.y = y;
	while (!UiWork_IsCompleteFar())
		WaitFrames(1);
	spr = &sprite;
	slot = Resource_LoadIntoFreeSlot(128);
	while (1)
	{
		QueueIoWriteDelay10((u32)&REG_WINOUT, 4);
		QueueIoWriteDelay6((u32)&REG_WINOUT, 16);
		/* FAKEMATCH: the one-pass loop keeps the BLDALPHA address load after the value. */
		do
		{
			u16 blend = 16;
			REG_BLDALPHA = blend;
		} while (0);
		((s32 *)spr)[1] = MENU_SPRITE_SIZE_16;
		((s32 *)spr)[2] = 0;
		spr->tile = Resource_GetBuffer(slot, pat);
		// 4 フレームごとに右上へゆらす
		spr->x = pos.x + ((gFrameCount & 4) >> 1) - 4;
		spr->y = pos.y - ((gFrameCount & 4) >> 2) - 8;
		Runtime_PushSlotEntry(spr, 240);
		if (gKeyState & KEY_OKURI)
			break;
		WaitFrames(1);
	}
	Resource_ResetEntry(slot);
	WaitFrames(1);
}
