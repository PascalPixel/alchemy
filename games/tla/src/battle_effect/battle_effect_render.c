/* The model renderers and projections the battle effects draw with, which
   follow Iris in the battle effect code. Part of battle_effect.c. */

/* The first three renderers draw a mesh straight into the canvas in C, filled,
   textured or as a wireframe, and nothing calls them; the effects draw through
   btRenderDrawModelList and the ARM kernels it copies into IWRAM. */

/* Swaps two integers without a temporary. */
#define XOR_SWAP(a, b) \
	{ \
		(a) ^= (b); \
		(b) ^= (a); \
		(a) ^= (b); \
	}

/* A triangle of the model: where its three corners start in the vertex buffer
   (in words) and its colour. */
typedef struct SOLID_FACE {
	Sint32 corner[3];
	Uint8  unread[12]; /* the renderer skips them */
	Uint8  col;
	Uint8  padding[3];
} SOLID_FACE;

/* Fills one scanline of the canvas (8x8 tiles, 16 to a row) from xa to xb,
   keeping the brighter of each pixel's colour and color. */
#define FILL_SPAN(row, xa, xb) \
	{ \
		Sint32 left = FIXEDtoINT(xa); \
		Sint32 right = FIXEDtoINT(xb); \
		Sint32 rowOfs; \
		Sint32 x; \
\
		if(left > right) \
			XOR_SWAP(left, right); \
		rowOfs = CANVAS_ROW_OFFSET(row); \
		for(x = left; x != right; x++) { \
			Sint32 pixel = ((x >> 3) << 6) + (x & 7) + rowOfs; \
\
			if(canvas[pixel] < col) \
				canvas[pixel] = col; \
		} \
	}

/* Unused: nothing calls it. A solid renderer, the filled twin of
   btRenderDrawMeshWireframe. Transforms the vertexCnt points in the sheet
   block by the current matrix, then fills every face of the effect state's list
   that faces the camera into the canvas, centred at (64, 64), keeping the
   brighter colour where faces overlap. Each triangle is sorted by y and filled
   in two halves, top to middle and middle to bottom, along 16.16 edge
   slopes. */
UNUSED void btRenderFillMesh(Sint32 vertexCnt, Sint32 faceCnt)
{
	Uint8	   *canvas = RamHeapSlots->battleCanvas;
	Sint32	   *vertices = RamHeapSlots->battleSheet;
	SOLID_FACE *faces = (SOLID_FACE *)RamHeapSlots->battleEff;
	Sint32		i;
	Sint32	   *ax, *bx, *cx, *cy, *ay, *by;
	Sint32		facing;
	Sint32 (*mul)(Sint32, Sint32);
	FIXED  slopeA, slopeB;
	Sint32 top, middle, bottom;
	Uint8  col;
	FIXED  edgeA, edgeB;
	Sint32 row;

	/* From here on, the count of words: three to a point. */
	vertexCnt *= 3;
	for(i = 0; i != vertexCnt; i += 3)
		syCalcPoint(&vertices[i], &vertices[i]);

	for(i = 0; i != faceCnt; i++) {
		/* The three corners of each face are copied after the vertices. */
		ax = (Sint32 *)(vertexCnt * 4 + (Sint32)vertices);
		bx = ax + 3;
		cx = ax + 6;
		cy = ax + 7;
		ay = ax + 1;
		by = ax + 4;
		mul = syMulFX;
		syDMAXCopy(
			&vertices[faces[i].corner[0]], ax, DMA_COPY_WORDS(3), REG_DMA3);
		syDMAXCopy(
			&vertices[faces[i].corner[1]], bx, DMA_COPY_WORDS(3), REG_DMA3);
		syDMAXCopy(
			&vertices[faces[i].corner[2]], cx, DMA_COPY_WORDS(3), REG_DMA3);

		/* The z of the edges' cross product: negative for a face turned
		   away. */
		facing = mul(*bx - *ax, *cy - *ay) - mul(*cx - *ax, *by - *ay);
		if(facing >= 0) {
			col = faces[i].col;
			*ax += toFIXED(64);
			*ay += toFIXED(64);
			*bx += toFIXED(64);
			*by += toFIXED(64);
			*cx += toFIXED(64);
			*cy += toFIXED(64);

			if(*ay > *by) {
				XOR_SWAP(*ax, *bx);
				*ay ^= *by;
				bx[1] = *by ^ *ay;
				*ay ^= bx[1];
			}
			if(*by > *cy) {
				XOR_SWAP(*bx, *cx);
				bx[1] = *by ^ *cy;
				cx[1] = *cy ^ bx[1];
				*by ^= cx[1];
				if(*ay > *by) {
					XOR_SWAP(*ax, *bx);
					*ay ^= *by;
					bx[1] = *by ^ *ay;
					*ay ^= bx[1];
				}
			}

			top = FIXEDtoINT(ax[1]);
			middle = FIXEDtoINT(ax[4]);
			bottom = FIXED_INTEGER_PART(ax[7]);
			if(top != middle)
				slopeA = syDivFX(ax[4] - ax[1], ax[3] - ax[0]);
			if(top != bottom)
				slopeB = syDivFX(ax[7] - ax[1], ax[6] - ax[0]);

			/* The upper half, between the short edge and the long one. */
			edgeB = ax[0];
			edgeA = ax[0];
			for(row = top; row != middle; row++) {
				FILL_SPAN(row, edgeA, edgeB);
				edgeA += slopeA;
				edgeB += slopeB;
			}

			/* The lower half, from the middle corner. */
			if(middle != bottom)
				slopeA = syDivFX(ax[7] - ax[4], ax[6] - ax[3]);
			edgeA = ax[3];
			for(row = middle; row != bottom; row++) {
				FILL_SPAN(row, edgeA, edgeB);
				edgeA += slopeA;
				edgeB += slopeB;
			}
		}
	}
}

