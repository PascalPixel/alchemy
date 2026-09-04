@ Retired from C: the registered source did not reproduce these bytes
@ under the family's flags. Disassembled from the audited extent at
@ 0x080071a8 (120 bytes); the draft C stays in games/gs1/recon/en/main/.
.syntax unified
	.thumb
	.global Func_080071a8
	.thumb_func
Func_080071a8:
	push	{r4, r5, r6, lr}
	adds	r5, r1, #0
	lsls	r0, r0, #16
	ldr	r3, [pc, #92]	@ (0x800720c)
	lsrs	r0, r0, #16
	adds	r4, r0, #0
	ldrb	r1, [r3, #28]
	lsls	r4, r1
	movs	r0, #224	@ 0xe0
	lsls	r0, r0, #20
	adds	r4, r4, r0
	ldr	r1, [pc, #80]	@ (0x8007210)
	ldrh	r0, [r1, #0]
	adds	r6, r0, #0
	movs	r0, #0
	strh	r0, [r1, #0]
	ldr	r2, [pc, #72]	@ (0x8007214)
	movs	r0, #170	@ 0xaa
	strb	r0, [r2, #0]
	ldr	r1, [pc, #72]	@ (0x8007218)
	movs	r0, #85	@ 0x55
	strb	r0, [r1, #0]
	movs	r0, #160	@ 0xa0
	strb	r0, [r2, #0]
	ldr	r1, [r3, #24]
	cmp	r1, #0
	beq.n	.L_080071ec
.L_080071de:
	ldrb	r0, [r5, #0]
	strb	r0, [r4, #0]
	adds	r5, #1
	adds	r4, #1
	subs	r1, #1
	cmp	r1, #0
	bne.n	.L_080071de
.L_080071ec:
	subs	r4, #1
	subs	r5, #1
	ldr	r0, [pc, #28]	@ (0x8007210)
	strh	r6, [r0, #0]
	ldr	r0, [pc, #36]	@ (0x800721c)
	ldrb	r2, [r5, #0]
	ldr	r3, [r0, #0]
	movs	r0, #1
	adds	r1, r4, #0
	bl	Func_080072f0
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	pop	{r4, r5, r6}
	pop	{r1}
	bx	r1
	.4byte 0x08007c10
	.4byte 0x04000208
	.4byte 0x0e005555
	.4byte 0x0e002aaa
	.4byte 0x02004c00
