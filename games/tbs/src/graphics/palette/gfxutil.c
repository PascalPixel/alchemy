//--------------------------------------------------------------------------
// render.c
// 表示：変換行列の退避・復帰と、回転・移動・拡大・カメラの行列づくり
//--------------------------------------------------------------------------

#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mem.h"
#include "edition.h"
#include "types.h"
#include "transform.h"
#include "dma.h"
#include "io_reg.h"
#include "fixed_math.h"
#include "resource.h"
#include "project.h"
#include "iwram_call.h"
#include "scene.h"
#include "global_cells.h"
#include "ramcode.h"
#include "save_state.h"
#include "irq.h"
#include "runtime_interfaces.h"
#include "flash.h"

// 変換行列 gTransform は s32 が 12 個（Q16 で 1.0 = 0x10000）。
// [0]〜[8] が 3x3 の回転、[9]〜[11] が平行移動。
// Iwram_TransformMatrix は渡した行列を今の行列にかけ合わせる。

#define MTX_DMA (DMA_ENABLE32 | DMA_32BIT | 12) // 行列 1 つ分の DMA

#if GAME_TLA
#include "ram_buffer.h"

//--------------------------------------------------------------------------
// 何もしない
//--------------------------------------------------------------------------
void Render_EmptyHook(void)
{
}

//--------------------------------------------------------------------------
// 行列の退避先を確保し、今の行列を単位行列にする
//--------------------------------------------------------------------------
void Render_ResetTransformState(void)
{
	gTransformStackTop = Runtime_AllocateBlock(8, sizeof(gTransform));
	gTransformStackDepth = 0;
	{
		s32 zero;

		CAMELOT_ASM("transform.h's identity store, ⚓️ building the one before the zeros")
		__asm__ volatile("mov r0, %1\n\tmov r1, #128\n\tlsl r1, r1, #9\n\tmov r2, #0\n\t"
						 "mov %0, #0\n\tmov r4, #0\n\t"
						 "stmia r0!, {r1, r2, %0, r4}\n\t"
						 "stmia r0!, {r1, r2, %0, r4}\n\t"
						 "stmia r0!, {r1, r2, %0, r4}"
			: "=l"(zero)
			: "r"(gTransform)
			: "r0", "r1", "r2", "r4", "memory");
	}
}

//--------------------------------------------------------------------------
// 退避先を作業領域の先頭に戻す
//--------------------------------------------------------------------------
void SceneTransform_RewindStack(void)
{
	gTransformStackTop = Ram_HeapSlots->transform_stack;
	gTransformStackDepth = 0;
}

#else
#include "system.h"
#include "iwram_call.h"

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

#endif

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

