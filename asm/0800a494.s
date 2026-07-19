@ 呼出側の転送元と転送長により境界を確定した再配置アーム処理。
@ 承認済みコンパイラはサム専用のため構造化アセンブリで保持する。
.syntax unified
	.arm
	.global Func_0800a494
Func_0800a494:
	push	{r5, r6, r7, r8, r9, sl, lr}
	mov	r2, #0
	ldr	r3, [pc, #928]
	sub	sp, sp, #16
	ldr	r5, [r3]
.L_0800a4a8:
	ldr	r3, [r5]
	add	r2, r2, #1
	str	r2, [sp]
	add	r1, r5, #112
	str	r1, [sp, #4]
	cmp	r3, #0
	beq	.L_0800a940
	ldr	r3, [r5, #108]
	cmp	r3, #0
	beq	.L_0800a4dc
	mov	r0, r5
	mov	lr, pc
	bx	r3
.L_0800a4dc:
	ldr	r3, [r5]
	cmp	r3, #0
	beq	.L_0800a940
	ldrb	r3, [r5, #91]
	cmp	r3, #0
	bne	.L_0800a940
	ldrsh	r3, [r5, #94]
	cmp	r3, #0
	ldrh	r3, [r5, #94]
	subne	r3, r3, #1
	strhne	r3, [r5, #94]
	bne	.L_0800a558
	ldr	r6, [pc, #820]
	b	.L_0800a520
.L_0800a514:
	ldrh	r3, [r5, #4]
	add	r3, r3, #1
	strh	r3, [r5, #4]
.L_0800a520:
	ldrsh	r2, [r5, #4]
	ldr	r3, [r5]
	ldr	r3, [r3, r2, lsl #2]
	cmp	r3, #63
	bhi	.L_0800a514
	mov	r0, r5
	ldr	ip, [r6, r3, lsl #2]
	mov	lr, pc
	bx	ip
	cmp	r0, #0
	bne	.L_0800a520
	ldr	r3, [r5]
	cmp	r3, #0
	beq	.L_0800a940
.L_0800a558:
	ldr	r3, [r5, #12]
	ldr	r9, [r5, #8]
	str	r3, [sp, #12]
	ldrb	r3, [r5, #97]
	cmp	r3, #0
	ldr	sl, [r5, #16]
	bne	.L_0800a7ac
	ldr	r2, [r5, #56]
	str	r3, [sp, #8]
	cmp	r2, #-2147483648
	beq	.L_0800a6c0
	rsb	r2, r9, r2
	add	r3, r2, #65280
	add	r3, r3, #255
	cmp	r2, #0
	ldr	r1, [r5, #64]
	asrge	r6, r2, #16
	asrlt	r6, r3, #16
	rsb	r1, sl, r1
	add	r3, r1, #65280
	add	r3, r3, #255
	cmp	r1, #0
	asrge	r7, r1, #16
	asrlt	r7, r3, #16
	mul	r2, r7, r7
	mla	r0, r6, r6, r2
	ldr	r1, [pc, #644]
	mov	lr, pc
	bx	r1
	lsl	r0, r0, #16
	cmp	r0, #16777216
	bge	.L_0800a614
	ldr	r3, [r5, #56]
	ldr	r2, [r5, #64]
	rsb	r6, r9, r3
	rsb	r7, sl, r2
	smull	r1, r3, r6, r6
	lsl	r3, r3, #16
	orr	r3, r3, r1, lsr #16
	smull	r2, r0, r7, r7
	lsl	r0, r0, #16
	orr	r0, r0, r2, lsr #16
	add	r0, r3, r0
	ldr	r3, [pc, #576]
	mov	lr, pc
	bx	r3
	lsl	r0, r0, #8
.L_0800a614:
	cmp	r0, #0
	ldreq	r9, [r5, #56]
	ldreq	sl, [r5, #64]
	beq	.L_0800a73c
	ldr	r1, [r5, #52]
	ldr	r3, [pc, #544]
	mov	lr, pc
	bx	r3
	smull	r1, r3, r0, r6
	lsl	r3, r3, #16
	orr	r3, r3, r1, lsr #16
	ldr	r2, [r5, #36]
	add	r8, r2, r3
	str	r8, [r5, #36]
	smull	r1, r3, r0, r7
	lsl	r3, r3, #16
	orr	r3, r3, r1, lsr #16
	ldr	r2, [r5, #44]
	add	r6, r2, r3
	str	r6, [r5, #44]
	smull	r2, r3, r8, r8
	lsl	r3, r3, #16
	orr	r3, r3, r2, lsr #16
	smull	r1, r0, r6, r6
	lsl	r0, r0, #16
	orr	r0, r0, r1, lsr #16
	ldr	r1, [pc, #456]
	add	r0, r3, r0
	mov	lr, pc
	bx	r1
	ldr	r1, [r5, #48]
	lsl	r0, r0, #8
	cmp	r0, r1
	ble	.L_0800a73c
	ldr	r3, [pc, #428]
	mov	lr, pc
	bx	r3
	smull	r2, r3, r0, r8
	lsl	r3, r3, #16
	orr	r3, r3, r2, lsr #16
	str	r3, [r5, #36]
	smull	r1, r2, r0, r6
	b	.L_0800a720
.L_0800a6c0:
	ldr	r6, [r5, #36]
	ldr	r7, [r5, #44]
	smull	r2, r3, r6, r6
	lsl	r3, r3, #16
	orr	r3, r3, r2, lsr #16
	smull	r1, r0, r7, r7
	lsl	r0, r0, #16
	orr	r0, r0, r1, lsr #16
	add	r0, r3, r0
	bl	.L_0800a960
	lsls	r0, r0, #8
	beq	.L_0800a730
	ldr	r3, [r5, #52]
	subs	r1, r0, r3
	ldrmi	r1, [sp, #8]
	strmi	r1, [r5, #36]
	strmi	r1, [r5, #44]
	bmi	.L_0800a73c
	bl	.L_0800a968
	smull	r2, r3, r0, r6
	lsl	r3, r3, #16
	orr	r3, r3, r2, lsr #16
	str	r3, [r5, #36]
	smull	r1, r2, r0, r7
.L_0800a720:
	lsl	r2, r2, #16
	orr	r2, r2, r1, lsr #16
	str	r2, [r5, #44]
	b	.L_0800a73c
.L_0800a730:
	ldr	r3, [sp, #8]
	str	r3, [r5, #36]
	str	r3, [r5, #44]
.L_0800a73c:
	ldrb	r3, [r5, #85]
	tst	r3, #2
	beq	.L_0800a7ac
	ldr	r1, [r5, #20]
	ldr	r3, [sp, #12]
	cmp	r3, r1
	ble	.L_0800a76c
	ldr	r3, [r5, #40]
	ldr	r2, [r5, #72]
	rsb	r3, r2, r3
	str	r3, [r5, #40]
	bgt	.L_0800a7ac
.L_0800a76c:
	ldr	r2, [r5, #40]
	cmp	r2, #0
	bge	.L_0800a7ac
	str	r1, [sp, #12]
	ldr	r3, [r5, #68]
	smull	r2, r3, r3, r2
	lsl	r3, r3, #16
	orr	r3, r3, r2, lsr #16
	rsb	r3, r3, #0
	str	r3, [r5, #40]
	ldr	r2, [r5, #72]
	cmp	r3, #0
	rsblt	r3, r3, #0
	cmp	r3, r2
	movle	r3, #0
	strle	r3, [r5, #40]
.L_0800a7ac:
	ldr	r3, [r5, #36]
	ldr	r2, [r5, #40]
	add	r9, r9, r3
	ldr	r3, [sp, #12]
	ldr	r1, [r5, #44]
	add	r3, r3, r2
	str	r3, [sp, #12]
	ldrb	r3, [r5, #86]
	add	sl, sl, r1
	cmp	r3, #17
	beq	.L_0800a810
	bgt	.L_0800a7e8
	cmp	r3, #16
	beq	.L_0800a7f4
	b	.L_0800a87c
.L_0800a7e8:
	cmp	r3, #18
	beq	.L_0800a854
	b	.L_0800a87c
.L_0800a7f4:
	ldr	r2, [r5, #56]
	cmp	r9, r2
	beq	.L_0800a874
	ldr	r3, [r5, #8]
	rsb	r3, r2, r3
	rsb	r2, r2, r9
	b	.L_0800a86c
.L_0800a810:
	ldr	r2, [r5, #60]
	ldr	r3, [sp, #12]
	cmp	r3, r2
	beq	.L_0800a838
	ldr	r3, [r5, #12]
	ldr	r1, [sp, #12]
	rsb	r3, r2, r3
	rsb	r2, r2, r1
	teq	r3, r2
	bpl	.L_0800a87c
.L_0800a838:
	mov	r3, #1
	str	r3, [sp, #8]
	b	.L_0800a87c
	.4byte 0x03001e64
	.4byte 0x08013624
	.4byte 0x030001d8
	.4byte 0x0300013c
.L_0800a854:
	ldr	r2, [r5, #64]
	cmp	sl, r2
	beq	.L_0800a874
	ldr	r3, [r5, #16]
	rsb	r3, r2, r3
	rsb	r2, r2, sl
.L_0800a86c:
	teq	r3, r2
	bpl	.L_0800a87c
.L_0800a874:
	mov	r1, #1
	str	r1, [sp, #8]
.L_0800a87c:
	ldr	r3, [sp, #8]
	cmp	r3, #0
	beq	.L_0800a8d4
	ldrb	r3, [r5, #88]
	cmp	r3, #0
	beq	.L_0800a8bc
	ldr	r9, [r5, #56]
	mov	r3, #0
	str	r3, [r5, #36]
	ldr	sl, [r5, #64]
	ldrb	r2, [r5, #85]
	str	r3, [r5, #44]
	cmp	r2, r3
	ldreq	r1, [r5, #60]
	streq	r1, [sp, #12]
	streq	r2, [r5, #40]
.L_0800a8bc:
	mov	r3, #-2147483648
	str	r3, [r5, #56]
	str	r3, [r5, #60]
	str	r3, [r5, #64]
	mov	r2, #0
	strb	r2, [r5, #86]
.L_0800a8d4:
	str	r9, [r5, #8]
	ldr	r3, [sp, #12]
	str	r3, [r5, #12]
	ldrb	r3, [r5, #90]
	str	sl, [r5, #16]
	tst	r3, #1
	beq	.L_0800a940
	ldr	r9, [r5, #36]
	ldr	sl, [r5, #44]
	cmp	sl, #0
	cmpeq	r9, #0
	beq	.L_0800a940
	mov	r0, sl
	mov	r1, r9
	bl	.L_0800a958
	lsl	r0, r0, #16
	ldrh	r3, [r5, #6]
	mov	r2, #-2147483648
	rsb	r0, r3, r0, lsr #16
	lsl	r0, r0, #16
	asr	r0, r0, #16
	cmp	r0, #4096
	movge	r0, #4096
	cmn	r0, #4096
	asrlt	r0, r2, #19
	add	r3, r3, r0
	strh	r3, [r5, #6]
.L_0800a940:
	ldm	sp, {r2, r5}
	cmp	r2, #7
	ble	.L_0800a4a8
	add	sp, sp, #16
	pop	{r5, r6, r7, r8, r9, sl, lr}
	bx	lr
.L_0800a958:
	ldr	r4, [pc, #16]
	bx	r4
.L_0800a960:
	ldr	r4, [pc, #12]
	bx	r4
.L_0800a968:
	ldr	r4, [pc, #8]
	bx	r4
	.4byte 0x080044d1
	.4byte 0x030001d8
	.4byte 0x0300013c
