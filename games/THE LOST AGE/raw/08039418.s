.syntax unified
	.thumb
	.set sub_0803a084, 0x0803a084
	.global Overlay_08039418
Overlay_08039418:
	push	{lr}
	movs	r3, #12
	ldrsh	r4, [r0, r3]
	movs	r3, #14
	ldrsh	r1, [r0, r3]
	ldrh	r2, [r0, #8]
	ldrh	r3, [r0, #10]
	adds	r0, r4, #0
	bl	sub_0803a084
	pop	{pc}
