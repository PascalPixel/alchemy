// The tick's sprite lists: affine matrices, depth slots and the OAM build. Part of system.c.

enum
{
	OBJ_AFFINE_MATRIX_COUNT = 32, // the OAM's rotation-scaling matrices
	RENDER_DEPTH_SLOT_COUNT = 256 // depth slots a sprite is linked into
};

// Starts a new tick of sprites: frees the affine matrices and empties the RENDER_DEPTH_SLOT_COUNT
// depth slots sprites are linked into.
void syOAMResetTick(void)
{
	OamMatrixCount = 0;
	// syIWRAMWordsClear(destination, bytes) clears the slot heads.
	_call_via_r3(((Uint32)&OamDepthLists),
	             RENDER_DEPTH_SLOT_COUNT * sizeof(void *),
	             ((Uint32)&OamMatrixCount),
	             (Uint32)syIWRAMWordsClear);
}

typedef void (*OAM_BUILD_ROUTINE)(void *work);

// Hands out the next OAM affine matrix for a sprite drawn with `src`'s scale and rotation and fills
// it in. Returns the matrix number, 0 to 31.
Sint32       syOAMMatrixAllocateFromTransform(AFFINE_TRANSFORM *src)
{
	OAM_MATRIX *matrix;
	Sint16     *coefficient;
	Sint32      xScale;
	Sint32      yScale;
	Sint32      angle;
	Uint8       index;

	index  = OamMatrixCount;
	xScale = (Sint16)src->scaleX;
	yScale = (Sint16)src->scaleY;
	angle  = src->angle;
	if (index >= OBJ_AFFINE_MATRIX_COUNT)
		return 0;

	matrix      = &OamMatrices[index];
	coefficient = matrix->coefficients;
	if ((xScale == yScale || -xScale == yScale) && angle == 0)
	{
		// Unrotated and evenly scaled (or mirrored): pa and pd are the scale's reciprocals
		// (toFIXED(1) / scale, the 8.8 inverse of an 8.8 scale), pb and pc 0, two halfwords to a
		// row.
		Sint32 reciprocal;
		Sint32 xReciprocal;

		reciprocal  = syIntegerDivide(toFIXED(1), yScale);
		xReciprocal = reciprocal;
		if (-xScale == yScale)
			xReciprocal = -reciprocal;

		matrix->rows[0] = (Uint16)xReciprocal;
		matrix->rows[1] = (Uint32)reciprocal << 16;
	}
	else
	{
		// The inverse of rotate-then-scale: a 16.16 sine over an 8.8 scale is an 8.8 coefficient.
		FIXED sine;
		FIXED cosine;

		sine         = sySin(angle);
		cosine       = syCos(angle);
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

// Links a sprite entry into depth slot `slot` (clamped to the RENDER_DEPTH_SLOT_COUNT slots) for
// this tick.
void syOAMAddSpriteAtDepth(void *slotEntry, Sint32 slot)
{
	void **head;
	void  *prevHead;

	if (slot >= RENDER_DEPTH_SLOT_COUNT)
		slot = RENDER_DEPTH_SLOT_COUNT - 1;
	if (slot < 0)
		slot = 0;
	head                = &OamDepthLists[slot];
	prevHead            = *head;
	*head               = slotEntry;
	*(void **)slotEntry = prevHead;
}

// Builds this tick's OAM list in `work` from the sprites linked into the depth slots, with a RAM
// copy of the ARM builder.
void syOAMBuildBuffer(void *work)
{
	OAM_BUILD_ROUTINE routine;

	RAM_CODE_LOAD(routine, syOAMBuildBufferKernel, syOAMBuildBufferKernelCodeSize);
	routine(work);
	syHeapRelease((void *)routine);
}
