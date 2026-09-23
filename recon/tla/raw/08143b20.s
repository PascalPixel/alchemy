.syntax unified
	.thumb
	.set sub_081435e0, 0x081435e0
	.global Func_08143b20
	.thumb_func
Func_08143b20:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	adds	r5, r0, #0
	bl	sub_081435e0
	ldr	r3, [pc, #40]
	movs	r2, #128
	orrs	r5, r3
	ldr	r3, [pc, #36]
	lsls	r2, r2, #19
	adds	r2, #12
	mov	r8, r3
	movs	r3, #128
	strh	r5, [r2, #0]
	lsls	r3, r3, #2
	movs	r5, #0
	mov	ip, r5
	movs	r7, #0
	mov	lr, r3
	movs	r6, #0
.L_08143b4c:
	movs	r3, #128
	lsls	r3, r3, #1
	movs	r4, #0
	adds	r0, r6, r3
	lsls	r1, r7, #1
	b.n	.L_08143b60
	.4byte 0x00004784
	.2byte 0x3800
	.2byte 0x0600
.L_08143b60:
	adds	r3, r0, #0
	orrs	r3, r1
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	mov	sl, r3
	mov	r3, r8
	adds	r2, r5, r3
	adds	r4, #1
	mov	r3, sl
	strh	r3, [r2, #0]
	add	r0, lr
	adds	r1, #2
	adds	r5, #2
	cmp	r4, #8
	bne.n	.L_08143b60
	ldr	r1, [pc, #12]
	ldr	r2, [pc, #4]
	movs	r4, #0
	b.n	.L_08143b90
	movs	r0, r0
	.4byte 0x00000f0f
	.2byte 0x3800
	.2byte 0x0600
.L_08143b90:
	adds	r3, r5, r1
	adds	r4, #1
	strh	r2, [r3, #0]
	adds	r5, #2
	cmp	r4, #8
	bne.n	.L_08143b90
	movs	r3, #128
	lsls	r3, r3, #5
	adds	r6, r6, r3
	movs	r3, #1
	add	ip, r3
	mov	r3, ip
	adds	r7, #8
	cmp	r3, #16
	bne.n	.L_08143b4c
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
