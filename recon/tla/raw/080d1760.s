.syntax unified
	.thumb
	.set sub_080d0c50, 0x080d0c50
	.set sub_080d0e1c, 0x080d0e1c
	.global Func_080d1760
	.thumb_func
Func_080d1760:
	push	{r5, r6, r7, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #128
	ldr	r3, [r3, #0]
	ldr	r6, [pc, #20]
	adds	r0, r3, #0
	ldr	r5, [pc, #20]
	movs	r3, #224
	movs	r7, #248
	movs	r4, #224
	lsls	r3, r3, #4
	lsls	r7, r7, #7
	lsls	r4, r4, #1
	adds	r1, r0, r3
	b.n	.L_080d1788
	.4byte 0x000003e0
	.2byte 0x001f
	.2byte 0x0000
.L_080d1788:
	ldrh	r3, [r1, #0]
	adds	r2, r7, #0
	ands	r2, r3
	ldrh	r3, [r1, #2]
	subs	r4, #1
	lsrs	r3, r3, #5
	ands	r3, r6
	orrs	r2, r3
	ldrh	r3, [r1, #4]
	adds	r1, #6
	lsrs	r3, r3, #10
	ands	r3, r5
	orrs	r2, r3
	strh	r2, [r0, #0]
	adds	r0, #2
	cmp	r4, #0
	bne.n	.L_080d1788
	pop	{r5, r6, r7, pc}
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #128
	ldr	r4, [r3, #0]
	adds	r5, r0, #0
	cmp	r4, #0
	beq.n	.L_080d17e6
	movs	r1, #168
	lsls	r1, r1, #6
	adds	r1, #1
	adds	r3, r4, r1
	adds	r1, #1
	movs	r2, #0
	strb	r5, [r3, #0]
	adds	r3, r4, r1
	strb	r2, [r3, #0]
	movs	r3, #224
	lsls	r3, r3, #4
	movs	r2, #224
	adds	r1, r4, r3
	movs	r3, #196
	lsls	r2, r2, #2
	lsls	r3, r3, #5
	adds	r0, r4, r2
	adds	r2, r4, r3
	adds	r3, r5, #0
	bl	sub_080d0c50
.L_080d17e6:
	pop	{r5, pc}
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #128
	ldr	r5, [r3, #0]
	adds	r1, r0, #0
	cmp	r5, #0
	beq.n	.L_080d1816
	movs	r2, #224
	ldr	r3, [pc, #28]
	lsls	r2, r2, #1
	adds	r0, r5, #0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x23e0
	lsls	r3, r3, #4
	movs	r0, #128
	adds	r2, r5, r3
	lsls	r0, r0, #9
	adds	r1, r5, #0
	movs	r3, #1
	bl	sub_080d0e1c
.L_080d1816:
	pop	{r5, pc}
	.2byte 0x0730
	.2byte 0x0300
	push	{lr}
	cmp	r0, #31
	ble.n	.L_080d1826
	movs	r0, #31
	b.n	.L_080d182c
.L_080d1826:
	cmp	r0, #0
	bge.n	.L_080d182c
	movs	r0, #0
.L_080d182c:
	pop	{pc}
	.2byte 0x0000
