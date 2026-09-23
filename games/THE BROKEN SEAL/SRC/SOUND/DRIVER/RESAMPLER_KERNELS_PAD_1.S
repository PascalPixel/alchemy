@ Three nop words that fill the second record (RESAMPLER_KERNELS_1.S) to
@ 152 bytes after its branch to the slot's end; they never run. Padding
@ carries no credit.
.syntax unified
	.arm
	.set AlchemyLoadAddress, 0x08000528
	nop
	nop
	nop
