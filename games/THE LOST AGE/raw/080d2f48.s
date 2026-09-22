.syntax unified
	.thumb
	.set sub_08020090, 0x08020090
	.set sub_08020138, 0x08020138
	.set sub_08020148, 0x08020148
	.set sub_08020150, 0x08020150
	.set sub_080cad84, 0x080cad84
	.set sub_080d3888, 0x080d3888
	.global Overlay_080d2f48
Overlay_080d2f48:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	adds	r7, r0, #0
	mov	r8, r1
	mov	sl, r2
	bl	sub_080cad84
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L_080d2fc0
	movs	r2, #10
	ldrsh	r3, [r6, r2]
	adds	r5, r3, #0
	cmp	r3, #0
	bge.n	.L_080d2f6c
	adds	r5, #15
.L_080d2f6c:
	asrs	r5, r5, #4
	lsls	r5, r5, #4
	adds	r2, r6, #0
	adds	r2, #91
	subs	r5, r3, r5
	movs	r3, #0
	strb	r3, [r2, #0]
	adds	r0, r6, #0
	bl	sub_08020138
	lsls	r5, r5, #16
	adds	r0, r6, #0
	movs	r1, #2
	bl	sub_08020090
	asrs	r5, r5, #16
	ldr	r1, [r6, #8]
	movs	r3, #8
	subs	r3, r3, r5
	lsls	r3, r3, #16
	adds	r1, r1, r3
	ldr	r2, [r6, #12]
	ldr	r3, [r6, #16]
	adds	r0, r6, #0
	bl	sub_08020148
	adds	r0, r6, #0
	bl	sub_08020150
	adds	r0, r7, #0
	mov	r1, r8
	bl	sub_080d3888
	mov	r3, sl
	lsls	r0, r3, #16
	ldr	r3, [r6, #16]
	ldr	r1, [r6, #8]
	adds	r3, r3, r0
	ldr	r2, [r6, #12]
	adds	r0, r6, #0
	bl	sub_08020148
.L_080d2fc0:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
