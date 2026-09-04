@ Retired from C: the registered source did not reproduce these bytes
@ under the family's flags. Disassembled from the audited extent at
@ 0x080fa280 (32 bytes); the draft C stays in games/gs1/recon/en/main/.
.syntax unified
	.thumb
	.global Func_080fa280
	.thumb_func
Func_080fa280:
	adds	r2, r0, #0
	lsls	r1, r1, #16
	lsrs	r1, r1, #16
	ldr	r3, [r2, #52]	@ 0x34
	ldr	r0, [pc, #16]	@ (0x80fa29c)
	cmp	r3, r0
	bne.n	.L_080fa298
	strh	r1, [r2, #38]	@ 0x26
	strh	r1, [r2, #36]	@ 0x24
	movs	r0, #128	@ 0x80
	lsls	r0, r0, #1
	strh	r0, [r2, #40]	@ 0x28
.L_080fa298:
	bx	lr
	movs	r0, r0
	.4byte 0x68736d53
