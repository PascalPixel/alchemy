@ Retired from C: the registered source did not reproduce these bytes
@ under the family's flags. Disassembled from the audited extent at
@ 0x080fb77c (18 bytes); the draft C stays in games/gs1/recon/en/main/.
.syntax unified
	.thumb
	.global Func_080fb77c
	.thumb_func
Func_080fb77c:
	ldr	r0, [r1, #64]	@ 0x40
	ldrb	r0, [r0, #0]
	adds	r2, r1, #0
	adds	r2, #39	@ 0x27
	strb	r0, [r2, #0]
	ldr	r0, [r1, #64]	@ 0x40
	adds	r0, #1
	str	r0, [r1, #64]	@ 0x40
	bx	lr
