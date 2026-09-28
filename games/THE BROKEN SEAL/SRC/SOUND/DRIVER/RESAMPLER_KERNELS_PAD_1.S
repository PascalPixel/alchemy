@ Three nop words that fill the second record (RESAMPLER_KERNELS_1.S) to
@ 152 bytes after its branch to the slot's end; they never run. Padding
@ carries no credit.
.syntax unified
	.section .padding,"a",%progbits
	.arm
	nop
	nop
	nop
