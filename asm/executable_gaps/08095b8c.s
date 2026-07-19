.syntax unified
.text
	.thumb
	.global Fragment_08095b8c
	.thumb_func
Fragment_08095b8c:
	ldr	r3, [pc, #20]
	ldr	r3, [r3, #0]
	movs	r2, #1
	lsrs	r3, r3, #2
	ldr	r1, [pc, #16]
	ands	r3, r2
	lsls	r3, r3, #2
	ldr	r3, [r3, r1]
	str	r3, [r0, #24]
	str	r3, [r0, #28]
	bx	lr
	.inst.n 0x0000
	.4byte 0x03001800
	.4byte 0x0809f0a4
