@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080d3854
	.thumb_func
Func_080d3854:
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
	sub	sp, #52
	str	r1, [sp, #40]
	ldr	r2, [pc, #84]
	ldr	r3, [r3, #0]
	str	r3, [sp, #36]
	adds	r3, r1, r2
	str	r0, [r3, #0]
	movs	r0, #1
	bl	Func_080cd594
	ldr	r2, [pc, #72]
	ldr	r3, [pc, #60]
	ldr	r0, [pc, #72]
	strh	r3, [r2, #0]
	bl	Func_08002f40
	adds	r5, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #64]
	adds	r1, r5, #0
	movs	r2, #128
	lsls	r0, r0, #19
	bl	Func_080072f0
	adds	r5, #128
	ldr	r1, [sp, #40]
	adds	r0, r5, #0
	bl	Func_08005340
	movs	r5, #2
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	movs	r0, #46
	str	r5, [sp, #0]
	bl	Func_080ed408
	ldr	r3, [r6, #28]
	movs	r1, #7
	str	r3, [sp, #44]
	movs	r2, #7
	b.n	.L0
	.4byte 0x00001010
	.4byte 0x03001eec
	.4byte 0x00007828
	.4byte 0x04000052
	.4byte 0x000000ce
	.4byte 0x03001388
.L0:
	movs	r3, #7
	movs	r0, #47
	str	r5, [sp, #0]
	bl	Func_080ed408
	ldr	r3, [r6, #32]
	mov	r0, sp
	adds	r0, #44
	str	r0, [sp, #24]
	str	r3, [r0, #4]
	ldr	r2, [sp, #40]
	movs	r3, #225
	movs	r1, #0
	lsls	r3, r3, #7
	mov	r8, r1
	adds	r5, r2, r3
.L1:
	bl	Func_08004458
	movs	r3, #31
	ands	r3, r0
	str	r3, [r5, #0]
	bl	Func_08004458
	movs	r3, #63
	ands	r3, r0
	adds	r3, #16
	str	r3, [r5, #4]
	bl	Func_08004458
	movs	r3, #15
	ands	r3, r0
	movs	r0, #1
	add	r8, r0
	negs	r3, r3
	mov	r1, r8
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r1, #16
	bne.n	.L1
	ldr	r3, [sp, #40]
	movs	r0, #239
	lsls	r0, r0, #7
	adds	r2, r3, r0
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r1, [sp, #40]
	ldr	r3, [pc, #76]
	adds	r2, r1, r3
	movs	r3, #50
	movs	r1, #144
	lsls	r1, r1, #3
	str	r3, [r2, #0]
	ldr	r0, [pc, #68]
	bl	Func_080041d8
	ldr	r2, [pc, #68]
	ldr	r3, [pc, #52]
	movs	r0, #1
	strh	r3, [r2, #0]
	bl	Func_080030f8
	movs	r0, #141
	bl	Func_080f9010
	ldr	r2, [pc, #52]
	ldr	r1, [sp, #40]
	adds	r2, r1, r2
	movs	r0, #0
	str	r2, [sp, #28]
	mov	fp, r0
.L21:
	mov	r3, fp
	lsls	r0, r3, #10
	bl	Func_08002322
	lsls	r0, r0, #4
	str	r0, [sp, #32]
	mov	r0, fp
	cmp	r0, #32
	bne.n	.L2
	movs	r0, #133
	bl	Func_080b50e8
	b.n	.L2
	movs	r0, r0
	.4byte 0x00001000
	.4byte 0x00007784
	.4byte 0x080cd261
	.4byte 0x04000052
	.4byte 0x00007828
.L2:
	movs	r1, #0
	ldr	r6, [pc, #56]
	mov	r9, r1
	movs	r5, #16
.L4:
	cmp	fp, r5
	bne.n	.L3
	movs	r1, #128
	ldr	r0, [sp, #36]
	lsls	r1, r1, #7
	ldr	r2, [pc, #44]
	bl	Func_080072fc
.L3:
	movs	r2, #1
	add	r9, r2
	mov	r3, r9
	adds	r5, #8
	cmp	r3, #7
	bne.n	.L4
	ldr	r0, [sp, #28]
	ldr	r3, [r0, #0]
	ldr	r3, [r3, #4]
	cmp	r3, #1
	bne.n	.L5
	ldr	r1, [sp, #32]
	movs	r2, #128
	lsls	r2, r2, #14
	adds	r1, r1, r2
	str	r1, [sp, #32]
	b.n	.L6
	movs	r0, r0
	.4byte 0x03000168
	.4byte 0x08080808
.L5:
	ldr	r3, [sp, #32]
	ldr	r0, [pc, #68]
	adds	r3, r3, r0
	str	r3, [sp, #32]
.L6:
	mov	r1, fp
	cmp	r1, #16
	bgt.n	.L7
	ldr	r2, [pc, #48]
	ldr	r1, [pc, #56]
	mov	r3, fp
	orrs	r3, r2
	strh	r3, [r1, #0]
.L7:
	mov	r2, fp
	cmp	r2, #63
	ble.n	.L8
	ldr	r2, [pc, #36]
	mov	r0, fp
	ldr	r1, [pc, #28]
	ldr	r3, [pc, #36]
	subs	r2, r2, r0
	orrs	r2, r1
	strh	r2, [r3, #0]
.L8:
	ldr	r2, [sp, #28]
	ldr	r3, [r2, #0]
	ldr	r2, [r3, #24]
	ldr	r0, [pc, #28]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	ldrb	r3, [r0, r3]
	movs	r1, #0
	mov	r9, r1
	b.n	.L9
	.4byte 0x00001000
	.4byte 0x0000004f
	.4byte 0xffe00000
	.4byte 0x04000052
	.4byte 0x080ee1ca
.L9:
	cmp	r3, #0
	bne.n	.L10
	b.n	.L11
.L10:
	mov	r1, fp
	mov	r2, fp
	ldr	r3, [sp, #40]
	asrs	r1, r1, #31
	lsls	r2, r2, #11
	str	r1, [sp, #20]
	str	r2, [sp, #16]
	str	r3, [sp, #12]
.L15:
	ldr	r0, [sp, #16]
	bl	Func_08002322
	ldr	r1, [sp, #28]
	ldr	r3, [r1, #0]
	ldr	r2, [r3, #24]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	ldr	r2, [pc, #516]
	adds	r3, #1
	ldrb	r3, [r2, r3]
	muls	r3, r0
	ldr	r0, [sp, #32]
	adds	r3, r3, r0
	asrs	r3, r3, #16
	ldr	r0, [sp, #16]
	adds	r3, #40
	mov	sl, r3
	bl	Func_0800231c
	ldr	r1, [sp, #20]
	lsls	r0, r0, #1
	asrs	r7, r0, #16
	lsrs	r0, r1, #31
	add	r0, fp
	movs	r1, #3
	asrs	r0, r0, #1
	bl	Func_080022fc
	lsls	r5, r0, #2
	ldr	r2, [sp, #40]
	adds	r5, r5, r0
	lsls	r6, r5, #9
	ldr	r3, [pc, #468]
	adds	r6, r2, r6
	adds	r1, r6, r3
	movs	r0, #40
	movs	r2, #32
	adds	r3, r7, #0
	str	r0, [sp, #0]
	str	r2, [sp, #4]
	ldr	r4, [sp, #44]
	ldr	r0, [sp, #36]
	adds	r3, #16
	mov	r2, sl
	bl	Func_080072f4
	ldr	r3, [sp, #40]
	ldr	r0, [pc, #444]
	lsls	r5, r5, #8
	adds	r5, r3, r5
	adds	r5, r5, r0
	movs	r1, #40
	movs	r2, #32
	adds	r3, r7, #0
	str	r1, [sp, #0]
	str	r2, [sp, #4]
	ldr	r4, [sp, #44]
	adds	r3, #48
	ldr	r0, [sp, #36]
	adds	r1, r5, #0
	mov	r2, sl
	bl	Func_080072f4
	ldr	r3, [pc, #416]
	movs	r0, #40
	adds	r6, r6, r3
	movs	r1, #32
	adds	r3, r7, #0
	adds	r3, #80
	str	r0, [sp, #0]
	str	r1, [sp, #4]
	mov	r2, sl
	adds	r1, r6, #0
	ldr	r4, [sp, #44]
	ldr	r0, [sp, #36]
	bl	Func_080072f4
	movs	r0, #225
	ldr	r3, [sp, #12]
	movs	r2, #0
	lsls	r0, r0, #7
	mov	r8, r2
	adds	r6, r3, r0
.L14:
	ldr	r3, [r6, #24]
	cmp	r3, #0
	blt.n	.L12
	mov	r1, r8
	lsrs	r2, r1, #31
	add	r2, r8
	asrs	r2, r2, #1
	lsrs	r4, r3, #31
	adds	r4, r3, r4
	lsls	r3, r2, #1
	adds	r3, r3, r2
	asrs	r4, r4, #1
	movs	r2, #1
	mov	r5, r8
	adds	r4, r4, r3
	ands	r5, r2
	ldr	r0, [pc, #348]
	ldr	r2, [pc, #348]
	lsls	r3, r4, #1
	ldrh	r1, [r2, r3]
	ldrb	r0, [r0, r4]
	ldr	r3, [sp, #40]
	ldr	r2, [r6, #0]
	adds	r1, r3, r1
	ldr	r3, [r6, #4]
	str	r0, [sp, #0]
	ldr	r0, [pc, #336]
	ldrb	r0, [r0, r4]
	str	r0, [sp, #4]
	ldr	r0, [sp, #24]
	lsls	r5, r5, #2
	adds	r3, r3, r7
	ldr	r4, [r5, r0]
	add	r2, sl
	ldr	r0, [sp, #36]
	bl	Func_080072f4
	ldr	r3, [r6, #24]
.L12:
	adds	r3, #1
	str	r3, [r6, #24]
	cmp	r3, #6
	bne.n	.L13
	bl	Func_08004458
	movs	r3, #31
	ands	r3, r0
	str	r3, [r6, #0]
	bl	Func_08004458
	movs	r3, #63
	ands	r3, r0
	adds	r3, #16
	str	r3, [r6, #4]
	movs	r3, #0
	str	r3, [r6, #24]
.L13:
	movs	r1, #1
	add	r8, r1
	mov	r2, r8
	adds	r6, #28
	cmp	r2, #4
	bne.n	.L14
	ldr	r3, [sp, #16]
	ldr	r1, [sp, #12]
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r3, r3, r0
	adds	r1, #112
	str	r1, [sp, #12]
	str	r3, [sp, #16]
	ldr	r0, [sp, #28]
	ldr	r3, [r0, #0]
	movs	r2, #1
	add	r9, r2
	ldr	r2, [r3, #24]
	ldr	r1, [pc, #216]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	ldrb	r3, [r1, r3]
	cmp	r9, r3
	beq.n	.L11
	b.n	.L15
.L11:
	ldr	r0, [sp, #28]
	ldr	r3, [r0, #0]
	ldr	r3, [r3, #20]
	movs	r2, #0
	mov	r8, r2
	cmp	r3, #0
	beq.n	.L16
	ldr	r1, [sp, #40]
	ldr	r2, [pc, #212]
	movs	r3, #0
	adds	r7, r1, r2
	mov	sl, r3
	movs	r4, #36
.L19:
	movs	r0, #0
	mov	r5, sl
	mov	r9, r0
	adds	r6, r4, #0
	adds	r5, #16
.L18:
	cmp	fp, r5
	bne.n	.L17
	ldr	r3, [r7, #0]
	ldrsh	r0, [r3, r6]
	movs	r3, #4
	str	r3, [sp, #0]
	movs	r2, #5
	movs	r1, #7
	mov	r3, r8
	str	r4, [sp, #8]
	bl	Func_080d6888
	ldr	r3, [r7, #0]
	movs	r1, #6
	ldrsh	r0, [r3, r6]
	bl	Func_080b5088
	ldr	r4, [sp, #8]
.L17:
	movs	r3, #1
	add	r9, r3
	mov	r0, r9
	adds	r5, #8
	cmp	r0, #7
	bne.n	.L18
	add	r8, r3
	ldr	r3, [r7, #0]
	ldr	r3, [r3, #20]
	movs	r1, #3
	add	sl, r1
	adds	r4, #2
	cmp	r8, r3
	bne.n	.L19
.L16:
	ldr	r2, [sp, #40]
	ldr	r0, [pc, #132]
	movs	r1, #1
	adds	r3, r2, r0
	str	r1, [r3, #0]
	ldr	r2, [sp, #28]
	ldr	r3, [r2, #0]
	ldr	r2, [r3, #24]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	ldr	r1, [pc, #80]
	adds	r3, #2
	ldrb	r0, [r1, r3]
	lsls	r1, r0, #1
	bl	Func_080e155c
	bl	Func_080cd52c
	ldr	r0, [pc, #100]
	ldr	r2, [sp, #40]
	movs	r1, #1
	adds	r3, r2, r0
	str	r1, [r3, #0]
	movs	r0, #1
	bl	Func_080030f8
	movs	r2, #1
	add	fp, r2
	mov	r3, fp
	cmp	r3, #80
	beq.n	.L20
	b.n	.L21
.L20:
	movs	r0, #47
	bl	Func_08002dd8
	movs	r0, #46
	bl	Func_08002dd8
	ldr	r0, [pc, #64]
	bl	Func_08004278
	bl	Func_080cdbc0
	add	sp, #52
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x080ee1ca
	.4byte 0x00000c56
	.4byte 0x00002a56
	.4byte 0x00001156
	.4byte 0x080edeca
	.4byte 0x080edebe
	.4byte 0x080eded0
	.4byte 0x00007828
	.4byte 0x000077a8
	.4byte 0x00007824
	.4byte 0x080cd261
