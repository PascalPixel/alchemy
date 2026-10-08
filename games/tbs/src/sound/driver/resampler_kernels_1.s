.syntax unified
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
