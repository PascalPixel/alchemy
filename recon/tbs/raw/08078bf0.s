.syntax unified
	.thumb
	.set sub_08077394, 0x08077394
	.set sub_08078414, 0x08078414
	.set sub_08079ad8, 0x08079ad8
	.global Owner_RefreshClassActions
	.global Func_08078bf0
	.thumb_func
Owner_RefreshClassActions:
Func_08078bf0:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #8
	bl	sub_08077394
	ldr	r5, [pc, #72]
	mov	r9, r0
	movs	r0, #88
	add	r5, r9
	add	r0, r9
	mov	r8, r0
	ldrb	r0, [r5, #0]
	bl	sub_08079ad8
	str	r0, [sp, #4]
	ldrb	r3, [r5, #0]
	movs	r4, #128
	lsls	r4, r4, #8
	ldr	r1, [pc, #44]
	mov	r2, r8
	movs	r5, #31
	movs	r0, #0
	cmp	r3, #0
	bne.n	.L_08078c2c
	b.n	.L_08078e16
.L_08078c2c:
	ldrh	r3, [r2, #0]
	ands	r3, r4
	cmp	r3, #0
	beq.n	.L_08078c36
	strh	r1, [r2, #0]
.L_08078c36:
	subs	r5, #1
	adds	r2, #4
	cmp	r5, #0
	bge.n	.L_08078c2c
	movs	r4, #128
	ldr	r1, [pc, #8]
	lsls	r4, r4, #7
	mov	r2, r8
	movs	r5, #31
	b.n	.L_08078c54
	movs	r0, r0
	.4byte 0x00000000
	.2byte 0x0129
	.2byte 0x0000
.L_08078c54:
	ldrh	r3, [r2, #0]
	ands	r3, r4
	cmp	r3, #0
	beq.n	.L_08078c5e
	strh	r1, [r2, #0]
.L_08078c5e:
	subs	r5, #1
	adds	r2, #4
	cmp	r5, #0
	bge.n	.L_08078c54
	mov	r1, r8
	movs	r4, #31
	movs	r5, #31
	adds	r1, #124
.L_08078c6e:
	lsls	r3, r4, #2
	mov	r0, r8
	ldrh	r2, [r3, r0]
	adds	r3, r2, #0
	cmp	r3, #0
	bne.n	.L_08078c7e
	subs	r4, #1
	b.n	.L_08078c86
.L_08078c7e:
	strh	r2, [r1, #0]
	subs	r4, #1
	subs	r1, #4
	subs	r5, #1
.L_08078c86:
	cmp	r4, #0
	bge.n	.L_08078c6e
	cmp	r5, #0
	blt.n	.L_08078ca4
	lsls	r3, r5, #2
	ldr	r2, [pc, #12]
	add	r3, r8
.L_08078c94:
	subs	r5, #1
	strh	r2, [r3, #0]
	subs	r3, #4
	cmp	r5, #0
	bge.n	.L_08078c94
	b.n	.L_08078ca4
	.2byte 0x0000
	.2byte 0x0000
.L_08078ca4:
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
.L_08078cb8:
	ldrb	r3, [r7, #0]
	cmp	r3, #0
	beq.n	.L_08078d24
	mov	r0, r9
	mov	r1, lr
	ldrb	r2, [r0, #15]
	ldrb	r3, [r1, #1]
	cmp	r2, r3
	bcc.n	.L_08078d24
	mov	r2, r8
	ldrh	r6, [r2, #0]
	ldrb	r3, [r1, #0]
	mov	ip, r6
	movs	r4, #0
	cmp	ip, r3
	beq.n	.L_08078cf0
	mov	ip, sl
	mov	r1, r8
.L_08078cdc:
	adds	r4, #1
	cmp	r4, #31
	bgt.n	.L_08078cf0
	ldr	r3, [sp, #4]
	adds	r1, #4
	mov	r0, ip
	ldrh	r2, [r1, #0]
	ldrb	r3, [r3, r0]
	cmp	r2, r3
	bne.n	.L_08078cdc
.L_08078cf0:
	cmp	r4, #32
	bne.n	.L_08078d24
	adds	r3, r6, #0
	movs	r4, #0
	cmp	r3, #0
	bne.n	.L_08078d06
	ldrb	r3, [r7, #0]
	mov	r1, fp
	orrs	r3, r1
	mov	r2, r8
	b.n	.L_08078d1e
.L_08078d06:
	adds	r4, #1
	cmp	r4, #31
	bgt.n	.L_08078d20
	lsls	r3, r4, #2
	mov	r0, r8
	adds	r2, r3, r0
	ldrh	r3, [r2, #0]
	cmp	r3, #0
	bne.n	.L_08078d06
	ldrb	r3, [r7, #0]
	mov	r1, fp
	orrs	r3, r1
.L_08078d1e:
	strh	r3, [r2, #0]
.L_08078d20:
	cmp	r4, #32
	beq.n	.L_08078d32
.L_08078d24:
	movs	r2, #4
	adds	r5, #1
	add	lr, r2
	adds	r7, #4
	add	sl, r2
	cmp	r5, #15
	ble.n	.L_08078cb8
.L_08078d32:
	movs	r3, #216
	movs	r5, #0
	mov	sl, r3
.L_08078d38:
	mov	r0, sl
	mov	r1, r9
	ldrh	r2, [r0, r1]
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L_08078dc4
	ldr	r3, [pc, #32]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08078dc4
	ldrh	r0, [r0, r1]
	bl	sub_08078414
	ldrb	r3, [r0, #12]
	cmp	r3, #3
	bne.n	.L_08078dc4
	mov	r2, r8
	ldrh	r6, [r2, #0]
	ldr	r3, [pc, #12]
	ldrh	r0, [r0, #40]
	ands	r3, r6
	mov	ip, r0
	movs	r4, #0
	b.n	.L_08078d70
	.4byte 0x00000200
	.2byte 0x3fff
	.2byte 0x0000
.L_08078d70:
	cmp	r3, ip
	beq.n	.L_08078d8a
	ldr	r7, [pc, #44]
	mov	r1, r8
.L_08078d78:
	adds	r4, #1
	cmp	r4, #31
	bgt.n	.L_08078d8a
	adds	r1, #4
	ldrh	r2, [r1, #0]
	adds	r3, r7, #0
	ands	r3, r2
	cmp	r3, ip
	bne.n	.L_08078d78
.L_08078d8a:
	cmp	r4, #32
	bne.n	.L_08078dc4
	adds	r3, r6, #0
	movs	r4, #0
	cmp	r3, #0
	bne.n	.L_08078da8
	ldr	r3, [pc, #8]
	orrs	r3, r0
	mov	r0, r8
	strh	r3, [r0, #0]
	b.n	.L_08078dc0
	.4byte 0x00004000
	.2byte 0x3fff
	.2byte 0x0000
.L_08078da8:
	adds	r4, #1
	cmp	r4, #31
	bgt.n	.L_08078dc0
	lsls	r3, r4, #2
	mov	r1, r8
	adds	r2, r3, r1
	ldrh	r3, [r2, #0]
	cmp	r3, #0
	bne.n	.L_08078da8
	ldr	r3, [pc, #40]
	orrs	r3, r0
	strh	r3, [r2, #0]
.L_08078dc0:
	cmp	r4, #32
	beq.n	.L_08078dce
.L_08078dc4:
	movs	r2, #2
	adds	r5, #1
	add	sl, r2
	cmp	r5, #14
	ble.n	.L_08078d38
.L_08078dce:
	movs	r4, #0
	movs	r5, #0
	mov	r1, r8
.L_08078dd4:
	lsls	r3, r4, #2
	mov	r0, r8
	ldrh	r2, [r3, r0]
	adds	r3, r2, #0
	cmp	r3, #0
	bne.n	.L_08078de8
	adds	r4, #1
	b.n	.L_08078df0
	.2byte 0x4000
	.2byte 0x0000
.L_08078de8:
	strh	r2, [r1, #0]
	adds	r4, #1
	adds	r1, #4
	adds	r5, #1
.L_08078df0:
	cmp	r4, #31
	ble.n	.L_08078dd4
	cmp	r5, #31
	bgt.n	.L_08078e14
	lsls	r3, r5, #2
	mov	r0, r8
	adds	r2, r3, r0
	ldr	r1, [pc, #16]
	movs	r3, #32
	subs	r5, r3, r5
.L_08078e04:
	subs	r5, #1
	strh	r1, [r2, #0]
	adds	r2, #4
	cmp	r5, #0
	bne.n	.L_08078e04
	b.n	.L_08078e14
	.2byte 0x0000
	.2byte 0x0000
.L_08078e14:
	movs	r0, #0
.L_08078e16:
	add	sp, #8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
