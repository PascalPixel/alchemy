@ Why Bios_CpuSet to Bios_SoundBiasOff are assembly:
@ svc 11 and svc 25 are BIOS software interrupts.
@ Why Bios_SoundBiasOn is assembly:
@ svc 25 is a BIOS software interrupt.
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
.syntax unified
	.section .text.sound_vsync_on,"ax",%progbits
	.thumb
	.global Bios_SoundBiasOn
	.thumb_func
Bios_SoundBiasOn:
	movs	r0, #1
	svc	25
	bx	lr
