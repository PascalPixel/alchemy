/* Draft, differ 205 of 228: The Broken Seal's Graphics_PrepareTransfer for The Lost Age: the
 * dot-product routine copied to the stack is eight words here (Math_DotProductQ16Code, frame 68),
 * the square roots are Iwram_Sqrt shifted left 8, and the native holds three routines in registers
 * for the whole function, the divide in r9, the stack routine in r10 and the multiply in r11,
 * loading only the square root entry from the pool at each use; it keeps eye, out, the view vector
 * and the side axis on the stack. This form gives r9 to out and reloads the divide entry at each
 * call, so nearly every row after the prologue differs in registers. Local pointers for the divide
 * and the multiply, with and without The Broken Seal's zero and dividend locals and its one-pass
 * block, all measured 202 to 208. */
#include "system.h"
#include "mem.h"
#include "edition.h"
#include "types.h"
#include "transform.h"
#include "dma.h"
#include "io_reg.h"
#include "fixed_math.h"
#include "iwram_call.h"

typedef s32 (*Dot3Fn)(s32 ax, s32 bx, s32 ay, s32 by, s32 az, s32 bz);
extern const u32 Math_DotProductQ16Code[];
#define SQRT_Q16(x) (Iwram_Sqrt(x) << 8)

void Graphics_PrepareTransfer(s32 *eye, s32 *target, s32 *out)
{
	u32 code[8];
	s32 dx, dy, dz;
	s32 ux, uz;
	s32 vx, vy, vz;
	s32 ex, ey, ez;
	s32 scale;
	s32 t;
	u32 (*divide)(u32 dividend, u32 divisor) = Iwram_UnsignedDivideEntry;
	Dot3Fn dot = (Dot3Fn)code;
	s32 (*mul)(s32 a, s32 b) = Iwram_MulQ16;

	Dma_Set(Math_DotProductQ16Code, code, DMA_ENABLE32 | DMA_32BIT | 8, REG_DMA3);

	dx = target[0] - eye[0];
	dy = target[1] - eye[1];
	dz = target[2] - eye[2];
	t = Iwram_Sqrt(dot(dx >> 8, dx >> 8, dy >> 8, dy >> 8, dz >> 8, dz >> 8));
	scale = -(s32)(divide(0x80000000, t) >> 15);
	dx = mul(dx, scale);
	dy = mul(dy, scale);
	dz = mul(dz, scale);

	uz = -dx;
	t = mul(dy, dy);
	scale = 0x10000;
	if (scale - t > 0)
		scale = divide(0x80000000, SQRT_Q16(scale - t)) << 1;
	ux = mul(dz, scale);
	uz = mul(uz, scale);

	vx = mul(dy, uz);
	vy = mul(dz, ux) - mul(dx, uz);
	vz = -mul(dy, ux);
	scale = divide(0x80000000, SQRT_Q16(dot(vx, vx, vy, vy, vz, vz))) << 1;
	vx = mul(vx, scale);
	vy = mul(vy, scale);
	vz = mul(vz, scale);

	ex = eye[0];
	ey = eye[1];
	ez = eye[2];
	out[0] = ux;
	out[3] = 0;
	out[6] = uz;
	out[9] = -dot(ex, ux, ez, uz, 0, 0);
	out[1] = vx;
	out[4] = vy;
	out[7] = vz;
	out[10] = -dot(ex, vx, ey, vy, ez, vz);
	out[2] = dx;
	out[5] = dy;
	out[8] = dz;
	out[11] = -dot(ex, dx, ey, dy, ez, dz);
}
