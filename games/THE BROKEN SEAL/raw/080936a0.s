.syntax unified
	.thumb
	.set sub_080030f8, 0x080030f8
	.set sub_080041d8, 0x080041d8
	.set sub_080048f4, 0x080048f4
	.set sub_080072f0, 0x080072f0
	.set sub_0808d394, 0x0808d394
	.set sub_08185000, 0x08185000
	.global Overlay_080936a0
Overlay_080936a0:
	push	{r5, r6, r7, lr}
	ldr	r3, [pc, #88]
	adds	r6, r0, #0
	adds	r7, r1, #0
	movs	r0, #27
	ldr	r1, [pc, #84]
	ldr	r5, [r3, #0]
	bl	sub_080048f4
	movs	r1, #207
	lsls	r1, r1, #1
	adds	r3, r0, r1
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #3
	bne.n	.L_080936f4
	movs	r1, #128
	ldr	r3, [pc, #64]
	lsls	r1, r1, #9
	adds	r0, r6, #0
	bl	sub_080072f0
	movs	r3, #212
	lsls	r3, r3, #2
	adds	r1, r5, r3
	adds	r3, #4
	adds	r2, r5, r3
	ldr	r3, [r2, #0]
	str	r3, [r1, #0]
	movs	r1, #214
	lsls	r1, r1, #2
	adds	r3, r5, r1
	adds	r1, #2
	str	r0, [r2, #0]
	strh	r7, [r3, #0]
	movs	r2, #0
	adds	r3, r5, r1
	strh	r2, [r3, #0]
	ldr	r0, [pc, #24]
	ldr	r1, [pc, #28]
	bl	sub_080041d8
.L_080936f4:
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001e70
	.4byte 0x00000ccc
	.4byte 0x0300013c
	.4byte 0x080935d5
	.2byte 0x0c94
	.2byte 0x0000
	push	{r5, r6, lr}
	ldr	r3, [pc, #76]
	ldr	r1, [pc, #76]
	movs	r0, #27
	ldr	r6, [r3, #0]
	bl	sub_080048f4
	movs	r2, #207
	lsls	r2, r2, #1
	adds	r3, r0, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #3
	bne.n	.L_08093758
	movs	r2, #214
	lsls	r2, r2, #2
	adds	r3, r6, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	movs	r5, #0
	cmp	r3, #0
	beq.n	.L_08093758
	movs	r3, #214
	lsls	r3, r3, #2
	adds	r6, r6, r3
.L_08093742:
	movs	r0, #1
	bl	sub_080030f8
	ldr	r2, [pc, #28]
	adds	r5, #1
	cmp	r5, r2
	bgt.n	.L_08093758
	movs	r2, #0
	ldrsh	r3, [r6, r2]
	cmp	r3, #0
	bne.n	.L_08093742
.L_08093758:
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001e70
	.4byte 0x00000ccc
	.2byte 0x012b
	.2byte 0x0000
	push	{r5, r6, lr}
	adds	r5, r0, #0
	ldr	r6, [r5, #104]
	cmp	r6, #0
	beq.n	.L_080937b0
	adds	r2, r5, #0
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
	ldr	r3, [r6, #8]
	str	r3, [r5, #8]
	adds	r3, r5, #0
	adds	r3, #102
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	bl	sub_0808d394
	movs	r3, #0
	ldrsh	r0, [r0, r3]
	bl	sub_08185000
	movs	r2, #8
	ldrsb	r2, [r0, r2]
	ldr	r3, [r6, #12]
	lsls	r2, r2, #16
	adds	r3, r3, r2
	movs	r2, #128
	lsls	r2, r2, #12
	adds	r3, r3, r2
	str	r3, [r5, #12]
	ldr	r3, [r6, #20]
	str	r3, [r5, #20]
	ldr	r3, [r6, #16]
	str	r3, [r5, #16]
.L_080937b0:
	movs	r0, #0
	pop	{r5, r6}
	pop	{r1}
	bx	r1
