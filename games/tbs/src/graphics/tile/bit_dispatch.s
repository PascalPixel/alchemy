@ C2: Dispatcher enters Model_BitCommands' shared r0/r2/r3 decoder linkage;
@ bit_commands.s documents its unsaved r5 and ordinary fp mask.
.syntax unified
.text
	.arm
	.global Model_DispatchBitCommand
Model_DispatchBitCommand:
	and	ip, r3, #7
	ldr	pc, [pc, ip, lsl #2]
AlchemyUncredited_080b5214:
	nop
AlchemyUncreditedEnd_080b5214:
