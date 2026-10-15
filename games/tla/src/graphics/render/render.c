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
#include "iwram_call.h"

// 変換行列 gTransform は s32 が 12 個（Q16 で 1.0 = 0x10000）。
// [0]〜[8] が 3x3 の回転、[9]〜[11] が平行移動。
// Iwram_TransformMatrix は渡した行列を今の行列にかけ合わせる。

#define MTX_DMA (DMA_ENABLE32 | DMA_32BIT | 12) // 行列 1 つ分の DMA

// ⚓️ は余弦を正弦の 90 度先で引く
#define COS(a) Trig_Sin((a) + 0x4000)

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
    CAMELOT_ASM("the fixed-register identity store of transform.h")
    Transform_ResetCurrent();
}

//--------------------------------------------------------------------------
// 退避先を作業領域の先頭に戻す
//--------------------------------------------------------------------------
void SceneTransform_RewindStack(void)
{
    gTransformStackTop = Ram_HeapSlots->transformStack;
    gTransformStackDepth = 0;
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
    /* FAKEMATCH: a one-pass block ends the scheduling block after the first sine: ⚓️ saves it and loads the angle before it builds 0x4000 (plainly the constant is built first, one reordered row). */
    do
    {
    } while (0);
    cx = COS(ang[0]);
    sy = Trig_Sin(ang[1]);
    cy = COS(ang[1]);
    sz = Trig_Sin(ang[2]);
    cz = COS(ang[2]);
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

// The identity store of transform.h's Transform_SetIdentity in ⚓️'s order,
// the 1.0 built before the zeros, as ⚓️'s Transform_ResetCurrent builds it.
static __inline__ void Render_SetIdentity(void *matrix)
{
    CAMELOT_ASM("three stmia no C store or copy compiles to")
    __asm__ volatile("mov r0, %0\n\tmov r1, #128\n\tlsl r1, r1, #9\n\tmov r2, #0\n\t"
                     "mov r3, #0\n\tmov r4, #0\n\t"
                     "stmia r0!, {r1, r2, r3, r4}\n\t"
                     "stmia r0!, {r1, r2, r3, r4}\n\t"
                     "stmia r0!, {r1, r2, r3, r4}"
        :
        : "r"(matrix)
        : "r0", "r1", "r2", "r3", "r4", "memory");
}

// Applies a rotation about the X axis.
void SceneTransform_ApplyPitch(s32 ang)
{
    s32 m[12];
    s32 sin, cos;

    CAMELOT_ASM("the fixed-register identity store above")
    Render_SetIdentity(m);
    cos = COS(ang);
    m[4] = cos;
    m[8] = cos;
    sin = Trig_Sin(ang);
    m[5] = sin;
    m[7] = -sin;
    Iwram_TransformMatrix(m);
}

// Applies a rotation about the Y axis.
void SceneTransform_ApplyYaw(s32 ang)
{
    s32 m[12];
    s32 sin, cos;

    CAMELOT_ASM("the fixed-register identity store above")
    Render_SetIdentity(m);
    cos = COS(ang);
    m[0] = cos;
    m[8] = cos;
    sin = Trig_Sin(ang);
    m[2] = -sin;
    m[6] = sin;
    Iwram_TransformMatrix(m);
}

// Makes the current matrix a rotation about the Y axis.
void Func_080150ac(s32 ang)
{
    s32 *m = gTransform;
    s32  sin, cos;

    CAMELOT_ASM("the fixed-register identity store above")
    Render_SetIdentity(m);
    cos = COS(ang);
    m[0] = cos;
    m[8] = cos;
    sin = Trig_Sin(ang);
    m[2] = -sin;
    m[6] = sin;
}

// Applies a rotation about the Z axis.
void Func_080150e4(s32 ang)
{
    s32 m[12];
    s32 sin, cos;

    CAMELOT_ASM("the fixed-register identity store above")
    Render_SetIdentity(m);
    cos = COS(ang);
    m[0] = cos;
    m[4] = cos;
    sin = Trig_Sin(ang);
    m[1] = sin;
    m[3] = -sin;
    Iwram_TransformMatrix(m);
}

// Applies a translation by pos, X, Y and Z in Q16.
void SceneTransform_ApplyPosition(const s32 *pos)
{
    s32 m[12];

    CAMELOT_ASM("the fixed-register identity store above")
    Render_SetIdentity(m);
    m[9] = pos[0];
    m[10] = pos[1];
    m[11] = pos[2];
    Iwram_TransformMatrix(m);
}

// Applies a translation by x, y and z in Q16.
void Func_08015160(s32 x, s32 y, s32 z)
{
    s32 m[12];

    CAMELOT_ASM("the fixed-register identity store above")
    Render_SetIdentity(m);
    m[9] = x;
    m[10] = y;
    m[11] = z;
    Iwram_TransformMatrix(m);
}

// Sets the current matrix's translation to pos.
void SceneTransform_SetPosition(const s32 *pos)
{
    gTransform[9] = pos[0];
    gTransform[10] = pos[1];
    gTransform[11] = pos[2];
}

// Applies a scale by scale's X, Y and Z factors in Q16.
void Func_080151ac(const s32 *scale)
{
    s32 m[12];

    CAMELOT_ASM("the fixed-register identity store above")
    Render_SetIdentity(m);
    m[0] = scale[0];
    m[4] = scale[1];
    m[8] = scale[2];
    Iwram_TransformMatrix(m);
}

// Applies a scale by x, y and z in Q16.
void Func_080151e4(s32 x, s32 y, s32 z)
{
    s32 m[12];

    CAMELOT_ASM("the fixed-register identity store above")
    Render_SetIdentity(m);
    m[0] = x;
    m[4] = y;
    m[8] = z;
    Iwram_TransformMatrix(m);
}

// Applies the same scale on all three axes.
void Func_0801521c(s32 scale)
{
    s32 m[12];

    CAMELOT_ASM("the fixed-register identity store above")
    Render_SetIdentity(m);
    m[0] = scale;
    m[4] = scale;
    m[8] = scale;
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
    /* FAKEMATCH: a one-pass block ends the scheduling block after the first sine: ⚓️ saves it and loads the angle before it builds 0x4000 (plainly the constant is built first, one reordered row). */
    do
    {
    } while (0);
    cx = COS(ang[0]);
    sy = Trig_Sin(ang[1]);
    cy = COS(ang[1]);
    sz = Trig_Sin(ang[2]);
    cz = COS(ang[2]);
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
    {
        s32 quarter;

        /* FAKEMATCH: a one-pass block and a forced temporary for 0x4000: ⚓️ starts the constant before it loads the angle (plainly the load comes first, one reordered row). */
        do
        {
        } while (0);
        quarter = 0x4000;
        cx = Trig_Sin(ang[0] + quarter);
    }
    sy = Trig_Sin(ang[1]);
    cy = COS(ang[1]);
    sz = Trig_Sin(ang[2]);
    cz = COS(ang[2]);
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
