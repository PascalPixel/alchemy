@ Retired from C: the registered source did not reproduce these bytes
@ under the family's flags. Disassembled from the audited extent at
@ 0x08006f84 (164 bytes); the draft C stays in games/gs1/recon/en/main/.
.syntax unified
	.thumb
	.global Func_08006f84
	.thumb_func
Func_08006f84:
	push	{r4, r5, r6, r7, lr}
	sub	sp, #64	@ 0x40
	adds	r7, r1, #0
	lsls	r0, r0, #16
	lsrs	r4, r0, #16
	cmp	r4, #15
	bls.n	.L_08006f9c
	ldr	r0, [pc, #4]	@ (0x8006f98)
	b.n	.L_08007018
	movs	r0, r0
	.2byte 0x80ff
	.2byte 0x0000
.L_08006f9c:
	adds	r0, r4, #0
	bl	Func_08006d50
	lsls	r0, r0, #16
	lsrs	r5, r0, #16
	cmp	r5, #0
	bne.n	.L_08007016
	mov	r0, sp
	bl	Func_08006ac0
	ldr	r3, [pc, #36]	@ (0x8006fd8)
	ldrh	r1, [r3, #0]
	ldr	r0, [pc, #36]	@ (0x8006fdc)
	ands	r1, r0
	ldr	r0, [pc, #36]	@ (0x8006fe0)
	ldr	r2, [r0, #0]
	ldrh	r0, [r2, #16]
	orrs	r0, r1
	strh	r0, [r3, #0]
	ldr	r1, [pc, #32]	@ (0x8006fe4)
	ldr	r0, [r2, #4]
	strh	r0, [r1, #0]
	ldrb	r0, [r2, #8]
	lsls	r4, r0
	movs	r0, #224	@ 0xe0
	lsls	r0, r0, #20
	adds	r4, r4, r0
	adds	r6, r1, #0
	b.n	.L_08006ff2
	movs	r0, r0
	.4byte 0x04000204
	.4byte 0x0000fffc
	.4byte 0x02004c08
	.2byte 0x4c0c
	.2byte 0x0200
.L_08006fe8:
	ldrh	r0, [r6, #0]
	subs	r0, #1
	strh	r0, [r6, #0]
	adds	r7, #1
	adds	r4, #1
.L_08006ff2:
	ldrh	r0, [r6, #0]
	cmp	r0, #0
	beq.n	.L_08007008
	adds	r0, r7, #0
	adds	r1, r4, #0
	bl	Func_08006dec
	lsls	r0, r0, #16
	lsrs	r5, r0, #16
	cmp	r5, #0
	beq.n	.L_08006fe8
.L_08007008:
	ldr	r2, [pc, #20]	@ (0x8007020)
	ldrh	r0, [r2, #0]
	ldr	r1, [pc, #20]	@ (0x8007024)
	ands	r0, r1
	movs	r1, #3
	orrs	r0, r1
	strh	r0, [r2, #0]
.L_08007016:
	adds	r0, r5, #0
.L_08007018:
	add	sp, #64	@ 0x40
	pop	{r4, r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x04000204
	.4byte 0x0000fffc
