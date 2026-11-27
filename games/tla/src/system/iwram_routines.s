@ Why assembly: smull r2, r0, r1, r0 puts the low half in r2 and the high
@ half in r0. mulsidi3 writes a DI register pair, RdHi = RdLo + 1,
@ earlyclobbered and so apart from both inputs; here RdHi is r0, not r3, and
@ is also an input. So this multiply did not come from mulsidi3.
@ smulsi3_highpart can emit this same smull, but its low half is a dead
@ scratch, and orr r0, r0, r2, lsr #16 reads it. Its Thumb entry's bx pc
@ switches to the ARM code that follows, a mode switch inside a function;
@ the halfword after bx pc is never run.
@ Why assembly: both smull halves are consumed from nonconsecutive r3/r0;
@ compiler DI multiply patterns require consecutive halves, highpart
@ patterns discard the low half. Its Thumb entry's bx pc switches to the ARM code that follows, a mode switch inside a function;
@ the halfword after bx pc is never run.
@ Why syIWRAMWordsClear to syIWRAMWordsFill are assembly:
@ push saves r5-r9 without lr and computed add pc enters the unrolled store
@ body. syIWRAMWordsClear's Thumb entry's bx pc switches to the ARM code that
@ follows, a mode switch inside a function; the halfword after bx pc is
@ never run.
@ Why syIWRAMSquart is assembly:
@ movs r1, r0 copies and tests without a shift. The compilers' copy-and-test
@ (*movsi_compare0) is subs r1, r0, #0; a register-to-register movs comes only
@ with a shift.
@ Secondary: the sign exit that follows is bxmi lr. *movsi_compare0 sets plain
@ CC, where a signed < 0 test is lt; mi is the CC_NOOV spelling, which a
@ copy-and-test never has.
@ Why the unnamed function after syIWRAMSquart is assembly:
@ bcc and bcs consume the carry set by lsls r3 in the bit-run loop.
@ Why syIWRAMTransformVector to syIWRAMMultiMatrix are assembly:
@ fp is a product high half rather than a frame pointer; stmia writes
@ arithmetic-transformed vectors, not compiler block-copy data.
@ syIWRAMMultiMatrix also copies and tests without a shift (movs r1, r1).
@ Each Thumb entry's bx pc switches to the ARM code that follows, a mode
@ switch inside a function; the halfword after bx pc is never run.
@ Why syIWRAMSignedRemainder to syIWRAMUnsignedDivide are assembly:
@ signed wrappers use unshifted movs (movs r2, r1) and return through ip
@ (mov ip, lr; bx ip). The core returns the quotient in r0 and the remainder
@ in r1; the remainder wrappers read that second result (mov r0, r1), a
@ custom internal linkage. lsrs r0, ip, #32 / bcc branches on the carry of
@ the shift, the sign bit of eor ip, r0, r1. Each Thumb entry's bx pc
@ switches to the ARM code that follows, a mode switch inside a function;
@ the halfword after bx pc is never run.
@ Why assembly: push saves r5/r6/r7 without lr, and bcc consumes the carry of
@ lsrs r3, r3, #1. Its Thumb entry's bx pc switches to the ARM code that follows, a mode switch inside a function;
@ the halfword after bx pc is never run.
@ Why assembly: computed add pc enters the unrolled copy body; r5-r9 are
@ saved without lr. Its Thumb entry's bx pc switches to the ARM code that follows, a mode switch inside a function;
@ the halfword after bx pc is never run.
@ Why assembly: the ARM loop uses svc 0x00030000, a software
@ interrupt, then branches back to the same svc instruction.
	.section .text,"ax",%progbits
	.balign 4
.syntax unified
	.thumb
	.global syIWRAMMulFX
syIWRAMMulFX:
	bx	pc
AlchemyUncredited080007d6:
	.2byte	0x0200
	.arm
AlchemyUncreditedEnd080007d6:
	smull	r2, r0, r1, r0
	lsl	r0, r0, #16
	orr	r0, r0, r2, lsr #16
	bx	lr

	.section .text,"ax",%progbits
	.balign 4
.syntax unified
	.thumb
	.global syIWRAMDivFX
syIWRAMDivFX:
	bx	pc
AlchemyUncredited080007ea:
	.2byte	0x0200
	.arm
