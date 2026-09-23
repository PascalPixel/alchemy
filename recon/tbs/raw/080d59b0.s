.syntax unified
	.thumb
	.set sub_08002dd8, 0x08002dd8
	.set sub_080030f8, 0x080030f8
	.set sub_080041d8, 0x080041d8
	.set sub_08004278, 0x08004278
	.set sub_08004458, 0x08004458
	.set sub_080049ac, 0x080049ac
	.set sub_08004cb4, 0x08004cb4
	.set sub_080051d8, 0x080051d8
	.set sub_080072f4, 0x080072f4
	.set sub_080b5098, 0x080b5098
	.set sub_080b50e8, 0x080b50e8
	.set sub_080cd594, 0x080cd594
	.set sub_080cdbc0, 0x080cdbc0
	.set sub_080d6888, 0x080d6888
	.set sub_080e0524, 0x080e0524
	.set sub_080ed408, 0x080ed408
	.set sub_080f9010, 0x080f9010
	.global Unnamed_080d59b0
	.global Func_080d59b0
	.thumb_func
Unnamed_080d59b0:
Func_080d59b0:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r6, [pc, #92]
	adds	r3, r6, #0
	ldmia	r3!, {r1}
	ldr	r3, [r3, #0]
	sub	sp, #40
	str	r3, [sp, #24]
	ldr	r3, [pc, #84]
	mov	r9, r1
	add	r3, r9
	str	r0, [r3, #0]
	movs	r0, #0
	bl	sub_080cd594
	ldr	r2, [pc, #72]
	ldr	r3, [pc, #56]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #56]
	adds	r2, #48
	strh	r3, [r2, #0]
	ldr	r0, [pc, #64]
	mov	r1, r9
	movs	r2, #1
	movs	r3, #1
	bl	sub_080e0524
	movs	r5, #1
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	movs	r0, #46
	str	r5, [sp, #0]
	bl	sub_080ed408
	ldr	r2, [r6, #28]
	movs	r0, #47
	str	r2, [sp, #12]
	movs	r1, #7
	movs	r2, #7
	movs	r3, #15
	str	r5, [sp, #0]
	bl	sub_080ed408
	b.n	.L_080d5a2c
	.4byte 0x00000100
	.4byte 0x00000000
	.4byte 0x03001eec
	.4byte 0x00007828
	.4byte 0x04000020
	.2byte 0x00a8
	.2byte 0x0000
.L_080d5a2c:
	ldr	r6, [r6, #32]
	ldr	r5, [pc, #488]
	str	r6, [sp, #16]
	movs	r7, #0
.L_080d5a34:
	bl	sub_08004458
	movs	r3, #63
	ands	r3, r0
	adds	r3, #32
	lsls	r3, r3, #16
	str	r3, [r5, #0]
	ldr	r3, [pc, #472]
	movs	r6, #0
	str	r3, [r5, #4]
	bl	sub_08004458
	str	r6, [r5, #16]
	bl	sub_08004458
	movs	r3, #3
	ands	r3, r0
	str	r3, [r5, #8]
	bl	sub_08004458
	movs	r3, #255
	ands	r3, r0
	adds	r7, #1
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r7, #32
	bne.n	.L_080d5a34
	ldr	r3, [pc, #436]
	add	r3, r9
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #4]
	cmp	r3, #1
	bne.n	.L_080d5a7c
	ldr	r2, [pc, #428]
	ldr	r3, [pc, #428]
	str	r3, [r2, #0]
.L_080d5a7c:
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, r9
	movs	r3, #1
	str	r3, [r2, #0]
	ldr	r3, [pc, #420]
	movs	r1, #144
	add	r3, r9
	str	r6, [r3, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #412]
	bl	sub_080041d8
	movs	r0, #142
	bl	sub_080f9010
	movs	r3, #0
	str	r3, [sp, #20]
.L_080d5aa0:
	ldr	r3, [pc, #400]
	ldr	r4, [sp, #20]
	ldr	r3, [r3, #0]
	mov	fp, r3
	cmp	r4, #80
	bne.n	.L_080d5ab2
	movs	r0, #0
	bl	sub_080b50e8
.L_080d5ab2:
	ldr	r2, [pc, #364]
	mov	r1, r9
	ldr	r3, [r1, r2]
	ldr	r3, [r3, #20]
	movs	r0, #0
	mov	sl, r0
	cmp	r3, #0
	beq.n	.L_080d5b2c
	mov	r3, fp
	adds	r3, #12
	movs	r4, #36
	str	r3, [sp, #8]
	add	r6, sp, #28
	mov	r8, r4
	adds	r7, r1, r2
.L_080d5ad0:
	ldr	r3, [r7, #0]
	mov	r1, r8
	ldrsh	r0, [r3, r1]
	bl	sub_080b5098
	ldr	r5, [r0, #0]
	bl	sub_080049ac
	mov	r0, fp
	ldr	r1, [sp, #8]
	bl	sub_080051d8
	ldr	r3, [r5, #8]
	str	r3, [r6, #0]
	movs	r3, #160
	lsls	r3, r3, #14
	str	r3, [r6, #4]
	ldr	r3, [r5, #16]
	adds	r0, r6, #0
	str	r3, [r6, #8]
	bl	sub_08004cb4
	mov	r4, sl
	lsls	r3, r4, #4
	ldr	r0, [sp, #20]
	adds	r3, #64
	cmp	r0, r3
	bne.n	.L_080d5b1c
	ldr	r3, [r7, #0]
	mov	r1, r8
	ldrsh	r0, [r3, r1]
	movs	r3, #0
	str	r3, [sp, #0]
	movs	r1, #0
	movs	r2, #5
	subs	r3, #1
	bl	sub_080d6888
.L_080d5b1c:
	movs	r3, #2
	add	r8, r3
	ldr	r3, [r7, #0]
	movs	r4, #1
	ldr	r3, [r3, #20]
	add	sl, r4
	cmp	sl, r3
	bne.n	.L_080d5ad0
.L_080d5b2c:
	movs	r0, #32
	ldr	r6, [pc, #232]
	movs	r7, #0
	mov	r8, r0
.L_080d5b34:
	ldr	r1, [sp, #20]
	lsls	r3, r7, #2
	cmp	r1, r3
	ble.n	.L_080d5bcc
	ldr	r0, [r6, #4]
	ldr	r2, [pc, #248]
	cmp	r0, r2
	bgt.n	.L_080d5bcc
	ldr	r1, [r6, #24]
	cmp	r1, #0
	bge.n	.L_080d5b4c
	adds	r1, #15
.L_080d5b4c:
	asrs	r1, r1, #4
	movs	r3, #7
	ands	r1, r3
	cmp	r1, #3
	bgt.n	.L_080d5b74
	movs	r3, #2
	ldrsh	r2, [r6, r3]
	mov	r4, r8
	asrs	r3, r0, #16
	lsls	r1, r1, #10
	str	r4, [sp, #0]
	str	r4, [sp, #4]
	add	r1, r9
	subs	r2, #16
	subs	r3, #16
	ldr	r0, [sp, #24]
	ldr	r4, [sp, #12]
	bl	sub_080072f4
	b.n	.L_080d5b94
.L_080d5b74:
	ldr	r2, [pc, #196]
	lsls	r1, r1, #10
	add	r1, r9
	adds	r1, r1, r2
	movs	r3, #2
	ldrsh	r2, [r6, r3]
	mov	r4, r8
	asrs	r3, r0, #16
	str	r4, [sp, #0]
	str	r4, [sp, #4]
	subs	r2, #16
	subs	r3, #16
	ldr	r0, [sp, #24]
	ldr	r4, [sp, #16]
	bl	sub_080072f4
.L_080d5b94:
	ldr	r4, [r6, #16]
	movs	r0, #128
	lsls	r0, r0, #6
	ldr	r2, [r6, #4]
	adds	r1, r4, r0
	ldr	r3, [r6, #24]
	ldr	r0, [r6, #8]
	movs	r5, #184
	adds	r2, r2, r4
	adds	r3, r3, r0
	lsls	r5, r5, #15
	str	r2, [r6, #4]
	str	r1, [r6, #16]
	str	r3, [r6, #24]
	cmp	r2, r5
	ble.n	.L_080d5bcc
	cmp	r1, #0
	bne.n	.L_080d5bcc
	ldr	r1, [pc, #132]
	adds	r3, r4, r1
	adds	r2, r0, #4
	negs	r3, r3
	str	r2, [r6, #8]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r5, [r6, #4]
	str	r3, [r6, #16]
.L_080d5bcc:
	adds	r7, #1
	adds	r6, #28
	cmp	r7, #12
	bne.n	.L_080d5b34
	ldr	r2, [pc, #108]
	movs	r3, #1
	add	r2, r9
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_080030f8
	ldr	r2, [sp, #20]
	adds	r2, #1
	str	r2, [sp, #20]
	cmp	r2, #148
	beq.n	.L_080d5bee
	b.n	.L_080d5aa0
.L_080d5bee:
	ldr	r0, [pc, #64]
	bl	sub_08004278
	movs	r0, #47
	bl	sub_08002dd8
	movs	r0, #46
	bl	sub_08002dd8
	bl	sub_080cdbc0
	add	sp, #40
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x02010000
	.4byte 0xffe00000
	.4byte 0x00007828
	.4byte 0x04000028
	.4byte 0xffff9000
	.4byte 0x00007784
	.4byte 0x080cd261
	.4byte 0x03001e80
	.4byte 0x007fffff
	.4byte 0xfffff000
	.4byte 0x00002001
	.4byte 0x00007824
