@ Two ARM routines in bank 09, beside the credits slides that use the second.
@
@ Vector_DotProduct(ax, bx, ay, by, az, bz): the 16.16 dot product
@ ax*bx + ay*by + az*bz, the last two arguments on the stack, summed in 64
@ bits and returned shifted right 16. Nothing in the game calls it.
@
@ Tile_ExpandSlide(source, destination, offset): decodes a credits slide's
@ packed picture (r0) into 8bpp tiles (r1), 15 bands of 8 tiles of 32 rows,
@ adding offset (r2, the same palette offset in every byte) to each pixel.
@ Graphics_LoadCharacterBlockAndPalette (credits.c) copies it into a heap slot
@ and calls it there, so its first loop relocates the table of decoder
@ commands (expand.s) for wherever the copy landed.
@
@ Why assembly:
@ C2: Vector_DotProduct:
@ smull ip, r0, r1, r0 and both smlal ip, r0, r2, r3 keep the 64-bit sum
@ with its low half in ip (r12) and its high half in r0. mulsidi3 and
@ *mulsidi3adddi write a DI register pair, RdHi = RdLo + 1, earlyclobbered
@ and so apart from the inputs; a pair whose low half is r12 would put its
@ high half in sp, and r0 is also an input of the smull. So these multiplies
@ did not come from the DI patterns. smulsi3_highpart leaves its low half a
@ dead scratch, and orr r0, r0, ip, lsr #16 reads it.
@ This proves a hand-written multiply sequence, not by itself a hand-written
@ file. Nothing else here is a tell: the routine is a frameless leaf, and
@ ldm sp, {r2, r3}, which reloads the two stack arguments into argument
@ registers already consumed, has no writeback and is within the compiler's
@ load-multiple patterns.
@ C2: Tile_ExpandSlide:
@ r11/fp is the mask 7 passed through the command linkage; the opening
@ str loop rewrites the command table used by ldr pc in its dispatcher.
.syntax unified
.text
	.arm
	.global Vector_DotProduct
Vector_DotProduct:
	smull	ip, r0, r1, r0
	smlal	ip, r0, r2, r3
	ldm	sp, {r2, r3}
	smlal	ip, r0, r2, r3
	lsl	r0, r0, #16
	orr	r0, r0, ip, lsr #16
	bx	lr
.syntax unified
.text
	.arm
	.global Tile_ExpandSlide
Tile_ExpandSlide:
	push	{r5, r6, r7, r8, r9, sl, fp, lr}
	mov	sl, r2
	ldr	r2, [pc, #192]
	add	r3, pc, #236
	sub	r2, r3, r2
	add	r4, pc, #196
	mov	r5, #8
.Lrelocate:
	ldr	r6, [r3], #4
	add	r6, r6, r2
	str	r6, [r4], #4
	subs	r5, r5, #1
	bne .Lrelocate
	mov	fp, #7
	ldrh	r3, [r0], #2
	mov	r2, #0
	mov	r5, #0
	mov	r9, #15
.Lband:
	mov	r8, #8
.Lcolumn:
	mov	r7, #32
.Lrow:
	bl Tile_ExpandSlideRead
	lsr	r6, r5, #25
	bl Tile_ExpandSlideRead
	orr	r6, r6, r5, lsr #17
	bl Tile_ExpandSlideRead
	orr	r6, r6, r5, lsr #9
	bl Tile_ExpandSlideRead
	orr	r6, r6, r5, lsr #1
	add	r6, r6, sl
	str	r6, [r1], #4
	bl Tile_ExpandSlideRead
	lsr	r6, r5, #25
	bl Tile_ExpandSlideRead
	orr	r6, r6, r5, lsr #17
	bl Tile_ExpandSlideRead
	orr	r6, r6, r5, lsr #9
	bl Tile_ExpandSlideRead
	orr	r6, r6, r5, lsr #1
	add	r6, r6, sl
	str	r6, [r1], #60
	subs	r7, r7, #1
	bne .Lrow
	sub	r1, r1, #2048
	add	r1, r1, #8
	subs	r8, r8, #1
	bne .Lcolumn
	sub	r1, r1, #64
	add	r1, r1, #2048
	subs	r9, r9, #1
	bne .Lband
	pop	{r5, r6, r7, r8, r9, sl, fp, lr}
	bx	lr
