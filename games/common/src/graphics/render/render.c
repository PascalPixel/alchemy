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
		CAMELOT_ASM("the identity row's four fixed registers")
		register s32 z2 __asm__("r3"), one __asm__("r1"), z1 __asm__("r2"), z3 __asm__("r4");

		CAMELOT_ASM("transform.h's identity store, ⚓️ building the one before the zeros")
		__asm__ volatile("mov r0, %4\n\tmov %1, #128\n\tlsl %1, %1, #9\n\tmov %2, #0\n\t"
						 "mov %0, #0\n\tmov %3, #0\n\t"
						 "stmia r0!, {%1, %2, %0, %3}\n\t"
						 "stmia r0!, {%1, %2, %0, %3}\n\t"
						 "stmia r0!, {%1, %2, %0, %3}"
			: "=r"(z2), "=r"(one), "=r"(z1), "=r"(z3)
			: "r"(gTransform)
			: "r0", "memory");
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
	Transform_SetIdentity(gTransform);
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
	Transform_SetIdentity(gTransform);
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
	divide = Iwram_UnsignedDivide;
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
		/* FAKEMATCH: the used zero argument remains in r0 beside the first row stores. */
		register s32 zero asm("r0");
		out[0] = ux;
		zero = 0;
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