#if !GAME_TLA

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
	u32 code[7];
	s32 dx, dy, dz;
	s32 ux;
	s32 zero = 0;
	s32 ex;
	Dot3Fn		 dot;
	s32			 ey;
	s32 uz;
	s32			 vx, vy, vz;
	s32			 scale;
	s32			 ez;
	s32			 t;
	u32			 num = 0x80000000; // 逆数を求めるときの被除数 2^31
	u32 (*divide)(u32 dividend, u32 divisor);

	// 内積のルーチンをスタックに写して、そこで呼ぶ
	dot = (Dot3Fn)code;
	Dma_Set(RelocatedQ16DotProduct, code, DMA_ENABLE32 | DMA_32BIT | 7, REG_DMA3);

	// 視線の向き d（target から eye へ、長さ 1）
	dx = target[0] - eye[0];
	dy = target[1] - eye[1];
	dz = target[2] - eye[2];
	t = Iwram_Sqrt(dot(dx >> 8, dx >> 8, dy >> 8, dy >> 8, dz >> 8, dz >> 8));
	/* FAKEMATCH: the routine is called through a local pointer to its fixed entry; a plain call matches The Broken Seal through its IWRAM call layer but not The Lost Age, whose header calls the entry directly (all six of its editions differ). */
	divide = Iwram_UnsignedDivideEntry;
	scale = divide(num, t) >> 15;
	scale = -scale;
	/* FAKEMATCH: the normalization keeps its CSE block; without it, the third
	 * product spills before the negated axis (four reordered instructions). */
	do
	{
		dx = Iwram_MulQ16(dx, scale);
		dy = Iwram_MulQ16(dy, scale);
		t = Iwram_MulQ16(dz, scale);
		uz = -dx;
		dz = t;
	} while (0);

	// 横の軸 u = (dz, 0, -dx) を長さ 1 に
	t = Iwram_MulQ16(dy, dy);
	scale = 0x10000;
	if (scale - t > 0)
		scale = divide(num, FixedSqrt(scale - t)) << 1;
	ux = Iwram_MulQ16(dz, scale);
	uz = Iwram_MulQ16(uz, scale);

	// 上の軸 v = d と u の外積
	vx = Iwram_MulQ16(dy, uz);
	vy = Iwram_MulQ16(dz, ux) - Iwram_MulQ16(dx, uz);
	vz = -Iwram_MulQ16(dy, ux);
	scale = divide(num, FixedSqrt(dot(vx, vx, vy, vy, vz, vz))) << 1;
	vx = Iwram_MulQ16(vx, scale);
	vy = Iwram_MulQ16(vy, scale);
	vz = Iwram_MulQ16(vz, scale);

	// 3 つの軸を列に置き、移動は eye との内積
	ex = eye[0];
	ey = eye[1];
	ez = eye[2];
	{
		out[0] = ux;
		out[3] = zero;
		out[6] = uz;
		out[9] = -dot(ex, ux, ez, uz, zero, zero);
	}
	out[4] = vy;
	out[7] = vz;
	out[1] = vx;
	out[10] = -dot(ex, vx, ey, vy, ez, vz);
	out[2] = dx;
	out[5] = dy;
	out[8] = dz;
	out[11] = -dot(ex, dx, ey, dy, ez, dz);
}
#endif

//--------------------------------------------------------------------------

// render.c

// 表示：変換行列の退避・復帰と、回転・移動・拡大・カメラの行列づくり

//--------------------------------------------------------------------------

#if GAME_TLA
#include "ram_buffer.h"

//--------------------------------------------------------------------------
// 何もしない
//--------------------------------------------------------------------------
void Render_EmptyHook(void)
{
}

//--------------------------------------------------------------------------
// 行列の退避先を確保し、今の行列を単位行列にする
//--------------------------------------------------------------------------
void Render_ResetTransformState(void)
{
	gTransformStackTop = Runtime_AllocateBlock(8, sizeof(gTransform));
	gTransformStackDepth = 0;
	{
		s32 zero;

		CAMELOT_ASM("transform.h's identity store, ⚓️ building the one before the zeros")
		__asm__ volatile("mov r0, %1\n\tmov r1, #128\n\tlsl r1, r1, #9\n\tmov r2, #0\n\t"
						 "mov %0, #0\n\tmov r4, #0\n\t"
						 "stmia r0!, {r1, r2, %0, r4}\n\t"
						 "stmia r0!, {r1, r2, %0, r4}\n\t"
						 "stmia r0!, {r1, r2, %0, r4}"
			: "=l"(zero)
			: "r"(gTransform)
			: "r0", "r1", "r2", "r4", "memory");
	}
}

//--------------------------------------------------------------------------
// 退避先を作業領域の先頭に戻す
//--------------------------------------------------------------------------
void SceneTransform_RewindStack(void)
{
	gTransformStackTop = Ram_HeapSlots->transform_stack;
	gTransformStackDepth = 0;
}

#else
#include "system.h"
#include "iwram_call.h"

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

#endif

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

