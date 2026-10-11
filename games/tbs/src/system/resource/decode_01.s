@ C2: Resource_DecodeType01Arm:
@ The bit reader branches on carry immediately after shifting its live
@ bit reservoir; it also uses unshifted movs r2,r2 for copy-and-test.
@ C2: Resource_Decode2:
@ add ip, pc, #36 followed by ldm ip, {sl, fp} loads a data block
@ relative to pc, including r11/fp as ordinary decoder state.
@ C2: Resource_DecodeType2Loop to Resource_RefillType2Bits:
@ Computed add pc enters fixed-size decoder code slots; command execution
@ also carries live reader register state across entry labels.
@ C2: Resource_ExecuteTransfer:
@ stmfd sp! saves callee-saved r5 without lr.
@ C2: Resource_PatchThumbBranch:
@ push saves callee-saved r5/r6 without lr; body also edits branch
@ instructions.
.syntax unified
	.arm
	.global Resource_DecodeType01Arm
Resource_DecodeType01Arm:
	push	{r5, r6, r7, r9, lr}
	ldrb	r2, [r0], #1
	cmp	r2, #0
	bne	.L_08002788
	sub	r1, r1, #1
	add	r9, r1, #33
	ands	r2, r0, #1
	lsl	r4, r2, #3
	ldrbne	r2, [r0], #1
	subs	r4, r4, #16
	blmi	.L_08002768
	b	.L_0800258c
