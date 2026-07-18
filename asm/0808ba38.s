@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0808ba38
	.thumb_func
Func_0808ba38:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r7, [pc, #212]
	movs	r2, #224
	lsls	r2, r2, #4
	adds	r2, r2, r7
	mov	fp, r2
	movs	r3, #226
	movs	r2, #228
	lsls	r3, r3, #4
	lsls	r2, r2, #4
	adds	r3, r3, r7
	adds	r2, r2, r7
	sub	sp, #4
	mov	r9, r3
	mov	sl, r2
	movs	r3, #0
	movs	r2, #66
	str	r2, [sp, #0]
	mov	r8, r3
	ldr	r3, [pc, #180]
	movs	r2, #207
	ldr	r3, [r3, #0]
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	adds	r6, r7, #0
	subs	r6, #32
	cmp	r3, #3
	bne.n	.L0
	movs	r3, #8
	str	r3, [sp, #0]
.L0:
	ldr	r2, [sp, #0]
	movs	r5, #0
	cmp	r5, r2
	bge.n	.L1
.L5:
	adds	r0, r5, #0
	bl	Func_0808ba1c
	adds	r4, r0, #0
	cmp	r4, #0
	beq.n	.L2
	strb	r5, [r6, #0]
	ldr	r3, [pc, #136]
	adds	r6, #1
	adds	r1, r7, #0
	ldr	r2, [pc, #132]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r3, r4, #0
	adds	r3, #84
	ldrb	r3, [r3, #0]
	cmp	r3, #1
	bne.n	.L3
	ldr	r2, [r4, #80]
	adds	r3, r2, #0
	adds	r3, #36
	ldrb	r4, [r3, #0]
	adds	r3, #2
	ldrb	r1, [r3, #0]
	ldrb	r3, [r2, #9]
	lsls	r3, r3, #28
	lsrs	r0, r3, #30
	b.n	.L4
.L3:
	movs	r4, #0
	movs	r1, #0
	movs	r0, #0
.L4:
	mov	r3, fp
	strb	r4, [r3, #0]
	mov	r3, r9
	movs	r2, #1
	strb	r1, [r3, #0]
	movs	r3, #1
	add	fp, r2
	add	r9, r2
	add	r8, r3
	mov	r2, sl
	strb	r0, [r2, #0]
	mov	r2, r8
	add	sl, r3
	adds	r7, #112
	cmp	r2, #31
	bhi.n	.L1
.L2:
	ldr	r3, [sp, #0]
	adds	r5, #1
	cmp	r5, r3
	blt.n	.L5
.L1:
	mov	r5, r8
	cmp	r5, #31
	bgt.n	.L6
	movs	r3, #32
	movs	r2, #255
	subs	r5, r3, r5
.L7:
	subs	r5, #1
	strb	r2, [r6, #0]
	adds	r6, #1
	cmp	r5, #0
	bne.n	.L7
.L6:
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x02001124
	.4byte 0x03001ebc
	.4byte 0x040000d4
	.4byte 0x8400001c
