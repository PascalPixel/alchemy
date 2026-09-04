@ Retired from C: the registered source did not reproduce these bytes
@ under the family's flags. Disassembled from the audited extent at
@ 0x08007098 (132 bytes); the draft C stays in games/gs1/recon/en/main/.
.syntax unified
	.thumb
	.global Func_08007098
	.thumb_func
Func_08007098:
	push	{r4, r5, lr}
	lsls	r0, r0, #16
	ldr	r3, [pc, #104]	@ (0x8007108)
	lsrs	r0, r0, #16
	adds	r4, r0, #0
	ldrb	r1, [r3, #28]
	lsls	r4, r1
	movs	r0, #224	@ 0xe0
	lsls	r0, r0, #20
	adds	r4, r4, r0
	ldr	r1, [pc, #92]	@ (0x800710c)
	ldrh	r0, [r1, #0]
	adds	r5, r0, #0
	movs	r0, #0
	strh	r0, [r1, #0]
	ldr	r2, [pc, #88]	@ (0x8007110)
	movs	r0, #170	@ 0xaa
	strb	r0, [r2, #0]
	ldr	r1, [pc, #84]	@ (0x8007114)
	movs	r0, #85	@ 0x55
	strb	r0, [r1, #0]
	movs	r0, #160	@ 0xa0
	strb	r0, [r2, #0]
	ldr	r0, [r3, #24]
	cmp	r0, #0
	beq.n	.L_080070d8
	movs	r1, #255	@ 0xff
.L_080070ce:
	strb	r1, [r4, #0]
	adds	r4, #1
	subs	r0, #1
	cmp	r0, #0
	bne.n	.L_080070ce
.L_080070d8:
	subs	r4, #1
	ldr	r0, [pc, #48]	@ (0x800710c)
	strh	r5, [r0, #0]
	ldr	r0, [pc, #56]	@ (0x8007118)
	ldr	r3, [r0, #0]
	movs	r0, #1
	adds	r1, r4, #0
	movs	r2, #255	@ 0xff
	bl	Func_080072f0
	lsls	r0, r0, #16
	lsrs	r1, r0, #16
	cmp	r1, #0
	beq.n	.L_080070fe
	movs	r0, #255	@ 0xff
	lsls	r0, r0, #8
	ands	r1, r0
	movs	r0, #2
	orrs	r1, r0
.L_080070fe:
	adds	r0, r1, #0
	pop	{r4, r5}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x08007c10
	.4byte 0x04000208
	.4byte 0x0e005555
	.4byte 0x0e002aaa
	.4byte 0x02004c00
