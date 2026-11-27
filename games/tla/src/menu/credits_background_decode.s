@ Two ARM functions in module 09, beside the credits that use the second, as The
@ Broken Seal's credits_background_decode.s.
@
@ mnVectorInnerProduct(ax, bx, ay, by, az, bz): the 16.16 dot product
@ ax*bx + ay*by + az*bz, the last two arguments on the stack, summed in 64
@ bits and returned shifted right 16. Nothing in the game calls it.
@
@ mnCreditsDecodeBackground(source, destination, offset): decodes a backdrop's
@ packed picture (r0) into 8bpp tiles (r1), adding offset (r2, the same
@ palette offset in every byte) to each pixel. mnCreditsLoadBackground
@ (credits.c) copies it into a heap block and calls it there, so its first
@ loop relocates the table of decoder commands (credits_background_expand.s)
@ for wherever the copy landed.
@
@ Why assembly:
@ Why mnVectorInnerProduct is assembly:
@ smull ip, r0, r1, r0 and both smlal ip, r0, r2, r3 keep the
@ 64-bit sum with its low half in ip (r12) and its high half in r0. mulsidi3
@ and *mulsidi3adddi write a DI register pair, RdHi = RdLo + 1,
@ earlyclobbered and so apart from the inputs; a pair whose low half is r12
@ would put its high half in sp, and r0 is also an input of the smull. So
@ these multiplies did not come from the DI patterns. smulsi3_highpart
@ leaves its low half a dead scratch, and orr r0, r0, ip, lsr #16 reads it.
@ This proves a hand-written multiply, not by itself a hand-written
@ file. Nothing else here is a tell: the function is a frameless leaf, and
@ ldm sp, {r2, r3}, which reloads the two stack arguments into argument
@ registers already consumed, has no writeback and is within the compiler's
@ load-multiple patterns.
@ Why mnCreditsDecodeBackground is assembly:
@ r11/fp is the mask 7 passed through the command linkage; the
@ opening str loop rewrites the command table used by ldr pc in its dispatcher.
@ Its Thumb entry's bx pc switches to the ARM code that follows, a mode
@ switch inside a function; the halfword after bx pc is never run.
.syntax unified
	.arm
	.global mnVectorInnerProduct
mnVectorInnerProduct:
	smull	ip, r0, r1, r0
	smlal	ip, r0, r2, r3
	ldm	sp, {r2, r3}
	smlal	ip, r0, r2, r3
	lsl	r0, r0, #16
	orr	r0, r0, ip, lsr #16
	bx	lr
	.thumb
	.global mnCreditsDecodeBackgroundCode
mnCreditsDecodeBackgroundCode:
	.global mnCreditsDecodeBackground
	.thumb_func
mnCreditsDecodeBackground:
	bx	pc
AlchemyUncredited_credits_background_decode:
	.2byte	0x0200
	.arm
AlchemyUncreditedEnd_credits_background_decode:
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
	mov	r7, #32
.L_row:
	bl	mnCreditsDecodeBackgroundRead
	lsr	r6, r5, #25
	bl	mnCreditsDecodeBackgroundRead
	orr	r6, r6, r5, lsr #17
	bl	mnCreditsDecodeBackgroundRead
	orr	r6, r6, r5, lsr #9
	bl	mnCreditsDecodeBackgroundRead
	orr	r6, r6, r5, lsr #1
	add	r6, r6, sl
	str	r6, [r1], #4
	bl	mnCreditsDecodeBackgroundRead
	lsr	r6, r5, #25
	bl	mnCreditsDecodeBackgroundRead
	orr	r6, r6, r5, lsr #17
	bl	mnCreditsDecodeBackgroundRead
	orr	r6, r6, r5, lsr #9
	bl	mnCreditsDecodeBackgroundRead
	orr	r6, r6, r5, lsr #1
	add	r6, r6, sl
	str	r6, [r1], #60
	subs	r7, r7, #1
	bne	.L_row
	sub	r1, r1, #0x800
	add	r1, r1, #8
	subs	r8, r8, #1
	bne	.L_tile
	sub	r1, r1, #64
	add	r1, r1, #0x800
	subs	r9, r9, #1
	bne	.L_band
	pop	{r5, r6, r7, r8, r9, sl, fp, lr}
	bx	lr
