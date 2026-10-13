#include "unit/runtime/math.h"
#include "unit/runtime/heap.h"
#include "unit/runtime/matrix.h"
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

#define MTX_DMA (DMA_ENABLE32 | DMA_32BIT | 12) // 行列 1 つ分の DMA

typedef s32 (*Dot3Fn)(s32 ax, s32 bx, s32 ay, s32 by, s32 az, s32 bz);

//--------------------------------------------------------------------------
// 行列の退避先を確保し、今の行列を単位行列にする
//--------------------------------------------------------------------------
void Render_ResetTransformState(void)
{
	gTransformStackTop = Runtime_AllocateBlock(2, sizeof(gTransform));
	gTransformStackDepth = 0;
	CAMELOT_ASM("the fixed-register identity store of transform.h")
	Transform_ResetCurrent();
}

//--------------------------------------------------------------------------
// 今の行列を退避する（1 段まで）
//--------------------------------------------------------------------------
void Graphics_SaveTransferWorkOnce(void)
{
	if (gTransformStackDepth <= 0)
	{
		Dma_Set(gTransform, gTransformStackTop, MTX_DMA, REG_DMA3);
		gTransformStackDepth++;
		gTransformStackTop += sizeof(gTransform);
	}
}

//--------------------------------------------------------------------------
// 今の行列を dst に写す
//--------------------------------------------------------------------------
void Graphics_SaveTransferWork(void *dst)
{
	Dma_Set(gTransform, dst, MTX_DMA, REG_DMA3);
}

//--------------------------------------------------------------------------
// src の行列を今の行列にする
//--------------------------------------------------------------------------
void Graphics_LoadTransferWork(const void *src)
{
	Dma_Set(src, gTransform, MTX_DMA, REG_DMA3);
}

//--------------------------------------------------------------------------
// 退避した行列を戻す
//--------------------------------------------------------------------------
void Graphics_RestoreTransferWork(void)
{
	if (gTransformStackDepth > 0)
	{
		--gTransformStackDepth;
		gTransformStackTop -= sizeof(gTransform);
		Dma_Set(gTransformStackTop, gTransform, MTX_DMA, REG_DMA3);
	}
}

//--------------------------------------------------------------------------
// 今の行列を単位行列にする
//--------------------------------------------------------------------------
void SceneTransform_ResetMatrix(void)
{
	CAMELOT_ASM("the fixed-register identity store of transform.h")
	Transform_ResetCurrent();
}

//--------------------------------------------------------------------------
// X・Y・Z 軸の回転をかける
// ang : 3 軸の角度
//--------------------------------------------------------------------------
void SceneTransform_ApplyRotation(s32 *ang)
{
	s32 sx, cx, sy, cy, sz, cz;
	s32 m[12];

	sx = Trig_Sin(ang[0]);
	cx = Trig_Cos(ang[0]);
	sy = Trig_Sin(ang[1]);
	cy = Trig_Cos(ang[1]);
	sz = Trig_Sin(ang[2]);
	cz = Trig_Cos(ang[2]);
	m[0] = Iwram_MulQ16(cy, cz);
	m[1] = Iwram_MulQ16(cy, sz);
	m[2] = -sy;
	m[3] = Iwram_MulQ16(Iwram_MulQ16(sx, sy), cz) - Iwram_MulQ16(cx, sz);
	m[4] = Iwram_MulQ16(Iwram_MulQ16(sx, sy), sz) + Iwram_MulQ16(cx, cz);
	m[5] = Iwram_MulQ16(sx, cy);
	m[6] = Iwram_MulQ16(Iwram_MulQ16(cx, sy), cz) + Iwram_MulQ16(sx, sz);
	m[7] = Iwram_MulQ16(Iwram_MulQ16(cx, sy), sz) - Iwram_MulQ16(sx, cz);
	m[8] = Iwram_MulQ16(cx, cy);
	m[9] = 0;
	m[10] = 0;
	m[11] = 0;
	Iwram_TransformMatrix(m);
}

