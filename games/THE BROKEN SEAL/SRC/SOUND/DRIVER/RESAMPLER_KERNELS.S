@ Five interchangeable 152-byte ARM records for the sample loop of the
@ resident mixer. The selector at 0x080037d4 treats an index above 4 as 0
@ and copies record index * 152 with DMA3 (control 0x84000026, 38 words)
@ over the loop body at 0x03000bd8 in SYSTEM/RUNTIME_MIX_RENDER.S, between
@ the loop's loads and its stmia r0!, {r1-r4}. A record has no entry or
@ return: it fills the slot or branches to the slot's end. This module is
@ the first record up to that branch; its nop padding is
@ RESAMPLER_KERNELS_PAD.S, the second record RESAMPLER_KERNELS_1.S and the
@ other three RESAMPLER_KERNELS_2.S. Fixed-size patch records that run
@ inside another routine's loop are not compiled functions. TBS only.
@ credit: handwritten — ARM sound-buffer record patched into the IWRAM runtime
.syntax unified
	.set	.Lrecord_1, 0x0800049c
.text
	.arm
	.global Func_08000404
	.global IwramCodeRecords
Func_08000404:
IwramCodeRecords:
	asrs	r1, fp, #20
	bicpl	r1, r1, #31
	adds	r1, r1, ip, asr #20
	addmi	r1, r1, #32
	add	r1, r1, r1, asr #1
	add	r4, r1, r1, lsl #16
	ror	fp, fp, #16
	ror	ip, ip, #16
	asrs	r1, fp, #20
	bicpl	r1, r1, #31
	adds	r1, r1, ip, asr #20
	addmi	r1, r1, #32
	add	r1, r1, r1, asr #1
	add	r2, r1, r1, lsl #16
	ror	fp, fp, #8
	ror	ip, ip, #8
	asrs	r1, fp, #20
	bicpl	r1, r1, #31
	adds	r1, r1, ip, asr #20
	addmi	r1, r1, #32
	add	r1, r1, r1, asr #1
	add	r3, r1, r1, lsl #16
	ror	fp, fp, #16
	ror	ip, ip, #16
	asrs	r1, fp, #20
	bicpl	r1, r1, #31
	adds	r1, r1, ip, asr #20
	addmi	r1, r1, #32
	add	r1, r1, r1, asr #1
	add	r1, r1, r1, lsl #16
	b	.Lrecord_1
