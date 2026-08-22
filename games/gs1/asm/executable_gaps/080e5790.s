.syntax unified
.text
	.thumb
	.set sub_080e67a2, 0x080e67a2
	.global Fragment_080e5790
	.thumb_func
Fragment_080e5790:
	lsls	r4, r6, #2
	movs	r0, r0
	lsls	r0, r4, #2
	movs	r0, r0
	lsls	r5, r7, #1
	movs	r0, r0
	udf	#72
	lsrs	r6, r1, #32
	bl sub_080e67a2
	ldrb	r0, [r5, #0]
	movs	r0, r0
	ands	r0, r0
	lsls	r1, r0, #8
	lsrs	r7, r7, #31
	movs	r0, r0
	subs	r6, r7, #7
	.inst.n 0x0000
