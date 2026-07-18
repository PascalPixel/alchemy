@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080a602c
	.thumb_func
Func_080a602c:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #68]
	adds	r4, r0, #0
	lsls	r0, r0, #2
	mov	r8, r0
	ldr	r7, [r3, #0]
	mov	r3, r8
	adds	r3, #20
	ldr	r0, [r7, r3]
	movs	r5, #0
	movs	r3, #1
	strb	r3, [r0, #5]
	strh	r5, [r0, #12]
	movs	r0, #135
	lsls	r0, r0, #2
	adds	r3, r7, r0
	ldr	r2, [r3, #0]
	subs	r0, #3
	movs	r3, #13
	strb	r3, [r2, #5]
	adds	r3, r7, r0
	ldrb	r3, [r3, #0]
	adds	r4, #28
	adds	r2, r7, #2
	ldrsb	r1, [r7, r4]
	strb	r3, [r2, r4]
	movs	r2, #1
	negs	r2, r2
	cmp	r1, r2
	bne.n	.L0
	ldr	r3, [pc, #4]
	movs	r6, #0
	strb	r3, [r7, r4]
	b.n	.L1
	.4byte 0x00000000
	.4byte 0x03001f2c
.L0:
	lsls	r6, r1, #1
	adds	r0, r6, r1
	lsls	r0, r0, #3
	subs	r0, #10
	movs	r1, #16
	bl	Func_080a1ac0
.L1:
	movs	r5, #130
	lsls	r5, r5, #2
	adds	r3, r6, r5
	ldrh	r0, [r7, r3]
	bl	Func_08077008
	movs	r3, #228
	lsls	r3, r3, #1
	adds	r6, r7, r3
	adds	r1, r6, #0
	movs	r2, #2
	bl	Func_080a68ec
	movs	r2, #134
	lsls	r2, r2, #2
	adds	r3, r7, r2
	adds	r5, r7, r5
	adds	r1, r6, #0
	strb	r0, [r3, #0]
	adds	r0, r5, #0
	bl	Func_080a60d4
	mov	r3, r8
	adds	r3, #20
	adds	r5, r0, #0
	ldr	r0, [r7, r3]
	bl	Func_080a17c4
	movs	r0, #1
	bl	Func_080030f8
	adds	r0, r5, #0
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
