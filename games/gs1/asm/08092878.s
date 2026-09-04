@ Retired from C: the registered source did not reproduce these bytes
@ under the family's flags. Disassembled from the audited extent at
@ 0x08092878 (172 bytes); the draft C stays in games/gs1/recon/en/main/.
.syntax unified
	.thumb
	.global Func_08092878
	.thumb_func
Func_08092878:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r6, r0, #0
	sub	sp, #4
	adds	r5, r1, #0
	cmp	r6, #0
	beq.n	.L_08092912
	cmp	r5, #0
	beq.n	.L_08092912
	ldr	r3, [r6, #16]
	ldr	r0, [r5, #16]
	ldr	r1, [r5, #8]
	subs	r0, r0, r3
	ldr	r3, [r6, #8]
	subs	r1, r1, r3
	bl	Func_080044d0
	lsls	r0, r0, #16
	lsrs	r7, r0, #16
	movs	r0, #128	@ 0x80
	lsls	r0, r0, #8
	adds	r0, r0, r7
	mov	r8, r0
	movs	r4, #0
.L_080928aa:
	ldrh	r2, [r6, #6]
	subs	r3, r7, r2
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	movs	r1, #2
	cmp	r3, #0
	beq.n	.L_080928d2
	movs	r0, #128	@ 0x80
	lsls	r0, r0, #5
	cmp	r3, r0
	ble.n	.L_080928c4
	movs	r3, #128	@ 0x80
	lsls	r3, r3, #5
.L_080928c4:
	ldr	r0, [pc, #88]	@ (0x8092920)
	cmp	r3, r0
	bge.n	.L_080928cc
	ldr	r3, [pc, #84]	@ (0x8092920)
.L_080928cc:
	adds	r3, r2, r3
	strh	r3, [r6, #6]
	b.n	.L_080928d4
.L_080928d2:
	movs	r1, #1
.L_080928d4:
	ldrh	r2, [r5, #6]
	mov	r0, r8
	subs	r3, r0, r2
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	cmp	r3, #0
	beq.n	.L_080928fc
	movs	r0, #128	@ 0x80
	lsls	r0, r0, #5
	cmp	r3, r0
	ble.n	.L_080928ee
	movs	r3, #128	@ 0x80
	lsls	r3, r3, #5
.L_080928ee:
	ldr	r0, [pc, #48]	@ (0x8092920)
	cmp	r3, r0
	bge.n	.L_080928f6
	ldr	r3, [pc, #40]	@ (0x8092920)
.L_080928f6:
	adds	r3, r2, r3
	strh	r3, [r5, #6]
	b.n	.L_080928fe
.L_080928fc:
	subs	r1, #1
.L_080928fe:
	cmp	r1, #0
	beq.n	.L_08092912
	movs	r0, #1
	str	r4, [sp, #0]
	bl	Func_080030f8
	ldr	r4, [sp, #0]
	adds	r4, #1
	cmp	r4, #59	@ 0x3b
	ble.n	.L_080928aa
.L_08092912:
	add	sp, #4
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0xfffff000
