.syntax unified
.text
	.thumb
	.global Fragment_080793c8
	.thumb_func
Fragment_080793c8:
	lsls	r3, r0, #20
	lsrs	r0, r3, #23
	ldr	r3, [pc, #4]
	strb	r1, [r3, r0]
	bx	lr
	.inst.n 0x0000
	.4byte 0x02000040
