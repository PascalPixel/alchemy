.syntax unified
	.thumb
	.set sub_0814cd48, 0x0814cd48
	.global Overlay_081434d8
Overlay_081434d8:
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r0, [r3, #92]
	movs	r3, #240
	lsls	r3, r3, #7
	adds	r3, #220
	adds	r0, r0, r3
	movs	r1, #8
	ldr	r3, [pc, #8]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0xbd00
	movs	r0, r0
	.2byte 0x0258
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r6, [r3, #92]
	movs	r3, #240
	movs	r1, #1
	lsls	r3, r3, #7
	negs	r1, r1
	adds	r3, #220
	sub	sp, #8
	movs	r4, #0
	mov	r8, r1
	movs	r7, #36
	adds	r5, r6, r3
.L_08143518:
	ldrb	r2, [r5, #0]
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L_08143548
	adds	r3, #255
	strb	r3, [r5, #0]
	lsls	r3, r3, #24
	lsrs	r2, r3, #24
	cmp	r2, #0
	bne.n	.L_08143548
	movs	r1, #240
	lsls	r1, r1, #7
	adds	r1, #240
	adds	r3, r6, r1
	ldr	r3, [r3, #0]
	str	r4, [sp, #4]
	ldrsh	r0, [r3, r7]
	str	r2, [sp, #0]
	movs	r1, #0
	mov	r2, r8
	mov	r3, r8
	bl	sub_0814cd48
	ldr	r4, [sp, #4]
.L_08143548:
	adds	r4, #1
	adds	r7, #2
	adds	r5, #1
	cmp	r4, #8
	bne.n	.L_08143518
	add	sp, #8
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
