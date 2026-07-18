@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0808ef70
	.thumb_func
Func_0808ef70:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	ldr	r2, [pc, #284]
	lsls	r0, r0, #2
	ldr	r6, [r2, #0]
	adds	r0, #20
	ldr	r7, [r6, r0]
	sub	sp, #12
	mov	r9, r1
	mov	sl, r2
	movs	r0, #0
	cmp	r7, #0
	bne.n	.L0
	b.n	.L1
.L0:
	ldr	r3, [r7, #8]
	mov	r5, sp
	str	r3, [r5, #0]
	ldr	r3, [r7, #12]
	str	r3, [r5, #4]
	ldr	r3, [r7, #16]
	str	r3, [r5, #8]
	movs	r0, #128
	ldrh	r1, [r7, #6]
	adds	r2, r5, #0
	lsls	r0, r0, #13
	bl	Func_0800447c
	ldr	r1, [pc, #236]
	ldr	r3, [r5, #0]
	movs	r2, #128
	lsls	r2, r2, #12
	ands	r3, r1
	adds	r3, r3, r2
	mov	r8, r3
	ldr	r3, [r5, #8]
	ands	r3, r1
	adds	r7, r3, r2
	ldr	r2, [pc, #220]
	adds	r3, r6, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	beq.n	.L2
	mov	r3, sl
	subs	r3, #88
	ldr	r5, [r3, #0]
	movs	r6, #63
.L5:
	ldr	r1, [r5, #0]
	cmp	r1, #0
	beq.n	.L3
	ldr	r2, [r5, #108]
	ldr	r3, [pc, #196]
	cmp	r2, r3
	bne.n	.L4
	adds	r0, r5, #0
	bl	Func_080090d0
	ldr	r1, [r5, #0]
.L4:
	ldr	r3, [pc, #188]
	cmp	r1, r3
	bne.n	.L3
	adds	r0, r5, #0
	bl	Func_080090d0
.L3:
	subs	r6, #1
	adds	r5, #112
	cmp	r6, #0
	bge.n	.L5
.L2:
	movs	r0, #3
	bl	Func_080030f8
	movs	r2, #128
	adds	r3, r7, #0
	lsls	r2, r2, #13
	movs	r0, #22
	mov	r1, r8
	bl	Func_080090c8
	adds	r7, r0, #0
	movs	r0, #0
	cmp	r7, #0
	beq.n	.L1
	ldr	r1, [pc, #144]
	adds	r0, r7, #0
	bl	Func_08009098
	ldr	r6, [r7, #80]
	adds	r2, r6, #0
	movs	r3, #0
	adds	r2, #38
	strb	r3, [r2, #0]
	adds	r2, #1
	strb	r3, [r2, #0]
	ldrb	r2, [r6, #5]
	subs	r3, #33
	ands	r3, r2
	ldrb	r2, [r6, #9]
	strb	r3, [r6, #5]
	movs	r3, #15
	ands	r3, r2
	movs	r2, #13
	negs	r2, r2
	ands	r3, r2
	movs	r2, #4
	orrs	r3, r2
	strb	r3, [r6, #9]
	movs	r3, #128
	lsls	r3, r3, #10
	str	r3, [r7, #40]
	movs	r3, #128
	lsls	r3, r3, #7
	movs	r1, #193
	str	r3, [r7, #72]
	lsls	r1, r1, #3
	movs	r0, #17
	bl	Func_080048b0
	adds	r5, r0, #0
	mov	r0, r9
	bl	Func_08015250
	movs	r3, #128
	lsls	r3, r3, #3
	adds	r5, r5, r3
	movs	r1, #128
	adds	r2, r5, #0
	ldrb	r0, [r6, #28]
	bl	Func_08003fa4
	ldr	r3, [pc, #24]
	ldrh	r2, [r6, #8]
	ands	r0, r3
	ldr	r3, [pc, #48]
	ands	r3, r2
	orrs	r3, r0
	strh	r3, [r6, #8]
	movs	r0, #17
	bl	Func_08002dd8
	ldr	r3, [pc, #36]
	str	r3, [r7, #108]
	adds	r0, r7, #0
	b.n	.L1
	.4byte 0x000003ff
	.4byte 0x03001ebc
	.4byte 0xfff00000
	.4byte 0x00000cb8
	.4byte 0x0808f28d
	.4byte 0x0809e87c
	.4byte 0x0809e6c0
	.4byte 0xfffffc00
	.4byte 0x0808eee5
.L1:
	add	sp, #12
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
