.syntax unified
	.thumb
	.set sub_08077018, 0x08077018
	.set sub_080a1f74, 0x080a1f74
	.global Overlay_080a1e38
Overlay_080a1e38:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #112
	str	r0, [sp, #12]
	add	r5, sp, #16
	movs	r0, #0
	str	r0, [sp, #8]
	mov	fp, r0
	adds	r0, r1, #0
	adds	r1, r5, #0
	bl	sub_080a1f74
	add	r1, sp, #48
	mov	r9, r1
	movs	r2, #0
	movs	r6, #14
.L_080a1e62:
	ldr	r0, [sp, #12]
	ldrh	r3, [r2, r0]
	subs	r6, #1
	strh	r3, [r2, r1]
	adds	r2, #2
	cmp	r6, #0
	bge.n	.L_080a1e62
	movs	r1, #0
	mov	r8, r1
	mov	r2, r9
	movs	r6, #14
.L_080a1e78:
	ldrh	r3, [r2, #0]
	adds	r2, #2
	cmp	r3, #0
	beq.n	.L_080a1e84
	movs	r3, #1
	add	r8, r3
.L_080a1e84:
	subs	r6, #1
	cmp	r6, #0
	bge.n	.L_080a1e78
	mov	r0, r8
	cmp	r0, #14
	bgt.n	.L_080a1eac
	add	r3, sp, #80
	lsls	r2, r0, #1
	adds	r2, r2, r3
	ldr	r1, [pc, #16]
	movs	r3, #15
	subs	r6, r3, r0
.L_080a1e9c:
	subs	r6, #1
	strh	r1, [r2, #0]
	adds	r2, #2
	cmp	r6, #0
	bne.n	.L_080a1e9c
	b.n	.L_080a1eac
	.2byte 0x0000
	.2byte 0x0000
.L_080a1eac:
	ldrb	r3, [r5, #0]
	cmp	r3, #255
	beq.n	.L_080a1f44
	mov	r1, sp
	adds	r1, #80
	str	r1, [sp, #4]
	mov	sl, r9
	adds	r7, r5, #0
.L_080a1ebc:
	movs	r6, #0
	movs	r4, #0
	cmp	r6, r8
	bge.n	.L_080a1f12
	mov	r5, r9
.L_080a1ec6:
	ldrh	r3, [r5, #0]
	cmp	r3, #0
	beq.n	.L_080a1f0a
	adds	r0, r3, #0
	str	r4, [sp, #0]
	bl	sub_08077018
	ldrb	r1, [r7, #0]
	movs	r2, #127
	ldrb	r3, [r0, #2]
	ands	r2, r1
	ldr	r4, [sp, #0]
	cmp	r2, r3
	bne.n	.L_080a1f0a
	movs	r3, #128
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_080a1efc
	ldrh	r2, [r5, #0]
	ldr	r3, [pc, #8]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080a1f0a
	b.n	.L_080a1efe
	movs	r0, r0
	.2byte 0x0200
	.2byte 0x0000
.L_080a1efc:
	ldrh	r2, [r5, #0]
.L_080a1efe:
	ldr	r3, [pc, #44]
	ands	r3, r2
	cmp	r4, r3
	bge.n	.L_080a1f0a
	str	r6, [sp, #8]
	adds	r4, r3, #0
.L_080a1f0a:
	adds	r6, #1
	adds	r5, #2
	cmp	r6, r8
	blt.n	.L_080a1ec6
.L_080a1f12:
	cmp	r4, #0
	beq.n	.L_080a1f3c
	ldr	r0, [sp, #8]
	mov	r3, fp
	lsls	r2, r0, #1
	mov	r0, sl
	lsls	r1, r3, #1
	ldrh	r3, [r0, r2]
	ldr	r0, [sp, #4]
	strh	r3, [r0, r1]
	ldr	r3, [pc, #8]
	mov	r1, sl
	b.n	.L_080a1f34
	.4byte 0x000001ff
	.2byte 0x0000
	.2byte 0x0000
.L_080a1f34:
	strh	r3, [r1, r2]
	movs	r3, #1
	add	fp, r3
	b.n	.L_080a1ebc
.L_080a1f3c:
	adds	r7, #1
	ldrb	r3, [r7, #0]
	cmp	r3, #255
	bne.n	.L_080a1ebc
.L_080a1f44:
	mov	r0, r8
	cmp	r0, #0
	ble.n	.L_080a1f5e
	add	r1, sp, #80
	movs	r2, #0
	mov	r6, r8
.L_080a1f50:
	ldrh	r3, [r2, r1]
	ldr	r0, [sp, #12]
	subs	r6, #1
	strh	r3, [r2, r0]
	adds	r2, #2
	cmp	r6, #0
	bne.n	.L_080a1f50
.L_080a1f5e:
	movs	r0, #1
	add	sp, #112
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
