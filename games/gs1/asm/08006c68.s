@ Retired from C: the registered source did not reproduce these bytes
@ under the family's flags. Disassembled from the audited extent at
@ 0x08006c68 (116 bytes); the draft C stays in games/gs1/recon/en/main/.
.syntax unified
	.thumb
	.global Func_08006c68
	.thumb_func
Func_08006c68:
	push	{r4, r5, lr}
	sub	sp, #256	@ 0x100
	adds	r5, r1, #0
	lsls	r0, r0, #16
	lsrs	r4, r0, #16
	ldr	r2, [pc, #32]	@ (0x8006c94)
	ldrh	r0, [r2, #0]
	ldr	r1, [pc, #32]	@ (0x8006c98)
	ands	r0, r1
	movs	r1, #3
	orrs	r0, r1
	strh	r0, [r2, #0]
	ldr	r3, [pc, #24]	@ (0x8006c9c)
	movs	r0, #1
	eors	r3, r0
	mov	r2, sp
	ldr	r0, [pc, #20]	@ (0x8006ca0)
	ldr	r1, [pc, #16]	@ (0x8006c9c)
	subs	r0, r0, r1
	lsls	r0, r0, #15
	b.n	.L_08006cb0
	movs	r0, r0
	.4byte 0x04000204
	.4byte 0x0000fffc
	.4byte 0x08006c25
	.2byte 0x6c69
	.2byte 0x0800
.L_08006ca4:
	ldrh	r0, [r3, #0]
	strh	r0, [r2, #0]
	adds	r3, #2
	adds	r2, #2
	subs	r0, r1, #1
	lsls	r0, r0, #16
.L_08006cb0:
	lsrs	r1, r0, #16
	cmp	r1, #0
	bne.n	.L_08006ca4
	mov	r3, sp
	adds	r3, #1
	ldr	r0, [pc, #28]	@ (0x8006cd8)
	ldrb	r1, [r0, #28]
	lsls	r4, r1
	adds	r1, r4, #0
	movs	r2, #224	@ 0xe0
	lsls	r2, r2, #20
	adds	r1, r1, r2
	ldrh	r2, [r0, #24]
	adds	r0, r5, #0
	bl	Func_080072f0
	add	sp, #256	@ 0x100
	pop	{r4, r5}
	pop	{r1}
	bx	r1
	.4byte 0x08007abc
