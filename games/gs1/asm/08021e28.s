@ Retired from C: the registered source did not reproduce these bytes
@ under the family's flags. Disassembled from the audited extent at
@ 0x08021e28 (32 bytes); the draft C stays in games/gs1/recon/en/main/.
.syntax unified
	.thumb
	.global Func_08021e28
	.thumb_func
Func_08021e28:
	push	{lr}
	ldr	r2, [pc, #20]	@ (0x8021e40)
	movs	r3, #0
	strh	r3, [r2, #0]
	ldr	r2, [pc, #16]	@ (0x8021e44)
	movs	r0, #2
	movs	r1, #136	@ 0x88
	bl	Func_0800307c
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x04000012
	.4byte 0x08021e15
