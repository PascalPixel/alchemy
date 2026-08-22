@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0809abb4
	.thumb_func
Func_0809abb4:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	ldr	r3, [pc, #420]
	ldr	r3, [r3, #0]
	ldr	r5, [r3, #16]
	mov	r9, r3
	ldr	r3, [r5, #12]
	mov	r0, r9
	str	r3, [r0, #8]
	movs	r1, #0
	movs	r0, #250
	movs	r2, #0
	movs	r3, #0
	sub	sp, #36
	bl	Func_08096c80
	movs	r1, #0
	adds	r6, r0, #0
	movs	r7, #0
	bl	Func_08009080
	cmp	r6, #0
	bne.n	.L0
	b.n	.L1
.L0:
	bl	Func_08097384
	ldr	r3, [r5, #8]
	add	r1, sp, #12
	str	r3, [r1, #0]
	movs	r2, #128
	ldr	r3, [r5, #12]
	lsls	r2, r2, #13
	adds	r3, r3, r2
	str	r3, [r1, #4]
	ldr	r3, [r5, #16]
	str	r3, [r1, #8]
	mov	r0, r9
	ldr	r3, [r0, #4]
	mov	r2, sp
	str	r3, [r2, #0]
	ldr	r3, [r0, #8]
	movs	r0, #128
	lsls	r0, r0, #12
	adds	r3, r3, r0
	str	r3, [r2, #4]
	mov	r0, r9
	ldr	r3, [r0, #12]
	str	r3, [r2, #8]
	mov	sl, r1
	mov	r8, r2
.L2:
	mov	r2, r8
	mov	r0, sl
	ldr	r3, [r2, #0]
	ldr	r5, [r0, #0]
	subs	r3, r3, r5
	adds	r0, r7, #0
	muls	r0, r3
	movs	r1, #10
	bl	Func_080022ec
	adds	r5, r5, r0
	str	r5, [r6, #8]
	mov	r2, r8
	mov	r0, sl
	ldr	r3, [r2, #4]
	ldr	r5, [r0, #4]
	subs	r3, r3, r5
	adds	r0, r7, #0
	muls	r0, r3
	movs	r1, #10
	bl	Func_080022ec
	adds	r5, r5, r0
	str	r5, [r6, #12]
	mov	r2, r8
	mov	r0, sl
	ldr	r3, [r2, #8]
	ldr	r5, [r0, #8]
	subs	r3, r3, r5
	adds	r0, r7, #0
	muls	r0, r3
	movs	r1, #10
	bl	Func_080022ec
	movs	r3, #192
	lsls	r3, r3, #8
	adds	r5, r5, r0
	movs	r1, #10
	adds	r0, r7, #0
	muls	r0, r3
	str	r5, [r6, #16]
	bl	Func_080022ec
	movs	r2, #128
	lsls	r2, r2, #7
	adds	r0, r0, r2
	str	r0, [r6, #24]
	str	r0, [r6, #28]
	adds	r7, #1
	movs	r0, #1
	bl	Func_080030f8
	cmp	r7, #11
	blt.n	.L2
	movs	r0, #5
	bl	Func_080030f8
	movs	r1, #1
	adds	r0, r6, #0
	bl	Func_08009080
	movs	r0, #108
	bl	Func_080f9010
	movs	r0, #10
	bl	Func_080030f8
	movs	r0, #108
	bl	Func_080f9010
	movs	r0, #10
	bl	Func_080030f8
	movs	r0, #108
	bl	Func_080f9010
	movs	r0, #10
	bl	Func_080030f8
	movs	r0, #109
	bl	Func_080f9010
	add	r3, sp, #24
	mov	r5, r9
	mov	r8, r3
	movs	r0, #15
	adds	r5, #88
	mov	r7, r8
	mov	sl, r0
.L3:
	ldr	r3, [r6, #8]
	str	r3, [r7, #0]
	movs	r2, #128
	ldr	r3, [r6, #12]
	lsls	r2, r2, #12
	adds	r3, r3, r2
	str	r3, [r7, #4]
	ldr	r3, [r6, #16]
	adds	r0, r7, #0
	str	r3, [r7, #8]
	bl	Func_080974d8
	bl	Func_08004458
	adds	r1, r0, #0
	movs	r0, #128
	lsls	r0, r0, #11
	adds	r2, r7, #0
	bl	Func_0800447c
	ldr	r3, [r7, #8]
	ldr	r2, [r7, #0]
	adds	r0, r5, #0
	ldr	r1, [pc, #104]
	bl	Func_0809ba90
	adds	r0, r5, #0
	ldr	r1, [pc, #100]
	bl	Func_0809ba7c
	ldr	r0, [r5, #0]
	movs	r1, #7
	bl	Func_08009248
	movs	r3, #1
	negs	r3, r3
	add	sl, r3
	mov	r0, sl
	adds	r5, #72
	cmp	r0, #0
	bge.n	.L3
	ldr	r3, [r6, #8]
	mov	r2, r8
	str	r3, [r2, #0]
	movs	r0, #128
	ldr	r3, [r6, #12]
	lsls	r0, r0, #12
	adds	r3, r3, r0
	str	r3, [r2, #4]
	ldr	r3, [r6, #16]
	movs	r0, #8
	str	r3, [r2, #8]
	bl	Func_080030f8
	adds	r0, r6, #0
	bl	Func_080090d0
	movs	r0, #4
	bl	Func_080030f8
	movs	r0, #30
	bl	Func_080030f8
	bl	Func_0809748c
.L1:
	add	sp, #36
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001f30
	.4byte 0x0000011d
	.4byte 0x0809aa99
