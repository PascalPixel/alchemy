@ Three zero words that fill the third record (RESAMPLER_RECORDS_3.S) to 76
@ bytes after its branch to the next record's boundary; they never run.
@ Padding carries no credit.
.syntax unified
	.arm
	.set AlchemyLoadAddress, 0x08000544
	.4byte	0
	.4byte	0
	.4byte	0
