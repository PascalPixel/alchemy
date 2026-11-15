@ Why Bios_CpuSet to Bios_SoundBiasOff are assembly:
@ svc 11 and svc 25 are BIOS software interrupts.
@ Why assembly: svc 25 is a BIOS software interrupt.
	.section .text,"ax",%progbits
	.balign 2
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
	.balign 4

	.section .text.bios_sound_bias_on,"ax",%progbits
	.balign 2
.syntax unified
	.thumb
	.global Bios_SoundBiasOn
	.thumb_func
Bios_SoundBiasOn:
	movs	r0, #1
	svc	25
	bx	lr
