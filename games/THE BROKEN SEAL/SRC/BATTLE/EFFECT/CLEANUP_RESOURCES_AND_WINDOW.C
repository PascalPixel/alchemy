//--------------------------------------------------------------------------
// CLEANUP_RESOURCES_AND_WINDOW.C
// ワールドマップ：地名の印・カーソル・地名ウインドウ
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
#include "RESOURCE.H"
#include "EDITION.H"
#include "TYPES.H"
#include "SCENE.H"
#include "IO_REG.H"
#include "IO_WRITE_QUEUE.H"
#include "PARTY_STATE.H"
#include "RAM_BUFFER.H"
#include "BTLSYS.H"
#include "text/MSG_IDS.H"

#include "OBJECT_LOOKUP.H"
#define ObjectTable_Get ((struct MapObject * (*)(s32)) ObjectTable_Get)

extern u8 gMapCellBuffer[];

extern void UiWork_FinalizeFar(u32 arg0, u32 arg1);

#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) \
	|| defined(TBS_EDITION_IT)
// キーを離すとカーソルが近くの地名へ寄る
#define WORLD_MAP_CURSOR_SNAPS 1
#define WORLD_MAP_NAME_RANGE 64	 // 地名を出す距離の 2 乗
#define WORLD_MAP_NAME_LEFT 3	 // 地名ウインドウを印から左へずらす量
#else
#define WORLD_MAP_CURSOR_SNAPS 0
#define WORLD_MAP_NAME_RANGE 100
#define WORLD_MAP_NAME_LEFT 1
#endif

#define FLAG_NOT_ON_MAP 284 // いまいる場所が地図にのっていない

// OBJ の属性（2 ワード目からが OAM と同じ並び）
struct MapMarker
{
	struct MapMarker *link;
	u16				  y : 8;
	u16				  affine : 2;
	u16				  blend_mode : 2; // 1 で半透明
	u16				  mosaic : 1;
	u16				  full_color : 1;
	u16				  shape : 2;
	u16				  x : 9;
	u16				  affine_index : 5;
	u16				  size : 2;
	u16				  tile : 10;
	u16				  priority : 2;
	u16				  palette : 4;
	u16				  padding0a;
};

// 16.16 固定小数（整数部がドット）
union MapFixed
{
	s32 value;
	struct
	{
		u16 fraction;
		s16 whole;
	} part;
};

// ワールドマップの作業領域（gMapCellBuffer に置く）
struct WorldMapWork
{
	u16				 vram_block; // 印の OBJ キャラクタの VRAM ブロック
	u16				 padding02;	 // もうひとつの VRAM ブロック（終わりに返す）
	union MapFixed	 x;			 // カーソル（ドット）
	union MapFixed	 z;
	u8				 padding0c[6];
	s16				 shown; // いま地名ウインドウに出している印（-1 はなし）
	u8				 padding14[4];
	s32				 speed; // カーソルの速さ
	s32				 window;
	struct MapMarker markers[67];
};

struct MapObject
{
	u8			   padding00[8];
	s32			   x;
	u8			   padding0c[4];
	union MapFixed z;
};

//--------------------------------------------------------------------------
// ワールドマップを閉じる：VRAM ブロックを返し、地名ウインドウを消す
//--------------------------------------------------------------------------
void BattleFx_CleanupResourcesAndWindow(void)
{
	struct WorldMapWork *work = (struct WorldMapWork *)gMapCellBuffer;

	Resource_ResetEntry(work->vram_block);
	Resource_ResetEntry(work->padding02);
	UiWork_FinalizeFar(work->window, 2);
}

extern struct WorldMapVramBlock gVramBlockCache[];
#ifndef MODERN // modern GCC keeps the headers' volatile declaration
extern u32						gFrameCount;
#endif
extern u32						gKeysHeld;

void Vector_AddPolarOffset(s32 magnitude, s32 angle, s32 *position);
s32	 ArcTan2(s32 dz, s32 dx);

