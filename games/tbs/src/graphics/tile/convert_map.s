@ C2: Tile_ConvertMapGrid:
@ push saves r5-r8 without lr.
@ C2: Tile_CopyStrided:
@ push saves r5-r9/sl/fp without lr; fp holds copied data.
.syntax unified
	.arm
	.global Tile_ConvertMapGrid
Tile_ConvertMapGrid:
	push	{r5, r6, r7, r8}
	mov	r5, #-16777216
	orr	r5, r5, r5, lsr #4
	orr	r4, r5, r5, lsr #8
	add	r4, r4, #1
	mov	r6, #16384
.Lcell:
	ldrh	ip, [r0], #2
	cmp	r5, ip, lsl #20
	bne	.Lstore_cell
	add	ip, ip, r4
	cmp	r4, #0
	addne	r4, r4, #1
.Lstore_cell:
	strh	ip, [r1], #4
	subs	r6, r6, #1
	bne	.Lcell
	add	r0, r2, #16384
	sub	r1, r1, #65536
	add	r1, r1, #2
	mov	r5, #255
	orr	r5, r5, r5, lsl #16
	mov	r8, #4096
.Linterleave:
	ldr	r4, [r2], #4
	ldr	ip, [r0], #4
	and	r3, r4, r5
	and	r6, ip, r5
	orr	r3, r3, r6, lsl #8
	and	r7, r4, r5, lsl #8
	and	r6, ip, r5, lsl #8
	orr	r7, r6, r7, lsr #8
	strh	r3, [r1], #4
	strh	r7, [r1], #4
	ror	r3, r3, #16
	ror	r7, r7, #16
	strh	r3, [r1], #4
	strh	r7, [r1], #4
	subs	r8, r8, #1
	bne	.Linterleave
	pop	{r5, r6, r7, r8}
	bx	lr
	.global Tile_ConvertMapGridCodeSize
	.set Tile_ConvertMapGridCodeSize, . - Tile_ConvertMapGrid
.syntax unified
	.arm
	.global Tile_CopyStrided
Tile_CopyStrided:
	bics	r1, r1, #7
	bicsne	r2, r2, #7
	bxeq	lr
	push	{r5, r6, r7, r8, r9, sl, fp}
.Lstride_row:
	mov	ip, r1
.Lstride_block:
	ldm	r0, {r4, r5}
	add	r0, r0, r1
	ldm	r0, {r6, r7}
	add	r0, r0, r1
	ldm	r0, {r8, r9}
	add	r0, r0, r1
	ldm	r0, {sl, fp}
	add	r0, r0, r1
	stmia	r3!, {r4, r5, r6, r7, r8, r9, sl, fp}
	ldm	r0, {r4, r5}
	add	r0, r0, r1
	ldm	r0, {r6, r7}
	add	r0, r0, r1
	ldm	r0, {r8, r9}
	add	r0, r0, r1
	ldm	r0!, {sl, fp}
	add	r0, r0, r1
	stmia	r3!, {r4, r5, r6, r7, r8, r9, sl, fp}
	subs	ip, ip, #8
	subgt	r0, r0, r1, lsl #3
	bgt	.Lstride_block
	sub	r0, r0, r1
	subs	r2, r2, #8
	bgt	.Lstride_row
	pop	{r5, r6, r7, r8, r9, sl, fp}
	bx	lr
	.global Tile_CopyStridedCodeSize
	.set Tile_CopyStridedCodeSize, . - Tile_CopyStrided
