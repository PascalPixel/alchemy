@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080d9fc8
	.thumb_func
Func_080d9fc8:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r6, [pc, #88]
	adds	r3, r6, #0
	ldmia	r3!, {r1}
	ldr	r5, [pc, #84]
	mov	r8, r1
	ldr	r3, [r3, #0]
	sub	sp, #20
	add	r5, r8
	str	r3, [sp, #16]
	str	r0, [r5, #0]
	movs	r0, #0
	bl	Func_080cd594
	ldr	r2, [pc, #68]
	ldr	r3, [pc, #52]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #52]
	adds	r2, #2
	strh	r3, [r2, #0]
	ldr	r0, [pc, #60]
	mov	r1, r8
	movs	r2, #1
	movs	r3, #1
	bl	Func_080e0524
	movs	r3, #3
	movs	r2, #7
	movs	r0, #46
	movs	r1, #7
	str	r3, [sp, #0]
	bl	Func_080ed408
	ldr	r6, [r6, #28]
	str	r6, [sp, #8]
	ldr	r3, [r5, #0]
	movs	r2, #36
	ldrsh	r3, [r3, r2]
	cmp	r3, #127
	ble.n	.L0
	b.n	.L1
	movs	r0, r0
	.4byte 0x00003f46
	.4byte 0x0000100e
	.4byte 0x03001eec
	.4byte 0x00007828
	.4byte 0x04000050
	.4byte 0x000000b4
.L1:
	movs	r3, #0
	movs	r4, #1
	mov	fp, r3
	mov	sl, r4
	b.n	.L2
.L0:
	movs	r1, #1
	movs	r0, #64
	negs	r1, r1
	mov	fp, r0
	mov	sl, r1
.L2:
	movs	r5, #225
	movs	r2, #0
	lsls	r5, r5, #7
	ldr	r6, [pc, #460]
	mov	r9, r2
	movs	r7, #0
	add	r5, r8
.L3:
	adds	r0, r6, #0
	bl	Func_08002322
	lsls	r0, r0, #5
	asrs	r0, r0, #16
	mov	r3, sl
	muls	r3, r0
	add	r3, fp
	adds	r3, #20
	str	r3, [r5, #0]
	adds	r0, r6, #0
	bl	Func_0800231c
	lsls	r0, r0, #4
	asrs	r0, r0, #16
	movs	r4, #1
	adds	r0, #40
	movs	r3, #128
	add	r9, r4
	str	r0, [r5, #4]
	lsls	r3, r3, #5
	mov	r0, r9
	str	r7, [r5, #24]
	adds	r6, r6, r3
	subs	r7, #4
	adds	r5, #28
	cmp	r0, #9
	bne.n	.L3
	movs	r2, #239
	lsls	r2, r2, #7
	movs	r3, #2
	add	r2, r8
	str	r3, [r2, #0]
	ldr	r3, [pc, #388]
	add	r3, r8
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #24]
	cmp	r3, #2
	bne.n	.L4
	ldr	r2, [pc, #380]
	movs	r3, #75
	b.n	.L5
.L4:
	ldr	r2, [pc, #376]
	movs	r3, #50
.L5:
	add	r2, r8
	str	r3, [r2, #0]
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #368]
	bl	Func_080041d8
	movs	r0, #136
	bl	Func_080f9010
	movs	r1, #0
	str	r1, [sp, #12]
.L16:
	ldr	r2, [sp, #12]
	cmp	r2, #24
	bne.n	.L6
	movs	r0, #133
	bl	Func_080b50e8
.L6:
	movs	r6, #225
	movs	r3, #0
	lsls	r6, r6, #7
	mov	r9, r3
	add	r6, r8
.L11:
	ldr	r3, [r6, #24]
	cmp	r3, #23
	bhi.n	.L7
	adds	r2, r3, #0
	cmp	r3, #0
	bge.n	.L8
	adds	r2, r3, #3
.L8:
	asrs	r5, r2, #2
	ldr	r0, [pc, #316]
	ldr	r2, [pc, #320]
	lsls	r4, r5, #1
	ldrh	r1, [r0, r4]
	mov	sl, r4
	ldrb	r4, [r2, r5]
	ldr	r2, [r6, #0]
	lsrs	r3, r4, #1
	subs	r2, r2, r3
	ldr	r3, [pc, #308]
	ldrb	r0, [r3, r5]
	mov	fp, r3
	ldr	r3, [r6, #4]
	str	r4, [sp, #0]
	ldr	r4, [pc, #300]
	adds	r3, r3, r0
	ldr	r7, [pc, #272]
	ldrb	r0, [r4, r5]
	add	r1, r8
	str	r0, [sp, #4]
	ldr	r4, [sp, #8]
	ldr	r0, [sp, #16]
	add	r7, r8
	bl	Func_080072f4
	ldr	r2, [r7, #0]
	ldr	r3, [r2, #24]
	cmp	r3, #0
	beq.n	.L9
	ldr	r3, [pc, #264]
	ldr	r0, [pc, #256]
	ldrb	r4, [r3, r5]
	mov	r2, sl
	ldrh	r1, [r0, r2]
	ldr	r2, [r6, #0]
	lsrs	r3, r4, #1
	subs	r2, r2, r3
	mov	r3, fp
	ldrb	r0, [r3, r5]
	ldr	r3, [r6, #4]
	str	r4, [sp, #0]
	ldr	r4, [pc, #248]
	adds	r3, r3, r0
	ldrb	r0, [r4, r5]
	add	r1, r8
	str	r0, [sp, #4]
	subs	r3, #16
	ldr	r0, [sp, #16]
	ldr	r4, [sp, #8]
	bl	Func_080072f4
	ldr	r2, [r7, #0]
.L9:
	ldr	r3, [r2, #24]
	cmp	r3, #2
	bne.n	.L10
	ldr	r3, [pc, #212]
	ldr	r0, [pc, #204]
	ldrb	r4, [r3, r5]
	mov	r2, sl
	ldrh	r1, [r0, r2]
	ldr	r2, [r6, #0]
	lsrs	r3, r4, #1
	subs	r2, r2, r3
	mov	r3, fp
	ldrb	r0, [r3, r5]
	ldr	r3, [r6, #4]
	str	r4, [sp, #0]
	ldr	r4, [pc, #196]
	adds	r3, r3, r0
	ldrb	r0, [r4, r5]
	subs	r3, #32
	str	r0, [sp, #4]
	add	r1, r8
	ldr	r0, [sp, #16]
	ldr	r4, [sp, #8]
	bl	Func_080072f4
.L10:
	ldr	r3, [r6, #24]
.L7:
	movs	r0, #1
	add	r9, r0
	adds	r3, #1
	mov	r1, r9
	str	r3, [r6, #24]
	adds	r6, #28
	cmp	r1, #9
	bne.n	.L11
	movs	r2, #0
	mov	r9, r2
	ldr	r2, [pc, #128]
	mov	r4, r8
	ldr	r3, [r4, r2]
	ldr	r3, [r3, #20]
	cmp	r3, #0
	beq.n	.L12
	movs	r6, #36
	movs	r5, #16
.L14:
	ldr	r0, [sp, #12]
	cmp	r0, r5
	bne.n	.L13
	mov	r1, r8
	ldr	r3, [r1, r2]
	ldrsh	r0, [r3, r6]
	movs	r3, #12
	str	r3, [sp, #0]
	movs	r1, #10
	movs	r2, #5
	mov	r3, r9
	bl	Func_080d6888
.L13:
	ldr	r2, [pc, #88]
	movs	r3, #1
	mov	r4, r8
	add	r9, r3
	ldr	r3, [r4, r2]
	ldr	r3, [r3, #20]
	adds	r6, #2
	adds	r5, #8
	cmp	r9, r3
	bne.n	.L14
.L12:
	bl	Func_080cd52c
	ldr	r2, [pc, #92]
	movs	r3, #1
	add	r2, r8
	movs	r0, #1
	str	r3, [r2, #0]
	bl	Func_080030f8
	ldr	r0, [sp, #12]
	adds	r0, #1
	str	r0, [sp, #12]
	cmp	r0, #80
	beq.n	.L15
	b.n	.L16
.L15:
	movs	r0, #46
	bl	Func_08002dd8
	ldr	r0, [pc, #40]
	bl	Func_08004278
	bl	Func_080cdbc0
	add	sp, #20
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0xffffc000
	.4byte 0x00007828
	.4byte 0x00007784
	.4byte 0x080cd261
	.4byte 0x080edeb2
	.4byte 0x080ede9f
	.4byte 0x080edeab
	.4byte 0x080edea5
	.4byte 0x00007824