#if !GAME_TLA

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
	u32 code[7];
	s32 dx, dy, dz;
	s32 ux;
	s32 zero = 0;
	s32 ex;
	Dot3Fn		 dot;
	s32			 ey;
	s32 uz;
	s32			 vx, vy, vz;
	s32			 scale;
	s32			 ez;
	s32			 t;
	u32			 num = 0x80000000; // 逆数を求めるときの被除数 2^31
	u32 (*divide)(u32 dividend, u32 divisor);

	// 内積のルーチンをスタックに写して、そこで呼ぶ
	dot = (Dot3Fn)code;
	Dma_Set(RelocatedQ16DotProduct, code, DMA_ENABLE32 | DMA_32BIT | 7, REG_DMA3);

	// 視線の向き d（target から eye へ、長さ 1）
	dx = target[0] - eye[0];
	dy = target[1] - eye[1];
	dz = target[2] - eye[2];
	t = Iwram_Sqrt(dot(dx >> 8, dx >> 8, dy >> 8, dy >> 8, dz >> 8, dz >> 8));
	/* FAKEMATCH: the routine is called through a local pointer to its fixed entry; a plain call matches The Broken Seal through its IWRAM call layer but not The Lost Age, whose header calls the entry directly (all six of its editions differ). */
	divide = Iwram_UnsignedDivideEntry;
	scale = divide(num, t) >> 15;
	scale = -scale;
	/* FAKEMATCH: the normalization keeps its CSE block; without it, the third
	 * product spills before the negated axis (four reordered instructions). */
	do
	{
		dx = Iwram_MulQ16(dx, scale);
		dy = Iwram_MulQ16(dy, scale);
		t = Iwram_MulQ16(dz, scale);
		uz = -dx;
		dz = t;
	} while (0);

	// 横の軸 u = (dz, 0, -dx) を長さ 1 に
	t = Iwram_MulQ16(dy, dy);
	scale = 0x10000;
	if (scale - t > 0)
		scale = divide(num, FixedSqrt(scale - t)) << 1;
	ux = Iwram_MulQ16(dz, scale);
	uz = Iwram_MulQ16(uz, scale);

	// 上の軸 v = d と u の外積
	vx = Iwram_MulQ16(dy, uz);
	vy = Iwram_MulQ16(dz, ux) - Iwram_MulQ16(dx, uz);
	vz = -Iwram_MulQ16(dy, ux);
	scale = divide(num, FixedSqrt(dot(vx, vx, vy, vy, vz, vz))) << 1;
	vx = Iwram_MulQ16(vx, scale);
	vy = Iwram_MulQ16(vy, scale);
	vz = Iwram_MulQ16(vz, scale);

	// 3 つの軸を列に置き、移動は eye との内積
	ex = eye[0];
	ey = eye[1];
	ez = eye[2];
	{
		out[0] = ux;
		out[3] = zero;
		out[6] = uz;
		out[9] = -dot(ex, ux, ez, uz, zero, zero);
	}
	out[4] = vy;
	out[7] = vz;
	out[1] = vx;
	out[10] = -dot(ex, vx, ey, vy, ez, vz);
	out[2] = dx;
	out[5] = dy;
	out[8] = dz;
	out[11] = -dot(ex, dx, ey, dy, ez, dz);
}
#endif

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
    result = Iwram_RatioMulQ14(
        first,
        Trig_Cos(half)* 0x50
    );
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
    s32 depth;
    s32 scale;
    s32 result;

    Iwram_TransformVector(point, screen);
    projection = &gProjection;
    depth = -screen[2];
    result = 0;
    if (depth >= projection->near && depth <= projection->far) {
        screen[2] = depth >> 16;
        if (projection->focal != 0) {
            u32 shifted = (u32)depth >> 11;

            scale = Iwram_UnsignedDivide(projection->focal << 5, shifted);
        } else {
            scale = 0x151eb;
        }
        screen[0] = projection->center_x + Iwram_MulQ16(screen[0], scale) / 0x10000;
        screen[1] = projection->center_y - Iwram_MulQ16(screen[1], scale) / 0x10000;
        result = scale;
    }
    return result;
}

