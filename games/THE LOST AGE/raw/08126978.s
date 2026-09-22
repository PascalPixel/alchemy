.syntax unified
	.thumb
	.set sub_08013560, 0x08013560
	.global Overlay_08126978
Overlay_08126978:
	push	{r5, r6, r7, lr}
	movs	r2, #128
	ldr	r3, [pc, #16]
	lsls	r2, r2, #19
	adds	r2, #80
	movs	r7, #128
	strh	r3, [r2, #0]
	ldr	r6, [pc, #12]
	lsls	r7, r7, #19
	movs	r5, #1
	adds	r7, #82
	b.n	.L_08126998
	.4byte 0x00002044
	.2byte 0x1000
	.2byte 0x0000
.L_08126998:
	adds	r3, r5, r6
	strh	r3, [r7, #0]
	movs	r0, #1
	adds	r5, #2
	bl	sub_08013560
	cmp	r5, #16
	ble.n	.L_08126998
	pop	{r5, r6, r7, pc}
