@ Retired from C: the registered source did not reproduce these bytes
@ under the family's flags. Disassembled from the audited extent at
@ 0x080a3d9c (64 bytes); the draft C stays in games/gs1/recon/en/main/.
.syntax unified
	.thumb
	.global Func_080a3d9c
	.thumb_func
Func_080a3d9c:
	push	{r5, r6, lr}
	adds	r6, r1, #0
	bl	Func_08077008
	ldr	r4, [pc, #48]	@ (0x80a3dd8)
	movs	r5, #0
	movs	r1, #0
	adds	r0, #216	@ 0xd8
.L_080a3dac:
	ldrh	r2, [r0, #0]
	adds	r3, r2, #0
	adds	r0, #2
	cmp	r3, #0
	beq.n	.L_080a3dca
	adds	r3, r4, #0
	ands	r3, r2
	cmp	r3, r6
	bne.n	.L_080a3dca
	movs	r3, #248	@ 0xf8
	lsls	r3, r3, #8
	ands	r3, r2
	lsrs	r5, r3, #11
	adds	r5, #1
	b.n	.L_080a3dd0
.L_080a3dca:
	adds	r1, #1
	cmp	r1, #14
	ble.n	.L_080a3dac
.L_080a3dd0:
	adds	r0, r5, #0
	pop	{r5, r6}
	pop	{r1}
	bx	r1
	.4byte 0x000001ff
