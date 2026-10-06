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
