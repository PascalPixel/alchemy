.syntax unified
	.thumb
	.global Tile_Decompress4bppCode
Tile_Decompress4bppCode:
	.global Tile_Decompress4bpp
	.thumb_func
Tile_Decompress4bpp:
	bx	pc
AlchemyUncredited_08038b16:
	.2byte	0x0200
	.arm
AlchemyUncreditedEnd_08038b16:
	push	{r5, r6}
	add	r5, pc, #40
	ldm	r5, {r2, r3}
	mov	r5, #15
	mov	ip, #1
	ands	r6, r0, #3
	beq	Tile_Decompress4bppLoop
	rsbs	r6, ip, r6, lsl #3
	bic	r0, r0, #3
	ldr	ip, [r0], #4
	rrx	ip, ip
	lsr	ip, ip, r6
	b	Tile_Decompress4bppLoop
