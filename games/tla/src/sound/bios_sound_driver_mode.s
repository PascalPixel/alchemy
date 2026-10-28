.syntax unified
	.thumb
	.global Bios_SoundDriverMode
	.thumb_func
Bios_SoundDriverMode:
	svc	27
	bx	lr
	.global Bios_Swi23
	.thumb_func
Bios_Swi23:
	svc	35
	bx	lr
