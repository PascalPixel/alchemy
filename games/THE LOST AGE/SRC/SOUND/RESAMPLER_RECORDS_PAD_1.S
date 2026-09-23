@ Eight zero words that fill the second record (RESAMPLER_RECORDS_2.S) to
@ 76 bytes after its branch to the next record's boundary; they never run.
@ Padding carries no credit.
.syntax unified
	.arm
	.set AlchemyLoadAddress, 0x080004e4
	.4byte	0, 0, 0, 0, 0, 0, 0, 0
