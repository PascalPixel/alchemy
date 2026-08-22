@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080b88d0
	.thumb_func
Func_080b88d0:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #128
	str	r0, [sp, #12]
	ldr	r1, [sp, #12]
	movs	r2, #0
	ldrsh	r1, [r1, r2]
	movs	r0, #0
	mov	r9, r0
	adds	r0, r1, #0
	str	r1, [sp, #8]
	bl	Func_080b8808
	cmp	r0, #0
	blt.n	.L0
	ldr	r2, [sp, #12]
	movs	r3, #10
	ldrsh	r2, [r2, r3]
	adds	r0, r2, #0
	str	r2, [sp, #4]
	bl	Func_080b8808
	cmp	r0, #0
	bge.n	.L1
.L0:
	movs	r0, #1
	negs	r0, r0
	b.n	.L2
.L1:
	ldr	r3, [pc, #68]
	ldr	r1, [sp, #12]
	ldr	r2, [r3, #0]
	movs	r0, #0
	ldrsh	r3, [r1, r0]
	movs	r1, #160
	lsls	r1, r1, #7
	cmp	r3, #4
	bgt.n	.L3
	movs	r1, #128
	lsls	r1, r1, #6
.L3:
	movs	r3, #60
	str	r1, [r2, #0]
	str	r3, [r2, #4]
	movs	r0, #10
	bl	Func_080030f8
	bl	Func_08004458
	ldr	r0, [sp, #8]
	bl	Func_080b7dd0
	ldr	r2, [sp, #4]
	ldr	r6, [r0, #0]
	cmp	r2, #7
	bhi.n	.L4
	add	r3, sp, #100
	mov	sl, r3
	movs	r0, #2
	mov	r1, sl
	bl	Func_080b6b40
	mov	fp, r0
	movs	r0, #128
	str	r0, [sp, #0]
	b.n	.L5
	.4byte 0x03001f00
.L4:
	add	r1, sp, #100
	movs	r0, #1
	mov	sl, r1
	bl	Func_080b6b40
	movs	r2, #0
	str	r2, [sp, #0]
	mov	fp, r0
.L5:
	mov	r3, fp
	movs	r5, #0
	cmp	r3, #0
	beq.n	.L6
.L8:
	ldr	r0, [sp, #0]
	ldr	r1, [sp, #8]
	adds	r3, r5, r0
	cmp	r3, r1
	bne.n	.L7
	adds	r0, r6, #0
	movs	r1, #3
	bl	Func_08009080
.L7:
	adds	r5, #1
	cmp	r5, fp
	bne.n	.L8
.L6:
	movs	r0, #30
	bl	Func_080030f8
	ldr	r2, [pc, #52]
	ldr	r3, [pc, #36]
	strh	r3, [r2, #0]
	mov	r2, fp
	movs	r5, #0
	cmp	r2, #0
	beq.n	.L9
.L10:
	ldr	r3, [sp, #0]
	movs	r1, #1
	adds	r0, r5, r3
	adds	r5, #1
	bl	Func_080c0f98
	cmp	r5, fp
	bne.n	.L10
.L9:
	ldr	r0, [pc, #24]
	ldr	r7, [pc, #12]
	ldr	r6, [pc, #12]
	movs	r5, #0
	mov	r8, r0
	b.n	.L11
	.4byte 0x00003f40
	.4byte 0x00000010
	.4byte 0x00001000
	.4byte 0x04000050
	.4byte 0x04000052
.L11:
	subs	r3, r7, r5
	orrs	r3, r6
	mov	r1, r8
	strh	r3, [r1, #0]
	movs	r0, #1
	adds	r5, #1
	bl	Func_080030f8
	cmp	r5, #16
	bne.n	.L11
	movs	r0, #9
	bl	Func_08015130
	ldr	r2, [sp, #4]
	cmp	r2, #127
	ble.n	.L12
	movs	r0, #2
	mov	r1, sl
	bl	Func_080b6b40
	mov	r8, r0
	movs	r5, #0
	cmp	r9, r8
	beq.n	.L13
	mov	r0, r9
	lsls	r3, r0, #1
	mov	r1, sl
	adds	r7, r3, r1
.L15:
	adds	r6, r5, #0
	adds	r6, #128
	adds	r0, r6, #0
	bl	Func_08077008
	movs	r2, #56
	ldrsh	r3, [r0, r2]
	cmp	r3, #0
	ble.n	.L14
	movs	r3, #1
	strh	r6, [r7, #0]
	add	r9, r3
	adds	r7, #2
.L14:
	adds	r5, #1
	cmp	r9, r8
	bne.n	.L15
	b.n	.L13
.L12:
	movs	r0, #1
	mov	r1, sl
	bl	Func_080b6b40
	adds	r7, r0, #0
	movs	r5, #0
	cmp	r9, r7
	beq.n	.L13
	mov	r0, r9
	lsls	r3, r0, #1
	mov	r1, sl
	adds	r6, r3, r1
.L17:
	adds	r0, r5, #0
	bl	Func_08077008
	movs	r2, #56
	ldrsh	r3, [r0, r2]
	cmp	r3, #0
	ble.n	.L16
	movs	r3, #1
	strh	r5, [r6, #0]
	add	r9, r3
	adds	r6, #2
.L16:
	adds	r5, #1
	cmp	r9, r7
	bne.n	.L17
.L13:
	ldr	r2, [pc, #48]
	mov	r0, r9
	lsls	r3, r0, #1
	mov	r1, sl
	strh	r2, [r1, r3]
	mov	r0, sl
	movs	r1, #0
	bl	Func_080b7b6c
	ldr	r1, [sp, #12]
	movs	r2, #8
	ldrsh	r3, [r1, r2]
	add	r0, sp, #16
	str	r3, [r0, #0]
	ldr	r2, [sp, #8]
	mov	r3, r9
	str	r2, [r0, #8]
	movs	r5, #0
	cmp	r3, #0
	beq.n	.L18
	mov	r1, sl
	add	r2, sp, #52
	movs	r4, #0
	b.n	.L19
	movs	r0, r0
	.4byte 0x000000ff
.L19:
	ldrh	r3, [r4, r1]
	adds	r5, #1
	strh	r3, [r2, #0]
	adds	r4, #2
	adds	r2, #2
	cmp	r5, r9
	bne.n	.L19
.L18:
	mov	r1, r9
	str	r1, [r0, #20]
	ldr	r2, [sp, #4]
	cmp	r2, #7
	bhi.n	.L20
	movs	r3, #1
	b.n	.L21
.L20:
	movs	r3, #0
.L21:
	str	r3, [r0, #4]
	bl	Func_080c9010
	movs	r0, #10
	bl	Func_080030f8
	bl	Func_080b6c90
	ldr	r3, [pc, #36]
	ldr	r2, [pc, #40]
	strh	r3, [r2, #0]
	mov	r3, fp
	movs	r5, #0
	cmp	r3, #0
	beq.n	.L22
.L23:
	ldr	r1, [sp, #0]
	adds	r0, r5, r1
	movs	r1, #1
	adds	r5, #1
	bl	Func_080c0f98
	cmp	r5, fp
	bne.n	.L23
.L22:
	ldr	r7, [pc, #16]
	ldr	r6, [pc, #8]
	movs	r5, #0
	b.n	.L24
	.4byte 0x00003f40
	.4byte 0x00001000
	.4byte 0x04000050
	.4byte 0x04000052
.L24:
	adds	r3, r5, #0
	orrs	r3, r6
	strh	r3, [r7, #0]
	movs	r0, #1
	adds	r5, #1
	bl	Func_080030f8
	cmp	r5, #16
	bne.n	.L24
	mov	r2, fp
	movs	r5, #0
	cmp	r2, #0
	beq.n	.L25
.L26:
	ldr	r3, [sp, #0]
	movs	r1, #0
	adds	r0, r5, r3
	adds	r5, #1
	bl	Func_080c0f98
	cmp	r5, fp
	bne.n	.L26
.L25:
	movs	r0, #0
	movs	r1, #0
	movs	r2, #0
	movs	r3, #100
	bl	Func_080c0cec
	movs	r0, #3
	bl	Func_080030f8
	movs	r0, #0
.L2:
	add	sp, #128
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
