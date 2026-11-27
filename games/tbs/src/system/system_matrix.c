/* @file */
/* The 3D transform, its stack, the look-at matrix and the projection. */
/* Part of system.c. */

/* The 3D transform. CurrentMatrix is MATRIX_WORDS s32 in 16.16 fixed point */
/* (1.0 is FIXED_ONE): [0] to [8] a 3x3 rotation, then the translation. */

#define MATRIX_DMA (DMA_COPY_WORDS(MATRIX_WORDS)) /* one matrix */

/* syMatrixBuildLookAt's reciprocals divide FIXED_ONE << RECIPROCAL_SHIFT, */
/* 2^31, the most a u32 holds: the quotient shifted right by RECIPROCAL_SHIFT */
/* is FIXED_ONE over a whole-number divisor, and doubled it is the 16.16 */
/* reciprocal of a 16.16 divisor. */
#define RECIPROCAL_SHIFT 15

/* syConvert3Dto2D's depth drops its low PROJECTION_DEPTH_SHIFT bits, */
/* and the focal length gains the rest of FIXED_ONE's, so that the focal */
/* length over the depth comes out in 16.16 within 32 bits. */
#define PROJECTION_DEPTH_SHIFT 11

typedef s32 (*DOT3_FN)(s32 ax, s32 bx, s32 ay, s32 by, s32 az, s32 bz);

/* Allocates the one-level save area (heap slot HEAP_SLOT_MATRIX_STACK), */
/* clears MatrixStackDepth and resets CurrentMatrix to the unit matrix. */
void syInitMatrix(void) {
	MatrixStackTop =
		syHeapSlotAllocEwram(HEAP_SLOT_MATRIX_STACK, sizeof(CurrentMatrix));
	MatrixStackDepth = 0;
	syMatrixSetCurrentUnit();
}

/* Saves CurrentMatrix into the save area (one level deep; a second save is */
/* ignored). */
void syPushMatrix(void) {
	if(MatrixStackDepth <= 0) {
		syDMAXCopy(CurrentMatrix, MatrixStackTop, MATRIX_DMA, REG_DMA3);
		MatrixStackDepth++;
		MatrixStackTop += sizeof(CurrentMatrix);
	}
}

/* Copies CurrentMatrix to `dst`. */
void syGetMatrix(MATRIX dst) {
	syDMAXCopy(CurrentMatrix, dst, MATRIX_DMA, REG_DMA3);
}

/* Copies the matrix at `src` into CurrentMatrix. */
void syLoadMatrix(const MATRIX src) {
	syDMAXCopy(src, CurrentMatrix, MATRIX_DMA, REG_DMA3);
}

/* Restores CurrentMatrix from the save area, if syPushMatrix saved it. */
void syPopMatrix(void) {
	if(MatrixStackDepth > 0) {
		--MatrixStackDepth;
		MatrixStackTop -= sizeof(CurrentMatrix);
		syDMAXCopy(MatrixStackTop, CurrentMatrix, MATRIX_DMA, REG_DMA3);
	}
}

/* Resets CurrentMatrix to the unit matrix. Reached through the stub call */
/* table. */
void syMatrixResetUnit(void) { syMatrixSetCurrentUnit(); }