/* One mesh face: its three corners as word offsets into the vertex buffer
   (vertex index * 3), padded to an effect step's 28 bytes. */
typedef struct MESH_POLYGON {
	Sint32 vertex[3];
	Sint32 padding[4];
} MESH_POLYGON;

/* The size of v. */
#define DIFF_ABS(v) ((v) >= 0 ? (v) : -(v))

/* One textured scanline at canvas row y, from xA to xB, keeping the brighter of
   the canvas and texture colour. */
#define DRAW_SPAN(y) \
	{ \
		Uint8 *row = state->sheet; \
		Sint32 left, right, u, v; \
		Sint32 rowOfs; \
		Sint32 x; \
\
		row += ((FIXEDtoINT(vA)) << textureShift) + (FIXEDtoINT(uA)); \
		left = FIXEDtoINT(xA); \
		right = FIXEDtoINT(xB); \
		u = 0; \
		v = 0; \
		if(left > right) \
			XOR_SWAP(left, right); \
		rowOfs = CANVAS_ROW_OFFSET(y); \
		for(x = left; x != right; x++) { \
			Sint32 texel = \
				(FIXEDtoINT(u)) + ((FIXEDtoINT(v)) << textureShift); \
			Sint32 at = ((x >> 3) << 6) + (x & 7) + rowOfs; \
\
			if(canvas[at] < row[texel]) \
				canvas[at] = row[texel]; \
			u += dudx; \
			v += dvdx; \
		} \
	}

/* The texture steps along a scanline, from the middle corner's row. */
#define STEP_RIGHTWARD() \
	{ \
		width = DIFF_ABS(ax[3] - ax[0] - syMulFX(dxB, ax[4] - ax[1])); \
		dudx = \
			syDivFX(width, uv[1][0] - uv[0][0] - syMulFX(duB, ax[4] - ax[1])); \
		dvdx = \
			syDivFX(width, uv[1][1] - uv[0][1] - syMulFX(dvB, ax[4] - ax[1])); \
		XOR_SWAP(duA, duB); \
		XOR_SWAP(dvA, dvB); \
	}
#define STEP_LEFTWARD() \
	{ \
		width = DIFF_ABS(syMulFX(dxB, ax[4] - ax[1]) - (ax[3] - ax[0])); \
		dudx = syDivFX( \
			width, syMulFX(duB, ax[4] - ax[1]) - (uv[1][0] - uv[0][0])); \
		dvdx = syDivFX( \
			width, syMulFX(dvB, ax[4] - ax[1]) - (uv[1][1] - uv[0][1])); \
	}

/* Unused: nothing calls it. Transforms vertexCnt vertices (16.16 x, y, z in
   the heap's vertex buffer) in place, then draws each front-facing face of the
   mesh at the start of the effect state as an affine-textured triangle: uvs
   gives each face's three texture corners (16.16), the texture is the effect
   sprite sheet, textureShift its row length as a power of two. Coordinates are
   offset by 64 pixels into the canvas. */
