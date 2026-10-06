.syntax unified
	.thumb
	.global Bios_SoundGetJumpList
	.thumb_func
Bios_SoundGetJumpList:
	svc	42
	bx	lr
