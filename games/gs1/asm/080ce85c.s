@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080ce85c
	.thumb_func
Func_080ce85c:
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
	ldr	r2, [pc, #92]
	mov	sl, r1
	ldr	r3, [r3, #0]
	sub	sp, #64
	add	r2, sl
	str	r3, [sp, #28]
	str	r0, [r2, #0]
	movs	r0, #0
	mov	r8, r2
	bl	Func_080cd594
	ldr	r2, [pc, #76]
	ldr	r3, [pc, #60]
	ldr	r0, [pc, #76]
	strh	r3, [r2, #0]
	bl	Func_08002f40
	adds	r5, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #68]
	adds	r1, r5, #0
	movs	r2, #128
	adds	r5, #128
	lsls	r0, r0, #19
	bl	Func_080072f0
	adds	r0, r5, #0
	mov	r1, sl
	bl	Func_08005340
	movs	r5, #2
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	movs	r0, #46
	str	r5, [sp, #0]
	bl	Func_080ed408
	ldr	r3, [r6, #28]
	movs	r2, #7
	str	r3, [sp, #32]
	movs	r1, #7
	b.n	.L0
	movs	r0, r0
	.4byte 0x00000100
	.4byte 0x03001eec
	.4byte 0x00007828
	.4byte 0x04000020
	.4byte 0x000000af
	.4byte 0x03001388
.L0:
	movs	r3, #15
	movs	r0, #47
	str	r5, [sp, #0]
	bl	Func_080ed408
	ldr	r3, [r6, #32]
	mov	r1, sp
	movs	r6, #144
	adds	r1, #32
	lsls	r6, r6, #3
	str	r1, [sp, #16]
	ldr	r0, [pc, #532]
	str	r3, [r1, #4]
	adds	r1, r6, #0
	bl	Func_080041d8
	movs	r3, #239
	lsls	r3, r3, #7
	ldr	r2, [pc, #520]
	add	r3, sl
	str	r5, [r3, #0]
	add	r2, sl
	movs	r3, #50
	str	r3, [r2, #0]
	ldr	r0, [pc, #512]
	adds	r1, r6, #0
	bl	Func_080041d8
	mov	r2, r8
	ldr	r3, [r2, #0]
	ldr	r3, [r3, #4]
	cmp	r3, #1
	bne.n	.L1
	ldr	r3, [pc, #500]
	ldr	r2, [pc, #500]
	str	r3, [r2, #0]
	movs	r3, #112
	negs	r3, r3
	str	r3, [sp, #20]
	b.n	.L2
.L1:
	movs	r1, #0
	str	r1, [sp, #20]
.L2:
	movs	r2, #0
	mov	r8, r2
	ldr	r2, [pc, #484]
	mov	r1, sl
	ldr	r3, [r1, r2]
	ldr	r3, [r3, #20]
	movs	r1, #48
	lsls	r3, r3, #4
	negs	r1, r1
	cmp	r3, r1
	bne.n	.L3
	b.n	.L4
.L3:
	ldr	r3, [pc, #468]
	ldr	r3, [r3, #0]
	mov	r1, sl
	mov	r9, r3
	ldr	r3, [r1, r2]
	movs	r6, #211
	ldr	r3, [r3, #4]
	lsls	r6, r6, #7
	add	r6, sl
	cmp	r3, #0
	bne.n	.L5
	movs	r3, #128
	mov	r2, r8
	movs	r7, #0
	lsls	r3, r3, #12
	lsls	r5, r2, #10
.L6:
	adds	r0, r5, #0
	str	r3, [sp, #8]
	bl	Func_08002322
	ldr	r3, [sp, #8]
	lsls	r0, r0, #3
	subs	r0, r3, r0
	movs	r1, #128
	asrs	r0, r0, #10
	lsls	r1, r1, #3
	adds	r7, #1
	stmia	r6!, {r0}
	adds	r5, r5, r1
	cmp	r7, #160
	bne.n	.L6
	b.n	.L7
.L5:
	mov	r2, r8
	movs	r7, #0
	lsls	r5, r2, #10
.L8:
	adds	r0, r5, #0
	bl	Func_08002322
	ldr	r3, [pc, #396]
	lsls	r0, r0, #3
	asrs	r0, r0, #10
	movs	r1, #128
	adds	r0, r0, r3
	lsls	r1, r1, #3
	adds	r7, #1
	stmia	r6!, {r0}
	adds	r5, r5, r1
	cmp	r7, #160
	bne.n	.L8
.L7:
	bl	Func_080049ac
	mov	r1, r9
	adds	r1, #12
	mov	r0, r9
	bl	Func_080051d8
	movs	r2, #0
	str	r2, [sp, #24]
	ldr	r2, [pc, #348]
	mov	r1, sl
	ldr	r3, [r1, r2]
	ldr	r3, [r3, #20]
	cmp	r3, #0
	beq.n	.L9
	movs	r3, #52
	movs	r1, #36
	add	r3, sp
	str	r1, [sp, #12]
	mov	r9, r3
.L15:
	mov	r3, sl
	adds	r5, r3, r2
	ldr	r3, [r5, #0]
	ldr	r1, [sp, #12]
	ldrsh	r0, [r3, r1]
	bl	Func_080b5098
	ldr	r3, [sp, #24]
	lsls	r2, r3, #4
	ldr	r6, [r0, #0]
	cmp	r8, r2
	ble.n	.L10
	adds	r3, r2, #0
	adds	r3, #60
	cmp	r8, r3
	bge.n	.L10
	subs	r3, #28
	cmp	r8, r3
	bne.n	.L11
	ldr	r3, [r5, #0]
	ldr	r1, [sp, #12]
	ldrsh	r0, [r3, r1]
	movs	r3, #0
	str	r3, [sp, #0]
	movs	r1, #0
	movs	r2, #5
	subs	r3, #1
	bl	Func_080d6888
.L11:
	ldr	r3, [r6, #8]
	mov	r1, r9
	str	r3, [r1, #0]
	movs	r3, #160
	lsls	r3, r3, #14
	str	r3, [r1, #4]
	ldr	r3, [r6, #16]
	add	r5, sp, #40
	str	r3, [r1, #8]
	mov	r0, r9
	adds	r1, r5, #0
	bl	Func_080e3944
	mov	r2, r8
	movs	r7, #0
	mov	fp, r5
	lsls	r6, r2, #9
.L14:
	adds	r0, r6, #0
	bl	Func_08002322
	mov	r1, fp
	ldr	r3, [r1, #0]
	lsls	r0, r0, #4
	asrs	r0, r0, #16
	ldr	r2, [sp, #20]
	adds	r3, r3, r0
	adds	r0, r6, #0
	adds	r5, r3, r2
	bl	Func_0800231c
	mov	r1, fp
	ldr	r3, [r1, #4]
	lsls	r0, r0, #4
	asrs	r0, r0, #16
	adds	r0, r3, r0
	mov	r3, r8
	cmp	r3, #0
	bge.n	.L12
	adds	r3, #15
.L12:
	asrs	r2, r3, #4
	movs	r3, #1
	ldr	r1, [sp, #16]
	ands	r3, r2
	lsls	r3, r3, #2
	adds	r4, r3, r1
	mov	r1, r8
	cmp	r1, #0
	bge.n	.L13
	adds	r1, #3
.L13:
	lsls	r3, r2, #2
	asrs	r1, r1, #2
	subs	r1, r1, r3
	adds	r3, r0, #0
	movs	r0, #32
	str	r0, [sp, #0]
	str	r0, [sp, #4]
	lsls	r1, r1, #10
	adds	r2, r5, #0
	subs	r2, #16
	add	r1, sl
	subs	r3, #16
	ldr	r4, [r4, #0]
	ldr	r0, [sp, #28]
	bl	Func_080072f4
	movs	r2, #128
	lsls	r2, r2, #7
	adds	r7, #1
	adds	r6, r6, r2
	cmp	r7, #4
	bne.n	.L14
.L10:
	ldr	r3, [sp, #12]
	ldr	r1, [sp, #24]
	adds	r3, #2
	adds	r1, #1
	str	r1, [sp, #24]
	str	r3, [sp, #12]
	ldr	r2, [pc, #116]
	mov	r1, sl
	ldr	r3, [r1, r2]
	ldr	r1, [sp, #24]
	ldr	r3, [r3, #20]
	cmp	r1, r3
	bne.n	.L15
.L9:
	ldr	r2, [pc, #112]
	movs	r3, #1
	add	r2, sl
	str	r3, [r2, #0]
	movs	r0, #1
	bl	Func_080030f8
	movs	r2, #1
	add	r8, r2
	ldr	r2, [pc, #84]
	mov	r1, sl
	ldr	r3, [r1, r2]
	ldr	r3, [r3, #20]
	lsls	r3, r3, #4
	adds	r3, #48
	cmp	r8, r3
	beq.n	.L4
	b.n	.L3
.L4:
	ldr	r0, [pc, #52]
	bl	Func_08004278
	ldr	r0, [pc, #40]
	bl	Func_08004278
	movs	r0, #47
	bl	Func_08002dd8
	movs	r0, #46
	bl	Func_08002dd8
	bl	Func_080cdbc0
	add	sp, #64
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x080dbb9d
	.4byte 0x00007784
	.4byte 0x080cd261
	.4byte 0xffff9800
	.4byte 0x04000028
	.4byte 0x00007828
	.4byte 0x03001e80
	.4byte 0xffff9000
	.4byte 0x00007824