UNUSED void btRenderDrawTexturedMesh(
	Sint32 vertexCnt, Sint32 faceCnt, Sint32 (*uvs)[3][2], Sint32 textureShift)
{
	Sint32				 i;
	Sint32				*vertices;
	Uint8				*canvas;
	MESH_POLYGON		*faces;
	Sint32				*ax, *bx, *cx, *cy, *ay, *by;
	Sint32				 uv[3][2];
	Sint32				 width;
	FIXED				 dxA, dxB, duA, dvA, duB, dvB;
	FIXED				 xA, xB, uA, vA, uB, vB;
	Sint32				 top, middle, bottom;
	Sint32				 dudx, dvdx;
	BATTLE_EFFECT_STATE *state;
	Uint32				 y;

	vertices = RamHeapSlots->battleSheet;
	faces = (MESH_POLYGON *)RamHeapSlots->battleEff;
	canvas = RamHeapSlots->battleCanvas;
	vertexCnt *= 3;
	for(i = 0; i != vertexCnt; i += 3)
		syCalcPoint(&vertices[i], &vertices[i]);

	for(i = 0; i != faceCnt; i++) {
		/* The three corners of each face are copied after the vertices. */
		ax = (Sint32 *)(vertexCnt * 4 + (Sint32)vertices);
		bx = &vertices[vertexCnt + 3];
		cx = &vertices[vertexCnt + 6];
		cy = &vertices[vertexCnt + 7];
		ay = &vertices[vertexCnt + 1];
		by = &vertices[vertexCnt + 4];
		syDMAXCopy(
			&vertices[faces[i].vertex[0]], ax, DMA_COPY_WORDS(3), REG_DMA3);
		syDMAXCopy(
			&vertices[faces[i].vertex[1]], bx, DMA_COPY_WORDS(3), REG_DMA3);
		syDMAXCopy(
			&vertices[faces[i].vertex[2]], cx, DMA_COPY_WORDS(3), REG_DMA3);

		/* Back faces are skipped. */
		if(syMulFX(*bx - *ax, *cy - *ay) - syMulFX(*cx - *ax, *by - *ay) >= 0) {
			state = RamHeapSlots->battleEff;
			syDMAXCopy(uvs[i], uv, DMA_COPY_WORDS(6), REG_DMA3);
			*ax += toFIXED(64);
			*ay += toFIXED(64);
			*bx += toFIXED(64);
			*by += toFIXED(64);
			*cx += toFIXED(64);
			*cy += toFIXED(64);

			/* Sort the corners from top to bottom. bx[1] is *by and cx[1] is
			 *cy. */
			if(*ay > *by) {
				XOR_SWAP(*ax, *bx);
				*ay ^= *by;
				bx[1] = *by ^ *ay;
				*ay ^= bx[1];
				XOR_SWAP(uv[0][0], uv[1][0]);
				XOR_SWAP(uv[0][1], uv[1][1]);
			}
			if(*by > *cy) {
				XOR_SWAP(*bx, *cx);
				bx[1] = *by ^ *cy;
				cx[1] = *cy ^ bx[1];
				*by ^= cx[1];
				XOR_SWAP(uv[1][0], uv[2][0]);
				XOR_SWAP(uv[1][1], uv[2][1]);
				if(*ay > *by) {
					XOR_SWAP(*ax, *bx);
					*ay ^= *by;
					bx[1] = *by ^ *ay;
					*ay ^= bx[1];
					XOR_SWAP(uv[0][0], uv[1][0]);
					XOR_SWAP(uv[0][1], uv[1][1]);
				}
			}

			top = FIXEDtoINT(ax[1]);
			middle = FIXEDtoINT(ax[4]);
			bottom = FIXEDtoINT(ax[7]);
			if(top != middle) {
				Sint32 height = ax[4] - ax[1];

				dxA = syDivFX(height, ax[3] - ax[0]);
				duA = syDivFX(height, uv[1][0] - uv[0][0]);
				dvA = syDivFX(height, uv[1][1] - uv[0][1]);
			}
			if(top != bottom) {
				Sint32 height = ax[7] - ax[1];

				dxB = syDivFX(height, ax[6] - ax[0]);
				duB = syDivFX(height, uv[2][0] - uv[0][0]);
				dvB = syDivFX(height, uv[2][1] - uv[0][1]);
			}
			xB = xA = ax[0];
			uA = uB = uv[0][0];
			vA = vB = uv[0][1];

			if(ax[3] >= ax[6])
				STEP_RIGHTWARD()
			else
				STEP_LEFTWARD()

			/* The upper half. */
			for(y = top; y != middle; y++) {
				DRAW_SPAN(y);
				xA += dxA;
				xB += dxB;
				uA += duA;
				vA += dvA;
				uB += duB;
				vB += dvB;
			}

			/* The lower half: edge A now runs from the middle corner to the
			   bottom. */
			if(middle != bottom) {
				Sint32 height = ax[7] - ax[4];

				dxA = syDivFX(height, ax[6] - ax[3]);
				if(ax[3] >= ax[6]) {
					duB = duA;
					dvB = dvA;
					uB = uA;
					vB = vA;
				}
				duA = syDivFX(height, uv[2][0] - uv[1][0]);
				dvA = syDivFX(height, uv[2][1] - uv[1][1]);
				xA = ax[3];
				uA = uv[1][0];
				vA = uv[1][1];
				if(ax[0] <= xA) {
					STEP_RIGHTWARD();
					uA = uB;
					vA = vB;
				} else
					STEP_LEFTWARD()
				for(y = middle; y != bottom; y++) {
					DRAW_SPAN(y);
					xA += dxA;
					xB += dxB;
					uA += duA;
					vA += dvA;
					/* LEFTOVER(template): edge B's steps, as btRenderFillMesh's
					 * lower half steps both its edges and this routine's upper
					 * half steps all six values, idle here; as the loop without
					 * them. */
					uB += duB;
					vB += dvB;
				}
			}
		}
	}
}

