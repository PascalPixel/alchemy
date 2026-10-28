.syntax unified
	.thumb
	.global umul3232H32
	.thumb_func
umul3232H32:
	add	r2, pc, #0
	bx	r2
	.arm
	umull	r2, r3, r0, r1
	add	r0, r3, #0
	bx	lr
