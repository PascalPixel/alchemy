.syntax unified
	.thumb
	.set sub_08013300, 0x08013300
	.set sub_08013560, 0x08013560
	.set sub_08014694, 0x08014694
	.set sub_0801587c, 0x0801587c
	.set sub_0802dd08, 0x0802dd08
	.global Overlay_0802c8a0
Overlay_0802c8a0:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #28]
	ldr	r5, [r3, #32]
	mov	sl, r2
	movs	r2, #128
	lsls	r2, r2, #1
	adds	r3, r5, r2
	movs	r2, #1
	mov	r8, r2
	mov	r2, r8
	strb	r2, [r3, #0]
	ldr	r0, [pc, #160]
	bl	sub_08014694
	movs	r2, #144
	lsls	r2, r2, #4
	adds	r2, #114
	adds	r3, r5, r2
	mov	r2, r8
	strb	r2, [r3, #0]
	movs	r2, #144
	lsls	r2, r2, #1
	adds	r3, r5, r2
	ldr	r6, [r3, #0]
	ldr	r0, [r6, #12]
	bl	sub_08013300
	ldr	r1, [pc, #132]
	bl	sub_0801587c
	movs	r3, #151
	lsls	r3, r3, #4
	adds	r7, r5, r3
	movs	r3, #0
	ldrsb	r3, [r7, r3]
	cmp	r3, #0
	beq.n	.L_0802c900
	ldr	r0, [r6, #16]
	bl	sub_08013300
	ldr	r1, [pc, #112]
	bl	sub_0801587c
.L_0802c900:
	movs	r3, #0
	strb	r3, [r7, #0]
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #212
	ldr	r0, [pc, #100]
	ldr	r1, [pc, #100]
	ldr	r2, [pc, #104]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r0, #1
	bl	sub_08013560
	ldr	r3, [pc, #96]
	mov	r2, r8
	ldr	r3, [r3, #0]
	ldr	r1, [pc, #92]
	ands	r3, r2
	lsls	r0, r3, #2
	adds	r0, r0, r3
	lsls	r0, r0, #10
	movs	r3, #200
	lsls	r3, r3, #4
	add	r0, sl
	adds	r0, r0, r3
	bl	sub_0802dd08
	movs	r3, #130
	lsls	r3, r3, #1
	adds	r2, r5, r3
	movs	r3, #200
	strh	r3, [r2, #0]
	adds	r3, #62
	adds	r2, r5, r3
	movs	r3, #255
	strh	r3, [r2, #0]
	ldr	r2, [pc, #56]
	ldr	r3, [pc, #60]
	str	r3, [r2, #0]
	movs	r2, #144
	lsls	r2, r2, #4
	adds	r2, #113
	adds	r3, r5, r2
	mov	r2, r8
	strb	r2, [r3, #0]
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x0802cb65
	.4byte 0x02038000
	.4byte 0x0203a000
	.4byte 0x06004000
	.4byte 0x0201c000
	.4byte 0x84000800
	.4byte 0x0300122c
	.4byte 0x02010000
	.4byte 0x030011f8
	.4byte 0x0802c865
