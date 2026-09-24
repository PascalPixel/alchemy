.syntax unified
	.thumb
	.set sub_080022ec, 0x080022ec
	.set sub_080022fc, 0x080022fc
	.set sub_08009008, 0x08009008
	.global Unnamed_080e6d3c
	.global Func_080e6d3c
	.thumb_func
Unnamed_080e6d3c:
Func_080e6d3c:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	ldr	r3, [pc, #308]
	mov	sl, r2
	ldr	r2, [r3, #0]
	ldr	r3, [pc, #304]
	ldr	r4, [r3, #4]
	ldr	r3, [r3, #0]
	sub	sp, #32
	str	r3, [sp, #8]
	str	r4, [sp, #12]
	ldr	r3, [pc, #296]
	ldr	r4, [r3, #4]
	ldr	r3, [r3, #0]
	mov	r9, r1
	str	r3, [sp, #0]
	str	r4, [sp, #4]
	add	r1, sp, #16
	movs	r3, #0
	str	r3, [r1, #12]
	movs	r3, #255
	lsls	r3, r3, #16
	str	r3, [r1, #4]
	cmp	r0, #1
	beq.n	.L_080e6dc4
	cmp	r0, #1
	bgt.n	.L_080e6d7e
	cmp	r0, #0
	beq.n	.L_080e6d88
	b.n	.L_080e6e6a
.L_080e6d7e:
	cmp	r0, #2
	beq.n	.L_080e6e00
	cmp	r0, #3
	beq.n	.L_080e6e3a
	b.n	.L_080e6e6a
.L_080e6d88:
	add	r3, sp, #8
	mov	r8, r3
	ldr	r3, [pc, #248]
	movs	r5, #0
	adds	r7, r1, #0
	adds	r6, r2, r3
.L_080e6d94:
	adds	r0, r5, #0
	movs	r1, #3
	bl	sub_080022fc
	lsls	r0, r0, #21
	add	r0, r9
	str	r0, [r7, #0]
	movs	r1, #3
	adds	r0, r5, #0
	bl	sub_080022ec
	lsls	r0, r0, #21
	add	r0, sl
	str	r0, [r7, #8]
	adds	r1, r7, #0
	ldmia	r6!, {r0}
	mov	r2, r8
	movs	r3, #0
	adds	r5, #1
	bl	sub_08009008
	cmp	r5, #9
	bne.n	.L_080e6d94
	b.n	.L_080e6e6a
.L_080e6dc4:
	add	r3, sp, #8
	mov	r8, r3
	ldr	r3, [pc, #188]
	movs	r5, #0
	adds	r6, r1, #0
	adds	r7, r2, r3
.L_080e6dd0:
	ldr	r3, [pc, #184]
	ldrb	r3, [r3, r5]
	ldr	r2, [pc, #184]
	lsls	r3, r3, #16
	add	r3, r9
	adds	r3, r3, r2
	str	r3, [r6, #0]
	ldr	r3, [pc, #180]
	ldrb	r3, [r3, r5]
	ldr	r2, [pc, #180]
	lsls	r3, r3, #16
	add	r3, sl
	adds	r3, r3, r2
	str	r3, [r6, #8]
	ldmia	r7!, {r0}
	adds	r1, r6, #0
	mov	r2, r8
	movs	r3, #0
	adds	r5, #1
	bl	sub_08009008
	cmp	r5, #12
	bne.n	.L_080e6dd0
	b.n	.L_080e6e6a
.L_080e6e00:
	add	r3, sp, #8
	mov	r8, r3
	ldr	r3, [pc, #128]
	movs	r5, #0
	adds	r6, r1, #0
	adds	r7, r2, r3
.L_080e6e0c:
	ldr	r3, [pc, #140]
	ldrb	r3, [r3, r5]
	movs	r2, #128
	lsls	r3, r3, #16
	lsls	r2, r2, #13
	add	r3, r9
	adds	r3, r3, r2
	str	r3, [r6, #0]
	ldr	r3, [pc, #128]
	ldrb	r3, [r3, r5]
	lsls	r3, r3, #16
	add	r3, sl
	str	r3, [r6, #8]
	ldmia	r7!, {r0}
	adds	r1, r6, #0
	mov	r2, r8
	movs	r3, #0
	adds	r5, #1
	bl	sub_08009008
	cmp	r5, #8
	bne.n	.L_080e6e0c
	b.n	.L_080e6e6a
.L_080e6e3a:
	ldr	r3, [pc, #76]
	movs	r5, #0
	mov	r8, sp
	adds	r6, r1, #0
	adds	r7, r2, r3
.L_080e6e44:
	ldr	r3, [pc, #92]
	ldrb	r3, [r3, r5]
	lsls	r3, r3, #16
	add	r3, r9
	str	r3, [r6, #0]
	ldr	r3, [pc, #88]
	ldrb	r3, [r3, r5]
	lsls	r3, r3, #16
	add	r3, sl
	str	r3, [r6, #8]
	ldmia	r7!, {r0}
	adds	r1, r6, #0
	mov	r2, r8
	movs	r3, #0
	adds	r5, #1
	bl	sub_08009008
	cmp	r5, #8
	bne.n	.L_080e6e44
.L_080e6e6a:
	add	sp, #32
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001eec
	.4byte 0x080edab8
	.4byte 0x080edac0
	.4byte 0x000077d8
	.4byte 0x080eee1e
	.4byte 0xfff00000
	.4byte 0x080eee2a
	.4byte 0xffe00000
	.4byte 0x080eee36
	.4byte 0x080eee3e
	.4byte 0x080eee46
	.4byte 0x080eee4e
