.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_0800205c, 0x0800205c
	.set sub_08013164, 0x08013164
	.set sub_08014878, 0x08014878
	.set sub_08014dac, 0x08014dac
	.set sub_08016ca4, 0x08016ca4
	.set sub_080ad3f8, 0x080ad3f8
	.set sub_080af79c, 0x080af79c
	.set sub_080b02d4, 0x080b02d4
	.global Func_080af8d0
	.thumb_func
Func_080af8d0:
.L_080af8d0:
	push	{r5, lr}
	adds	r5, r1, #0
	bl	sub_08016ca4
	movs	r1, #42
	adds	r2, r0, #0
	adds	r1, #255
	adds	r3, r2, r1
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_080af912
	movs	r0, #0
	cmp	r5, #0
	ble.n	.L_080af916
	cmp	r5, #99
	bgt.n	.L_080af912
	movs	r3, #165
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	cmp	r3, #7
	bhi.n	.L_080af912
	adds	r2, r3, #0
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r2, r3, #5
	adds	r3, r3, r2
	ldr	r1, [pc, #16]
	adds	r3, r3, r5
	lsls	r3, r3, #2
	subs	r3, #4
	ldr	r0, [r1, r3]
	b.n	.L_080af916
.L_080af912:
	movs	r0, #1
	negs	r0, r0
.L_080af916:
	pop	{r5, pc}
	.2byte 0x12c8
	.2byte 0x080b
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	adds	r6, r1, #0
	mov	fp, r0
	bl	sub_08016ca4
	mov	sl, r0
	movs	r0, #44
	bl	sub_08014dac
	movs	r3, #42
	adds	r3, #255
	add	r3, sl
	ldrb	r3, [r3, #0]
	mov	r9, r0
	str	r3, [r0, #0]
	mov	r0, sl
	ldrb	r1, [r0, #15]
	movs	r3, #255
	mov	r2, r9
	lsls	r3, r3, #8
	str	r1, [r2, #4]
	adds	r3, #255
	movs	r2, #0
	strh	r1, [r6, #0]
	strh	r3, [r6, #2]
	strh	r2, [r6, #4]
	strh	r2, [r6, #6]
	strh	r2, [r6, #8]
	strh	r2, [r6, #10]
	strh	r2, [r6, #12]
	strh	r2, [r6, #14]
	cmp	r1, #98
	ble.n	.L_080af96c
	b.n	.L_080afb6a
.L_080af96c:
	ldrb	r3, [r0, #15]
	adds	r3, #1
	strb	r3, [r0, #15]
	adds	r3, r1, #1
	strh	r3, [r6, #0]
	ldrb	r1, [r0, #15]
	mov	r0, fp
	bl	.L_080af8d0
	movs	r3, #1
	negs	r3, r3
	cmp	r0, r3
	beq.n	.L_080af994
	movs	r2, #146
	lsls	r2, r2, #1
	add	r2, sl
	ldr	r3, [r2, #0]
	cmp	r3, r0
	bcs.n	.L_080af994
	str	r0, [r2, #0]
.L_080af994:
	mov	r0, fp
	bl	sub_080af79c
	movs	r2, #0
	ldrsh	r3, [r6, r2]
	adds	r1, r0, #0
	mov	r0, r9
	str	r1, [r0, #8]
	ldrh	r0, [r6, #0]
	cmp	r3, #1
	bne.n	.L_080af9f2
	adds	r2, r1, #0
	adds	r2, #80
	ldrh	r3, [r6, #4]
	ldrh	r2, [r2, #0]
	adds	r3, r3, r2
	adds	r2, r1, #0
	adds	r2, #92
	ldrh	r2, [r2, #0]
	strh	r3, [r6, #4]
	ldrh	r3, [r6, #6]
	adds	r3, r3, r2
	adds	r2, r1, #0
	adds	r2, #104
	ldrh	r2, [r2, #0]
	strh	r3, [r6, #6]
	ldrh	r3, [r6, #8]
	adds	r3, r3, r2
	adds	r2, r1, #0
	adds	r2, #116
	ldrh	r2, [r2, #0]
	strh	r3, [r6, #8]
	ldrh	r3, [r6, #10]
	adds	r3, r3, r2
	adds	r2, r1, #0
	adds	r2, #128
	ldrh	r2, [r2, #0]
	strh	r3, [r6, #10]
	ldrh	r3, [r6, #12]
	adds	r3, r3, r2
	strh	r3, [r6, #12]
	adds	r3, r1, #0
	adds	r3, #140
	ldrb	r2, [r3, #0]
	ldrh	r3, [r6, #14]
	adds	r3, r3, r2
	strh	r3, [r6, #14]
.L_080af9f2:
	lsls	r0, r0, #16
	asrs	r0, r0, #16
	movs	r1, #20
	bl	sub_08002054
	lsls	r0, r0, #16
	asrs	r5, r0, #16
	cmp	r5, #0
	bge.n	.L_080afa06
	movs	r5, #0
.L_080afa06:
	cmp	r5, #4
	ble.n	.L_080afa0c
	movs	r5, #4
.L_080afa0c:
	lsls	r0, r5, #1
	mov	r3, r9
	mov	r8, r0
	ldr	r1, [r3, #8]
	mov	r3, r8
	adds	r3, #82
	ldrsh	r2, [r1, r3]
	subs	r3, #2
	ldrsh	r3, [r1, r3]
	subs	r7, r2, r3
	bl	sub_08014878
	adds	r3, r0, #0
	lsls	r0, r3, #2
	adds	r0, r0, r3
	lsls	r0, r0, #2
	lsrs	r0, r0, #16
	adds	r0, r0, r7
	movs	r1, #20
	bl	sub_0800205c
	ldrh	r3, [r6, #4]
	adds	r3, r3, r0
	strh	r3, [r6, #4]
	mov	r3, r9
	ldr	r2, [r3, #8]
	mov	r3, r8
	adds	r3, #94
	ldrsh	r1, [r2, r3]
	subs	r3, #2
	ldrsh	r3, [r2, r3]
	subs	r7, r1, r3
	bl	sub_08014878
	adds	r3, r0, #0
	lsls	r0, r3, #2
	adds	r0, r0, r3
	lsls	r0, r0, #2
	lsrs	r0, r0, #16
	adds	r0, r0, r7
	movs	r1, #20
	bl	sub_0800205c
	ldrh	r3, [r6, #6]
	adds	r3, r3, r0
	strh	r3, [r6, #6]
	mov	r3, r9
	ldr	r2, [r3, #8]
	mov	r3, r8
	adds	r3, #106
	ldrh	r1, [r2, r3]
	subs	r3, #2
	ldrh	r3, [r2, r3]
	subs	r7, r1, r3
	bl	sub_08014878
	adds	r3, r0, #0
	lsls	r0, r3, #2
	adds	r0, r0, r3
	lsls	r0, r0, #2
	lsrs	r0, r0, #16
	adds	r0, r0, r7
	movs	r1, #20
	bl	sub_0800205c
	ldrh	r3, [r6, #8]
	adds	r3, r3, r0
	mov	r0, r9
	ldr	r2, [r0, #8]
	strh	r3, [r6, #8]
	mov	r3, r8
	adds	r3, #118
	ldrh	r1, [r2, r3]
	subs	r3, #2
	ldrh	r3, [r2, r3]
	subs	r7, r1, r3
	bl	sub_08014878
	adds	r3, r0, #0
	lsls	r0, r3, #2
	adds	r0, r0, r3
	lsls	r0, r0, #2
	lsrs	r0, r0, #16
	adds	r0, r0, r7
	movs	r1, #20
	bl	sub_0800205c
	ldrh	r3, [r6, #10]
	adds	r3, r3, r0
	strh	r3, [r6, #10]
	mov	r3, r9
	ldr	r2, [r3, #8]
	mov	r3, r8
	adds	r3, #130
	ldrh	r1, [r2, r3]
	subs	r3, #2
	ldrh	r3, [r2, r3]
	subs	r7, r1, r3
	bl	sub_08014878
	adds	r3, r0, #0
	lsls	r0, r3, #2
	adds	r0, r0, r3
	lsls	r0, r0, #2
	lsrs	r0, r0, #16
	adds	r0, r0, r7
	movs	r1, #20
	bl	sub_0800205c
	ldrh	r3, [r6, #12]
	adds	r3, r3, r0
	mov	r0, r9
	ldr	r2, [r0, #8]
	strh	r3, [r6, #12]
	adds	r3, r5, #0
	adds	r3, #141
	ldrb	r1, [r2, r3]
	subs	r3, #1
	ldrb	r3, [r2, r3]
	subs	r7, r1, r3
	bl	sub_08014878
	adds	r3, r0, #0
	lsls	r0, r3, #2
	adds	r0, r0, r3
	lsls	r0, r0, #2
	lsrs	r0, r0, #16
	movs	r1, #20
	adds	r0, r0, r7
	bl	sub_0800205c
	mov	r2, sl
	ldrh	r3, [r2, #16]
	ldrh	r1, [r6, #14]
	ldrh	r2, [r6, #4]
	adds	r1, r1, r0
	adds	r3, r3, r2
	mov	r0, sl
	strh	r3, [r0, #16]
	ldrh	r2, [r6, #6]
	ldrh	r3, [r0, #18]
	strh	r1, [r6, #14]
	adds	r3, r3, r2
	mov	r2, sl
	strh	r3, [r2, #18]
	ldrh	r3, [r2, #24]
	ldrh	r2, [r6, #8]
	adds	r3, r3, r2
	strh	r3, [r0, #24]
	ldrh	r2, [r6, #10]
	ldrh	r3, [r0, #26]
	adds	r3, r3, r2
	mov	r2, sl
	strh	r3, [r2, #26]
	ldrh	r3, [r2, #28]
	ldrh	r2, [r6, #12]
	adds	r3, r3, r2
	strh	r3, [r0, #28]
	ldrb	r3, [r0, #30]
	movs	r2, #0
	adds	r3, r3, r1
	strb	r3, [r0, #30]
	movs	r3, #1
	strb	r3, [r0, #31]
	mov	r3, sl
	adds	r3, #32
	strb	r2, [r3, #0]
	adds	r3, #1
	strb	r2, [r3, #0]
	mov	r0, fp
	bl	sub_080b02d4
	mov	r0, fp
	bl	sub_080ad3f8
.L_080afb6a:
	mov	r0, r9
	bl	sub_08013164
	adds	r0, r6, #0
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
