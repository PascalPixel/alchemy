@ 呼出側の転送元と転送長により境界を確定した再配置アーム処理。
@ 承認済みコンパイラはサム専用のため構造化アセンブリで保持する。
.syntax unified
	.arm
	.global Func_08009e7c
Func_08009e7c:
	push	{r5, r6, r7, r8, r9, sl, fp, lr}
	mov	r4, #10485760
	orr	r4, r4, #56
	orr	r4, r4, #-16777216
.L_08009e8c:
	ldrsh	r6, [r0, #4]
	ldr	r7, [r0, #8]
	ldr	r8, [r0, #12]
	ldrsh	r5, [r0], #32
	lsl	r7, r7, #15
	lsl	r8, r8, #15
	add	r4, r4, #503316480
.L_08009ea8:
	lsr	sl, r8, #26
	add	fp, r3, r7, lsr #26
	ldrb	ip, [fp, sl, lsl #6]!
	and	sl, r4, r8, lsr #20
	lsl	lr, r7, #6
	add	sl, sl, lr, lsr #29
	add	sl, sl, r2
	cmp	ip, #0
	ldrbne	ip, [sl, ip, lsl #6]
	cmp	ip, #0
	subeq	fp, fp, #4096
	ldrbeq	ip, [fp]
	subeq	sl, sl, #16384
	ldrbeq	ip, [sl, ip, lsl #6]
	add	r7, r7, r5, lsl #15
	add	r8, r8, r6, lsl #15
	lsr	sl, r8, #26
	add	fp, r3, r7, lsr #26
	ldrb	r9, [fp, sl, lsl #6]!
	and	sl, r4, r8, lsr #20
	lsl	lr, r7, #6
	add	sl, sl, lr, lsr #29
	add	sl, sl, r2
	cmp	r9, #0
	ldrbne	r9, [sl, r9, lsl #6]
	cmp	r9, #0
	subeq	fp, fp, #4096
	ldrbeq	r9, [fp]
	subeq	sl, sl, #16384
	ldrbeq	r9, [sl, r9, lsl #6]
	add	r7, r7, r5, lsl #15
	add	r8, r8, r6, lsl #15
	orr	r9, ip, r9, lsl #8
	strh	r9, [r1], #2
	lsr	sl, r8, #26
	add	fp, r3, r7, lsr #26
	ldrb	ip, [fp, sl, lsl #6]!
	and	sl, r4, r8, lsr #20
	lsl	lr, r7, #6
	add	sl, sl, lr, lsr #29
	add	sl, sl, r2
	cmp	ip, #0
	ldrbne	ip, [sl, ip, lsl #6]
	cmp	ip, #0
	subeq	fp, fp, #4096
	ldrbeq	ip, [fp]
	subeq	sl, sl, #16384
	ldrbeq	ip, [sl, ip, lsl #6]
	add	r7, r7, r5, lsl #15
	add	r8, r8, r6, lsl #15
	lsr	sl, r8, #26
	add	fp, r3, r7, lsr #26
	ldrb	r9, [fp, sl, lsl #6]!
	and	sl, r4, r8, lsr #20
	lsl	lr, r7, #6
	add	sl, sl, lr, lsr #29
	add	sl, sl, r2
	cmp	r9, #0
	ldrbne	r9, [sl, r9, lsl #6]
	cmp	r9, #0
	subeq	fp, fp, #4096
	ldrbeq	r9, [fp]
	subeq	sl, sl, #16384
	ldrbeq	r9, [sl, r9, lsl #6]
	add	r7, r7, r5, lsl #15
	add	r8, r8, r6, lsl #15
	orr	r9, ip, r9, lsl #8
	strh	r9, [r1], #2
	lsr	sl, r8, #26
	add	fp, r3, r7, lsr #26
	ldrb	ip, [fp, sl, lsl #6]!
	and	sl, r4, r8, lsr #20
	lsl	lr, r7, #6
	add	sl, sl, lr, lsr #29
	add	sl, sl, r2
	cmp	ip, #0
	ldrbne	ip, [sl, ip, lsl #6]
	cmp	ip, #0
	subeq	fp, fp, #4096
	ldrbeq	ip, [fp]
	subeq	sl, sl, #16384
	ldrbeq	ip, [sl, ip, lsl #6]
	add	r7, r7, r5, lsl #15
	add	r8, r8, r6, lsl #15
	lsr	sl, r8, #26
	add	fp, r3, r7, lsr #26
	ldrb	r9, [fp, sl, lsl #6]!
	and	sl, r4, r8, lsr #20
	lsl	lr, r7, #6
	add	sl, sl, lr, lsr #29
	add	sl, sl, r2
	cmp	r9, #0
	ldrbne	r9, [sl, r9, lsl #6]
	cmp	r9, #0
	subeq	fp, fp, #4096
	ldrbeq	r9, [fp]
	subeq	sl, sl, #16384
	ldrbeq	r9, [sl, r9, lsl #6]
	add	r7, r7, r5, lsl #15
	add	r8, r8, r6, lsl #15
	orr	r9, ip, r9, lsl #8
	strh	r9, [r1], #2
	lsr	sl, r8, #26
	add	fp, r3, r7, lsr #26
	ldrb	ip, [fp, sl, lsl #6]!
	and	sl, r4, r8, lsr #20
	lsl	lr, r7, #6
	add	sl, sl, lr, lsr #29
	add	sl, sl, r2
	cmp	ip, #0
	ldrbne	ip, [sl, ip, lsl #6]
	cmp	ip, #0
	subeq	fp, fp, #4096
	ldrbeq	ip, [fp]
	subeq	sl, sl, #16384
	ldrbeq	ip, [sl, ip, lsl #6]
	add	r7, r7, r5, lsl #15
	add	r8, r8, r6, lsl #15
	lsr	sl, r8, #26
	add	fp, r3, r7, lsr #26
	ldrb	r9, [fp, sl, lsl #6]!
	and	sl, r4, r8, lsr #20
	lsl	lr, r7, #6
	add	sl, sl, lr, lsr #29
	add	sl, sl, r2
	cmp	r9, #0
	ldrbne	r9, [sl, r9, lsl #6]
	cmp	r9, #0
	subeq	fp, fp, #4096
	ldrbeq	r9, [fp]
	subeq	sl, sl, #16384
	ldrbeq	r9, [sl, r9, lsl #6]
	add	r7, r7, r5, lsl #15
	add	r8, r8, r6, lsl #15
	orr	r9, ip, r9, lsl #8
	strh	r9, [r1], #2
	add	r1, r1, #56
	subs	r4, r4, #16777216
	bpl	.L_08009ea8
	sub	r1, r1, #1920
	add	r1, r1, #8
	sub	r4, r4, #65536
	tst	r4, #458752
	addeq	r1, r1, #1856
	tst	r4, #16711680
	bne	.L_08009e8c
	pop	{r5, r6, r7, r8, r9, sl, fp, lr}
	bx	lr