AlchemyUncreditedEnd080007ea:
	push	{r1, lr}
	mov	r1, r0
	mov	r0, #0x40000000
	bl	syIWRAMDivide
	pop	{r1, lr}
	smull	r3, r0, r1, r0
	lsl	r0, r0, #18
	orr	r0, r0, r3, lsr #14
	bx	lr

	.section .text,"ax",%progbits
	.balign 4
	.include "games/tla/include/edition.inc"
.syntax unified
	.thumb
	.global syIWRAMWordsClear
syIWRAMWordsClear:
	bx	pc
AlchemyUncredited08000812:
	.2byte	0x0200
	.arm
AlchemyUncreditedEnd08000812:
	mov	r2, #0
	.global syIWRAMWordsFill
syIWRAMWordsFill:
@ 78 47 00 02: Thumb bx pc and lsls r0, r0, #8 for a caller in Thumb state;
@ syIWRAMWordsClear, falling through in ARM state, runs it as this andeq.
	andeq	r4, r0, #120, 14
	push	{r5, r6, r7, r8, r9}
	mov	r3, r2
	mov	r4, r2
	mov	r5, r2
	mov	r6, r2
	mov	r7, r2
	mov	r8, r2
	mov	r9, r2
	ands	ip, r1, #224
	rsb	ip, ip, #224
	add	pc, pc, ip, lsr #3
.L_03000290:
	stmia	r0!, {r2, r3, r4, r5, r6, r7, r8, r9}
	stmia	r0!, {r2, r3, r4, r5, r6, r7, r8, r9}
	stmia	r0!, {r2, r3, r4, r5, r6, r7, r8, r9}
	stmia	r0!, {r2, r3, r4, r5, r6, r7, r8, r9}
	stmia	r0!, {r2, r3, r4, r5, r6, r7, r8, r9}
	stmia	r0!, {r2, r3, r4, r5, r6, r7, r8, r9}
	stmia	r0!, {r2, r3, r4, r5, r6, r7, r8, r9}
	stmia	r0!, {r2, r3, r4, r5, r6, r7, r8, r9}
	subs	r1, r1, #0x100
	bpl	.L_03000290
	ands	r1, r1, #28
	beq	.L_030002cc
.L_030002c0:
	stmia	r0!, {r2}
	subs	r1, r1, #4
	bgt	.L_030002c0
.L_030002cc:
	pop	{r5, r6, r7, r8, r9}
	bx	lr
	.thumb
	.global syIWRAMSquart
syIWRAMSquart:
	bx	pc
AlchemyUncredited0800088e:
	.2byte	0x0200
	.arm
AlchemyUncreditedEnd0800088e:
	movs	r1, r0
	mov	r0, #0
	bxmi	lr
	mov	r2, #0x8000
.L_030002e8:
	add	r0, r0, r2
	mul	r3, r0, r0
	cmp	r3, r1
	subhi	r0, r0, r2
	add	r0, r0, r2, lsr #1
	mul	r3, r0, r0
	cmp	r3, r1
	subhi	r0, r0, r2, lsr #1
	lsrs	r2, r2, #2
	bne	.L_030002e8
	bx	lr
	.thumb
	.global syIWRAMExpandBitRuns
syIWRAMExpandBitRuns:
	bx	pc
AlchemyUncredited080008ce:
	.2byte	0x0200
	.arm
AlchemyUncreditedEnd080008ce:
	.ifeq EDITION_INTERNATIONAL
	add	ip, r0, #24
	.else
	add	ip, r0, #28
	.endif
.L_0300031c:
	mov	r4, r1
	ldrh	r3, [r0], #2
	bic	r3, r3, #3
	lsls	r3, r3, #17
	bcc	.L_0300033c
.L_03000330:
	strb	r2, [r4], #1
.L_03000334:
	lsls	r3, r3, #1
	bcs	.L_03000330
.L_0300033c:
	add	r4, r4, #1
	bne	.L_03000334
	add	r1, r1, #16
	cmp	r0, ip
	bne	.L_0300031c
	bx	lr

	.section .text,"ax",%progbits
	.balign 4
.syntax unified
	.thumb
	.global syIWRAMTransformVector
syIWRAMTransformVector:
	bx	pc
AlchemyUncredited0800090e:
	.2byte	0x0200
	.arm
