@ Why btModelDispatchBitCommand is assembly:
@ Dispatcher enters btModelBitCommands' shared r0/r2/r3 decoder linkage;
@ model_bit_commands.s documents its unsaved r5 and ordinary fp mask.
@ In this file itself: it is a two-instruction fragment of that decoder, with
@ no entry code and no return of its own, and its tell is the computed pc into
@ code. and ip, r3, #7 / ldr pc, [pc, ip, lsl #2] is unconditional and has
@ no bounds test, where the compiler's casesi always emits cmp, ldrls pc and a
@ default branch; the table it indexes starts after the nop.
.syntax unified
	.arm
	.global btModelDispatchBitCommand
btModelDispatchBitCommand:
	and	ip, r3, #7
	ldr	pc, [pc, ip, lsl #2]
@ The word ldr pc steps over (pc reads 8 bytes ahead); never run.
AlchemyUncredited08118240:
	nop
AlchemyUncreditedEnd08118240:
@ btModelDecode writes the handlers' addresses here, relocated to where it runs,
@ from the table after them.
	.space 32, 0
	.global btModelBitCommandTable
btModelBitCommandTable:
	.4byte btModelBitCommands, btModelAddShortDelta, btModelAddSignedDelta, btModelAddLongDelta
	.4byte btModelBitCommands, btModelSubtractShortDelta, btModelSetBitValue, btModelSubtractLongDelta
