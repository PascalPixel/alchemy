@ C2: System_Entry:
@ msr CPSR_fc selects IRQ and system modes before their stack setup.
@ C2: IwramCodeRecords:
@ fp is a packed sample operand in arithmetic and rotates, not a frame
@ pointer.
@ C2: IwramCodeRecord1:
@ fp is a packed sample operand in arithmetic and rotates, not a frame
@ pointer.
@ C2: IwramCodeRecord2:
@ fp is a packed sample operand in arithmetic and rotates, not a frame
@ pointer.
@ C2: IwramCodeRecordsEnd:
@ this unused packed-sample arithmetic suffix uses fp (r11) as an
@ arithmetic and rotate operand, not a frame pointer.
@ The final 152-byte preset record begins in the preceding object with
@ four zeroing moves and a branch over this 132-byte suffix. The DMA loader
@ copies the whole record; its known entry skips to the mixer continuation
@ immediately after the copied record.
.syntax unified
.text
	.arm
	.global System_Entry
System_Entry:
	mov	r0, #18
	msr	CPSR_fc, r0
	ldr	sp, [pc, #40]
	mov	r0, #31
	msr	CPSR_fc, r0
	ldr	sp, [pc, #24]
	ldr	r1, [pc, #28]
	add	r0, pc, #908
	str	r0, [r1]
	ldr	r1, [pc, #20]
	mov	lr, pc
	bx	r1
	b System_Entry
	.4byte 0x03007f00
	.4byte 0x03007fa0
	.4byte 0x03007ffc
	.4byte System_Initialize
.syntax unified
.text
	.arm
	.global IwramCodeRecords
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
	b	IwramCodeRecord1
.syntax unified
	.section .padding,"a",%progbits
	.arm
	nop
	nop
	nop
	nop
	nop
	nop
	nop
.syntax unified
	.section .text.resampler_kernels_1,"ax",%progbits
	.arm
	.global IwramCodeRecord1
IwramCodeRecord1:
	asrs	r7, fp, #19
	adds	r7, r7, ip, asr #20
	addmi	r7, r7, #48
	asrs	r6, ip, #19
	adds	r6, r6, fp, asr #20
	addmi	r6, r6, #48
	adds	r4, r7, r6, lsl #16
	ror	fp, fp, #16
	ror	ip, ip, #16
	asrs	r7, fp, #19
	adds	r7, r7, ip, asr #20
	addmi	r7, r7, #48
	asrs	r6, ip, #19
	adds	r6, r6, fp, asr #20
	addmi	r6, r6, #48
	adds	r2, r7, r6, lsl #16
	ror	fp, fp, #8
	ror	ip, ip, #8
	asrs	r7, fp, #19
	adds	r7, r7, ip, asr #20
	addmi	r7, r7, #48
	asrs	r6, ip, #19
	adds	r6, r6, fp, asr #20
	addmi	r6, r6, #48
	adds	r3, r7, r6, lsl #16
	ror	fp, fp, #16
	ror	ip, ip, #16
	asrs	r7, fp, #19
	adds	r7, r7, ip, asr #20
	addmi	r7, r7, #48
	asrs	r6, ip, #19
	adds	r6, r6, fp, asr #20
	addmi	r6, r6, #48
	adds	r1, r7, r6, lsl #16
	b	IwramCodeRecord2
.syntax unified
	.section .padding.resampler_kernels_pad_1,"a",%progbits
	.arm
	nop
	nop
	nop
.syntax unified
	.section .text.resampler_kernels_2,"ax",%progbits
	.arm
	.global IwramCodeRecord2
IwramCodeRecord2:
	asrs	r6, r4, #18
	adds	r6, r6, fp, asr #19
	addmi	r6, r6, #40
	ror	r4, r4, #16
	asrs	r7, r4, #18
	adds	r7, r7, ip, asr #19
	addmi	r7, r7, #40
	adds	r4, r7, r6, lsl #16
	ror	fp, fp, #16
	ror	ip, ip, #16
	asrs	r6, r2, #18
	adds	r6, r6, fp, asr #19
	addmi	r6, r6, #40
	ror	r2, r2, #16
	asrs	r7, r2, #18
	adds	r7, r7, ip, asr #19
	addmi	r7, r7, #40
	adds	r2, r7, r6, lsl #16
	ror	fp, fp, #8
	ror	ip, ip, #8
	asrs	r6, r3, #18
	adds	r6, r6, fp, asr #19
	addmi	r6, r6, #40
	ror	r3, r3, #16
	asrs	r7, r3, #18
	adds	r7, r7, ip, asr #19
	addmi	r7, r7, #40
	adds	r3, r7, r6, lsl #16
	ror	fp, fp, #16
	ror	ip, ip, #16
	asrs	r6, r1, #18
	adds	r6, r6, fp, asr #19
	addmi	r6, r6, #40
	ror	r1, r1, #16
	asrs	r7, r1, #18
	adds	r7, r7, ip, asr #19
	addmi	r7, r7, #40
	adds	r1, r7, r6, lsl #16
.Lrecord_3:
	asrs	r6, r4, #19
	adds	r6, r6, fp, asr #18
	addmi	r6, r6, #72
	ror	r4, r4, #16
	asrs	r7, r4, #19
	adds	r7, r7, ip, asr #18
	addmi	r7, r7, #72
	adds	r4, r7, r6, lsl #16
	ror	fp, fp, #16
	ror	ip, ip, #16
	asrs	r6, r2, #19
	adds	r6, r6, fp, asr #18
	addmi	r6, r6, #72
	ror	r2, r2, #16
	asrs	r7, r2, #19
	adds	r7, r7, ip, asr #18
	addmi	r7, r7, #72
	adds	r2, r7, r6, lsl #16
	ror	fp, fp, #8
	ror	ip, ip, #8
	asrs	r6, r3, #19
	adds	r6, r6, fp, asr #18
	addmi	r6, r6, #72
	ror	r3, r3, #16
	asrs	r7, r3, #19
	adds	r7, r7, ip, asr #18
	addmi	r7, r7, #72
	adds	r3, r7, r6, lsl #16
	ror	fp, fp, #16
	ror	ip, ip, #16
	asrs	r6, r1, #19
	adds	r6, r6, fp, asr #18
	addmi	r6, r6, #72
	ror	r1, r1, #16
	asrs	r7, r1, #19
	adds	r7, r7, ip, asr #18
	addmi	r7, r7, #72
	adds	r1, r7, r6, lsl #16
.Lrecord_4:
	mov	r1, #0
	mov	r2, #0
	mov	r3, #0
	mov	r4, #0
	b	IwramCodeRecordsEnd
.syntax unified
	.section .text.resampler_kernels_2,"ax",%progbits
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
