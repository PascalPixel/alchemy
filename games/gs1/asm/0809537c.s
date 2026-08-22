@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0809537c
	.thumb_func
Func_0809537c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #48
	mov	sl, r0
	bl	Func_08092054
	ldr	r3, [pc, #532]
	adds	r6, r0, #0
	movs	r0, #250
	lsls	r0, r0, #1
	adds	r3, r3, r0
	ldr	r0, [r3, #0]
	bl	Func_08092054
	movs	r1, #128
	ldrh	r3, [r0, #6]
	lsls	r1, r1, #6
	adds	r1, r1, r3
	movs	r3, #192
	lsls	r3, r3, #8
	ands	r1, r3
	mov	fp, r1
	bl	Func_080916b0
	movs	r0, #10
	bl	Func_080030f8
	movs	r0, #173
	bl	Func_080f9010
	movs	r1, #1
	mov	r0, sl
	bl	Func_080925cc
	movs	r5, #128
	movs	r0, #175
	bl	Func_080f9010
	lsls	r5, r5, #8
	movs	r1, #1
	mov	r0, sl
	bl	Func_080925cc
	add	r5, fp
	movs	r0, #20
	bl	Func_080030f8
	adds	r1, r5, #0
	movs	r2, #0
	mov	r0, sl
	bl	Func_08092adc
	movs	r0, #10
	bl	Func_080030f8
	ldr	r1, [r6, #80]
	movs	r3, #13
	ldrb	r2, [r1, #9]
	negs	r3, r3
	ands	r3, r2
	strb	r3, [r1, #9]
	strh	r5, [r6, #6]
	mov	r0, sl
	bl	Func_08092054
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #254
	ands	r3, r2
	movs	r5, #128
	adds	r2, r6, #0
	strb	r3, [r0, #0]
	adds	r2, #85
	movs	r3, #2
	lsls	r5, r5, #13
	str	r2, [sp, #4]
	adds	r1, r5, #0
	strb	r3, [r2, #0]
	adds	r0, r6, #0
	mov	r2, fp
	bl	Func_08096bec
	movs	r0, #152
	bl	Func_080f9010
	movs	r1, #4
	movs	r2, #0
	mov	r0, sl
	bl	Func_08092560
	adds	r0, r6, #0
	bl	Func_08009158
	adds	r1, r5, #0
	mov	r2, fp
	adds	r0, r6, #0
	bl	Func_08096bec
	movs	r0, #152
	bl	Func_080f9010
	movs	r1, #4
	movs	r2, #0
	mov	r0, sl
	bl	Func_08092560
	adds	r0, r6, #0
	bl	Func_08009158
	adds	r1, r5, #0
	mov	r2, fp
	adds	r0, r6, #0
	bl	Func_08096bec
	movs	r0, #152
	bl	Func_080f9010
	movs	r1, #4
	movs	r2, #0
	mov	r0, sl
	bl	Func_08092560
	adds	r0, r6, #0
	bl	Func_08009158
	movs	r0, #20
	bl	Func_080030f8
	ldr	r3, [r6, #80]
	ldr	r3, [r3, #40]
	movs	r1, #0
	ldrsh	r0, [r3, r1]
	movs	r2, #9
	mov	r8, r0
	str	r2, [sp, #8]
	cmp	r0, #90
	bne.n	.L0
	movs	r3, #2
	str	r3, [sp, #8]
.L0:
	mov	r0, r8
	cmp	r0, #92
	bne.n	.L1
	movs	r1, #10
	str	r1, [sp, #8]
.L1:
	mov	r2, r8
	cmp	r2, #91
	bne.n	.L2
	movs	r3, #9
	str	r3, [sp, #8]
.L2:
	mov	r1, sp
	movs	r0, #0
	adds	r1, #16
	str	r0, [sp, #12]
	str	r1, [sp, #0]
	mov	r9, r6
	movs	r7, #0
.L4:
	ldr	r2, [r6, #12]
	ldr	r3, [r6, #16]
	ldr	r1, [r6, #8]
	mov	r0, r8
	bl	Func_080090c8
	ldr	r2, [sp, #0]
	adds	r5, r0, #0
	lsls	r3, r7, #2
	str	r5, [r2, r3]
	cmp	r5, #0
	beq.n	.L3
	movs	r3, #240
	lsls	r3, r3, #8
	adds	r2, r5, #0
	str	r3, [r5, #28]
	str	r3, [r5, #24]
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
	subs	r2, #50
	movs	r3, #2
	strb	r3, [r2, #0]
	adds	r1, r5, #0
	adds	r1, #90
	ldrb	r3, [r1, #0]
	movs	r2, #1
	orrs	r3, r2
	strb	r3, [r1, #0]
	ldr	r3, [pc, #176]
	str	r3, [r5, #108]
	ldr	r1, [r5, #80]
	ldrh	r3, [r6, #6]
	movs	r0, #13
	strh	r3, [r5, #6]
	negs	r0, r0
	ldrb	r3, [r1, #9]
	adds	r2, r0, #0
	ands	r3, r2
	strb	r3, [r1, #9]
	ldr	r1, [sp, #8]
	adds	r0, r5, #0
	bl	Func_08009240
	adds	r0, r5, #0
	movs	r1, #0
	bl	Func_08009080
	adds	r0, r5, #0
	movs	r1, #0
	bl	Func_080091e0
	ldr	r0, [r5, #80]
	ldr	r1, [sp, #12]
	bl	Func_08096c48
	mov	r1, r9
	str	r0, [sp, #12]
	str	r1, [r5, #104]
	mov	r9, r5
.L3:
	adds	r7, #1
	cmp	r7, #7
	ble.n	.L4
	movs	r2, #128
	movs	r1, #128
	lsls	r2, r2, #8
	add	r2, fp
	lsls	r1, r1, #15
	adds	r0, r6, #0
	bl	Func_08096bec
	movs	r0, #136
	bl	Func_080f9010
	mov	r0, sl
	movs	r1, #12
	movs	r2, #0
	bl	Func_08092560
	movs	r0, #24
	bl	Func_080030f8
	ldr	r2, [sp, #4]
	movs	r3, #0
	strb	r3, [r2, #0]
	str	r3, [r6, #36]
	str	r3, [r6, #44]
	str	r3, [r6, #40]
	movs	r3, #128
	lsls	r3, r3, #24
	str	r3, [r6, #56]
	str	r3, [r6, #64]
	str	r3, [r6, #60]
	adds	r0, r6, #0
	movs	r1, #0
	bl	Func_08009080
	ldr	r1, [r6, #80]
	movs	r3, #13
	ldrb	r2, [r1, #9]
	negs	r3, r3
	ands	r3, r2
	movs	r2, #8
	orrs	r3, r2
	strb	r3, [r1, #9]
	bl	Func_08091750
	add	sp, #48
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x02000240
	.4byte 0x08095349
