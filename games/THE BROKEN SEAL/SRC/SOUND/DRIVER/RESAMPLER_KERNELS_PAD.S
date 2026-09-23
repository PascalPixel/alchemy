@ Seven nop words that fill the first record of RESAMPLER_KERNELS.S to 152
@ bytes after its branch to the slot's end; they never run. Padding
@ carries no credit.
.syntax unified
	.arm
	.set AlchemyLoadAddress, 0x08000480
	nop
	nop
	nop
	nop
	nop
	nop
	nop
