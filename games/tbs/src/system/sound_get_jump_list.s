@ C2: svc 42 is a BIOS software interrupt.
.syntax unified
	.thumb
	.global Bios_SoundGetJumpList
	.thumb_func
Bios_SoundGetJumpList:
	svc	42
	bx	lr
