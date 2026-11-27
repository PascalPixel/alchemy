/* The tick's sprite lists: affine matrices and depth slots. Part of
   system.c. */

enum {
	OBJ_AFFINE_MATRIX_COUNT = 32, /* the OAM's rotation-scaling matrices */
	RENDER_DEPTH_SLOT_COUNT = 256 /* depth slots a sprite is linked into */
};

/* Starts a new tick of sprites: frees the affine matrices and clears the masks
   of the depth slots sprites are linked into, a bit a slot. */
void syOAMResetTick(void)
{
	OamMatrixCount = 0;
	syWordsClearEntry(OamDepthMasks, RENDER_DEPTH_SLOT_COUNT / 8);
}

/* Hands out the next OAM affine matrix for a sprite drawn with `src`'s scale
   and rotation and fills it in. Returns the matrix number, or the last
   (OBJ_AFFINE_MATRIX_COUNT - 1) when all are taken. */
Sint32 syOAMMatrixAllocFromTransform(AFFINE_TRANSFORM *src)
{
	AFFINE_MATRIX *matrix;
	Sint16		  *coefficient;
	Sint32		   xScale;
	Sint32		   yScale;
	Sint32		   ang;
	Uint8		   idx;

	xScale = (Sint16)src->scaleX;
	ang = src->ang;
	yScale = (Sint16)src->scaleY;
	idx = OamMatrixCount;
	if(idx >= OBJ_AFFINE_MATRIX_COUNT)
		return OBJ_AFFINE_MATRIX_COUNT - 1;

	matrix = &OamMatrices[idx];
	coefficient = matrix->coefficients;
	OamMatrixCount = idx + 1;
	if((xScale == yScale || xScale == -yScale) && ang == 0) {
		/* Unrotated and evenly scaled (or mirrored): pa and pd are the scale's
		   reciprocals (toFIXED(1) / scale, the 8.8 inverse of an 8.8 scale), pb
		   and pc 0, two halfwords to a row. */
		Sint32 reciprocal;
		Sint32 xReciprocal;

		reciprocal = syIntegerDivide(toFIXED(1), yScale);
		xReciprocal = reciprocal;
		if(xScale == -yScale)
			xReciprocal = -reciprocal;

		matrix->rows[0] = (Uint16)xReciprocal;
		matrix->rows[1] = (Uint32)reciprocal << 16;
	} else {
		/* The inverse of rotate-then-scale: a 16.16 sine over an 8.8 scale is
		   an 8.8 coefficient. */
		FIXED sine;
		FIXED cosine;

		sine = sySin(ang);
		cosine = syCos(ang);
		*coefficient = cosine / xScale;
		coefficient++;
		*coefficient = sine / xScale;
		coefficient++;
		*coefficient = (-sine) / yScale;
		coefficient++;
		*coefficient = cosine / yScale;
	}

	return idx;
}

/* As syOAMAddSprAtDepth, with the depth clamped below OAM_DEPTH_WINDOW, so that
   the sprite stays behind the windows' sprites. */
void syOAMAddSprAtDepthClamped(void *entry, Sint32 slot)
{
	Uint8 *mask;
	void **buckets;
	Sint32 bit;

	if((Uint32)slot > OAM_DEPTH_WINDOW - 1) {
		if(slot < 0)
			slot = 0;
		else
			slot = OAM_DEPTH_WINDOW - 1;
	}
	mask = &OamDepthMasks[(Uint32)slot >> 3];
	bit = slot & 7;
	buckets = OamDepthLists;
	if(((Uint32)*mask >> bit) & 1) {
		void **head = &buckets[slot];
		void  *prev = *head;

		*head = entry;
		*(void **)entry = prev;
	} else {
		*mask |= 1 << bit;
		buckets[slot] = entry;
		*(void **)entry = NULL;
	}
}

/* Links a sprite entry at the head of depth slot `slot`'s list (clamped to the
   RENDER_DEPTH_SLOT_COUNT slots) for this tick, and marks the slot taken. */
void syOAMAddSprAtDepth(void *entry, Sint32 slot)
{
	Uint8 *mask;
	void **head;
	Sint32 bit;

	if(slot >= RENDER_DEPTH_SLOT_COUNT)
		slot = RENDER_DEPTH_SLOT_COUNT - 1;
	if(slot < 0)
		slot = 0;
	mask = &OamDepthMasks[(Uint32)slot >> 3];
	head = &OamDepthLists[slot];
	bit = slot & 7;
	if(((Uint32)*mask >> bit) & 1) {
		void *prev = *head;

		*head = entry;
		*(void **)entry = prev;
	} else {
		*mask |= 1 << bit;
		*head = entry;
		*(void **)entry = NULL;
	}
}
