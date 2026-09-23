@ Decode loop of the format 2 decoder (DECODE_2.S). It rebuilds nibbles by
@ calling into a table of 32-byte code slots with mov lr, pc;
@ add pc, r9, r8, lsl #5, tests code bits on the carry out of lsrs, and
@ sets the return address of its refill subroutine by hand
@ (sub lr, pc, #280). None of this is compiler output. The 50 nop words
@ that fill the slots after their bx lr or branch never run and carry no
@ credit, as in TBS. These 1,188 bytes equal TBS
@ SYSTEM/RESOURCE/DECODE_2_LOOP.S (0x08002850).
@ credit: library — TBS/TLA common ARM graphics and runtime kernel bank, compression format 2 decoder loop
.syntax unified
	.arm
	.set AlchemyLoadAddress, 0x08012bbc
.L_08012bbc:
	and	r8, r2, #15
	add	r9, pc, #0x2ac
	mov	lr, pc
	add	pc, r9, r8, lsl #5
	mov	r5, r8
	and	r8, r2, #240
	add	r9, pc, #0x298
	mov	lr, pc
	add	pc, r9, r8, lsl #1
	orr	r5, r5, r8, lsl #4
	lsr	r2, r2, #8
	subs	r4, r4, #11
	blmi	.L_08012e54
	b	.L_08012c74
.L_08012bf4:
	lsrs	r2, r2, #1
	bcc	.L_08012bbc
	and	r8, r2, #7
	add	r9, pc, #0x26c
	mov	lr, pc
	add	pc, r9, r8, lsl #5
	mov	r5, r8
	and	r8, r2, #56
	add	r9, pc, #0x258
	mov	lr, pc
	add	pc, r9, r8, lsl #2
	orr	r5, r5, r8, lsl #4
	lsr	r2, r2, #6
	subs	r4, r4, #9
	blmi	.L_08012e54
	b	.L_08012c74
.L_08012c34:
	push	{r5, r9}
	lsrs	r2, r2, #1
	bcc	.L_08012bf4
	and	r8, r2, #3
	add	r9, pc, #0x228
	mov	lr, pc
	add	pc, r9, r8, lsl #5
	mov	r5, r8
	and	r8, r2, #12
	add	r9, pc, #0x214
	mov	lr, pc
	add	pc, r9, r8, lsl #3
	orr	r5, r5, r8, lsl #4
	lsr	r2, r2, #4
	subs	r4, r4, #6
	blmi	.L_08012e54
.L_08012c74:
	strb	r5, [r1, #1]!
	pop	{r5, r9}
.L_08012c7c:
	lsrs	r2, r2, #1
	bcs	.L_08012c34
	lsrs	r2, r2, #1
	bcc	.L_08012d38
	lsrs	r2, r2, #1
	bcc	.L_08012cfc
	lsrs	r2, r2, #1
	bcc	.L_08012d08
	lsrs	r2, r2, #1
	bcc	.L_08012d18
	lsrs	r2, r2, #1
	bcs	.L_08012cc4
	lsrs	r2, r2, #1
	bcc	.L_08012d28
	subs	r4, r4, #7
	mov	r5, #6
	bpl	.L_08012d40
	blmi	.L_08012e50
.L_08012cc4:
	ands	r5, r2, #3
	lsr	r2, r2, #2
	beq	.L_08012ce0
	add	r5, r5, #6
	subs	r4, r4, #8
	bpl	.L_08012d40
	blmi	.L_08012e50
.L_08012ce0:
	ands	r5, r2, #127
	beq	.L_08012e64
	add	r5, r5, #9
	lsr	r2, r2, #7
	subs	r4, r4, #15
	bpl	.L_08012d40
	blmi	.L_08012e50
.L_08012cfc:
	subs	r4, r4, #3
	mov	r5, #2
	b	.L_08012d40
.L_08012d08:
	subs	r4, r4, #4
	mov	r5, #3
	bpl	.L_08012d40
	blmi	.L_08012e50
.L_08012d18:
	subs	r4, r4, #5
	mov	r5, #4
	bpl	.L_08012d40
	blmi	.L_08012e50
.L_08012d28:
	subs	r4, r4, #7
	mov	r5, #5
	bpl	.L_08012d40
	blmi	.L_08012e50
.L_08012d38:
	subs	r4, r4, #2
	mov	r5, #1
.L_08012d40:
	lsrs	r2, r2, #1
	bcc	.L_08012d84
	and	r6, r2, #31
	subs	r4, r4, #6
	lsr	r2, r2, #5
.L_08012d54:
	blmi	.L_08012e54
	ldrb	r7, [r1, -r6]
	strb	r7, [r1, #1]!
.L_08012d60:
	ldrb	r7, [r1, -r6]
	strb	r7, [r1, #1]!
	subs	r5, r5, #1
	beq	.L_08012c7c
	ldrb	r7, [r1, -r6]
	strb	r7, [r1, #1]!
	subs	r5, r5, #1
	bne	.L_08012d60
	b	.L_08012c7c
.L_08012d84:
	sub	r6, r1, r9
	cmp	r6, #0x800
	bcc	.L_08012da8
	ror	r6, r2, #12
	lsr	r2, r2, #12
	lsr	r6, r6, #20
	add	r6, r6, #32
	subs	r4, r4, #13
	b	.L_08012d54
.L_08012da8:
	mov	r7, #11
	lsls	r6, r6, #22
	bcs	.L_08012e30
	sub	r7, r7, #1
	lsls	r6, r6, #1
	bcs	.L_08012e30
	sub	r7, r7, #1
	lsls	r6, r6, #1
	bcs	.L_08012e30
	sub	r7, r7, #1
	lsls	r6, r6, #1
	bcs	.L_08012e30
	sub	r7, r7, #1
	lsls	r6, r6, #1
	bcs	.L_08012e30
	sub	r7, r7, #1
	lsls	r6, r6, #1
	bcs	.L_08012e30
	sub	r7, r7, #1
	lsls	r6, r6, #1
	bcs	.L_08012e30
	sub	r7, r7, #1
	lsls	r6, r6, #1
	bcs	.L_08012e30
	sub	r7, r7, #1
	lsls	r6, r6, #1
	bcs	.L_08012e30
	sub	r7, r7, #1
	lsls	r6, r6, #1
	bcs	.L_08012e30
	sub	r7, r7, #1
	lsls	r6, r6, #1
	bcs	.L_08012e30
	sub	r7, r7, #1
.L_08012e30:
	mov	r6, #1
	rsb	r6, r6, r6, lsl r7
	and	r6, r2, r6
	add	r6, r6, #32
	lsr	r2, r2, r7
	sub	r4, r4, #1
	subs	r4, r4, r7
	b	.L_08012d54
.L_08012e50:
	sub	lr, pc, #0x118
.L_08012e54:
	ldrh	r3, [r0], #2
	add	r4, r4, #16
	orr	r2, r2, r3, lsl r4
	bx	lr
.L_08012e64:
	sub	r9, r9, #33
	sub	r0, r1, r9
	pop	{r5, r6, r7, r8, r9, sl, fp, lr}
	bx	lr
	and	r8, ip, fp
	bx	lr
AlchemyUncredited_08012e7c:
	nop
	nop
	nop
	nop
	nop
	nop
AlchemyUncreditedEnd_08012e7c:
	lsr	r9, fp, #8
	orr	r9, r9, fp, lsl #28
	and	r8, ip, fp, lsr #4
	orr	fp, r8, r9, ror #24
	bx	lr
AlchemyUncredited_08012ea8:
	nop
	nop
	nop
AlchemyUncreditedEnd_08012ea8:
	lsr	r9, fp, #12
	orr	r9, r9, fp, lsl #24
	and	r8, ip, fp, lsr #8
	orr	fp, r8, r9, ror #20
	bx	lr
AlchemyUncredited_08012ec8:
	nop
	nop
	nop
AlchemyUncreditedEnd_08012ec8:
	lsr	r9, fp, #16
	orr	r9, r9, fp, lsl #20
	and	r8, ip, fp, lsr #12
	orr	fp, r8, r9, ror #16
	bx	lr
AlchemyUncredited_08012ee8:
	nop
	nop
	nop
AlchemyUncreditedEnd_08012ee8:
	lsr	r9, fp, #20
	orr	r9, r9, fp, lsl #16
	and	r8, ip, fp, lsr #16
	orr	fp, r8, r9, ror #12
	bx	lr
AlchemyUncredited_08012f08:
	nop
	nop
	nop
AlchemyUncreditedEnd_08012f08:
	lsr	r9, fp, #24
	orr	r9, r9, fp, lsl #12
	and	r8, ip, fp, lsr #20
	orr	fp, r8, r9, ror #8
	bx	lr
AlchemyUncredited_08012f28:
	nop
	nop
	nop
AlchemyUncreditedEnd_08012f28:
	lsr	r9, fp, #28
	orr	r9, r9, fp, lsl #8
	and	r8, ip, fp, lsr #24
	orr	fp, r8, r9, ror #4
	bx	lr
AlchemyUncredited_08012f48:
	nop
	nop
	nop
AlchemyUncreditedEnd_08012f48:
	ror	fp, fp, #28
	and	r8, ip, fp
	bx	lr
AlchemyUncredited_08012f60:
	nop
	nop
	nop
	nop
	nop
AlchemyUncreditedEnd_08012f60:
	and	r8, ip, sl
	bic	r9, sl, #15
.L_08012f7c:
	orr	sl, r9, fp, lsr #28
	orr	fp, r8, fp, lsl #4
	bx	lr
AlchemyUncredited_08012f88:
	nop
	nop
	nop
AlchemyUncreditedEnd_08012f88:
	and	r8, ip, sl, lsr #4
	lsr	r9, sl, #8
	orr	r9, r9, sl, lsl #28
	ror	r9, r9, #24
	b	.L_08012f7c
AlchemyUncredited_08012fa8:
	nop
	nop
	nop
AlchemyUncreditedEnd_08012fa8:
	and	r8, ip, sl, lsr #8
	lsr	r9, sl, #12
	orr	r9, r9, sl, lsl #24
	ror	r9, r9, #20
	b	.L_08012f7c
AlchemyUncredited_08012fc8:
	nop
	nop
	nop
AlchemyUncreditedEnd_08012fc8:
	and	r8, ip, sl, lsr #12
	lsr	r9, sl, #16
	orr	r9, r9, sl, lsl #20
	ror	r9, r9, #16
	b	.L_08012f7c
AlchemyUncredited_08012fe8:
	nop
	nop
	nop
AlchemyUncreditedEnd_08012fe8:
	and	r8, ip, sl, lsr #16
	lsr	r9, sl, #20
	orr	r9, r9, sl, lsl #16
	ror	r9, r9, #12
	b	.L_08012f7c
AlchemyUncredited_08013008:
	nop
	nop
	nop
AlchemyUncreditedEnd_08013008:
	and	r8, ip, sl, lsr #20
	lsr	r9, sl, #24
	orr	r9, r9, sl, lsl #12
	ror	r9, r9, #8
	b	.L_08012f7c
AlchemyUncredited_08013028:
	nop
	nop
	nop
AlchemyUncreditedEnd_08013028:
	and	r8, ip, sl, lsr #24
	lsr	r9, sl, #28
	orr	r9, r9, sl, lsl #8
	ror	r9, r9, #4
	b	.L_08012f7c
AlchemyUncredited_08013048:
	nop
	nop
	nop
AlchemyUncreditedEnd_08013048:
	and	r8, ip, sl, lsr #28
	lsl	r9, sl, #4
	b	.L_08012f7c
