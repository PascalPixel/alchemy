@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0808b8e8
	.thumb_func
Func_0808b8e8:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #136]
	movs	r0, #240
	ldr	r1, [r3, #0]
	lsls	r0, r0, #1
	adds	r3, r1, r0
	ldr	r2, [r3, #0]
	ldr	r0, [pc, #128]
	ldr	r3, [r2, #8]
	adds	r0, r0, r3
	mov	r9, r0
	movs	r0, #160
	lsls	r0, r0, #16
	adds	r0, r0, r3
	ldr	r3, [r2, #16]
	ldr	r2, [pc, #116]
	adds	r5, r1, #0
	mov	sl, r0
	movs	r1, #2
	movs	r0, #200
	adds	r2, r2, r3
	lsls	r0, r0, #15
	negs	r1, r1
	mov	r8, r2
	adds	r7, r3, r0
	adds	r5, #52
	mov	fp, r1
	movs	r6, #57
.L3:
	ldr	r0, [r5, #0]
	cmp	r0, #0
	beq.n	.L0
	ldr	r3, [r0, #8]
	ldr	r2, [r0, #16]
	cmp	r3, #0
	bne.n	.L1
	cmp	r2, #0
	beq.n	.L0
.L1:
	cmp	r3, r9
	blt.n	.L2
	cmp	r3, sl
	bgt.n	.L2
	cmp	r2, r8
	blt.n	.L2
	cmp	r2, r7
	ble.n	.L0
.L2:
	adds	r2, r0, #0
	adds	r2, #84
	movs	r3, #1
	strb	r3, [r2, #0]
	ldr	r2, [r0, #80]
	ldrb	r3, [r2, #29]
	mov	r1, fp
	ands	r3, r1
	strb	r3, [r2, #29]
	bl	Func_080090d0
	movs	r2, #0
	str	r2, [r5, #0]
.L0:
	subs	r6, #1
	adds	r5, #4
	cmp	r6, #0
	bge.n	.L3
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001ebc
	.4byte 0xff600000
	.4byte 0xff380000
