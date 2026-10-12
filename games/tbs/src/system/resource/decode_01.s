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
	bne	.Lnot_type0
	sub	r1, r1, #1
	add	r9, r1, #33
	ands	r2, r0, #1
	lsl	r4, r2, #3
	ldrbne	r2, [r0], #1
	subs	r4, r4, #16
	blmi	.Ltype0_refill
	b	.Ltype0_next
.Ltype0_literal:
	strb	r2, [r1, #1]!
	lsr	r2, r2, #8
	subs	r4, r4, #9
	ldrhmi	r3, [r0], #2
	addmi	r4, r4, #16
	orrmi	r2, r2, r3, lsl r4
.Ltype0_next:
	lsrs	r2, r2, #1
	bcs	.Ltype0_literal
	lsrs	r2, r2, #1
	bcc	.Ltype0_length_2
	lsrs	r2, r2, #1
	bcc	.Ltype0_length_3
	lsrs	r2, r2, #1
	bcc	.Ltype0_length_4
	lsrs	r2, r2, #1
	bcc	.Ltype0_length_5
	lsrs	r2, r2, #1
	bcs	.Ltype0_length_8_to_10
	lsrs	r2, r2, #1
	bcc	.Ltype0_length_6
	subs	r4, r4, #7
	mov	r5, #7
	bpl	.Ltype0_distance
	blmi	.Ltype0_refill_to_distance
.Ltype0_length_8_to_10:
	ands	r5, r2, #3
	lsr	r2, r2, #2
	beq	.Ltype0_length_11_up
	add	r5, r5, #7
	subs	r4, r4, #8
	bpl	.Ltype0_distance
	blmi	.Ltype0_refill_to_distance
.Ltype0_length_11_up:
	ands	r5, r2, #127
	beq	.Ltype0_end
	add	r5, r5, #10
	lsr	r2, r2, #7
	subs	r4, r4, #15
	bpl	.Ltype0_distance
	blmi	.Ltype0_refill_to_distance
.Ltype0_length_3:
	subs	r4, r4, #3
	mov	r5, #3
	b	.Ltype0_distance
.Ltype0_length_4:
	subs	r4, r4, #4
	mov	r5, #4
	bpl	.Ltype0_distance
	blmi	.Ltype0_refill_to_distance
.Ltype0_length_5:
	subs	r4, r4, #5
	mov	r5, #5
	bpl	.Ltype0_distance
	blmi	.Ltype0_refill_to_distance
.Ltype0_length_6:
	subs	r4, r4, #7
	mov	r5, #6
	bpl	.Ltype0_distance
	blmi	.Ltype0_refill_to_distance
.Ltype0_length_2:
	subs	r4, r4, #2
	mov	r5, #2
.Ltype0_distance:
	lsrs	r2, r2, #1
	bcc	.Ltype0_distance_long
	and	r6, r2, #31
	subs	r4, r4, #6
	lsr	r2, r2, #5
.Ltype0_copy:
	ldrhmi	r3, [r0], #2
	addmi	r4, r4, #16
	orrmi	r2, r2, r3, lsl r4
	lsrs	r5, r5, #1
	ldrbcs	r7, [r1, -r6]
	strbcs	r7, [r1, #1]!
.Ltype0_copy_pair:
	ldrb	r7, [r1, -r6]
	strb	r7, [r1, #1]!
	ldrb	r7, [r1, -r6]
	strb	r7, [r1, #1]!
	subs	r5, r5, #1
	bne	.Ltype0_copy_pair
	b	.Ltype0_next
.Ltype0_distance_long:
	sub	r6, r1, r9
	cmp	r6, #2048
	bcc	.Ltype0_distance_bits
	ror	r6, r2, #12
	lsr	r2, r2, #12
	lsr	r6, r6, #20
	add	r6, r6, #32
	subs	r4, r4, #13
	b	.Ltype0_copy
.Ltype0_distance_bits:
	mov	r7, #11
	lsls	r6, r6, #22
	bcs	.Ltype0_distance_read
	sub	r7, r7, #1
	lsls	r6, r6, #1
	bcs	.Ltype0_distance_read
	sub	r7, r7, #1
	lsls	r6, r6, #1
	bcs	.Ltype0_distance_read
	sub	r7, r7, #1
	lsls	r6, r6, #1
	bcs	.Ltype0_distance_read
	sub	r7, r7, #1
	lsls	r6, r6, #1
	bcs	.Ltype0_distance_read
	sub	r7, r7, #1
	lsls	r6, r6, #1
	bcs	.Ltype0_distance_read
	sub	r7, r7, #1
	lsls	r6, r6, #1
	bcs	.Ltype0_distance_read
	sub	r7, r7, #1
	lsls	r6, r6, #1
	bcs	.Ltype0_distance_read
	sub	r7, r7, #1
	lsls	r6, r6, #1
	bcs	.Ltype0_distance_read
	sub	r7, r7, #1
	lsls	r6, r6, #1
	bcs	.Ltype0_distance_read
	sub	r7, r7, #1
	lsls	r6, r6, #1
	bcs	.Ltype0_distance_read
	sub	r7, r7, #1
.Ltype0_distance_read:
	mov	r6, #1
	rsb	r6, r6, r6, lsl r7
	and	r6, r2, r6
	add	r6, r6, #32
	lsr	r2, r2, r7
	sub	r4, r4, #1
	subs	r4, r4, r7
	b	.Ltype0_copy
.Ltype0_refill_to_distance:
	sub	lr, pc, #284
.Ltype0_refill:
	ldrh	r3, [r0], #2
	add	r4, r4, #16
	orr	r2, r2, r3, lsl r4
	bx	lr
.Ltype0_end:
	sub	r9, r9, #33
.Ldecode_done:
	sub	r0, r1, r9
	pop	{r5, r6, r7, r9, lr}
	bx	lr
.Lnot_type0:
	cmp	r2, #1
	beq	.Ltype1
	sub	r9, r1, #4
	b	.Ldecode_done
.Ltype1:
	mov	r9, r1
.Ltype1_flags:
	ldrb	lr, [r0], #1
	mov	r3, #16777216
	orrs	lr, r3, lr, lsl #25
	bcs	.Ltype1_match
.Ltype1_literal:
	ldrb	r3, [r0], #1
	strb	r3, [r1], #1
.Ltype1_next_flag:
	lsls	lr, lr, #1
	bcc	.Ltype1_literal
	beq	.Ltype1_flags
.Ltype1_match:
	ldrb	r3, [r0], #1
	ldrb	r4, [r0], #1
	and	r2, r3, #240
	orr	r2, r4, r2, lsl #4
	ands	r3, r3, #15
	beq	.Ltype1_match_long
.Ltype1_copy:
	ldrb	r4, [r1, -r2]
	strb	r4, [r1], #1
	subs	r3, r3, #1
	bne	.Ltype1_copy
	ldrb	r4, [r1, -r2]
	strb	r4, [r1], #1
	b	.Ltype1_next_flag
.Ltype1_match_long:
	movs	r2, r2
	beq	.Ldecode_done
	ldrb	r3, [r0], #1
	add	r3, r3, #16
	b	.Ltype1_copy
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
.Ltype2_nibble_list:
	.4byte 0xfedcba98
	.4byte 0x76543210
.syntax unified
	.section .text.decode_2_loop,"ax",%progbits
	.arm
.Ltype2_literal_4bit:
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
	b	.Ltype2_literal_store
.Ltype2_literal_3bit:
	lsrs	r2, r2, #1
	bcc	.Ltype2_literal_4bit
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
	b	.Ltype2_literal_store
.Ltype2_literal:
	push	{r5, r9}
	lsrs	r2, r2, #1
	bcc	.Ltype2_literal_3bit
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
.Ltype2_literal_store:
	strb	r5, [r1, #1]!
	pop	{r5, r9}
	.global Resource_DecodeType2Loop
Resource_DecodeType2Loop:
	lsrs	r2, r2, #1
	bcs	.Ltype2_literal
	lsrs	r2, r2, #1
	bcc	.Ltype2_length_2
	lsrs	r2, r2, #1
	bcc	.Ltype2_length_3
	lsrs	r2, r2, #1
	bcc	.Ltype2_length_4
	lsrs	r2, r2, #1
	bcc	.Ltype2_length_5
	lsrs	r2, r2, #1
	bcs	.Ltype2_length_8_to_10
	lsrs	r2, r2, #1
	bcc	.Ltype2_length_6
	subs	r4, r4, #7
	mov	r5, #6
	bpl	.Ltype2_distance
	blmi	.Ltype2_refill_to_distance
.Ltype2_length_8_to_10:
	ands	r5, r2, #3
	lsr	r2, r2, #2
	beq	.Ltype2_length_11_up
	add	r5, r5, #6
	subs	r4, r4, #8
	bpl	.Ltype2_distance
	blmi	.Ltype2_refill_to_distance
.Ltype2_length_11_up:
	ands	r5, r2, #127
	beq	.Ltype2_end
	add	r5, r5, #9
	lsr	r2, r2, #7
	subs	r4, r4, #15
	bpl	.Ltype2_distance
	blmi	.Ltype2_refill_to_distance
.Ltype2_length_3:
	subs	r4, r4, #3
	mov	r5, #2
	b	.Ltype2_distance
.Ltype2_length_4:
	subs	r4, r4, #4
	mov	r5, #3
	bpl	.Ltype2_distance
	blmi	.Ltype2_refill_to_distance
.Ltype2_length_5:
	subs	r4, r4, #5
	mov	r5, #4
	bpl	.Ltype2_distance
	blmi	.Ltype2_refill_to_distance
.Ltype2_length_6:
	subs	r4, r4, #7
	mov	r5, #5
	bpl	.Ltype2_distance
	blmi	.Ltype2_refill_to_distance
.Ltype2_length_2:
	subs	r4, r4, #2
	mov	r5, #1
.Ltype2_distance:
	lsrs	r2, r2, #1
	bcc	.Ltype2_distance_long
	and	r6, r2, #31
	subs	r4, r4, #6
	lsr	r2, r2, #5
.Ltype2_copy:
	blmi	Resource_RefillType2Bits
	ldrb	r7, [r1, -r6]
	strb	r7, [r1, #1]!
.Ltype2_copy_byte:
	ldrb	r7, [r1, -r6]
	strb	r7, [r1, #1]!
	subs	r5, r5, #1
	beq	Resource_DecodeType2Loop
	ldrb	r7, [r1, -r6]
	strb	r7, [r1, #1]!
	subs	r5, r5, #1
	bne	.Ltype2_copy_byte
	b	Resource_DecodeType2Loop
.Ltype2_distance_long:
	sub	r6, r1, r9
	cmp	r6, #2048
	bcc	.Ltype2_distance_bits
	ror	r6, r2, #12
	lsr	r2, r2, #12
	lsr	r6, r6, #20
	add	r6, r6, #32
	subs	r4, r4, #13
	b	.Ltype2_copy
.Ltype2_distance_bits:
	mov	r7, #11
	lsls	r6, r6, #22
	bcs	.Ltype2_distance_read
	sub	r7, r7, #1
	lsls	r6, r6, #1
	bcs	.Ltype2_distance_read
	sub	r7, r7, #1
	lsls	r6, r6, #1
	bcs	.Ltype2_distance_read
	sub	r7, r7, #1
	lsls	r6, r6, #1
	bcs	.Ltype2_distance_read
	sub	r7, r7, #1
	lsls	r6, r6, #1
	bcs	.Ltype2_distance_read
	sub	r7, r7, #1
	lsls	r6, r6, #1
	bcs	.Ltype2_distance_read
	sub	r7, r7, #1
	lsls	r6, r6, #1
	bcs	.Ltype2_distance_read
	sub	r7, r7, #1
	lsls	r6, r6, #1
	bcs	.Ltype2_distance_read
	sub	r7, r7, #1
	lsls	r6, r6, #1
	bcs	.Ltype2_distance_read
	sub	r7, r7, #1
	lsls	r6, r6, #1
	bcs	.Ltype2_distance_read
	sub	r7, r7, #1
	lsls	r6, r6, #1
	bcs	.Ltype2_distance_read
	sub	r7, r7, #1
.Ltype2_distance_read:
	mov	r6, #1
	rsb	r6, r6, r6, lsl r7
	and	r6, r2, r6
	add	r6, r6, #32
	lsr	r2, r2, r7
	sub	r4, r4, #1
	subs	r4, r4, r7
	b	.Ltype2_copy
.Ltype2_refill_to_distance:
	sub	lr, pc, #280
	.global Resource_RefillType2Bits
Resource_RefillType2Bits:
	ldrh	r3, [r0], #2
	add	r4, r4, #16
	orr	r2, r2, r3, lsl r4
	bx	lr
.Ltype2_end:
	sub	r9, r9, #33
	sub	r0, r1, r9
	pop	{r5, r6, r7, r8, r9, sl, fp, lr}
	bx	lr
.Ltake_0:
	and	r8, ip, fp
	bx	lr
AlchemyUncredited_decode_01_1:
	nop
	nop
	nop
	nop
	nop
	nop
.Ltake_1:
AlchemyUncreditedEnd_decode_01_1:
	lsr	r9, fp, #8
	orr	r9, r9, fp, lsl #28
	and	r8, ip, fp, lsr #4
	orr	fp, r8, r9, ror #24
	bx	lr
AlchemyUncredited_decode_01_2:
	nop
	nop
	nop
.Ltake_2:
AlchemyUncreditedEnd_decode_01_2:
	lsr	r9, fp, #12
	orr	r9, r9, fp, lsl #24
	and	r8, ip, fp, lsr #8
	orr	fp, r8, r9, ror #20
	bx	lr
AlchemyUncredited_decode_01_3:
	nop
	nop
	nop
.Ltake_3:
AlchemyUncreditedEnd_decode_01_3:
	lsr	r9, fp, #16
	orr	r9, r9, fp, lsl #20
	and	r8, ip, fp, lsr #12
	orr	fp, r8, r9, ror #16
	bx	lr
AlchemyUncredited_decode_01_4:
	nop
	nop
	nop
.Ltake_4:
AlchemyUncreditedEnd_decode_01_4:
	lsr	r9, fp, #20
	orr	r9, r9, fp, lsl #16
	and	r8, ip, fp, lsr #16
	orr	fp, r8, r9, ror #12
	bx	lr
AlchemyUncredited_decode_01_5:
	nop
	nop
	nop
.Ltake_5:
AlchemyUncreditedEnd_decode_01_5:
	lsr	r9, fp, #24
	orr	r9, r9, fp, lsl #12
	and	r8, ip, fp, lsr #20
	orr	fp, r8, r9, ror #8
	bx	lr
AlchemyUncredited_decode_01_6:
	nop
	nop
	nop
.Ltake_6:
AlchemyUncreditedEnd_decode_01_6:
	lsr	r9, fp, #28
	orr	r9, r9, fp, lsl #8
	and	r8, ip, fp, lsr #24
	orr	fp, r8, r9, ror #4
	bx	lr
AlchemyUncredited_decode_01_7:
	nop
	nop
	nop
.Ltake_7:
AlchemyUncreditedEnd_decode_01_7:
	ror	fp, fp, #28
	and	r8, ip, fp
	bx	lr
AlchemyUncredited_decode_01_8:
	nop
	nop
	nop
	nop
	nop
.Ltake_8:
AlchemyUncreditedEnd_decode_01_8:
	and	r8, ip, sl
	bic	r9, sl, #15
.Lpush_front:
	orr	sl, r9, fp, lsr #28
	orr	fp, r8, fp, lsl #4
	bx	lr
AlchemyUncredited_decode_01_9:
	nop
	nop
	nop
.Ltake_9:
AlchemyUncreditedEnd_decode_01_9:
	and	r8, ip, sl, lsr #4
	lsr	r9, sl, #8
	orr	r9, r9, sl, lsl #28
	ror	r9, r9, #24
	b	.Lpush_front
AlchemyUncredited_decode_01_10:
	nop
	nop
	nop
.Ltake_10:
AlchemyUncreditedEnd_decode_01_10:
	and	r8, ip, sl, lsr #8
	lsr	r9, sl, #12
	orr	r9, r9, sl, lsl #24
	ror	r9, r9, #20
	b	.Lpush_front
AlchemyUncredited_decode_01_11:
	nop
	nop
	nop
.Ltake_11:
AlchemyUncreditedEnd_decode_01_11:
	and	r8, ip, sl, lsr #12
	lsr	r9, sl, #16
	orr	r9, r9, sl, lsl #20
	ror	r9, r9, #16
	b	.Lpush_front
AlchemyUncredited_decode_01_12:
	nop
	nop
	nop
.Ltake_12:
AlchemyUncreditedEnd_decode_01_12:
	and	r8, ip, sl, lsr #16
	lsr	r9, sl, #20
	orr	r9, r9, sl, lsl #16
	ror	r9, r9, #12
	b	.Lpush_front
AlchemyUncredited_decode_01_13:
	nop
	nop
	nop
.Ltake_13:
AlchemyUncreditedEnd_decode_01_13:
	and	r8, ip, sl, lsr #20
	lsr	r9, sl, #24
	orr	r9, r9, sl, lsl #12
	ror	r9, r9, #8
	b	.Lpush_front
AlchemyUncredited_decode_01_14:
	nop
	nop
	nop
.Ltake_14:
AlchemyUncreditedEnd_decode_01_14:
	and	r8, ip, sl, lsr #24
	lsr	r9, sl, #28
	orr	r9, r9, sl, lsl #8
	ror	r9, r9, #4
	b	.Lpush_front
AlchemyUncredited_decode_01_15:
	nop
	nop
	nop
.Ltake_15:
AlchemyUncreditedEnd_decode_01_15:
	and	r8, ip, sl, lsr #28
	lsl	r9, sl, #4
	b	.Lpush_front
.syntax unified
	.arm
	.global Resource_ExecuteTransfer
Resource_ExecuteTransfer:
	stmfd	sp!, {r5}
	add	r0, r0, #4
.Ltransfer_next:
	ldm	r0!, {r2, r3, r4}
	ands	ip, r4, #196608
	beq	.Ltransfer_dma
	tst	r4, #786432
	beq	.Ltransfer_store
	cmp	ip, #131072
	ldrbcc	r5, [r3]
	ldrheq	r5, [r3]
	ldrgt	r5, [r3]
	tst	r4, #262144
	orrne	r2, r5, r2
	biceq	r2, r5, r2
.Ltransfer_store:
	cmp	ip, #131072
	strbcc	r2, [r3]
	strheq	r2, [r3]
	strgt	r2, [r3]
.Ltransfer_count:
	subs	r1, r1, #1
	bgt	.Ltransfer_next
	ldmfd	sp!, {r5}
	bx	lr
.Ltransfer_dma:
	mov	ip, #67108864
	add	ip, ip, #212
	stm	ip, {r2, r3, r4}
	b	.Ltransfer_count
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
.Lpatch_scan:
	cmp	r0, r2
	bcs	.Lpatch_done
	ldrh	ip, [r0], #2
	cmp	r4, ip, lsr #11
	bne	.Lpatch_scan
	ldrh	r3, [r0, #-4]
	cmp	r5, r3, lsr #11
	bne	.Lpatch_scan
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
	b	.Lpatch_scan
.Lpatch_done:
	pop	{r5, r6}
	bx	lr
	.global Resource_PatchThumbBranchCodeSize
	.set Resource_PatchThumbBranchCodeSize, . - Resource_PatchThumbBranch
