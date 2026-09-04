@ Retired from C: the registered source did not reproduce these bytes
@ under the family's flags. Disassembled from the audited extent at
@ 0x080d40ec (184 bytes); the draft C stays in games/gs1/recon/en/main/.
.syntax unified
	.thumb
	.global Func_080d40ec
	.thumb_func
Func_080d40ec:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	adds	r5, r0, #0
	lsls	r5, r5, #10
	adds	r1, r5, r1
	adds	r0, r1, #0
	sub	sp, #128	@ 0x80
	adds	r6, r2, #0
	mov	r8, r3
	bl	Func_08002322
	lsls	r0, r0, #4
	adds	r6, r5, r6
	asrs	r0, r0, #15
	mov	sl, r0
	adds	r0, r6, #0
	bl	Func_08002322
	add	r5, r8
	lsls	r0, r0, #4
	asrs	r7, r0, #15
	adds	r0, r5, #0
	bl	Func_08002322
	movs	r3, #0
	mov	r2, sp
	lsls	r0, r0, #4
	mov	r6, sp
	strh	r3, [r2, #0]
	asrs	r0, r0, #15
	movs	r5, #1
	adds	r6, #2
.L_080d4130:
	mov	r2, sl
	adds	r3, r5, r2
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r4, r3, #1
	adds	r3, r5, r7
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r1, r3, #1
	adds	r3, r5, r0
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	cmp	r4, #0
	bge.n	.L_080d4150
	movs	r4, #0
.L_080d4150:
	cmp	r4, #31
	ble.n	.L_080d4156
	movs	r4, #31
.L_080d4156:
	cmp	r1, #0
	bge.n	.L_080d415c
	movs	r1, #0
.L_080d415c:
	cmp	r1, #31
	ble.n	.L_080d4162
	movs	r1, #31
.L_080d4162:
	cmp	r3, #0
	bge.n	.L_080d4168
	movs	r3, #0
.L_080d4168:
	cmp	r3, #31
	ble.n	.L_080d416e
	movs	r3, #31
.L_080d416e:
	lsls	r3, r3, #10
	lsls	r2, r1, #5
	orrs	r3, r2
	orrs	r3, r4
	adds	r5, #1
	strh	r3, [r6, #0]
	adds	r6, #2
	cmp	r5, #64	@ 0x40
	bne.n	.L_080d4130
	mov	r1, sp
	ldr	r3, [pc, #24]	@ (0x80d419c)
	movs	r2, #128	@ 0x80
	ldr	r0, [pc, #24]	@ (0x80d41a0)
	bl	Func_080072f0
	add	sp, #128	@ 0x80
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001388
	.4byte 0x05000002
