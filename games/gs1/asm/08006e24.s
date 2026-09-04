@ Retired from C: the registered source did not reproduce these bytes
@ under the family's flags. Disassembled from the audited extent at
@ 0x08006e24 (292 bytes); the draft C stays in games/gs1/recon/en/main/.
.syntax unified
	.thumb
	.global Func_08006e24
	.thumb_func
Func_08006e24:
	push	{r4, r5, r6, r7, lr}
	mov	r7, r9
	mov	r6, r8
	push	{r6, r7}
	sub	sp, #96	@ 0x60
	mov	r9, r1
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	mov	r8, r0
	cmp	r0, #15
	bls.n	.L_08006e44
	ldr	r0, [pc, #4]	@ (0x8006e40)
	b.n	.L_08006f32
	movs	r0, r0
	.2byte 0x80ff
	.2byte 0x0000
.L_08006e44:
	ldr	r0, [pc, #28]	@ (0x8006e64)
	ldr	r0, [r0, #0]
	ldrb	r0, [r0, #8]
	mov	r7, r8
	lsls	r7, r0
	movs	r0, #224	@ 0xe0
	lsls	r0, r0, #20
	adds	r7, r7, r0
	ldr	r1, [pc, #16]	@ (0x8006e68)
	movs	r0, #1
	adds	r3, r1, #0
	eors	r3, r0
	mov	r2, sp
	ldr	r0, [pc, #12]	@ (0x8006e6c)
	subs	r0, r0, r1
	b.n	.L_08006e7a
	.4byte 0x02004c08
	.4byte 0x08006f49
	.2byte 0x6f6d
	.2byte 0x0800
.L_08006e70:
	ldrh	r0, [r3, #0]
	strh	r0, [r2, #0]
	adds	r3, #2
	adds	r2, #2
	subs	r0, r1, #2
.L_08006e7a:
	lsls	r0, r0, #16
	lsrs	r1, r0, #16
	cmp	r1, #0
	bne.n	.L_08006e70
	movs	r4, #0
	b.n	.L_08006e90
.L_08006e86:
	adds	r0, r4, #1
	lsls	r0, r0, #24
	lsrs	r4, r0, #24
	cmp	r4, #81	@ 0x51
	beq.n	.L_08006f30
.L_08006e90:
	mov	r0, r8
	bl	Func_08006d50
	lsls	r0, r0, #16
	lsrs	r5, r0, #16
	cmp	r5, #0
	bne.n	.L_08006e86
	adds	r0, r7, #0
	mov	r1, sp
	adds	r1, #1
	bl	Func_08006f6c
	lsls	r0, r0, #16
	lsrs	r5, r0, #16
	cmp	r5, #0
	bne.n	.L_08006e86
	movs	r6, #1
	cmp	r4, #0
	beq.n	.L_08006eb8
	movs	r6, #6
.L_08006eb8:
	movs	r4, #1
	cmp	r4, r6
	bhi.n	.L_08006ece
.L_08006ebe:
	mov	r0, r8
	bl	Func_08006d50
	adds	r0, r4, #1
	lsls	r0, r0, #24
	lsrs	r4, r0, #24
	cmp	r4, r6
	bls.n	.L_08006ebe
.L_08006ece:
	mov	r0, sp
	bl	Func_08006ac0
	ldr	r3, [pc, #24]	@ (0x8006ef0)
	ldrh	r1, [r3, #0]
	ldr	r0, [pc, #24]	@ (0x8006ef4)
	ands	r1, r0
	ldr	r0, [pc, #24]	@ (0x8006ef8)
	ldr	r2, [r0, #0]
	ldrh	r0, [r2, #16]
	orrs	r0, r1
	strh	r0, [r3, #0]
	ldr	r1, [pc, #20]	@ (0x8006efc)
	ldr	r0, [r2, #4]
	strh	r0, [r1, #0]
	adds	r4, r1, #0
	b.n	.L_08006f0c
	.4byte 0x04000204
	.4byte 0x0000fffc
	.4byte 0x02004c08
	.2byte 0x4c0c
	.2byte 0x0200
.L_08006f00:
	ldrh	r0, [r4, #0]
	subs	r0, #1
	strh	r0, [r4, #0]
	movs	r0, #1
	add	r9, r0
	adds	r7, #1
.L_08006f0c:
	ldrh	r0, [r4, #0]
	cmp	r0, #0
	beq.n	.L_08006f22
	mov	r0, r9
	adds	r1, r7, #0
	bl	Func_08006dec
	lsls	r0, r0, #16
	lsrs	r5, r0, #16
	cmp	r5, #0
	beq.n	.L_08006f00
.L_08006f22:
	ldr	r2, [pc, #28]	@ (0x8006f40)
	ldrh	r0, [r2, #0]
	ldr	r1, [pc, #28]	@ (0x8006f44)
	ands	r0, r1
	movs	r1, #3
	orrs	r0, r1
	strh	r0, [r2, #0]
.L_08006f30:
	adds	r0, r5, #0
.L_08006f32:
	add	sp, #96	@ 0x60
	pop	{r3, r4}
	mov	r8, r3
	mov	r9, r4
	pop	{r4, r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x04000204
	.4byte 0x0000fffc