//--------------------------------------------------------------------------
// X 軸のまわりの回転をかける
//--------------------------------------------------------------------------
void SceneTransform_ApplyPitch(s32 ang)
{
	s32 m[12];
	s32 sin = Trig_Sin(ang);
	s32 cos = Trig_Cos(ang);

	CAMELOT_ASM("the fixed-register identity store of transform.h")
	Transform_SetIdentity(m);
	m[4] = cos;
	m[5] = sin;
	m[7] = -sin;
	m[8] = cos;
	Iwram_TransformMatrix(m);
}

//--------------------------------------------------------------------------
// Y 軸のまわりの回転をかける
//--------------------------------------------------------------------------
void SceneTransform_ApplyYaw(s32 ang)
{
	s32 m[12];
	s32 sin = Trig_Sin(ang);
	s32 cos = Trig_Cos(ang);

	CAMELOT_ASM("the fixed-register identity store of transform.h")
	Transform_SetIdentity(m);
	m[0] = cos;
	m[2] = -sin;
	m[6] = sin;
	m[8] = cos;
	Iwram_TransformMatrix(m);
}

//--------------------------------------------------------------------------
// Z 軸のまわりの回転をかける
//--------------------------------------------------------------------------
void SceneTransform_ApplyRoll(s32 ang)
{
	s32 m[12];
	s32 sin = Trig_Sin(ang);
	s32 cos = Trig_Cos(ang);

	CAMELOT_ASM("the fixed-register identity store of transform.h")
	Transform_SetIdentity(m);
	m[0] = cos;
	m[1] = sin;
	m[3] = -sin;
	m[4] = cos;
	Iwram_TransformMatrix(m);
}

//--------------------------------------------------------------------------
// 平行移動をかける
// pos : X・Y・Z（Q16）
//--------------------------------------------------------------------------
void SceneTransform_ApplyPosition(const s32 *pos)
{
	s32 m[12];

	CAMELOT_ASM("the fixed-register identity store of transform.h")
	Transform_SetIdentity(m);
	m[9] = pos[0];
	m[10] = pos[1];
	m[11] = pos[2];
	Iwram_TransformMatrix(m);
}

//--------------------------------------------------------------------------
// 拡大縮小をかける
// scale : X・Y・Z の倍率（Q16）
//--------------------------------------------------------------------------
void SceneTransform_ApplyScale(const s32 *scale)
{
	s32 m[12];

	CAMELOT_ASM("the fixed-register identity store of transform.h")
	Transform_SetIdentity(m);
	m[0] = scale[0];
	m[4] = scale[1];
	m[8] = scale[2];
	Iwram_TransformMatrix(m);
}

//--------------------------------------------------------------------------
// 回転と平行移動をまとめてかける
//--------------------------------------------------------------------------
void SceneTransform_ApplyRotationTranslation(s32 *ang, s32 *pos)
{
	s32 sx, cx, sy, cy, sz, cz;
	s32 m[12];

	sx = Trig_Sin(ang[0]);
	cx = Trig_Cos(ang[0]);
	sy = Trig_Sin(ang[1]);
	cy = Trig_Cos(ang[1]);
	sz = Trig_Sin(ang[2]);
	cz = Trig_Cos(ang[2]);
	m[0] = Iwram_MulQ16(cy, cz);
	m[1] = Iwram_MulQ16(cy, sz);
	m[2] = -sy;
	m[3] = Iwram_MulQ16(Iwram_MulQ16(sx, sy), cz) - Iwram_MulQ16(cx, sz);
	m[4] = Iwram_MulQ16(Iwram_MulQ16(sx, sy), sz) + Iwram_MulQ16(cx, cz);
	m[5] = Iwram_MulQ16(sx, cy);
	m[6] = Iwram_MulQ16(Iwram_MulQ16(cx, sy), cz) + Iwram_MulQ16(sx, sz);
	m[7] = Iwram_MulQ16(Iwram_MulQ16(cx, sy), sz) - Iwram_MulQ16(sx, cz);
	m[8] = Iwram_MulQ16(cx, cy);
	m[9] = pos[0];
	m[10] = pos[1];
	m[11] = pos[2];
	Iwram_TransformMatrix(m);
}

