@ Why IwramMulQ16ReturnIp is assembly:
@ Unpaired live smull halves r2/r0; add ip,#1 and bx ip return through a
@ non-procedure-standard register.
@ Why IwramMulQ16 is assembly:
@ smull r2, r0, r1, r0 puts the low half in r2 and the high half in r0.
@ mulsidi3 writes a DI register pair, RdHi = RdLo + 1, earlyclobbered and so
@ apart from both inputs; here RdHi is r0, not r3, and is also an input. So
@ this multiply did not come from mulsidi3. smulsi3_highpart can emit this
@ same smull, but its low half is a dead scratch, and orr r0, r0, r2, lsr #16
@ reads it.
@ This proves a hand-written multiply, not by itself a hand-written
@ file. The routine has no other tell: its four instructions are a frameless
@ leaf that reuses the argument registers, as compiled code also may.
@ Why IwramRatioMulQ14 is assembly:
@ Both smull halves are consumed from nonconsecutive r3/r0; compiler DI
@ multiply patterns require consecutive halves, highpart patterns discard low
@ half.
@ Why IwramClearWords to IwramFillWords are assembly:
@ push saves r5-r9 without lr and computed add pc enters the unrolled store
@ body.
@ Why IwramSqrt is assembly:
@ movs r1, r0 copies and tests without a shift. The compilers' copy-and-test
@ (*movsi_compare0) is subs r1, r0, #0; a register-to-register movs comes only
@ with a shift.
@ Secondary: the sign exit that follows is bxmi lr. *movsi_compare0 sets plain
@ CC, where a signed < 0 test is lt; mi is the CC_NOOV spelling, which a
@ copy-and-test never has.
@ Why IwramExpandBitRuns is assembly:
@ bcc and bcs consume the carry set by lsls r3 in the bit-run loop.
@ Why IwramTransformVector to IwramTransformMatrix are assembly:
@ fp is a product high half rather than a frame pointer; stmia writes
@ arithmetic-transformed vectors, not compiler block-copy data.
@ Why IwramSignedDivide to IwramUnsignedDivide are assembly:
@ signed wrappers use unshifted movs and return through ip. The core
@ returns quotient in r0 and remainder in r1; the remainder wrappers read
@ that second result, a custom internal linkage.
@ Why IwramDecompress is assembly:
@ push saves r5/r6/r7 without lr.
.syntax unified
	.arm

	.global IwramMulQ16ReturnIp
IwramMulQ16ReturnIp:
	smull	r2, r0, r1, r0
	lsl	r0, r0, #16
	orr	r0, r0, r2, lsr #16
	add	ip, ip, #1
	bx	ip
.syntax unified
	.arm

	.global IwramMulQ16
IwramMulQ16:
	smull	r2, r0, r1, r0
	lsl	r0, r0, #16
	orr	r0, r0, r2, lsr #16
	bx	lr
.syntax unified
	.arm

	.global IwramRatioMulQ14
IwramRatioMulQ14:
	stmfd	sp!, {lr}
	mov	r4, r1
	mov	r1, r0
	mov	r0, #0x40000000
	bl	IwramSignedDivide
	smull	r3, r0, r4, r0
	ldmfd	sp!, {lr}
	lsl	r0, r0, #18
	orr	r0, r0, r3, lsr #14
	bx	lr
.syntax unified
	.arm

	.global IwramClearWords
IwramClearWords:
	mov	r2, #0
	.global IwramFillWords
IwramFillWords:
	push	{r5, r6, r7, r8, r9}
	mov	r3, r2
	mov	r4, r2
	mov	r5, r2
	mov	r6, r2
	mov	r7, r2
	mov	r8, r2
	mov	r9, r2
	ands	ip, r1, #0xe0
	rsb	ip, ip, #0xe0
	add	pc, pc, ip, lsr #3
.L_clear_256:
	stmia	r0!, {r2, r3, r4, r5, r6, r7, r8, r9}
	stmia	r0!, {r2, r3, r4, r5, r6, r7, r8, r9}
	stmia	r0!, {r2, r3, r4, r5, r6, r7, r8, r9}
	stmia	r0!, {r2, r3, r4, r5, r6, r7, r8, r9}
	stmia	r0!, {r2, r3, r4, r5, r6, r7, r8, r9}
	stmia	r0!, {r2, r3, r4, r5, r6, r7, r8, r9}
	stmia	r0!, {r2, r3, r4, r5, r6, r7, r8, r9}
	stmia	r0!, {r2, r3, r4, r5, r6, r7, r8, r9}
	subs	r1, r1, #0x100
	bpl	.L_clear_256
	ands	r1, r1, #0x1c
	beq	.L_clear_done
.L_clear_word:
	stmia	r0!, {r2}
	subs	r1, r1, #4
	bgt	.L_clear_word
.L_clear_done:
	pop	{r5, r6, r7, r8, r9}
	bx	lr
.syntax unified
	.arm

	.global IwramSqrt
IwramSqrt:
	movs	r1, r0
	mov	r0, #0
	bxmi	lr
	mov	r2, #0x8000
.L_sqrt_coarse:
	add	r0, r0, r2
	mul	r3, r0, r0
	cmp	r3, r1
	subhi	r0, r0, r2
	add	r0, r0, r2, lsr #1
	mul	r3, r0, r0
	cmp	r3, r1
	subhi	r0, r0, r2, lsr #1
	lsrs	r2, r2, #2
	bne	.L_sqrt_coarse
	bx	lr
	.include "games/tbs/include/game/ed_asm.h"
.syntax unified
	.arm

	.global IwramExpandBitRuns
IwramExpandBitRuns:
	.ifeq EDITION_INTERNATIONAL
	add	ip, r0, #24
	.else
	add	ip, r0, #28
	.endif
.L_expand_halfword:
	mov	r4, r1
	ldrh	r3, [r0], #2
	bic	r3, r3, #3
	lsls	r3, r3, #17
	bcc	.L_expand_zero
.L_expand_store:
	strb	r2, [r4], #1
.L_expand_shift:
	lsls	r3, r3, #1
	bcs	.L_expand_store
.L_expand_zero:
	add	r4, r4, #1
	bne	.L_expand_shift
	add	r1, r1, #16
	cmp	r0, ip
	bne	.L_expand_halfword
	bx	lr
.syntax unified
	.arm

	.global IwramTransformVector
IwramTransformVector:
	push	{r5, r6, r7, r8, r9, sl, fp, lr}
	ldm	r0, {r2, r3, r4}
	add	r0, pc, #0xf0
	ldmia	r0!, {r5, r6, r7}
	smull	r8, r9, r2, r5
	smull	sl, fp, r2, r6
	smull	ip, lr, r2, r7
	ldmia	r0!, {r5, r6, r7}
	smlal	r8, r9, r3, r5
	smlal	sl, fp, r3, r6
	smlal	ip, lr, r3, r7
	ldmia	r0!, {r5, r6, r7}
	smlal	r8, r9, r4, r5
	smlal	sl, fp, r4, r6
	smlal	ip, lr, r4, r7
	lsr	r8, r8, #16
	orr	r8, r8, r9, lsl #16
	lsr	sl, sl, #16
	orr	sl, sl, fp, lsl #16
	lsr	ip, ip, #16
	orr	ip, ip, lr, lsl #16
	ldmia	r0!, {r5, r6, r7}
	add	r5, r5, r8
	add	r6, r6, sl
	add	r7, r7, ip
	stmia	r1!, {r5, r6, r7}
	pop	{r5, r6, r7, r8, r9, sl, fp, lr}
	bx	lr

	.global IwramTransformMatrix
IwramTransformMatrix:
	push	{r5, r6, r7, r8, r9, sl, fp, lr}
	sub	sp, sp, #0x24
	mov	r4, sp
.L_matrix_row:
	add	r1, pc, #0x7c
	ldmia	r0!, {r2, r3}
	ldmia	r1!, {r5, r6, r7}
	smull	r8, r9, r5, r2
	smull	sl, fp, r6, r2
	smull	ip, lr, r7, r2
	ldmia	r1!, {r5, r6, r7}
	smlal	r8, r9, r5, r3
	smlal	sl, fp, r6, r3
	smlal	ip, lr, r7, r3
	ldmia	r1!, {r5, r6, r7}
	ldr	r2, [r0], #4
	smlal	r8, r9, r5, r2
	smlal	sl, fp, r6, r2
	smlal	ip, lr, r7, r2
	lsr	r8, r8, #16
	orr	r8, r8, r9, lsl #16
	lsr	sl, sl, #16
	orr	sl, sl, fp, lsl #16
	lsr	ip, ip, #16
	orr	ip, ip, lr, lsl #16
	sub	r2, r4, sp
	cmp	r2, #0x24
	stmiane	r4!, {r8, sl, ip}
	bne	.L_matrix_row
	ldmia	r1!, {r5, r6, r7}
	add	ip, ip, r7
	add	fp, sl, r6
	add	sl, r8, r5
	pop	{r0, r2, r3, r4, r5, r6, r7, r8, r9}
	stmdb	r1, {r0, r2, r3, r4, r5, r6, r7, r8, r9, sl, fp, ip}
	pop	{r5, r6, r7, r8, r9, sl, fp, lr}
	bx	lr
.syntax unified
	.section .data,"aw",%progbits
	.arm

	.global gTransform
gTransform:
	.4byte 0x00010000, 0, 0, 0
	.4byte 0x00010000, 0, 0, 0
	.4byte 0x00010000, 0, 0, 0
.syntax unified
	.section .text.runtime_divide,"ax",%progbits
	.arm

	.global IwramSignedDivide
IwramSignedDivide:
	eor	ip, r0, r1
	movs	r2, r1
	rsbmi	r2, r2, #0
	movs	r1, r0
	rsbmi	r1, r1, #0
	lsrs	r0, ip, #32
	bcc	.L_divide_core
	mov	ip, lr
	bl	.L_divide_core
	rsb	r0, r0, #0
	bx	ip

	.global IwramSignedRemainder
IwramSignedRemainder:
	stmfd	sp!, {lr}
	eor	ip, r0, r1
	movs	r2, r1
	rsbmi	r2, r2, #0
	movs	r1, r0
	rsbmi	r1, r1, #0
	mov	r0, #0
	bl	.L_divide_core
	mov	r0, r1
	movs	ip, ip
	rsbmi	r0, r0, #0
	ldmfd	sp!, {lr}
	bx	lr

	.global IwramUnsignedRemainder
IwramUnsignedRemainder:
	mov	ip, lr
	bl	IwramUnsignedDivide
	mov	r0, r1
	bx	ip

	.global IwramUnsignedDivide
IwramUnsignedDivide:
	mov	r2, r1
	mov	r1, r0
	mov	r0, #0
.L_divide_core:
	rsbs	r3, r2, r1, lsr #28
	bcc	.L_divide_24
	rsbs	r3, r2, r1, lsr #31
	orrcs	r0, r0, #0x80000000
	subcs	r1, r1, r2, lsl #31
	rsbs	r3, r2, r1, lsr #30
	orrcs	r0, r0, #0x40000000
	subcs	r1, r1, r2, lsl #30
	rsbs	r3, r2, r1, lsr #29
	orrcs	r0, r0, #0x20000000
	subcs	r1, r1, r2, lsl #29
	rsbs	r3, r2, r1, lsr #28
	orrcs	r0, r0, #0x10000000
	subcs	r1, r1, r2, lsl #28
.L_divide_24:
	rsbs	r3, r2, r1, lsr #24
	bcc	.L_divide_20
	rsbs	r3, r2, r1, lsr #27
	orrcs	r0, r0, #0x08000000
	subcs	r1, r1, r2, lsl #27
	rsbs	r3, r2, r1, lsr #26
	orrcs	r0, r0, #0x04000000
	subcs	r1, r1, r2, lsl #26
	rsbs	r3, r2, r1, lsr #25
	orrcs	r0, r0, #0x02000000
	subcs	r1, r1, r2, lsl #25
	rsbs	r3, r2, r1, lsr #24
	orrcs	r0, r0, #0x01000000
	subcs	r1, r1, r2, lsl #24
.L_divide_20:
	rsbs	r3, r2, r1, lsr #20
	bcc	.L_divide_16
	rsbs	r3, r2, r1, lsr #23
	orrcs	r0, r0, #0x00800000
	subcs	r1, r1, r2, lsl #23
	rsbs	r3, r2, r1, lsr #22
	orrcs	r0, r0, #0x00400000
	subcs	r1, r1, r2, lsl #22
	rsbs	r3, r2, r1, lsr #21
	orrcs	r0, r0, #0x00200000
	subcs	r1, r1, r2, lsl #21
	rsbs	r3, r2, r1, lsr #20
	orrcs	r0, r0, #0x00100000
	subcs	r1, r1, r2, lsl #20
.L_divide_16:
	rsbs	r3, r2, r1, lsr #16
	bcc	.L_divide_12
	rsbs	r3, r2, r1, lsr #19
	orrcs	r0, r0, #0x00080000
	subcs	r1, r1, r2, lsl #19
	rsbs	r3, r2, r1, lsr #18
	orrcs	r0, r0, #0x00040000
	subcs	r1, r1, r2, lsl #18
	rsbs	r3, r2, r1, lsr #17
	orrcs	r0, r0, #0x00020000
	subcs	r1, r1, r2, lsl #17
	rsbs	r3, r2, r1, lsr #16
	orrcs	r0, r0, #0x00010000
	subcs	r1, r1, r2, lsl #16
