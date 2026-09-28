@ BIOS SoundGetJumpList wrapper: software interrupt 0x2a, then return.
@ Neither approved compiler emits svc. Every TBS and TLA edition has these
@ bytes (TLA 0x081c2310).
@ credit: library — TBS/TLA common BIOS SoundGetJumpList wrapper
.syntax unified
	.thumb
	.global Bios_SoundGetJumpList
	.thumb_func
Bios_SoundGetJumpList:
	svc	42
	bx	lr