// IME を止めて I/O 書き込みキューに 1 件積む（QueueIoWriteDelay2 と同じ）
#define QUEUE_IO_WRITE_DELAY2(address, value)                                                         \
	do                                                                                                \
	{                                                                                                 \
		u32 saved;                                                                                    \
		s32 count;                                                                                    \
                                                                                                      \
		/* FAKEMATCH: a block of its own keeps the saved IME read first. */                           \
		do                                                                                            \
		{                                                                                             \
			/* FAKEMATCH: removing this one-pass boundary changes measured instruction scheduling. */ \
			saved = *ime;                                                                             \
		} while (0);                                                                                  \
		*ime = (u16)ime;                                                                              \
		count = q->count;                                                                             \
		if (count <= 31)                                                                              \
		{                                                                                             \
			u32 *destination = (u32 *)((u8 *)q + count * 12 + 4);                                     \
			*(u16 *)&q->count = count + 1;                                                            \
			*destination++ = (value);                                                                 \
			*destination++ = (address);                                                               \
			*destination = 0x20000;                                                                   \
		}                                                                                             \
		*ime = saved;                                                                                 \
	} while (0)

//--------------------------------------------------------------------------
// ワールドマップの毎フレーム処理（スケジューラのコールバック）
// フラグの立った場所に印を出し、十字キーでカーソルを動かし、
// カーソルにいちばん近い印の地名をウインドウに出す
// L・R を押している間はカーソルを現在地に合わせる
//--------------------------------------------------------------------------
void Map_UpdateWorldMapMarkers(void)
{
	s32					 leader;
	const s32			*place;
	s32					 tile_base;
	s32					 blend;
	s32					 best;
	s32					 best_dist;
	s32					 cursor_x;
	s32					 best_x;
	s32					 best_y;
	s32					 mes;
	s32					 best_mes;
	s32					 height;
	s32					 width;
	s32					 pos[3];
	struct WorldMapWork *work;
	struct MapObject	*obj;
	s32					 cursor_y;
	s32					 tile;
	s32					 mode;
	s32					 flag;
	s32					 i;
	struct MapMarker	*marker;
	s32					 x;
	s32					 y;
	s32					 angle;
	struct IoWriteQueue *q;
	volatile u16		*ime;

#if WORLD_MAP_CURSOR_SNAPS
	leader = gGameState.selected_actor;
	work = (struct WorldMapWork *)Ram_MapCellBuffer;
	marker = work->markers;
	place = WorldMap_PlaceMarkers;
	tile_base = gVramBlockCache[work->vram_block].offset >> 5;
#else
	work = (struct WorldMapWork *)Ram_MapCellBuffer;
	leader = gGameState.selected_actor;
	place = WorldMap_PlaceMarkers;
	tile_base = gVramBlockCache[work->vram_block].offset >> 5;
	marker = work->markers;
#endif
	best = -1;
	best_dist = WORLD_MAP_NAME_RANGE;
	blend = WorldMap_MarkerBlendCycle[(gFrameCount >> 1) & 31];

	//------------------------------------------------------------------
	// カーソル
	//------------------------------------------------------------------
	if (!GameFlag_TestFar(FLAG_NOT_ON_MAP) && (gKeysHeld & KEYS_SHOULDERS))
	{
		// L・R：現在地
		obj = ObjectTable_Get(gGameState.selected_actor);
		if (obj == NULL)
			goto markers;
		cursor_x = ((obj->x - 0x10000000) >> 16) * 240 / 4096;
		cursor_y = obj->z.part.whole * 160 / 4096;
	}
	else
	{
		// 十字キー：押し続けると速くなる
		angle = WorldMap_CursorDirectionAngles[(gKeysHeld >> 4) & 15];
		if (angle != 0xffff)
		{
			pos[0] = work->x.value;
			pos[1] = 0;
			pos[2] = work->z.value;
			Vector_AddPolarOffset(work->speed, angle, pos);
			if (pos[0] < 0x100000) // x は 16〜239 ドット
				pos[0] = 0x100000;
			if (pos[0] > 0xef0000)
				pos[0] = 0xef0000;
			if (pos[2] < 0) // y は 0〜141 ドット
				pos[2] = 0;
			if (pos[2] > 0x8d0000)
				pos[2] = 0x8d0000;
			work->x.value = pos[0];
			work->z.value = pos[2];
			if (work->speed < 0x60000)
				work->speed += 0x2000;
		}
		else
		{
			work->speed = 0x10000;
		}
		cursor_x = work->x.part.whole;
		cursor_y = work->z.part.whole;
	}

	//------------------------------------------------------------------
	// 印：0 番は現在地、11〜65 番は WorldMap_PlaceMarkers の場所
	// 表は（フラグ, 地名）の組で、フラグの上位 16bit はキャラクタ番号
	//------------------------------------------------------------------
markers:
	for (i = 0; i <= 65; i++)
	{
		if (i != leader && i <= 10)
			continue;
		if (i == 0)
		{
			if (GameFlag_TestFar(FLAG_NOT_ON_MAP))
				continue;
			tile = 0;
			flag = leader;
			mode = 0;
		}
		else
		{
			flag = *place++;
			mes = *place++;
			if (flag == 0)
				continue;
#if WORLD_MAP_CURSOR_SNAPS
			if (flag == -1)
				break;
			tile = flag >> 16;
			mode = 1;
#else
			tile = flag >> 16;
			mode = 1;
			if (flag == -1)
				break;
#endif
		}
		if (!GameFlag_TestFar(flag))
			continue;
		obj = ObjectTable_Get(i);
		if (obj == NULL)
			continue;

		// ワールドの 4096 ドット四方を 240x160 に縮める
		x = ((obj->x - 0x10000000) >> 16) * 240 / 4096;
		y = obj->z.part.whole * 160 / 4096;
		marker->blend_mode = mode;
		marker->tile = tile_base + tile;
		marker->x = x - 1;
		marker->y = y - 1;
		width = x - cursor_x;
		height = y - cursor_y;
		if (width * width + height * height < best_dist)
		{
			best_mes = mes;
			best = i;
			best_dist = width * width + height * height;
			best_x = x;
			best_y = y;
		}
		if (i != 0 || (gFrameCount & 15) <= 7) // 現在地は 8 フレームごとに点滅
			Runtime_PushSlotEntry(marker++, y);
	}
#if WORLD_MAP_CURSOR_SNAPS
	// キーを離していれば、カーソルをいちばん近い印へ寄せる
	if (gKeysHeld == 0)
	{
		if (best != -1)
		{
			if (best_dist <= 1)
			{
				work->x.value = best_x << 16;
				work->z.value = best_y << 16;
				work->speed = 0x10000;
			}
			else
			{
				if (work->speed < 0x20000)
					work->speed = 0x20000;
				angle = ArcTan2(best_y - cursor_y, best_x - cursor_x);
				pos[0] = work->x.value;
				pos[1] = 0;
				pos[2] = work->z.value;
				Vector_AddPolarOffset(work->speed, (u16)angle, pos);
				work->x.value = pos[0];
				work->z.value = pos[2];
				if (work->speed < 0x60000)
					work->speed += 0x2000;
			}
			cursor_x = work->x.part.whole;
			cursor_y = work->z.part.whole;
		}
		else
		{
			work->speed = 0x10000;
		}
	}
#endif

	// いちばん近い印を囲む枠（点滅）
	if (best != -1 && (gFrameCount & 15) <= 7)
	{
		marker->blend_mode = 0;
		marker->tile = tile_base + 3;
		marker->x = best_x - 2;
		marker->y = best_y - 2;
		Runtime_PushSlotEntry(marker, best_y);
	}

	// カーソル
	marker = &work->markers[66];
	marker->affine_index = 0;
	marker->x = cursor_x - 17;
	marker->y = cursor_y + 1;
	Runtime_PushSlotEntry(marker, 246);

	//------------------------------------------------------------------
	// 地名ウインドウ：印が変わったときだけ書き直す
	//------------------------------------------------------------------
	if (work->shown != best)
	{
		work->shown = best;
		UiWindow_Clear(work->window);
		if (best != -1)
		{
			if (best == 0)
				best_mes = (s32)MsgPresentLocation;
			else
				best_mes = Scene_GetAreaNameIndex(best_mes, 1) + (s32)MsgDebugEntryName;
			UiText_MeasureResourceEntriesFar(best_mes, &width, &height);
			cursor_x = best_x;
			cursor_y = best_y;
			cursor_x -= WORLD_MAP_NAME_LEFT;
			cursor_y -= 11;
			if (cursor_x + width > 240)
			{
				// 右へはみ出すなら、印の左上へ回す
				cursor_x = 232 - width;
				cursor_y = best_y - 20;
			}
			if (cursor_y < 0)
				cursor_y = 0;
			UiText_DrawResourceFar(best_mes, work->window, cursor_x, cursor_y);
		}
	}

	// 印の半透明を明滅させる
	q = &gIoWriteQueue;
	ime = &REG_IME;
	QUEUE_IO_WRITE_DELAY2((u32)&REG_BLDCNT, 0x3f00);
	QUEUE_IO_WRITE_DELAY2((u32)&REG_BLDALPHA, ((16 - blend) << 8) | blend);
}
