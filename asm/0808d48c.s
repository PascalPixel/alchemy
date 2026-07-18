@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0808d48c
	.thumb_func
Func_0808d48c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #228]
	movs	r2, #250
	ldr	r7, [r3, #0]
	ldr	r3, [pc, #224]
	lsls	r2, r2, #1
	adds	r3, r3, r2
	mov	fp, r0
	ldr	r0, [r3, #0]
	ldr	r6, [r7, #16]
	mov	r9, r1
	bl	Func_0808ba1c
	movs	r3, #1
	ldrh	r0, [r0, #6]
	ldr	r5, [r6, #0]
	negs	r3, r3
	mov	r8, r0
	cmp	r5, r3
	beq.n	.L0
	ldr	r2, [pc, #196]
	mov	sl, r2
.L11:
	movs	r3, #15
	ands	r3, r5
	cmp	r3, fp
	bne.n	.L1
	ldrb	r3, [r6, #4]
	cmp	r3, r9
	bne.n	.L1
	ldr	r1, [r6, #8]
	adds	r0, r5, #0
	bl	Func_0808d458
	cmp	r0, #0
	bne.n	.L2
	movs	r3, #6
	ldrsh	r0, [r6, r3]
	bl	Func_0808d428
	cmp	r0, #0
	beq.n	.L1
	ldr	r5, [r6, #0]
.L2:
	movs	r3, #128
	lsls	r3, r3, #4
	ands	r3, r5
	movs	r4, #0
	movs	r0, #12
	cmp	r3, #0
	beq.n	.L3
	movs	r0, #2
.L3:
	movs	r1, #192
	lsls	r1, r1, #3
	adds	r3, r5, #0
	movs	r2, #128
	ands	r3, r1
	lsls	r2, r2, #2
	cmp	r3, r2
	beq.n	.L4
	cmp	r3, r2
	bgt.n	.L5
	cmp	r3, #0
	beq.n	.L6
	b.n	.L7
.L5:
	movs	r2, #128
	lsls	r2, r2, #3
	cmp	r3, r2
	beq.n	.L8
	cmp	r3, r1
	beq.n	.L9
	b.n	.L7
.L4:
	movs	r2, #206
	lsls	r2, r2, #1
	adds	r3, r7, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, r0
	ble.n	.L7
	b.n	.L6
.L9:
	movs	r2, #206
	lsls	r2, r2, #1
	adds	r3, r7, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, r0
	ble.n	.L7
.L8:
	movs	r3, #240
	lsls	r3, r3, #8
	mov	r2, r8
	ands	r5, r3
	subs	r3, r5, r2
	ldr	r2, [pc, #60]
	adds	r3, r3, r2
	lsls	r3, r3, #16
	lsrs	r3, r3, #16
	cmp	r3, sl
	bhi.n	.L7
.L6:
	movs	r4, #1
.L7:
	adds	r0, r6, #0
	cmp	r4, #0
	bne.n	.L10
.L1:
	adds	r6, #12
	ldr	r3, [r6, #0]
	movs	r2, #1
	negs	r2, r2
	adds	r5, r3, #0
	cmp	r3, r2
	bne.n	.L11
.L0:
	movs	r0, #0
.L10:
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x03001ebc
	.4byte 0x02000240
	.4byte 0x00003ffe
	.4byte 0x00001fff
