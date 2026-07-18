@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0808ce74
	.thumb_func
Func_0808ce74:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #228]
	movs	r1, #250
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r0, #0
	mov	sl, r0
	ldr	r0, [r3, #0]
	sub	sp, #12
	bl	Func_0808ba1c
	ldr	r3, [pc, #212]
	ldr	r6, [r3, #0]
	subs	r3, #76
	ldr	r3, [r3, #0]
	adds	r7, r0, #0
	mov	r8, r3
	cmp	r7, #0
	beq.n	.L0
	ldr	r3, [r7, #8]
	mov	r5, sp
	str	r3, [r5, #0]
	ldr	r3, [r7, #12]
	str	r3, [r5, #4]
	ldr	r3, [r7, #16]
	str	r3, [r5, #8]
	movs	r0, #128
	lsls	r0, r0, #13
	adds	r2, r5, #0
	ldrh	r1, [r7, #6]
	bl	Func_0800447c
	movs	r2, #207
	lsls	r2, r2, #1
	adds	r3, r6, r2
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	cmp	r3, #3
	bne.n	.L1
	ldr	r3, [r5, #0]
	cmp	r3, #0
	bge.n	.L2
	ldr	r1, [pc, #156]
	adds	r3, r3, r1
.L2:
	asrs	r2, r3, #21
	ldr	r3, [r5, #8]
	movs	r1, #31
	ands	r2, r1
	cmp	r3, #0
	bge.n	.L3
	ldr	r0, [pc, #140]
	adds	r3, r3, r0
.L3:
	asrs	r3, r3, #21
	ands	r3, r1
	lsls	r3, r3, #5
	adds	r3, r2, r3
	ldr	r2, [pc, #132]
	lsls	r3, r3, #2
	adds	r1, r3, r2
	b.n	.L4
.L1:
	movs	r3, #152
	lsls	r3, r3, #1
	add	r3, r8
	ldr	r1, [r3, #0]
	ldr	r3, [r5, #0]
	cmp	r3, #0
	bge.n	.L5
	ldr	r0, [pc, #112]
	adds	r3, r3, r0
.L5:
	asrs	r2, r3, #20
	ldr	r3, [r5, #8]
	cmp	r3, #0
	bge.n	.L6
	ldr	r0, [pc, #100]
	adds	r3, r3, r0
.L6:
	asrs	r3, r3, #20
	lsls	r3, r3, #7
	adds	r3, r2, r3
	lsls	r3, r3, #2
	adds	r1, r1, r3
.L4:
	ldrb	r6, [r1, #2]
	adds	r3, r6, #0
	subs	r3, #242
	cmp	r3, #5
	bhi.n	.L7
	adds	r3, r7, #0
	adds	r3, #34
	ldr	r2, [r5, #8]
	ldrb	r0, [r3, #0]
	ldr	r1, [r5, #0]
	bl	Func_080091a8
	adds	r2, r0, #0
	ldr	r0, [r7, #12]
	cmp	r2, r0
	blt.n	.L0
	movs	r1, #128
	lsls	r1, r1, #15
	adds	r3, r0, r1
	cmp	r2, r3
	bgt.n	.L0
	b.n	.L8
.L7:
	movs	r0, #3
	adds	r1, r6, #0
	bl	Func_0808d48c
	cmp	r0, #0
	beq.n	.L0
.L8:
	mov	sl, r6
.L0:
	mov	r0, sl
	add	sp, #12
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x02000240
	.4byte 0x03001ebc
	.4byte 0x001fffff
	.4byte 0x02020000
	.4byte 0x000fffff
