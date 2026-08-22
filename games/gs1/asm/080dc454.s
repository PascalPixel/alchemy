@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080dc454
	.thumb_func
Func_080dc454:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r6, [pc, #96]
	adds	r3, r6, #0
	ldmia	r3!, {r1}
	ldr	r3, [r3, #0]
	sub	sp, #88
	str	r3, [sp, #48]
	adds	r3, r6, #0
	subs	r3, #108
	ldr	r3, [r3, #0]
	movs	r0, #1
	mov	r9, r1
	str	r3, [sp, #28]
	bl	Func_080cd594
	ldr	r2, [pc, #72]
	ldr	r3, [pc, #56]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #56]
	adds	r2, #50
	strh	r3, [r2, #0]
	ldr	r0, [pc, #64]
	bl	Func_08002f40
	adds	r5, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #56]
	adds	r1, r5, #0
	movs	r2, #128
	adds	r5, #128
	lsls	r0, r0, #19
	bl	Func_080072f0
	adds	r0, r5, #0
	mov	r1, r9
	bl	Func_08005340
	movs	r5, #2
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	movs	r0, #46
	str	r5, [sp, #0]
	bl	Func_080ed408
	b.n	.L0
	.4byte 0x00000100
	.4byte 0x00001010
	.4byte 0x03001eec
	.4byte 0x04000020
	.4byte 0x000000a9
	.4byte 0x03001388
.L0:
	movs	r1, #239
	lsls	r1, r1, #7
	ldr	r2, [pc, #456]
	add	r1, r9
	movs	r3, #1
	str	r3, [r1, #0]
	add	r2, r9
	movs	r3, #0
	str	r3, [r2, #0]
	ldr	r6, [r6, #28]
	str	r5, [r1, #0]
	movs	r3, #75
	movs	r1, #144
	str	r3, [r2, #0]
	ldr	r0, [pc, #436]
	lsls	r1, r1, #3
	str	r6, [sp, #36]
	bl	Func_080041d8
	ldr	r3, [pc, #432]
	add	r3, r9
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #4]
	movs	r2, #0
	str	r2, [sp, #24]
	cmp	r3, #0
	beq.n	.L1
	movs	r3, #112
	negs	r3, r3
	str	r3, [sp, #24]
.L1:
	ldr	r5, [sp, #24]
	ldr	r2, [pc, #412]
	lsls	r3, r5, #8
	str	r3, [r2, #0]
	movs	r4, #0
	movs	r3, #225
	lsls	r3, r3, #7
	movs	r2, #0
	movs	r1, #4
	add	r3, r9
.L2:
	adds	r4, #1
	str	r2, [r3, #0]
	str	r2, [r3, #4]
	str	r2, [r3, #12]
	str	r1, [r3, #8]
	adds	r3, #28
	cmp	r4, #64
	bne.n	.L2
	movs	r1, #0
	str	r1, [sp, #44]
	ldr	r2, [pc, #368]
	mov	r5, r9
	ldr	r3, [r5, r2]
	ldr	r3, [r3, #20]
	subs	r1, #64
	lsls	r3, r3, #4
	cmp	r3, r1
	bne.n	.L3
	b.n	.L4
.L3:
	movs	r3, #0
	str	r3, [sp, #32]
	mov	r5, r9
	ldr	r3, [r5, r2]
	ldr	r3, [r3, #20]
	cmp	r3, #0
	bne.n	.L5
	b.n	.L6
.L5:
	ldr	r3, [sp, #44]
	movs	r1, #52
	movs	r5, #36
	add	r1, sp
	str	r3, [sp, #16]
	str	r5, [sp, #12]
	mov	fp, r1
.L9:
	mov	r1, r9
	adds	r5, r1, r2
	ldr	r2, [sp, #32]
	lsls	r2, r2, #1
	ldr	r3, [r5, #0]
	ldr	r1, [sp, #12]
	str	r2, [sp, #20]
	ldrsh	r0, [r3, r1]
	bl	Func_080b5098
	ldr	r3, [r5, #0]
	ldr	r5, [sp, #12]
	ldr	r6, [r0, #0]
	ldrsh	r0, [r3, r5]
	mov	r1, fp
	bl	Func_080e396c
	mov	r2, fp
	ldr	r3, [r2, #0]
	ldr	r5, [sp, #24]
	adds	r3, r3, r5
	str	r3, [r2, #0]
	ldr	r1, [sp, #16]
	cmp	r1, #63
	bhi.n	.L7
	bl	Func_080049ac
	ldr	r0, [sp, #28]
	adds	r1, r0, #0
	adds	r1, #12
	bl	Func_080051d8
	ldr	r3, [r6, #8]
	add	r0, sp, #76
	str	r3, [r0, #0]
	ldr	r3, [r6, #12]
	str	r3, [r0, #4]
	ldr	r3, [r6, #16]
	add	r5, sp, #64
	str	r3, [r0, #8]
	adds	r1, r5, #0
	bl	Func_080e3944
	ldr	r3, [r5, #0]
	ldr	r2, [sp, #24]
	adds	r3, r3, r2
	str	r3, [r5, #0]
	movs	r4, #0
	mov	sl, fp
	mov	r7, r9
	mov	r8, r4
.L8:
	ldr	r5, [sp, #20]
	ldr	r1, [sp, #32]
	adds	r3, r5, r1
	adds	r3, r3, r4
	lsls	r5, r3, #3
	subs	r5, r5, r3
	lsls	r5, r5, #2
	movs	r2, #225
	lsls	r2, r2, #7
	add	r5, r9
	adds	r5, r5, r2
	ldr	r0, [r5, #12]
	add	r0, r8
	str	r4, [sp, #8]
	bl	Func_08002322
	mov	r3, sl
	ldr	r6, [r3, #0]
	lsls	r0, r0, #3
	asrs	r0, r0, #16
	adds	r6, r6, r0
	ldr	r0, [r5, #12]
	add	r0, r8
	bl	Func_0800231c
	mov	r1, sl
	ldr	r3, [r1, #4]
	ldr	r2, [r5, #12]
	movs	r1, #128
	lsls	r1, r1, #2
	lsls	r0, r0, #3
	adds	r2, r2, r1
	asrs	r0, r0, #16
	str	r2, [r5, #12]
	adds	r3, r3, r0
	movs	r2, #24
	subs	r6, #12
	adds	r1, r7, #0
	str	r2, [sp, #0]
	str	r2, [sp, #4]
	subs	r3, #28
	adds	r2, r6, #0
	ldr	r0, [sp, #48]
	ldr	r5, [sp, #36]
	bl	Func_080072f8
	ldr	r4, [sp, #8]
	movs	r1, #144
	ldr	r2, [pc, #132]
	lsls	r1, r1, #2
	adds	r4, #1
	adds	r7, r7, r1
	add	r8, r2
	cmp	r4, #3
	bne.n	.L8
.L7:
	ldr	r3, [sp, #16]
	ldr	r5, [sp, #12]
	ldr	r1, [sp, #32]
	subs	r3, #16
	adds	r5, #2
	adds	r1, #1
	str	r5, [sp, #12]
	str	r3, [sp, #16]
	str	r1, [sp, #32]
	ldr	r2, [pc, #92]
	mov	r5, r9
	ldr	r3, [r5, r2]
	ldr	r3, [r3, #20]
	cmp	r1, r3
	bne.n	.L9
.L6:
	ldr	r2, [pc, #92]
	movs	r3, #1
	add	r2, r9
	str	r3, [r2, #0]
	movs	r0, #1
	bl	Func_080030f8
	ldr	r1, [sp, #44]
	adds	r1, #1
	str	r1, [sp, #44]
	ldr	r2, [pc, #60]
	mov	r5, r9
	ldr	r3, [r5, r2]
	ldr	r3, [r3, #20]
	lsls	r3, r3, #4
	adds	r3, #64
	cmp	r1, r3
	beq.n	.L4
	b.n	.L3
.L4:
	ldr	r0, [pc, #36]
	bl	Func_08004278
	movs	r0, #46
	bl	Func_08002dd8
	bl	Func_080cdbc0
	add	sp, #88
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x00007784
	.4byte 0x080cd261
	.4byte 0x00007828
	.4byte 0x04000028
	.4byte 0x00005555
	.4byte 0x00007824
