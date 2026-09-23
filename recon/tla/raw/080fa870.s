.syntax unified
	.thumb
	.set sub_08013560, 0x08013560
	.set sub_080145a8, 0x080145a8
	.set sub_08016ca4, 0x08016ca4
	.set sub_08016ce4, 0x08016ce4
	.set sub_08016d18, 0x08016d18
	.set sub_08020030, 0x08020030
	.set sub_08038068, 0x08038068
	.set sub_08038070, 0x08038070
	.set sub_08038080, 0x08038080
	.set sub_080380b0, 0x080380b0
	.set sub_08038260, 0x08038260
	.set sub_080ad1c0, 0x080ad1c0
	.set sub_080f8170, 0x080f8170
	.set sub_080f8888, 0x080f8888
	.set sub_080f8a44, 0x080f8a44
	.set sub_080f93a4, 0x080f93a4
	.set sub_080fac58, 0x080fac58
	.set sub_080faca0, 0x080faca0
	.set sub_080fad1c, 0x080fad1c
	.set sub_080fad48, 0x080fad48
	.set sub_080fae2c, 0x080fae2c
	.set sub_080fae8c, 0x080fae8c
	.set sub_08104c00, 0x08104c00
	.set sub_08104d5c, 0x08104d5c
	.set sub_08104ef8, 0x08104ef8
	.set sub_081c0010, 0x081c0010
	.global Func_080fa870
	.thumb_func
