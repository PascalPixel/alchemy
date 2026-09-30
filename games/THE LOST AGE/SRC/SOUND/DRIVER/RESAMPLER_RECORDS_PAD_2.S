@ Three zero words that fill the third record (RESAMPLER_RECORDS_3.S) to 76
@ bytes after its branch to the next record's boundary; they never run.
@ Padding carries no credit.
.syntax unified
	.section .padding,"a",%progbits
	.arm
	.4byte	0
	.4byte	0
	.4byte	0
