.syntax unified
.text
	.thumb
	.global Fragment_080935b0
	.thumb_func
Fragment_080935b0:
	ldr	r4, [pc, #28]
	ldr	r4, [r4, #0]
	mov	ip, r4
	adds	r4, #236
	str	r0, [r4, #0]
	mov	r0, ip
	adds	r0, #240
	str	r1, [r0, #0]
	mov	r1, ip
	adds	r1, #244
	str	r2, [r1, #0]
	mov	r2, ip
	adds	r2, #248
	str	r3, [r2, #0]
	bx	lr
	.inst.n 0x0000
	.4byte 0x03001e70