/* A triangle of the wireframe model: where its three corners start in the
   vertex buffer (in words) and the colour its edges are drawn in. */
typedef struct WIRE_FACE {
	Sint32 corner[3];
	Uint8  unread[12]; /* the renderer skips them */
	Uint8  col;
	Uint8  padding[3];
} WIRE_FACE;

/* Unused: nothing calls it. A wireframe renderer. Transforms the vertexCnt
   points in the sheet block by the current matrix, then draws the three edges
   of every face of the effect state's list that faces the camera (its corners
   run counter-clockwise on screen) as clipped lines into the canvas, centred at
   (64, 64). */
UNUSED void btRenderDrawMeshWireframe(Sint32 vertexCnt, Sint32 faceCnt)
{
	Sint32	  *vertices = RamHeapSlots->battleSheet;
	WIRE_FACE *faces = (WIRE_FACE *)RamHeapSlots->battleEff;
	Sint32	   i;
	Sint32	   facing;
	Sint32	   col;

	vertexCnt *= 3;
	for(i = 0; i != vertexCnt; i += 3)
		syCalcPoint(&vertices[i], &vertices[i]);

	/* The three corners of each face are copied after the vertices. */
	for(i = 0; i != faceCnt; i++) {
		Sint32 *x0 = (Sint32 *)(vertexCnt * 4 + (Sint32)vertices);
		Sint32 *x1 = x0 + 3;
		Sint32 *x2 = x0 + 6;
		Sint32 *y2 = x0 + 7;
		Sint32 *y0 = x0 + 1;
		Sint32 *y1 = x0 + 4;

		syDMAXCopy(
			&vertices[faces[i].corner[0]], x0, DMA_COPY_WORDS(3), REG_DMA3);
		syDMAXCopy(
			&vertices[faces[i].corner[1]], x1, DMA_COPY_WORDS(3), REG_DMA3);
		syDMAXCopy(
			&vertices[faces[i].corner[2]], x2, DMA_COPY_WORDS(3), REG_DMA3);

		/* The z of the edges' cross product: negative for a face turned
		   away. */
		facing = syMulFX(*x1 - *x0, *y2 - *y0) - syMulFX(*x2 - *x0, *y1 - *y0);
		if(facing >= 0) {
			col = faces[i].col;
			*x0 += toFIXED(64);
			*y0 += toFIXED(64);
			*x1 += toFIXED(64);
			*y1 += toFIXED(64);
			*x2 += toFIXED(64);
			*y2 += toFIXED(64);
			btEffectDrawClippedCanvasLine(FIXED_INTEGER_PART(*x0),
				FIXED_INTEGER_PART(*y0), FIXED_INTEGER_PART(*x1),
				FIXED_INTEGER_PART(*y1), col);
			btEffectDrawClippedCanvasLine(FIXED_INTEGER_PART(*x1),
				FIXED_INTEGER_PART(*y1), FIXED_INTEGER_PART(*x2),
				FIXED_INTEGER_PART(*y2), col);
			btEffectDrawClippedCanvasLine(FIXED_INTEGER_PART(*x0),
				FIXED_INTEGER_PART(*y0), FIXED_INTEGER_PART(*x2),
				FIXED_INTEGER_PART(*y2), col);
		}
	}
}

/* btEffectBuildBlitter(slot, 7, 7, flags, mode) for a 128 by 128 canvas, with
   the flags and the mode packed into one word: mode << 4 | flags. */
Sint32 btEffectLoadBlitter(Sint32 slot, Sint32 modeAndFlg)
{
	return btEffectBuildBlitter(
		slot, 7, 7, modeAndFlg & 0xf, (Uint32)modeAndFlg >> 4);
}

/* btEffectBuildBlitter walks The Lost Age's blitter template once and writes
   the blitter's ARM code into a heap slot: a list of steps with short copies as
   block moves and longer ones by DMA, its runs those of BLITTER_ADDITIVE,
   BLITTER_MAX, BLITTER_TRANSPARENT or a plain copy, and the second run copied
   from the first by its measured length. */

/* Copies two words from source to destination with one block load and store. */
static __inline__ void CopyWords2(const void *src, void *dst)
{
	ASM_REGISTER(const void *from, "r0") = src;
	ASM_REGISTER(void *to, "r1") = dst;
	ASM_COPY_WORDS_2(from, to);
}

/* Copies three words from source to destination with one block load and
   store. */
static __inline__ void CopyWords3(const void *src, void *dst)
{
	ASM_REGISTER(const void *from, "r0") = src;
	ASM_REGISTER(void *to, "r1") = dst;
	ASM_COPY_WORDS_3(from, to);
}

/* Copies four words from source to destination with one block load and
   store. */
