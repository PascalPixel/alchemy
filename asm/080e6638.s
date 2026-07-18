@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080e6638
	.thumb_func
Func_080e6638:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #120]
	ldmia	r3!, {r2}
	ldr	r5, [pc, #120]
	mov	fp, r2
	ldr	r3, [r3, #0]
	sub	sp, #40
	add	r5, fp
	adds	r6, r0, #0
	movs	r0, #128
	str	r3, [sp, #20]
	lsls	r0, r0, #6
	str	r6, [r5, #0]
	bl	Func_080cd594
	ldr	r3, [r5, #0]
	ldr	r2, [r3, #4]
	add	r3, sp, #36
	str	r3, [sp, #0]
	add	r3, sp, #32
	str	r3, [sp, #4]
	adds	r0, r6, #0
	movs	r1, #6
	movs	r3, #2
	bl	Func_080de2f8
	ldr	r2, [pc, #76]
	ldr	r3, [pc, #56]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #56]
	adds	r2, #70
	strh	r3, [r2, #0]
	ldr	r3, [pc, #52]
	subs	r2, #50
	strh	r3, [r2, #0]
	ldr	r3, [r5, #0]
	add	r1, sp, #24
	ldr	r0, [r3, #4]
	bl	Func_080cef64
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, fp
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r2, [pc, #44]
	movs	r3, #75
	add	r2, fp
	movs	r1, #144
	str	r3, [r2, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #36]
	bl	Func_080041d8
	movs	r3, #254
	b.n	.L0
	.4byte 0x00002784
	.4byte 0x00001000
	.4byte 0x000000aa
	.4byte 0x03001eec
	.4byte 0x00007828
	.4byte 0x0400000c
	.4byte 0x00007784
	.4byte 0x080cd261
.L0:
	lsls	r3, r3, #6
	movs	r1, #0
	mov	r9, r3
	mov	r5, fp
	add	r5, r9
	str	r1, [sp, #16]
	mov	sl, r5
.L5:
	ldr	r3, [sp, #16]
	movs	r2, #127
	add	r3, fp
	add	r2, sl
	mov	r7, r9
	adds	r6, r3, #0
	movs	r4, #0
	mov	r8, r2
	add	r7, fp
	adds	r6, #127
	adds	r5, r3, #0
.L4:
	adds	r2, r1, #0
	cmp	r1, #0
	bge.n	.L1
	adds	r2, r1, #7
.L1:
	asrs	r2, r2, #3
	adds	r3, r4, #0
	adds	r2, #64
	subs	r2, r1, r2
	subs	r3, #64
	adds	r0, r3, #0
	muls	r0, r3
	adds	r3, r2, #0
	muls	r3, r2
	str	r1, [sp, #12]
	adds	r0, r0, r3
	str	r4, [sp, #8]
	ldr	r3, [pc, #244]
	bl	Func_080072f0
	lsrs	r3, r0, #31
	adds	r3, r0, r3
	asrs	r0, r3, #1
	ldr	r1, [sp, #12]
	ldr	r4, [sp, #8]
	cmp	r0, #0
	bne.n	.L2
	movs	r0, #1
.L2:
	cmp	r0, #63
	ble.n	.L3
	movs	r0, #63
.L3:
	movs	r2, #1
	mov	r3, r8
	negs	r2, r2
	adds	r4, #1
	strb	r0, [r5, #0]
	add	r8, r2
	strb	r0, [r6, #0]
	adds	r5, #1
	strb	r0, [r7, #0]
	subs	r6, #1
	adds	r7, #1
	strb	r0, [r3, #0]
	cmp	r4, #64
	bne.n	.L4
	ldr	r3, [sp, #16]
	movs	r5, #128
	negs	r5, r5
	adds	r3, #128
	adds	r1, #1
	str	r3, [sp, #16]
	add	r9, r5
	add	sl, r5
	cmp	r1, #64
	bne.n	.L5
	ldr	r4, [pc, #172]
	movs	r7, #1
.L14:
	cmp	r7, #31
	ble.n	.L6
	movs	r3, #64
	subs	r2, r3, r7
	b.n	.L7
.L6:
	adds	r2, r7, #0
.L7:
	lsls	r3, r2, #3
	adds	r0, r3, r2
	subs	r3, r3, r2
	adds	r1, r3, #0
	adds	r2, r3, #0
	subs	r1, #42
	subs	r2, #56
	cmp	r0, #0
	bge.n	.L8
	movs	r0, #0
.L8:
	cmp	r1, #0
	bge.n	.L9
	movs	r1, #0
.L9:
	cmp	r2, #0
	bge.n	.L10
	movs	r2, #0
.L10:
	cmp	r0, #255
	ble.n	.L11
	movs	r0, #255
.L11:
	cmp	r1, #255
	ble.n	.L12
	movs	r1, #255
.L12:
	cmp	r2, #250
	ble.n	.L13
	movs	r2, #250
.L13:
	asrs	r1, r1, #3
	asrs	r2, r2, #3
	movs	r5, #160
	lsls	r2, r2, #10
	lsls	r1, r1, #5
	lsls	r3, r7, #1
	asrs	r0, r0, #3
	lsls	r5, r5, #19
	orrs	r2, r1
	orrs	r2, r0
	adds	r3, r3, r5
	adds	r7, #1
	strh	r2, [r3, #0]
	strh	r2, [r4, #0]
	adds	r4, #2
	cmp	r7, #64
	bne.n	.L14
	movs	r3, #128
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	ldr	r4, [sp, #24]
	ldr	r0, [sp, #20]
	mov	r1, fp
	movs	r2, #0
	movs	r3, #0
	bl	Func_080072f4
	ldr	r2, [pc, #56]
	movs	r3, #1
	add	r2, fp
	movs	r1, #144
	str	r3, [r2, #0]
	ldr	r0, [pc, #52]
	lsls	r1, r1, #3
	bl	Func_080041d8
	ldr	r2, [pc, #48]
	ldr	r3, [pc, #48]
	movs	r4, #0
	mov	r9, r2
	mov	sl, r3
	mov	r8, r4
.L23:
	cmp	r4, #8
	bgt.n	.L15
	ldr	r2, [pc, #12]
	mov	r3, r8
	ldr	r5, [pc, #36]
	orrs	r3, r2
	mov	r1, r8
	strh	r3, [r5, #0]
	b.n	.L16
	.4byte 0x00001000
	.4byte 0x030001d8
	.4byte 0x02010002
	.4byte 0x00007824
	.4byte 0x080dbb9d
	.4byte 0x0201007e
	.4byte 0x04000208
	.4byte 0x04000052
.L15:
	lsls	r1, r4, #1
.L16:
	cmp	r4, #88
	ble.n	.L17
	ldr	r3, [pc, #28]
	mov	r2, r8
	subs	r3, r3, r2
	ldr	r2, [pc, #24]
	ldr	r5, [pc, #28]
	orrs	r3, r2
	strh	r3, [r5, #0]
.L17:
	movs	r6, #211
	lsls	r6, r6, #7
	lsls	r3, r1, #9
	add	r6, fp
	movs	r7, #0
	negs	r5, r3
	b.n	.L18
	movs	r0, r0
	.4byte 0x000000c0
	.4byte 0x00001000
	.4byte 0x04000052
.L18:
	adds	r0, r5, #0
	str	r4, [sp, #8]
	bl	Func_08002322
	lsls	r3, r7, #18
	lsls	r0, r0, #7
	movs	r2, #128
	subs	r3, r3, r0
	lsls	r2, r2, #11
	adds	r3, r3, r2
	asrs	r3, r3, #10
	stmia	r6!, {r3}
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r7, #1
	adds	r5, r5, r3
	ldr	r4, [sp, #8]
	cmp	r7, #160
	bne.n	.L18
	cmp	r4, #127
	ble.n	.L19
	ldr	r2, [pc, #148]
	movs	r3, #1
	add	r2, fp
	str	r3, [r2, #0]
	b.n	.L20
.L19:
	mov	r5, r9
	ldrh	r3, [r5, #0]
	ldr	r2, [pc, #136]
	ldr	r0, [pc, #140]
	strh	r3, [r2, #2]
	mov	r1, r9
	ldr	r3, [pc, #136]
	ldr	r2, [pc, #140]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r1, [pc, #136]
	mov	r5, sl
	ldrh	r3, [r5, #0]
	adds	r0, r3, #0
	mov	r2, sl
	mov	r3, sl
	strh	r2, [r3, #0]
	ldrh	r2, [r1, #0]
	cmp	r2, #31
	bgt.n	.L21
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r2, #1
	adds	r3, r3, r1
	strh	r2, [r1, #0]
	ldr	r2, [pc, #108]
	adds	r3, #4
	stmia	r3!, {r2}
	ldr	r2, [pc, #108]
	stmia	r3!, {r2}
	ldr	r2, [pc, #108]
	str	r2, [r3, #0]
.L21:
	mov	r5, sl
	strh	r0, [r5, #0]
.L20:
	movs	r0, #1
	str	r4, [sp, #8]
	bl	Func_080030f8
	ldr	r4, [sp, #8]
	movs	r2, #2
	adds	r4, #1
	add	r8, r2
	cmp	r4, #96
	beq.n	.L22
	b.n	.L23
.L22:
	ldr	r0, [pc, #80]
	bl	Func_08004278
	ldr	r0, [pc, #80]
	bl	Func_08004278
	movs	r0, #47
	bl	Func_08002dd8
	movs	r0, #46
	bl	Func_08002dd8
	bl	Func_080cdbc0
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
	.4byte 0x00007824
	.4byte 0x02010000
	.4byte 0x0201007c
	.4byte 0x040000d4
	.4byte 0x80a0003e
	.4byte 0x02002090
	.4byte 0x02010002
	.4byte 0x05000002
	.4byte 0x8000003f
	.4byte 0x080dbb9d
	.4byte 0x080cd261