//--------------------------------------------------------------------------
// 回転・行ごとの拡大・平行移動をまとめてかける
// scale : 回転行列の 1〜3 行目にかける倍率
//--------------------------------------------------------------------------
void SceneTransform_ApplyScaledRotation(s32 *ang, s32 *pos, s32 *scale)
{
	s32 sx, cx, sy, cy, sz, cz;
	s32 s;
	s32 m[12];

	sx = Trig_Sin(ang[0]);
	cx = Trig_Cos(ang[0]);
	sy = Trig_Sin(ang[1]);
	cy = Trig_Cos(ang[1]);
	sz = Trig_Sin(ang[2]);
	cz = Trig_Cos(ang[2]);
	s = scale[0];
	m[0] = Iwram_MulQ16(s, Iwram_MulQ16(cy, cz));
	m[1] = Iwram_MulQ16(s, Iwram_MulQ16(cy, sz));
	m[2] = Iwram_MulQ16(s, -sy);
	s = scale[1];
	m[3] = Iwram_MulQ16(s, Iwram_MulQ16(Iwram_MulQ16(sx, sy), cz) - Iwram_MulQ16(cx, sz));
	m[4] = Iwram_MulQ16(s, Iwram_MulQ16(Iwram_MulQ16(sx, sy), sz) + Iwram_MulQ16(cx, cz));
	m[5] = Iwram_MulQ16(s, Iwram_MulQ16(sx, cy));
	s = scale[2];
	m[6] = Iwram_MulQ16(s, Iwram_MulQ16(Iwram_MulQ16(cx, sy), cz) + Iwram_MulQ16(sx, sz));
	m[7] = Iwram_MulQ16(s, Iwram_MulQ16(Iwram_MulQ16(cx, sy), sz) - Iwram_MulQ16(sx, cz));
	m[8] = Iwram_MulQ16(s, Iwram_MulQ16(cx, cy));
	m[9] = pos[0];
	m[10] = pos[1];
	m[11] = pos[2];
	Iwram_TransformMatrix(m);
}

