.syntax unified
	.thumb
	.global Trig_Cos
	.thumb_func
Trig_Cos:
	movs	r1, #64
	lsls	r1, r1, #8
	adds	r0, r0, r1
	.global Trig_Sin
	.thumb_func
Trig_Sin:
	adds	r0, #32
	lsls	r1, r0, #16
	lsrs	r1, r1, #22
	lsls	r1, r1, #1
	add	r0, pc, #8
	ldrsh	r0, [r0, r1]
	lsls	r0, r0, #2
	bx	lr
