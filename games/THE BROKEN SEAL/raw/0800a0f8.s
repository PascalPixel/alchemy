@ 呼出側の転送元と転送長により境界を確定した再配置アーム処理。
@ 承認済みコンパイラはサム専用のため構造化アセンブリで保持する。
.syntax unified
	.arm
	.global Func_0800a0f8
Func_0800a0f8:
	push	{r5, r6, r7, r8, r9, sl, lr}
	mov	r5, r1
	ldr	r9, [pc, #516]
	sub	sp, sp, #8
	ldr	r1, [r5]
	ldr	ip, [r5, #8]
	mov	r7, r2
	ldr	r2, [r0, #8]
	mov	r6, r3
	ldr	r3, [r0]
	rsb	r4, r2, ip
	rsb	r1, r3, r1
	asr	r0, r1, #4
	asr	r1, r4, #4
	ldr	ip, [r9, #68]
	mov	lr, pc
	bx	ip
	lsl	r0, r0, #16
	lsr	r8, r0, #16
	mov	r1, #32768
	ldr	r3, [r5]
	smull	r0, r3, r1, r3
	lsl	r3, r3, #16
	orr	r2, r3, r0, lsr #16
	str	r2, [sp, #4]
	ldr	r2, [r5, #8]
	smull	r3, r2, r1, r2
	lsl	r2, r2, #16
	orr	r1, r2, r3, lsr #16
	str	r1, [sp]
	ldr	r2, [pc, #408]
	ldr	r3, [r2]
	cmp	r8, r3
	beq	.L_0800a294
	mov	r0, r8
	str	r8, [r2]
	ldr	ip, [r9, #92]
	mov	lr, pc
	bx	ip
	mov	sl, r0
	mov	r0, r8
	ldr	ip, [r9, #100]
	mov	lr, pc
	bx	ip
	mov	r5, r0
	rsb	r9, sl, #0
	mov	r8, #159
.L_0800a1b4:
	ldm	r7, {r2, r4}
	smull	r1, r3, r5, r2
	lsl	r3, r3, #16
	orr	r3, r3, r1, lsr #16
	cmp	r3, #0
	addlt	r3, r3, #255
	asr	r3, r3, #8
	rsb	r3, r3, #0
	lsl	r1, r3, #16
	strh	r3, [r7, #16]
	asr	lr, r1, #16
	strh	r3, [r6]
	smull	r1, r2, sl, r2
	lsl	r2, r2, #16
	orr	r2, r2, r1, lsr #16
	cmp	r2, #0
	addlt	r2, r2, #255
	asr	r2, r2, #8
	strh	r2, [r7, #18]
	strh	r2, [r6, #4]
	ldr	r3, [pc, #264]
	ldr	r0, [r3]
	lsl	r3, lr, #8
	mul	lr, r0, r3
	lsl	r2, r2, #16
	asr	r2, r2, #8
	rsb	r0, r0, #0
	mul	ip, r0, r2
	smull	r3, r1, r4, r9
	lsl	r1, r1, #16
	orr	r1, r1, r3, lsr #16
	add	r1, lr, r1
	rsb	r1, r1, #0
	str	r1, [r7, #8]
	smull	r2, r3, r4, r5
	lsl	r3, r3, #16
	orr	r3, r3, r2, lsr #16
	add	ip, ip, r3
	str	ip, [r7, #12]
	ldr	r2, [sp, #4]
	add	r1, r2, r1
	asr	r1, r1, #8
	str	r1, [r6, #8]
	ldr	r1, [sp]
	subs	r8, r8, #1
	ldr	r3, [r7, #12]
	add	ip, r6, #16
	add	r3, r1, r3
	asr	r3, r3, #8
	str	r3, [r6, #12]
	add	r7, r7, #20
	ldm	r6, {r0, r1, r2, r3}
	add	r6, r6, #32
	stm	ip, {r0, r1, r2, r3}
	bpl	.L_0800a1b4
	b	.L_0800a370
.L_0800a294:
	mov	r8, #159
	ldr	lr, [pc, #120]
.L_0800a29c:
	ldr	r3, [lr]
	tst	r3, #1
	beq	.L_0800a31c
	ldr	r1, [r7, #8]
	ldr	r2, [sp, #4]
	ldrsh	r3, [r7, #18]
	add	r1, r2, r1
	cmp	r3, #0
	addlt	r3, r3, #3
	asr	r3, r3, #2
	ldrsh	r2, [r7, #16]
	add	r3, r3, r1, asr #8
	add	r2, r2, r2, lsr #31
	add	r3, r3, r2, asr #1
	str	r3, [r6, #8]
	ldrsh	r3, [r7, #16]
	ldr	r2, [sp]
	cmp	r3, #0
	addlt	r3, r3, #3
	ldr	r1, [r7, #12]
	asr	r3, r3, #2
	add	r1, r2, r1
	ldrsh	r2, [r7, #18]
	add	r3, r3, r1, asr #8
	add	r2, r2, r2, lsr #31
	add	r3, r3, r2, asr #1
	str	r3, [r6, #12]
	b	.L_0800a344
	.4byte 0x080000c0
	.4byte 0x03001f60
	.4byte 0x03001cec
	.4byte 0x03001e40
.L_0800a31c:
	ldr	r3, [r7, #8]
	ldr	r1, [sp, #4]
	add	r3, r1, r3
	asr	r3, r3, #8
	str	r3, [r6, #8]
	ldr	r3, [sp]
	ldr	r2, [r7, #12]
	add	r2, r3, r2
	asr	r2, r2, #8
	str	r2, [r6, #12]
.L_0800a344:
	ldrh	r1, [r7, #16]
	strh	r1, [r6]
	add	ip, r6, #16
	ldrh	r2, [r7, #18]
	subs	r8, r8, #1
	strh	r2, [r6, #4]
	add	r7, r7, #20
	ldm	r6, {r0, r1, r2, r3}
	add	r6, r6, #32
	stm	ip, {r0, r1, r2, r3}
	bpl	.L_0800a29c
.L_0800a370:
	add	sp, sp, #8
	pop	{r5, r6, r7, r8, r9, sl, lr}
	bx	lr
