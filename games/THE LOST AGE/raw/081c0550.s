.syntax unified
	.thumb
	.set sub_081c0c84, 0x081c0c84
	.global Overlay_081c0550
Overlay_081c0550:
	push	{lr}
	lsls	r0, r0, #16
	ldr	r2, [pc, #28]
	ldr	r1, [pc, #32]
	lsrs	r0, r0, #13
	adds	r0, r0, r1
	ldrh	r3, [r0, #4]
	lsls	r1, r3, #1
	adds	r1, r1, r3
	lsls	r1, r1, #2
	adds	r1, r1, r2
	ldr	r2, [r1, #0]
	ldr	r1, [r0, #0]
	adds	r0, r2, #0
	bl	sub_081c0c84
	pop	{r0}
	bx	r0
	.4byte 0x00000000
	.2byte 0x0000
	.2byte 0x0000
	push	{lr}
	lsls	r0, r0, #16
	ldr	r2, [pc, #36]
	ldr	r1, [pc, #40]
	lsrs	r0, r0, #13
	adds	r0, r0, r1
	ldrh	r3, [r0, #4]
	lsls	r1, r3, #1
	adds	r1, r1, r3
	lsls	r1, r1, #2
	adds	r1, r1, r2
	ldr	r1, [r1, #0]
	ldr	r3, [r1, #0]
	ldr	r2, [r0, #0]
	cmp	r3, r2
	beq.n	.L_081c05b0
	adds	r0, r1, #0
	adds	r1, r2, #0
	bl	sub_081c0c84
	b.n	.L_081c05c4
	.2byte 0x0000
	.4byte 0x00000000
	.2byte 0x0000
	.2byte 0x0000
.L_081c05b0:
	ldr	r2, [r1, #4]
	ldrh	r0, [r1, #4]
	cmp	r0, #0
	beq.n	.L_081c05bc
	cmp	r2, #0
	bge.n	.L_081c05c4
.L_081c05bc:
	adds	r0, r1, #0
	adds	r1, r3, #0
	bl	sub_081c0c84
.L_081c05c4:
	pop	{r0}
	bx	r0
