.syntax unified
	.thumb
	.global Overlay_080063bc
Overlay_080063bc:
	push	{r5, r6, r7, lr}
	ldr	r5, [pc, #52]
	ldr	r4, [r5, #0]
	adds	r6, r1, #0
	ldr	r7, [pc, #48]
	cmp	r4, #0
	beq.n	.L_080063d0
	movs	r0, #1
	negs	r0, r0
	b.n	.L_080063ec
.L_080063d0:
	ldr	r2, [pc, #40]
	ldrh	r1, [r2, #0]
	strh	r2, [r2, #0]
	movs	r3, #128
	strb	r3, [r7, #1]
	ldr	r3, [pc, #36]
	strh	r6, [r3, #0]
	ldr	r3, [pc, #36]
	strb	r4, [r3, #0]
	movs	r3, #1
	str	r0, [r5, #0]
	strb	r3, [r7, #0]
	strh	r1, [r2, #0]
	movs	r0, #0
.L_080063ec:
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x02002080
	.4byte 0x02002220
	.4byte 0x04000208
	.4byte 0x02002008
	.4byte 0x020023a4
