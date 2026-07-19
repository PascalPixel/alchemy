.syntax unified
.text
	.thumb
	.global Fragment_080f94f8
	.thumb_func
Fragment_080f94f8:
	ldr	r3, [pc, #8]
	strh	r0, [r3, #0]
	ldr	r3, [pc, #8]
	strh	r1, [r3, #0]
	bx	lr
	.inst.n 0x0000
	.4byte 0x02003030
	.4byte 0x0200300c
