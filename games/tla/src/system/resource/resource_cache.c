//--------------------------------------------------------------------------
// resource_cache.c
// リソース：OBJ の VRAM をブロックに分けて貸し出すキャッシュ
//--------------------------------------------------------------------------

#include "resource.h"
#include "vram_tab.h"
#include "fixed_math.h"
#include "iwram_call.h"
#include "affine.h"

// OBJ のキャラクター（0x06010000 から）を 64 バイトのブロック 512 個に分け、
// gResourceBlockOwners にブロックごとの持ち主（キャッシュの番号）を置く。
// キャッシュは 96 個で、offset（バイト）が 0xffff ならあき。
// size が 16 以下のものは VRAM を持っていない

//--------------------------------------------------------------------------
// キャッシュ no に、今の大きさのまま src を写しなおす
// 戻り値 : OBJ のタイル番号
//--------------------------------------------------------------------------
s32 Resource_GetBuffer(s32 no, s32 src)
{
#if GAME_TLA
	return VramBlock_LoadCached(no, gResourceTableEntries[no].size, (const void *)src);
#endif
}

// Takes the next of the 32 sprite affine matrices and fills it with a
// rotation by angle and the scales; returns its index, or 31 when all are
// taken. The body is AffineMatrix_BuildForEffect's.
s32 AffineMatrix_Build(s32 angle, s32 x_scale, s32 y_scale)
{
    union AffineMatrix *matrix;
    s16 *coefficient;
    u8 index;

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
