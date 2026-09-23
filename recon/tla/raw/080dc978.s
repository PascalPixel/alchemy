.syntax unified
	.thumb
	.set sub_0800206c, 0x0800206c
	.set sub_0801314c, 0x0801314c
	.set sub_080145a8, 0x080145a8
	.set sub_08014644, 0x08014644
	.set sub_08014d00, 0x08014d00
	.set sub_080d170c, 0x080d170c
	.set sub_080d17ac, 0x080d17ac
	.set sub_080d2d84, 0x080d2d84
	.set sub_080dc294, 0x080dc294
	.set sub_080dc384, 0x080dc384
	.set sub_080dcadc, 0x080dcadc
	.set sub_080dce60, 0x080dce60
	.set sub_080dcf2c, 0x080dcf2c
	.global Func_080dc978
	.thumb_func
Func_080dc978:
	push	{r5, r6, lr}
	mov	r6, r8
	push	{r6}
	mov	r8, r1
	movs	r1, #166
	adds	r6, r0, #0
	lsls	r1, r1, #2
	movs	r0, #88
	sub	sp, #4
	bl	sub_08014d00
	movs	r3, #0
	adds	r5, r0, #0
	mov	r0, sp
	str	r3, [r0, #0]
	movs	r2, #133
	movs	r3, #128
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	adds	r1, r5, #0
	adds	r2, #166
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r3, [pc, #108]
	movs	r1, #180
	ldr	r0, [r3, #0]
	lsls	r1, r1, #1
	lsls	r0, r0, #1
	bl	sub_0800206c
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #142
	adds	r3, r5, r2
	adds	r2, #2
	strh	r0, [r3, #0]
	adds	r3, r5, r2
	adds	r2, #2
	strh	r6, [r3, #0]
	adds	r3, r5, r2
	mov	r2, r8
	strh	r2, [r3, #0]
	movs	r3, #165
	lsls	r3, r3, #2
	adds	r2, r5, r3
	movs	r3, #8
	strb	r3, [r2, #0]
	movs	r2, #203
	ldr	r1, [pc, #56]
	lsls	r2, r2, #1
	adds	r2, #255
	adds	r3, r5, r2
	strb	r1, [r3, #0]
	bl	sub_080dc294
	bl	sub_080dce60
	movs	r2, #199
	lsls	r2, r2, #1
	adds	r2, #255
	adds	r3, r5, r2
	subs	r2, #1
	movs	r0, #0
	ldrsb	r0, [r3, r0]
	adds	r3, r5, r2
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	lsls	r0, r0, #10
	lsls	r3, r3, #5
	orrs	r0, r3
	movs	r3, #198
	lsls	r3, r3, #1
	adds	r3, #255
	adds	r5, r5, r3
	b.n	.L_080dca1c
	movs	r0, r0
	.4byte 0x00000000
	.2byte 0x122c
	.2byte 0x0300
.L_080dca1c:
	movs	r3, #0
	ldrsb	r3, [r5, r3]
	movs	r1, #1
	orrs	r0, r3
	movs	r3, #128
	lsls	r3, r3, #14
	orrs	r0, r3
	bl	sub_080d170c
	movs	r0, #8
	bl	sub_080d17ac
	bl	.L_080dca84
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #12]
	bl	sub_080145a8
	add	sp, #4
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, pc}
	movs	r0, r0
	.2byte 0xcb45
	.2byte 0x080d
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #88]
	bl	sub_080dcadc
	ldr	r0, [pc, #32]
	bl	sub_08014644
	movs	r3, #164
	lsls	r3, r3, #2
	adds	r5, r5, r3
	ldrh	r0, [r5, #0]
	bl	sub_080d2d84
	movs	r1, #1
	bl	sub_080dcf2c
	bl	sub_080dc384
	movs	r0, #88
	bl	sub_0801314c
	pop	{r5, pc}
	.2byte 0xcb45
	.2byte 0x080d
.L_080dca84:
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #60]
	movs	r3, #1
	strb	r3, [r2, #4]
	ldr	r2, [pc, #56]
	ldr	r3, [pc, #60]
	movs	r1, #192
	strh	r2, [r3, #0]
	adds	r3, #4
	strh	r2, [r3, #0]
	adds	r3, #16
	strh	r2, [r3, #0]
	adds	r3, #2
	strh	r2, [r3, #0]
	subs	r3, #16
	strh	r2, [r3, #0]
	adds	r3, #2
	strh	r2, [r3, #0]
	adds	r3, #2
	strh	r2, [r3, #0]
	adds	r3, #2
	strh	r2, [r3, #0]
	adds	r3, #2
	strh	r2, [r3, #0]
	adds	r3, #2
	strh	r2, [r3, #0]
	lsls	r1, r1, #4
	adds	r3, #2
	strh	r2, [r3, #0]
	adds	r1, #108
	ldr	r0, [pc, #12]
	bl	sub_080145a8
	b.n	.L_080dcad8
	.4byte 0x0000739c
	.4byte 0x050001e2
	.2byte 0xcdc9
	.2byte 0x080d
.L_080dcad8:
	pop	{pc}
	.align 2, 0
