@ Why assembly: shared decoder commands reuse live r0/r2/r3, change unsaved
@ r5 and use fp as a mask. ldr pc jumps into those commands, in
@ credits_background_expand_commands.s.
@ In this file itself: it is a two-instruction fragment of that decoder, with
@ no prologue and no return of its own, and its tell is the computed pc into
@ code. and ip, r3, #7 / ldr pc, [pc, ip, lsl #2] is unconditional and has
@ no bounds test, where the compiler's casesi always emits cmp, ldrls pc and a
@ default branch; the table it indexes starts after the nop, in the next
@ object, credits_background_expand_tables.s.
.syntax unified
	.arm
	.global BattleEffect_ExpandRead
BattleEffect_ExpandRead:
	and	ip, r3, #7
	ldr	pc, [pc, ip, lsl #2]
AlchemyUncredited_081a012c:
	nop
AlchemyUncreditedEnd_081a012c:
