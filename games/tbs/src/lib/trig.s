@ C2: Trig_SinOnAxis and Trig_Sin branch on lsrs carry; Trig_Cos falls
@ through directly into Trig_Sin's entry.
.syntax unified
	.thumb
	.global Trig_SinOnAxis
	.thumb_func
Trig_SinOnAxis:
	lsrs	r2, r0, #14
	movs	r0, #1
	ands	r0, r2
	lsls	r0, r0, #16
	lsrs	r2, r2, #2
	bcc.n	.L_helper_return
	negs	r0, r0
.L_helper_return:
	bx	lr
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
	lsls	r1, r0, #18
	lsrs	r1, r1, #24
	beq.n	Trig_SinOnAxis
	lsrs	r2, r0, #15
	bcc.n	.L_index
	movs	r3, #128
	lsls	r3, r3, #1
	subs	r1, r3, r1
.L_index:
	lsls	r1, r1, #1
	add	r3, pc, #12
	ldrh	r0, [r3, r1]
	lsrs	r2, r2, #1
	bcc.n	.L_return
	negs	r0, r0
	bx	lr
.L_return:
	bx	lr
