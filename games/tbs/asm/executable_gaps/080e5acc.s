.syntax unified
.text
	.thumb
	.global Fragment_080e5acc
	.thumb_func
Fragment_080e5acc:
	lsrs	r6, r2, #17
	lsls	r1, r0, #8
	udf	#202
	lsrs	r6, r1, #32
	udf	#208
	lsrs	r6, r1, #32
	.inst.n 0xffff
	.inst.n 0x0000
	.inst.n 0xedd0
	.inst.n 0x080e
	udf	#190
	lsrs	r6, r1, #32
	movs	r0, r0
	lsls	r1, r0, #8
	subs	r4, r1, #4
	lsls	r0, r0, #12
	ldrb	r0, [r5, #0]
	movs	r0, r0
	subs	r0, r2, #1
	lsls	r0, r0, #12
	lsls	r0, r5, #27
	lsls	r1, r0, #8
	strb	r0, [r5, #30]
	.inst.n 0x0000
