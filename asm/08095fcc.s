@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08095fcc
	.thumb_func
Func_08095fcc:
	push	{r5, r6, r7, lr}
	ldr	r3, [pc, #108]
	movs	r1, #250
	lsls	r1, r1, #1
	adds	r3, r3, r1
	adds	r7, r0, #0
	ldr	r0, [r3, #0]
	sub	sp, #12
	bl	Func_08092054
	adds	r3, r7, #0
	adds	r3, #100
	ldrh	r1, [r3, #0]
	subs	r1, #1
	strh	r1, [r3, #0]
	adds	r6, r0, #0
	ldr	r3, [r6, #8]
	mov	r5, sp
	str	r3, [r5, #0]
	ldr	r3, [r6, #16]
	lsls	r1, r1, #16
	str	r3, [r5, #8]
	ldr	r3, [pc, #68]
	asrs	r1, r1, #16
	adds	r0, r1, #0
	muls	r0, r3
	adds	r3, r7, #0
	adds	r3, #102
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	lsls	r1, r1, #11
	adds	r1, r1, r3
	adds	r2, r5, #0
	bl	Func_0800447c
	ldr	r3, [r5, #0]
	str	r3, [r7, #8]
	ldr	r3, [r5, #8]
	ldr	r2, [r7, #12]
	str	r3, [r7, #16]
	ldr	r3, [pc, #36]
	adds	r2, r2, r3
	str	r2, [r7, #12]
	movs	r1, #160
	ldr	r3, [r6, #12]
	lsls	r1, r1, #13
	adds	r3, r3, r1
	cmp	r2, r3
	bge.n	.L0
	adds	r0, r7, #0
	bl	Func_080090d0
.L0:
	add	sp, #12
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x02000240
	.4byte 0x00006666
	.4byte 0xffff0000
