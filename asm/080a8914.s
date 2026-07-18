@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080a8914
	.thumb_func
Func_080a8914:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #460]
	adds	r6, r0, #0
	adds	r0, r1, #0
	mov	r8, r2
	ldr	r5, [r3, #0]
	sub	sp, #4
	bl	Func_08077008
	movs	r2, #190
	lsls	r2, r2, #1
	adds	r5, r5, r2
	ldr	r2, [r5, #0]
	movs	r3, #1
	strb	r3, [r2, #5]
	mov	r2, r8
	adds	r3, #255
	ands	r2, r3
	adds	r7, r0, #0
	mov	r8, r2
	cmp	r2, #0
	bne.n	.L0
	movs	r3, #40
	str	r3, [sp, #0]
	adds	r0, r6, #0
	movs	r1, #0
	movs	r2, #0
	movs	r3, #128
	bl	Func_08015068
.L0:
	adds	r0, r7, #0
	adds	r1, r6, #0
	movs	r2, #40
	movs	r3, #0
	bl	Func_08015090
	ldr	r2, [pc, #392]
	adds	r3, r7, r2
	ldrb	r0, [r3, #0]
	ldr	r3, [pc, #392]
	adds	r1, r6, #0
	adds	r0, r0, r3
	movs	r2, #0
	movs	r3, #32
	bl	Func_08015080
	adds	r1, r6, #0
	movs	r2, #104
	movs	r3, #0
	ldr	r0, [pc, #376]
	bl	Func_08015090
	movs	r0, #15
	bl	Func_080150b8
	movs	r3, #0
	ldrb	r0, [r7, #15]
	movs	r1, #2
	str	r3, [sp, #0]
	adds	r2, r6, #0
	movs	r3, #128
	bl	Func_080150b0
	movs	r2, #40
	ldr	r0, [pc, #348]
	adds	r1, r6, #0
	movs	r3, #16
	bl	Func_08015090
	movs	r3, #16
	movs	r2, #56
	ldrsh	r0, [r7, r2]
	mov	r9, r3
	str	r3, [sp, #0]
	adds	r2, r6, #0
	movs	r1, #4
	movs	r3, #72
	bl	Func_080150b0
	mov	r3, r9
	movs	r2, #52
	ldrsh	r0, [r7, r2]
	movs	r1, #4
	str	r3, [sp, #0]
	adds	r2, r6, #0
	movs	r3, #112
	bl	Func_080150b0
	ldr	r5, [pc, #304]
	adds	r1, r6, #0
	adds	r0, r5, #0
	movs	r2, #104
	movs	r3, #16
	bl	Func_08015098
	movs	r2, #40
	ldr	r0, [pc, #292]
	adds	r1, r6, #0
	movs	r3, #24
	bl	Func_08015090
	movs	r3, #24
	movs	r2, #58
	ldrsh	r0, [r7, r2]
	mov	sl, r3
	str	r3, [sp, #0]
	adds	r2, r6, #0
	movs	r1, #4
	movs	r3, #72
	bl	Func_080150b0
	mov	r3, sl
	movs	r2, #54
	ldrsh	r0, [r7, r2]
	movs	r1, #4
	str	r3, [sp, #0]
	adds	r2, r6, #0
	movs	r3, #112
	bl	Func_080150b0
	adds	r0, r5, #0
	adds	r1, r6, #0
	movs	r2, #104
	movs	r3, #24
	bl	Func_08015098
	ldr	r5, [pc, #236]
	adds	r1, r6, #0
	adds	r0, r5, #0
	movs	r2, #40
	movs	r3, #8
	bl	Func_08015080
	movs	r2, #146
	lsls	r2, r2, #1
	adds	r3, r7, r2
	ldr	r0, [r3, #0]
	movs	r3, #8
	str	r3, [sp, #0]
	adds	r2, r6, #0
	mov	fp, r3
	movs	r1, #7
	movs	r3, #88
	bl	Func_080150b0
	mov	r2, r8
	cmp	r2, #0
	bne.n	.L1
	movs	r0, #1
	bl	Func_080030f8
	movs	r3, #40
	str	r3, [sp, #0]
	adds	r0, r6, #0
	movs	r1, #144
	movs	r2, #0
	movs	r3, #224
	bl	Func_08015068
.L1:
	adds	r0, r5, #0
	adds	r1, r6, #0
	subs	r0, #23
	movs	r2, #152
	movs	r3, #0
	bl	Func_08015080
	adds	r0, r5, #0
	adds	r1, r6, #0
	subs	r0, #22
	movs	r2, #152
	movs	r3, #8
	bl	Func_08015080
	adds	r0, r5, #0
	adds	r1, r6, #0
	subs	r0, #21
	movs	r2, #152
	movs	r3, #16
	bl	Func_08015080
	adds	r0, r5, #0
	adds	r1, r6, #0
	subs	r0, #20
	movs	r2, #152
	movs	r3, #24
	bl	Func_08015080
	movs	r3, #0
	ldrh	r0, [r7, #60]
	adds	r2, r6, #0
	str	r3, [sp, #0]
	movs	r1, #3
	movs	r3, #200
	bl	Func_080150b0
	mov	r2, fp
	ldrh	r0, [r7, #62]
	movs	r1, #3
	str	r2, [sp, #0]
	movs	r3, #200
	adds	r2, r6, #0
	bl	Func_080150b0
	adds	r3, r7, #0
	adds	r3, #64
	ldrh	r0, [r3, #0]
	mov	r3, r9
	str	r3, [sp, #0]
	adds	r2, r6, #0
	movs	r1, #3
	movs	r3, #200
	bl	Func_080150b0
	adds	r3, r7, #0
	mov	r2, sl
	adds	r3, #66
	ldrb	r0, [r3, #0]
	movs	r1, #3
	str	r2, [sp, #0]
	movs	r3, #200
	adds	r2, r6, #0
	bl	Func_080150b0
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001f2c
	.4byte 0x00000129
	.4byte 0x00000741
	.4byte 0x080af22c
	.4byte 0x080af234
	.4byte 0x080af230
	.4byte 0x080af238
	.4byte 0x00000b0e
