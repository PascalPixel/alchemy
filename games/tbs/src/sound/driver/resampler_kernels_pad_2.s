@ C2: this unused packed-sample arithmetic suffix uses fp (r11) as an
@ arithmetic and rotate operand, not a frame pointer.
@ The final 152-byte preset record begins in the preceding object with
@ four zeroing moves and a branch over this 132-byte suffix. The DMA loader
@ copies the whole record; its known entry skips to the mixer continuation
@ immediately after the copied record.
.syntax unified
	.section .text,"ax",%progbits
	.arm
	adds	r7, r7, fp, asr #18
	addmi	r7, r7, #64
	adds	r4, r7, r6, lsl #16
	ror	fp, fp, #16
	ror	ip, ip, #16
	asrs	r6, r2, #19
	adds	r6, r6, ip, asr #18
	addmi	r6, r6, #64
	ror	r2, r2, #16
	asrs	r7, r2, #19
	adds	r7, r7, fp, asr #18
	addmi	r7, r7, #64
	adds	r2, r7, r6, lsl #16
	ror	fp, fp, #8
	ror	ip, ip, #8
	asrs	r6, r3, #19
	adds	r6, r6, ip, asr #18
	addmi	r6, r6, #64
	ror	r3, r3, #16
	asrs	r7, r3, #19
	adds	r7, r7, fp, asr #18
	addmi	r7, r7, #64
	adds	r3, r7, r6, lsl #16
	ror	fp, fp, #16
	ror	ip, ip, #16
	asrs	r6, r1, #19
	adds	r6, r6, ip, asr #18
	addmi	r6, r6, #64
	ror	r1, r1, #16
	asrs	r7, r1, #19
	adds	r7, r7, fp, asr #18
	addmi	r7, r7, #64
	adds	r1, r7, r6, lsl #16
	.global IwramCodeRecordsEnd
IwramCodeRecordsEnd:
