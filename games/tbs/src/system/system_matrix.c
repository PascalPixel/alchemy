/* The 3D transform, its stack, the look-at matrix and the projection. Part of
   system.c. */

/* The 3D transform. CurrentMatrix is MATRIX_WORDS Sint32 in 16.16 fixed point
   (1.0 is toFIXED(1)): [0] to [8] a 3x3 rotation, then the translation. */

#define MATRIX_DMA (DMA_COPY_WORDS(MATRIX_WORDS)) /* one matrix */

/* syMatrixBuildLookAt's reciprocals divide toFIXED(1) << RECIPROCAL_SHIFT,
   2^31, the most a Uint32 holds: the quotient shifted right by RECIPROCAL_SHIFT
   is toFIXED(1) over a whole-number divisor, and doubled it is the 16.16
   reciprocal of a 16.16 divisor. */
#define RECIPROCAL_SHIFT 15

/* syConvert3Dto2D's depth drops its low PROJECTION_DEPTH_SHIFT bits, and the
   focal length gains the rest of toFIXED(1)'s, so that the focal length over
   the depth comes out in 16.16 within 32 bits. */
#define PROJECTION_DEPTH_SHIFT 11

typedef Sint32 (*DOT3_FN)(
	Sint32 ax, Sint32 bx, Sint32 ay, Sint32 by, Sint32 az, Sint32 bz);

/* Allocates the one-level save area (heap slot HEAP_SLOT_MATRIX_STACK), clears
   MatrixStackDepth and resets CurrentMatrix to the unit matrix. */
void syInitMatrix(void)
{
	MatrixStackTop =
		syHeapSlotAllocEWRAM(HEAP_SLOT_MATRIX_STACK, sizeof(CurrentMatrix));
	MatrixStackDepth = 0;
	syUnitMatrixCurrentInline();
}

/* Saves CurrentMatrix into the save area (one level deep; a second save is
   ignored). */
void syPushMatrix(void)
{
	if(MatrixStackDepth <= 0) {
		syDMAXCopy(CurrentMatrix, MatrixStackTop, MATRIX_DMA, REG_DMA3);
		MatrixStackDepth++;
		MatrixStackTop += sizeof(CurrentMatrix);
	}
}

/* Copies CurrentMatrix to `dst`. */
void syGetMatrix(MATRIX dst)
{
	syDMAXCopy(CurrentMatrix, dst, MATRIX_DMA, REG_DMA3);
}

/* Copies the matrix at `src` into CurrentMatrix. */
void syLoadMatrix(const MATRIX src)
{
	syDMAXCopy(src, CurrentMatrix, MATRIX_DMA, REG_DMA3);
}

/* Restores CurrentMatrix from the save area, if syPushMatrix saved it. */
void syPopMatrix(void)
{
	if(MatrixStackDepth > 0) {
		--MatrixStackDepth;
		MatrixStackTop -= sizeof(CurrentMatrix);
		syDMAXCopy(MatrixStackTop, CurrentMatrix, MATRIX_DMA, REG_DMA3);
	}
}

/* Resets CurrentMatrix to the unit matrix. Reached through the stub call
   table. */
void syUnitMatrixCurrent(void)
{
	syUnitMatrixCurrentInline();
}

/* Rotates CurrentMatrix by angles[0], angles[1] and angles[2] about the X, Y
   and Z axes, in that order, with one combined matrix. */
void syMatrixRotate(Sint32 *angles)
{
	FIXED  sinX, cosX, sinY, cosY, sinZ, cosZ;
	MATRIX matrix;

	sinX = sySin(angles[0]);
	cosX = syCos(angles[0]);
	sinY = sySin(angles[1]);
	cosY = syCos(angles[1]);
	sinZ = sySin(angles[2]);
	cosZ = syCos(angles[2]);
	matrix[0] = syMulFX(cosY, cosZ);
	matrix[1] = syMulFX(cosY, sinZ);
	matrix[2] = -sinY;
	matrix[3] = syMulFX(syMulFX(sinX, sinY), cosZ) - syMulFX(cosX, sinZ);
	matrix[4] = syMulFX(syMulFX(sinX, sinY), sinZ) + syMulFX(cosX, cosZ);
	matrix[5] = syMulFX(sinX, cosY);
	matrix[6] = syMulFX(syMulFX(cosX, sinY), cosZ) + syMulFX(sinX, sinZ);
	matrix[7] = syMulFX(syMulFX(cosX, sinY), sinZ) - syMulFX(sinX, cosZ);
	matrix[8] = syMulFX(cosX, cosY);
	matrix[MATRIX_TRANSLATION_X] = 0;
	matrix[MATRIX_TRANSLATION_Y] = 0;
	matrix[MATRIX_TRANSLATION_Z] = 0;
	syMultiMatrix(matrix);
}

