.syntax unified
.text
	.thumb
	.global Fragment_080d7e5c
	.thumb_func
Fragment_080d7e5c:
	asrs	r0, r2, #32
	movs	r0, r0
	.inst.n 0xe9be
	.inst.n 0x080e
	.inst.n 0xe9a8
	.inst.n 0x080e
	.inst.n 0xe9b3
	.inst.n 0x080e
	movs	r0, r0
	lsls	r1, r0, #8
	.inst.n 0xe9da
	.inst.n 0x080e
	.inst.n 0xe9d4
	.inst.n 0x080e
	.inst.n 0xe9d7
	.inst.n 0x080e
	lsrs	r0, r0, #24
	lsls	r1, r0, #8
	udf	#150
	lsrs	r6, r1, #32
	udf	#132
	lsrs	r6, r1, #32
	ldrb	r4, [r4, #0]
	movs	r0, r0
	subs	r4, r0, r4
	lsls	r0, r0, #12
	strb	r0, [r3, #31]
	movs	r0, r0
	lsls	r2, r2, #1
	lsls	r0, r0, #16
	lsls	r1, r4, #4
	movs	r0, r0
	.4byte 0xfff00000