void Resource_DecompressHalfwords(const void *source, void *destination)
{
    u32 words = (u32)Resource_DecompressHalfwordsCodeSize >> 2;
    u32 routine[words];
    Dma_Set((const void *)Resource_DecodeHalfwordLz, routine, words | 0x84000000, REG_DMA3);
    ((void (*)(const void *, void *))routine)(source, destination);
}

s32 Resource_DecodeType01(const void *source, void *destination)
{
    s32 (*routine)(const void *, void *);
    s32 result;

    RamCode_Load(routine, Resource_DecodeType01Arm, Resource_DecodeType01CodeSize);
    result = routine(source, destination);
    MemFree(routine);
    return result;
}

s32 Resource_DecodeType2(const void *source, void *destination)
{
    s32 (*routine)(const void *, void *);
    s32 result;

    RamCode_Load(routine, Resource_Decode2, Resource_Decode2CodeSize);
    result = routine(source, destination);
    MemFree(routine);
    return result;
}

s32 Resource_DecodeByteLz(const void *source, void *destination)
{
    s32 (*routine)(const void *, void *);
    s32 result;

    RamCode_Load(routine, Resource_DecodeByteLzArm, Resource_DecodeByteLzCodeSize);
    result = routine(source, destination);
    MemFree(routine);
    return result;
}

void ColorBuffer_BackupAndBrighten(u8 *buffer, u32 amount, u8 *backup, u32 bytes)
{
    u32 words = (u32)ColorBuffer_BackupAndBrightenCodeSize >> 2;
    u32 routine[words];
    Dma_Set((const void *)ColorBuffer_BrightenKernel, routine, words | 0x84000000, REG_DMA3);
    ((void (*)(u8 *, u32, u8 *, u32))routine)(buffer, amount, backup, bytes);
}

void ColorBuffer_BackupAndDarken(u8 *buffer, u32 amount, u8 *backup, u32 bytes)
{
    u32 words = (u32)ColorBuffer_BackupAndDarkenCodeSize >> 2;
    u32 routine[words];
    Dma_Set((const void *)ColorBuffer_DarkenKernel, routine, words | 0x84000000, REG_DMA3);
    ((void (*)(u8 *, u32, u8 *, u32))routine)(buffer, amount, backup, bytes);
}

void ColorBuffer_BackupAndScaleThreeQuarters(u8 *buffer, u8 *backup, u32 bytes)
{
    u32 words = (u32)ColorBuffer_BackupAndScaleThreeQuartersCodeSize >> 2;
    u32 routine[words];
    Dma_Set((const void *)ColorBuffer_ScaleThreeQuartersKernel, routine, words | 0x84000000, REG_DMA3);
    ((void (*)(u8 *, u8 *, u32))routine)(buffer, backup, bytes);
}

void ColorBuffer_BackupAndHalve(u8 *buffer, u8 *backup, u32 bytes)
{
    u32 words = (u32)ColorBuffer_BackupAndHalveCodeSize >> 2;
    u32 routine[words];
    Dma_Set((const void *)ColorBuffer_HalveKernel, routine, words | 0x84000000, REG_DMA3);
    ((void (*)(u8 *, u8 *, u32))routine)(buffer, backup, bytes);
}

void ColorBuffer_BackupAndBrightenPartial(u8 *buffer, u32 amount, u8 *backup, u32 bytes)
{
    u32 words = (u32)ColorBuffer_BackupAndBrightenPartialCodeSize >> 2;
    u32 routine[words];
    Dma_Set((const void *)ColorBuffer_BrightenPartialKernel, routine, words | 0x84000000, REG_DMA3);
    ((void (*)(u8 *, u32, u8 *, u32))routine)(buffer, amount, backup, bytes);
}

