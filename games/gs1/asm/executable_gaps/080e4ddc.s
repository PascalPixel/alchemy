.syntax unified
.text
	.thumb
	.set sub_080e4eaa, 0x080e4eaa
	.global Fragment_080e4ddc
	.thumb_func
Fragment_080e4ddc:
	lsls	r0, r0, #4
	movs	r0, r0
	strb	r4, [r0, #30]
	movs	r0, r0
	bcs sub_080e4eaa
	lsrs	r4, r1, #32
	ldrb	r0, [r5, #0]
	movs	r0, r0
	strb	r4, [r6, #30]
	movs	r0, r0
	strb	r0, [r7, #30]
	movs	r0, r0
	.inst.n 0xffff
	.inst.n 0x0000
	udf	#72
	lsrs	r6, r1, #32
	subs	r4, #86
	lsls	r1, r0, #8
	ldrb	r4, [r4, #0]
	movs	r0, r0
	movs	r0, r4
	lsls	r0, r0, #16
	movs	r0, r5
	lsls	r0, r0, #16
