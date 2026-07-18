@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0808bd24
	.thumb_func
Func_0808bd24:
	push	{r5, r6, r7, lr}
	ldr	r2, [pc, #164]
	movs	r1, #250
	lsls	r1, r1, #1
	adds	r2, r2, r1
	ldr	r3, [pc, #160]
	ldr	r2, [r2, #0]
	ldr	r6, [r3, #0]
	lsls	r2, r2, #2
	adds	r2, #20
	ldr	r2, [r6, r2]
	subs	r3, #76
	sub	sp, #12
	ldr	r7, [r3, #0]
	movs	r0, #0
	cmp	r2, #0
	beq.n	.L0
	ldr	r3, [r2, #8]
	mov	r5, sp
	str	r3, [r5, #0]
	ldr	r3, [r2, #12]
	str	r3, [r5, #4]
	ldr	r3, [r2, #16]
	str	r3, [r5, #8]
	movs	r0, #128
	ldrh	r1, [r2, #6]
	lsls	r0, r0, #13
	adds	r2, r5, #0
	bl	Func_0800447c
	movs	r2, #207
	lsls	r2, r2, #1
	adds	r3, r6, r2
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #3
	bne.n	.L1
	ldr	r3, [r5, #0]
	cmp	r3, #0
	bge.n	.L2
	ldr	r2, [pc, #92]
	adds	r3, r3, r2
.L2:
	ldr	r2, [r5, #8]
	asrs	r1, r3, #21
	movs	r0, #31
	ands	r1, r0
	cmp	r2, #0
	bge.n	.L3
	ldr	r3, [pc, #76]
	adds	r2, r2, r3
.L3:
	asrs	r3, r2, #21
	ands	r3, r0
	lsls	r3, r3, #5
	adds	r3, r1, r3
	ldr	r2, [pc, #68]
	lsls	r3, r3, #2
	adds	r1, r3, r2
	b.n	.L4
.L1:
	movs	r1, #152
	lsls	r1, r1, #1
	adds	r3, r7, r1
	ldr	r1, [r3, #0]
	ldr	r3, [r5, #0]
	cmp	r3, #0
	bge.n	.L5
	ldr	r2, [pc, #52]
	adds	r3, r3, r2
.L5:
	ldr	r2, [r5, #8]
	asrs	r0, r3, #20
	cmp	r2, #0
	bge.n	.L6
	ldr	r3, [pc, #40]
	adds	r2, r2, r3
.L6:
	asrs	r3, r2, #20
	lsls	r3, r3, #7
	adds	r3, r0, r3
	lsls	r3, r3, #2
	adds	r1, r1, r3
.L4:
	ldrb	r0, [r1, #2]
.L0:
	add	sp, #12
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x02000240
	.4byte 0x03001ebc
	.4byte 0x001fffff
	.4byte 0x02020000
	.4byte 0x000fffff
