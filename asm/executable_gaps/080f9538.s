.syntax unified
.text
	.thumb
	.global Fragment_080f9538
	.thumb_func
Fragment_080f9538:
	ldr	r3, [pc, #8]
	strh	r0, [r3, #0]
	ldr	r3, [pc, #8]
	strh	r1, [r3, #0]
	bx	lr
	.inst.n 0x0000
	.4byte 0x02003034
	.4byte 0x02003010
