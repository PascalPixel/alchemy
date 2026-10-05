.syntax unified
	.thumb
	.global Bios_SoundBiasOn
	.thumb_func
Bios_SoundBiasOn:
	movs	r0, #1
	svc	25
	bx	lr