static __inline__ void CopyWords4(const void *src, void *dst)
{
	ASM_REGISTER(const void *from, "r0") = src;
	ASM_REGISTER(void *to, "r1") = dst;
	ASM_COPY_WORDS_4(from, to);
}

/* The most bytes a built blitter takes. */
#define BLITTER_BYTES	   436
/* The run fragments' lengths in words: eight pixels copied, eight copied where
   not 0, and four kept where brighter or added, each laid twice. */
#define COPY8_WORDS		   16
#define TRANSPARENT8_WORDS 25
#define MAX4_WORDS		   16
#define COLOR_WORDS		   14

/* An ARM branch's 24-bit word offset from `site` to `target`. */
#define BRANCH_FIELD(site, target) \
	((((Uint32)(target) - (Uint32)(site) - 8) >> 2) & 0xffffff)

/* Writing a blitter at dst: a word; words copied from a fragment, by CPU or
   DMA, once or twice; a branch to target; or the branch at site aimed here. */
#define PUT(word) \
	do { \
		*dst++ = (word); \
	} while(0)
#define COPY_FROM(from, n) \
	CopyWords##n(from, dst); \
	dst += (n)
#define COPY(n) \
	do { \
		COPY_FROM(src, n); \
		src += (n); \
	} while(0)
#define COPY_LONG(n) \
	do { \
		syDMAXCopy(src, dst, control | (n), REG_DMA3); \
		dst += (n); \
		src += (n); \
	} while(0)
#define COPY_DMA(from, n) \
	syDMAXCopy(from, dst, DMA_COPY_WORDS(n), REG_DMA3); \
	dst += (n)
#define COPY_TWICE(from, n) \
	{ \
		const Uint32 *part = (from); \
		Uint32		  control = DMA_COPY_WORDS(n); \
		syDMAXCopy(part, dst, control, REG_DMA3); \
		dst += (n); \
		syDMAXCopy(part, dst, control, REG_DMA3); \
		dst += (n); \
	}
#define PUT_BRANCH(target) \
	*dst = *src++ + BRANCH_FIELD(dst, target); \
	dst++
#define PATCH(site) \
	do { \
		*(site) |= BRANCH_FIELD(site, dst); \
	} while(0)

/* Writes a drawing function for the canvas into heap slot `slot`, built from
   the template BlitterTemplate for one clip, mirroring and blend mode. */
