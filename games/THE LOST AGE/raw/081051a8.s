.syntax unified
	.thumb
	.set sub_080145a8, 0x080145a8
	.set sub_08014644, 0x08014644
	.set sub_08020030, 0x08020030
	.set sub_08020040, 0x08020040
	.set sub_08020048, 0x08020048
	.global Overlay_081051a8
Overlay_081051a8:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r3, [r3, #0]
	movs	r5, #136
	sub	sp, #8
	mov	r8, r3
	movs	r6, #0
	lsls	r5, r5, #2
	movs	r4, #3
.L_081051c2:
	mov	r1, r8
	ldr	r0, [r5, r1]
	cmp	r0, #0
	beq.n	.L_081051d6
	str	r4, [sp, #0]
	bl	sub_08020048
	mov	r2, r8
	str	r6, [r5, r2]
	ldr	r4, [sp, #0]
.L_081051d6:
	subs	r4, #1
	adds	r5, #4
	cmp	r4, #0
	bge.n	.L_081051c2
	ldr	r3, [pc, #124]
	movs	r6, #140
	movs	r7, #136
	str	r3, [sp, #4]
	lsls	r6, r6, #2
	lsls	r7, r7, #2
	movs	r4, #0
	add	r6, r8
	add	r7, r8
.L_081051f0:
	ldr	r2, [sp, #4]
	str	r4, [sp, #0]
	ldmia	r2!, {r0}
	adds	r1, r2, #0
	str	r1, [sp, #4]
	bl	sub_08020040
	adds	r5, r0, #0
	ldr	r4, [sp, #0]
	cmp	r5, #0
	beq.n	.L_0810521a
	movs	r1, #2
	bl	sub_08020030
	ldrb	r3, [r5, #9]
	movs	r1, #13
	negs	r1, r1
	adds	r2, r1, #0
	ands	r3, r2
	strb	r3, [r5, #9]
	ldr	r4, [sp, #0]
.L_0810521a:
	ldr	r3, [pc, #52]
	str	r5, [r7, #0]
	strh	r3, [r6, #0]
	ldr	r3, [pc, #48]
	adds	r4, #1
	strh	r3, [r6, #8]
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r7, #32]
	ldr	r3, [pc, #40]
	adds	r7, #4
	strh	r3, [r6, #32]
	adds	r6, #2
	cmp	r4, #3
	ble.n	.L_081051f0
	movs	r2, #160
	lsls	r2, r2, #3
	adds	r2, #10
	add	r2, r8
	movs	r3, #1
	movs	r1, #144
	strb	r3, [r2, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #20]
	bl	sub_080145a8
	b.n	.L_08105264
	.4byte 0x00000010
	.4byte 0x000000c8
	.4byte 0x00004000
	.4byte 0x08105a40
	.2byte 0x50cd
	.2byte 0x0810
.L_08105264:
	add	sp, #8
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r6, [r3, #0]
	movs	r5, #136
	movs	r3, #0
	mov	r8, r3
	lsls	r5, r5, #2
	movs	r7, #3
.L_08105284:
	ldr	r0, [r5, r6]
	cmp	r0, #0
	beq.n	.L_08105292
	bl	sub_08020048
	mov	r3, r8
	str	r3, [r5, r6]
.L_08105292:
	subs	r7, #1
	adds	r5, #4
	cmp	r7, #0
	bge.n	.L_08105284
	ldr	r0, [pc, #12]
	bl	sub_08014644
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0x50cd
	.2byte 0x0810
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r7, [r3, #0]
	movs	r3, #136
	lsls	r0, r0, #2
	lsls	r3, r3, #2
	adds	r6, r0, r3
	ldr	r0, [r7, r6]
	adds	r5, r1, #0
	mov	r8, r2
	cmp	r0, #0
	beq.n	.L_081052d4
	bl	sub_08020048
	movs	r3, #0
	str	r3, [r7, r6]
.L_081052d4:
	ldr	r3, [pc, #36]
	lsls	r2, r5, #2
	ldr	r0, [r3, r2]
	bl	sub_08020040
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_081052f4
	mov	r1, r8
	bl	sub_08020030
	ldrb	r2, [r5, #9]
	movs	r3, #13
	negs	r3, r3
	ands	r3, r2
	strb	r3, [r5, #9]
.L_081052f4:
	str	r5, [r7, r6]
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x5a40
	.2byte 0x0810
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r4, [r3, #0]
	movs	r5, #136
	lsls	r3, r0, #2
	lsls	r5, r5, #2
	adds	r3, r3, r5
	ldr	r3, [r4, r3]
	cmp	r3, #0
	beq.n	.L_08105328
	lsls	r0, r0, #1
	adds	r5, #16
	adds	r3, r0, r5
	strh	r1, [r4, r3]
	movs	r1, #142
	lsls	r1, r1, #2
	adds	r3, r0, r1
	strh	r2, [r4, r3]
.L_08105328:
	pop	{r5, pc}
