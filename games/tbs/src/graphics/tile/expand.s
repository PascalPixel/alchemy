@ The command decoder Credits_DecodeBackground (dot_product.s) calls: ExpandRead
@ takes the next packed value and jumps, through Credits_DecodeBackgroundCaseTable,
@ to one of the case commands below, which update the running pixel value in
@ r5. They share the decoder's registers (r0 source, r2 and r3 read state,
@ r5 value, r11 the mask 7) and have no frames of their own.
@
@ Why assembly:
@ Why Credits_DecodeBackgroundRead is assembly:
@ Shared decoder commands reuse live r0/r2/r3, change unsaved r5 and use
@ fp as a mask. ldr pc jumps into those commands, below in this file
@ and in its .text.battle_effect_expand_commands section.
@ In this file itself: it is a two-instruction fragment of that decoder, with
@ no prologue and no return of its own, and its tell is the computed pc into
@ code. and ip, r3, #7 / ldr pc, [pc, ip, lsl #2] is unconditional and has
@ no bounds test, where the compiler's casesi always emits cmp, ldrls pc and a
@ default branch; the table it indexes starts after the nop, in the next
@ object.
@ Why Credits_DecodeBackgroundCase2 to Credits_DecodeBackgroundCase10 are assembly:
@ entries share live r0/r2/r3 decoder state with their caller; delta
@ entries update unsaved r5 and use r11/fp as a data mask.
.syntax unified
	.section .rodata,"a",%progbits
	.arm
.L_bias:
	.4byte 0x80808080
.L_dispatch_base:
	.4byte Credits_DecodeBackgroundCaseTable
.syntax unified
.text
	.arm
	.global Credits_DecodeBackgroundRead
Credits_DecodeBackgroundRead:
	and	ip, r3, #7
	ldr	pc, [pc, ip, lsl #2]
AlchemyUncredited_battle_effect_expand_1:
	nop
AlchemyUncreditedEnd_battle_effect_expand_1:
.syntax unified
	.section .data,"aw",%progbits
	.arm
.L_dispatch_runtime:
	.space 32, 0
	.global Credits_DecodeBackgroundCaseTable
Credits_DecodeBackgroundCaseTable:
	.4byte Credits_DecodeBackgroundCase2
	.4byte Credits_DecodeBackgroundCase4Plus
	.4byte Credits_DecodeBackgroundCase8
	.4byte Credits_DecodeBackgroundCase6Plus
	.4byte Credits_DecodeBackgroundCase2
	.4byte Credits_DecodeBackgroundCase4Minus
	.4byte Credits_DecodeBackgroundCase10
	.4byte Credits_DecodeBackgroundCase6Minus
.syntax unified
	.section .text.battle_effect_expand_commands,"ax",%progbits
	.arm
	.global Credits_DecodeBackgroundCase2
Credits_DecodeBackgroundCase2:
	lsr	r3, r3, #2
	subs	r2, r2, #2
	movpl	pc, lr
	ldrh	ip, [r0], #2
	add	r2, r2, #16
	orr	r3, r3, ip, lsl r2
	mov	pc, lr
	.global Credits_DecodeBackgroundCase4Plus
Credits_DecodeBackgroundCase4Plus:
	and	ip, r3, #8
	lsrs	r3, r3, #4
	add	ip, ip, #8
	add	r5, r5, ip, lsl #22
	subs	r2, r2, #4
	movpl	pc, lr
	ldrh	ip, [r0], #2
	add	r2, r2, #16
	orr	r3, r3, ip, lsl r2
	mov	pc, lr
	.global Credits_DecodeBackgroundCase4Minus
Credits_DecodeBackgroundCase4Minus:
	and	ip, r3, #8
	lsrs	r3, r3, #4
	add	ip, ip, #8
	sub	r5, r5, ip, lsl #22
	subs	r2, r2, #4
	movpl	pc, lr
	ldrh	ip, [r0], #2
	add	r2, r2, #16
	orr	r3, r3, ip, lsl r2
	mov	pc, lr
	.global Credits_DecodeBackgroundCase6Plus
Credits_DecodeBackgroundCase6Plus:
	and	ip, fp, r3, lsr #3
	lsr	r3, r3, #6
	add	ip, ip, #3
	add	r5, r5, ip, lsl #25
	subs	r2, r2, #6
	movpl	pc, lr
	ldrh	ip, [r0], #2
	add	r2, r2, #16
	orr	r3, r3, ip, lsl r2
	mov	pc, lr
	.global Credits_DecodeBackgroundCase6Minus
Credits_DecodeBackgroundCase6Minus:
	and	ip, fp, r3, lsr #3
	lsr	r3, r3, #6
	add	ip, ip, #3
	sub	r5, r5, ip, lsl #25
	subs	r2, r2, #6
	movpl	pc, lr
	ldrh	ip, [r0], #2
	add	r2, r2, #16
	orr	r3, r3, ip, lsl r2
	mov	pc, lr
	.global Credits_DecodeBackgroundCase8
Credits_DecodeBackgroundCase8:
	mov	ip, #15
	ands	ip, ip, r3, lsr #4
	add	ip, ip, #11
	rsbcs	ip, ip, #0
	add	r5, r5, ip, lsl #25
	lsr	r3, r3, #8
	subs	r2, r2, #8
	movpl	pc, lr
	ldrh	ip, [r0], #2
	add	r2, r2, #16
	orr	r3, r3, ip, lsl r2
	mov	pc, lr
	.global Credits_DecodeBackgroundCase10
Credits_DecodeBackgroundCase10:
	ror	r5, r3, #10
	and	r5, r5, #-33554432
	lsr	r3, r3, #10
	subs	r2, r2, #10
	movpl	pc, lr
	ldrh	ip, [r0], #2
	add	r2, r2, #16
	orr	r3, r3, ip, lsl r2
	mov	pc, lr
