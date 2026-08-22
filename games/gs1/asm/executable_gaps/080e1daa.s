.syntax unified
.text
	.thumb
	.global Fragment_080e1daa
	.thumb_func
Fragment_080e1daa:
	movs	r0, r0
	asrs	r0, r2, #32
	movs	r0, r0
	subs	r4, r1, #4
	lsls	r0, r0, #12
	udf	#72
	lsrs	r6, r1, #32
	movs	r7, #16
	movs	r0, r0
	ldrb	r4, [r4, #0]
	movs	r0, r0
	subs	r4, r0, r4
	lsls	r0, r0, #12
	str	r0, [sp, #916]
	lsrs	r4, r1, #32
	ldrb	r0, [r5, #0]
	movs	r0, r0
	lsls	r3, r6, #5
	movs	r0, r0
	lsls	r6, r1, #3
	movs	r0, r0
	movs	r0, r0
	lsls	r1, r0, #8
	lsls	r1, r2, #3
	movs	r0, r0
	lsls	r6, r4, #1
	movs	r0, r0
	lsls	r7, r1, #3
	movs	r0, r0
	asrs	r0, r1, #14
	lsls	r0, r0, #12
	lsls	r4, r6, #1
	movs	r0, r0
	lsls	r2, r2, #1
	lsls	r0, r0, #16
	lsls	r6, r1, #14
	movs	r0, r0
	lsrs	r0, r6, #17
	lsls	r1, r0, #8
