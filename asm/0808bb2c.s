@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0808bb2c
	.thumb_func
Func_0808bb2c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r1, [pc, #240]
	movs	r2, #32
	mov	r8, r1
	negs	r2, r2
	add	r2, r8
	movs	r3, #224
	movs	r1, #226
	mov	sl, r2
	lsls	r3, r3, #4
	lsls	r1, r1, #4
	movs	r2, #228
	sub	sp, #8
	add	r3, r8
	add	r1, r8
	lsls	r2, r2, #4
	str	r3, [sp, #4]
	str	r1, [sp, #0]
	add	r2, r8
	mov	r1, sl
	mov	fp, r2
	movs	r2, #31
	negs	r2, r2
	ldrb	r7, [r1, #0]
	movs	r3, #0
	add	r2, r8
	mov	r9, r3
	mov	sl, r2
	cmp	r7, #255
	beq.n	.L0
.L3:
	adds	r0, r7, #0
	bl	Func_0808ba1c
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L1
	ldr	r6, [r5, #80]
	ldr	r3, [pc, #172]
	mov	r0, r8
	adds	r1, r5, #0
	ldr	r2, [pc, #168]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r3, [sp, #4]
	ldrb	r1, [r3, #0]
	cmp	r1, #0
	beq.n	.L2
	adds	r0, r5, #0
	bl	Func_08009080
.L2:
	ldr	r2, [sp, #0]
	adds	r0, r5, #0
	ldrb	r1, [r2, #0]
	bl	Func_080091e0
	mov	r3, fp
	ldrb	r1, [r3, #0]
	movs	r3, #3
	ldrb	r2, [r6, #9]
	ands	r1, r3
	movs	r3, #13
	negs	r3, r3
	lsls	r1, r1, #2
	ands	r3, r2
	orrs	r3, r1
	strb	r3, [r6, #9]
	ldrb	r2, [r6, #21]
	movs	r3, #13
	negs	r3, r3
	ands	r3, r2
	orrs	r3, r1
	strb	r3, [r6, #21]
	ldr	r1, [pc, #108]
	ldr	r3, [r1, #0]
	str	r6, [r5, #80]
	cmp	r7, r3
	bne.n	.L1
	ldr	r2, [pc, #104]
	movs	r1, #240
	ldr	r3, [r2, #0]
	lsls	r1, r1, #1
	adds	r3, r3, r1
	ldr	r1, [pc, #96]
	ldr	r2, [r3, #0]
	ldr	r3, [r1, #0]
	ldr	r1, [r3, #0]
	ldr	r3, [r5, #12]
	adds	r0, r5, #0
	str	r3, [r2, #20]
	str	r3, [r2, #12]
	str	r3, [r1, #4]
	bl	Func_08009140
.L1:
	movs	r2, #112
	ldr	r3, [sp, #4]
	ldr	r1, [sp, #0]
	add	r8, r2
	movs	r2, #1
	adds	r3, #1
	add	r9, r2
	str	r3, [sp, #4]
	adds	r1, #1
	mov	r3, r9
	str	r1, [sp, #0]
	add	fp, r2
	cmp	r3, #31
	bgt.n	.L0
	mov	r1, sl
	ldrb	r7, [r1, #0]
	add	sl, r2
	cmp	r7, #255
	bne.n	.L3
.L0:
	add	sp, #8
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
	.4byte 0x040000d4
	.4byte 0x8400001c
	.4byte 0x02000434
	.4byte 0x03001ebc
	.4byte 0x03001e70