void ColorBuffer_BackupAndDarkenPartial(u8 *buffer, u32 amount, u8 *backup, u32 bytes)
{
    u32 words = (u32)ColorBuffer_BackupAndDarkenPartialCodeSize >> 2;
    u32 routine[words];
    Dma_Set((const void *)ColorBuffer_DarkenPartialKernel, routine, words | 0x84000000, REG_DMA3);
    ((void (*)(u8 *, u32, u8 *, u32))routine)(buffer, amount, backup, bytes);
}

void ColorBuffer_BackupAndScaleNonzeroThreeQuarters(u8 *buffer, u8 *backup, u32 bytes)
{
    u32 words = (u32)ColorBuffer_BackupAndScaleNonzeroThreeQuartersCodeSize >> 2;
    u32 routine[words];
    Dma_Set((const void *)ColorBuffer_ScaleNonzeroThreeQuartersKernel, routine, words | 0x84000000, REG_DMA3);
    ((void (*)(u8 *, u8 *, u32))routine)(buffer, backup, bytes);
}

void ColorBuffer_BackupAndHalveNonzero(u8 *buffer, u8 *backup, u32 bytes)
{
    u32 words = (u32)ColorBuffer_BackupAndHalveNonzeroCodeSize >> 2;
    u32 routine[words];
    Dma_Set((const void *)ColorBuffer_HalveNonzeroKernel, routine, words | 0x84000000, REG_DMA3);
    ((void (*)(u8 *, u8 *, u32))routine)(buffer, backup, bytes);
}

s32 SaveState_InitializeWorkspace(void)
{
    struct SaveWorkspace *work;
    struct SaveSlotHeader header;
    struct SaveSlotHeader *slot;
    volatile u32 zero;
    u32 index;

    work = Runtime_AllocateBlock(0x33, sizeof(*work));
    zero = 0;
    START_DMA(&zero, work, 0x85000440);
    SetFlashTimerIntr(2, &gIrqHandlers[5]);

    for (index = 0; index < 8; index++) {
        if ((u16)IdentifyFlash() == 0)
            break;
        WaitFrames(1);
    }
    if (index > 7)
        return 1;

    for (index = 0; index <= 15; index++) {
        u32 status;
        u32 record;
        u8 id;

        work->occupied[index] = 0;
        work->record_id[index] = 0x10;
        work->sequence[index] = 0;
        status = SaveState_ReadSlotAndCheckChecksum(index);
        START_DMA(&work->slot, &header, 0x84000004);
        WAIT_DMA();
        slot = &header;

        if (SaveState_CompareBytes(slot->signature, (u8 *)Save_Signature, 7) != 0)
            continue;
        work->sequence[index] = slot->sequence;
        record = slot->record_id;
        id = record;
        if (id > 15)
            continue;
        if (status != 0)
            continue;

        work->occupied[index] = 1;
        work->record_id[index] = record;
        if (status < index) {
            do {
                if (work->record_id[status] == (u8)record) {
                    if (work->sequence[status] < slot->sequence)
                        work->occupied[status] = 0;
                    else
                        work->occupied[index] = 0;
                }
                status++;
            } while (status < index);
        }
    }
    return 0;
}

u32 Random16(void);

u32 SaveState_SelectWriteSlot(s32 mode)
{
    u32 empty[16];
    s32 count;
    u32 index;
    u8 *slot;
    u8 value;

    slot = gSaveWorkspace->occupied;
    count = 0;
    index = 0;
    do {
        value = *slot++;
        if (value == 0) {
            empty[count++] = index;
        }
        index += 1;
    } while (index <= 0xFU);
    index = 0x10;
    if (count != 0) {
        if (count == 1) {
            index = empty[0];
            if (SaveState_FindLatestSlot(mode) == 0x10) {
                index = 0x10;
            }
        } else {
            index = Random16() % count;
            index = empty[index];
        }
    }
    return index;
}

s32 _call_via_r3(s32, s32, s32, s32);

