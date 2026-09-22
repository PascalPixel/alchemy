.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_08016ca4, 0x08016ca4
	.set sub_080ad3a8, 0x080ad3a8
	.set sub_080af79c, 0x080af79c
	.global Overlay_080affac
Overlay_080affac:
.L_080affac:
	push	{r5, r6, lr}
	adds	r5, r0, #0
	adds	r6, r2, #0
	cmp	r5, #7
	ble.n	.L_080affe8
	bl	sub_080ad3a8
	adds	r0, #42
	ldrb	r1, [r0, #0]
	cmp	r1, #47
	bls.n	.L_080affc4
	movs	r1, #0
.L_080affc4:
	lsls	r3, r1, #1
	ldr	r2, [pc, #92]
	adds	r3, r3, r1
	lsls	r3, r3, #3
	adds	r3, r3, r2
	movs	r4, #0
	adds	r0, r6, #0
	adds	r1, r3, #4
.L_080affd4:
	ldrb	r2, [r1, #0]
	adds	r4, #1
	lsls	r3, r2, #2
	adds	r3, r3, r2
	lsls	r3, r3, #1
	adds	r1, #1
	stmia	r0!, {r3}
	cmp	r4, #3
	ble.n	.L_080affd4
	b.n	.L_080b0020
.L_080affe8:
	adds	r0, r6, #0
	adds	r1, #36
	movs	r4, #3
.L_080affee:
	ldrb	r2, [r1, #0]
	subs	r4, #1
	lsls	r3, r2, #2
	adds	r3, r3, r2
	lsls	r3, r3, #1
	adds	r1, #1
	stmia	r0!, {r3}
	cmp	r4, #0
	bge.n	.L_080affee
	cmp	r5, #7
	bgt.n	.L_080b0020
	adds	r0, r5, #0
	bl	sub_080af79c
	adds	r1, r6, #0
	adds	r0, #146
	movs	r4, #3
.L_080b0010:
	ldrb	r2, [r0, #0]
	ldr	r3, [r1, #0]
	subs	r4, #1
	adds	r3, r3, r2
	adds	r0, #1
	stmia	r1!, {r3}
	cmp	r4, #0
	bge.n	.L_080b0010
.L_080b0020:
	movs	r0, #0
	pop	{r5, r6, pc}
	.2byte 0x6684
	.2byte 0x080c
	push	{r5, r6, lr}
	adds	r6, r1, #0
	sub	sp, #16
	bl	sub_08016ca4
	adds	r1, r0, #0
	movs	r0, #0
	cmp	r6, #3
	bgt.n	.L_080b0056
	movs	r2, #165
	lsls	r2, r2, #1
	adds	r3, r1, r2
	mov	r5, sp
	ldrh	r0, [r3, #0]
	adds	r1, #248
	adds	r2, r5, #0
	bl	.L_080affac
	lsls	r3, r6, #2
	ldr	r0, [r5, r3]
	movs	r1, #10
	bl	sub_08002054
.L_080b0056:
	add	sp, #16
	pop	{r5, r6, pc}
