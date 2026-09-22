.syntax unified
	.thumb
	.set sub_0801314c, 0x0801314c
	.set sub_08013300, 0x08013300
	.set sub_080142d4, 0x080142d4
	.set sub_080143ac, 0x080143ac
	.set sub_08014cc0, 0x08014cc0
	.set sub_0803d4d0, 0x0803d4d0
	.set sub_0803db54, 0x0803db54
	.global Overlay_0803d9bc
Overlay_0803d9bc:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	adds	r7, r1, #0
	movs	r1, #193
	adds	r6, r0, #0
	lsls	r1, r1, #3
	movs	r0, #68
	mov	sl, r2
	mov	r9, r3
	bl	sub_08014cc0
	movs	r1, #0
	adds	r5, r0, #0
	mov	r8, r1
	bl	sub_0803d4d0
	cmp	r6, r0
	bcc.n	.L_0803d9e8
	movs	r6, #0
.L_0803d9e8:
	cmp	r7, #0
	beq.n	.L_0803da16
	movs	r3, #192
	lsls	r3, r3, #3
	adds	r3, #4
	adds	r2, r5, r3
	ldr	r3, [pc, #120]
	movs	r1, #192
	ldr	r3, [r3, #8]
	lsls	r1, r1, #3
	str	r3, [r2, #0]
	movs	r2, #2
	adds	r3, r5, r1
	adds	r1, #2
	strh	r2, [r3, #0]
	adds	r3, r5, r1
	strh	r2, [r3, #0]
	adds	r0, r5, #0
	movs	r1, #0
	bl	sub_0803db54
	movs	r2, #1
	mov	r8, r2
.L_0803da16:
	movs	r3, #192
	ldr	r2, [pc, #88]
	lsls	r3, r3, #3
	adds	r3, #4
	adds	r1, r5, r3
	lsls	r3, r6, #2
	ldr	r3, [r2, r3]
	movs	r2, #2
	str	r3, [r1, #0]
	movs	r1, #192
	lsls	r1, r1, #3
	adds	r3, r5, r1
	adds	r1, #2
	strh	r2, [r3, #0]
	adds	r3, r5, r1
	strh	r2, [r3, #0]
	adds	r0, r5, #0
	mov	r1, r8
	bl	sub_0803db54
	ldr	r2, [sp, #28]
	cmp	r2, #0
	bne.n	.L_0803da4c
	bl	sub_080143ac
	mov	r3, sl
	str	r0, [r3, #0]
.L_0803da4c:
	movs	r3, #128
	mov	r1, sl
	lsls	r3, r3, #3
	ldr	r0, [r1, #0]
	adds	r2, r5, r3
	movs	r1, #128
	bl	sub_080142d4
	mov	r1, r9
	str	r0, [r1, #0]
	movs	r0, #68
	bl	sub_0801314c
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.4byte 0x0804e684
	.2byte 0x4a14
	.2byte 0x0805
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #68]
	movs	r3, #192
	lsls	r3, r3, #3
	adds	r3, #4
	adds	r2, r1, r3
	ldr	r3, [pc, #32]
	lsls	r0, r0, #2
	ldr	r3, [r3, r0]
	movs	r0, #192
	lsls	r0, r0, #3
	str	r3, [r2, #0]
	adds	r3, r1, r0
	movs	r2, #2
	adds	r0, #2
	strh	r2, [r3, #0]
	adds	r3, r1, r0
	strh	r2, [r3, #0]
	adds	r0, r1, #0
	movs	r1, #0
	bl	sub_0803db54
	pop	{pc}
	movs	r0, r0
	.2byte 0x8ff4
	.2byte 0x0805
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r1, #193
	adds	r5, r0, #0
	lsls	r1, r1, #3
	movs	r0, #68
	mov	r8, r3
	adds	r7, r2, #0
	bl	sub_08014cc0
	adds	r6, r0, #0
	ldr	r0, [pc, #124]
	bl	sub_08013300
	adds	r3, r5, #0
	cmp	r5, #127
	bls.n	.L_0803dad6
	subs	r3, #112
.L_0803dad6:
	lsls	r3, r3, #1
	ldrh	r3, [r3, r0]
	movs	r1, #192
	adds	r5, r0, r3
	lsls	r1, r1, #3
	adds	r1, #4
	adds	r3, r5, #0
	adds	r2, r6, r1
	adds	r3, #32
	str	r3, [r2, #0]
	movs	r2, #192
	lsls	r2, r2, #3
	adds	r3, r6, r2
	subs	r1, #2
	movs	r2, #4
	strh	r2, [r3, #0]
	adds	r3, r6, r1
	strh	r2, [r3, #0]
	adds	r0, r6, #0
	movs	r1, #0
	bl	sub_0803db54
	ldr	r2, [sp, #24]
	cmp	r2, #0
	bne.n	.L_0803db0e
	bl	sub_080143ac
	str	r0, [r7, #0]
.L_0803db0e:
	movs	r3, #128
	lsls	r3, r3, #3
	movs	r1, #128
	adds	r2, r6, r3
	ldr	r0, [r7, #0]
	lsls	r1, r1, #2
	bl	sub_080142d4
	mov	r1, r8
	str	r0, [r1, #0]
	movs	r0, #68
	bl	sub_0801314c
	ldr	r1, [sp, #20]
	ldr	r2, [pc, #32]
	lsls	r1, r1, #5
	adds	r1, r1, r2
	movs	r3, #128
	movs	r2, #128
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	adds	r0, r5, #0
	adds	r2, #16
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.4byte 0x000001d6
	.4byte 0x05000200