u32 SaveState_WriteWorkspaceSlot(s32 sector)
{
    s32 *param = &Flash_Handler0;
    s32 result;
    struct SaveWorkspace *work;
    s32 value;

    work = gSaveWorkspace;
    value = (u16)sector;
    if ((_call_via_r3(value, (s32)&work->slot,
                       (s32)param, *param) << 0x10) != 0) {
        return 1U;
    }
    result = Flash_VerifySector(value, (s32)&work->slot);
    return (u32)((0 - result) | result) >> 0x1F;
}

#define Flash_Handler0 (*(s32 *)&Flash_Handler0)

s32 SaveState_ReadSlotAndCheckChecksum(s32 index)
{
    struct SaveWorkspace *work;
    struct SaveSlotHeader header;
    u32 checksum;

    work = gSaveWorkspace;
    ReadFlash(index, 0, work->slot.bytes, sizeof(work->slot));
    Dma_Set(&work->slot, &header, 0x84000004, REG_DMA3);
    WAIT_DMA();
    checksum = SaveState_ChecksumWorkspace();
    return (u16)checksum - header.checksum;
}

typedef u16 (*EraseSectorProc)(u16);

u16 SaveState_EraseSlotSector(u16 value)
{
    return ((EraseSectorProc)gEraseFlashSector)(value);
}

s32 SaveState_WriteRecord(s32 record_id, void *source)
{
    struct SaveWorkspace *work;
    struct SaveSlotHeader header;
    volatile u32 zero;
    u32 current;
    u32 slot;

    work = gSaveWorkspace;
    zero = 0;
    START_DMA(&zero, &work->slot, 0x85000400);
    WAIT_DMA();
    current = SaveState_FindLatestSlot(record_id);
    slot = SaveState_SelectWriteSlot(record_id);
    if (slot > 15)
        return 1;

    START_DMA(source, work->slot.record.payload, 0x840003fc);
    WAIT_DMA();
    START_DMA(Save_HeaderTemplate, &header, 0x84000002);
    WAIT_DMA();
    header.record_id = record_id;
    header.checksum = SaveState_ChecksumWorkspace();
    header.sequence = SaveState_GetLatestSequence(record_id) + 1;
    START_DMA(&header, &work->slot.record.header, 0x84000004);
    WAIT_DMA();

    if (SaveState_WriteWorkspaceSlot(slot) != 0)
        return 1;
    if (current <= 15 && SaveState_InvalidateSlot(current) != 0)
        return 1;

    if (header.sequence > 0xfde8) {
        header.sequence = 1;
        START_DMA(&header, &work->slot.record.header, 0x84000004);
        WAIT_DMA();
        if (SaveState_WriteWorkspaceSlot(current) != 0)
            return 1;
        if (SaveState_InvalidateSlot(slot) != 0)
            return 1;
        slot = current;
    }

    work->occupied[slot] = 1;
    work->record_id[slot] = record_id;
    work->sequence[slot] = header.sequence;
    return 0;
}

u32 SaveState_ReadRecordPayload(s32 record_id, void *destination)
{
    struct SaveWorkspace *work;
    u32 index;

    work = gSaveWorkspace;
    index = SaveState_FindLatestSlot(record_id);
    if (index > 15)
        return 1;
    SaveState_ReadSlotAndCheckChecksum(index);
    START_DMA(work->slot.record.payload, destination, 0x840003fc);
    WAIT_DMA();
    return 0;
}

u32 SaveState_DeleteRecord(s32 record_id)
{
    s32 index;
    s32 deletion_result;

    index = SaveState_FindLatestSlot(record_id);
    if (index > 0xFU) {
        return 1U;
    }
    deletion_result = SaveState_InvalidateSlot(index);
    return (u32)((0 - deletion_result) | deletion_result) >> 0x1F;
}

