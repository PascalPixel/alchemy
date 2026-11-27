/* @file */
/* The frame's sprite lists: affine matrices, depth slots and the OAM build. */
/* Part of system.c. */

enum {
	OBJ_AFFINE_MATRIX_COUNT = 32, /* the OAM's rotation-scaling matrices */
	RENDER_DEPTH_SLOT_COUNT = 256 /* depth slots a sprite is linked into */
};

/* Starts a new frame of sprites: frees the affine matrices and empties the */
/* RENDER_DEPTH_SLOT_COUNT depth slots sprites are linked into. */
void syOamResetFrame(void) {
	OamMatrixCount = 0;
	/* IwramClearWords(destination, bytes) clears the slot heads. */
	_call_via_r3(((u32)&OamDepthLists),
				 RENDER_DEPTH_SLOT_COUNT * sizeof(void *),
				 ((u32)&OamMatrixCount), (u32)IwramClearWords);
}

typedef void (*OAM_BUILD_ROUTINE)(void *work);

/* Hands out the next OAM affine matrix for a sprite drawn with `source`'s */
/* scale and rotation and fills it in. Returns the matrix number, 0 to 31. */
s32 syOamMatrixAllocFromTransform(AFFINE_TRANSFORM *source) {
	OAM_MATRIX *matrix;
	s16 *coefficient;
	s32 xScale;
	s32 yScale;
	s32 angle;
	u8 index;

	index = OamMatrixCount;
	xScale = (s16)source->scaleX;
	yScale = (s16)source->scaleY;
	angle = source->ang;
	if(index >= OBJ_AFFINE_MATRIX_COUNT)
		return 0;

	matrix = &OamMatrices[index];
	coefficient = matrix->coefficients;
	if((xScale == yScale || -xScale == yScale) && angle == 0) {
		/* Unrotated and evenly scaled (or mirrored): pa and pd are the scale's
		 */
		/* reciprocals (FIXED_ONE / scale, the 8.8 inverse of an 8.8 scale), pb
		 * and */
		/* pc 0, two halfwords to a row. */
		s32 reciprocal;
		s32 xReciprocal;

		reciprocal = syIntegerDivide(FIXED_ONE, yScale);
		xReciprocal = reciprocal;
		if(-xScale == yScale)
			xReciprocal = -reciprocal;

		matrix->rows[0] = (u16)xReciprocal;
		matrix->rows[1] = (u32)reciprocal << 16;
	}
	else {
		/* The inverse of rotate-then-scale: a 16.16 sine over an 8.8 scale is
		 */
		/* an 8.8 coefficient. */
		FIXED sine;
		FIXED cosine;

		sine = sySin(angle);
		cosine = syCos(angle);
		*coefficient = cosine / xScale;
		coefficient++;
		*coefficient = sine / xScale;
		coefficient++;
		*coefficient = (-sine) / yScale;
		coefficient++;
		*coefficient = cosine / yScale;
	}

	OamMatrixCount = index + 1;
	return index;
}

/* Links a sprite entry into depth slot `slot` (clamped to the */
/* RENDER_DEPTH_SLOT_COUNT slots) for this frame. */
void syOamAddSprAtDepth(void *slotEntry, s32 slot) {
	void **head;
	void *previousHead;

	if(slot >= RENDER_DEPTH_SLOT_COUNT)
		slot = RENDER_DEPTH_SLOT_COUNT - 1;
	if(slot < 0)
		slot = 0;
	head = &OamDepthLists[slot];
	previousHead = *head;
	*head = slotEntry;
	*(void **)slotEntry = previousHead;
}

/* Builds this frame's OAM list in `work` from the sprites linked into the */
/* depth slots, with a RAM copy of the ARM builder. */
void syOamBuildBuffer(void *work) {
	OAM_BUILD_ROUTINE routine;

	syRamCodeLoad(routine, syOamBuildBufferKernel,
				  syOamBuildBufferKernelCodeSize);
	routine(work);
	syHeapRelease((void *)routine);
}
