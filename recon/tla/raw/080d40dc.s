.syntax unified
	.thumb
	.set sub_08013560, 0x08013560
	.set sub_08038038, 0x08038038
	.set sub_08038050, 0x08038050
	.set sub_080cad84, 0x080cad84
	.global Func_080d40dc
	.thumb_func
Func_080d40dc:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	adds	r5, r0, #0
	mov	r8, r3
	movs	r3, #240
	lsls	r3, r3, #4
	adds	r3, #255
	ands	r5, r3
	adds	r0, r5, #0
	bl	sub_080cad84
	movs	r3, #240
	lsls	r3, r3, #1
	add	r3, r8
	str	r5, [r3, #0]
	movs	r3, #220
	lsls	r3, r3, #1
	add	r3, r8
	ldr	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_080d4164
	adds	r2, r7, #0
	adds	r1, r6, #0
	cmp	r2, #119
	ble.n	.L_080d411a
	adds	r2, #32
	b.n	.L_080d411c
.L_080d411a:
	subs	r2, #32
.L_080d411c:
	cmp	r1, #8
	bge.n	.L_080d4122
	movs	r1, #8
.L_080d4122:
	movs	r3, #156
	lsls	r3, r3, #1
	cmp	r1, r3
	ble.n	.L_080d412c
	adds	r1, r3, #0
.L_080d412c:
	cmp	r2, #20
	bge.n	.L_080d4132
	movs	r2, #20
.L_080d4132:
	cmp	r2, #220
	ble.n	.L_080d4138
	movs	r2, #220
.L_080d4138:
	movs	r3, #226
	lsls	r3, r3, #1
	add	r3, r8
	movs	r4, #0
	ldrsh	r0, [r3, r4]
	movs	r3, #1
	bl	sub_08038038
	movs	r3, #242
	lsls	r3, r3, #1
	adds	r5, r0, #0
	add	r3, r8
	str	r5, [r3, #0]
	b.n	.L_080d415a
.L_080d4154:
	movs	r0, #1
	bl	sub_08013560
.L_080d415a:
	adds	r0, r5, #0
	bl	sub_08038050
	cmp	r0, #0
	beq.n	.L_080d4154
.L_080d4164:
	movs	r2, #226
	lsls	r2, r2, #1
	add	r2, r8
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
