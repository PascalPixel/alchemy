.syntax unified
	.thumb
	.set sub_080072f0, 0x080072f0
	.set sub_080072f8, 0x080072f8
	.global Overlay_080c00d8
Overlay_080c00d8:
	push	{r5, r6, lr}
	movs	r1, #128
	movs	r2, #1
	ldr	r5, [pc, #64]
	lsls	r1, r1, #1
	negs	r2, r2
	adds	r6, r0, #0
	bl	sub_080072f8
	movs	r3, #128
	lsls	r3, r3, #1
	adds	r6, r6, r3
	adds	r0, r6, #0
	movs	r1, #128
	ldr	r2, [pc, #44]
	bl	sub_080072f8
	ldr	r2, [pc, #44]
	ldr	r1, [pc, #44]
	adds	r6, #128
	movs	r3, #0
.L_080c0102:
	adds	r3, #1
	stmia	r6!, {r2}
	adds	r2, r2, r1
	cmp	r3, #239
	bls.n	.L_080c0102
	movs	r1, #160
	ldr	r3, [pc, #16]
	adds	r0, r6, #0
	lsls	r1, r1, #2
	ldr	r2, [pc, #12]
	bl	sub_080072f0
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	.4byte 0x03000168
	.4byte 0x03ff03ff
	.4byte 0x02010200
	.4byte 0x00020002
