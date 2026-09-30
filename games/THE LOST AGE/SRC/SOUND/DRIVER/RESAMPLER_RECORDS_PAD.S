@ Sixteen zero words that fill the first record of RESAMPLER_RECORDS.S to 76
@ bytes after its branch to the next record's boundary; they never run.
@ Padding carries no credit, as TBS SOUND/DRIVER/RESAMPLER_KERNELS_PAD.S.
.syntax unified
	.section .padding,"a",%progbits
	.arm
	.4byte	0, 0, 0, 0, 0, 0, 0, 0
	.4byte	0, 0, 0, 0, 0, 0, 0, 0