/* Rotates CurrentMatrix about the X axis by `ang`. */
void syRotX(ANGLE ang)
{
	MATRIX matrix;
	FIXED  sine = sySin(ang);
	FIXED  cosine = syCos(ang);

	syUnitMatrix(matrix);
	matrix[4] = cosine;
	matrix[5] = sine;
	matrix[7] = -sine;
	matrix[8] = cosine;
	syMultiMatrix(matrix);
}

/* Rotates CurrentMatrix about the Y axis by `ang`. */
void syRotY(ANGLE ang)
{
	MATRIX matrix;
	FIXED  sine = sySin(ang);
	FIXED  cosine = syCos(ang);

	syUnitMatrix(matrix);
	matrix[0] = cosine;
	matrix[2] = -sine;
	matrix[6] = sine;
	matrix[8] = cosine;
	syMultiMatrix(matrix);
}

/* Rotates CurrentMatrix about the Z axis by `ang`. */
void syRotZ(ANGLE ang)
{
	MATRIX matrix;
	FIXED  sine = sySin(ang);
	FIXED  cosine = syCos(ang);

	syUnitMatrix(matrix);
	matrix[0] = cosine;
	matrix[1] = sine;
	matrix[3] = -sine;
	matrix[4] = cosine;
	syMultiMatrix(matrix);
}

/* Translates CurrentMatrix by `pos` (x, y, z in 16.16). */
void syMatrixTranslateVector(const POINT pos)
{
	MATRIX matrix;

	syUnitMatrix(matrix);
	matrix[MATRIX_TRANSLATION_X] = pos[0];
	matrix[MATRIX_TRANSLATION_Y] = pos[1];
	matrix[MATRIX_TRANSLATION_Z] = pos[2];
	syMultiMatrix(matrix);
}

/* Scales CurrentMatrix by `scale` (x, y, z factors in 16.16). */
void syMatrixScaleVector(const VECTOR scale)
{
	MATRIX matrix;

	syUnitMatrix(matrix);
	matrix[0] = scale[0];
	matrix[4] = scale[1];
	matrix[8] = scale[2];
	syMultiMatrix(matrix);
}

/* Rotates CurrentMatrix by `angles` and translates it by `pos` in one
   matrix. Reached through the stub table. */
void syMatrixRotateTranslate(Sint32 *angles, POINT pos)
{
	FIXED  sinX, cosX, sinY, cosY, sinZ, cosZ;
	MATRIX matrix;

	sinX = sySin(angles[0]);
	cosX = syCos(angles[0]);
	sinY = sySin(angles[1]);
	cosY = syCos(angles[1]);
	sinZ = sySin(angles[2]);
	cosZ = syCos(angles[2]);
	matrix[0] = syMulFX(cosY, cosZ);
	matrix[1] = syMulFX(cosY, sinZ);
	matrix[2] = -sinY;
	matrix[3] = syMulFX(syMulFX(sinX, sinY), cosZ) - syMulFX(cosX, sinZ);
	matrix[4] = syMulFX(syMulFX(sinX, sinY), sinZ) + syMulFX(cosX, cosZ);
	matrix[5] = syMulFX(sinX, cosY);
	matrix[6] = syMulFX(syMulFX(cosX, sinY), cosZ) + syMulFX(sinX, sinZ);
	matrix[7] = syMulFX(syMulFX(cosX, sinY), sinZ) - syMulFX(sinX, cosZ);
	matrix[8] = syMulFX(cosX, cosY);
	matrix[MATRIX_TRANSLATION_X] = pos[0];
	matrix[MATRIX_TRANSLATION_Y] = pos[1];
	matrix[MATRIX_TRANSLATION_Z] = pos[2];
	syMultiMatrix(matrix);
}

