//--------------------------------------------------------------------------
// RESOBJ.C
// リソース：アニメのスプライトオブジェクトの作成・解放と、コマの長さ
//--------------------------------------------------------------------------

#include "EDITION.H"
#include "RESOURCE.H"
#include "DMA.H"
#include "IO_REG.H"
#include "ANIMSPR.H"

struct ResourceObjectWork
{
#if GAME_TLA
	u8 unknown_00[16];
	u8 resource; // VRAM キャッシュの番号
	u8 flags;	 // ビット 0：VRAM キャッシュを返さない
	u8 unknown_12[22];
#else
	u8 unknown_00[28];
	u8 resource; // VRAM キャッシュの番号
	u8 flags;	 // ビット 0：VRAM キャッシュを返さない
	u8 unknown_1e[10];
#endif
	void *children[4];
};

typedef char ResourceObjectWork_size[sizeof(struct ResourceObjectWork) == 56 ? 1 : -1];

void ResourceMetadata_ClearRecord(void *destination);

#if !GAME_TLA
// 失われし時代の版（0x08022d40）は、まだリストのまま
#include "METADATA_LOOKUP.H"
#include "VRAM_TAB.H"

extern struct AnimationObject *gSpriteObjects;

s32 ResourceMetadata_Register(struct AnimationObject *state, s32 id);

//--------------------------------------------------------------------------
// アニメ id のスプライトオブジェクトを作る
// あいている 64 個のオブジェクトの 1 つと VRAM キャッシュを使う
// 戻り値 : オブジェクト（作れなければ NULL）
//--------------------------------------------------------------------------
struct ResourceObject *ResourceObject_Create(s32 id)
{
	struct AnimationMetadata *meta;
	struct AnimationObject	 *obj;
	struct AnimationObject	 *found;
	u32						 *word;
	s32						  slot;
	s32						  tile;
	s32						  i;
	u32						  shape;

	found = NULL;
	meta = Resource_GetMetadataRecordFar(id);
	slot = Resource_FindFreeEntry();
	obj = gSpriteObjects;
	if (meta->width == 0)
		return NULL;
	for (i = 0; i <= 63; i++, obj++)
	{
		if (obj->width == 0) // 幅 0 はあき
		{
			found = obj;
			break;
		}
	}
	if (found == NULL)
		return NULL;
	if (slot == VRAM_CACHE_ENTRY_COUNT)
		return NULL;
	tile = VramBlock_LoadCached(slot, 0, 0);
	if (tile == 0)
		return NULL;
	found->slot = slot;
	found->rotation = 0;
	found->flags = 1;

	// 幅・高さ（ドット）から OBJ の形とサイズのビット（属性 0・1）
	switch ((u32)((meta->width << 8) + meta->height))
	{
	case 0x0808: // 8x8
		shape = 0;
		break;
	case 0x0810: // 8x16
		shape = 0x8000;
		break;
	case 0x1008: // 16x8
		shape = 0x4000;
		break;
	case 0x1010: // 16x16
		shape = 0x40000000;
		break;
	case 0x1020: // 16x32
		shape = 0x80008000;
		break;
	case 0x2010: // 32x16
		shape = 0x80004000;
		break;
	case 0x2020: // 32x32
		shape = 0x80000000;
		break;
	case 0x2040: // 32x64
		shape = 0xc0008000;
		break;
	case 0x4020: // 64x32
		shape = 0xc0004000;
		break;
	case 0x4040: // 64x64
		shape = 0xc0000000;
		break;
	default:
		shape = 0;
		break;
	}

	// パーツ 0：256 色、優先度 2
	// パーツ 1：16x8・256 色、VRAM キャッシュ 93 のタイル、優先度 2
	word = (u32 *)found->part;
	*word++ = 0;
	*word++ = shape | 0x2000;
	*word++ = tile | 0x800;
	*word++ = 0;
	*word++ = 0x6000;
	*word = (gVramBlockCache[93].offset >> 5) | 0x800;
	if (ResourceMetadata_Register(obj, id) == -1)
	{
		/* FAKEMATCH: native dead -1 after Register (movs/negs); empty arm
		   after a same-byte reload keeps the carrier (differ 32 -> 0). */
		found->dirty = found->dirty;
	}
	return (struct ResourceObject *)found;
}
#endif

//--------------------------------------------------------------------------
// オブジェクトを解放する
// VRAM キャッシュ（flags のビット 0 が立っていなければ）と 4 つの子を
// 返し、ワーク 56 バイトを 0 で埋める
//--------------------------------------------------------------------------
void ResourceObject_Release(struct ResourceObjectWork *work)
{
	volatile u32 zero;
	s32			 n;
	void	   **child;

	if (work)
	{
		if (!(work->flags & 1))
			Resource_ResetEntry(work->resource);
		child = work->children;
		n = 3;
		do
		{
			ResourceMetadata_ClearRecord(*child++);
		} while (--n >= 0);
		zero = 0;
		Dma_Set(&zero, work, (DMA_ENABLE32 | DMA_32BIT | DMA_SRC_FIXED32) | (sizeof(*work) / 4),
			REG_DMA3);
	}
}

#include "METADATA_LOOKUP.H"
#include "TYPES.H"
#include "SCENE.H"

//--------------------------------------------------------------------------
// アニメ id の no 番の、はじめの cnt コマの長さを足す
// コマンドは 2 バイト（コマンド・値）で、254・241・253・239 で終わる。
// 245・255・238 以下のコマンドの値を足す
//--------------------------------------------------------------------------
s32 ResourceMetadata_SumCommandLengths(s32 id, u32 no, s32 cnt)
{
	struct AnimationMetadata *info;
	u8						 *p;
	u8						  op;
	u8						  val;
	s32						  sum = 0;

	info = Resource_GetMetadataRecordFar(id);
	if (no >= info->animation_count)
	{
		return 0;
	}
	p = ((u8 **)info->animation)[no];
	for (;;)
	{
		op = p[0];
		val = p[1];
		p += 2;
		if (op == 254 || op == 241 || op == 253 || op == 239)
		{
			break;
		}
		if (op == 245 || op == 255 || op <= 238)
		{
			sum += val;
			cnt--;
			if (cnt == 0)
			{
				break;
			}
		}
	}
	return sum;
}
