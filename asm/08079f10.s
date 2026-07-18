@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08079f10
	.thumb_func
Func_08079f10:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	mov	r9, r1
	adds	r5, r0, #0
	mov	r0, r9
	mov	r8, r3
	mov	fp, r2
	bl	Func_08077394
	adds	r6, r0, #0
	movs	r0, #1
	mov	sl, r0
	mov	r0, r8
	bl	Func_08079ef8
	cmp	r0, #0
	beq.n	.L0
	movs	r1, #56
	ldrsh	r3, [r6, r1]
	movs	r0, #0
	cmp	r3, #0
	beq.n	.L0
	b.n	.L1
.L0:
	mov	r2, r8
	cmp	r2, #3
	bne.n	.L2
	ldr	r4, [pc, #360]
	adds	r3, r6, r4
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	bne.n	.L2
	b.n	.L3
.L5:
	ldr	r7, [pc, #348]
	adds	r3, r6, r7
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L4
	movs	r0, #158
	lsls	r0, r0, #1
	adds	r3, r6, r0
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L4
	ldr	r1, [pc, #328]
	adds	r3, r6, r1
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L4
	ldr	r2, [pc, #324]
	adds	r3, r6, r2
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L4
	b.n	.L3
.L2:
	mov	r3, r8
	cmp	r3, #4
	bne.n	.L4
	movs	r4, #156
	lsls	r4, r4, #1
	adds	r3, r6, r4
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L4
	ldr	r7, [pc, #296]
	adds	r3, r6, r7
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L4
	movs	r0, #157
	lsls	r0, r0, #1
	adds	r3, r6, r0
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L5
.L4:
	ldr	r1, [pc, #260]
	ldr	r7, [pc, #272]
	adds	r3, r6, r1
	adds	r1, r6, r7
	adds	r7, #2
	adds	r7, r7, r6
	movs	r4, #156
	lsls	r4, r4, #1
	mov	ip, r7
	movs	r7, #158
	lsls	r7, r7, #1
	adds	r2, r6, r4
	adds	r4, #2
	adds	r0, r6, r4
	adds	r4, r6, r7
	mov	r7, r8
	cmp	r7, #64
	bne.n	.L6
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	bne.n	.L6
	ldrb	r3, [r2, #0]
	cmp	r3, #0
	bne.n	.L6
	ldrb	r3, [r1, #0]
	cmp	r3, #0
	bne.n	.L6
	ldrb	r3, [r0, #0]
	cmp	r3, #0
	bne.n	.L6
	mov	r0, ip
	ldrb	r3, [r0, #0]
	cmp	r3, #0
	bne.n	.L6
	ldrb	r3, [r4, #0]
	cmp	r3, #0
	bne.n	.L6
	ldr	r1, [pc, #188]
	adds	r3, r6, r1
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L6
	ldr	r2, [pc, #184]
	adds	r3, r6, r2
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L6
	movs	r4, #160
	lsls	r4, r4, #1
	adds	r3, r6, r4
	ldrb	r3, [r3, #0]
	movs	r0, #0
	cmp	r3, #0
	beq.n	.L1
.L6:
	mov	r7, r8
	cmp	r7, #28
	bne.n	.L7
	ldr	r0, [pc, #152]
	adds	r3, r6, r0
	ldrb	r3, [r3, #0]
	movs	r0, #0
	cmp	r3, #1
	beq.n	.L1
.L7:
	mov	r0, r8
	bl	Func_08079d7c
	adds	r7, r0, #0
	cmp	r7, #0
	ble.n	.L8
	adds	r0, r5, #0
	mov	r1, fp
	bl	Func_0807987c
	mov	r1, fp
	adds	r5, r0, #0
	mov	r0, r9
	bl	Func_0807987c
	adds	r3, r6, #0
	adds	r3, #66
	ldrb	r3, [r3, #0]
	subs	r5, r5, r0
	lsrs	r3, r3, #1
	subs	r5, r5, r3
	lsls	r3, r5, #1
	adds	r3, r3, r5
	adds	r0, r6, #0
	mov	r1, r8
	adds	r7, r7, r3
	bl	Func_08079e9c
	cmp	r0, #0
	beq.n	.L9
	adds	r7, #25
	b.n	.L9
.L11:
	movs	r0, #1
	b.n	.L1
.L8:
	negs	r7, r7
.L9:
	mov	r1, r8
	cmp	r1, #67
	bne.n	.L10
	movs	r2, #3
	mov	sl, r2
.L10:
	movs	r6, #0
	cmp	r6, sl
	bge.n	.L3
	ldr	r3, [sp, #32]
	muls	r7, r3
.L12:
	adds	r0, r7, #0
	movs	r1, #100
	bl	Func_080022ec
	adds	r5, r0, #0
	bl	Func_08079be8
	cmp	r5, r0
	bge.n	.L11
	adds	r6, #1
	cmp	r6, sl
	blt.n	.L12
.L3:
	movs	r0, #0
.L1:
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x00000131
	.4byte 0x0000013b
	.4byte 0x0000013d
	.4byte 0x00000141
	.4byte 0x00000139
