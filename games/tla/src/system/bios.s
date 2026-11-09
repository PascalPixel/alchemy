.syntax unified
	.thumb
	.global Bios_CpuSet
	.thumb_func
Bios_CpuSet:
	svc	11
	bx	lr
	.global Bios_SoundBiasOff
	.thumb_func
Bios_SoundBiasOff:
	movs	r0, #0
	svc	25
	bx	lr

	.section .padding,"a",%progbits
	.balign 4
