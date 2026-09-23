.syntax unified
	.thumb
	.set sub_080030f8, 0x080030f8
	.set sub_080041d8, 0x080041d8
	.set sub_08004278, 0x08004278
	.set sub_08004458, 0x08004458
	.set sub_08009248, 0x08009248
	.set sub_08009280, 0x08009280
	.set sub_0808a518, 0x0808a518
	.set sub_0808a520, 0x0808a520
	.set sub_0808a528, 0x0808a528
	.set sub_0808a530, 0x0808a530
	.set sub_080b04c4, 0x080b04c4
	.set sub_080b0840, 0x080b0840
	.set sub_080b0894, 0x080b0894
	.set sub_080f9010, 0x080f9010
	.global Func_080b3050
	.thumb_func
Func_080b3050:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #392]
	ldr	r3, [r3, #0]
	movs	r1, #224
	mov	r8, r3
	lsls	r1, r1, #2
	add	r1, r8
	ldr	r3, [r1, #0]
	ldr	r2, [pc, #380]
	ldrb	r3, [r3, #5]
	add	r2, r8
	mov	fp, r3
	movs	r3, #255
	strb	r3, [r2, #0]
	ldr	r2, [r1, #0]
	movs	r3, #13
	strb	r3, [r2, #5]
	ldr	r3, [pc, #368]
	add	r3, r8
	ldr	r2, [pc, #368]
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	mov	sl, r0
	ldrsb	r0, [r2, r3]
	sub	sp, #12
	bl	sub_080f9010
	ldr	r0, [pc, #352]
	bl	sub_080b0840
	mov	r0, sl
	lsls	r0, r0, #2
	movs	r3, #138
	mov	r9, r0
	lsls	r3, r3, #1
	add	r3, r9
	mov	r1, r8
	ldr	r0, [r1, r3]
	movs	r1, #0
	bl	sub_08009280
	movs	r0, #20
	bl	sub_080030f8
	movs	r1, #200
	lsls	r1, r1, #4
	ldr	r0, [pc, #320]
	bl	sub_080041d8
	mov	r3, sl
	movs	r0, #154
	lsls	r2, r3, #1
	lsls	r0, r0, #1
	adds	r3, r2, r0
	mov	r1, r8
	ldrsh	r3, [r1, r3]
	mov	r6, sp
	lsls	r3, r3, #16
	movs	r1, #162
	str	r3, [r6, #0]
	lsls	r1, r1, #1
	adds	r3, r2, r1
	mov	r2, r8
	ldrsh	r3, [r2, r3]
	ldr	r1, [pc, #288]
	lsls	r3, r3, #16
	adds	r3, r3, r1
	movs	r5, #236
	lsls	r5, r5, #2
	str	r3, [r6, #8]
	movs	r7, #0
	add	r5, r8
.L_080b30ee:
	movs	r1, #142
	ldr	r3, [r6, #8]
	ldr	r2, [r6, #0]
	adds	r0, r5, #0
	lsls	r1, r1, #1
	bl	sub_0808a528
	adds	r0, r5, #0
	ldr	r1, [pc, #260]
	bl	sub_0808a520
	movs	r1, #7
	adds	r0, r5, #0
	bl	sub_0808a518
	bl	sub_08004458
	lsls	r1, r0, #3
	subs	r1, r1, r0
	lsrs	r1, r1, #16
	ldr	r0, [r5, #0]
	bl	sub_08009248
	ldr	r3, [pc, #232]
	movs	r0, #3
	str	r3, [r5, #44]
	str	r3, [r5, #40]
	bl	sub_080030f8
	cmp	r7, #5
	bne.n	.L_080b3134
	ldr	r3, [pc, #188]
	mov	r2, sl
	add	r3, r8
	strb	r2, [r3, #0]
.L_080b3134:
	adds	r7, #1
	adds	r5, #72
	cmp	r7, #17
	ble.n	.L_080b30ee
	bl	sub_080b04c4
	movs	r2, #252
	lsls	r2, r2, #2
	movs	r1, #2
	add	r2, r8
	movs	r7, #23
.L_080b314a:
	movs	r3, #5
	ldrsb	r3, [r2, r3]
	cmp	r3, #0
	beq.n	.L_080b3154
	strb	r1, [r2, #0]
.L_080b3154:
	subs	r7, #1
	adds	r2, #72
	cmp	r7, #0
	bge.n	.L_080b314a
	movs	r0, #20
	bl	sub_080030f8
	movs	r0, #126
	bl	sub_080f9010
	ldr	r2, [pc, #128]
	movs	r3, #255
	add	r2, r8
	strb	r3, [r2, #0]
	adds	r3, #21
	add	r3, r9
	mov	r1, r8
	ldr	r0, [r1, r3]
	movs	r1, #0
	bl	sub_08009248
	movs	r0, #20
	bl	sub_080030f8
	ldr	r6, [pc, #132]
	movs	r5, #236
	lsls	r5, r5, #2
	add	r6, r8
	add	r5, r8
	movs	r7, #23
.L_080b3190:
	ldrb	r3, [r6, #0]
	lsls	r3, r3, #24
	adds	r6, #72
	cmp	r3, #0
	beq.n	.L_080b31a0
	adds	r0, r5, #0
	bl	sub_0808a530
.L_080b31a0:
	subs	r7, #1
	adds	r5, #72
	cmp	r7, #0
	bge.n	.L_080b3190
	ldr	r0, [pc, #80]
	bl	sub_08004278
	movs	r3, #138
	lsls	r3, r3, #1
	add	r3, r9
	mov	r2, r8
	ldr	r0, [r2, r3]
	movs	r1, #16
	bl	sub_08009280
	bl	sub_080b0894
	movs	r0, #30
	bl	sub_080030f8
	movs	r3, #224
	lsls	r3, r3, #2
	add	r3, r8
	ldr	r3, [r3, #0]
	mov	r0, fp
	strb	r0, [r3, #5]
	add	sp, #12
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001f2c
	.4byte 0x000003ab
	.4byte 0x000003aa
	.4byte 0x080b4ab2
	.4byte 0x00202108
	.4byte 0x080b2ffd
	.4byte 0xfff40000
	.4byte 0x080b2f4d
	.4byte 0x0000b333
	.4byte 0x000003f5
