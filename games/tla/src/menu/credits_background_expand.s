@ The command decoder (its decode file) calls Read, which takes the next packed
@ value and jumps, through the case table, to one of the case commands below,
@ which update the running pixel value in r5. They share the decoder's
@ registers (r0 source, r2 and r3 read state, r5 value, r11 the mask 7) and
@ have no ticks of their own. One object, as The Broken Seal's
@ credits_background_expand.s: the relocation words, the dispatch, the case
@ table and the commands, which the linker script places in that order.
@
@ Why assembly:
@ Why Read is assembly: shared decoder commands reuse live r0/r2/r3, change
@ unsaved r5 and use fp as a mask. ldr pc jumps into those commands, below
@ in this file. It is a two-instruction fragment of that decoder, with
@ no entry code and no return of its own, and its tell is the computed pc into
@ code. and ip, r3, #7 / ldr pc, [pc, ip, lsl #2] is unconditional and has
@ no bounds test, where the compiler's casesi always emits cmp, ldrls pc and a
@ default branch; the table it indexes starts after the nop.
@ Why the case commands are assembly: entries share live r0/r2/r3 decoder
@ state with their caller; delta entries update unsaved r5 and use r11/fp as
@ a data mask.
.syntax unified
	.section .rodata,"a",%progbits
	.arm
	.4byte	0x80808080
	.4byte	mnCreditsDecodeBackgroundCaseTable
.text
	.arm
	.global mnCreditsDecodeBackgroundRead
mnCreditsDecodeBackgroundRead:
	and	ip, r3, #7
	ldr	pc, [pc, ip, lsl #2]
AlchemyUncreditedCreditsBackgroundRead:
	nop
AlchemyUncreditedEndCreditsBackgroundRead:
	.section .data,"aw",%progbits
	.arm
	.4byte	0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000
	.global mnCreditsDecodeBackgroundCaseTable
mnCreditsDecodeBackgroundCaseTable:
	.4byte	mnCreditsDecodeBackgroundCase2
	.4byte	mnCreditsDecodeBackgroundCase4Plus
	.4byte	mnCreditsDecodeBackgroundCase8
	.4byte	mnCreditsDecodeBackgroundCase6Plus
	.4byte	mnCreditsDecodeBackgroundCase2
	.4byte	mnCreditsDecodeBackgroundCase4Minus
	.4byte	mnCreditsDecodeBackgroundCase10
	.4byte	mnCreditsDecodeBackgroundCase6Minus
	.section .text.credits_background_expand_commands,"ax",%progbits
	.arm
	.global mnCreditsDecodeBackgroundCommands
mnCreditsDecodeBackgroundCommands:
	.global mnCreditsDecodeBackgroundCase2
mnCreditsDecodeBackgroundCase2:
	lsr	r3, r3, #2
	subs	r2, r2, #2
	movpl	pc, lr
	ldrh	ip, [r0], #2
	add	r2, r2, #16
	orr	r3, r3, ip, lsl r2
	mov	pc, lr
	.global mnCreditsDecodeBackgroundCase4Plus
mnCreditsDecodeBackgroundCase4Plus:
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
	.global mnCreditsDecodeBackgroundCase4Minus
mnCreditsDecodeBackgroundCase4Minus:
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
	.global mnCreditsDecodeBackgroundCase6Plus
mnCreditsDecodeBackgroundCase6Plus:
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
	.global mnCreditsDecodeBackgroundCase6Minus
mnCreditsDecodeBackgroundCase6Minus:
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
	.global mnCreditsDecodeBackgroundCase8
mnCreditsDecodeBackgroundCase8:
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
	.global mnCreditsDecodeBackgroundCase10
mnCreditsDecodeBackgroundCase10:
	ror	r5, r3, #10
	and	r5, r5, #0xfe000000
	lsr	r3, r3, #10
	subs	r2, r2, #10
	movpl	pc, lr
	ldrh	ip, [r0], #2
	add	r2, r2, #16
	orr	r3, r3, ip, lsl r2
	mov	pc, lr
