@ Why Model_DispatchBitCommand is assembly:
@ Dispatcher enters Model_BitCommands' shared r0/r2/r3 decoder linkage;
@ model_bit_commands.s documents its unsaved r5 and ordinary fp mask.
@ In this file itself: it is a two-instruction fragment of that decoder, with
@ no prologue and no return of its own, and its tell is the computed pc into
@ code. and ip, r3, #7 / ldr pc, [pc, ip, lsl #2] is unconditional and has
@ no bounds test, where the compiler's casesi always emits cmp, ldrls pc and a
@ default branch; the table it indexes starts after the nop.
.syntax unified
	.arm
	.global Model_DispatchBitCommand
Model_DispatchBitCommand:
	and	ip, r3, #7
	ldr	pc, [pc, ip, lsl #2]
@ The word ldr pc steps over (pc reads 8 bytes ahead); never run.
AlchemyUncredited_08118240:
	nop
AlchemyUncreditedEnd_08118240:
@ Model_Decode writes the handlers' addresses here, relocated to where it runs,
@ from the table after them.
	.space 32, 0
	.global Model_BitCommandTable
Model_BitCommandTable:
	.4byte Model_BitCommands, Model_AddShortDelta, Model_AddSignedDelta, Model_AddLongDelta
	.4byte Model_BitCommands, Model_SubtractShortDelta, Model_SetBitValue, Model_SubtractLongDelta