AlchemyUncreditedEnd0800090e:
	push	{r5, r6, r7, r8, r9, sl, fp, lr}
	ldm	r0, {r2, r3, r4}
	add	r0, pc, #0x110
	ldm	r0!, {r5, r6, r7}
	smull	r8, r9, r2, r5
	smull	sl, fp, r2, r6
	smull	ip, lr, r2, r7
	ldm	r0!, {r5, r6, r7}
	smlal	r8, r9, r3, r5
	smlal	sl, fp, r3, r6
	smlal	ip, lr, r3, r7
	ldm	r0!, {r5, r6, r7}
	smlal	r8, r9, r4, r5
	smlal	sl, fp, r4, r6
	smlal	ip, lr, r4, r7
	lsr	r8, r8, #16
	orr	r8, r8, r9, lsl #16
	lsr	sl, sl, #16
	orr	sl, sl, fp, lsl #16
	lsr	ip, ip, #16
	orr	ip, ip, lr, lsl #16
	ldm	r0!, {r5, r6, r7}
	add	r5, r5, r8
	add	r6, r6, sl
	add	r7, r7, ip
	stmia	r1!, {r5, r6, r7}
	pop	{r5, r6, r7, r8, r9, sl, fp, lr}
	bx	lr
	.thumb
	.global syIWRAMMultiMatrix
syIWRAMMultiMatrix:
	bx	pc
AlchemyUncredited08000982:
	.2byte	0x0200
	.arm
AlchemyUncreditedEnd08000982:
	push	{r5, r6, r7, r8, r9, sl, fp, lr}
	sub	sp, sp, #36
	mov	r4, sp
.L_030003d8:
	ldm	r0!, {r1, r2, r3}
	movs	r1, r1
	add	r5, pc, #144
	ldmne	r5, {r5, r6, r7}
	smull	r8, r9, r5, r1
	smull	sl, fp, r6, r1
	smull	ip, lr, r7, r1
	movs	r2, r2
	beq	.L_03000410
	add	r5, pc, #128
	ldmne	r5, {r5, r6, r7}
	smlalne	r8, r9, r5, r2
	smlalne	sl, fp, r6, r2
	smlalne	ip, lr, r7, r2
.L_03000410:
	movs	r3, r3
	beq	.L_0300042c
	add	r5, pc, #112
	ldmne	r5, {r5, r6, r7}
	smlalne	r8, r9, r5, r3
	smlalne	sl, fp, r6, r3
	smlalne	ip, lr, r7, r3
.L_0300042c:
	lsr	r8, r8, #16
	orr	r8, r8, r9, lsl #16
	lsr	sl, sl, #16
	orr	sl, sl, fp, lsl #16
	lsr	ip, ip, #16
	orr	ip, ip, lr, lsl #16
	sub	r2, r4, sp
	cmp	r2, #36
	stmiane	r4!, {r8, sl, ip}
	bne	.L_030003d8
	add	r1, pc, #64
	ldm	r1!, {r5, r6, r7}
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
	.global CurrentMatrix
CurrentMatrix:
	.4byte	0x00010000
	.4byte	0x00000000
	.4byte	0x00000000
	.4byte	0
	.4byte	0x00010000
	.4byte	0
	.4byte	0
	.4byte	0
	.4byte	0x00010000
	.global CurrentMatrixTranslation
CurrentMatrixTranslation:
	.4byte	0x00000000
	.4byte	0x00000000
	.4byte	0x00000000

	.section .text.iwram_divide,"ax",%progbits
	.balign 4
.syntax unified
	.arm
.L_030004a8:
	eor	ip, r0, r1
	movs	r2, r1
	rsbmi	r2, r2, #0
	movs	r1, r0
	rsbmi	r1, r1, #0
	lsrs	r0, ip, #32
	bcc	.L_03000544
	mov	ip, lr
	bl	.L_03000544
	rsb	r0, r0, #0
	bx	ip
.L_030004d4:
	stmfd	sp!, {lr}
	eor	ip, r0, r1
	movs	r2, r1
	rsbmi	r2, r2, #0
	movs	r1, r0
	rsbmi	r1, r1, #0
	mov	r0, #0
	bl	.L_03000544
	mov	r0, r1
	movs	ip, ip
	rsbmi	r0, r0, #0
	ldmfd	sp!, {lr}
	bx	lr
	.thumb
	.global syIWRAMSignedRemainder
syIWRAMSignedRemainder:
	bx	pc
AlchemyUncredited08000ac2:
	.2byte	0x0200
	.arm
AlchemyUncreditedEnd08000ac2:
	orrs	r3, r0, r1
	bmi	.L_030004d4
	.global syIWRAMUnsignedRemainder
