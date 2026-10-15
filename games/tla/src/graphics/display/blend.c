//--------------------------------------------------------------------------
// blend.c
// 表示：画面の明るさのフェード（白・黒）の設定と、終わるまでの待ち
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
#include "types.h"
#include "fixed_math.h"
#include "iwram_call.h"

// フェードは VBlank ごとに BlendTransition_Update が進める。
// 明るさ（BLDY）は 0 で元のまま、16 で真っ白または真っ黒。
//   gBlendBrighten   : 0 黒へ / 1 白へ
//   gBlendLayers     : BLDCNT の対象レイヤー（下の 4 つは 0x3e で BG1〜3・OBJ・背景色）
//   gBlendFramesLeft : 残りフレーム数

//--------------------------------------------------------------------------
// 黒へフェードアウト
// frames : かけるフレーム数
//--------------------------------------------------------------------------
void Blend_SetDarkenTarget16(s32 frames)
{
	gBlendBrighten = 0;
	gBlendLayers = 0x3e;
	gBlendStartLevel = gBlendTargetLevel;
	gBlendTargetLevel = 16;
	gBlendDuration = frames;
	gBlendFramesLeft = gBlendDuration;
}

//--------------------------------------------------------------------------
// 黒からフェードイン
//--------------------------------------------------------------------------
void Blend_SetDarkenTarget0(s32 frames)
{
	gBlendBrighten = 0;
	gBlendLayers = 0x3e;
	gBlendStartLevel = gBlendTargetLevel;
	gBlendTargetLevel = 0;
	gBlendDuration = frames;
	gBlendFramesLeft = gBlendDuration;
}

//--------------------------------------------------------------------------
// 白へフェードアウト
//--------------------------------------------------------------------------
void Blend_SetBrightenTarget16(s32 frames)
{
	gBlendBrighten = 1;
	gBlendLayers = 0x3e;
	gBlendStartLevel = gBlendTargetLevel;
	gBlendTargetLevel = 16;
	gBlendDuration = frames;
	gBlendFramesLeft = gBlendDuration;
}

//--------------------------------------------------------------------------
// 白からフェードイン
//--------------------------------------------------------------------------
void Blend_SetBrightenTarget0(s32 frames)
{
	gBlendBrighten = 1;
	gBlendLayers = 0x3e;
	gBlendStartLevel = gBlendTargetLevel;
	gBlendTargetLevel = 0;
	gBlendDuration = frames;
	gBlendFramesLeft = gBlendDuration;
}

//--------------------------------------------------------------------------
// フェードを指定する
// bright : 0 黒へ / 1 白へ
// layer  : BLDCNT の対象レイヤー（下位 6 ビット）
// start  : はじめの明るさ（16 より大きければ今の目標の明るさから）
// target : 目標の明るさ
// frames : かけるフレーム数
//--------------------------------------------------------------------------
void Blend_ConfigureTransition(s8 bright, s32 layer, u32 start, s32 target, s32 frames)
{
	gBlendBrighten = bright;
	gBlendLayers = layer & 0x3f;
	if (start > 16U)
	{
		gBlendStartLevel = gBlendTargetLevel;
	}
	else
	{
		gBlendStartLevel = start;
	}
	gBlendTargetLevel = target;
	gBlendFramesLeft = (gBlendDuration = frames);
}

//--------------------------------------------------------------------------
// フェードが終わるまで待つ
//--------------------------------------------------------------------------
void Blend_WaitForTransition(void)
{
	if (gBlendDuration != 0)
	{
		do
		{
			WaitFrames(1);
		} while (gBlendDuration != 0);
	}
}

extern u8 gObjAffineCount;
extern u8 gOamBucketMasks[];

void Graphics_ResetFrameState(void)
{
    gObjAffineCount = 0;
    Iwram_ClearWords(gOamBucketMasks, 32);
}

struct AffineTransform
{
    u16 scale_x;
    u16 scale_y;
    u16 angle;
    u16 unknown_06;
};

union AffineMatrix
{
    s16 coefficients[4];
    u32 rows[2];
};

extern union AffineMatrix gObjAffineMatrices[];

// Takes the next of the 32 sprite affine matrices and fills it with the
// rotation and scale of source; returns its index, or 31 when all are taken.
s32 AffineMatrix_BuildForEffect(struct AffineTransform *source)
{
    union AffineMatrix *matrix;
    s16 *coefficient;
    s32 x_scale;
    s32 y_scale;
    s32 angle;
    u8 index;

    x_scale = (s16)source->scale_x;
    angle = source->angle;
    y_scale = (s16)source->scale_y;
    index = gObjAffineCount;
    if (index > 31)
        return 31;

    matrix = &gObjAffineMatrices[index];
    coefficient = matrix->coefficients;
    gObjAffineCount = index + 1;
    if ((x_scale == y_scale || x_scale == -y_scale) && angle == 0)
    {
        s32 reciprocal;
        s32 x_reciprocal;

        reciprocal = Iwram_SignedDivide(0x10000, y_scale);
        x_reciprocal = reciprocal;
        if (x_scale == -y_scale)
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

    return index;
}
