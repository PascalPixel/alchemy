.syntax unified
.text
	.thumb
	.global Fragment_080108c4
	.thumb_func
Fragment_080108c4:
	ldr	r3, [pc, #20]
	ldr	r4, [r3, #0]
	movs	r2, #224
	ldrh	r1, [r4, #20]
	ldr	r3, [pc, #16]
	lsls	r2, r2, #4
	ands	r2, r0
	ands	r3, r1
	orrs	r3, r2
	strh	r3, [r4, #20]
	bx	lr
	.inst.n 0x0000
	.4byte 0x03001e70
	.4byte 0x0000f1ff