//--------------------------------------------------------------------------
// eye から target を見るカメラの行列を out につくる
// 横の軸は水平（Y 成分 0）にとる
//--------------------------------------------------------------------------
void Graphics_PrepareTransfer(s32 *eye, s32 *target, s32 *out)
{
	u32	   code[7];
	s32	   dx, dy, dz;
	s32	   ux;
	s32	   zero = 0;
	s32	   ex;
	Dot3Fn dot;
	s32	   ey;
	s32	   uz;
	s32	   vx, vy, vz;
	s32	   scale;
	s32	   ez;
	s32	   t;
	u32	   num = 0x80000000; // 逆数を求めるときの被除数 2^31

	// 内積のルーチンをスタックに写して、そこで呼ぶ
	dot = (Dot3Fn)code;
	Dma_Set(RelocatedQ16DotProduct, code, DMA_ENABLE32 | DMA_32BIT | 7, REG_DMA3);

	// 視線の向き d（target から eye へ、長さ 1）
	dx = target[0] - eye[0];
	dy = target[1] - eye[1];
	dz = target[2] - eye[2];
	t = Iwram_Sqrt(dot(dx >> 8, dx >> 8, dy >> 8, dy >> 8, dz >> 8, dz >> 8));
	scale = Iwram_UnsignedDivide(num, t) >> 15;
	scale = -scale;
	/* FAKEMATCH: the normalization keeps its CSE block; without it, the third
	 * product spills before the negated axis (four reordered instructions).
	 * Measured as well: uz = -dx moved below the block differs 4, and the
	 * negation folded into uz = Iwram_MulQ16(-dx, scale) differs 38. */
	do
	{
		dx = Iwram_MulQ16(dx, scale);
		dy = Iwram_MulQ16(dy, scale);
		dz = Iwram_MulQ16(dz, scale);
		uz = -dx;
	} while (0);

	// 横の軸 u = (dz, 0, -dx) を長さ 1 に
	t = Iwram_MulQ16(dy, dy);
	scale = 0x10000;
	if (scale - t > 0)
		scale = Iwram_UnsignedDivide(num, FixedSqrt(scale - t)) << 1;
	ux = Iwram_MulQ16(dz, scale);
	uz = Iwram_MulQ16(uz, scale);

	// 上の軸 v = d と u の外積
	vx = Iwram_MulQ16(dy, uz);
	vy = Iwram_MulQ16(dz, ux) - Iwram_MulQ16(dx, uz);
	vz = -Iwram_MulQ16(dy, ux);
	scale = Iwram_UnsignedDivide(num, FixedSqrt(dot(vx, vx, vy, vy, vz, vz))) << 1;
	vx = Iwram_MulQ16(vx, scale);
	vy = Iwram_MulQ16(vy, scale);
	vz = Iwram_MulQ16(vz, scale);

	// 3 つの軸を列に置き、移動は eye との内積
	ex = eye[0];
	ey = eye[1];
	ez = eye[2];
	out[0] = ux;
	out[3] = zero;
	out[6] = uz;
	out[9] = -dot(ex, ux, ez, uz, zero, zero);
	out[4] = vy;
	out[7] = vz;
	out[1] = vx;
	out[10] = -dot(ex, vx, ey, vy, ez, vz);
	out[2] = dx;
	out[5] = dy;
	out[8] = dz;
	out[11] = -dot(ex, dx, ey, dy, ez, dz);
}

void Graphics_PrepareTransferInIwramWork(s32 *eye, s32 *target)
{
	Graphics_PrepareTransfer(eye, target, gTransform);
}

void Graphics_PrepareTransferAndRun(s32 *eye, s32 *target)
{
	s32 work[12];

	Graphics_PrepareTransfer(eye, target, work);
	Iwram_TransformMatrix(work);
}

void Camera_SetAngleParameters(u32 value, s32 param1, s32 param2)
{
	s32 half;
	s32 first;
	s32 result;

	half = (s32)(value + (value >> 31)) >> 1;
	first = Trig_Sin(half);
	result = Iwram_RatioMulQ14(first, Trig_Cos(half) * 0x50);
	gProjection.near = param1;
	gProjection.focal = result;
	gProjection.far = param2;
}

void Camera_StoreSceneParameters(u32 value0, u32 value1, u32 value2)
{
	struct Projection *work = &gProjection;

	work->focal = value0;
	work->near = value1;
	work->far = value2;
}

s32 Render_ProjectPoint(s32 *point, s32 *screen)
{
	struct Projection *projection;
	s32				   depth;
	s32				   scale;
	s32				   result;

	Iwram_TransformVector(point, screen);
	projection = &gProjection;
	depth = -screen[2];
	result = 0;
	if (depth >= projection->near && depth <= projection->far)
	{
		screen[2] = depth >> 16;
		if (projection->focal != 0)
		{
			u32 shifted = (u32)depth >> 11;

			scale = Iwram_UnsignedDivide(projection->focal << 5, shifted);
		}
		else
		{
			scale = 0x151eb;
		}
		screen[0] = projection->center_x + Iwram_MulQ16(screen[0], scale) / 0x10000;
		screen[1] = projection->center_y - Iwram_MulQ16(screen[1], scale) / 0x10000;
		result = scale;
	}
	return result;
}
