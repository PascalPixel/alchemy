@ Why assembly: the bit reader branches on carry immediately after shifting
@ its live bit reservoir (lsrs r2, r2, #1 / bcs); it also uses unshifted
@ movs r2, r2 for copy-and-test. Its Thumb entry's bx pc switches to the ARM code that follows, a mode switch inside a function;
@ the halfword after bx pc is never run.
@ Why assembly: add ip, pc, #36 followed by ldm ip, {sl, fp} loads a data block
@ relative to pc, including r11/fp as ordinary decoder state. Its Thumb
@ entry's bx pc switches to the ARM code that follows, a mode switch inside a function;
@ the halfword after bx pc is never run.
@ Why syDecompressMoveToFrontLZLoop to Resource_RefillMoveToFrontLzBits are assembly:
@ computed add pc (add pc, r9, r8, lsl #5) enters fixed-size 32-byte decoder
@ code slots; command execution also carries live reader register state
@ across entry labels. push {r5, r9} saves without lr, and r11 (fp) is an
@ ordinary register: with sl it holds the packed nibble list the slots
@ rotate (lsr r9, fp, #8; orr fp, r8, r9, ror #24), not a frame pointer.
@ Why assembly: stmfd sp! saves callee-saved r5 without lr. Its Thumb entry's
@ bx pc switches to the ARM code that follows, a mode switch inside a function;
@ the halfword after bx pc is never run.
@ Why assembly: push saves callee-saved r5/r6 without lr; body also edits
@ branch instructions. Its Thumb entry's bx pc switches to the ARM code that follows, a mode switch inside a function;
@ the halfword after bx pc is never run.
	.section .text,"ax",%progbits
	.balign 4
.syntax unified
	.thumb
	.global syDecompressTaggedLZArmCode
syDecompressTaggedLZArmCode:
	.global syDecompressTaggedLZArm
	.thumb_func
syDecompressTaggedLZArm:
	bx	pc
AlchemyUncredited_080128aa:
	.2byte	0x0200
	.arm
AlchemyUncreditedEnd_080128aa:
	push	{r5, r6, r7, r9, lr}
	ldrb	r2, [r0], #1
	cmp	r2, #0
	bne	.L_08012af0
	sub	r1, r1, #1
	add	r9, r1, #33
	ands	r2, r0, #1
	lsl	r4, r2, #3
	ldrbne	r2, [r0], #1
	subs	r4, r4, #16
	blmi	.L_08012ad0
	b	.L_080128f4
.L_080128dc:
	strb	r2, [r1, #1]!
	lsr	r2, r2, #8
	subs	r4, r4, #9
	ldrhmi	r3, [r0], #2
	addmi	r4, r4, #16
	orrmi	r2, r2, r3, lsl r4
.L_080128f4:
	lsrs	r2, r2, #1
	bcs	.L_080128dc
	lsrs	r2, r2, #1
	bcc	.L_080129b0
	lsrs	r2, r2, #1
	bcc	.L_08012974
	lsrs	r2, r2, #1
	bcc	.L_08012980
	lsrs	r2, r2, #1
	bcc	.L_08012990
	lsrs	r2, r2, #1
	bcs	.L_0801293c
	lsrs	r2, r2, #1
	bcc	.L_080129a0
	subs	r4, r4, #7
	mov	r5, #7
	bpl	.L_080129b8
	blmi	.L_08012acc
.L_0801293c:
	ands	r5, r2, #3
	lsr	r2, r2, #2
	beq	.L_08012958
	add	r5, r5, #7
	subs	r4, r4, #8
	bpl	.L_080129b8
	blmi	.L_08012acc
.L_08012958:
	ands	r5, r2, #127
	beq	.L_08012ae0
	add	r5, r5, #10
	lsr	r2, r2, #7
	subs	r4, r4, #15
	bpl	.L_080129b8
	blmi	.L_08012acc
.L_08012974:
	subs	r4, r4, #3
	mov	r5, #3
	b	.L_080129b8
.L_08012980:
	subs	r4, r4, #4
	mov	r5, #4
	bpl	.L_080129b8
	blmi	.L_08012acc
.L_08012990:
	subs	r4, r4, #5
	mov	r5, #5
	bpl	.L_080129b8
	blmi	.L_08012acc
.L_080129a0:
	subs	r4, r4, #7
	mov	r5, #6
	bpl	.L_080129b8
	blmi	.L_08012acc
.L_080129b0:
	subs	r4, r4, #2
	mov	r5, #2
.L_080129b8:
	lsrs	r2, r2, #1
	bcc	.L_08012a00
	and	r6, r2, #31
	subs	r4, r4, #6
	lsr	r2, r2, #5
.L_080129cc:
	ldrhmi	r3, [r0], #2
	addmi	r4, r4, #16
	orrmi	r2, r2, r3, lsl r4
	lsrs	r5, r5, #1
	ldrbcs	r7, [r1, -r6]
	strbcs	r7, [r1, #1]!
.L_080129e4:
	ldrb	r7, [r1, -r6]
	strb	r7, [r1, #1]!
	ldrb	r7, [r1, -r6]
	strb	r7, [r1, #1]!
	subs	r5, r5, #1
	bne	.L_080129e4
	b	.L_080128f4
.L_08012a00:
	sub	r6, r1, r9
	cmp	r6, #0x800
	bcc	.L_08012a24
	ror	r6, r2, #12
	lsr	r2, r2, #12
	lsr	r6, r6, #20
	add	r6, r6, #32
	subs	r4, r4, #13
	b	.L_080129cc
.L_08012a24:
	mov	r7, #11
	lsls	r6, r6, #22
	bcs	.L_08012aac
	sub	r7, r7, #1
	lsls	r6, r6, #1
	bcs	.L_08012aac
	sub	r7, r7, #1
	lsls	r6, r6, #1
	bcs	.L_08012aac
	sub	r7, r7, #1
	lsls	r6, r6, #1
	bcs	.L_08012aac
	sub	r7, r7, #1
	lsls	r6, r6, #1
	bcs	.L_08012aac
	sub	r7, r7, #1
	lsls	r6, r6, #1
	bcs	.L_08012aac
	sub	r7, r7, #1
	lsls	r6, r6, #1
	bcs	.L_08012aac
	sub	r7, r7, #1
	lsls	r6, r6, #1
	bcs	.L_08012aac
	sub	r7, r7, #1
	lsls	r6, r6, #1
	bcs	.L_08012aac
	sub	r7, r7, #1
	lsls	r6, r6, #1
	bcs	.L_08012aac
	sub	r7, r7, #1
	lsls	r6, r6, #1
	bcs	.L_08012aac
	sub	r7, r7, #1
.L_08012aac:
	mov	r6, #1
	rsb	r6, r6, r6, lsl r7
	and	r6, r2, r6
	add	r6, r6, #32
	lsr	r2, r2, r7
	sub	r4, r4, #1
	subs	r4, r4, r7
	b	.L_080129cc
.L_08012acc:
	sub	lr, pc, #0x11c
.L_08012ad0:
	ldrh	r3, [r0], #2
	add	r4, r4, #16
	orr	r2, r2, r3, lsl r4
	bx	lr
.L_08012ae0:
	sub	r9, r9, #33
.L_08012ae4:
	sub	r0, r1, r9
	pop	{r5, r6, r7, r9, lr}
	bx	lr
.L_08012af0:
	cmp	r2, #1
	beq	.L_08012b00
	sub	r9, r1, #4
	b	.L_08012ae4
.L_08012b00:
	mov	r9, r1
.L_08012b04:
	ldrb	lr, [r0], #1
	mov	r3, #0x1000000
	orrs	lr, r3, lr, lsl #25
	bcs	.L_08012b28
.L_08012b14:
	ldrb	r3, [r0], #1
	strb	r3, [r1], #1
.L_08012b1c:
	lsls	lr, lr, #1
	bcc	.L_08012b14
	beq	.L_08012b04
.L_08012b28:
	ldrb	r3, [r0], #1
	ldrb	r4, [r0], #1
	and	r2, r3, #240
	orr	r2, r4, r2, lsl #4
	ands	r3, r3, #15
	beq	.L_08012b5c
.L_08012b40:
	ldrb	r4, [r1, -r2]
	strb	r4, [r1], #1
	subs	r3, r3, #1
	bne	.L_08012b40
	ldrb	r4, [r1, -r2]
	strb	r4, [r1], #1
	b	.L_08012b1c
.L_08012b5c:
	movs	r2, r2
	beq	.L_08012ae4
	ldrb	r3, [r0], #1
	add	r3, r3, #16
	b	.L_08012b40
	.global syDecompressTaggedLZCodeSize
	.set syDecompressTaggedLZCodeSize, . - syDecompressTaggedLZArmCode

	.section .text,"ax",%progbits
	.balign 4
.syntax unified
	.thumb
	.global syDecompressMoveToFrontLZCommandsCode
syDecompressMoveToFrontLZCommandsCode:
	.global syDecompressMoveToFrontLZCommands
	.thumb_func
syDecompressMoveToFrontLZCommands:
	bx	pc
AlchemyUncredited_08012b72:
	.2byte	0x0200
	.arm
AlchemyUncreditedEnd_08012b72:
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
	blmi	Resource_RefillMoveToFrontLzBits
	b	syDecompressMoveToFrontLZLoop

.syntax unified
	.section .rodata,"a",%progbits
	.arm
	.4byte	0xfedcba98
	.4byte	0x76543210

	.section .text.decode_2_loop,"ax",%progbits
	.balign 4
.syntax unified
	.arm
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
	blmi	Resource_RefillMoveToFrontLzBits
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
	blmi	Resource_RefillMoveToFrontLzBits
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
	blmi	Resource_RefillMoveToFrontLzBits
.L_08012c74:
	strb	r5, [r1, #1]!
	pop	{r5, r9}
	.global syDecompressMoveToFrontLZLoop
syDecompressMoveToFrontLZLoop:
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
	blmi	Resource_RefillMoveToFrontLzBits
	ldrb	r7, [r1, -r6]
	strb	r7, [r1, #1]!
.L_08012d60:
	ldrb	r7, [r1, -r6]
	strb	r7, [r1, #1]!
	subs	r5, r5, #1
	beq	syDecompressMoveToFrontLZLoop
	ldrb	r7, [r1, -r6]
	strb	r7, [r1, #1]!
	subs	r5, r5, #1
	bne	.L_08012d60
	b	syDecompressMoveToFrontLZLoop
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
	.global Resource_RefillMoveToFrontLzBits
Resource_RefillMoveToFrontLzBits:
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
@ nops fill each 32-byte code slot after its bx lr or b; never run.
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
	.global syDecompressMoveToFrontLZCommandsEnd
syDecompressMoveToFrontLZCommandsEnd:

	.section .text.decode_2_loop,"ax",%progbits
	.balign 4
.syntax unified
	.thumb
	.global Resource_ExecuteTransferCode
Resource_ExecuteTransferCode:
	.global Resource_ExecuteTransfer
	.thumb_func
Resource_ExecuteTransfer:
	bx	pc
AlchemyUncredited_08013062:
	.2byte	0x0200
	.arm
AlchemyUncreditedEnd_08013062:
	stmfd	sp!, {r5}
	add	r0, r0, #4
.L_0801306c:
	ldm	r0!, {r2, r3, r4}
	ands	ip, r4, #0x30000
	beq	.L_080130bc
	tst	r4, #0xc0000
	beq	.L_0801309c
	cmp	ip, #0x20000
	ldrbcc	r5, [r3]
	ldrheq	r5, [r3]
	ldrgt	r5, [r3]
	tst	r4, #0x40000
	orrne	r2, r5, r2
	biceq	r2, r5, r2
.L_0801309c:
	cmp	ip, #0x20000
	strbcc	r2, [r3]
	strheq	r2, [r3]
	strgt	r2, [r3]
.L_080130ac:
	subs	r1, r1, #1
	bgt	.L_0801306c
	ldmfd	sp!, {r5}
	bx	lr
.L_080130bc:
	mov	ip, #0x4000000
	add	ip, ip, #212
	stm	ip, {r2, r3, r4}
	b	.L_080130ac
	.global Resource_ExecuteTransferSize
	.set Resource_ExecuteTransferSize, . - Resource_ExecuteTransferCode

	.section .text.decode_2_loop,"ax",%progbits
	.balign 4
.syntax unified
	.thumb
	.global syResourcePatchThumbBranch
syResourcePatchThumbBranch:
	bx	pc
AlchemyUncredited_080130ce:
	.2byte	0x0200
	.arm
AlchemyUncreditedEnd_080130ce:
	push	{r5, r6}
	add	r1, r1, r1
	add	r2, r0, r1
	add	r0, r0, #2
	mov	r6, r0
	mov	r4, #31
	mov	r5, #30
.L_080130ec:
	cmp	r0, r2
	bcs	.L_08013144
	ldrh	ip, [r0], #2
	cmp	r4, ip, lsr #11
	bne	.L_080130ec
	ldrh	r3, [r0, #-4]
	cmp	r5, r3, lsr #11
	bne	.L_080130ec
	bic	ip, ip, #0xf800
	bic	r3, r3, #0xf800
	orr	ip, ip, r3, lsl #11
	lsl	ip, ip, #1
	sub	r3, r0, r6
	sub	ip, ip, r3
	lsr	r3, ip, #12
	bic	r3, r3, #0xf800
	orr	r3, r3, #0xf000
	lsr	ip, ip, #1
	orr	ip, ip, #0xf800
	strh	r3, [r0, #-4]
	strh	ip, [r0, #-2]
	b	.L_080130ec
.L_08013144:
	pop	{r5, r6}
	bx	lr
	.global syResourcePatchThumbBranchCodeSize
	.set syResourcePatchThumbBranchCodeSize, . - syResourcePatchThumbBranch