Func_080fa870:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r6, [r3, #0]
	sub	sp, #16
	movs	r3, #29
	ldrsb	r3, [r6, r3]
	movs	r2, #139
	str	r3, [sp, #12]
	lsls	r2, r2, #1
	adds	r2, #255
	adds	r3, r6, r2
	ldrb	r3, [r3, #0]
	ldr	r1, [r6, #36]
	mov	r9, r3
	movs	r3, #0
	str	r3, [sp, #8]
	str	r3, [sp, #4]
	mov	r8, r1
	movs	r3, #12
	str	r3, [sp, #0]
	movs	r2, #5
	movs	r1, #13
	movs	r3, #17
	mov	sl, r0
	mov	r0, r8
	bl	sub_080f93a4
	ldr	r0, [r6, #36]
	bl	sub_08038260
	movs	r3, #28
	ldrsb	r3, [r6, r3]
	movs	r1, #129
	lsls	r1, r1, #2
	lsls	r3, r3, #1
	adds	r3, r3, r1
	ldrh	r0, [r6, r3]
	bl	sub_08016ca4
	movs	r1, #144
	ldr	r0, [pc, #16]
	lsls	r1, r1, #3
	bl	sub_080145a8
	movs	r2, #128
	lsls	r2, r2, #1
	adds	r2, #255
	movs	r5, #2
	mov	fp, r2
	b.n	.L_080fab36
	.2byte 0xabe1
	.2byte 0x080f
.L_080fa8e8:
	lsls	r3, r5, #24
	asrs	r7, r3, #24
	cmp	r7, #0
	bne.n	.L_080fa8f2
	b.n	.L_080faab4
.L_080fa8f2:
	ldr	r3, [r6, #36]
	movs	r1, #129
	mov	r8, r3
	ldr	r3, [sp, #12]
	lsls	r1, r1, #2
	lsls	r3, r3, #1
	adds	r3, r3, r1
	ldrh	r0, [r6, r3]
	bl	sub_08016ca4
	ldr	r3, [r6, #16]
	ldr	r4, [sp, #12]
	ldrh	r2, [r3, #12]
	lsls	r1, r4, #1
	adds	r1, r1, r4
	ldr	r5, [r6, #24]
	adds	r2, r2, r1
	ldr	r3, [pc, #36]
	lsls	r2, r2, #3
	subs	r2, #2
	strh	r2, [r5, #6]
	ands	r2, r3
	mov	r3, fp
	ands	r2, r3
	ldr	r1, [pc, #28]
	ldrh	r3, [r5, #22]
	ands	r3, r1
	orrs	r3, r2
	strh	r3, [r5, #22]
	cmp	r7, #2
	bne.n	.L_080fa952
	adds	r0, r4, #0
	cmp	r4, #0
	bge.n	.L_080fa944
	adds	r0, r4, #3
	b.n	.L_080fa944
	movs	r0, r0
	.4byte 0x0000ffff
	.2byte 0xfe00
	.2byte 0xffff
.L_080fa944:
	asrs	r0, r0, #2
	lsls	r0, r0, #2
	bl	sub_08104ef8
	movs	r0, #1
	bl	sub_08013560
.L_080fa952:
	ldr	r1, [sp, #12]
	ldr	r0, [r6, #16]
	mov	r2, r9
	bl	sub_08104d5c
	mov	r1, sl
	cmp	r1, #1
	bne.n	.L_080faa20
	ldr	r3, [sp, #12]
	movs	r2, #129
	lsls	r2, r2, #2
	lsls	r3, r3, #1
	adds	r3, r3, r2
	ldrh	r0, [r6, r3]
	movs	r1, #1
	bl	sub_080fae2c
	movs	r3, #9
	str	r3, [sp, #0]
	mov	r0, r8
	movs	r1, #0
	movs	r2, #9
	movs	r3, #16
	bl	sub_08038070
	movs	r3, #80
	str	r3, [sp, #0]
	movs	r2, #72
	movs	r3, #120
	mov	r0, r8
	movs	r1, #0
	bl	sub_08038068
	movs	r3, #28
	ldrsb	r3, [r6, r3]
	ldr	r2, [sp, #12]
	cmp	r2, r3
	beq.n	.L_080faa00
	movs	r1, #129
	lsls	r3, r2, #1
	lsls	r1, r1, #2
	movs	r2, #182
	adds	r3, r3, r1
	lsls	r2, r2, #1
	ldrh	r0, [r6, r3]
	adds	r3, r6, r2
	ldrh	r3, [r3, #0]
	mov	r1, fp
	ands	r1, r3
	bl	sub_080fad48
	ldr	r3, [sp, #12]
	movs	r1, #129
	lsls	r3, r3, #1
	lsls	r1, r1, #2
	adds	r3, r3, r1
	adds	r5, r0, #0
	ldrh	r0, [r6, r3]
	bl	sub_080fad1c
	cmp	r0, #15
	bne.n	.L_080fa9d8
	cmp	r5, #0
	bne.n	.L_080fa9dc
	movs	r2, #0
	ldr	r0, [pc, #500]
	b.n	.L_080fa9f6
.L_080fa9d8:
	cmp	r5, #0
	beq.n	.L_080fa9f2
.L_080fa9dc:
	movs	r3, #72
	str	r3, [sp, #0]
	adds	r0, r5, #0
	movs	r1, #2
	mov	r2, r8
	movs	r3, #8
	bl	sub_080380b0
	movs	r2, #32
	ldr	r0, [pc, #480]
	b.n	.L_080fa9f6
.L_080fa9f2:
	movs	r2, #16
	ldr	r0, [pc, #476]
.L_080fa9f6:
	mov	r1, r8
	movs	r3, #72
	bl	sub_08038080
	ldr	r2, [sp, #12]
.L_080faa00:
	movs	r1, #128
	lsls	r1, r1, #2
	adds	r1, #22
	adds	r3, r6, r1
	subs	r1, #174
	ldrb	r0, [r3, #0]
	adds	r3, r6, r1
	ldrh	r1, [r3, #0]
	lsls	r3, r2, #1
	movs	r2, #129
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldrh	r3, [r6, r3]
	movs	r2, #0
	bl	sub_080fae8c
.L_080faa20:
	mov	r3, sl
	cmp	r3, #0
	bne.n	.L_080faab2
	movs	r1, #182
	lsls	r1, r1, #1
	adds	r3, r6, r1
	ldrh	r3, [r3, #0]
	mov	r0, fp
	ands	r0, r3
	bl	sub_080faca0
	cmp	r0, #0
	beq.n	.L_080faa56
	ldr	r3, [sp, #12]
	movs	r2, #129
	lsls	r2, r2, #2
	lsls	r3, r3, #1
	adds	r3, r3, r2
	subs	r2, #156
	ldrh	r1, [r6, r3]
	adds	r3, r6, r2
	ldrh	r2, [r3, #0]
	ldr	r0, [r6, #40]
	movs	r3, #8
	bl	sub_080f8170
	b.n	.L_080faa72
.L_080faa56:
	ldr	r3, [sp, #12]
	movs	r1, #129
	lsls	r1, r1, #2
	lsls	r3, r3, #1
	movs	r2, #180
	adds	r3, r3, r1
	lsls	r2, r2, #1
	ldrh	r1, [r6, r3]
	adds	r3, r6, r2
	ldrh	r2, [r3, #0]
	ldr	r0, [r6, #40]
	movs	r3, #0
	bl	sub_080f8170
.L_080faa72:
	movs	r0, #82
	adds	r0, #255
	bl	sub_08016ce4
	cmp	r0, #0
	bne.n	.L_080faaaa
	ldr	r3, [sp, #4]
	cmp	r3, #0
	bne.n	.L_080faaaa
	ldr	r0, [r6, #48]
	bl	sub_08038260
	movs	r1, #182
	lsls	r1, r1, #1
	adds	r3, r6, r1
	ldrh	r3, [r3, #0]
	mov	r0, fp
	ands	r0, r3
	ldr	r3, [pc, #320]
	movs	r2, #0
	adds	r0, r0, r3
	ldr	r1, [r6, #48]
	movs	r3, #0
	bl	sub_08038080
	movs	r2, #1
	str	r2, [sp, #4]
	b.n	.L_080faab2
.L_080faaaa:
	movs	r0, #82
	adds	r0, #255
	bl	sub_08016d18
.L_080faab2:
	movs	r5, #0
.L_080faab4:
	ldr	r2, [sp, #12]
	adds	r3, r2, #0
	cmp	r2, #0
	bge.n	.L_080faabe
	adds	r3, r2, #3
.L_080faabe:
	asrs	r3, r3, #2
	lsls	r3, r3, #2
	subs	r3, r2, r3
	lsls	r0, r3, #1
	adds	r0, r0, r3
	lsls	r0, r0, #3
	movs	r1, #16
	subs	r0, #10
	bl	sub_080f8a44
	movs	r0, #1
	bl	sub_08013560
	ldr	r1, [pc, #256]
	movs	r2, #1
	ldr	r3, [r1, #4]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080fab12
	mov	r3, sl
	cmp	r3, #1
	bne.n	.L_080faafc
	movs	r2, #28
	ldrsb	r2, [r6, r2]
	ldr	r3, [sp, #12]
	cmp	r3, r2
	bne.n	.L_080faafc
	movs	r0, #114
	bl	sub_081c0010
	b.n	.L_080fab36
.L_080faafc:
	movs	r0, #112
	bl	sub_081c0010
	ldr	r3, [sp, #12]
	movs	r1, #129
	lsls	r3, r3, #1
	lsls	r1, r1, #2
	adds	r3, r3, r1
	ldrb	r3, [r6, r3]
	str	r3, [sp, #8]
	b.n	.L_080fab44
.L_080fab12:
	ldr	r3, [r1, #4]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080fab28
	movs	r0, #113
	bl	sub_081c0010
	movs	r2, #255
	str	r2, [sp, #8]
	b.n	.L_080fab44
.L_080fab28:
	add	r0, sp, #12
	mov	r1, r9
	movs	r2, #4
	bl	sub_08104c00
	lsls	r0, r0, #24
	lsrs	r5, r0, #24
.L_080fab36:
	movs	r0, #168
	lsls	r0, r0, #1
	bl	sub_08016ce4
	cmp	r0, #0
	bne.n	.L_080fab44
	b.n	.L_080fa8e8
.L_080fab44:
	ldr	r5, [r6, #24]
	movs	r7, #13
	adds	r0, r5, #0
	bl	sub_080f8888
	strb	r7, [r5, #5]
	bl	sub_080fac58
	movs	r0, #1
	bl	sub_08013560
	ldr	r3, [sp, #12]
	movs	r2, #129
	strb	r3, [r6, #29]
	ldr	r3, [sp, #12]
	lsls	r2, r2, #2
	lsls	r3, r3, #1
	adds	r3, r3, r2
	ldrh	r3, [r6, r3]
	movs	r1, #140
	str	r3, [r6, #8]
	ldr	r3, [sp, #12]
	lsls	r1, r1, #1
	lsls	r3, r3, #1
	adds	r3, r3, r2
	ldrh	r2, [r6, r3]
	adds	r1, #255
	adds	r3, r6, r1
	strb	r2, [r3, #0]
	movs	r3, #28
	ldrsb	r3, [r6, r3]
	str	r3, [sp, #12]
	movs	r2, #30
	ldrsb	r2, [r6, r2]
	mov	r9, r2
	cmp	r3, #0
	bge.n	.L_080fab90
	adds	r3, #3
.L_080fab90:
	asrs	r0, r3, #2
	lsls	r0, r0, #2
	bl	sub_08104ef8
	ldr	r0, [r6, #16]
	ldr	r1, [sp, #12]
	mov	r2, r9
	bl	sub_08104d5c
	movs	r1, #188
	lsls	r1, r1, #1
	adds	r3, r6, r1
	ldr	r3, [r3, #0]
	movs	r2, #190
	lsls	r2, r2, #1
	strb	r7, [r3, #5]
	adds	r3, r6, r2
	ldr	r3, [r3, #0]
	strb	r7, [r3, #5]
	ldr	r3, [sp, #8]
	add	sp, #16
	lsls	r0, r3, #24
	asrs	r0, r0, #24
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x0000105f
	.4byte 0x0000105e
	.4byte 0x00001060
	.4byte 0x00000092
	.2byte 0x1150
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r6, [r3, #0]
	ldr	r3, [pc, #72]
	movs	r2, #31
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_080fac54
	movs	r7, #0
.L_080fabf8:
	asrs	r5, r7, #24
	movs	r2, #172
	lsls	r2, r2, #1
	lsls	r3, r5, #1
	adds	r3, r3, r2
	ldrsh	r0, [r6, r3]
	movs	r3, #1
	negs	r3, r3
	cmp	r0, r3
	beq.n	.L_080fac44
	movs	r2, #182
	lsls	r2, r2, #1
	adds	r3, r6, r2
	ldrh	r3, [r3, #0]
	ldr	r1, [pc, #24]
	ands	r1, r3
	bl	sub_080ad1c0
	cmp	r0, #0
	beq.n	.L_080fac38
	lsls	r3, r5, #2
	adds	r3, #248
	ldr	r0, [r6, r3]
	movs	r1, #3
	bl	sub_08020030
	b.n	.L_080fac44
	movs	r0, r0
	.4byte 0x000001ff
	.2byte 0x122c
	.2byte 0x0300
.L_080fac38:
	lsls	r3, r5, #2
	adds	r3, #248
	ldr	r0, [r6, r3]
	movs	r1, #1
	bl	sub_08020030
.L_080fac44:
	movs	r2, #128
	lsls	r2, r2, #17
	adds	r3, r7, r2
	movs	r2, #192
	lsls	r2, r2, #18
	adds	r7, r3, #0
	cmp	r3, r2
	ble.n	.L_080fabf8
.L_080fac54:
	pop	{r5, r6, r7, pc}
	.align 2, 0
