@ Retired from C: the registered source did not reproduce these bytes
@ under the family's flags. Disassembled from the audited extent at
@ 0x0800711c (140 bytes); the draft C stays in games/gs1/recon/en/main/.
.syntax unified
	.thumb
	.global Func_0800711c
	.thumb_func
Func_0800711c:
	push	{r4, r5, r6, lr}
	sub	sp, #64	@ 0x40
	lsls	r0, r0, #16
	lsrs	r4, r0, #16
	cmp	r4, #15
	bls.n	.L_08007130
	ldr	r0, [pc, #0]	@ (0x800712c)
	b.n	.L_08007198
	.2byte 0x80ff
	.2byte 0x0000
.L_08007130:
	mov	r0, sp
	bl	Func_08006ac0
	ldr	r2, [pc, #24]	@ (0x8007150)
	ldrh	r0, [r2, #0]
	ldr	r1, [pc, #24]	@ (0x8007154)
	ands	r0, r1
	ldr	r1, [pc, #24]	@ (0x8007158)
	ldrh	r1, [r1, #36]	@ 0x24
	orrs	r0, r1
	strh	r0, [r2, #0]
	lsls	r0, r4, #21
	lsrs	r5, r0, #16
	movs	r6, #0
.L_0800714c:
	movs	r4, #2
	b.n	.L_08007166
	.4byte 0x04000204
	.4byte 0x0000fffc
	.2byte 0x7c10
	.2byte 0x0800
.L_0800715c:
	subs	r0, r4, #1
	lsls	r0, r0, #16
	lsrs	r4, r0, #16
	cmp	r4, #0
	beq.n	.L_08007174
.L_08007166:
	adds	r0, r5, #0
	bl	Func_08007098
	lsls	r0, r0, #16
	lsrs	r3, r0, #16
	cmp	r3, #0
	bne.n	.L_0800715c
.L_08007174:
	adds	r0, r5, #1
	lsls	r0, r0, #16
	lsrs	r5, r0, #16
	cmp	r3, #0
	bne.n	.L_08007188
	adds	r0, r6, #1
	lsls	r0, r0, #16
	lsrs	r6, r0, #16
	cmp	r6, #31
	bls.n	.L_0800714c
.L_08007188:
	ldr	r2, [pc, #20]	@ (0x80071a0)
	ldrh	r0, [r2, #0]
	ldr	r1, [pc, #20]	@ (0x80071a4)
	ands	r0, r1
	movs	r1, #3
	orrs	r0, r1
	strh	r0, [r2, #0]
	adds	r0, r3, #0
.L_08007198:
	add	sp, #64	@ 0x40
	pop	{r4, r5, r6}
	pop	{r1}
	bx	r1
	.4byte 0x04000204
	.4byte 0x0000fffc
