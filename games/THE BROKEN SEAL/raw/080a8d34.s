.syntax unified
	.thumb
	.set sub_080030f8, 0x080030f8
	.set sub_08015068, 0x08015068
	.set sub_08015080, 0x08015080
	.set sub_08015270, 0x08015270
	.set sub_08015280, 0x08015280
	.set sub_08077080, 0x08077080
	.set sub_080a2268, 0x080a2268
	.global Overlay_080a8d34
Overlay_080a8d34:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #484]
	mov	fp, r2
	ldr	r3, [r3, #0]
	ldr	r2, [r2, #8]
	mov	r1, fp
	mov	r8, r3
	lsls	r3, r2, #2
	adds	r3, r3, r2
	ldr	r2, [r1, #16]
	adds	r3, r3, r2
	mov	r2, r8
	str	r3, [r1, #24]
	adds	r7, r0, #0
	ldr	r0, [r2, #44]
	sub	sp, #8
	bl	sub_08015270
	movs	r0, #1
	bl	sub_080030f8
	mov	r1, fp
	ldr	r3, [r1, #24]
	movs	r2, #228
	lsls	r2, r2, #1
	lsls	r3, r3, #1
	adds	r3, r3, r2
	mov	r1, r8
	ldrh	r2, [r1, r3]
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L_080a8e1a
	ldr	r5, [pc, #424]
	ldr	r3, [pc, #428]
	adds	r0, r5, #0
	ands	r0, r2
	adds	r0, r0, r3
	ldr	r1, [r1, #44]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08015080
	mov	r2, fp
	ldr	r3, [r2, #24]
	movs	r1, #228
	lsls	r1, r1, #1
	lsls	r3, r3, #1
	adds	r3, r3, r1
	mov	r2, r8
	ldrh	r3, [r2, r3]
	ands	r5, r3
	adds	r0, r5, #0
	bl	sub_08077080
	movs	r3, #104
	adds	r5, r0, #0
	str	r3, [sp, #0]
	adds	r0, r7, #0
	movs	r3, #224
	movs	r1, #0
	movs	r2, #96
	bl	sub_08015068
	movs	r3, #0
	mov	sl, r3
	ldrb	r3, [r5, #12]
	cmp	r3, #0
	bne.n	.L_080a8dd4
	ldrb	r0, [r5, #1]
	movs	r3, #64
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L_080a8dda
	b.n	.L_080a8dd6
.L_080a8dd4:
	ldrb	r0, [r5, #1]
.L_080a8dd6:
	movs	r1, #2
	mov	sl, r1
.L_080a8dda:
	movs	r3, #128
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L_080a8dea
	mov	r2, sl
	movs	r3, #1
	orrs	r2, r3
	mov	sl, r2
.L_080a8dea:
	mov	r3, sl
	cmp	r3, #3
	bne.n	.L_080a8df4
	ldr	r0, [pc, #320]
	b.n	.L_080a8dfc
.L_080a8df4:
	mov	r1, sl
	cmp	r1, #2
	bne.n	.L_080a8e08
	ldr	r0, [pc, #316]
.L_080a8dfc:
	adds	r1, r7, #0
	movs	r2, #0
	movs	r3, #96
	bl	sub_08015080
	b.n	.L_080a8e1a
.L_080a8e08:
	mov	r2, sl
	cmp	r2, #1
	bne.n	.L_080a8e1a
	ldr	r0, [pc, #300]
	adds	r1, r7, #0
	movs	r2, #0
	movs	r3, #96
	bl	sub_08015080
.L_080a8e1a:
	mov	r3, fp
	ldr	r2, [r3, #8]
	lsls	r3, r2, #2
	adds	r3, r3, r2
	movs	r1, #0
	mov	sl, r1
	lsls	r3, r3, #1
	movs	r1, #228
	add	r3, r8
	lsls	r1, r1, #1
	movs	r2, #1
	adds	r1, r1, r3
	mov	r9, r2
	movs	r6, #2
	mov	r8, r1
.L_080a8e38:
	mov	r2, fp
	ldr	r3, [r2, #16]
	cmp	sl, r3
	bne.n	.L_080a8e9a
	mov	r1, r8
	ldrh	r3, [r1, #0]
	ldr	r0, [pc, #228]
	ands	r0, r3
	bl	sub_08077080
	adds	r5, r0, #0
	ldrb	r3, [r5, #2]
	cmp	r3, #4
	beq.n	.L_080a8e84
	adds	r1, r3, #0
	movs	r3, #0
	str	r3, [sp, #0]
	adds	r1, #1
	adds	r0, r7, #0
	movs	r2, #24
	adds	r3, r6, #0
	bl	sub_08015280
	mov	r2, r9
	movs	r3, #14
	str	r2, [sp, #0]
	str	r3, [sp, #4]
	adds	r0, r7, #0
	movs	r1, #9
	adds	r2, r6, #0
	movs	r3, #15
	bl	sub_080a2268
	mov	r1, r9
	movs	r2, #14
	str	r1, [sp, #0]
	str	r2, [sp, #4]
	b.n	.L_080a8edc
.L_080a8e84:
	mov	r3, r9
	movs	r1, #14
	str	r3, [sp, #0]
	str	r1, [sp, #4]
	adds	r0, r7, #0
	movs	r1, #9
	adds	r2, r6, #0
	movs	r3, #19
	bl	sub_080a2268
	b.n	.L_080a8efe
.L_080a8e9a:
	mov	r2, r8
	ldrh	r3, [r2, #0]
	ldr	r0, [pc, #140]
	ands	r0, r3
	bl	sub_08077080
	adds	r5, r0, #0
	ldrb	r3, [r5, #2]
	cmp	r3, #4
	beq.n	.L_080a8eea
	adds	r1, r3, #0
	movs	r3, #4
	str	r3, [sp, #0]
	adds	r1, #1
	adds	r0, r7, #0
	movs	r2, #24
	adds	r3, r6, #0
	bl	sub_08015280
	mov	r3, r9
	movs	r1, #15
	str	r3, [sp, #0]
	str	r1, [sp, #4]
	adds	r0, r7, #0
	movs	r1, #9
	adds	r2, r6, #0
	movs	r3, #15
	bl	sub_080a2268
	mov	r2, r9
	movs	r3, #15
	str	r2, [sp, #0]
	str	r3, [sp, #4]
.L_080a8edc:
	adds	r0, r7, #0
	movs	r1, #25
	adds	r2, r6, #0
	movs	r3, #3
	bl	sub_080a2268
	b.n	.L_080a8efe
.L_080a8eea:
	mov	r1, r9
	movs	r2, #15
	str	r1, [sp, #0]
	str	r2, [sp, #4]
	adds	r0, r7, #0
	movs	r1, #9
	adds	r2, r6, #0
	movs	r3, #19
	bl	sub_080a2268
.L_080a8efe:
	movs	r1, #1
	add	sl, r1
	movs	r3, #2
	mov	r2, sl
	adds	r6, #2
	add	r8, r3
	cmp	r2, #4
	ble.n	.L_080a8e38
	movs	r0, #1
	bl	sub_080030f8
	movs	r0, #1
	add	sp, #8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x03001f2c
	.4byte 0x00003fff
	.4byte 0x0000053a
	.4byte 0x00000b15
	.4byte 0x00000b14
	.4byte 0x00000b13
