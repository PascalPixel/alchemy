.syntax unified
	.thumb
	.set sub_08012cb0, 0x08012cb0
	.set sub_080135b8, 0x080135b8
	.set sub_0801369e, 0x0801369e
	.set sub_080138c2, 0x080138c2
	.set sub_080138d2, 0x080138d2
	.global Func_08013164
	.thumb_func
Func_08013164:
	movs	r4, #3
	lsls	r4, r4, #24
	movs	r1, #4
	lsrs	r2, r0, #22
	ands	r2, r1
	str	r0, [r2, r4]
	bx	lr
	movs	r0, r0
	movs	r0, r0
	.2byte 0xef03
	.2byte 0xfffc
	.2byte 0xeaff
	.2byte 0xc301
	b.n	sub_080138c2
	subs	r4, #2
	b.n	sub_0801369e
	movs	r0, #0
	b.n	sub_08012cb0
	adds	r2, r4, r0
	b.n	.L_08013192
	movs	r0, #0
	b.n	sub_080138d2
	lsrs	r2, r0, #8
.L_08013192:
	b.n	sub_080135b8
	lsls	r4, r7, #5
	asrs	r3, r0, #21
	.2byte 0xfffe
	.2byte 0x1aff
