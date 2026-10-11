@ C2: svc 11 and svc 25 are BIOS software interrupts.
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
@ C2: svc 25 is a BIOS software interrupt.
.syntax unified
	.thumb
	.global Bios_SoundBiasOn
	.thumb_func
Bios_SoundBiasOn:
	movs	r0, #1
	svc	25
	bx	lr
