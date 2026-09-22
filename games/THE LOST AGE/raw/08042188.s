.syntax unified
	.thumb
	.set sub_08013164, 0x08013164
	.set sub_08014dac, 0x08014dac
	.set sub_0803acd4, 0x0803acd4
	.set sub_0803ae14, 0x0803ae14
	.set sub_080420a8, 0x080420a8
	.global Overlay_08042188
Overlay_08042188:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	adds	r5, r0, #0
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r7, r2, #0
	mov	r8, r3
	mov	sl, r1
	bl	sub_08014dac
	ldrb	r3, [r5, #0]
	adds	r6, r0, #0
	adds	r2, r6, #0
	cmp	r3, #0
	beq.n	.L_080421b8
.L_080421aa:
	ldrb	r3, [r5, #0]
	adds	r5, #1
	strh	r3, [r2, #0]
	adds	r2, #2
	ldrb	r3, [r5, #0]
	cmp	r3, #0
	bne.n	.L_080421aa
.L_080421b8:
	ldr	r3, [pc, #28]
	lsrs	r7, r7, #3
	strh	r3, [r2, #0]
	mov	r3, r8
	lsrs	r3, r3, #3
	adds	r0, r6, #0
	mov	r1, sl
	adds	r2, r7, #0
	mov	r8, r3
	bl	sub_0803acd4
	adds	r0, r6, #0
	bl	sub_08013164
	b.n	.L_080421dc
	movs	r0, r0
	.2byte 0x0000
	.2byte 0x0000
.L_080421dc:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	sub	sp, #16
	ldr	r7, [sp, #36]
	adds	r4, r0, #0
	adds	r5, r1, #0
	mov	r8, r3
	adds	r1, r4, #0
	adds	r6, r2, #0
	mov	r0, sp
	adds	r2, r5, #0
	bl	sub_0803ae14
	adds	r1, r6, #0
	mov	r2, r8
	adds	r3, r7, #0
	bl	sub_080420a8
	add	sp, #16
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
