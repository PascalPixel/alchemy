@ Unsigned high-word multiply for the sound driver: add r2, pc, #0; bx r2
@ switches to ARM for umull, which returns the high word. Neither approved
@ compiler emits the Thumb add r2, pc, #0 or changes instruction set inside
@ a function. These 16 bytes equal TBS SOUND/DRIVER/UMUL_HIGH.S
@ (0x080f95e0).
@ credit: library — TBS/TLA common sound-driver unsigned high-word multiply helper
.syntax unified
	.thumb
	.set AlchemyLoadAddress, 0x081c127c
	.global SoundDriver_UnsignedMulHigh
	.thumb_func
SoundDriver_UnsignedMulHigh:
	add	r2, pc, #0
	bx	r2
	.arm
	umull	r2, r3, r0, r1
	add	r0, r3, #0
	bx	lr
