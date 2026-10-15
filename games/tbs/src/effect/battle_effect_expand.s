@ C2: SentouKouka_TenkaiYomi:
@ Shared decoder commands reuse live r0/r2/r3, change unsaved r5 and use
@ fp as a mask. ldr pc jumps into those commands in battle_effect_expand.s
@ and battle_effect_expand_commands.s.
@ In this file itself: it is a two-instruction fragment of that decoder, with
@ no prologue and no return of its own, and its tell is the computed pc into
@ code. and ip, r3, #7 / ldr pc, [pc, ip, lsl #2] is unconditional and has
@ no bounds test, where the compiler's casesi always emits cmp, ldrls pc and a
@ default branch; the table it indexes starts after the nop, in the next
@ object.
@ C2: SentouKouka_TenkaiCase2 to SentouKouka_TenkaiCase10:
@ entries share live r0/r2/r3 decoder state with their caller; delta
@ entries update unsaved r5 and use r11/fp as a data mask.
.syntax unified
	.section .rodata,"a",%progbits
	.arm
.L_bias:
	.4byte 0x80808080
.L_dispatch_base:
	.4byte SentouKouka_TenkaiCaseTable
.syntax unified
.text
	.arm
	.global SentouKouka_TenkaiYomi
SentouKouka_TenkaiYomi:
	and	ip, r3, #7
	ldr	pc, [pc, ip, lsl #2]
AlchemyUncredited_sentou_kouka_tenkai_1:
	nop
AlchemyUncreditedEnd_sentou_kouka_tenkai_1:
.syntax unified
	.section .data,"aw",%progbits
	.arm
.L_dispatch_runtime:
	.space 32, 0
	.global SentouKouka_TenkaiCaseTable
SentouKouka_TenkaiCaseTable:
	.4byte SentouKouka_TenkaiCase2
	.4byte SentouKouka_TenkaiCase4Plus
	.4byte SentouKouka_TenkaiCase8
	.4byte SentouKouka_TenkaiCase6Plus
	.4byte SentouKouka_TenkaiCase2
	.4byte SentouKouka_TenkaiCase4Minus
	.4byte SentouKouka_TenkaiCase10
	.4byte SentouKouka_TenkaiCase6Minus
.syntax unified
	.section .text.sentou_kouka_tenkai_commands,"ax",%progbits
	.arm
	.global SentouKouka_TenkaiCase2
SentouKouka_TenkaiCase2:
	lsr	r3, r3, #2
	subs	r2, r2, #2
	movpl	pc, lr
	ldrh	ip, [r0], #2
	add	r2, r2, #16
	orr	r3, r3, ip, lsl r2
	mov	pc, lr
	.global SentouKouka_TenkaiCase4Plus
SentouKouka_TenkaiCase4Plus:
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
	.global SentouKouka_TenkaiCase4Minus
SentouKouka_TenkaiCase4Minus:
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
	.global SentouKouka_TenkaiCase6Plus
SentouKouka_TenkaiCase6Plus:
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
	.global SentouKouka_TenkaiCase6Minus
SentouKouka_TenkaiCase6Minus:
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
	.global SentouKouka_TenkaiCase8
SentouKouka_TenkaiCase8:
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
	.global SentouKouka_TenkaiCase10
SentouKouka_TenkaiCase10:
	ror	r5, r3, #10
	and	r5, r5, #-33554432
	lsr	r3, r3, #10
	subs	r2, r2, #10
	movpl	pc, lr
	ldrh	ip, [r0], #2
	add	r2, r2, #16
	orr	r3, r3, ip, lsl r2
	mov	pc, lr
