@ Retired from C: the registered source did not reproduce these bytes
@ under the family's flags. Disassembled from the audited extent at
@ 0x080b110c (152 bytes); the draft C stays in games/gs1/recon/en/main/.
.syntax unified
	.thumb
	.global Func_080b110c
	.thumb_func
Func_080b110c:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r6, r0, #0
	sub	sp, #4
	adds	r5, r1, #0
	mov	r8, r2
	adds	r7, r3, #0
	cmp	r6, #0
	bne.n	.L_080b1158
	b.n	.L_080b1186
.L_080b1122:
	ldr	r0, [pc, #112]	@ (0x80b1194)
	adds	r1, r6, #0
	movs	r2, #0
	b.n	.L_080b1150
.L_080b112a:
	ldr	r5, [pc, #108]	@ (0x80b1198)
	adds	r1, r6, #0
	adds	r0, r5, #0
	movs	r2, #0
	movs	r3, #8
	bl	Func_08015080
	movs	r3, #8
	str	r3, [sp, #0]
	mov	r0, r8
	movs	r1, #5
	adds	r2, r6, #0
	movs	r3, #32
	subs	r5, #3
	bl	Func_080150b0
	adds	r0, r5, #0
	adds	r1, r6, #0
	movs	r2, #72	@ 0x48
.L_080b1150:
	movs	r3, #8
	bl	Func_08015080
	b.n	.L_080b1186
.L_080b1158:
	adds	r0, r6, #0
	bl	Func_08015270
	ldr	r0, [pc, #60]	@ (0x80b119c)
	movs	r3, #0
	adds	r0, r5, r0
	adds	r1, r6, #0
	movs	r2, #0
	bl	Func_08015080
	mov	r3, r8
	cmp	r3, #0
	bne.n	.L_080b112a
	cmp	r7, #1
	beq.n	.L_080b1122
	cmp	r7, #2
	bne.n	.L_080b112a
	ldr	r0, [pc, #36]	@ (0x80b11a0)
	adds	r1, r6, #0
	movs	r2, #0
	movs	r3, #8
	bl	Func_08015080
.L_080b1186:
	add	sp, #4
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x00000c92
	.4byte 0x00000c8b
	.4byte 0x00000182
	.4byte 0x00000c93