.L_08002574:
	strb	r2, [r1, #1]!
	lsr	r2, r2, #8
	subs	r4, r4, #9
	ldrhmi	r3, [r0], #2
	addmi	r4, r4, #16
	orrmi	r2, r2, r3, lsl r4
.L_0800258c:
	lsrs	r2, r2, #1
	bcs	.L_08002574
	lsrs	r2, r2, #1
	bcc	.L_08002648
	lsrs	r2, r2, #1
	bcc	.L_0800260c
	lsrs	r2, r2, #1
	bcc	.L_08002618
	lsrs	r2, r2, #1
	bcc	.L_08002628
	lsrs	r2, r2, #1
	bcs	.L_080025d4
	lsrs	r2, r2, #1
	bcc	.L_08002638
	subs	r4, r4, #7
	mov	r5, #7
	bpl	.L_08002650
	blmi	.L_08002764
.L_080025d4:
	ands	r5, r2, #3
	lsr	r2, r2, #2
	beq	.L_080025f0
	add	r5, r5, #7
	subs	r4, r4, #8
	bpl	.L_08002650
	blmi	.L_08002764
.L_080025f0:
	ands	r5, r2, #127
	beq	.L_08002778
	add	r5, r5, #10
	lsr	r2, r2, #7
	subs	r4, r4, #15
	bpl	.L_08002650
	blmi	.L_08002764
.L_0800260c:
	subs	r4, r4, #3
	mov	r5, #3
	b	.L_08002650
.L_08002618:
	subs	r4, r4, #4
	mov	r5, #4
	bpl	.L_08002650
	blmi	.L_08002764
.L_08002628:
	subs	r4, r4, #5
	mov	r5, #5
	bpl	.L_08002650
	blmi	.L_08002764
.L_08002638:
	subs	r4, r4, #7
	mov	r5, #6
	bpl	.L_08002650
	blmi	.L_08002764
.L_08002648:
	subs	r4, r4, #2
	mov	r5, #2
.L_08002650:
	lsrs	r2, r2, #1
	bcc	.L_08002698
	and	r6, r2, #31
	subs	r4, r4, #6
	lsr	r2, r2, #5
.L_08002664:
	ldrhmi	r3, [r0], #2
	addmi	r4, r4, #16
	orrmi	r2, r2, r3, lsl r4
	lsrs	r5, r5, #1
	ldrbcs	r7, [r1, -r6]
	strbcs	r7, [r1, #1]!
.L_0800267c:
	ldrb	r7, [r1, -r6]
	strb	r7, [r1, #1]!
	ldrb	r7, [r1, -r6]
	strb	r7, [r1, #1]!
	subs	r5, r5, #1
	bne	.L_0800267c
	b	.L_0800258c
.L_08002698:
	sub	r6, r1, r9
	cmp	r6, #2048
	bcc	.L_080026bc
	ror	r6, r2, #12
	lsr	r2, r2, #12
	lsr	r6, r6, #20
	add	r6, r6, #32
	subs	r4, r4, #13
	b	.L_08002664
.L_080026bc:
	mov	r7, #11
	lsls	r6, r6, #22
	bcs	.L_08002744
	sub	r7, r7, #1
	lsls	r6, r6, #1
	bcs	.L_08002744
	sub	r7, r7, #1
	lsls	r6, r6, #1
	bcs	.L_08002744
	sub	r7, r7, #1
	lsls	r6, r6, #1
	bcs	.L_08002744
	sub	r7, r7, #1
	lsls	r6, r6, #1
	bcs	.L_08002744
	sub	r7, r7, #1
	lsls	r6, r6, #1
	bcs	.L_08002744
	sub	r7, r7, #1
	lsls	r6, r6, #1
	bcs	.L_08002744
	sub	r7, r7, #1
	lsls	r6, r6, #1
	bcs	.L_08002744
	sub	r7, r7, #1
	lsls	r6, r6, #1
	bcs	.L_08002744
	sub	r7, r7, #1
	lsls	r6, r6, #1
	bcs	.L_08002744
	sub	r7, r7, #1
	lsls	r6, r6, #1
	bcs	.L_08002744
	sub	r7, r7, #1
.L_08002744:
	mov	r6, #1
	rsb	r6, r6, r6, lsl r7
	and	r6, r2, r6
	add	r6, r6, #32
	lsr	r2, r2, r7
	sub	r4, r4, #1
	subs	r4, r4, r7
	b	.L_08002664
.L_08002764:
	sub	lr, pc, #284
.L_08002768:
	ldrh	r3, [r0], #2
	add	r4, r4, #16
	orr	r2, r2, r3, lsl r4
	bx	lr
.L_08002778:
	sub	r9, r9, #33
.L_0800277c:
	sub	r0, r1, r9
	pop	{r5, r6, r7, r9, lr}
	bx	lr
.L_08002788:
	cmp	r2, #1
	beq	.L_08002798
	sub	r9, r1, #4
	b	.L_0800277c
.L_08002798:
	mov	r9, r1
.L_0800279c:
	ldrb	lr, [r0], #1
	mov	r3, #16777216
	orrs	lr, r3, lr, lsl #25
	bcs	.L_080027c0
.L_080027ac:
	ldrb	r3, [r0], #1
	strb	r3, [r1], #1
.L_080027b4:
	lsls	lr, lr, #1
	bcc	.L_080027ac
	beq	.L_0800279c
.L_080027c0:
	ldrb	r3, [r0], #1
	ldrb	r4, [r0], #1
	and	r2, r3, #240
	orr	r2, r4, r2, lsl #4
	ands	r3, r3, #15
	beq	.L_080027f4
.L_080027d8:
	ldrb	r4, [r1, -r2]
	strb	r4, [r1], #1
	subs	r3, r3, #1
	bne	.L_080027d8
	ldrb	r4, [r1, -r2]
	strb	r4, [r1], #1
	b	.L_080027b4
.L_080027f4:
	movs	r2, r2
	beq	.L_0800277c
	ldrb	r3, [r0], #1
	add	r3, r3, #16
	b	.L_080027d8
	.global Resource_DecodeType01CodeSize
	.set Resource_DecodeType01CodeSize, . - Resource_DecodeType01Arm
.syntax unified
	.arm
	.global Resource_Decode2
Resource_Decode2:
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
	blmi	Resource_RefillType2Bits
	b	Resource_DecodeType2Loop
.syntax unified
	.section .rodata,"a",%progbits
	.arm
.L_08002848:
	.4byte 0xfedcba98
	.4byte 0x76543210
.syntax unified
	.section .text.decode_2_loop,"ax",%progbits
	.arm
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
	blmi	Resource_RefillType2Bits
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
	blmi	Resource_RefillType2Bits
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
	blmi	Resource_RefillType2Bits
.L_08002908:
	strb	r5, [r1, #1]!
	pop	{r5, r9}
	.global Resource_DecodeType2Loop
Resource_DecodeType2Loop:
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
	blmi	Resource_RefillType2Bits
	ldrb	r7, [r1, -r6]
	strb	r7, [r1, #1]!
.L_080029f4:
	ldrb	r7, [r1, -r6]
	strb	r7, [r1, #1]!
	subs	r5, r5, #1
	beq	Resource_DecodeType2Loop
	ldrb	r7, [r1, -r6]
	strb	r7, [r1, #1]!
	subs	r5, r5, #1
	bne	.L_080029f4
	b	Resource_DecodeType2Loop
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
	.global Resource_RefillType2Bits
Resource_RefillType2Bits:
	ldrh	r3, [r0], #2
	add	r4, r4, #16
	orr	r2, r2, r3, lsl r4
	bx	lr
.L_08002af8:
	sub	r9, r9, #33
	sub	r0, r1, r9
	pop	{r5, r6, r7, r8, r9, sl, fp, lr}
	bx	lr
.L_08002b08:
	and	r8, ip, fp
	bx	lr
AlchemyUncredited_08002b10:
	nop
	nop
	nop
	nop
	nop
	nop
.L_08002b28:
AlchemyUncreditedEnd_08002b10:
	lsr	r9, fp, #8
	orr	r9, r9, fp, lsl #28
	and	r8, ip, fp, lsr #4
	orr	fp, r8, r9, ror #24
	bx	lr
AlchemyUncredited_08002b3c:
	nop
	nop
	nop
.L_08002b48:
AlchemyUncreditedEnd_08002b3c:
	lsr	r9, fp, #12
	orr	r9, r9, fp, lsl #24
	and	r8, ip, fp, lsr #8
	orr	fp, r8, r9, ror #20
	bx	lr
AlchemyUncredited_08002b5c:
	nop
	nop
	nop
.L_08002b68:
AlchemyUncreditedEnd_08002b5c:
	lsr	r9, fp, #16
	orr	r9, r9, fp, lsl #20
	and	r8, ip, fp, lsr #12
	orr	fp, r8, r9, ror #16
	bx	lr
AlchemyUncredited_08002b7c:
	nop
	nop
	nop
.L_08002b88:
AlchemyUncreditedEnd_08002b7c:
	lsr	r9, fp, #20
	orr	r9, r9, fp, lsl #16
	and	r8, ip, fp, lsr #16
	orr	fp, r8, r9, ror #12
	bx	lr
AlchemyUncredited_08002b9c:
	nop
	nop
	nop
.L_08002ba8:
AlchemyUncreditedEnd_08002b9c:
	lsr	r9, fp, #24
	orr	r9, r9, fp, lsl #12
	and	r8, ip, fp, lsr #20
	orr	fp, r8, r9, ror #8
	bx	lr
AlchemyUncredited_08002bbc:
	nop
	nop
	nop
.L_08002bc8:
AlchemyUncreditedEnd_08002bbc:
	lsr	r9, fp, #28
	orr	r9, r9, fp, lsl #8
	and	r8, ip, fp, lsr #24
	orr	fp, r8, r9, ror #4
	bx	lr
AlchemyUncredited_08002bdc:
	nop
	nop
	nop
.L_08002be8:
AlchemyUncreditedEnd_08002bdc:
	ror	fp, fp, #28
	and	r8, ip, fp
	bx	lr
AlchemyUncredited_08002bf4:
	nop
	nop
	nop
	nop
	nop
.L_08002c08:
AlchemyUncreditedEnd_08002bf4:
	and	r8, ip, sl
	bic	r9, sl, #15
.L_08002c10:
	orr	sl, r9, fp, lsr #28
	orr	fp, r8, fp, lsl #4
	bx	lr
AlchemyUncredited_08002c1c:
	nop
	nop
	nop
.L_08002c28:
AlchemyUncreditedEnd_08002c1c:
	and	r8, ip, sl, lsr #4
	lsr	r9, sl, #8
	orr	r9, r9, sl, lsl #28
	ror	r9, r9, #24
	b	.L_08002c10
AlchemyUncredited_08002c3c:
	nop
	nop
	nop
.L_08002c48:
AlchemyUncreditedEnd_08002c3c:
	and	r8, ip, sl, lsr #8
	lsr	r9, sl, #12
	orr	r9, r9, sl, lsl #24
	ror	r9, r9, #20
	b	.L_08002c10
AlchemyUncredited_08002c5c:
	nop
	nop
	nop
.L_08002c68:
AlchemyUncreditedEnd_08002c5c:
	and	r8, ip, sl, lsr #12
	lsr	r9, sl, #16
	orr	r9, r9, sl, lsl #20
	ror	r9, r9, #16
	b	.L_08002c10
AlchemyUncredited_08002c7c:
	nop
	nop
	nop
.L_08002c88:
AlchemyUncreditedEnd_08002c7c:
	and	r8, ip, sl, lsr #16
	lsr	r9, sl, #20
	orr	r9, r9, sl, lsl #16
	ror	r9, r9, #12
	b	.L_08002c10
AlchemyUncredited_08002c9c:
	nop
	nop
	nop
.L_08002ca8:
AlchemyUncreditedEnd_08002c9c:
	and	r8, ip, sl, lsr #20
	lsr	r9, sl, #24
	orr	r9, r9, sl, lsl #12
	ror	r9, r9, #8
	b	.L_08002c10
AlchemyUncredited_08002cbc:
	nop
	nop
	nop
.L_08002cc8:
AlchemyUncreditedEnd_08002cbc:
	and	r8, ip, sl, lsr #24
	lsr	r9, sl, #28
	orr	r9, r9, sl, lsl #8
	ror	r9, r9, #4
	b	.L_08002c10
AlchemyUncredited_08002cdc:
	nop
	nop
	nop
.L_08002ce8:
AlchemyUncreditedEnd_08002cdc:
	and	r8, ip, sl, lsr #28
	lsl	r9, sl, #4
	b	.L_08002c10
.syntax unified
	.arm
	.global Resource_ExecuteTransfer
Resource_ExecuteTransfer:
	stmfd	sp!, {r5}
	add	r0, r0, #4
.L_08002cfc:
	ldm	r0!, {r2, r3, r4}
	ands	ip, r4, #196608
	beq	.L_08002d4c
	tst	r4, #786432
	beq	.L_08002d2c
	cmp	ip, #131072
	ldrbcc	r5, [r3]
	ldrheq	r5, [r3]
	ldrgt	r5, [r3]
	tst	r4, #262144
	orrne	r2, r5, r2
	biceq	r2, r5, r2
.L_08002d2c:
	cmp	ip, #131072
	strbcc	r2, [r3]
	strheq	r2, [r3]
	strgt	r2, [r3]
.L_08002d3c:
	subs	r1, r1, #1
	bgt	.L_08002cfc
	ldmfd	sp!, {r5}
	bx	lr
.L_08002d4c:
	mov	ip, #67108864
	add	ip, ip, #212
	stm	ip, {r2, r3, r4}
	b	.L_08002d3c
	.global TransferCommandExecutor_Size
	.set TransferCommandExecutor_Size, . - Resource_ExecuteTransfer
.syntax unified
	.arm
	.global Resource_PatchThumbBranch
Resource_PatchThumbBranch:
	push	{r5, r6}
	add	r1, r1, r1
	add	r2, r0, r1
	add	r0, r0, #2
	mov	r6, r0
	mov	r4, #31
	mov	r5, #30
.L_08002d78:
	cmp	r0, r2
	bcs	.L_08002dd0
	ldrh	ip, [r0], #2
	cmp	r4, ip, lsr #11
	bne	.L_08002d78
	ldrh	r3, [r0, #-4]
	cmp	r5, r3, lsr #11
	bne	.L_08002d78
	bic	ip, ip, #63488
	bic	r3, r3, #63488
	orr	ip, ip, r3, lsl #11
	lsl	ip, ip, #1
	sub	r3, r0, r6
	sub	ip, ip, r3
	lsr	r3, ip, #12
	bic	r3, r3, #63488
	orr	r3, r3, #61440
	lsr	ip, ip, #1
	orr	ip, ip, #63488
	strh	r3, [r0, #-4]
	strh	ip, [r0, #-2]
	b	.L_08002d78
.L_08002dd0:
	pop	{r5, r6}
	bx	lr
	.global Resource_PatchThumbBranchCodeSize
	.set Resource_PatchThumbBranchCodeSize, . - Resource_PatchThumbBranch
