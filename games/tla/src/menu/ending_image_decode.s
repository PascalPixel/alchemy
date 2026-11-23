@ The same two ARM routines again, beside the ending's scrolling images that
@ use the second: a copy of credits_background_decode.s whose decoder writes
@ 64 rows a band and steps 4 KB, the images' map strips.
@
@ EndingImage_InnerProduct: Vector_InnerProduct again. Nothing calls it.
@
@ EndingImage_Decode(source, destination, offset): decodes an image's packed
@ map (r0) into the map buffer (r1). EndingImage_Show (credits.c) copies it
@ into a heap block and calls it there, so its first loop relocates the table
@ of decoder commands (ending_image_expand.s) for wherever the copy landed.
@
@ Why assembly: as credits_background_decode.s's routines.
.syntax unified
	.arm
	.global EndingImage_InnerProduct
EndingImage_InnerProduct:
	smull	ip, r0, r1, r0
	smlal	ip, r0, r2, r3
	ldm	sp, {r2, r3}
	smlal	ip, r0, r2, r3
	lsl	r0, r0, #16
	orr	r0, r0, ip, lsr #16
	bx	lr
	.thumb
	.global EndingImage_DecodeCode
EndingImage_DecodeCode:
	.global EndingImage_Decode
	.thumb_func
EndingImage_Decode:
	bx	pc
AlchemyUncredited_ending_image_decode:
	.2byte	0x0200
	.arm
AlchemyUncreditedEnd_ending_image_decode:
	push	{r5, r6, r7, r8, r9, sl, fp, lr}
	mov	sl, r2
	ldr	r2, [pc, #192]
	add	r3, pc, #236
	sub	r2, r3, r2
	add	r4, pc, #196
	mov	r5, #8
.L_relocate:
	ldr	r6, [r3], #4
	add	r6, r6, r2
	str	r6, [r4], #4
	subs	r5, r5, #1
	bne	.L_relocate
	mov	fp, #7
	ldrh	r3, [r0], #2
	mov	r2, #0
	mov	r5, #0
	mov	r9, #15
.L_band:
	mov	r8, #8
.L_tile:
	mov	r7, #64
.L_row:
	bl	EndingImage_DecodeRead
	lsr	r6, r5, #25
	bl	EndingImage_DecodeRead
	orr	r6, r6, r5, lsr #17
	bl	EndingImage_DecodeRead
	orr	r6, r6, r5, lsr #9
	bl	EndingImage_DecodeRead
	orr	r6, r6, r5, lsr #1
	add	r6, r6, sl
	str	r6, [r1], #4
	bl	EndingImage_DecodeRead
	lsr	r6, r5, #25
	bl	EndingImage_DecodeRead
	orr	r6, r6, r5, lsr #17
	bl	EndingImage_DecodeRead
	orr	r6, r6, r5, lsr #9
	bl	EndingImage_DecodeRead
	orr	r6, r6, r5, lsr #1
	add	r6, r6, sl
	str	r6, [r1], #60
	subs	r7, r7, #1
	bne	.L_row
	sub	r1, r1, #0x1000
	add	r1, r1, #8
	subs	r8, r8, #1
	bne	.L_tile
	sub	r1, r1, #64
	add	r1, r1, #0x1000
	subs	r9, r9, #1
	bne	.L_band
	pop	{r5, r6, r7, r8, r9, sl, fp, lr}
	bx	lr
