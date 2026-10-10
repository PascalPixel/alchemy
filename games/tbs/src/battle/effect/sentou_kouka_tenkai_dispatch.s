@ C2: Shared decoder commands reuse live r0/r2/r3, change unsaved r5 and use
@ fp as a mask. ldr pc jumps into those commands in sentou_kouka_tenkai.s
@ and sentou_kouka_tenkai_commands.s.
@ In this file itself: it is a two-instruction fragment of that decoder, with
@ no prologue and no return of its own, and its tell is the computed pc into
@ code. and ip, r3, #7 / ldr pc, [pc, ip, lsl #2] is unconditional and has
@ no bounds test, where the compiler's casesi always emits cmp, ldrls pc and a
@ default branch; the table it indexes starts after the nop, in the next
@ object.
.syntax unified
	.arm
	.global SentouKouka_TenkaiYomi
SentouKouka_TenkaiYomi:
	and	ip, r3, #7
	ldr	pc, [pc, ip, lsl #2]
AlchemyUncredited_080f0100:
	nop
AlchemyUncreditedEnd_080f0100:
