#include "types.h"
#include "frame.h"
#include "mulq16.h"
#include "project.h"

#define LINES 160

// 8 バイトの中継: ldr r4 / bx r4 と飛び先
struct FarCallEntry
{
	u32	  stub;
	void *target;
};

extern struct FarCallEntry FarCall_RuntimeTable[];
extern u32				   gScanlineAngle;

#define FAR_CALL(index, type) ((type)FarCall_RuntimeTable[index].target)
#define ArcTan2(x, z)		  FAR_CALL(8, s32 (*)(s32, s32))(x, z)
#define Sin(angle)			  FAR_CALL(11, s32 (*)(s32))(angle)
#define Cos(angle)			  FAR_CALL(12, s32 (*)(s32))(angle)

struct Position
{
	s32 x;
	s32 y;
	s32 z;
};

// 走査線 1 本の地面の点
struct ScanVertex
{
	s32 x;
	s32 z;
	s32 dx;
	s32 dz;
	s16 u;
	s16 v;
};

// BG2PA ... BG2Y
struct BgAffine
{
	s16 pa;
	s16 pb;
	s16 pc;
	s16 pd;
	s32 x;
	s32 y;
};

void Transform_UpdateVertices(struct Position *eye, struct Position *target, struct ScanVertex *vtx,
							  struct BgAffine *out)
{
	u32 angle;
	s32 px;
	s32 pz;
	s32 sin;
	s32 cos;
	s32 nsin;
	s32 i;
	s32 z;

	{
		s32 tx = target->x;
		s32 tz = target->z;
		s32 ez = eye->z;
		s32 ex = eye->x;

		z = tz - ez;
		tx -= ex;
		angle = (u16)ArcTan2(tx >> 4, z >> 4);
	}
	px = MulQ16(target->x, 0x8000);
	pz = MulQ16(target->z, 0x8000);
	if (angle != gScanlineAngle)
	{
		// 角度が変わった
		gScanlineAngle = angle;
		sin = Sin(angle);
		cos = Cos(angle);
		nsin = -sin;
		for (i = LINES - 1; i >= 0; i--)
		{
			s32 x = vtx->x;
			s32 cx;
			s32 a;
			s32 b;
			s32 u;
			s32 v;

			z = vtx->z;
			u = -(MulQ16(x, cos) / 256);
			a = (s16)u;
			vtx->u = u;
			out->pa = u;
			v = MulQ16(x, sin) / 256;
			vtx->v = v;
			out->pc = v;
			/* FAKEMATCH: the centre is read through its own address, so the native single literal for it is built; member access loads gProjection and reads at offset 12 (differ 2). */
			cx = *&gProjection.center_x;
			a = (a << 8) * cx;
			b = ((s16)v << 8) * -cx;
			vtx->dx = -(a + MulQ16(nsin, z));
			vtx->dz = b + MulQ16(cos, z);
			out->x = (px + vtx->dx) >> 8;
			out->y = (pz + vtx->dz) >> 8;
			out[1] = out[0];
			vtx++;
			out += 2;
		}
	}
	else
	{
		// 角度が同じ
		for (i = LINES - 1; i >= 0; i--)
		{
			if (gFrameCount & 1)
			{
				// 奇数フレーム
				/* FAKEMATCH: one scratch variable holds the x sum and then u, as the native keeps both in r3; with separate values three instructions of out->y come in another order (differ 6). */
				s32 work;

				work = ((px + vtx->dx) >> 8) + vtx->v / 4 + vtx->u / 2;
				out->x = work;
				work = vtx->u;
				out->y = ((pz + vtx->dz) >> 8) + work / 4 + vtx->v / 2;
			}
			else
			{
				out->x = (px + vtx->dx) >> 8;
				out->y = (pz + vtx->dz) >> 8;
			}
			out->pa = vtx->u;
			out->pc = vtx->v;
			out[1] = out[0];
			vtx++;
			out += 2;
		}
	}
}
