@ Why assembly: msr CPSR_fc selects IRQ and system modes before their stack setup.
@ Why assembly: it zeroes r2 and r4 and branches into Sound_ResamplerRecord1
@ with no return; the records it enters take their samples in ip and fp and
@ their volumes in r1 and r6, a linkage no procedure-call standard produces.
@ Why assembly: fp is a packed sample operand in arithmetic and rotates, not a
@ frame pointer.
@ Why assembly: fp is a packed sample operand in arithmetic and rotates, not a
@ frame pointer.
@ Why assembly: fp is a packed sample operand in arithmetic and rotates, not a
@ frame pointer; it ends with no return.
	.section .text,"ax",%progbits
	.balign 4
.syntax unified
	.arm
	.global Entry
Entry:
	mov	r0, #18
	msr	CPSR_fc, r0
	ldr	sp, [pc, #40]
	mov	r0, #31
	msr	CPSR_fc, r0
	ldr	sp, [pc, #24]
	ldr	r1, [pc, #28]
	add	r0, pc, #0x26c
	str	r0, [r1]
	ldr	r1, [pc, #20]
	mov	lr, pc
	bx	r1
	b	Entry
	.4byte	0x03007f00
	.4byte	0x03007fa0
	.4byte	0x03007ffc
	.4byte	syInit

	.section .text,"ax",%progbits
	.balign 4
.syntax unified
	.arm
	.global Sound_ResamplerRecords
Sound_ResamplerRecords:
	mov	r2, #0
	mov	r4, #0
	b	Sound_ResamplerRecord1

.syntax unified
	.section .padding,"a",%progbits
	.arm
	.4byte	0, 0, 0, 0, 0, 0, 0, 0
	.4byte	0, 0, 0, 0, 0, 0, 0, 0

	.section .text.resampler_records_2,"ax",%progbits
	.balign 4
.syntax unified
	.arm
	.global Sound_ResamplerRecord1
Sound_ResamplerRecord1:
	asrs	r4, ip, #24
	mulne	r4, r6, r4
	rors	ip, ip, #16
	asrs	r2, ip, #24
	mulne	r2, r6, r2
	asrs	ip, fp, #24
	mlane	r4, r1, ip, r4
	rors	fp, fp, #16
	asrs	ip, fp, #24
	mlane	r2, r1, ip, r2
	b	Sound_ResamplerRecord2

.syntax unified
	.section .padding.resampler_records_pad_1,"a",%progbits
	.arm
	.4byte	0, 0, 0, 0, 0, 0, 0, 0

	.section .text.resampler_records_3,"ax",%progbits
	.balign 4
.syntax unified
	.arm
	.global Sound_ResamplerRecord2
Sound_ResamplerRecord2:
	and	r7, r7, r8
	add	r7, r7, r7, lsr #7
	asrs	r4, ip, #24
	mulne	r4, r6, r4
	add	r4, r4, r7, asr #19
	rors	ip, ip, #16
	asrs	r2, ip, #24
	mulne	r2, r6, r2
	rors	r7, r7, #16
	add	r2, r2, r7, asr #19
	asrs	ip, fp, #24
	mlane	r4, r1, ip, r4
	rors	fp, fp, #16
	asrs	ip, fp, #24
	mlane	r2, r1, ip, r2
	b	Sound_ResamplerRecord3

.syntax unified
	.section .padding.resampler_records_pad_2,"a",%progbits
	.arm
	.4byte	0
	.4byte	0
	.4byte	0

	.section .text.resampler_records_4,"ax",%progbits
	.balign 4
.syntax unified
	.arm
	.global Sound_ResamplerRecord3
Sound_ResamplerRecord3:
	and	r7, r7, r8
	add	r7, r7, r7, lsr #7
	asrs	r4, ip, #24
	mulne	r4, r6, r4
	add	r4, r4, r7, asr #19
	rors	ip, ip, #16
	asrs	r2, ip, #24
	mulne	r2, r6, r2
	rors	r7, r7, #16
	add	r2, r2, r7, asr #19
	asrs	ip, fp, #24
	mlane	r4, r1, ip, r4
	rors	fp, fp, #16
	asrs	ip, fp, #24
	mlane	r2, r1, ip, r2
	ldrsb	ip, [sl], #2
	add	r2, r2, ip, lsl #21
	ldrsb	ip, [sl], #2
	add	r4, r4, ip, lsl #21
	and	r7, r7, r8
	add	r7, r7, r7, lsr #7
	asrs	r4, ip, #24
	mulne	r4, r6, r4
	add	r4, r4, r7, asr #19
	rors	ip, ip, #16
	asrs	r2, ip, #24
	mulne	r2, r6, r2
	rors	r7, r7, #16
	add	r2, r2, r7, asr #19
	asrs	ip, fp, #24
	mlane	r4, r1, ip, r4
	rors	fp, fp, #16
	asrs	ip, fp, #24
	mlane	r2, r1, ip, r2
	ldrsb	ip, [sl], #2
	add	r2, r2, ip, lsl #21
	ldrsb	ip, [sl], #2
	add	r4, r4, ip, lsl #21
