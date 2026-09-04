@ Retired from C: the registered source did not reproduce these bytes
@ under the family's flags. Disassembled from the audited extent at
@ 0x080f9a50 (46 bytes); the draft C stays in games/gs1/recon/en/main/.
.syntax unified
	.thumb
	.global Func_080f9a50
	.thumb_func
Func_080f9a50:
	push	{r4, r5, lr}
	adds	r5, r1, #0
	ldr	r4, [r5, #32]
	cmp	r4, #0
	beq.n	.L_080f9a74
.L_080f9a5a:
	ldrb	r1, [r4, #0]
	movs	r0, #199	@ 0xc7
	tst	r0, r1
	beq.n	.L_080f9a68
	movs	r0, #64	@ 0x40
	orrs	r1, r0
	strb	r1, [r4, #0]
.L_080f9a68:
	adds	r0, r4, #0
	bl	Func_080f9a30
	ldr	r4, [r4, #52]	@ 0x34
	cmp	r4, #0
	bne.n	.L_080f9a5a
.L_080f9a74:
	movs	r0, #0
	strb	r0, [r5, #0]
	pop	{r4, r5}
	pop	{r0}
	bx	r0
