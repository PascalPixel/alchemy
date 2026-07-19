.syntax unified
.text
	.thumb
	.global Fragment_080cd488
	.thumb_func
Fragment_080cd488:
	ldr	r3, [pc, #24]
	ldr	r0, [pc, #28]
	ldr	r2, [r3, #0]
	adds	r3, r2, r0
	ldr	r3, [r3, #0]
	ldr	r1, [pc, #24]
	str	r3, [r1, #0]
	ldr	r3, [pc, #24]
	adds	r2, r2, r3
	ldr	r3, [r2, #0]
	adds	r1, #4
	str	r3, [r1, #0]
	bx	lr
	.inst.n 0x0000
	.4byte 0x03001eec
	.4byte 0x000077d0
	.4byte 0x04000028
	.4byte 0x000077d4
