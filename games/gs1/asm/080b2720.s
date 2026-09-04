@ Retired from C: the registered source did not reproduce these bytes
@ under the family's flags. Disassembled from the audited extent at
@ 0x080b2720 (68 bytes); the draft C stays in games/gs1/recon/en/main/.
.syntax unified
	.thumb
	.global Func_080b2720
	.thumb_func
Func_080b2720:
	push	{r5, lr}
	lsls	r3, r0, #5
	adds	r3, r3, r0
	adds	r5, r1, #0
	ldr	r1, [pc, #52]	@ (0x80b2760)
	lsls	r2, r3, #1
	ldrsh	r3, [r1, r2]
	movs	r4, #0
	cmp	r3, #0
	beq.n	.L_080b274e
	adds	r0, r5, #0
	adds	r2, r2, r1
.L_080b2738:
	ldrh	r3, [r2, #0]
	adds	r4, #1
	strh	r3, [r0, #0]
	adds	r2, #2
	adds	r0, #2
	cmp	r4, #23
	bgt.n	.L_080b274e
	movs	r1, #0
	ldrsh	r3, [r2, r1]
	cmp	r3, #0
	bne.n	.L_080b2738
.L_080b274e:
	ldr	r3, [pc, #12]	@ (0x80b275c)
	lsls	r2, r4, #1
	strh	r3, [r2, r5]
	adds	r0, r4, #0
	pop	{r5}
	pop	{r1}
	bx	r1
	.4byte 0x00000000
	.4byte 0x080b41ac
