.syntax unified
	.thumb
	.set sub_080030f8, 0x080030f8
	.set sub_08015070, 0x08015070
	.set sub_08015078, 0x08015078
	.set sub_08015080, 0x08015080
	.set sub_08015270, 0x08015270
	.set sub_08015278, 0x08015278
	.set sub_08015280, 0x08015280
	.set sub_080770c0, 0x080770c0
	.set sub_080a9d84, 0x080a9d84
	.set sub_080f9010, 0x080f9010
	.global Overlay_080a7850
Overlay_080a7850:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #108]
	ldr	r3, [r3, #0]
	sub	sp, #28
	str	r3, [sp, #24]
	bl	sub_080a9d84
	movs	r0, #112
	bl	sub_080f9010
	movs	r1, #134
	ldr	r0, [sp, #24]
	lsls	r1, r1, #1
	adds	r5, r0, r1
	ldr	r0, [r5, #0]
	bl	sub_08015270
	ldr	r1, [r5, #0]
	ldr	r0, [pc, #76]
	movs	r2, #0
	movs	r3, #16
	bl	sub_08015080
	ldr	r2, [sp, #24]
	ldr	r3, [r2, #20]
	movs	r2, #13
	strb	r2, [r3, #5]
	ldr	r0, [sp, #24]
	movs	r1, #190
	lsls	r1, r1, #1
	adds	r3, r0, r1
	ldr	r3, [r3, #0]
	movs	r0, #1
	strb	r2, [r3, #5]
	bl	sub_080030f8
	ldr	r2, [sp, #24]
	ldr	r3, [sp, #24]
	ldr	r2, [r2, #36]
	movs	r0, #162
	movs	r1, #141
	lsls	r0, r0, #1
	lsls	r1, r1, #2
	ldr	r5, [pc, #16]
	ldr	r4, [pc, #16]
	mov	sl, r2
	adds	r2, r3, r0
	adds	r3, r3, r1
	movs	r0, #32
	movs	r1, #3
	b.n	.L_080a78d4
	movs	r0, r0
	.4byte 0x00000046
	.4byte 0x0000001e
	.4byte 0x03001f2c
	.2byte 0x0b17
	.2byte 0x0000
.L_080a78d4:
	subs	r1, #1
	strh	r0, [r3, #0]
	strh	r5, [r3, #8]
	adds	r0, #56
	strh	r4, [r2, #0]
	adds	r3, #2
	adds	r2, #2
	cmp	r1, #0
	bge.n	.L_080a78d4
	mov	r0, sl
	bl	sub_08015270
	movs	r3, #11
	str	r3, [sp, #0]
	mov	r0, sl
	movs	r1, #0
	movs	r2, #11
	movs	r3, #28
	bl	sub_08015070
	movs	r1, #134
	ldr	r2, [sp, #24]
	lsls	r1, r1, #1
	adds	r3, r2, r1
	movs	r2, #96
	ldr	r1, [r3, #0]
	negs	r2, r2
	ldr	r0, [pc, #212]
	movs	r3, #132
	bl	sub_08015078
	movs	r2, #0
	str	r2, [sp, #12]
	str	r2, [sp, #8]
	str	r2, [sp, #4]
	mov	r9, r2
.L_080a791c:
	ldr	r3, [sp, #4]
	ldr	r0, [sp, #8]
	ldr	r1, [sp, #12]
	movs	r2, #24
	adds	r5, r3, #0
	str	r3, [sp, #20]
	str	r0, [sp, #16]
	movs	r6, #0
	mov	fp, r1
	mov	r8, r2
	adds	r5, #48
.L_080a7932:
	ldr	r3, [sp, #20]
	adds	r0, r5, #0
	adds	r7, r3, r6
	bl	sub_080770c0
	cmp	r0, #0
	beq.n	.L_080a7964
	movs	r2, #0
	str	r2, [sp, #0]
	ldr	r1, [pc, #156]
	ldr	r2, [sp, #16]
	adds	r3, r6, #3
	mov	r0, sl
	add	r1, r9
	adds	r2, #1
	bl	sub_08015280
	ldr	r0, [pc, #144]
	mov	r2, fp
	adds	r0, r7, r0
	mov	r1, sl
	adds	r2, #16
	mov	r3, r8
	bl	sub_08015080
.L_080a7964:
	movs	r0, #8
	adds	r6, #1
	add	r8, r0
	adds	r5, #1
	cmp	r6, #6
	ble.n	.L_080a7932
	ldr	r1, [sp, #12]
	ldr	r2, [sp, #8]
	ldr	r3, [sp, #4]
	movs	r0, #1
	adds	r1, #56
	add	r9, r0
	str	r1, [sp, #12]
	adds	r2, #7
	adds	r3, #20
	mov	r1, r9
	str	r2, [sp, #8]
	str	r3, [sp, #4]
	cmp	r1, #3
	ble.n	.L_080a791c
	ldr	r3, [pc, #92]
	ldr	r2, [pc, #96]
	ldr	r3, [r3, #0]
	adds	r3, r3, r2
	movs	r2, #1
	strb	r2, [r3, #0]
	ldr	r6, [pc, #88]
	movs	r5, #7
.L_080a799c:
	movs	r0, #168
	lsls	r0, r0, #1
	bl	sub_080770c0
	cmp	r0, #0
	bne.n	.L_080a79b6
	movs	r0, #1
	bl	sub_080030f8
	ldr	r3, [r6, #0]
	ands	r3, r5
	cmp	r3, #0
	beq.n	.L_080a799c
.L_080a79b6:
	ldr	r3, [sp, #24]
	ldr	r0, [r3, #36]
	bl	sub_08015270
	movs	r1, #134
	ldr	r0, [sp, #24]
	lsls	r1, r1, #1
	adds	r3, r0, r1
	ldr	r0, [r3, #0]
	bl	sub_08015278
	movs	r1, #141
	ldr	r2, [sp, #24]
	lsls	r1, r1, #2
	ldr	r0, [pc, #8]
	adds	r3, r2, r1
	movs	r1, #130
	movs	r2, #3
	b.n	.L_080a79f8
	.4byte 0x00000080
	.4byte 0x00000b18
	.4byte 0x00001001
	.4byte 0x0000045f
	.4byte 0x03001e8c
	.4byte 0x00000ea3
	.2byte 0x1c94
	.2byte 0x0300
.L_080a79f8:
	subs	r2, #1
	strh	r1, [r3, #0]
	strh	r0, [r3, #8]
	adds	r1, #32
	adds	r3, #2
	cmp	r2, #0
	bge.n	.L_080a79f8
	ldr	r2, [sp, #24]
	ldr	r3, [r2, #20]
	movs	r2, #1
	strb	r2, [r3, #5]
	ldr	r0, [sp, #24]
	movs	r1, #190
	lsls	r1, r1, #1
	adds	r3, r0, r1
	ldr	r3, [r3, #0]
	movs	r0, #113
	strb	r2, [r3, #5]
	bl	sub_080f9010
	add	sp, #28
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
