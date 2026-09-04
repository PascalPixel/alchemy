@ Retired from C: the registered source did not reproduce these bytes
@ under the family's flags. Disassembled from the audited extent at
@ 0x080fb670 (32 bytes); the draft C stays in games/gs1/recon/en/main/.
.syntax unified
	.thumb
	.global Func_080fb670
	.thumb_func
Func_080fb670:
	push	{lr}
	ldr	r2, [r1, #64]	@ 0x40
	ldrb	r3, [r2, #0]
	adds	r2, #1
	str	r2, [r1, #64]	@ 0x40
	ldr	r2, [pc, #16]	@ (0x80fb68c)
	lsls	r3, r3, #2
	adds	r3, r3, r2
	ldr	r2, [r3, #0]
	bl	Func_080072ec
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x080fba48