Sint32 btEffectBuildBlitter(
	Sint32 slot, Sint32 widthShift, Sint32 heightShift, Sint32 flg, Uint32 mode)
{
	const Uint32 *src;
	Uint32		 *dst;
	Uint32		 *noRows;
	Uint32		 *noColumns;
	Uint32		 *row;
	Uint32		 *skip;
	Uint32		 *run;
	Sint32		  runSize;
	Uint32		 *runCopy;
	Uint32		  control;
	/* LEFTOVER(frame): an unused local the ROM's frame reserves and never
	 * touches. */
	Uint32 frm[34];

	src = BlitterTemplate;
	/* FAKEMATCH(signature): dst, a Uint32 *, holds the blitter's size before it
	 * holds the allocation. */
	dst = (Uint32 *)BLITTER_BYTES;
	dst = syHeapSlotAlloc(slot, (Sint32)dst);

	/* Block 1, the entry code. */
	COPY(4);
	if(mode == BLITTER_ADDITIVE) {
		COPY_FROM(BattleEffectMask, 3);
	}

	if((flg & COMPOSITE_FLIP) == COMPOSITE_FLIP_X) {
		COPY_FROM(src, 3);
	}
	src += 3;
	if((flg & COMPOSITE_FLIP) == COMPOSITE_FLIP_Y) {
		COPY_FROM(src, 4);
	}
	src += 4;
	if((flg & COMPOSITE_FLIP) == COMPOSITE_FLIP) {
		COPY_FROM(src, 3);
	}
	src += 3;
	if((flg & COMPOSITE_FLIP) == 0) {
		PUT(src[0]);
	}
	src += 1;

	/* Block 2, the vertical clip: the height enters the last two words. */
	if((flg & COMPOSITE_CLIP_Y) != 0) {
		COPY(2);
		if((flg & COMPOSITE_FLIP_Y) != 0) {
			PUT(src[0]);
		} else {
			PUT(src[1]);
		}
		src += 2;
		COPY(2);
		PUT(src[0] + (1 << heightShift));
		PUT(src[1] + (1 << heightShift));
		src -= 6;
	}
	src += 8;

	/* Block 3, the columns: the empty-rows skip, the width mask or the twelve
	   clip words, the empty-columns skip, then the rest of the set-up. */
	PUT(*src++);
	noRows = dst;
	PUT(*src++);

	if((flg & COMPOSITE_CLIP_X) == 0) {
		PUT(src[0] + (1 << widthShift) - 1);
	}
	src += 1;
	if((flg & COMPOSITE_CLIP_X) != 0) {
		PUT(src[0]);
		PUT(src[1]);
		if((flg & COMPOSITE_FLIP_X) != 0) {
			PUT(src[2]);
			PUT(src[3]);
		} else {
			PUT(src[4]);
			PUT(src[5]);
		}
		PUT(src[6]);
		PUT(src[7]);
		PUT(src[8] + BlitterImmediatePowers[widthShift]);
		if((flg & COMPOSITE_FLIP_X) != 0) {
			PUT(src[9]);
		} else {
			PUT(src[10]);
		}
		PUT(src[11] + BlitterImmediatePowers[widthShift]);
	}
	src += 12;

	PUT(*src++);
	noColumns = dst;
	PUT(*src++);

	control = DMA_COPY_WORDS(0);
	COPY_LONG(6);

	PUT(*src++);
	if((flg & COMPOSITE_CLIP_X) == 0) {
		PUT(src[0]);
	}
	src += 1;

	COPY_LONG(5);

	/* Block 4, the row loop: its head, the leading partial run, the full-cell
	   loop, the trailing partial run and the step back to the head. */
	row = dst;
	PUT(src[0]);
	PUT(src[1] + (1 << (heightShift - 3)) - 1);
	PUT(src[2] + ((widthShift - 3) << 7));
	PUT(src[3]);
	src += 4;

	if((flg & COMPOSITE_CLIP_X) == 0) {
		PUT(src[0]);
	}
	src += 1;

	skip = dst + 1;
	COPY(2);
	if((flg & COMPOSITE_FLIP_X) != 0) {
		PUT(src[0]);
	} else {
		PUT(src[1]);
	}
	src += 2;

	run = dst;
	runCopy = dst;
	switch(mode) {
	default: {
		const Uint32 *part;

		if((flg & COMPOSITE_FLIP_X) != 0) {
			part = BattleEffectCopyReverse;
		} else {
			part = BattleEffectCopyForward;
		}
		COPY_FROM(part, 2);
		break;
	}
	case BLITTER_TRANSPARENT: {
		const Uint32 *part;

		if((flg & COMPOSITE_FLIP_X) != 0) {
			part = BattleEffectTransparentReverse;
		} else {
			part = BattleEffectTransparentForward;
		}
		COPY_FROM(part, 4);
		break;
	}
	case BLITTER_MAX: {
		const Uint32 *part;

		if((flg & COMPOSITE_FLIP_X) != 0) {
			part = BattleEffectMaxReverse;
		} else {
			part = BattleEffectMaxForward;
		}
		COPY_FROM(part, 4);
		break;
	}
	case BLITTER_ADDITIVE:
		if((flg & COMPOSITE_FLIP_X) != 0) {
			COPY_DMA(BattleEffectAdditiveReverse, 6);
		} else {
			COPY_DMA(BattleEffectAdditiveForward, 6);
		}
		break;
	}

	PUT(*src++);
	PUT_BRANCH(run);
	runSize = (Uint8 *)dst - (Uint8 *)runCopy;
	PUT(*src++);
	if((flg & COMPOSITE_CLIP_X) == 0) {
		PUT(src[0]);
		PUT(src[1]);
	}
	src += 2;
	PATCH(skip);

	skip = dst + 1;
	COPY(2);

	run = dst;
	switch(mode) {
	default:
		if((flg & COMPOSITE_FLIP_X) != 0) {
			COPY_DMA(BattleEffectCopy8Reverse, COPY8_WORDS);
		} else {
			COPY_DMA(BattleEffectCopy8Forward, COPY8_WORDS);
		}
		break;
	case BLITTER_TRANSPARENT:
		if((flg & COMPOSITE_FLIP_X) != 0) {
			COPY_DMA(BattleEffectTransparent8Reverse, TRANSPARENT8_WORDS);
		} else {
			COPY_DMA(BattleEffectTransparent8Forward, TRANSPARENT8_WORDS);
		}
		break;
	case BLITTER_MAX:
		if((flg & COMPOSITE_FLIP_X) != 0) {
			COPY_TWICE(BattleEffectMax4Reverse, MAX4_WORDS);
		} else {
			COPY_TWICE(BattleEffectMax4Forward, MAX4_WORDS);
		}
		break;
	case BLITTER_ADDITIVE:
		if((flg & COMPOSITE_FLIP_X) != 0) {
			COPY_TWICE(BattleEffectColorReverse, COLOR_WORDS);
		} else {
			COPY_TWICE(BattleEffectColorForward, COLOR_WORDS);
		}
		break;
	}

	PUT(*src++);
	if((flg & COMPOSITE_CLIP_X) == 0) {
		PUT(src[0]);
		PUT(src[1]);
	}
	src += 2;

	PUT(*src++);
	PUT_BRANCH(run);
	PATCH(skip);

	PUT(*src++);
	skip = dst;
	PUT(*src++);
	if((flg & COMPOSITE_FLIP_X) != 0) {
		PUT(src[0]);
		src += 2;
	} else {
		PUT(src[1]);
		src += 2;
	}
	{
		Sint32 cnt = (Uint32)runSize / 4 * 4;

		/* FAKEMATCH(dead): the division by 4 rounded towards zero by hand,
		 * under a test never true of a count of bytes. */
		if(cnt < 0)
			cnt += 3;
		syDMAXCopy(runCopy, dst, DMA_COPY_WORDS(cnt >> 2), REG_DMA3);
		dst += (Uint32)runSize / 4;
		/* FAKEMATCH(dead): count set to 0, a store nothing reads. */
		cnt = 0;
	}
	PATCH(skip);

	COPY(3);

	PUT_BRANCH(row);
	PATCH(noRows);
	PATCH(noColumns);

	/* Block 5, the exit, where both skips of block 3 land. */
	COPY(2);
	if(src == (const Uint32 *)BlitterImmediatePowers)
		return TRUE;
	return FALSE;
}

