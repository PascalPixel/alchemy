@ Why BiosSoundDriverMode to BiosSwi23 are assembly:
@ svc 27 and svc 35 are BIOS software interrupts.
.syntax unified
	.thumb
	.global BiosSoundDriverMode
	.thumb_func
BiosSoundDriverMode:
	svc	27
	bx	lr
	.global BiosSwi23
	.thumb_func
BiosSwi23:
	svc	35
	bx	lr
