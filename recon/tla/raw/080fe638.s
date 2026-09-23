.syntax unified
	.thumb
	.set sub_08002064, 0x08002064
	.set sub_08013560, 0x08013560
	.set sub_080138a8, 0x080138a8
	.set sub_08016ce4, 0x08016ce4
	.set sub_08038070, 0x08038070
	.set sub_08038078, 0x08038078
	.set sub_08038080, 0x08038080
	.set sub_08038260, 0x08038260
	.set sub_08038268, 0x08038268
	.set sub_08038270, 0x08038270
	.set sub_080383f8, 0x080383f8
	.set sub_080f9170, 0x080f9170
	.set sub_081054cc, 0x081054cc
	.set sub_081c0010, 0x081c0010
	.global Func_080fe638
	.thumb_func
Func_080fe638:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r3, [r3, #0]
	sub	sp, #36
	movs	r0, #0
	str	r0, [sp, #28]
	movs	r0, #112
	str	r3, [sp, #32]
	bl	sub_081c0010
	ldr	r1, [sp, #32]
	movs	r5, #13
	adds	r1, #240
	str	r1, [sp, #24]
	movs	r6, #1
	ldr	r0, [r1, #0]
	bl	sub_08038260
	ldr	r2, [sp, #24]
	ldr	r0, [pc, #124]
	ldr	r1, [r2, #0]
	movs	r3, #16
	movs	r2, #0
	bl	sub_08038080
	ldr	r3, [sp, #24]
	movs	r2, #96
	ldr	r1, [r3, #0]
	ldr	r0, [pc, #108]
	negs	r2, r2
	movs	r3, #132
	bl	sub_08038078
	ldr	r0, [sp, #32]
	movs	r1, #188
	lsls	r1, r1, #1
	adds	r3, r0, r1
	ldr	r3, [r3, #0]
	movs	r2, #190
	lsls	r2, r2, #1
	strb	r5, [r3, #5]
	adds	r3, r0, r2
	ldr	r3, [r3, #0]
	movs	r0, #0
	strb	r5, [r3, #5]
	bl	sub_081054cc
	ldr	r0, [sp, #32]
	movs	r1, #184
	ldr	r3, [r0, #20]
	lsls	r1, r1, #1
	strb	r5, [r3, #5]
	adds	r3, r0, r1
	ldr	r3, [r3, #0]
	movs	r2, #148
	lsls	r2, r2, #1
	strb	r5, [r3, #5]
	adds	r1, r0, r2
	ldr	r5, [pc, #36]
	movs	r2, #140
	ldr	r4, [pc, #36]
	lsls	r2, r2, #2
	adds	r3, r0, r2
	movs	r0, #32
	movs	r2, #3
.L_080fe6ca:
	subs	r2, #1
	strh	r0, [r3, #0]
	strh	r5, [r3, #8]
	adds	r0, #56
	strh	r4, [r1, #0]
	adds	r3, #2
	adds	r1, #2
	cmp	r2, #0
	bge.n	.L_080fe6ca
	movs	r0, #1
	bl	sub_08013560
	b.n	.L_080fe826
	.4byte 0x00000038
	.4byte 0x0000001a
	.4byte 0x00001046
	.2byte 0x1047
	.2byte 0x0000
.L_080fe6f4:
	cmp	r6, #0
	beq.n	.L_080fe7c6
	ldr	r3, [sp, #32]
	ldr	r3, [r3, #40]
	mov	r8, r3
	mov	r0, r8
	bl	sub_08038260
	movs	r0, #1
	bl	sub_08013560
	movs	r3, #11
	str	r3, [sp, #0]
	mov	r0, r8
	movs	r1, #0
	movs	r2, #11
	movs	r3, #28
	bl	sub_08038070
	movs	r3, #28
	str	r3, [sp, #0]
	mov	r0, r8
	movs	r1, #2
	movs	r2, #1
	ldr	r3, [sp, #28]
	bl	sub_080f9170
	ldr	r0, [sp, #28]
	movs	r4, #0
	lsls	r0, r0, #3
	str	r0, [sp, #20]
	str	r4, [sp, #16]
	str	r4, [sp, #12]
	str	r4, [sp, #8]
.L_080fe738:
	ldr	r1, [sp, #8]
	ldr	r2, [sp, #12]
	ldr	r3, [sp, #16]
	movs	r7, #0
	mov	fp, r1
	mov	r9, r2
	mov	sl, r3
.L_080fe746:
	ldr	r1, [sp, #20]
	ldr	r2, [sp, #28]
	str	r4, [sp, #4]
	adds	r0, r1, r2
	adds	r0, r7, r0
	movs	r1, #18
	adds	r0, #7
	bl	sub_08002064
	mov	r3, fp
	adds	r6, r3, r0
	adds	r0, r6, #0
	adds	r0, #48
	bl	sub_08016ce4
	ldr	r4, [sp, #4]
	cmp	r0, #0
	beq.n	.L_080fe79a
	movs	r2, #128
	lsls	r2, r2, #5
	adds	r2, #1
	adds	r1, r4, r2
	movs	r3, #0
	adds	r5, r7, #1
	mov	r2, r9
	str	r3, [sp, #0]
	mov	r0, r8
	adds	r2, #1
	adds	r3, r5, #0
	bl	sub_08038270
	ldr	r0, [pc, #116]
	lsls	r3, r7, #3
	mov	r2, sl
	adds	r0, r6, r0
	adds	r3, #8
	mov	r1, r8
	adds	r2, #16
	bl	sub_08038080
	ldr	r4, [sp, #4]
	b.n	.L_080fe79c
.L_080fe79a:
	adds	r5, r7, #1
.L_080fe79c:
	adds	r7, r5, #0
	cmp	r7, #8
	ble.n	.L_080fe746
	ldr	r3, [sp, #16]
	ldr	r0, [sp, #12]
	ldr	r1, [sp, #8]
	adds	r3, #56
	adds	r0, #7
	adds	r1, #20
	adds	r4, #1
	str	r3, [sp, #16]
	str	r0, [sp, #12]
	str	r1, [sp, #8]
	cmp	r4, #3
	ble.n	.L_080fe738
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #60]
	movs	r3, #1
	strb	r3, [r2, #3]
	movs	r6, #0
.L_080fe7c6:
	ldr	r0, [pc, #56]
	bl	sub_080383f8
	movs	r0, #1
	bl	sub_08013560
	ldr	r1, [pc, #48]
	movs	r2, #7
	ldr	r3, [r1, #4]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_080fe834
	ldr	r3, [r1, #12]
	movs	r2, #48
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080fe826
	ldr	r3, [r1, #12]
	movs	r2, #16
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080fe808
	ldr	r2, [sp, #28]
	adds	r2, #1
	str	r2, [sp, #28]
	b.n	.L_080fe80e
	movs	r0, r0
	.4byte 0x000006d3
	.4byte 0x06002500
	.2byte 0x1150
	.2byte 0x0300
.L_080fe808:
	ldr	r3, [sp, #28]
	subs	r3, #1
	str	r3, [sp, #28]
.L_080fe80e:
	ldr	r0, [sp, #28]
	movs	r1, #2
	adds	r0, #2
	bl	sub_08002064
	str	r0, [sp, #28]
	movs	r0, #111
	bl	sub_081c0010
	bl	sub_080138a8
	movs	r6, #1
.L_080fe826:
	movs	r0, #168
	lsls	r0, r0, #1
	bl	sub_08016ce4
	cmp	r0, #0
	bne.n	.L_080fe834
	b.n	.L_080fe6f4
.L_080fe834:
	ldr	r1, [sp, #32]
	ldr	r0, [r1, #40]
	bl	sub_08038260
	ldr	r2, [sp, #24]
	ldr	r0, [r2, #0]
	bl	sub_08038268
	ldr	r1, [sp, #32]
	movs	r2, #140
	ldr	r0, [pc, #8]
	lsls	r2, r2, #2
	adds	r3, r1, r2
	movs	r1, #130
	movs	r2, #3
	b.n	.L_080fe858
	.2byte 0x0080
	.2byte 0x0000
.L_080fe858:
	subs	r2, #1
	strh	r1, [r3, #0]
	strh	r0, [r3, #8]
	adds	r1, #32
	adds	r3, #2
	cmp	r2, #0
	bge.n	.L_080fe858
	ldr	r0, [sp, #32]
	movs	r1, #184
	ldr	r3, [r0, #20]
	movs	r2, #1
	lsls	r1, r1, #1
	strb	r2, [r3, #5]
	adds	r3, r0, r1
	ldr	r3, [r3, #0]
	movs	r0, #1
	strb	r2, [r3, #5]
	bl	sub_081054cc
	movs	r0, #113
	bl	sub_081c0010
	add	sp, #36
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.align 2, 0