/* Unused: nothing calls it. Copies btRenderFillMeshKernel
   (render_fill_mesh_kernels.s), an ARM build of btRenderFillMesh: the same
   transform of vertexCnt points centred at (64, 64), back faces dropped and
   each triangle sorted by y with XOR swaps, into a heap block (DMA 3, words),
   runs it there and frees the block. */
UNUSED void btRenderRunFillMeshKernel(Sint32 vertexCnt, Sint32 faceCnt)
{
	void (*routine)(Sint32, Sint32);

	RAM_CODE_LOAD(
		routine, btRenderFillMeshKernel, btRenderFillMeshKernelCodeSize);
	routine(vertexCnt, faceCnt);
	syHeapRelease(routine);
}

/* Copies the ARM vertex kernel into a heap block and runs it: count packed
   Sint8 vertices through the current transform into screen coordinates. */
void btRenderTransformVertices(const void *corners, void *vertices, Sint32 cnt)
{
	void (*routine)(const void *, void *, Sint32);

	RAM_CODE_LOAD(
		routine, btRenderTransformKernel, btRenderTransformKernelCodeSize);
	routine(corners, vertices, cnt);
	syHeapRelease(routine);
}

/* Projects count model vertices through syConvert3Dto2D into screen
   coordinates. */
void btRenderProjectVertices(
	MODEL_VERTEX *vertices, PROJECTED_VERTEX *screen, Sint32 cnt)
{
	Sint32 point[3];

	for(; cnt != 0; cnt--) {
		SET_VECTOR(point, toFIXED(vertices->x), toFIXED(vertices->y),
			toFIXED(vertices->z));
		syConvert3Dto2D(point, point);
		screen->x = point[0];
		screen->y = point[1];
		screen->depth = point[2];
		vertices++;
		screen++;
	}
}

/* A RENDER_MODEL as btRenderAllocateModelList clears it: head and next over its
   kind and flags, tail over its texture scroll and the padding after it, one
   word. */
typedef struct POOL_ENTRY {
	Sint32 head;
	Sint32 next;
	Uint8  unused[16]; /* nothing reads or writes them */
	Sint32 tail;
} POOL_ENTRY;

/* Allocates count model entries plus a cleared one that ends the list. */
RENDER_MODEL *btRenderAllocateModelList(Sint32 cnt)
{
	POOL_ENTRY *entries = syHeapAllocEWRAM((cnt + 1) * sizeof(POOL_ENTRY));
	Sint32		i;

	for(i = 0; i < cnt + 1; i++) {
		entries[i].head = 0;
		entries[i].next = 0;
		entries[i].tail = 0;
	}
	return (RENDER_MODEL *)entries;
}

/* Copies a texture's pixels into a heap block when IWRAM has room for them;
   returns 1 with copy set up, 0 when there is no room. */
Bool btRenderCopyTextureToIWRAM(TEXTURE *src, TEXTURE *copy)
{
	Sint32 size = (1 << src->logWidth) << src->logHeight;
	Uint32 pixels;
	Sint32 misalign;

	if(syHeapGetRemainingIWRAM() < size + 8)
		return FALSE;
	*(Uint16 *)copy = *(Uint16 *)src;
	copy->pixels = syHeapAlloc(size + 8);
	pixels = (Uint32)src->pixels;
	misalign = pixels & 3;
	syWordsCopyEntry(
		copy->pixels, (void *)(pixels - misalign), (size + misalign + 3) & -4);
	copy->pixels = (Uint8 *)copy->pixels + misalign;
	return TRUE;
}

/* The texture stays where it is, even when IWRAM has room for a copy. */
#define MODEL_TEXTURE_IN_PLACE 4

typedef void (*MODEL_KERNEL)(RENDER_MODEL *model);

/* Copies a kernel whose DMA control is a word of its own. */
#define LOAD_KERNEL_WORDS(to, code, bytes, words) \
	{ \
		(to) = syHeapAlloc((Uint32)(bytes)); \
		syDMAXCopy( \
			(code), (to), DMA_COPY_WORDS(0) + (Uint32)(words), REG_DMA3); \
	}

