@ Why assembly: push saves r5/r6 without lr and rrx consumes reader carry.
@ Its Thumb entry's bx pc switches to the ARM code that follows, a mode
@ switch inside a function; the halfword after bx pc is never run.
.syntax unified
	.thumb
	.global mnTileDecompress4bppCode
mnTileDecompress4bppCode:
	.global mnTileDecompress4bpp
	.thumb_func
mnTileDecompress4bpp:
	bx	pc
AlchemyUncredited08038b16:
	.2byte	0x0200
	.arm
AlchemyUncreditedEnd08038b16:
	push	{r5, r6}
	add	r5, pc, #40
	ldm	r5, {r2, r3}
	mov	r5, #15
	mov	ip, #1
	ands	r6, r0, #3
	beq	mnTileDecompress4bppLoop
	rsbs	r6, ip, r6, lsl #3
	bic	r0, r0, #3
	ldr	ip, [r0], #4
	rrx	ip, ip
	lsr	ip, ip, r6
	b	mnTileDecompress4bppLoop
@ The function's constants, which it reads beside its code (add, pc).
	.4byte	0xfedcba98
	.4byte	0x76543210
