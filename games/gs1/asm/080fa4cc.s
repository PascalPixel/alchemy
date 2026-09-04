@ Retired from C: the registered source did not reproduce these bytes
@ under the family's flags. Disassembled from the audited extent at
@ 0x080fa4cc (72 bytes); the draft C stays in games/gs1/recon/en/main/.
.syntax unified
	.thumb
	.global Func_080fa4cc
	.thumb_func
Func_080fa4cc:
	adds	r2, r0, #0
	lsls	r1, r1, #16
	lsrs	r1, r1, #16
	ldr	r3, [r2, #52]	@ 0x34
	ldr	r0, [pc, #12]	@ (0x80fa4e4)
	cmp	r3, r0
	bne.n	.L_080fa4e2
	strh	r1, [r2, #38]	@ 0x26
	strh	r1, [r2, #36]	@ 0x24
	ldr	r0, [pc, #8]	@ (0x80fa4e8)
	strh	r0, [r2, #40]	@ 0x28
.L_080fa4e2:
	bx	lr
	.4byte 0x68736d53
	.2byte 0x0101
	.2byte 0x0000
	adds	r2, r0, #0
	lsls	r1, r1, #16
	lsrs	r1, r1, #16
	ldr	r3, [r2, #52]	@ 0x34
	ldr	r0, [pc, #20]	@ (0x80fa50c)
	cmp	r3, r0
	bne.n	.L_080fa50a
	strh	r1, [r2, #38]	@ 0x26
	strh	r1, [r2, #36]	@ 0x24
	movs	r0, #2
	strh	r0, [r2, #40]	@ 0x28
	ldr	r0, [r2, #4]
	ldr	r1, [pc, #8]	@ (0x80fa510)
	ands	r0, r1
	str	r0, [r2, #4]
.L_080fa50a:
	bx	lr
	.4byte 0x68736d53
	.4byte 0x7fffffff