/* Rotates CurrentMatrix by angles[0], angles[1] and angles[2] about the X, Y */
/* and Z axes, in that order, with one combined matrix. */
void syMatrixRotate(s32 *angles) {
	FIXED sinX, cosX, sinY, cosY, sinZ, cosZ;
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

/* Rotates CurrentMatrix about the X axis by `angle`. */
void syRotX(ANGLE angle) {
	MATRIX matrix;
	FIXED sine = sySin(angle);
	FIXED cosine = syCos(angle);

	syUnitMatrix(matrix);
	matrix[4] = cosine;
	matrix[5] = sine;
	matrix[7] = -sine;
	matrix[8] = cosine;
	syMultiMatrix(matrix);
}

/* Rotates CurrentMatrix about the Y axis by `angle`. */
void syRotY(ANGLE angle) {
	MATRIX matrix;
	FIXED sine = sySin(angle);
	FIXED cosine = syCos(angle);

	syUnitMatrix(matrix);
	matrix[0] = cosine;
	matrix[2] = -sine;
	matrix[6] = sine;
	matrix[8] = cosine;
	syMultiMatrix(matrix);
}

/* Rotates CurrentMatrix about the Z axis by `angle`. */
void syRotZ(ANGLE angle) {
	MATRIX matrix;
	FIXED sine = sySin(angle);
	FIXED cosine = syCos(angle);

	syUnitMatrix(matrix);
	matrix[0] = cosine;
	matrix[1] = sine;
	matrix[3] = -sine;
	matrix[4] = cosine;
	syMultiMatrix(matrix);
}

/* Translates CurrentMatrix by `position` (x, y, z in 16.16). */
void syMatrixTranslateVector(const POINT position) {
	MATRIX matrix;

	syUnitMatrix(matrix);
	matrix[MATRIX_TRANSLATION_X] = position[0];
	matrix[MATRIX_TRANSLATION_Y] = position[1];
	matrix[MATRIX_TRANSLATION_Z] = position[2];
	syMultiMatrix(matrix);
}

/* Scales CurrentMatrix by `scale` (x, y, z factors in 16.16). */
void syMatrixScaleVector(const VECTOR scale) {
	MATRIX matrix;

	syUnitMatrix(matrix);
	matrix[0] = scale[0];
	matrix[4] = scale[1];
	matrix[8] = scale[2];
	syMultiMatrix(matrix);
}

/* Rotates CurrentMatrix by `angles` and translates it by `position` in one */
/* matrix. Reached through the stub table. */
void syMatrixRotateTranslate(s32 *angles, POINT position) {
	FIXED sinX, cosX, sinY, cosY, sinZ, cosZ;
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
	matrix[MATRIX_TRANSLATION_X] = position[0];
	matrix[MATRIX_TRANSLATION_Y] = position[1];
	matrix[MATRIX_TRANSLATION_Z] = position[2];
	syMultiMatrix(matrix);
}

/* Rotates by `angles`, scales each rotation row by `scale` and translates by */
/* `position`, in one matrix. */
void syMatrixRotateTranslateScale(s32 *angles, POINT position, VECTOR scale) {
	FIXED sinX, cosX, sinY, cosY, sinZ, cosZ;
	FIXED axisScale;
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
	matrix[3] = syMulFX(axisScale, syMulFX(syMulFX(sinX, sinY), cosZ) -
									   syMulFX(cosX, sinZ));
	matrix[4] = syMulFX(axisScale, syMulFX(syMulFX(sinX, sinY), sinZ) +
									   syMulFX(cosX, cosZ));
	matrix[5] = syMulFX(axisScale, syMulFX(sinX, cosY));
	axisScale = scale[2];
	matrix[6] = syMulFX(axisScale, syMulFX(syMulFX(cosX, sinY), cosZ) +
									   syMulFX(sinX, sinZ));
	matrix[7] = syMulFX(axisScale, syMulFX(syMulFX(cosX, sinY), sinZ) -
									   syMulFX(sinX, cosZ));
	matrix[8] = syMulFX(axisScale, syMulFX(cosX, cosY));
	matrix[MATRIX_TRANSLATION_X] = position[0];
	matrix[MATRIX_TRANSLATION_Y] = position[1];
	matrix[MATRIX_TRANSLATION_Z] = position[2];
	syMultiMatrix(matrix);
}

/* Builds in `out` the camera matrix of an eye at `eye` looking at `target`. */
/* The camera's sideways axis is kept level (no Y component). */
void syMatrixBuildLookAt(POINT eye, POINT target, MATRIX out) {
	u32 code[7];
	s32 dx, dy, dz;
	FIXED ux;
	s32 uy = 0; /* the sideways axis is kept level */
	s32 ex;
	DOT3_FN dot;
	s32 ey;
	FIXED uz;
	FIXED vx, vy, vz;
	s32 scale;
	s32 ez;
	s32 t;
	u32 dividend = (u32)FIXED_ONE << RECIPROCAL_SHIFT;

	/* Copy the dot product function to the stack and call it there. */
	dot = (DOT3_FN)code;
	syDMAXCopy(RelocatedInnerProduct, code, DMA_COPY_WORDS(7), REG_DMA3);

	/* The view axis d, pointing from the target back to the eye, normalized: */
	/* its length in whole pixels, and FIXED_ONE over it, negated. */
	dx = target[0] - eye[0];
	dy = target[1] - eye[1];
	dz = target[2] - eye[2];
	t = sySquart(dot(dx >> 8, dx >> 8, dy >> 8, dy >> 8, dz >> 8, dz >> 8));
	scale = syIntegerDivideUnsigned(dividend, t) >> RECIPROCAL_SHIFT;
	scale = -scale;
	dx = syMulFX(dx, scale);
	dy = syMulFX(dy, scale);
	dz = syMulFX(dz, scale);
	uz = -dx;

	/* The sideways axis u = (dz, 0, -dx), normalized, unless d is vertical. */
	do {
		t = syMulFX(dy, dy);
		scale = FIXED_ONE;
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
	scale = syIntegerDivideUnsigned(dividend,
									sySquartFX(dot(vx, vx, vy, vy, vz, vz)))
			<< 1;
	vx = syMulFX(vx, scale);
	vy = syMulFX(vy, scale);
	vz = syMulFX(vz, scale);

	/* The three axes are the matrix columns; the translation is each axis's */
	/* dot product with the eye, negated. */
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

/* Makes the camera at `eye` looking at `target` the whole of CurrentMatrix. */
void syMatrixSetLookAt(POINT eye, POINT target) {
	syMatrixBuildLookAt(eye, target, CurrentMatrix);
}

/* Multiplies the camera at `eye` looking at `target` into CurrentMatrix. */
void syLookAt(POINT eye, POINT target) {
	MATRIX work;

	syMatrixBuildLookAt(eye, target, work);
	syMultiMatrix(work);
}

/* Sets ProjectionWork from a vertical field of view (1/65536 turns): the focal
 */
/* length, in 16.16 pixels, is 80 (half the screen height) over the tangent of
 */
/* half the angle. */
void syPerspective(u32 fieldOfView, FIXED nearDepth, FIXED farDepth) {
	s32 halfAngle;
	FIXED sine;
	FIXED focal;

	halfAngle = (s32)fieldOfView / 2;
	sine = sySin(halfAngle);
	/* syDivFX(d, v) is v * FIXED_ONE / d: here */
	/* (DISPLAY_HEIGHT / 2) * cos / sin */
	focal = syDivFX(sine, syCos(halfAngle) * (DISPLAY_HEIGHT / 2));
	ProjectionWork.near = nearDepth;
	ProjectionWork.focal = focal;
	ProjectionWork.far = farDepth;
}

/* Sets ProjectionWork's focal length and depth range directly. */
void sySetScreenDist(u32 focal, u32 nearDepth, u32 farDepth) {
	PROJECTION *projection = &ProjectionWork;

	projection->focal = focal;
	projection->near = nearDepth;
	projection->far = farDepth;
}

/* Reads three word-aligned coordinates from `point`, transforms them by */
/* CurrentMatrix and projects them to the screen: screen[0] and screen[1] are */
/* its pixel position and screen[2] its whole-unit depth. Returns the scale the
 */
/* point is drawn at, in 16.16, or 0 when it lies outside the projection's */
/* depth range and the screen position is not written. */
s32 syConvert3Dto2D(const POINT point, s32 *screen) {
	PROJECTION *projection;
	s32 depth;
	s32 scale;
	s32 result;

	syCalcPoint(point, screen);
	projection = &ProjectionWork;
	/* The camera looks down -z, so a point in front of it has a negative z. */
	depth = -screen[2];
	result = 0;
	if(depth >= projection->near && depth <= projection->far) {
		screen[2] = FIXEDtoINT(depth);
		if(projection->focal != 0) {
			u32 shifted = (u32)depth >> PROJECTION_DEPTH_SHIFT;

			scale = syIntegerDivideUnsigned(
				projection->focal * (FIXED_ONE >> PROJECTION_DEPTH_SHIFT),
				shifted);
		}
		else {
			scale = FIXED_ONE * 33 / 25; /* 1.32 */
		}
		screen[0] = projection->centerX + syMulFX(screen[0], scale) / FIXED_ONE;
		screen[1] = projection->centerY - syMulFX(screen[1], scale) / FIXED_ONE;
		result = scale;
	}
	return result;
}

#undef MATRIX_DMA
#undef RECIPROCAL_SHIFT
#undef PROJECTION_DEPTH_SHIFT
