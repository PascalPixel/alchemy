@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0808df1c
	.thumb_func
Func_0808df1c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #12
	mov	r8, r1
	movs	r1, #1
	str	r0, [sp, #8]
	negs	r1, r1
	mov	r0, r8
	str	r1, [sp, #4]
	bl	Func_0808ddb8
	str	r0, [sp, #0]
	ldr	r0, [sp, #8]
	bl	Func_0808ba1c
	adds	r7, r0, #0
	cmp	r7, #0
	bne.n	.L0
	b.n	.L1
.L0:
	ldrh	r3, [r7, #6]
	movs	r2, #128
	lsls	r2, r2, #6
	adds	r2, r2, r3
	movs	r3, #192
	lsls	r3, r3, #8
	ands	r2, r3
	movs	r3, #0
	mov	fp, r2
	mov	r9, r3
.L15:
	ldr	r1, [sp, #8]
	cmp	r9, r1
	beq.n	.L2
	mov	r0, r9
	bl	Func_0808ba1c
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L2
	movs	r2, #89
	adds	r2, r2, r6
	mov	sl, r2
	ldrb	r2, [r2, #0]
	movs	r3, #8
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L2
	movs	r0, #128
	mov	r3, r8
	lsls	r0, r0, #12
	cmp	r3, #13
	bne.n	.L3
	movs	r0, #192
	lsls	r0, r0, #14
.L3:
	mov	r1, r8
	cmp	r1, #5
	bne.n	.L4
	movs	r0, #128
	lsls	r0, r0, #15
.L4:
	mov	r2, r8
	cmp	r2, #2
	bne.n	.L5
	movs	r0, #128
	lsls	r0, r0, #13
.L5:
	ldr	r1, [r6, #12]
	ldr	r3, [r7, #12]
	subs	r2, r1, r3
	cmp	r2, #0
	blt.n	.L6
	cmp	r2, r0
	ble.n	.L7
	b.n	.L2
.L6:
	subs	r3, r3, r1
	cmp	r3, r0
	bgt.n	.L2
.L7:
	ldr	r2, [r6, #8]
	ldr	r3, [r7, #8]
	subs	r0, r2, r3
	cmp	r0, #0
	bge.n	.L8
	ldr	r3, [pc, #168]
	adds	r0, r0, r3
.L8:
	ldr	r2, [r6, #16]
	ldr	r3, [r7, #16]
	subs	r2, r2, r3
	asrs	r0, r0, #16
	cmp	r2, #0
	bge.n	.L9
	ldr	r1, [pc, #152]
	adds	r2, r2, r1
.L9:
	asrs	r3, r2, #16
	adds	r1, r3, #0
	muls	r1, r3
	adds	r2, r0, #0
	muls	r2, r0
	adds	r3, r1, #0
	adds	r0, r2, #0
	adds	r0, r0, r3
	ldr	r3, [pc, #136]
	bl	Func_080072f0
	mov	r3, sl
	ldrb	r2, [r3, #0]
	movs	r3, #16
	ands	r3, r2
	adds	r5, r0, #0
	cmp	r3, #0
	beq.n	.L10
	lsls	r0, r5, #1
	movs	r1, #3
	bl	Func_080022ec
	adds	r5, r0, #0
.L10:
	ldr	r1, [sp, #0]
	cmp	r5, r1
	bge.n	.L2
	ldr	r3, [r7, #16]
	ldr	r0, [r6, #16]
	ldr	r1, [r6, #8]
	subs	r0, r0, r3
	ldr	r3, [r7, #8]
	subs	r1, r1, r3
	bl	Func_080044d0
	movs	r2, #192
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	lsls	r2, r2, #5
	cmp	r5, #19
	ble.n	.L11
	movs	r2, #128
	lsls	r2, r2, #5
.L11:
	mov	r3, r8
	cmp	r3, #2
	bne.n	.L12
	movs	r2, #128
	lsls	r2, r2, #6
.L12:
	cmp	r5, #11
	ble.n	.L13
	mov	r1, fp
	subs	r3, r0, r1
	lsls	r3, r3, #16
	asrs	r0, r3, #16
	cmp	r0, #0
	bge.n	.L14
	negs	r0, r0
.L14:
	cmp	r0, r2
	bge.n	.L2
.L13:
	mov	r2, r9
	str	r2, [sp, #4]
	str	r5, [sp, #0]
.L2:
	movs	r3, #1
	add	r9, r3
	mov	r1, r9
	cmp	r1, #66
	ble.n	.L15
.L1:
	ldr	r0, [sp, #4]
	add	sp, #12
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x0000ffff
	.4byte 0x030001d8
