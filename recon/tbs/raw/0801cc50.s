.syntax unified
	.thumb
	.global Color_ScaleComponents
	.global Func_0801cc50
	.thumb_func
Color_ScaleComponents:
Func_0801cc50:
	push	{r5, r6, r7, lr}
	adds	r5, r0, #0
	adds	r6, r2, #0
	movs	r2, #0
	ldrsh	r0, [r5, r2]
	ldr	r4, [pc, #96]
	lsls	r0, r0, #16
	movs	r0, r0
	mov	ip, pc
	bx	r4
	asrs	r7, r0, #16
	movs	r2, #2
	ldrsh	r0, [r5, r2]
	adds	r1, r6, #0
	lsls	r0, r0, #16
	movs	r0, r0
	mov	ip, pc
	bx	r4
	asrs	r6, r0, #16
	movs	r2, #4
	ldrsh	r0, [r5, r2]
	adds	r1, r3, #0
	lsls	r0, r0, #16
	movs	r0, r0
	mov	ip, pc
	bx	r4
	asrs	r0, r0, #16
	cmp	r7, #0
	bge.n	.L_0801cc8c
	movs	r7, #0
.L_0801cc8c:
	cmp	r7, #31
	ble.n	.L_0801cc92
	movs	r7, #31
.L_0801cc92:
	cmp	r6, #0
	bge.n	.L_0801cc98
	movs	r6, #0
.L_0801cc98:
	cmp	r6, #31
	ble.n	.L_0801cc9e
	movs	r6, #31
.L_0801cc9e:
	cmp	r0, #0
	bge.n	.L_0801cca4
	movs	r0, #0
.L_0801cca4:
	cmp	r0, #31
	ble.n	.L_0801ccaa
	movs	r0, #31
.L_0801ccaa:
	lsls	r3, r6, #5
	lsls	r0, r0, #10
	adds	r0, r0, r3
	adds	r0, r7, r0
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x03000118
