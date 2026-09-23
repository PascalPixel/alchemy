.syntax unified
	.thumb
	.set sub_08013300, 0x08013300
	.set sub_0803a084, 0x0803a084
	.set sub_0803a1c0, 0x0803a1c0
	.global Func_0803cba8
	.thumb_func
Func_0803cba8:
.L_0803cba8:
	push	{r5, r6, lr}
	mov	r6, r8
	push	{r6}
	adds	r5, r0, #0
	ldr	r0, [pc, #60]
	mov	r8, r1
	bl	sub_08013300
	ldr	r3, [r5, #0]
	add	r0, r8
	movs	r2, #12
	ldrsh	r4, [r3, r2]
	movs	r2, #14
	ldrsh	r5, [r3, r2]
	ldrh	r6, [r3, #8]
	ldrh	r3, [r3, #10]
	movs	r2, #132
	mov	ip, r3
	movs	r3, #128
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	ldr	r1, [pc, #28]
	adds	r2, #64
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r0, r4, #0
	adds	r1, r5, #0
	adds	r2, r6, #0
	mov	r3, ip
	bl	sub_0803a1c0
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, pc}
	movs	r0, r0
	.4byte 0x000001d5
	.2byte 0x0100
	.2byte 0x0600
	push	{r5, r6, lr}
	adds	r5, r0, #0
	ldr	r3, [r5, #0]
	ldrh	r3, [r3, #18]
	cmp	r3, #4
	bne.n	.L_0803cc9e
	ldrh	r2, [r5, #20]
	adds	r3, r2, #0
	cmp	r3, #10
	bne.n	.L_0803cc16
	movs	r1, #192
	lsls	r1, r1, #2
	bl	.L_0803cba8
	b.n	.L_0803cc54
.L_0803cc16:
	cmp	r3, #9
	bne.n	.L_0803cc26
	movs	r1, #128
	lsls	r1, r1, #2
	adds	r0, r5, #0
	bl	.L_0803cba8
	b.n	.L_0803cc54
.L_0803cc26:
	cmp	r3, #8
	bne.n	.L_0803cc36
	movs	r1, #128
	lsls	r1, r1, #1
	adds	r0, r5, #0
	bl	.L_0803cba8
	b.n	.L_0803cc54
.L_0803cc36:
	cmp	r3, #6
	bne.n	.L_0803cc46
	movs	r1, #128
	lsls	r1, r1, #2
	adds	r0, r5, #0
	bl	.L_0803cba8
	b.n	.L_0803cc54
.L_0803cc46:
	cmp	r3, #4
	bne.n	.L_0803cc56
	movs	r1, #192
	lsls	r1, r1, #2
	adds	r0, r5, #0
	bl	.L_0803cba8
.L_0803cc54:
	ldrh	r2, [r5, #20]
.L_0803cc56:
	adds	r3, r2, #0
	cmp	r3, #2
	bne.n	.L_0803cc70
	ldr	r3, [r5, #0]
	movs	r1, #12
	ldrsh	r0, [r3, r1]
	movs	r2, #14
	ldrsh	r1, [r3, r2]
	ldrh	r2, [r3, #8]
	ldrh	r3, [r3, #10]
	bl	sub_0803a084
	ldrh	r2, [r5, #20]
.L_0803cc70:
	movs	r1, #255
	lsls	r1, r1, #8
	adds	r1, #255
	adds	r3, r2, r1
	strh	r3, [r5, #20]
	lsls	r3, r3, #16
	lsrs	r6, r3, #16
	cmp	r6, #0
	bne.n	.L_0803cc9e
	ldr	r0, [pc, #28]
	bl	sub_08013300
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	ldr	r1, [pc, #16]
	adds	r2, #64
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r3, [r5, #0]
	strh	r6, [r3, #18]
.L_0803cc9e:
	pop	{r5, r6, pc}
	.4byte 0x000001d5
	.2byte 0x0100
	.2byte 0x0600
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #60]
	movs	r4, #152
	lsls	r4, r4, #5
	adds	r4, #108
	adds	r2, r3, r4
	subs	r4, #32
	movs	r1, #0
	movs	r0, #0
	adds	r3, r3, r4
.L_0803ccc0:
	adds	r1, #1
	stmia	r3!, {r0}
	strh	r0, [r2, #0]
	adds	r2, #2
	cmp	r1, #8
	bne.n	.L_0803ccc0
	pop	{pc}
	.2byte 0x0000
