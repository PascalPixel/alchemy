@ C2: Thumb entry switches into ARM in the same function before umull; Nintendo
@ library umul3232H32.
.syntax unified
	.thumb
	.global Audio_UmulHigh32
	.thumb_func
Audio_UmulHigh32:
	add	r2, pc, #0
	bx	r2
	.arm
	umull	r2, r3, r0, r1
	add	r0, r3, #0
	bx	lr
