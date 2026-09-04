@ Retired from C: the registered source did not reproduce these bytes
@ under the family's flags. Disassembled from the audited extent at
@ 0x08007220 (196 bytes); the draft C stays in games/gs1/recon/en/main/.
.syntax unified
	.thumb
	.global Func_08007220
	.thumb_func
Func_08007220:
	push	{r4, r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	sub	sp, #64	@ 0x40
	adds	r7, r1, #0
	lsls	r0, r0, #16
	lsrs	r4, r0, #16
	cmp	r4, #15
	bls.n	.L_0800723c
	ldr	r0, [pc, #4]	@ (0x8007238)
	b.n	.L_080072ce
	movs	r0, r0
	.2byte 0x80ff
	.2byte 0x0000
.L_0800723c:
	mov	r0, sp
	bl	Func_08006ac0
	ldr	r2, [pc, #32]	@ (0x8007264)
	ldrh	r0, [r2, #0]
	ldr	r1, [pc, #32]	@ (0x8007268)
	ands	r0, r1
	ldr	r1, [pc, #32]	@ (0x800726c)
	ldrh	r1, [r1, #36]	@ 0x24
	orrs	r0, r1
	strh	r0, [r2, #0]
	lsls	r0, r4, #21
	lsrs	r5, r0, #16
	ldr	r1, [pc, #24]	@ (0x8007270)
	ldr	r0, [pc, #24]	@ (0x8007274)
	ldr	r0, [r0, #24]
	strh	r0, [r1, #0]
	adds	r0, r1, #0
	mov	r8, r0
	b.n	.L_0800728e
	.4byte 0x04000204
	.4byte 0x0000fffc
	.4byte 0x08007c10
	.4byte 0x02004c0c
	.2byte 0x7be4
	.2byte 0x0800
.L_08007278:
	ldr	r0, [pc, #32]	@ (0x800729c)
	ldr	r1, [r0, #24]
	mov	r2, r8
	ldrh	r2, [r2, #0]
	subs	r0, r2, r1
	mov	r3, r8
	strh	r0, [r3, #0]
	adds	r7, r7, r1
	adds	r0, r5, #1
	lsls	r0, r0, #16
	lsrs	r5, r0, #16
.L_0800728e:
	mov	r1, r8
	ldrh	r0, [r1, #0]
	cmp	r0, #0
	beq.n	.L_080072be
	movs	r4, #2
	b.n	.L_080072aa
	movs	r0, r0
	.2byte 0x7c10
	.2byte 0x0800
.L_080072a0:
	subs	r0, r4, #1
	lsls	r0, r0, #16
	lsrs	r4, r0, #16
	cmp	r4, #0
	beq.n	.L_080072ba
.L_080072aa:
	adds	r0, r5, #0
	adds	r1, r7, #0
	bl	Func_080071a8
	lsls	r0, r0, #16
	lsrs	r6, r0, #16
	cmp	r6, #0
	bne.n	.L_080072a0
.L_080072ba:
	cmp	r6, #0
	beq.n	.L_08007278
.L_080072be:
	ldr	r2, [pc, #28]	@ (0x80072dc)
	ldrh	r0, [r2, #0]
	ldr	r1, [pc, #28]	@ (0x80072e0)
	ands	r0, r1
	movs	r1, #3
	orrs	r0, r1
	strh	r0, [r2, #0]
	adds	r0, r6, #0
.L_080072ce:
	add	sp, #64	@ 0x40
	pop	{r3}
	mov	r8, r3
	pop	{r4, r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x04000204
	.4byte 0x0000fffc