/* Copies the model's texture into IWRAM unless it is to stay in place. */
#define COPY_TEXTURE() \
	{ \
		if(!(model->flg & MODEL_TEXTURE_IN_PLACE) && \
			btRenderCopyTextureToIWRAM(texture, &copy)) \
			model->texture = &copy; \
	}

/* Draws a list of models. A run of models of one kind loads that kind's
   rasterizer into IWRAM once, and it is freed when the kind changes. Before a
   textured model is drawn, its texture is copied into IWRAM where there is
   room, and the copy is freed after the model is drawn. */
void btRenderDrawModelList(RENDER_MODEL *model)
{
	MODEL_KERNEL code = NULL;
	Sint32		 lastKind = 0;
	TEXTURE		 copy;
	TEXTURE		*texture;

	for(;;) {
		texture = model->texture;
		if(model->kind != lastKind && code != NULL)
			syHeapRelease(code);
		if(model->kind == MODEL_END)
			break;
		switch(model->kind) {
		case MODEL_SOLID:
			if(model->kind != lastKind)
				RAM_CODE_LOAD(
					code, btRenderSolidKernel, btRenderSolidKernelCodeSize);
			code(model);
			break;
		case MODEL_SOLID_NARROW:
			if(model->kind != lastKind)
				RAM_CODE_LOAD(code, btRenderSolidNarrowKernel,
					btRenderSolidNarrowKernelCodeSize);
			code(model);
			break;
		case MODEL_SOLID_MAX:
			if(model->kind != lastKind)
				RAM_CODE_LOAD(code, btRenderSolidMaxKernel,
					btRenderSolidMaxKernelCodeSize);
			code(model);
			break;
		case MODEL_SOLID_ADDITIVE:
			if(model->kind != lastKind)
				RAM_CODE_LOAD(code, btRenderSolidAdditiveKernel,
					btRenderSolidAdditiveKernelCodeSize);
			code(model);
			break;
		case MODEL_TEXTURED:
			if(model->kind != lastKind)
				LOAD_KERNEL_WORDS(code, btRenderTexturedKernel,
					btRenderTexturedKernelCodeSize,
					btRenderTexturedKernelCodeWords);
			COPY_TEXTURE();
			code(model);
			break;
		case MODEL_SHADED:
			if(model->kind != lastKind)
				LOAD_KERNEL_WORDS(code, btRenderShadedKernel,
					btRenderShadedKernelCodeSize,
					btRenderShadedKernelCodeWords);
			COPY_TEXTURE();
			code(model);
			break;
		case MODEL_TEXTURED_NARROW:
			if(model->kind != lastKind)
				LOAD_KERNEL_WORDS(code, btRenderTexturedNarrowKernel,
					btRenderTexturedNarrowKernelCodeSize,
					btRenderTexturedNarrowKernelCodeWords);
			COPY_TEXTURE();
			code(model);
			break;
		case MODEL_TEXTURED_MAX:
			if(model->kind != lastKind)
				RAM_CODE_LOAD(code, btRenderTexturedMaxKernel,
					btRenderTexturedMaxKernelCodeSize);
			COPY_TEXTURE();
			code(model);
			break;
		case MODEL_TEXTURED_ADDITIVE:
			if(model->kind != lastKind)
				RAM_CODE_LOAD(code, btRenderTexturedAdditiveKernel,
					btRenderTexturedAdditiveKernelCodeSize);
			COPY_TEXTURE();
			code(model);
			break;
		case MODEL_SHADED_MAX:
			if(model->kind != lastKind)
				RAM_CODE_LOAD(code, btRenderShadedMaxKernel,
					btRenderShadedMaxKernelCodeSize);
			COPY_TEXTURE();
			code(model);
			break;
		case MODEL_SHADED_ADDITIVE:
			if(model->kind != lastKind)
				RAM_CODE_LOAD(code, btRenderShadedAdditiveKernel,
					btRenderShadedAdditiveKernelCodeSize);
			COPY_TEXTURE();
			code(model);
			break;
		}
		lastKind = model->kind;
		if(model->texture == &copy) {
			syHeapRelease((void *)((Uint32)model->texture->pixels & ~3));
			model->texture = texture;
		}
		model++;
	}
}

#undef XOR_SWAP
#undef FILL_SPAN
#undef DIFF_ABS
#undef DRAW_SPAN
#undef STEP_RIGHTWARD
#undef STEP_LEFTWARD
#undef BRANCH_FIELD
#undef PUT
#undef COPY_FROM
#undef COPY
#undef COPY_LONG
#undef COPY_DMA
#undef COPY_TWICE
#undef PUT_BRANCH
#undef PATCH
#undef MODEL_TEXTURE_IN_PLACE
#undef LOAD_KERNEL_WORDS
#undef COPY_TEXTURE
