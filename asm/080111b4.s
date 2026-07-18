@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080111b4
	.thumb_func
Func_080111b4:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #504]
	ldr	r1, [r3, #0]
	sub	sp, #36
	str	r1, [sp, #20]
	adds	r2, r3, #0
	subs	r2, #20
	ldr	r2, [r2, #0]
	str	r2, [sp, #16]
	subs	r3, #16
	ldr	r6, [r3, #0]
	adds	r1, #12
	ldr	r2, [r6, #0]
	ldr	r3, [sp, #16]
	mov	sl, r1
	movs	r1, #200
	lsls	r1, r1, #4
	str	r2, [sp, #12]
	adds	r1, r3, r1
	movs	r2, #210
	str	r1, [sp, #8]
	lsls	r2, r2, #2
	adds	r3, r6, r2
	ldr	r3, [r3, #0]
	movs	r1, #211
	str	r3, [sp, #4]
	lsls	r1, r1, #2
	adds	r3, r6, r1
	ldr	r3, [r3, #0]
	str	r3, [sp, #0]
	bl	Func_080114a0
	ldr	r2, [sp, #12]
	cmp	r2, #0
	bne.n	.L0
	b.n	.L1
.L0:
	ldr	r3, [r2, #8]
	mov	r8, r3
	ldr	r3, [r6, #4]
	ldr	r7, [r2, #0]
	cmp	r3, #0
	beq.n	.L2
	bl	Func_08004458
	adds	r5, r0, #0
	bl	Func_08004458
	ldr	r4, [r6, #4]
	adds	r1, r0, #0
	ldr	r3, [pc, #412]
	adds	r0, r4, #0
	subs	r1, r5, r1
	mov	ip, pc
	bx	r3
	adds	r7, r7, r0
	ldr	r1, [r6, #12]
	adds	r0, r4, #0
	movs	r0, r0
	mov	ip, pc
	bx	r3
	str	r0, [r6, #4]
.L2:
	ldr	r3, [r6, #8]
	cmp	r3, #0
	beq.n	.L3
	bl	Func_08004458
	adds	r5, r0, #0
	bl	Func_08004458
	ldr	r4, [r6, #8]
	adds	r1, r0, #0
	ldr	r3, [pc, #368]
	adds	r0, r4, #0
	subs	r1, r5, r1
	mov	ip, pc
	bx	r3
	add	r8, r0
	ldr	r1, [r6, #12]
	adds	r0, r4, #0
	movs	r0, r0
	mov	ip, pc
	bx	r3
	str	r0, [r6, #8]
.L3:
	adds	r0, r7, #0
	cmp	r7, #0
	bge.n	.L4
	ldr	r1, [pc, #340]
	adds	r0, r7, r1
.L4:
	asrs	r4, r0, #20
	mov	r0, r8
	cmp	r0, #0
	bge.n	.L5
	ldr	r0, [pc, #328]
	add	r0, r8
.L5:
	movs	r2, #228
	adds	r2, r2, r6
	ldr	r1, [r2, #0]
	mov	fp, r2
	adds	r3, r1, #0
	movs	r2, #128
	eors	r3, r7
	lsls	r2, r2, #13
	asrs	r0, r0, #20
	ands	r3, r2
	mov	r9, r0
	cmp	r3, #0
	beq.n	.L6
	cmp	r1, r7
	bge.n	.L7
	adds	r0, r4, #0
	adds	r0, #16
	bl	Func_08011164
	b.n	.L6
.L7:
	adds	r0, r4, #0
	subs	r0, #16
	bl	Func_08011164
.L6:
	adds	r5, r6, #0
	adds	r5, #232
	ldr	r1, [r5, #0]
	mov	r2, r8
	adds	r3, r1, #0
	eors	r3, r2
	movs	r2, #128
	lsls	r2, r2, #13
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L8
	cmp	r1, r8
	bge.n	.L9
	mov	r0, r9
	adds	r0, #12
	bl	Func_080110e0
	b.n	.L8
.L9:
	mov	r0, r9
	subs	r0, #18
	bl	Func_080110e0
.L8:
	mov	r3, fp
	mov	r1, r8
	str	r7, [r3, #0]
	str	r1, [r5, #0]
.L1:
	ldr	r2, [pc, #228]
	movs	r3, #120
	str	r3, [r2, #12]
	movs	r3, #96
	str	r3, [r2, #16]
	ldr	r2, [sp, #0]
	lsrs	r1, r2, #31
	adds	r1, r2, r1
	ldr	r0, [sp, #4]
	asrs	r1, r1, #1
	lsls	r2, r2, #1
	bl	Func_08005258
	ldr	r2, [sp, #12]
	ldmia	r2!, {r3}
	adds	r1, r2, #0
	str	r1, [sp, #12]
	movs	r7, #0
	mov	r1, sl
	str	r3, [r1, #0]
	str	r7, [r1, #4]
	ldr	r3, [r2, #4]
	str	r3, [r1, #8]
	bl	Func_080049ac
	mov	r0, sl
	bl	Func_08004cb4
	movs	r2, #141
	lsls	r2, r2, #1
	adds	r3, r6, r2
	ldrh	r0, [r3, #0]
	bl	Func_08004c1c
	movs	r3, #140
	lsls	r3, r3, #1
	adds	r6, r6, r3
	ldrh	r0, [r6, #0]
	bl	Func_08004bd4
	add	r0, sp, #24
	str	r7, [r0, #0]
	str	r7, [r0, #4]
	ldr	r1, [sp, #0]
	movs	r2, #128
	lsls	r2, r2, #9
	adds	r3, r1, r2
	str	r3, [r0, #8]
	ldr	r1, [sp, #20]
	ldr	r3, [pc, #136]
	bl	Func_080072f0
	bl	Func_080049ac
	mov	r1, sl
	ldr	r0, [sp, #20]
	bl	Func_080051d8
	ldr	r3, [pc, #120]
	ldrh	r0, [r6, #0]
	mov	r8, r3
	ldr	r3, [r3, #0]
	cmp	r3, r0
	beq.n	.L10
	bl	Func_0800231c
	adds	r5, r0, #0
	ldrh	r0, [r6, #0]
	bl	Func_08002322
	ldr	r3, [pc, #100]
	adds	r1, r0, #0
	adds	r0, r5, #0
	bl	Func_080072f0
	mov	r1, sl
	ldr	r2, [sp, #16]
	bl	Func_080123f4
	ldr	r3, [pc, #88]
	str	r7, [r3, #0]
	ldrh	r3, [r6, #0]
	mov	r1, r8
	str	r3, [r1, #0]
.L10:
	ldr	r3, [pc, #80]
	ldr	r2, [r3, #0]
	movs	r3, #1
	ands	r2, r3
	ldr	r1, [pc, #76]
	lsls	r3, r2, #2
	adds	r3, r3, r2
	ldr	r2, [sp, #8]
	adds	r1, #184
	lsls	r3, r3, #10
	adds	r3, r2, r3
	ldr	r4, [r1, #0]
	ldr	r2, [sp, #16]
	ldr	r0, [sp, #20]
	mov	r1, sl
	bl	Func_080072f4
	add	sp, #36
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001e80
	.4byte 0x03000118
	.4byte 0x000fffff
	.4byte 0x03001ce0
	.4byte 0x03000250
	.4byte 0x03001af4
	.4byte 0x0300013c
	.4byte 0x03001f60
	.4byte 0x03001e40
	.4byte 0x03001e50
