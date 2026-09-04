@ Retired from C: the registered source did not reproduce these bytes
@ under the family's flags. Disassembled from the audited extent at
@ 0x08006878 (150 bytes); the draft C stays in games/gs1/recon/en/main/.
.syntax unified
	.thumb
	.global Func_08006878
	.thumb_func
Func_08006878:
	push	{r4, r5, lr}
	sub	sp, #68	@ 0x44
	mov	r0, sp
	bl	Func_08006ac0
	mov	r5, sp
	adds	r5, #1
	ldr	r2, [pc, #24]	@ (0x80068a0)
	movs	r0, #170	@ 0xaa
	strb	r0, [r2, #0]
	ldr	r1, [pc, #20]	@ (0x80068a4)
	movs	r0, #85	@ 0x55
	strb	r0, [r1, #0]
	movs	r0, #144	@ 0x90
	strb	r0, [r2, #0]
	add	r1, sp, #64	@ 0x40
	ldr	r2, [pc, #12]	@ (0x80068a8)
	adds	r0, r2, #0
	b.n	.L_080068b0
	movs	r0, r0
	.4byte 0x0e005555
	.4byte 0x0e002aaa
	.2byte 0x4e20
	.2byte 0x0000
.L_080068ac:
	ldrh	r0, [r1, #0]
	subs	r0, #1
.L_080068b0:
	strh	r0, [r1, #0]
	ldrh	r0, [r1, #0]
	cmp	r0, #0
	bne.n	.L_080068ac
	ldr	r0, [pc, #44]	@ (0x80068e8)
	bl	Func_080072f8
	lsls	r0, r0, #24
	lsrs	r4, r0, #16
	movs	r0, #224	@ 0xe0
	lsls	r0, r0, #20
	bl	Func_080072f8
	lsls	r0, r0, #24
	lsrs	r0, r0, #24
	orrs	r4, r0
	ldr	r2, [pc, #24]	@ (0x80068ec)
	movs	r0, #170	@ 0xaa
	strb	r0, [r2, #0]
	ldr	r1, [pc, #24]	@ (0x80068f0)
	movs	r0, #85	@ 0x55
	strb	r0, [r1, #0]
	movs	r0, #240	@ 0xf0
	strb	r0, [r2, #0]
	add	r1, sp, #64	@ 0x40
	ldr	r2, [pc, #16]	@ (0x80068f4)
	adds	r0, r2, #0
	b.n	.L_080068fc
	.4byte 0x0e000001
	.4byte 0x0e005555
	.4byte 0x0e002aaa
	.2byte 0x4e20
	.2byte 0x0000
.L_080068f8:
	ldrh	r0, [r1, #0]
	subs	r0, #1
.L_080068fc:
	strh	r0, [r1, #0]
	ldrh	r0, [r1, #0]
	cmp	r0, #0
	bne.n	.L_080068f8
	adds	r0, r4, #0
	add	sp, #68	@ 0x44
	pop	{r4, r5}
	pop	{r1}
	bx	r1
