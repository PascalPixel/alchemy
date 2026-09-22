.syntax unified
	.thumb
	.set sub_08016cfc, 0x08016cfc
	.set sub_08038040, 0x08038040
	.set sub_08038120, 0x08038120
	.set sub_080ad080, 0x080ad080
	.set sub_080ad2f0, 0x080ad2f0
	.set sub_080cf424, 0x080cf424
	.set sub_080d01cc, 0x080d01cc
	.set sub_080d2240, 0x080d2240
	.set sub_080d260c, 0x080d260c
	.set sub_080d2d84, 0x080d2d84
	.set sub_080d3118, 0x080d3118
	.set sub_080d359c, 0x080d359c
	.set sub_081c0010, 0x081c0010
	.global Overlay_080d296c
Overlay_080d296c:
	push	{r5, r6, r7, lr}
	adds	r5, r1, #0
	adds	r6, r0, #0
	adds	r0, r5, #0
	adds	r7, r2, #0
	bl	sub_080ad2f0
	cmp	r0, #0
	bge.n	.L_080d29d6
	adds	r0, r6, #0
	bl	sub_080d2d84
	movs	r3, #0
	str	r3, [r0, #24]
	movs	r1, #3
	adds	r0, r5, #0
	bl	sub_080cf424
	adds	r0, r5, #0
	movs	r1, #2
	bl	sub_08038120
	ldr	r5, [pc, #104]
	movs	r1, #1
	adds	r0, r5, #0
	adds	r5, #4
	bl	sub_08038040
	movs	r1, #1
	adds	r0, r5, #0
	bl	sub_08038040
	adds	r0, r6, #0
	bl	sub_080d2d84
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r0, #24]
	ldr	r3, [pc, #76]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	adds	r1, r6, #0
	movs	r2, #0
	bl	sub_080d359c
	movs	r0, #20
	bl	sub_080d2240
	movs	r0, #1
	negs	r0, r0
	b.n	.L_080d2a00
.L_080d29d6:
	movs	r2, #0
	adds	r0, r6, #0
	movs	r1, #0
	bl	sub_080d3118
	adds	r0, r5, #0
	movs	r1, #3
	bl	sub_080cf424
	adds	r0, r5, #0
	movs	r1, #0
	bl	sub_080d260c
	movs	r3, #1
	negs	r3, r3
	cmp	r7, r3
	beq.n	.L_080d29fe
	adds	r0, r7, #0
	bl	sub_08016cfc
.L_080d29fe:
	movs	r0, #0
.L_080d2a00:
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x00000e0f
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, lr}
	adds	r6, r1, #0
	adds	r5, r0, #0
	bl	sub_080ad080
	movs	r0, #83
	bl	sub_081c0010
	adds	r0, r5, #0
	movs	r1, #1
	bl	sub_08038120
	adds	r0, r6, #0
	movs	r1, #4
	bl	sub_08038120
	ldr	r0, [pc, #8]
	movs	r1, #3
	bl	sub_08038040
	pop	{r5, r6, pc}
	movs	r0, r0
	.2byte 0x0039
	.2byte 0x0000
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r5, r2
	adds	r2, #8
	ldr	r0, [r3, #0]
	adds	r3, r5, r2
	ldr	r1, [r3, #0]
	bl	sub_080d01cc
	movs	r3, #217
	lsls	r3, r3, #1
	adds	r2, r5, r3
	movs	r3, #1
	strh	r3, [r2, #0]
	pop	{r5, pc}
