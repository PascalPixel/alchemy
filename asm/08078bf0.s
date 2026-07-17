@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08078bf0
	.thumb_func
Func_08078bf0:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #8
	bl	Func_08077394
	ldr	r5, [pc, #72]
	mov	r9, r0
	movs	r0, #88
	add	r5, r9
	add	r0, r9
	mov	r8, r0
	ldrb	r0, [r5, #0]
	bl	Func_08079ad8
	str	r0, [sp, #4]
	ldrb	r3, [r5, #0]
	movs	r4, #128
	lsls	r4, r4, #8
	ldr	r1, [pc, #44]
	mov	r2, r8
	movs	r5, #31
	movs	r0, #0
	cmp	r3, #0
	bne.n	.L0
	b.n	.L1
.L0:
	ldrh	r3, [r2, #0]
	ands	r3, r4
	cmp	r3, #0
	beq.n	.L2
	strh	r1, [r2, #0]
.L2:
	subs	r5, #1
	adds	r2, #4
	cmp	r5, #0
	bge.n	.L0
	movs	r4, #128
	ldr	r1, [pc, #8]
	lsls	r4, r4, #7
	mov	r2, r8
	movs	r5, #31
	b.n	.L3
	movs	r0, r0
	.4byte 0x00000000
	.4byte 0x00000129
.L3:
	ldrh	r3, [r2, #0]
	ands	r3, r4
	cmp	r3, #0
	beq.n	.L4
	strh	r1, [r2, #0]
.L4:
	subs	r5, #1
	adds	r2, #4
	cmp	r5, #0
	bge.n	.L3
	mov	r1, r8
	movs	r4, #31
	movs	r5, #31
	adds	r1, #124
.L7:
	lsls	r3, r4, #2
	mov	r0, r8
	ldrh	r2, [r3, r0]
	adds	r3, r2, #0
	cmp	r3, #0
	bne.n	.L5
	subs	r4, #1
	b.n	.L6
.L5:
	strh	r2, [r1, #0]
	subs	r4, #1
	subs	r1, #4
	subs	r5, #1
.L6:
	cmp	r4, #0
	bge.n	.L7
	cmp	r5, #0
	blt.n	.L8
	lsls	r3, r5, #2
	ldr	r2, [pc, #12]
	add	r3, r8
.L9:
	subs	r5, #1
	strh	r2, [r3, #0]
	subs	r3, #4
	cmp	r5, #0
	bge.n	.L9
	b.n	.L8
	.4byte 0x00000000
.L8:
	ldr	r2, [sp, #4]
	movs	r1, #128
	adds	r2, #16
	lsls	r1, r1, #8
	mov	lr, r2
	movs	r3, #16
	movs	r5, #0
	mov	fp, r1
	mov	sl, r3
	mov	r7, lr
.L17:
	ldrb	r3, [r7, #0]
	cmp	r3, #0
	beq.n	.L10
	mov	r0, r9
	mov	r1, lr
	ldrb	r2, [r0, #15]
	ldrb	r3, [r1, #1]
	cmp	r2, r3
	bcc.n	.L10
	mov	r2, r8
	ldrh	r6, [r2, #0]
	ldrb	r3, [r1, #0]
	mov	ip, r6
	movs	r4, #0
	cmp	ip, r3
	beq.n	.L11
	mov	ip, sl
	mov	r1, r8
.L12:
	adds	r4, #1
	cmp	r4, #31
	bgt.n	.L11
	ldr	r3, [sp, #4]
	adds	r1, #4
	mov	r0, ip
	ldrh	r2, [r1, #0]
	ldrb	r3, [r3, r0]
	cmp	r2, r3
	bne.n	.L12
.L11:
	cmp	r4, #32
	bne.n	.L10
	adds	r3, r6, #0
	movs	r4, #0
	cmp	r3, #0
	bne.n	.L13
	ldrb	r3, [r7, #0]
	mov	r1, fp
	orrs	r3, r1
	mov	r2, r8
	b.n	.L14
.L13:
	adds	r4, #1
	cmp	r4, #31
	bgt.n	.L15
	lsls	r3, r4, #2
	mov	r0, r8
	adds	r2, r3, r0
	ldrh	r3, [r2, #0]
	cmp	r3, #0
	bne.n	.L13
	ldrb	r3, [r7, #0]
	mov	r1, fp
	orrs	r3, r1
.L14:
	strh	r3, [r2, #0]
.L15:
	cmp	r4, #32
	beq.n	.L16
.L10:
	movs	r2, #4
	adds	r5, #1
	add	lr, r2
	adds	r7, #4
	add	sl, r2
	cmp	r5, #15
	ble.n	.L17
.L16:
	movs	r3, #216
	movs	r5, #0
	mov	sl, r3
.L25:
	mov	r0, sl
	mov	r1, r9
	ldrh	r2, [r0, r1]
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L18
	ldr	r3, [pc, #32]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L18
	ldrh	r0, [r0, r1]
	bl	Func_08078414
	ldrb	r3, [r0, #12]
	cmp	r3, #3
	bne.n	.L18
	mov	r2, r8
	ldrh	r6, [r2, #0]
	ldr	r3, [pc, #12]
	ldrh	r0, [r0, #40]
	ands	r3, r6
	mov	ip, r0
	movs	r4, #0
	b.n	.L19
	.4byte 0x00000200
	.4byte 0x00003fff
.L19:
	cmp	r3, ip
	beq.n	.L20
	ldr	r7, [pc, #44]
	mov	r1, r8
.L21:
	adds	r4, #1
	cmp	r4, #31
	bgt.n	.L20
	adds	r1, #4
	ldrh	r2, [r1, #0]
	adds	r3, r7, #0
	ands	r3, r2
	cmp	r3, ip
	bne.n	.L21
.L20:
	cmp	r4, #32
	bne.n	.L18
	adds	r3, r6, #0
	movs	r4, #0
	cmp	r3, #0
	bne.n	.L22
	ldr	r3, [pc, #8]
	orrs	r3, r0
	mov	r0, r8
	strh	r3, [r0, #0]
	b.n	.L23
	.4byte 0x00004000
	.4byte 0x00003fff
.L22:
	adds	r4, #1
	cmp	r4, #31
	bgt.n	.L23
	lsls	r3, r4, #2
	mov	r1, r8
	adds	r2, r3, r1
	ldrh	r3, [r2, #0]
	cmp	r3, #0
	bne.n	.L22
	ldr	r3, [pc, #40]
	orrs	r3, r0
	strh	r3, [r2, #0]
.L23:
	cmp	r4, #32
	beq.n	.L24
.L18:
	movs	r2, #2
	adds	r5, #1
	add	sl, r2
	cmp	r5, #14
	ble.n	.L25
.L24:
	movs	r4, #0
	movs	r5, #0
	mov	r1, r8
.L28:
	lsls	r3, r4, #2
	mov	r0, r8
	ldrh	r2, [r3, r0]
	adds	r3, r2, #0
	cmp	r3, #0
	bne.n	.L26
	adds	r4, #1
	b.n	.L27
	.4byte 0x00004000
.L26:
	strh	r2, [r1, #0]
	adds	r4, #1
	adds	r1, #4
	adds	r5, #1
.L27:
	cmp	r4, #31
	ble.n	.L28
	cmp	r5, #31
	bgt.n	.L29
	lsls	r3, r5, #2
	mov	r0, r8
	adds	r2, r3, r0
	ldr	r1, [pc, #16]
	movs	r3, #32
	subs	r5, r3, r5
.L30:
	subs	r5, #1
	strh	r1, [r2, #0]
	adds	r2, #4
	cmp	r5, #0
	bne.n	.L30
	b.n	.L29
	.4byte 0x00000000
.L29:
	movs	r0, #0
.L1:
	add	sp, #8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
