@ Five fixed-size 76-byte ARM records in the idioms of the resident sound
@ mixer, between SYSTEM/ENTRY.S and the VSync DMA refresh
@ (0x0800046c..0x080005e8), where TBS keeps the five 152-byte records of
@ SOUND/DRIVER/RESAMPLER_KERNELS.S. The first three stop with a branch to
@ the next record's boundary over zero padding (0x08000478, 0x080004e4 and
@ 0x08000544: RESAMPLER_RECORDS_PAD.S to _PAD_2.S, which carry no credit);
@ none has an entry or a return. Code padded with zero words to a fixed
@ record size is not what any compiler emits. This module is the first
@ record (mov r2, #0;
@ mov r4, #0; b); RESAMPLER_RECORDS_2.S to _4.S hold the others. The TLA
@ code that selects a record is not identified.
@ credit: handwritten — TLA fixed-size ARM sound resampler record
.syntax unified
	.arm
	.set AlchemyLoadAddress, 0x0800046c
	.set	.L_080004b8, 0x080004b8
	.global Sound_ResamplerRecords
Sound_ResamplerRecords:
	mov	r2, #0
	mov	r4, #0
	b	.L_080004b8
