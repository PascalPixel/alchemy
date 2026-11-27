/* The 3D transform, its stack, the look-at matrix and the projection. Part of
   system.c. */

/* A point in view space, and where syConvert3Dto2D puts it on the screen. */
typedef struct PROJECTED_POINT {
	Sint32 x;
	Sint32 y;
	Sint32 z;
} PROJECTED_POINT;

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

/* The cosine: the sine a quarter turn on. */
#define COS(a) sySin((a) + DEGtoANG(90))

/* An empty function. */
void syRenderNoOperationHook(void)
{
}

/* Allocates the one-level save area (heap slot HEAP_SLOT_MATRIX_STACK), clears
   MatrixStackDepth and resets CurrentMatrix to the unit matrix. */
void syInitMatrix(void)
{
	MatrixStackTop =
		syHeapSlotAllocEWRAM(HEAP_SLOT_MATRIX_STACK, sizeof(CurrentMatrix));
	MatrixStackDepth = 0;
	syUnitMatrixCurrentInline();
}

/* Empties the matrix stack: the next syPushMatrix saves at the start of its
   save area again. */
void syMatrixRewindStack(void)
{
	MatrixStackTop = Ram_HeapSlots->matrixStack;
	MatrixStackDepth = 0;
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
	FIXED  sinX;
	FIXED  cosX;
	FIXED  sinY;
	FIXED  cosY;
	FIXED  sinZ;
	FIXED  cosZ;
	MATRIX matrix;

	sinX = sySin(angles[0]);
	cosX = COS(angles[0]);
	sinY = sySin(angles[1]);
	cosY = COS(angles[1]);
	sinZ = sySin(angles[2]);
	cosZ = COS(angles[2]);
	SET_VECTOR(matrix, syMulFX(cosY, cosZ), syMulFX(cosY, sinZ), -sinY);
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

/* Stores the unit matrix at `matrix`: three runs of 1.0 and three zeros, the
   rotation's diagonal and a translation of 0. */
static __inline__ void syMatrixStoreUnit(MATRIX matrix)
{
	ASM_MATRIX_UNIT_STORE(matrix);
}

/* Rotates CurrentMatrix about the X axis by `ang`. */
void syRotX(ANGLE ang)
{
	MATRIX matrix;
	FIXED  sine;
	FIXED  cosine;

	syMatrixStoreUnit(matrix);
	cosine = COS(ang);
	matrix[4] = cosine;
	matrix[8] = cosine;
	sine = sySin(ang);
	matrix[5] = sine;
	matrix[7] = -sine;
	syMultiMatrix(matrix);
}

/* Rotates CurrentMatrix about the Y axis by `ang`. */
void syRotY(ANGLE ang)
{
	MATRIX matrix;
	FIXED  sine;
	FIXED  cosine;

	syMatrixStoreUnit(matrix);
	cosine = COS(ang);
	matrix[0] = cosine;
	matrix[8] = cosine;
	sine = sySin(ang);
	matrix[2] = -sine;
	matrix[6] = sine;
	syMultiMatrix(matrix);
}

/* Makes CurrentMatrix a rotation about the Y axis by `ang`. */
void syMatrixSetRotateY(ANGLE ang)
{
	FIXED *matrix = CurrentMatrix;
	FIXED  sine;
	FIXED  cosine;

	syMatrixStoreUnit(matrix);
	cosine = COS(ang);
	matrix[0] = cosine;
	matrix[8] = cosine;
	sine = sySin(ang);
	matrix[2] = -sine;
	matrix[6] = sine;
}

/* Rotates CurrentMatrix about the Z axis by `ang`. */
void syRotZ(ANGLE ang)
{
	MATRIX matrix;
	FIXED  sine;
	FIXED  cosine;

	syMatrixStoreUnit(matrix);
	cosine = COS(ang);
	matrix[0] = cosine;
	matrix[4] = cosine;
	sine = sySin(ang);
	matrix[1] = sine;
	matrix[3] = -sine;
	syMultiMatrix(matrix);
}

/* Translates CurrentMatrix by `pos` (x, y, z in 16.16). */
void syMatrixTranslateVector(const POINT pos)
{
	MATRIX matrix;

	syMatrixStoreUnit(matrix);
	matrix[MATRIX_TRANSLATION_X] = pos[0];
	matrix[MATRIX_TRANSLATION_Y] = pos[1];
	matrix[MATRIX_TRANSLATION_Z] = pos[2];
	syMultiMatrix(matrix);
}

/* Translates CurrentMatrix by `x`, `y` and `z` in 16.16. */
void syTranslate(FIXED x, FIXED y, FIXED z)
{
	MATRIX matrix;

	syMatrixStoreUnit(matrix);
	matrix[MATRIX_TRANSLATION_X] = x;
	matrix[MATRIX_TRANSLATION_Y] = y;
	matrix[MATRIX_TRANSLATION_Z] = z;
	syMultiMatrix(matrix);
}

/* Sets CurrentMatrix's translation to `pos`. */
void syMatrixSetTranslation(const POINT pos)
{
	CurrentMatrix[MATRIX_TRANSLATION_X] = pos[0];
	CurrentMatrix[MATRIX_TRANSLATION_Y] = pos[1];
	CurrentMatrix[MATRIX_TRANSLATION_Z] = pos[2];
}

/* Scales CurrentMatrix by `scale` (x, y, z factors in 16.16). */
void syMatrixScaleVector(const VECTOR scale)
{
	MATRIX matrix;

	syMatrixStoreUnit(matrix);
	matrix[0] = scale[0];
	matrix[4] = scale[1];
	matrix[8] = scale[2];
	syMultiMatrix(matrix);
}

/* Scales CurrentMatrix by `x`, `y` and `z` in 16.16. */
void syScale(FIXED x, FIXED y, FIXED z)
{
	MATRIX matrix;

	syMatrixStoreUnit(matrix);
	matrix[0] = x;
	matrix[4] = y;
	matrix[8] = z;
	syMultiMatrix(matrix);
}

/* Scales CurrentMatrix by `scale` on all three axes. */
void syMatrixScaleUniform(FIXED scale)
{
	MATRIX matrix;

	syMatrixStoreUnit(matrix);
	matrix[0] = scale;
	matrix[4] = scale;
	matrix[8] = scale;
	syMultiMatrix(matrix);
}

/* Rotates CurrentMatrix by `angles` and translates it by `pos` in one
   matrix. Reached through the stub table. */
void syMatrixRotateTranslate(Sint32 *angles, POINT pos)
{
	FIXED  sinX;
	FIXED  cosX;
	FIXED  sinY;
	FIXED  cosY;
	FIXED  sinZ;
	FIXED  cosZ;
	MATRIX matrix;

	sinX = sySin(angles[0]);
	cosX = COS(angles[0]);
	sinY = sySin(angles[1]);
	cosY = COS(angles[1]);
	sinZ = sySin(angles[2]);
	cosZ = COS(angles[2]);
	SET_VECTOR(matrix, syMulFX(cosY, cosZ), syMulFX(cosY, sinZ), -sinY);
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
	FIXED  sinX, cosX, sinY;
	FIXED  cosY;
	FIXED  sinZ;
	FIXED  cosZ;
	FIXED  axisScale;
	MATRIX matrix;

	sinX = sySin(angles[0]);
	cosX = COS(angles[0]);
	sinY = sySin(angles[1]);
	cosY = COS(angles[1]);
	sinZ = sySin(angles[2]);
	cosZ = COS(angles[2]);
	axisScale = scale[0];
	SET_VECTOR(matrix, syMulFX(axisScale, syMulFX(cosY, cosZ)),
		syMulFX(axisScale, syMulFX(cosY, sinZ)), syMulFX(axisScale, -sinY));
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

/* sySquartFX written in place, as syMatrixBuildLookAt takes it. */
#define sySquartFX(v) (sySquartIWRAM(v) << 8)

/* Builds in `out` the camera matrix of an eye at `eye` looking at `tgt`. The
   camera's sideways axis is kept level (no Y component). */
void syMatrixBuildLookAt(POINT eye, POINT tgt, MATRIX out)
{
	ASM_REGISTER(DOT_PRODUCT_ROUTINE dot, "sl");
	Uint32 code[8];
	Sint32 dx, dy, dz;
	Sint32 ux;
	Sint32 uy;
	Sint32 ex;
	Sint32 ey;
	Sint32 uz;
	Sint32 vx, vy, vz;
	Sint32 scale;
	Sint32 ez;
	Sint32 length;
	FIXED  levelSquared;
	Uint32 dividend = (Uint32)toFIXED(1) << RECIPROCAL_SHIFT;
	ASM_REGISTER(Uint32 (*divide)(Uint32 dividend, Uint32 divisor), "r9");
	ASM_REGISTER(Sint32 (*mul)(Sint32 a, Sint32 b), "fp");

	/* The three functions this calls again and again are kept by address in
	   registers Thumb code has little other use for; the call through a
	   register takes any of them. The dot-product function is copied to the
	   stack and called there. */
	mul = syMulFX;
	dot = (DOT_PRODUCT_ROUTINE)code;
	divide = syIntegerDivideUnsignedEntry;
	syDMAXCopy(RelocatedInnerProduct, code, DMA_COPY_WORDS(8), REG_DMA3);

	/* The view axis d, pointing from the target back to the eye, normalized:
	   its length in whole pixels, and toFIXED(1) over it, negated. */
	dx = tgt[0] - eye[0];
	dy = tgt[1] - eye[1];
	dz = tgt[2] - eye[2];
	length = sySquartIWRAM(
		dot(dx >> 8, dx >> 8, dy >> 8, dy >> 8, dz >> 8, dz >> 8));
	scale = divide(dividend, length) >> RECIPROCAL_SHIFT;
	scale = -scale;
	dx = mul(dx, scale);
	dy = mul(dy, scale);
	dz = mul(dz, scale);
	uz = -dx;

	/* Normalize the horizontal axis u = (dz, 0, -dx), unless d is vertical. */
	do {
		levelSquared = toFIXED(1) - mul(dy, dy);
		scale = toFIXED(1);
		if(levelSquared <= 0)
			break;
		scale = divide(dividend, sySquartFX(levelSquared)) << 1;
	} while(0);
	ux = mul(dz, scale);
	/* FAKEMATCH(temporary): u's level Y component, 0, set here; as 0. */
	uy = 0;
	uz = mul(uz, scale);

	/* Up axis v is the cross product of d and u. */
	vx = mul(dy, uz);
	vy = mul(dz, ux) - mul(dx, uz);
	vz = -mul(dy, ux);
	scale = divide(dividend, sySquartFX(dot(vx, vx, vy, vy, vz, vz))) << 1;
	vx = mul(vx, scale);
	vy = mul(vy, scale);
	vz = mul(vz, scale);

	/* Place the three axes in columns; translation uses dot products with
	   eye. */
	ex = eye[0];
	ey = eye[1];
	ez = eye[2];
	out[0] = ux;
	out[3] = uy;
	out[6] = uz;
	out[MATRIX_TRANSLATION_X] = -dot(ex, ux, ez, uz, uy, uy);
	out[1] = vx;
	out[4] = vy;
	out[7] = vz;
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
	MATRIX state;

	syMatrixBuildLookAt(eye, tgt, state);
	syMultiMatrix(state);
}

/* Sets the projection from a vertical field of view (1/65536 turns): the focal
   length, in 16.16 pixels, is half the screen's height over the tangent of half
   the angle. */
void syPerspective(Sint32 fieldOfView, FIXED nearDepth, FIXED farDepth)
{
	Sint32 halfAng;
	FIXED  sine;
	FIXED  focal;

	halfAng = fieldOfView / 2;
	sine = sySin(halfAng);
	focal = syDivFX(sine, COS(halfAng) * (DISPLAY_HEIGHT / 2));
	ProjectionWork.near = nearDepth;
	ProjectionWork.focal = focal;
	ProjectionWork.far = farDepth;
}

/* Sets the projection's focal length and depth range directly. */
void sySetScreenDist(Uint32 focal, Uint32 nearDepth, Uint32 farDepth)
{
	PROJECTION *projection = &ProjectionWork;

	projection->focal = focal;
	projection->near = nearDepth;
	projection->far = farDepth;
}

/* Transforms `point` by CurrentMatrix and projects it onto the screen into
   `screen`, returning the scale at its depth; a point outside the near and far
   depths keeps its view coordinates and returns 0. */
Sint32 syConvert3Dto2D(const POINT point, PROJECTED_POINT *screen)
{
	PROJECTED_POINT view;
	PROJECTION	   *projection;
	Sint32			depth;
	Sint32			scale;
	Sint32			x;
	Sint32			y;

	syCalcPoint(point, &view.x);
	depth = -view.z;
	projection = &ProjectionWork;
	if(depth >= projection->near && depth <= projection->far) {
		screen->z = FIXEDtoINT(depth);
		if(projection->focal != 0) {
			Uint32 shifted = (Uint32)depth >> PROJECTION_DEPTH_SHIFT;

			scale = syIntegerDivideUnsigned(
				projection->focal * (toFIXED(1) >> PROJECTION_DEPTH_SHIFT),
				shifted);
		} else {
			scale = toFIXED(33) / 25; /* 1.32 */
		}
		x = projection->centerX + syMulFX(view.x, scale) / toFIXED(1);
		y = projection->centerY - syMulFX(view.y, scale) / toFIXED(1);
		screen->x = x;
		screen->y = y;
		return scale;
	}
	*screen = view;
	return 0;
}

#undef MATRIX_DMA
#undef RECIPROCAL_SHIFT
#undef PROJECTION_DEPTH_SHIFT
#undef COS
#undef sySquartFX
