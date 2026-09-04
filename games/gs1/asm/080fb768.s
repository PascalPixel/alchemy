@ Retired from C: the registered source did not reproduce these bytes
@ under the family's flags. Disassembled from the audited extent at
@ 0x080fb768 (18 bytes); the draft C stays in games/gs1/recon/en/main/.
.syntax unified
	.thumb
	.global Func_080fb768
	.thumb_func
Func_080fb768:
	ldr	r0, [r1, #64]	@ 0x40
	ldrb	r0, [r0, #0]
	adds	r2, r1, #0
	adds	r2, #38	@ 0x26
	strb	r0, [r2, #0]
	ldr	r0, [r1, #64]	@ 0x40
	adds	r0, #1
	str	r0, [r1, #64]	@ 0x40
	bx	lr