/* Rotates by `angles`, scales each rotation row by `scale` and translates by
   `pos`, in one matrix. */
void syMatrixRotateTranslateScale(Sint32 *angles, POINT pos, VECTOR scale)
{
	FIXED  sinX, cosX, sinY, cosY, sinZ, cosZ;
	FIXED  axisScale;
	MATRIX matrix;

	sinX = sySin(angles[0]);
	cosX = syCos(angles[0]);
	sinY = sySin(angles[1]);
	cosY = syCos(angles[1]);
	sinZ = sySin(angles[2]);
	cosZ = syCos(angles[2]);
	axisScale = scale[0];
	matrix[0] = syMulFX(axisScale, syMulFX(cosY, cosZ));
	matrix[1] = syMulFX(axisScale, syMulFX(cosY, sinZ));
	matrix[2] = syMulFX(axisScale, -sinY);
	axisScale = scale[1];
	matrix[3] = syMulFX(
		axisScale, syMulFX(syMulFX(sinX, sinY), cosZ) - syMulFX(cosX, sinZ));
	matrix[4] = syMulFX(
		axisScale, syMulFX(syMulFX(sinX, sinY), sinZ) + syMulFX(cosX, cosZ));
	matrix[5] = syMulFX(axisScale, syMulFX(sinX, cosY));
	axisScale = scale[2];
	matrix[6] = syMulFX(
		axisScale, syMulFX(syMulFX(cosX, sinY), cosZ) + syMulFX(sinX, sinZ));
	matrix[7] = syMulFX(
		axisScale, syMulFX(syMulFX(cosX, sinY), sinZ) - syMulFX(sinX, cosZ));
	matrix[8] = syMulFX(axisScale, syMulFX(cosX, cosY));
	matrix[MATRIX_TRANSLATION_X] = pos[0];
	matrix[MATRIX_TRANSLATION_Y] = pos[1];
	matrix[MATRIX_TRANSLATION_Z] = pos[2];
	syMultiMatrix(matrix);
}

/* Builds in `out` the camera matrix of an eye at `eye` looking at `tgt`. The
   camera's sideways axis is kept level (no Y component). */
void syMatrixBuildLookAt(POINT eye, POINT tgt, MATRIX out)
{
	Uint32	code[7];
	Sint32	dx, dy, dz;
	FIXED	ux;
	Sint32	uy = 0; /* the sideways axis is kept level */
	Sint32	ex;
	DOT3_FN dot;
	Sint32	ey;
	FIXED	uz;
	FIXED	vx, vy, vz;
	Sint32	scale;
	Sint32	ez;
	Sint32	t;
	Uint32	dividend = (Uint32)toFIXED(1) << RECIPROCAL_SHIFT;

	/* Copy the dot product function to the stack and call it there. */
	dot = (DOT3_FN)code;
	syDMAXCopy(RelocatedInnerProduct, code, DMA_COPY_WORDS(7), REG_DMA3);

	/* The view axis d, pointing from the target back to the eye, normalized:
	   its length in whole pixels, and toFIXED(1) over it, negated. */
	dx = tgt[0] - eye[0];
	dy = tgt[1] - eye[1];
	dz = tgt[2] - eye[2];
	t = sySquartIWRAM(
		dot(dx >> 8, dx >> 8, dy >> 8, dy >> 8, dz >> 8, dz >> 8));
	scale = syIntegerDivideUnsigned(dividend, t) >> RECIPROCAL_SHIFT;
	scale = -scale;
	dx = syMulFX(dx, scale);
	dy = syMulFX(dy, scale);
	dz = syMulFX(dz, scale);
	uz = -dx;

	/* The sideways axis u = (dz, 0, -dx), normalized, unless d is vertical. */
	do {
		t = syMulFX(dy, dy);
		scale = toFIXED(1);
		if(scale - t <= 0)
			break;
		scale = syIntegerDivideUnsigned(dividend, sySquartFX(scale - t)) << 1;
	} while(0);
	ux = syMulFX(dz, scale);
	uz = syMulFX(uz, scale);

	/* The up axis v, the cross product of d and u, normalized. */
	vx = syMulFX(dy, uz);
	vy = syMulFX(dz, ux) - syMulFX(dx, uz);
	vz = -syMulFX(dy, ux);
	scale = syIntegerDivideUnsigned(
				dividend, sySquartFX(dot(vx, vx, vy, vy, vz, vz)))
		<< 1;
	vx = syMulFX(vx, scale);
	vy = syMulFX(vy, scale);
	vz = syMulFX(vz, scale);

	/* The three axes are the matrix columns; the translation is each axis's dot
	   product with the eye, negated. */
	ex = eye[0];
	ey = eye[1];
	ez = eye[2];
	out[0] = ux;
	out[3] = uy;
	out[6] = uz;
	out[MATRIX_TRANSLATION_X] = -dot(ex, ux, ez, uz, uy, uy);
	out[4] = vy;
	out[7] = vz;
	out[1] = vx;
	out[MATRIX_TRANSLATION_Y] = -dot(ex, vx, ey, vy, ez, vz);
	out[2] = dx;
	out[5] = dy;
	out[8] = dz;
	out[MATRIX_TRANSLATION_Z] = -dot(ex, dx, ey, dy, ez, dz);
}