s32 SaveState_ChecksumWorkspace(void)
{
    struct SaveWorkspace *work;
    u32 limit;
    u32 offset;
    s32 sum;

    work = gSaveWorkspace;
    limit = 0xFE7;
    sum = 0;
    offset = 0;
    do {
        sum += work->slot.record.payload[offset + 0];
        sum += work->slot.record.payload[offset + 1];
        sum += work->slot.record.payload[offset + 2];
        sum += work->slot.record.payload[offset + 3];
        sum += work->slot.record.payload[offset + 4];
        sum += work->slot.record.payload[offset + 5];
        sum += work->slot.record.payload[offset + 6];
        sum += work->slot.record.payload[offset + 7];
        offset += 8;
    } while (offset <= limit);
    return sum;
}

u32 SaveState_FindLatestSlot(s32 record_id)
{
    struct SaveWorkspace *work;
    u32 latest_sequence;
    u32 latest_slot;
    u32 i;

    work = gSaveWorkspace;
    latest_slot = 0x10;
    latest_sequence = 0;
    for (i = 0; i <= 15; i++) {
        if (work->occupied[i] != 0 && record_id == work->record_id[i]) {
            if (latest_sequence < work->sequence[i]) {
                latest_sequence = work->sequence[i];
                latest_slot = i;
            }
        }
    }
    return latest_slot;
}

s32 SaveState_InvalidateSlot(s32 index)
{
    struct SaveWorkspace *work;
    struct SaveSlotHeader header;
    volatile u32 zero;

    work = gSaveWorkspace;
    zero = 0;
    START_DMA(&zero, &header, 0x85000004);
    WAIT_DMA();
    START_DMA(Save_HeaderTemplate, &header, 0x84000002);
    WAIT_DMA();
    header.record_id = 0x10;
    header.sequence = 0;
    START_DMA(&header, &work->slot.record.header, 0x84000004);
    WAIT_DMA();
    if (SaveState_WriteWorkspaceSlot(index) != 0)
        return 1;
    work->occupied[index] = 0;
    work->record_id[index] = 0x10;
    work->sequence[index] = 0;
    return 0;
}

s32 SaveState_CompareBytes(u8 *left, u8 *right, s32 count)
{
    s32 difference = 0;

    while (count != 0) {
        difference = *left - *right;
        if (difference != 0)
            break;
        count--;
        left++;
        right++;
    }
    return difference;
}

u32 SaveState_GetLatestSequence(s32 record_id)
{
    struct SaveWorkspace *work;
    u32 latest_sequence;
    u32 i;

    work = gSaveWorkspace;
    latest_sequence = 0;
    for (i = 0; i <= 15; i++) {
        if (work->occupied[i] != 0 && record_id == work->record_id[i]) {
            if (latest_sequence < work->sequence[i])
                latest_sequence = work->sequence[i];
        }
    }
    return latest_sequence;
}

s32 SaveState_LoadSummaryRecords(void)
{
    struct SaveWorkspace *work;
    struct SaveSummary *summary;
    volatile u32 zero;
    u32 group;
    s32 count;

    work = gSaveWorkspace;
    summary = work->summary;
    count = 0;
    group = 0;
    do {
        u32 index;

        zero = 0;
        START_DMA(&zero, summary, 0x85000010);
        index = SaveState_FindLatestSlot(group);
        if (index <= 15) {
            ReadFlash(index, 0, (u8 *)summary, sizeof(*summary));
            count++;
        }
        index = SaveState_FindLatestSlot(group + 3);
        if (index <= 15)
            ReadFlash(index, 0x110, summary->unknown_38, sizeof(summary->unknown_38));
        else
            *(u32 *)summary->unknown_38 = 0;
        group++;
        summary++;
    } while (group <= 2);
    return count;
}

u32 SaveState_ReleaseWorkspace(void)
{
    // 値を返さない int 関数 (pop {r1}; bx r1)
    Runtime_SetIrqHandler(5, 0, NULL);
    Runtime_ReleaseHeapBlock(51);
}
