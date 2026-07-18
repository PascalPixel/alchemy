@ 圧縮形式二の展開処理。転送後のアーム領域で実行する。
@ 呼出側の転送元と転送長により境界を確定した再構築コード。
.syntax unified
	.arm
	.global Func_08002808
Func_08002808:
	ldrb	r2, [r0], #1
	cmp	r2, #2
	movne	r0, #4
	bxne	lr
	push	{r5, r6, r7, r8, r9, sl, fp, lr}
	add	ip, pc, #36
	ldm	ip, {sl, fp}
	mov	ip, #15
	sub	r1, r1, #1
	add	r9, r1, #33
	ands	r2, r0, #1
	lsl	r4, r2, #3
	ldrbne	r2, [r0], #1
	subs	r4, r4, #16
	blmi	.L_08002ae8
	b	.L_08002910
@ ニブル選択に用いる並べ替え定数。
.L_08002848:
	.4byte 0xfedcba98
	.4byte 0x76543210
.L_08002850:
	and	r8, r2, #15
	add	r9, pc, #684
	mov	lr, pc
	add	pc, r9, r8, lsl #5
	mov	r5, r8
	and	r8, r2, #240
	add	r9, pc, #664
	mov	lr, pc
	add	pc, r9, r8, lsl #1
	orr	r5, r5, r8, lsl #4
	lsr	r2, r2, #8
	subs	r4, r4, #11
	blmi	.L_08002ae8
	b	.L_08002908
.L_08002888:
	lsrs	r2, r2, #1
	bcc	.L_08002850
	and	r8, r2, #7
	add	r9, pc, #620
	mov	lr, pc
	add	pc, r9, r8, lsl #5
	mov	r5, r8
	and	r8, r2, #56
	add	r9, pc, #600
	mov	lr, pc
	add	pc, r9, r8, lsl #2
	orr	r5, r5, r8, lsl #4
	lsr	r2, r2, #6
	subs	r4, r4, #9
	blmi	.L_08002ae8
	b	.L_08002908
.L_080028c8:
	push	{r5, r9}
	lsrs	r2, r2, #1
	bcc	.L_08002888
	and	r8, r2, #3
	add	r9, pc, #552
	mov	lr, pc
	add	pc, r9, r8, lsl #5
	mov	r5, r8
	and	r8, r2, #12
	add	r9, pc, #532
	mov	lr, pc
	add	pc, r9, r8, lsl #3
	orr	r5, r5, r8, lsl #4
	lsr	r2, r2, #4
	subs	r4, r4, #6
	blmi	.L_08002ae8
