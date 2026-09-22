.syntax unified
	.thumb
	.global Overlay_0803a3e4
Overlay_0803a3e4:
	push	{lr}
	cmp	r0, #0
	bne.n	.L_0803a3ee
	movs	r0, #1
	b.n	.L_0803a400
.L_0803a3ee:
	ldrh	r3, [r0, #22]
	cmp	r3, #0
	bne.n	.L_0803a3fe
	movs	r2, #26
	ldrsh	r3, [r0, r2]
	movs	r0, #1
	cmp	r3, #0
	beq.n	.L_0803a400
.L_0803a3fe:
	movs	r0, #0
.L_0803a400:
	pop	{pc}
