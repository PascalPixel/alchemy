@ Why BiosCpuSet to BiosSoundBiasOff are assembly:
@ svc 11 and svc 25 are BIOS software interrupts.
@ Why BiosSoundBiasOn is assembly:
@ svc 25 is a BIOS software interrupt.
.syntax unified
	.thumb
	.global BiosCpuSet
	.thumb_func
BiosCpuSet:
	svc	11
	bx	lr
	.global BiosSoundBiasOff
	.thumb_func
BiosSoundBiasOff:
	movs	r0, #0
	svc	25
	bx	lr

	.section .padding,"a",%progbits
	.balign 4
.syntax unified
	.section .text.sound_vsync_on,"ax",%progbits
	.thumb
	.global BiosSoundBiasOn
	.thumb_func
BiosSoundBiasOn:
	movs	r0, #1
	svc	25
	bx	lr