.L_divide_12:
	rsbs	r3, r2, r1, lsr #12
	bcc	.L_divide_8
	rsbs	r3, r2, r1, lsr #15
	orrcs	r0, r0, #0x00008000
	subcs	r1, r1, r2, lsl #15
	rsbs	r3, r2, r1, lsr #14
	orrcs	r0, r0, #0x00004000
	subcs	r1, r1, r2, lsl #14
	rsbs	r3, r2, r1, lsr #13
	orrcs	r0, r0, #0x00002000
	subcs	r1, r1, r2, lsl #13
	rsbs	r3, r2, r1, lsr #12
	orrcs	r0, r0, #0x00001000
	subcs	r1, r1, r2, lsl #12
.L_divide_8:
	rsbs	r3, r2, r1, lsr #8
	bcc	.L_divide_4
	rsbs	r3, r2, r1, lsr #11
	orrcs	r0, r0, #0x00000800
	subcs	r1, r1, r2, lsl #11
	rsbs	r3, r2, r1, lsr #10
	orrcs	r0, r0, #0x00000400
	subcs	r1, r1, r2, lsl #10
	rsbs	r3, r2, r1, lsr #9
	orrcs	r0, r0, #0x00000200
	subcs	r1, r1, r2, lsl #9
	rsbs	r3, r2, r1, lsr #8
	orrcs	r0, r0, #0x00000100
	subcs	r1, r1, r2, lsl #8
.L_divide_4:
	rsbs	r3, r2, r1, lsr #4
	bcc	.L_divide_low
	rsbs	r3, r2, r1, lsr #7
	orrcs	r0, r0, #0x80
	subcs	r1, r1, r2, lsl #7
	rsbs	r3, r2, r1, lsr #6
	orrcs	r0, r0, #0x40
	subcs	r1, r1, r2, lsl #6
	rsbs	r3, r2, r1, lsr #5
	orrcs	r0, r0, #0x20
	subcs	r1, r1, r2, lsl #5
	rsbs	r3, r2, r1, lsr #4
	orrcs	r0, r0, #0x10
	subcs	r1, r1, r2, lsl #4
.L_divide_low:
	rsbs	r3, r2, r1
	bxcc	lr
	rsbs	r3, r2, r1, lsr #3
	orrcs	r0, r0, #8
	subcs	r1, r1, r2, lsl #3
	rsbs	r3, r2, r1, lsr #2
	orrcs	r0, r0, #4
	subcs	r1, r1, r2, lsl #2
	rsbs	r3, r2, r1, lsr #1
	orrcs	r0, r0, #2
	subcs	r1, r1, r2, lsl #1
	rsbs	r3, r2, r1
	orrcs	r0, r0, #1
	subcs	r1, r1, r2
	bx	lr
.syntax unified
	.arm

	.global IwramDecompress
IwramDecompress:
	ldrb	r2, [r0], #1
	ldrb	r3, [r0], #1
	orrs	r2, r2, r3, lsl #8
	bxeq	lr
	push	{r5, r6, r7}
	mov	r6, r0
	add	r2, r2, r0
	sub	r2, r2, #2
.L_decompress_flag_reload:
	ldrb	r3, [r2], #1
	orr	r3, r3, #0x100
.L_decompress_flag:
	lsrs	r3, r3, #1
	bcc	.L_decompress_backref
	beq	.L_decompress_flag_reload
	ldrb	r4, [r0], #1
	subs	r5, r4, #0xdf
	addhi	r1, r1, r5
	strbls	r4, [r1], #1
	b	.L_decompress_flag
.L_decompress_backref:
	ldrb	r4, [r2], #1
	ldrb	ip, [r2], #1
	orrs	r4, ip, r4, lsl #8
	beq	.L_decompress_done
	bic	ip, r4, #0xf000
	sub	ip, r6, ip
	lsrs	r4, r4, #12
	ldrbeq	r4, [r2], #1
	addeq	r4, r4, #16
	add	r4, r4, #1
.L_decompress_copy:
	ldrb	r5, [ip], #1
	subs	r7, r5, #0xdf
	addhi	r1, r1, r7
	strbls	r5, [r1], #1
	subs	r4, r4, #1
	bpl	.L_decompress_copy
	b	.L_decompress_flag
.L_decompress_done:
	pop	{r5, r6, r7}
	mov	r0, #0
	bx	lr
