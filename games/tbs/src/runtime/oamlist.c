#include "unit/runtime/runtime.h"
#include "unit/runtime/oamlist.h"
#include "unit/runtime/heap.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mem.h"
#include "dma.h"
#include "ramcode.h"
#include "io_reg.h"
#include "io_write_queue.h"
#include "callback_scheduler.h"
#include "irq.h"
#include "types.h"
#include "scene.h"
#include "resource.h"
#include "resource_ids.h"
#include "global_cells.h"
#include "cartboot.h"
#include "ram_buffer.h"
#include "serial_runtime.h"
#include "frame.h"
#include "keystate.h"
#include "oam.h"
#include "map_scroll.h"
#include "heap_state.h"
#include "irqlock.h"
#include "iowrite.h"
#include "lib.h"
#include "fixed_math.h"
#include "affine.h"
#include "iwram_call.h"
#include "vram_block.h"
#include "string.h"
#include "low_runtime.h"
#include "vram_tab.h"
#include "layout_guard.h"
#include "edition.h"
#include "transform.h"
#include "project.h"
#include "save_state.h"
#include "runtime_interfaces.h"
#include "flash.h"
#include "audio_engine_symbols.h"

extern u8 IwramClearWords[];

s32 _call_via_r3(s32, s32, s32, s32);

void Graphics_ResetFrameState(void)
{
	gObjAffineCount = 0;
	_call_via_r3(((u32)&gRenderSlotHeads), 0x400, ((u32)&gObjAffineCount), (u32)IwramClearWords);
}

typedef void (*OamBuildRoutine)(void *work);

s32 AffineMatrix_BuildForEffect(struct AffineTransform *source)
{
	union AffineMatrix *matrix;
	s16				   *coefficient;
	s32					x_scale;
	s32					y_scale;
	s32					angle;
	u8					index;

	index = gObjAffineCount;
	x_scale = (s16)source->scale_x;
	y_scale = (s16)source->scale_y;
	angle = source->angle;
	if (index > 31)
		return 0;

	matrix = &gObjAffineMatrices[index];
	coefficient = matrix->coefficients;
	if ((x_scale == y_scale || -x_scale == y_scale) && angle == 0)
	{
		s32 reciprocal;
		s32 x_reciprocal;

		reciprocal = Iwram_SignedDivide(0x10000, y_scale);
		x_reciprocal = reciprocal;
		if (-x_scale == y_scale)
			x_reciprocal = -reciprocal;

		matrix->rows[0] = (u16)x_reciprocal;
		matrix->rows[1] = reciprocal << 16;
	}
	else
	{
		s32 sine;
		s32 cosine;

		sine = Trig_Sin(angle);
		cosine = Trig_Cos(angle);
		*coefficient = cosine / x_scale;
		coefficient++;
		*coefficient = sine / x_scale;
		coefficient++;
		*coefficient = (-sine) / y_scale;
		coefficient++;
		*coefficient = cosine / y_scale;
	}

	gObjAffineCount = index + 1;
	return index;
}

void Runtime_PushSlotEntry(void *slot_entry, s32 slot)
{
	void **head;
	void  *previous_head;

	if (slot > 0xFF)
		slot = 0xFF;
	if (slot < 0)
		slot = 0;
	head = &gRenderSlotHeads[slot];
	previous_head = *head;
	*head = slot_entry;
	*(void **)slot_entry = previous_head;
}

void Render_BuildOamList(void *work)
{
	OamBuildRoutine routine;

	RamCode_Load(routine, Render_BuildOamListKernel, LoadedRuntime_Size);
	routine(work);
	MemFree((void *)routine);
}

//--------------------------------------------------------------------------
// rescache.c
// リソース：OBJ の VRAM をブロックに分けて貸し出すキャッシュ
//--------------------------------------------------------------------------
// OBJ のキャラクター（0x06010000 から）を 64 バイトのブロック 512 個に分け、
// ResourceBlockOwners にブロックごとの持ち主（キャッシュの番号）を置く。
// キャッシュは 96 個で、offset（バイト）が 0xffff ならあき。
// size が 16 以下のものは VRAM を持っていない
