.syntax unified
	.thumb
	.set sub_080afe78, 0x080afe78
	.set sub_080b0bb8, 0x080b0bb8
	.set sub_080b0c9c, 0x080b0c9c
	.set sub_080b0d58, 0x080b0d58
	.set sub_08118108, 0x08118108
	.global Func_080b1004
	.thumb_func
Func_080b1004:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	sub	sp, #32
	mov	r5, sp
	adds	r0, r5, #0
	bl	sub_080afe78
	mov	sl, r0
	bl	sub_08118108
	movs	r3, #0
	mov	r8, r3
	cmp	r8, sl
	bge.n	.L_080b1062
	adds	r7, r5, #0
.L_080b1026:
	movs	r6, #0
.L_080b1028:
	movs	r5, #0
.L_080b102a:
	ldrh	r0, [r7, #0]
	adds	r1, r6, #0
	adds	r2, r5, #0
	bl	sub_080b0bb8
	cmp	r0, #0
	beq.n	.L_080b104c
	adds	r1, r6, #0
	adds	r2, r5, #0
	ldrh	r0, [r7, #0]
	bl	sub_080b0c9c
	ldrh	r0, [r7, #0]
	adds	r1, r6, #0
	adds	r2, r5, #0
	bl	sub_080b0d58
.L_080b104c:
	adds	r5, #1
	cmp	r5, #19
	ble.n	.L_080b102a
	adds	r6, #1
	cmp	r6, #3
	ble.n	.L_080b1028
	movs	r3, #1
	add	r8, r3
	adds	r7, #2
	cmp	r8, sl
	blt.n	.L_080b1026
.L_080b1062:
	add	sp, #32
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	push	{lr}
	movs	r3, #128
	lsls	r3, r3, #1
	adds	r3, #255
	ands	r0, r3
	cmp	r0, #196
	bgt.n	.L_080b1082
	cmp	r0, #191
	blt.n	.L_080b1082
	movs	r0, #1
	b.n	.L_080b1084
.L_080b1082:
	movs	r0, #0
.L_080b1084:
	pop	{pc}
	.2byte 0x0000
