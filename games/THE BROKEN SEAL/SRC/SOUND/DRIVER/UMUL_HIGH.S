@ Returns the high 32 bits of the unsigned 64-bit product of r0 and r1. The
@ Thumb entry switches to ARM halfway through with add r2, pc, #0; bx r2 to
@ use umull and returns with bx lr; neither approved compiler changes
@ instruction set inside a function or emits the Thumb add r2, pc, #0.
@ Every TBS and TLA edition has these bytes.
@ credit: library — TBS/TLA common sound-driver unsigned high-word multiply helper
.syntax unified
	.thumb
	.global Func_080f95e0
	.thumb_func
Func_080f95e0:
	add	r2, pc, #0
	bx	r2
	.arm
	umull	r2, r3, r0, r1
	add	r0, r3, #0
	bx	lr