syIWRAMUnsignedRemainder:
@ 78 47 00 02: Thumb bx pc and lsls r0, r0, #8 for a caller in Thumb state;
@ syIWRAMSignedRemainder, falling through in ARM state, runs it as this andeq.
	andeq	r4, r0, #120, 14
	mov	ip, lr
	bl	.L_03000534
	mov	r0, r1
	bx	ip
	.global syIWRAMDivide
syIWRAMDivide:
	.thumb
	.global syIWRAMSignedDivide
syIWRAMSignedDivide:
	bx	pc
AlchemyUncredited08000ae2:
	.2byte	0x0200
	.arm
AlchemyUncreditedEnd08000ae2:
	orrs	r3, r0, r1
	bmi	.L_030004a8
	.global syIWRAMUnsignedDivide
syIWRAMUnsignedDivide:
.L_03000534:
@ The same Thumb entry word, run as andeq by bl .L_03000534 and by syIWRAMSignedDivide falling through.
	andeq	r4, r0, #120, 14
	mov	r2, r1
	mov	r1, r0
	mov	r0, #0
.L_03000544:
@ The same Thumb entry word, run as andeq when ARM code branches to .L_03000544 or falls through.
	andeq	r4, r0, #120, 14
	cmp	r2, #0x4000
	bcs	.L_03000578
	ldr	r3, [pc, #28]
	ldr	r3, [r3, r2, lsl #2]
	umulls	r3, r0, r1, r3
	mulne	r3, r0, r2
	subne	r1, r1, r3
	bxne	lr
	mov	r0, r1
	mov	r1, #0
	bx	lr
	.4byte	SystemReciprocalTable
.L_03000578:
	rsbs	r3, r2, r1, lsr #16
	bcc	.L_030005b0
	rsbs	r3, r2, r1, lsr #19
	orrcs	r0, r0, #0x80000
	subcs	r1, r1, r2, lsl #19
	rsbs	r3, r2, r1, lsr #18
	orrcs	r0, r0, #0x40000
	subcs	r1, r1, r2, lsl #18
	rsbs	r3, r2, r1, lsr #17
	orrcs	r0, r0, #0x20000
	subcs	r1, r1, r2, lsl #17
	rsbs	r3, r2, r1, lsr #16
	orrcs	r0, r0, #0x10000
	subcs	r1, r1, r2, lsl #16
.L_030005b0:
	rsbs	r3, r2, r1, lsr #12
	bcc	.L_030005e8
	rsbs	r3, r2, r1, lsr #15
	orrcs	r0, r0, #0x8000
	subcs	r1, r1, r2, lsl #15
	rsbs	r3, r2, r1, lsr #14
	orrcs	r0, r0, #0x4000
	subcs	r1, r1, r2, lsl #14
	rsbs	r3, r2, r1, lsr #13
	orrcs	r0, r0, #0x2000
	subcs	r1, r1, r2, lsl #13
	rsbs	r3, r2, r1, lsr #12
	orrcs	r0, r0, #0x1000
	subcs	r1, r1, r2, lsl #12
.L_030005e8:
	rsbs	r3, r2, r1, lsr #8
	bcc	.L_03000620
	rsbs	r3, r2, r1, lsr #11
	orrcs	r0, r0, #0x800
	subcs	r1, r1, r2, lsl #11
	rsbs	r3, r2, r1, lsr #10
	orrcs	r0, r0, #0x400
	subcs	r1, r1, r2, lsl #10
	rsbs	r3, r2, r1, lsr #9
	orrcs	r0, r0, #0x200
	subcs	r1, r1, r2, lsl #9
	rsbs	r3, r2, r1, lsr #8
	orrcs	r0, r0, #0x100
	subcs	r1, r1, r2, lsl #8
.L_03000620:
	rsbs	r3, r2, r1, lsr #4
	bcc	.L_03000658
	rsbs	r3, r2, r1, lsr #7
	orrcs	r0, r0, #128
	subcs	r1, r1, r2, lsl #7
	rsbs	r3, r2, r1, lsr #6
	orrcs	r0, r0, #64
	subcs	r1, r1, r2, lsl #6
	rsbs	r3, r2, r1, lsr #5
	orrcs	r0, r0, #32
	subcs	r1, r1, r2, lsl #5
	rsbs	r3, r2, r1, lsr #4
	orrcs	r0, r0, #16
	subcs	r1, r1, r2, lsl #4
.L_03000658:
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

	.section .text.iwram_divide,"ax",%progbits
	.balign 4
.syntax unified
	.arm
	.thumb
	.global syIWRAMDecompress
syIWRAMDecompress:
	bx	pc
AlchemyUncredited08000c4e:
	.2byte	0x0200
	.arm
AlchemyUncreditedEnd08000c4e:
	ldrb	r2, [r0], #1
	ldrb	r3, [r0], #1
	orrs	r2, r2, r3, lsl #8
	bxeq	lr
	push	{r5, r6, r7}
	mov	r6, r0
	add	r2, r2, r0
	sub	r2, r2, #2
.L_030006b8:
	ldrb	r3, [r2], #1
	orr	r3, r3, #0x100
.L_030006c0:
	lsrs	r3, r3, #1
	bcc	.L_030006e0
	beq	.L_030006b8
	ldrb	r4, [r0], #1
	subs	r5, r4, #223
	addhi	r1, r1, r5
	strbls	r4, [r1], #1
	b	.L_030006c0
.L_030006e0:
	ldrb	r4, [r2], #1
	ldrb	ip, [r2], #1
	orrs	r4, ip, r4, lsl #8
	beq	.L_03000724
	bic	ip, r4, #0xf000
	sub	ip, r6, ip
	lsrs	r4, r4, #12
	ldrbeq	r4, [r2], #1
	addeq	r4, r4, #16
	add	r4, r4, #1
.L_03000708:
	ldrb	r5, [ip], #1
	subs	r7, r5, #223
	addhi	r1, r1, r7
	strbls	r5, [r1], #1
	subs	r4, r4, #1
	bpl	.L_03000708
	b	.L_030006c0
.L_03000724:
	pop	{r5, r6, r7}
	mov	r0, #0
	bx	lr

	.section .text.iwram_divide,"ax",%progbits
	.balign 4
.syntax unified
	.arm
	.thumb
	.global syIWRAMWordsCopy
syIWRAMWordsCopy:
	bx	pc
AlchemyUncredited08000cea:
	.2byte	0x0200
	.arm
AlchemyUncreditedEnd08000cea:
	push	{r5, r6, r7, r8, r9}
	ands	ip, r2, #224
	rsb	ip, ip, #240
	add	pc, pc, ip, lsr #2
.L_03000744:
	ldm	r1!, {r3, r4, r5, r6, r7, r8, r9, ip}
	stmia	r0!, {r3, r4, r5, r6, r7, r8, r9, ip}
	ldm	r1!, {r3, r4, r5, r6, r7, r8, r9, ip}
	stmia	r0!, {r3, r4, r5, r6, r7, r8, r9, ip}
	ldm	r1!, {r3, r4, r5, r6, r7, r8, r9, ip}
	stmia	r0!, {r3, r4, r5, r6, r7, r8, r9, ip}
	ldm	r1!, {r3, r4, r5, r6, r7, r8, r9, ip}
	stmia	r0!, {r3, r4, r5, r6, r7, r8, r9, ip}
	ldm	r1!, {r3, r4, r5, r6, r7, r8, r9, ip}
	stmia	r0!, {r3, r4, r5, r6, r7, r8, r9, ip}
	ldm	r1!, {r3, r4, r5, r6, r7, r8, r9, ip}
	stmia	r0!, {r3, r4, r5, r6, r7, r8, r9, ip}
	ldm	r1!, {r3, r4, r5, r6, r7, r8, r9, ip}
	stmia	r0!, {r3, r4, r5, r6, r7, r8, r9, ip}
	ldm	r1!, {r3, r4, r5, r6, r7, r8, r9, ip}
	stmia	r0!, {r3, r4, r5, r6, r7, r8, r9, ip}
	subs	r2, r2, #0x100
	bpl	.L_03000744
	ands	r2, r2, #28
	beq	.L_030007a4
.L_03000794:
	ldm	r1!, {r3}
	stmia	r0!, {r3}
	subs	r2, r2, #4
	bgt	.L_03000794
.L_030007a4:
	pop	{r5, r6, r7, r8, r9}
	bx	lr

	.section .text.iwram_divide,"ax",%progbits
	.balign 4
.syntax unified
	.arm
	.global syHalt
syHalt:
.L_030007ac:
	svc	0x00030000
	b	.L_030007ac