.L_08002908:
	strb	r5, [r1, #1]!
	pop	{r5, r9}
.L_08002910:
	lsrs	r2, r2, #1
	bcs	.L_080028c8
	lsrs	r2, r2, #1
	bcc	.L_080029cc
	lsrs	r2, r2, #1
	bcc	.L_08002990
	lsrs	r2, r2, #1
	bcc	.L_0800299c
	lsrs	r2, r2, #1
	bcc	.L_080029ac
	lsrs	r2, r2, #1
	bcs	.L_08002958
	lsrs	r2, r2, #1
	bcc	.L_080029bc
	subs	r4, r4, #7
	mov	r5, #6
	bpl	.L_080029d4
	blmi	.L_08002ae4
.L_08002958:
	ands	r5, r2, #3
	lsr	r2, r2, #2
	beq	.L_08002974
	add	r5, r5, #6
	subs	r4, r4, #8
	bpl	.L_080029d4
	blmi	.L_08002ae4
.L_08002974:
	ands	r5, r2, #127
	beq	.L_08002af8
	add	r5, r5, #9
	lsr	r2, r2, #7
	subs	r4, r4, #15
	bpl	.L_080029d4
	blmi	.L_08002ae4
.L_08002990:
	subs	r4, r4, #3
	mov	r5, #2
	b	.L_080029d4
.L_0800299c:
	subs	r4, r4, #4
	mov	r5, #3
	bpl	.L_080029d4
	blmi	.L_08002ae4
.L_080029ac:
	subs	r4, r4, #5
	mov	r5, #4
	bpl	.L_080029d4
	blmi	.L_08002ae4
.L_080029bc:
	subs	r4, r4, #7
	mov	r5, #5
	bpl	.L_080029d4
	blmi	.L_08002ae4
.L_080029cc:
	subs	r4, r4, #2
	mov	r5, #1
.L_080029d4:
	lsrs	r2, r2, #1
	bcc	.L_08002a18
	and	r6, r2, #31
	subs	r4, r4, #6
	lsr	r2, r2, #5
.L_080029e8:
	blmi	.L_08002ae8
	ldrb	r7, [r1, -r6]
	strb	r7, [r1, #1]!
.L_080029f4:
	ldrb	r7, [r1, -r6]
	strb	r7, [r1, #1]!
	subs	r5, r5, #1
	beq	.L_08002910
	ldrb	r7, [r1, -r6]
	strb	r7, [r1, #1]!
	subs	r5, r5, #1
	bne	.L_080029f4
	b	.L_08002910
.L_08002a18:
	sub	r6, r1, r9
	cmp	r6, #2048
	bcc	.L_08002a3c
	ror	r6, r2, #12
	lsr	r2, r2, #12
	lsr	r6, r6, #20
	add	r6, r6, #32
	subs	r4, r4, #13
	b	.L_080029e8
.L_08002a3c:
	mov	r7, #11
	lsls	r6, r6, #22
	bcs	.L_08002ac4
	sub	r7, r7, #1
	lsls	r6, r6, #1
	bcs	.L_08002ac4
	sub	r7, r7, #1
	lsls	r6, r6, #1
	bcs	.L_08002ac4
	sub	r7, r7, #1
	lsls	r6, r6, #1
	bcs	.L_08002ac4
	sub	r7, r7, #1
	lsls	r6, r6, #1
	bcs	.L_08002ac4
	sub	r7, r7, #1
	lsls	r6, r6, #1
	bcs	.L_08002ac4
	sub	r7, r7, #1
	lsls	r6, r6, #1
	bcs	.L_08002ac4
	sub	r7, r7, #1
	lsls	r6, r6, #1
	bcs	.L_08002ac4
	sub	r7, r7, #1
	lsls	r6, r6, #1
	bcs	.L_08002ac4
	sub	r7, r7, #1
	lsls	r6, r6, #1
	bcs	.L_08002ac4
	sub	r7, r7, #1
	lsls	r6, r6, #1
	bcs	.L_08002ac4
	sub	r7, r7, #1
.L_08002ac4:
	mov	r6, #1
	rsb	r6, r6, r6, lsl r7
	and	r6, r2, r6
	add	r6, r6, #32
	lsr	r2, r2, r7
	sub	r4, r4, #1
	subs	r4, r4, r7
	b	.L_080029e8
.L_08002ae4:
	sub	lr, pc, #280
.L_08002ae8:
	ldrh	r3, [r0], #2
	add	r4, r4, #16
	orr	r2, r2, r3, lsl r4
	bx	lr
.L_08002af8:
	sub	r9, r9, #33
	sub	r0, r1, r9
	pop	{r5, r6, r7, r8, r9, sl, fp, lr}
	bx	lr
@ ニブル位置ごとの合成処理。各入口は三十二バイト間隔。
.L_08002b08:
	and	r8, ip, fp
	bx	lr
	nop
	nop
	nop
	nop
	nop
	nop
.L_08002b28:
	lsr	r9, fp, #8
	orr	r9, r9, fp, lsl #28
	and	r8, ip, fp, lsr #4
	orr	fp, r8, r9, ror #24
	bx	lr
	nop
	nop
	nop
.L_08002b48:
	lsr	r9, fp, #12
	orr	r9, r9, fp, lsl #24
	and	r8, ip, fp, lsr #8
	orr	fp, r8, r9, ror #20
	bx	lr
	nop
	nop
	nop
.L_08002b68:
	lsr	r9, fp, #16
	orr	r9, r9, fp, lsl #20
	and	r8, ip, fp, lsr #12
	orr	fp, r8, r9, ror #16
	bx	lr
	nop
	nop
	nop
.L_08002b88:
	lsr	r9, fp, #20
	orr	r9, r9, fp, lsl #16
	and	r8, ip, fp, lsr #16
	orr	fp, r8, r9, ror #12
	bx	lr
	nop
	nop
	nop
.L_08002ba8:
	lsr	r9, fp, #24
	orr	r9, r9, fp, lsl #12
	and	r8, ip, fp, lsr #20
	orr	fp, r8, r9, ror #8
	bx	lr
	nop
	nop
	nop
.L_08002bc8:
	lsr	r9, fp, #28
	orr	r9, r9, fp, lsl #8
	and	r8, ip, fp, lsr #24
	orr	fp, r8, r9, ror #4
	bx	lr
	nop
	nop
	nop
.L_08002be8:
	ror	fp, fp, #28
	and	r8, ip, fp
	bx	lr
	nop
	nop
	nop
	nop
	nop
.L_08002c08:
	and	r8, ip, sl
	bic	r9, sl, #15
.L_08002c10:
	orr	sl, r9, fp, lsr #28
	orr	fp, r8, fp, lsl #4
	bx	lr
	nop
	nop
	nop
.L_08002c28:
	and	r8, ip, sl, lsr #4
	lsr	r9, sl, #8
	orr	r9, r9, sl, lsl #28
	ror	r9, r9, #24
	b	.L_08002c10
	nop
	nop
	nop
.L_08002c48:
	and	r8, ip, sl, lsr #8
	lsr	r9, sl, #12
	orr	r9, r9, sl, lsl #24
	ror	r9, r9, #20
	b	.L_08002c10
	nop
	nop
	nop
.L_08002c68:
	and	r8, ip, sl, lsr #12
	lsr	r9, sl, #16
	orr	r9, r9, sl, lsl #20
	ror	r9, r9, #16
	b	.L_08002c10
	nop
	nop
	nop
.L_08002c88:
	and	r8, ip, sl, lsr #16
	lsr	r9, sl, #20
	orr	r9, r9, sl, lsl #16
	ror	r9, r9, #12
	b	.L_08002c10
	nop
	nop
	nop
.L_08002ca8:
	and	r8, ip, sl, lsr #20
	lsr	r9, sl, #24
	orr	r9, r9, sl, lsl #12
	ror	r9, r9, #8
	b	.L_08002c10
	nop
	nop
	nop
.L_08002cc8:
	and	r8, ip, sl, lsr #24
	lsr	r9, sl, #28
	orr	r9, r9, sl, lsl #8
	ror	r9, r9, #4
	b	.L_08002c10
	nop
	nop
	nop
.L_08002ce8:
	and	r8, ip, sl, lsr #28
	lsl	r9, sl, #4
	b	.L_08002c10
