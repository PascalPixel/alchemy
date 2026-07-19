.syntax unified
.text
	.thumb
	.global Fragment_08079418
	.thumb_func
Fragment_08079418:
	lsls	r3, r0, #20
	movs	r1, #4
	ands	r1, r0
	lsrs	r0, r3, #23
	ldr	r3, [pc, #12]
	movs	r2, #15
	ldrb	r0, [r3, r0]
	lsls	r2, r1
	ands	r0, r2
	asrs	r0, r1
	bx	lr
	.inst.n 0x0000
	.4byte 0x02000040
