@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0800fb38
	.thumb_func
Func_0800fb38:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r1, #128
	lsls	r1, r1, #19
	ldrh	r2, [r1, #0]
	ldr	r3, [pc, #476]
	ands	r3, r2
	adds	r6, r0, #0
	strh	r3, [r1, #0]
	movs	r0, #0
	sub	sp, #12
	bl	Func_08003bb4
	lsls	r3, r6, #1
	ldr	r2, [pc, #464]
	adds	r3, r3, r6
	movs	r6, #202
	lsls	r6, r6, #1
	lsls	r3, r3, #2
	adds	r3, r3, r2
	adds	r1, r6, #0
	movs	r0, #8
	str	r3, [sp, #8]
	bl	Func_080048f4
	adds	r1, r6, #0
	ldr	r3, [pc, #444]
	mov	r8, r0
	bl	Func_080072f0
	ldr	r2, [sp, #8]
	ldr	r3, [pc, #436]
	ldrh	r0, [r2, #0]
	adds	r0, r0, r3
	bl	Func_08002f40
	adds	r5, r0, #0
	ldr	r3, [r5, #36]
	ldr	r1, [pc, #428]
	adds	r0, r5, r3
	bl	Func_08005340
	bl	Func_0800f9f4
	ldr	r3, [r5, #40]
	ldr	r1, [pc, #416]
	adds	r0, r5, r3
	bl	Func_08005340
	ldr	r3, [r5, #44]
	ldr	r1, [pc, #412]
	adds	r0, r5, r3
	bl	Func_08005340
	bl	Func_0800fac8
	ldr	r0, [r5, #48]
	cmp	r0, #0
	beq.n	.L0
	ldr	r6, [pc, #396]
	adds	r0, r5, r0
	adds	r1, r6, #0
	bl	Func_08005340
	adds	r0, r6, #0
	bl	Func_080118d8
.L0:
	ldr	r0, [r5, #52]
	cmp	r0, #0
	beq.n	.L1
	ldr	r6, [pc, #380]
	adds	r0, r5, r0
	adds	r1, r6, #0
	bl	Func_08005340
	adds	r0, r6, #0
	bl	Func_08011a84
.L1:
	ldr	r3, [r5, #56]
	mov	r2, r8
	adds	r3, r5, r3
	str	r3, [r2, #16]
	ldrb	r3, [r5, #0]
	adds	r2, #236
	lsls	r3, r3, #19
	str	r3, [r2, #0]
	ldrb	r3, [r5, #1]
	adds	r2, #4
	lsls	r3, r3, #19
	str	r3, [r2, #0]
	ldrb	r3, [r5, #2]
	adds	r2, #4
	lsls	r3, r3, #19
	str	r3, [r2, #0]
	ldrb	r3, [r5, #3]
	adds	r2, #4
	lsls	r3, r3, #19
	str	r3, [r2, #0]
	mov	r3, r8
	adds	r3, #228
	str	r3, [sp, #4]
	ldr	r2, [sp, #4]
	movs	r3, #0
	str	r3, [r2, #0]
	mov	r2, r8
	adds	r2, #232
	str	r2, [sp, #0]
	str	r3, [r2, #0]
	movs	r3, #128
	ldrb	r2, [r5, #4]
	lsls	r3, r3, #1
	add	r3, r8
	strb	r2, [r3, #0]
	ldr	r2, [pc, #296]
	ldrb	r3, [r5, #5]
	add	r2, r8
	strb	r3, [r2, #0]
	movs	r2, #129
	ldrb	r3, [r5, #6]
	lsls	r2, r2, #1
	add	r2, r8
	strb	r3, [r2, #0]
	movs	r7, #130
	ldr	r3, [pc, #280]
	lsls	r7, r7, #1
	adds	r6, r5, #0
	movs	r2, #2
	add	r7, r8
	adds	r6, #12
	mov	r9, r3
	mov	fp, r2
.L2:
	ldrb	r0, [r6, #0]
	ldrb	r2, [r6, #1]
	lsls	r3, r0, #19
	str	r3, [r7, #8]
	mov	lr, r3
	lsls	r3, r2, #19
	str	r3, [r7, #12]
	mov	sl, r3
	movs	r3, #4
	ldrsb	r3, [r6, r3]
	lsls	r3, r3, #12
	str	r3, [r7, #24]
	movs	r3, #5
	ldrsb	r3, [r6, r3]
	lsls	r3, r3, #12
	str	r3, [r7, #28]
	ldrb	r3, [r6, #6]
	strh	r3, [r7, #40]
	ldrb	r3, [r6, #7]
	lsrs	r2, r2, #1
	strh	r3, [r7, #42]
	lsrs	r0, r0, #1
	movs	r3, #0
	lsls	r2, r2, #7
	str	r3, [r7, #32]
	str	r3, [r7, #36]
	movs	r1, #2
	ldrsb	r1, [r6, r1]
	movs	r4, #3
	ldrsb	r4, [r6, r4]
	adds	r2, r2, r0
	ldr	r3, [pc, #188]
	lsls	r2, r2, #2
	adds	r2, r2, r3
	lsls	r1, r1, #12
	lsls	r4, r4, #12
	str	r1, [r7, #16]
	str	r4, [r7, #20]
	str	r2, [r7, #44]
	ldr	r2, [sp, #4]
	ldr	r0, [r2, #0]
	movs	r0, r0
	mov	ip, pc
	bx	r9
	add	r0, lr
	str	r0, [r7, #0]
	ldr	r3, [sp, #0]
	adds	r1, r4, #0
	ldr	r0, [r3, #0]
	movs	r0, r0
	mov	ip, pc
	bx	r9
	movs	r2, #1
	negs	r2, r2
	add	fp, r2
	add	r0, sl
	mov	r3, fp
	str	r0, [r7, #4]
	adds	r6, #8
	adds	r7, #48
	cmp	r3, #0
	bge.n	.L2
	movs	r3, #128
	lsls	r3, r3, #5
	mov	r2, r8
	movs	r1, #128
	strh	r3, [r2, #20]
	lsls	r1, r1, #1
	add	r1, r8
	ldrb	r3, [r1, #0]
	cmp	r3, #0
	beq.n	.L3
	movs	r3, #192
	lsls	r3, r3, #5
	strh	r3, [r2, #20]
.L3:
	ldr	r0, [pc, #112]
	add	r0, r8
	ldrb	r3, [r0, #0]
	cmp	r3, #0
	beq.n	.L4
	mov	r2, r8
	ldrh	r3, [r2, #20]
	ldr	r2, [pc, #52]
	orrs	r3, r2
	mov	r2, r8
	strh	r3, [r2, #20]
.L4:
	movs	r3, #129
	lsls	r3, r3, #1
	add	r3, r8
	mov	ip, r3
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L5
	mov	r2, r8
	ldrh	r3, [r2, #20]
	ldr	r2, [pc, #32]
	orrs	r3, r2
	mov	r2, r8
	strh	r3, [r2, #20]
.L5:
	ldrb	r3, [r5, #7]
	ldrb	r2, [r1, #0]
	lsls	r3, r3, #2
	orrs	r2, r3
	movs	r3, #160
	lsls	r3, r3, #3
	orrs	r2, r3
	ldr	r3, [pc, #56]
	strh	r2, [r3, #0]
	b.n	Func_0800fd5c
	movs	r0, r0
	.4byte 0x00000400
	.4byte 0x00000200
	.4byte 0x0000c1ff
	.4byte 0x08013784
	.4byte 0x03000164
	.4byte 0x00000128
	.4byte 0x02010001
	.4byte 0x0202c000
	.4byte 0x02010000
	.4byte 0x0202d000
	.4byte 0x0202de00
	.4byte 0x00000101
	.4byte 0x03000118
	.4byte 0x0400000e
