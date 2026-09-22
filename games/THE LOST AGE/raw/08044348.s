.syntax unified
	.thumb
	.set sub_08013164, 0x08013164
	.set sub_080143ac, 0x080143ac
	.set sub_08014dac, 0x08014dac
	.set sub_0801587c, 0x0801587c
	.set sub_0803b918, 0x0803b918
	.set sub_08108088, 0x08108088
	.global Overlay_08044348
Overlay_08044348:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	adds	r6, r0, #0
	movs	r3, #192
	movs	r0, #192
	lsls	r3, r3, #18
	lsls	r0, r0, #2
	ldr	r5, [r3, #60]
	mov	r8, r1
	bl	sub_08014dac
	mov	sl, r0
	mov	r1, sl
	mov	r0, r8
	bl	sub_0801587c
	movs	r1, #14
	ldrsh	r3, [r6, r1]
	movs	r1, #12
	ldrsh	r2, [r6, r1]
	lsls	r3, r3, #5
	adds	r3, r3, r2
	ldr	r2, [pc, #88]
	lsls	r3, r3, #1
	ldrh	r4, [r6, #10]
	adds	r5, r5, r3
	adds	r0, r3, r2
	movs	r3, #0
	mov	ip, r3
	mov	r7, sl
	adds	r5, #8
	cmp	ip, r4
	bge.n	.L_080443c4
	ldrh	r2, [r6, #8]
	movs	r1, #32
	mov	lr, r1
.L_08044394:
	movs	r1, #0
	cmp	r1, r2
	bge.n	.L_080443b2
.L_0804439a:
	movs	r2, #0
	ldrsh	r3, [r7, r2]
	adds	r1, #1
	strh	r3, [r0, #0]
	strh	r3, [r5, #0]
	adds	r7, #2
	ldrh	r2, [r6, #8]
	adds	r0, #2
	adds	r5, #2
	cmp	r1, r2
	blt.n	.L_0804439a
	ldrh	r4, [r6, #10]
.L_080443b2:
	mov	r1, lr
	subs	r3, r1, r2
	lsls	r3, r3, #1
	adds	r0, r0, r3
	adds	r5, r5, r3
	movs	r3, #1
	add	ip, r3
	cmp	ip, r4
	blt.n	.L_08044394
.L_080443c4:
	mov	r0, sl
	bl	sub_08013164
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0x2000
	.2byte 0x0600
	push	{r5, r6, r7, lr}
	adds	r6, r3, #0
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #60]
	ldr	r7, [sp, #20]
	mov	ip, r3
	movs	r5, #12
	ldrsh	r3, [r0, r5]
	ldr	r4, [sp, #16]
	adds	r3, r1, r3
	adds	r1, r3, #1
	movs	r5, #14
	ldrsh	r3, [r0, r5]
	lsls	r7, r7, #12
	adds	r3, r2, r3
	adds	r2, r3, #1
	cmp	r1, #0
	bge.n	.L_08044402
	adds	r6, r6, r1
	movs	r1, #0
.L_08044402:
	adds	r3, r1, r6
	cmp	r3, #29
	ble.n	.L_0804440c
	movs	r3, #30
	subs	r6, r3, r1
.L_0804440c:
	cmp	r2, #0
	bge.n	.L_08044414
	adds	r4, r4, r2
	movs	r2, #0
.L_08044414:
	adds	r3, r2, r4
	cmp	r3, #29
	ble.n	.L_0804441e
	movs	r3, #20
	subs	r4, r3, r2
.L_0804441e:
	cmp	r6, #0
	ble.n	.L_08044458
	cmp	r4, #0
	ble.n	.L_08044458
	lsls	r3, r1, #1
	lsls	r2, r2, #6
	add	r3, ip
	adds	r0, r2, r3
.L_0804442e:
	adds	r1, r0, #0
	adds	r2, r6, #0
	adds	r1, #8
	cmp	r2, #0
	beq.n	.L_0804444a
	ldr	r5, [pc, #32]
.L_0804443a:
	ldrh	r3, [r1, #0]
	subs	r2, #1
	ands	r3, r5
	orrs	r3, r7
	strh	r3, [r1, #0]
	adds	r1, #2
	cmp	r2, #0
	bne.n	.L_0804443a
.L_0804444a:
	subs	r4, #1
	adds	r0, #64
	cmp	r4, #0
	bne.n	.L_0804442e
	movs	r3, #1
	mov	r2, ip
	strb	r3, [r2, #3]
.L_08044458:
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0xefff
	.2byte 0xffff
	push	{r5, lr}
	bl	sub_080143ac
	ldr	r2, [pc, #12]
	movs	r1, #128
	adds	r5, r0, #0
	bl	sub_08108088
	adds	r0, r5, #0
	pop	{r5, pc}
	.2byte 0x01fa
	.2byte 0x0000
	push	{lr}
	ldr	r2, [pc, #8]
	movs	r1, #128
	bl	sub_08108088
	pop	{pc}
	.2byte 0x01fa
	.2byte 0x0000
	push	{lr}
	ldr	r2, [pc, #8]
	movs	r1, #128
	bl	sub_08108088
	pop	{pc}
	.2byte 0x0202
	.2byte 0x0000
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r4, [r3, #60]
	ldrb	r3, [r0, #0]
	sub	sp, #8
	movs	r1, #0
	cmp	r3, #0
	beq.n	.L_080444c0
	movs	r3, #244
	lsls	r3, r3, #4
	adds	r2, r4, r3
.L_080444b0:
	ldrb	r3, [r0, #0]
	adds	r0, #1
	strh	r3, [r2, #0]
	adds	r1, #1
	ldrb	r3, [r0, #0]
	adds	r2, #2
	cmp	r3, #0
	bne.n	.L_080444b0
.L_080444c0:
	movs	r2, #244
	lsls	r3, r1, #1
	lsls	r2, r2, #4
	adds	r3, r3, r2
	ldr	r2, [pc, #20]
	add	r1, sp, #4
	strh	r2, [r4, r3]
	movs	r0, #0
	mov	r2, sp
	movs	r3, #0
	bl	sub_0803b918
	ldr	r0, [sp, #4]
	add	sp, #8
	b.n	.L_080444e4
	movs	r0, r0
	.2byte 0x0000
	.2byte 0x0000
.L_080444e4:
	pop	{pc}
