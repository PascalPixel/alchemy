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
	b	.Lrecord_1
	nop
	nop
	nop
	nop
	nop
	nop
	nop
.Lrecord_1:
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
	b	.Lrecord_2
	nop
	nop
	nop
.Lrecord_2:
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
	b	.Lrecords_end
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
.Lrecords_end:
