.syntax unified
	.thumb
	.set sub_08013560, 0x08013560
	.set sub_081c20f4, 0x081c20f4
	.set sub_081c212c, 0x081c212c
	.set sub_081c25c4, 0x081c25c4
	.set sub_081c2f68, 0x081c2f68
	.global Func_081c0f84
	.thumb_func
Func_081c0f84:
	push	{r5, lr}
	adds	r2, r0, #0
	lsls	r2, r2, #16
	asrs	r5, r2, #16
	ldr	r0, [pc, #16]
	lsrs	r2, r2, #16
	movs	r1, #255
	bl	sub_081c2f68
	ldr	r3, [pc, #12]
	strh	r5, [r3, #0]
	ldr	r3, [pc, #12]
	strh	r5, [r3, #0]
	pop	{r5, pc}
	.4byte 0x02006a90
	.4byte 0x02005838
	.2byte 0x5808
	.2byte 0x0200
	ldr	r3, [pc, #8]
	strh	r0, [r3, #0]
	ldr	r3, [pc, #8]
	strh	r1, [r3, #0]
	bx	lr
	movs	r0, r0
	.4byte 0x02005838
	.2byte 0x5810
	.2byte 0x0200
	push	{lr}
	bl	sub_081c20f4
	pop	{pc}
	push	{lr}
	bl	sub_081c212c
	pop	{pc}
	ldr	r3, [pc, #4]
	ldrb	r0, [r3, #0]
	bx	lr
	movs	r0, r0
	.2byte 0x5800
	.2byte 0x0200
	push	{r5, r6, lr}
	ldr	r6, [pc, #28]
	movs	r5, #0
.L_081c0fe2:
	ldrb	r3, [r6, #0]
	cmp	r3, #0
	beq.n	.L_081c0ff8
	movs	r0, #1
	bl	sub_08013560
	movs	r3, #44
	adds	r5, #1
	adds	r3, #255
	cmp	r5, r3
	ble.n	.L_081c0fe2
.L_081c0ff8:
	pop	{r5, r6, pc}
	movs	r0, r0
	.2byte 0x5800
	.2byte 0x0200
	push	{lr}
	bl	sub_081c25c4
	pop	{pc}
