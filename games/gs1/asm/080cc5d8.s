@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080cc5d8
	.thumb_func
Func_080cc5d8:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #40
	str	r0, [sp, #24]
	ldr	r1, [pc, #120]
	movs	r0, #39
	bl	Func_080048b0
	movs	r1, #128
	mov	r9, r0
	lsls	r1, r1, #7
	movs	r0, #40
	bl	Func_080048b0
	ldr	r1, [pc, #104]
	str	r0, [sp, #20]
	movs	r0, #41
	bl	Func_080048b0
	ldr	r5, [pc, #96]
	str	r0, [sp, #8]
	ldr	r0, [sp, #24]
	add	r5, r9
	str	r0, [r5, #0]
	movs	r0, #0
	bl	Func_080cd594
	ldr	r2, [pc, #84]
	movs	r3, #24
	add	r2, r9
	str	r3, [r2, #0]
	ldr	r2, [pc, #80]
	movs	r3, #0
	add	r2, r9
	str	r3, [r2, #0]
	ldr	r2, [pc, #76]
	ldr	r3, [pc, #48]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #48]
	subs	r2, #50
	strh	r3, [r2, #0]
	ldr	r0, [pc, #68]
	mov	r1, r9
	movs	r2, #1
	movs	r3, #0
	bl	Func_080e0524
	movs	r3, #0
	ldr	r0, [pc, #60]
	ldr	r1, [sp, #8]
	movs	r2, #0
	bl	Func_080e0524
	ldr	r3, [r5, #0]
	ldr	r3, [r3, #0]
	cmp	r3, #1
	beq.n	.L0
	cmp	r3, #1
	bgt.n	.L1
	b.n	.L2
	movs	r0, r0
	.4byte 0x0000100c
	.4byte 0x00000100
	.4byte 0x0000782c
	.4byte 0x0000060e
	.4byte 0x00007828
	.4byte 0x000077b4
	.4byte 0x000077b8
	.4byte 0x04000052
	.4byte 0x00000045
	.4byte 0x00000076
.L2:
	cmp	r3, #0
	beq.n	.L3
	b.n	.L4
.L1:
	cmp	r3, #2
	beq.n	.L5
	b.n	.L4
.L3:
	ldr	r0, [pc, #8]
	b.n	.L6
.L0:
	ldr	r0, [pc, #8]
	b.n	.L6
.L5:
	ldr	r0, [pc, #8]
	b.n	.L6
	.4byte 0x00000048
	.4byte 0x00000057
	.4byte 0x00000047
.L4:
	ldr	r0, [pc, #272]
.L6:
	bl	Func_08002f40
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #268]
	lsls	r0, r0, #19
	movs	r2, #128
	bl	Func_080072f0
	ldr	r5, [pc, #260]
	movs	r7, #0
.L7:
	movs	r3, #128
	lsls	r3, r3, #16
	str	r3, [r5, #4]
	bl	Func_08004458
	ldr	r3, [pc, #252]
	ands	r3, r0
	str	r3, [r5, #0]
	bl	Func_08004458
	ldr	r3, [pc, #244]
	movs	r1, #128
	ands	r3, r0
	lsls	r1, r1, #3
	adds	r3, r3, r1
	str	r3, [r5, #8]
	negs	r3, r7
	adds	r7, #1
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r7, #128
	bne.n	.L7
	movs	r5, #225
	lsls	r5, r5, #7
	movs	r7, #0
	add	r5, r9
.L8:
	bl	Func_08004458
	ldr	r3, [pc, #204]
	ands	r3, r0
	str	r3, [r5, #0]
	bl	Func_08004458
	movs	r3, #31
	ands	r3, r0
	adds	r3, #16
	str	r3, [r5, #4]
	movs	r3, #15
	ands	r3, r7
	adds	r3, #16
	adds	r7, #1
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r7, #64
	bne.n	.L8
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, r9
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r2, [pc, #168]
	movs	r3, #75
	add	r2, r9
	movs	r1, #144
	str	r3, [r2, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #160]
	bl	Func_080041d8
	movs	r3, #3
	str	r3, [sp, #0]
	movs	r2, #7
	movs	r1, #7
	movs	r3, #7
	movs	r0, #46
	bl	Func_080ed408
	ldr	r3, [pc, #144]
	adds	r3, #184
	ldr	r3, [r3, #0]
	movs	r0, #140
	str	r3, [sp, #12]
	bl	Func_080f9010
	movs	r3, #28
	movs	r2, #0
	add	r3, sp
	mov	sl, r2
	mov	fp, r3
.L19:
	ldr	r4, [sp, #24]
	mov	r1, fp
	ldr	r0, [r4, #8]
	mov	r5, fp
	bl	Func_080e396c
	ldr	r2, [r5, #0]
	movs	r3, #64
	ldr	r1, [pc, #108]
	subs	r3, r3, r2
	lsls	r3, r3, #8
	mov	r0, sl
	str	r3, [r1, #0]
	cmp	r0, #49
	ble.n	.L9
	ldr	r3, [pc, #52]
	lsls	r2, r0, #1
	subs	r3, r3, r2
	ldr	r2, [pc, #52]
	adds	r1, #42
	orrs	r3, r2
	strh	r3, [r1, #0]
.L9:
	mov	r1, sl
	cmp	r1, #26
	bne.n	.L10
	movs	r0, #212
	bl	Func_080f9010
	ldr	r3, [pc, #72]
	add	r3, r9
	ldr	r3, [r3, #0]
	movs	r2, #36
	ldrsh	r0, [r3, r2]
	movs	r3, #20
	movs	r2, #1
	str	r3, [sp, #0]
	movs	r1, #7
	negs	r2, r2
	movs	r3, #0
	bl	Func_080d6888
	b.n	.L10
	movs	r0, r0
	.4byte 0x00000070
	.4byte 0x00001000
	.4byte 0x00000046
	.4byte 0x03001388
	.4byte 0x02010000
	.4byte 0x0000ffff
	.4byte 0x000001ff
	.4byte 0x00007784
	.4byte 0x080cd261
	.4byte 0x03001e50
	.4byte 0x04000028
	.4byte 0x00007828
.L10:
	mov	r0, sl
	subs	r0, #28
	cmp	r0, #20
	bhi.n	.L11
	movs	r1, #3
	bl	Func_080022ec
	lsls	r1, r0, #3
	adds	r1, r1, r0
	lsls	r1, r1, #8
	movs	r3, #160
	lsls	r3, r3, #5
	add	r1, r9
	mov	r4, fp
	adds	r1, r1, r3
	ldr	r3, [r4, #4]
	movs	r2, #48
	str	r2, [sp, #0]
	str	r2, [sp, #4]
	subs	r3, #24
	ldr	r0, [sp, #20]
	movs	r2, #40
	ldr	r5, [sp, #12]
	bl	Func_080072f8
.L11:
	mov	r0, sl
	cmp	r0, #14
	bhi.n	.L12
	movs	r1, #3
	bl	Func_080022ec
	movs	r1, #5
	bl	Func_080022fc
	ldr	r6, [pc, #280]
	lsls	r0, r0, #10
	movs	r7, #0
	mov	r8, r0
.L13:
	ldr	r3, [pc, #276]
	ldrb	r2, [r3, r7]
	movs	r3, #3
	orrs	r3, r2
	movs	r2, #2
	str	r2, [sp, #0]
	movs	r1, #7
	movs	r2, #7
	movs	r0, #47
	bl	Func_080ed408
	ldr	r3, [pc, #260]
	ldrsb	r2, [r3, r7]
	ldr	r3, [pc, #260]
	mov	r5, fp
	ldrsb	r1, [r3, r7]
	ldr	r3, [r5, #4]
	ldr	r4, [r6, #0]
	adds	r3, r3, r1
	movs	r1, #32
	str	r1, [sp, #0]
	str	r1, [sp, #4]
	mov	r1, r9
	str	r4, [sp, #16]
	adds	r2, #32
	subs	r3, #32
	ldr	r0, [sp, #20]
	add	r1, r8
	bl	Func_080072f4
	adds	r7, #1
	movs	r0, #47
	bl	Func_08002dd8
	cmp	r7, #4
	bne.n	.L13
.L12:
	mov	r0, sl
	cmp	r0, #0
	blt.n	.L14
	movs	r5, #225
	lsls	r5, r5, #7
	movs	r7, #0
	add	r5, r9
.L17:
	ldr	r2, [r5, #24]
	cmp	r2, #0
	blt.n	.L15
	ldr	r3, [r5, #4]
	cmp	r3, #0
	ble.n	.L15
	asrs	r3, r2, #3
	ldr	r0, [r5, #0]
	adds	r6, r3, #1
	bl	Func_08002322
	ldr	r3, [r5, #4]
	muls	r3, r0
	asrs	r3, r3, #16
	adds	r3, #64
	ldr	r0, [r5, #0]
	mov	r8, r3
	bl	Func_0800231c
	ldr	r3, [r5, #4]
	muls	r3, r0
	mov	r1, fp
	ldr	r2, [r1, #4]
	asrs	r3, r3, #16
	adds	r4, r3, r2
	cmp	r6, #0
	bgt.n	.L16
	movs	r6, #1
.L16:
	lsls	r0, r6, #1
	ldr	r2, [pc, #144]
	subs	r3, r0, #2
	ldrh	r1, [r2, r3]
	ldr	r2, [sp, #8]
	mov	r3, r8
	adds	r1, r2, r1
	str	r0, [sp, #0]
	subs	r2, r3, r6
	str	r0, [sp, #4]
	subs	r3, r4, r6
	ldr	r0, [sp, #20]
	ldr	r4, [sp, #12]
	bl	Func_080072f4
	ldr	r3, [r5, #4]
	subs	r3, #2
	str	r3, [r5, #4]
	ldr	r3, [r5, #24]
	subs	r3, #1
	str	r3, [r5, #24]
.L15:
	adds	r7, #1
	adds	r5, #28
	cmp	r7, #64
	bne.n	.L17
.L14:
	bl	Func_080cd52c
	ldr	r2, [pc, #96]
	movs	r3, #1
	add	r2, r9
	movs	r5, #1
	movs	r0, #1
	str	r3, [r2, #0]
	add	sl, r5
	bl	Func_080030f8
	mov	r0, sl
	cmp	r0, #56
	beq.n	.L18
	b.n	.L19
.L18:
	movs	r0, #46
	bl	Func_08002dd8
	ldr	r0, [pc, #68]
	bl	Func_08004278
	bl	Func_080cdbc0
	movs	r0, #41
	bl	Func_08002dd8
	movs	r0, #40
	bl	Func_08002dd8
	movs	r0, #39
	bl	Func_08002dd8
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
	.4byte 0x03001f0c
	.4byte 0x080ee060
	.4byte 0x080ee058
	.4byte 0x080ee05c
	.4byte 0x080ede5c
	.4byte 0x00007824
	.4byte 0x080cd261
