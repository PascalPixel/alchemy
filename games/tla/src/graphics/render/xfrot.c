//--------------------------------------------------------------------------
// xfrot.c
// 表示：回転と平行移動、行ごとの拡大をまとめてかける
// 式は ☀️ と同じ（games/common/src/graphics/render/render.c）。⚓️ では
// 間に単位行列を使う行列づくりが並ぶので、別のファイルから置く。
//--------------------------------------------------------------------------

#include "types.h"
#include "transform.h"
#include "fixed_math.h"
#include "iwram_call.h"

// ⚓️ は余弦を正弦の 90 度先で引く
#define COS(a) Trig_Sin((a) + 0x4000)

//--------------------------------------------------------------------------
// 回転と平行移動をまとめてかける
//--------------------------------------------------------------------------
void SceneTransform_ApplyRotationTranslation(s32 *ang, s32 *pos)
{
	s32 sx, cx, sy, cy, sz, cz;
	s32 m[12];

	sx = Trig_Sin(ang[0]);
	/* FAKEMATCH: a one-pass block ends the scheduling block after the first sine: ⚓️ saves it and loads the angle before it builds 0x4000 (plainly the constant is built first, one reordered row). */
	do {} while (0);
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
		do {} while (0);
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
