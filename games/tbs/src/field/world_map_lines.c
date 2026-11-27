/* The World Map's tilt: each line of the screen draws the flat map at its own
   scale and turn, so the ground seems to stretch away to a horizon. The
   function runs from its copy in HEAP_SLOT_RAM_CODE (field_world_map.c), so it
   calls the arctangent, sine and cosine through the stub table of the system
   library's module. */

#include "field/world_map_lines.h"
#include "field/world_map_affine.h"
#include "fixed_math.h"
#include "static_assert.h"
#include "types.h"
#include "system/globals.h"
#include "system/mul_fx.h"
#include "system/vector3.h"

/* A 16.16 value in the 8.8 fixed point of the affine registers, rounded toward
   zero, and an 8.8 value back in 16.16. */
#define FIXEDtoQ_8_8(value) ((value) / (toFIXED(1) / Q_8_8(1)))
#define syQ88toFIXED(value) ((value) * (toFIXED(1) / Q_8_8(1)))

/* Builds the world map's per-scanline affine table for one frame: the ground
   plane seen from eye towards target. line points at the 160 scanlines' ground
   terms, whose rotation terms stay cached while the yaw (ScanlineAngle) is
   unchanged; affine receives each line's parameters twice, for BG2 and then
   BG3, as the HBlank DMA writes both. */
void fdWorldMapUpdateAffineLines(VECTOR3 *eye, VECTOR3 *target,
	WORLD_MAP_SCANLINE *line, WORLD_MAP_AFFINE_LINE *affine)
{
	Uint32 angle;
	FIXED  originX;
	FIXED  originZ;
	Sint32 sinYaw;
	Sint32 cosYaw;
	FIXED  negativeSinYaw;
	Sint32 scanline;
	Sint32 depth;

	{
		Sint32 targetDeltaX = target->x;
		Sint32 targetZ = target->z;
		Sint32 eyeZ = eye->z;
		Sint32 eyeX = eye->x;

		depth = targetZ - eyeZ;
		targetDeltaX -= eyeX;
		angle = (Uint16)Stubs_System.arcTan2.tgt(targetDeltaX >> 4, depth >> 4);
	}
	/* The texture origin: the target at half scale. */
	originX = syMulFXArm(target->x, toFIXED(1) / 2);
	originZ = syMulFXArm(target->z, toFIXED(1) / 2);
	if(angle != ScanlineAngle) {
		/* The camera turned: recompute every scanline's rotation terms. */
		ScanlineAngle = angle;
		sinYaw = Stubs_System.sin.tgt(angle);
		cosYaw = Stubs_System.cos.tgt(angle);
		negativeSinYaw = -sinYaw;
		for(scanline = 0; scanline < DISPLAY_HEIGHT; scanline++) {
			FIXED  widthScale = line->scale;
			Sint32 centerX;
			Sint32 horizontalOffset;
			Sint32 depthOffset;
			Sint32 affinePa;
			Sint32 affinePc;

			depth = line->depth;
			affinePa = -FIXEDtoQ_8_8(syMulFXArm(widthScale, cosYaw));
			horizontalOffset = (Sint16)affinePa;
			line->pa = affinePa;
			affine->pa = affinePa;
			affinePc = FIXEDtoQ_8_8(syMulFXArm(widthScale, sinYaw));
			line->pc = affinePc;
			affine->pc = affinePc;
			/* FAKEMATCH(dead): the projection's centre read through its taken
			 * address. */
			centerX = *&ProjectionWork.centerX;
			/* UB: affine coefficients can be negative, and shifting a negative
			   value left is undefined; UBFIX multiplies. */
#if FIX_UNDEFINED_SHIFT
			horizontalOffset = syQ88toFIXED(horizontalOffset) * centerX;
			depthOffset = syQ88toFIXED((Sint16)affinePc) * -centerX;
#else
			horizontalOffset = (horizontalOffset << 8) * centerX;
			depthOffset = ((Sint16)affinePc << 8) * -centerX;
#endif
			/* The line's start: back from its centre by centerX pixels of the
			   line's step, then along the view by its depth. */
			line->ofsX =
				-(horizontalOffset + syMulFXArm(negativeSinYaw, depth));
			line->ofsY = depthOffset + syMulFXArm(cosYaw, depth);
			affine->x = (originX + line->ofsX) >> 8;
			affine->y = (originZ + line->ofsY) >> 8;
			affine[1] = affine[0];
			line++;
			affine += 2;
		}
	} else {
		/* Same angle: the cached terms stand and only the position moves. */
		for(scanline = 0; scanline < DISPLAY_HEIGHT; scanline++) {
			if(FrameCount & 1) {
				/* On odd frames the sample point shifts by half a pixel step
				   along the line and a quarter step across it, so alternate
				   frames sample between each other's texels. */
				Sint32 sampleOffset;

				sampleOffset =
					((originX + line->ofsX) >> 8) + line->pc / 4 + line->pa / 2;
				affine->x = sampleOffset;
				/* FAKEMATCH(dead): sampleOffset set to line->pa, never read. */
				sampleOffset = line->pa;
				affine->y =
					((originZ + line->ofsY) >> 8) + line->pa / 4 + line->pc / 2;
			} else {
				affine->x = (originX + line->ofsX) >> 8;
				affine->y = (originZ + line->ofsY) >> 8;
			}
			affine->pa = line->pa;
			affine->pc = line->pc;
			affine[1] = affine[0];
			line++;
			affine += 2;
		}
	}
}

#undef FIXEDtoQ_8_8
#undef syQ88toFIXED