/* Makes the camera at `eye` looking at `tgt` the whole of CurrentMatrix. */
void syLookAtLoad(POINT eye, POINT tgt)
{
	syMatrixBuildLookAt(eye, tgt, CurrentMatrix);
}

/* Multiplies the camera at `eye` looking at `tgt` into CurrentMatrix. */
void syLookAt(POINT eye, POINT tgt)
{
	MATRIX work;

	syMatrixBuildLookAt(eye, tgt, work);
	syMultiMatrix(work);
}

/* Sets ProjectionWork from a vertical field of view (1/65536 turns): the focal
   length, in 16.16 pixels, is 80 (half the screen height) over the tangent of
   half the angle. */
void syPerspective(Uint32 fieldOfView, FIXED nearDepth, FIXED farDepth)
{
	Sint32 halfAng;
	FIXED  sine;
	FIXED  focal;

	halfAng = (Sint32)fieldOfView / 2;
	sine = sySin(halfAng);
	/* syDivFX(d, v) is v * toFIXED(1) / d: here (DISPLAY_HEIGHT / 2) * cos /
	   sin */
	focal = syDivFX(sine, syCos(halfAng) * (DISPLAY_HEIGHT / 2));
	ProjectionWork.near = nearDepth;
	ProjectionWork.focal = focal;
	ProjectionWork.far = farDepth;
}

/* Sets ProjectionWork's focal length and depth range directly. */
void sySetScreenDist(Uint32 focal, Uint32 nearDepth, Uint32 farDepth)
{
	PROJECTION *projection = &ProjectionWork;

	projection->focal = focal;
	projection->near = nearDepth;
	projection->far = farDepth;
}

/* Reads three word-aligned coordinates from `point`, transforms them by
   CurrentMatrix and projects them to the screen: screen[0] and screen[1] are
   its pixel position and screen[2] its whole-unit depth. Returns the scale the
   point is drawn at, in 16.16, or 0 when it lies outside the projection's depth
   range and the screen position is not written. */
Sint32 syConvert3Dto2D(const POINT point, Sint32 *screen)
{
	PROJECTION *projection;
	Sint32		depth;
	Sint32		scale;
	Sint32		result;

	syCalcPoint(point, screen);
	projection = &ProjectionWork;
	/* The camera looks down -z, so a point in front of it has a negative z. */
	depth = -screen[2];
	result = 0;
	if(depth >= projection->near && depth <= projection->far) {
		screen[2] = FIXEDtoINT(depth);
		if(projection->focal != 0) {
			Uint32 shifted = (Uint32)depth >> PROJECTION_DEPTH_SHIFT;

			scale = syIntegerDivideUnsigned(
				projection->focal * (toFIXED(1) >> PROJECTION_DEPTH_SHIFT),
				shifted);
		} else {
			scale = toFIXED(33) / 25; /* 1.32 */
		}
		screen[0] =
			projection->centerX + syMulFX(screen[0], scale) / toFIXED(1);
		screen[1] =
			projection->centerY - syMulFX(screen[1], scale) / toFIXED(1);
		result = scale;
	}
	return result;
}

#undef MATRIX_DMA
#undef RECIPROCAL_SHIFT
#undef PROJECTION_DEPTH_SHIFT
