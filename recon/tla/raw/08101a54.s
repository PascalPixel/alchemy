.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_08013560, 0x08013560
	.set sub_08038070, 0x08038070
	.set sub_08038080, 0x08038080
	.set sub_080380b8, 0x080380b8
	.set sub_08038260, 0x08038260
	.set sub_08038270, 0x08038270
	.set sub_080ad0f0, 0x080ad0f0
	.set sub_080ad250, 0x080ad250
	.set sub_081019a4, 0x081019a4
	.set sub_08101a04, 0x08101a04
	.global Func_08101a54
	.thumb_func
Func_08101a54:
.L_08101a54:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r7, r2, #0
	movs	r5, #224
	adds	r2, r3, #0
	ands	r5, r2
	sub	sp, #4
	mov	r8, r0
	adds	r6, r1, #0
	cmp	r5, #0
	bge.n	.L_08101a6e
	adds	r5, #31
.L_08101a6e:
	asrs	r5, r5, #5
	lsls	r0, r5, #2
	adds	r0, r0, r5
	movs	r3, #31
	ands	r3, r2
	lsls	r0, r0, #2
	adds	r0, r0, r3
	ldr	r3, [pc, #68]
	adds	r2, r6, #0
	adds	r0, r0, r3
	mov	r1, r8
	adds	r3, r7, #0
	adds	r2, #8
	bl	sub_08038080
	movs	r3, #160
	lsls	r3, r3, #7
	adds	r3, #1
	adds	r1, r6, #0
	adds	r5, r5, r3
	cmp	r1, #0
	bge.n	.L_08101a9c
	adds	r1, #7
.L_08101a9c:
	adds	r2, r7, #0
	asrs	r4, r1, #3
	cmp	r2, #0
	bge.n	.L_08101aa6
	adds	r2, #7
.L_08101aa6:
	asrs	r3, r2, #3
	movs	r2, #0
	str	r2, [sp, #0]
	mov	r0, r8
	adds	r1, r5, #0
	adds	r2, r4, #0
	bl	sub_08038270
	movs	r0, #15
	bl	sub_080380b8
	add	sp, #4
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x06d3
	.2byte 0x0000
.L_08101ac8:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r2, #192
	lsls	r2, r2, #18
	adds	r3, r2, #0
	adds	r3, #220
	ldr	r2, [r2, #60]
	ldr	r7, [r3, #0]
	sub	sp, #28
	str	r2, [sp, #24]
	adds	r6, r1, #0
	ldr	r1, [r7, #52]
	movs	r3, #1
	str	r1, [sp, #20]
	strb	r3, [r2, #6]
	mov	fp, r0
	ldr	r0, [sp, #20]
	bl	sub_08038260
	bl	sub_080ad0f0
	adds	r5, r0, #0
	movs	r0, #1
	negs	r0, r0
	bl	sub_080ad250
	cmp	r5, #0
	beq.n	.L_08101b32
	subs	r0, #1
	adds	r1, r5, #0
	bl	sub_08002054
	cmp	r0, #6
	bgt.n	.L_08101b32
	ldr	r0, [pc, #240]
	ldr	r1, [sp, #20]
	movs	r2, #0
	movs	r3, #80
	bl	sub_08038080
	movs	r3, #10
	str	r3, [sp, #0]
	ldr	r0, [sp, #20]
	movs	r1, #0
	movs	r2, #10
	movs	r3, #28
	bl	sub_08038070
.L_08101b32:
	mov	r0, fp
	bl	sub_081019a4
	movs	r3, #8
	str	r3, [sp, #12]
	adds	r1, r6, #0
	lsls	r3, r6, #2
	movs	r2, #0
	adds	r1, #160
	adds	r7, #248
	adds	r3, r3, r6
	lsls	r3, r3, #2
	str	r2, [sp, #16]
	str	r1, [sp, #8]
	str	r7, [sp, #4]
	add	r3, fp
	mov	sl, r3
.L_08101b54:
	movs	r2, #16
	mov	r9, r2
	ldr	r2, [sp, #4]
	ldmia	r2!, {r3}
	adds	r1, r2, #0
	str	r1, [sp, #4]
	cmp	r3, #0
	beq.n	.L_08101bce
	ldr	r7, [sp, #8]
	movs	r3, #0
	mov	r8, r3
.L_08101b6a:
	mov	r1, fp
	ldrsb	r3, [r1, r7]
	movs	r6, #0
	cmp	r6, r3
	bge.n	.L_08101bc4
	mov	r5, sl
.L_08101b76:
	ldrh	r2, [r5, #0]
	movs	r3, #224
	ands	r3, r2
	lsrs	r3, r3, #5
	cmp	r8, r3
	bne.n	.L_08101bb8
	ldrh	r0, [r5, #0]
	bl	sub_08101a04
	cmp	r0, #0
	beq.n	.L_08101b92
	cmp	r0, #1
	beq.n	.L_08101b9a
	b.n	.L_08101ba2
.L_08101b92:
	movs	r0, #2
	bl	sub_080380b8
	b.n	.L_08101ba8
.L_08101b9a:
	movs	r0, #15
	bl	sub_080380b8
	b.n	.L_08101ba8
.L_08101ba2:
	movs	r0, #4
	bl	sub_080380b8
.L_08101ba8:
	mov	r2, r9
	ldrh	r3, [r5, #0]
	ldr	r0, [sp, #20]
	ldr	r1, [sp, #12]
	bl	.L_08101a54
	movs	r2, #8
	add	r9, r2
.L_08101bb8:
	mov	r1, fp
	ldrsb	r3, [r1, r7]
	adds	r6, #1
	adds	r5, #2
	cmp	r6, r3
	blt.n	.L_08101b76
.L_08101bc4:
	movs	r2, #1
	add	r8, r2
	mov	r3, r8
	cmp	r3, #3
	ble.n	.L_08101b6a
.L_08101bce:
	ldr	r1, [sp, #8]
	ldr	r3, [sp, #16]
	adds	r1, #1
	str	r1, [sp, #8]
	ldr	r1, [sp, #12]
	movs	r2, #20
	adds	r3, #1
	adds	r1, #56
	add	sl, r2
	str	r3, [sp, #16]
	str	r1, [sp, #12]
	cmp	r3, #3
	ble.n	.L_08101b54
	ldr	r1, [sp, #24]
	movs	r2, #0
	movs	r3, #1
	strb	r3, [r1, #3]
	strb	r2, [r1, #6]
	movs	r0, #3
	bl	sub_08013560
	add	sp, #28
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0x10de
	.2byte 0x0000
	push	{lr}
	movs	r1, #0
	bl	.L_08101ac8
	pop	{pc}
	.align 2, 0
