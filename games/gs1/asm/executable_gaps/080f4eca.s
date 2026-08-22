.syntax unified
.text
	.thumb
	.global Fragment_080f4eca
	.thumb_func
Fragment_080f4eca:
	movs	r0, r0
	lsls	r5, r3, #5
	movs	r0, r0
	lsls	r2, r1, #10
	movs	r0, r0
	.inst.n 0xfea2
	.inst.n 0xffff
	lsls	r7, r7, #7
	movs	r0, r0
	movs	r3, #0
	ands	r0, r0
	strb	r0, [r1, r0]
	lsrs	r7, r1, #32
	lsls	r7, r7, #15
	movs	r0, r0
	movs	r0, #0
	strh	r0, [r0, #0]
	lsrs	r0, r3, #5
	movs	r0, r0
	.4byte 0x000009c3
	.4byte 0x030001d8
	.4byte 0xff380000
	.4byte 0xff6a0000
	.4byte 0xffff8000
