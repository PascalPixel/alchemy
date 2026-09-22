.syntax unified
	.thumb
	.set sub_08002096, 0x08002096
	.set sub_08013560, 0x08013560
	.set sub_08013c58, 0x08013c58
	.set sub_08013d0c, 0x08013d0c
	.set sub_08014128, 0x08014128
	.set sub_08014274, 0x08014274
	.set sub_080143e0, 0x080143e0
	.set sub_080143f8, 0x080143f8
	.set sub_08016ca4, 0x08016ca4
	.set sub_080200c8, 0x080200c8
	.set sub_08038048, 0x08038048
	.set sub_080383d0, 0x080383d0
	.set sub_080ad0d8, 0x080ad0d8
	.set sub_0811bc64, 0x0811bc64
	.set sub_0811be3c, 0x0811be3c
	.set sub_0811f3b8, 0x0811f3b8
	.set sub_081c0010, 0x081c0010
	.global Overlay_0811ff08
Overlay_0811ff08:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #16
	b.n	.L_0811ff20
.L_0811ff1a:
	movs	r0, #1
	bl	sub_08013560
.L_0811ff20:
	bl	sub_08038048
	cmp	r0, #0
	beq.n	.L_0811ff1a
	movs	r0, #128
	add	r7, sp, #4
	bl	sub_080143e0
	mov	fp, r0
	movs	r0, #0
.L_0811ff34:
	str	r0, [sp, #0]
	ldr	r1, [pc, #148]
	movs	r3, #7
	ldr	r5, [r1, #0]
	movs	r6, #128
	lsrs	r5, r5, #2
	ands	r5, r3
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #148
	ldr	r3, [r3, #0]
	ldr	r2, [pc, #132]
	ldr	r0, [r3, #0]
	lsls	r6, r6, #19
	ldr	r3, [r3, #4]
	adds	r6, #74
	mov	r8, r1
	mov	sl, r0
	movs	r1, #4
	adds	r0, r6, #0
	lsls	r5, r5, #7
	adds	r5, r5, r2
	mov	r9, r3
	bl	sub_08013d0c
	adds	r0, r6, #0
	movs	r1, #16
	bl	sub_08013c58
	movs	r2, #16
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #82
	strh	r2, [r3, #0]
	movs	r3, #164
	lsls	r3, r3, #8
	str	r3, [r7, #4]
	movs	r3, #0
	str	r3, [r7, #8]
	adds	r1, r5, #0
	mov	r0, fp
	bl	sub_080143f8
	ldr	r3, [pc, #48]
	ldr	r2, [pc, #48]
	ands	r0, r3
	ldrh	r3, [r7, #8]
	ands	r3, r2
	orrs	r3, r0
	strh	r3, [r7, #8]
	mov	r0, r9
	mov	r3, sl
	movs	r2, #12
	ldrsh	r1, [r3, r2]
	ldrh	r3, [r0, #4]
	lsls	r1, r1, #3
	lsrs	r3, r3, #8
	adds	r1, r1, r3
	ldr	r3, [pc, #24]
	adds	r1, #4
	ands	r1, r3
	ldr	r2, [pc, #24]
	ldrh	r3, [r7, #6]
	ands	r3, r2
	orrs	r3, r1
	mov	r1, r8
	ldr	r0, [r1, #0]
	b.n	.L_0811ffd4
	.4byte 0x000003ff
	.4byte 0xfffffc00
	.4byte 0x000001ff
	.4byte 0xfffffe00
	.4byte 0x0300122c
	.2byte 0x996c
	.2byte 0x0812
.L_0811ffd4:
	strh	r3, [r7, #6]
	lsls	r0, r0, #12
	bl	sub_08002096
	cmp	r0, #0
	bge.n	.L_0811ffe8
	movs	r2, #254
	lsls	r2, r2, #7
	adds	r2, #255
	adds	r0, r0, r2
.L_0811ffe8:
	mov	r1, sl
	asrs	r2, r0, #15
	movs	r0, #14
	ldrsh	r3, [r1, r0]
	mov	r0, r9
	lsls	r3, r3, #3
	adds	r2, r2, r3
	ldrh	r3, [r0, #6]
	movs	r1, #240
	lsrs	r3, r3, #8
	adds	r3, r3, r2
	adds	r3, #6
	strb	r3, [r7, #4]
	adds	r0, r7, #0
	bl	sub_08014128
	ldr	r1, [pc, #80]
	movs	r2, #2
	ldr	r3, [r1, #0]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_0812003c
	ldr	r3, [r1, #4]
	movs	r2, #129
	lsls	r2, r2, #2
	adds	r2, #255
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_0812003c
	ldr	r3, [sp, #0]
	cmp	r3, #15
	ble.n	.L_08120030
	ldr	r3, [r1, #0]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_0812003c
.L_08120030:
	movs	r0, #1
	bl	sub_08013560
	ldr	r0, [sp, #0]
	adds	r0, #1
	b.n	.L_0811ff34
.L_0812003c:
	movs	r0, #111
	bl	sub_081c0010
	mov	r0, fp
	bl	sub_08014274
	movs	r0, #1
	bl	sub_08013560
	add	sp, #16
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x1150
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	adds	r5, r0, #0
	movs	r0, #0
	sub	sp, #20
	adds	r6, r1, #0
	bl	sub_080383d0
	mov	sl, r0
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	b.n	.L_08120082
.L_0812007c:
	movs	r0, #1
	bl	sub_08013560
.L_08120082:
	bl	sub_08038048
	cmp	r0, #0
	beq.n	.L_0812007c
	movs	r0, #128
	add	r5, sp, #8
	bl	sub_080143e0
	mov	r3, sp
	ldrh	r3, [r3, #0]
	movs	r7, #128
	lsls	r7, r7, #19
	adds	r6, r0, #0
	adds	r7, #74
	mov	r8, r3
.L_081200a0:
	adds	r0, r7, #0
	movs	r1, #4
	bl	sub_08013d0c
	adds	r0, r7, #0
	movs	r1, #16
	bl	sub_08013c58
	movs	r2, #16
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #82
	strh	r2, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #23
	str	r3, [r5, #4]
	movs	r3, #0
	str	r3, [r5, #8]
	mov	r1, sl
	adds	r0, r6, #0
	bl	sub_080143f8
	ldr	r3, [pc, #48]
	ldr	r2, [pc, #52]
	ands	r0, r3
	ldrh	r3, [r5, #8]
	ldr	r1, [pc, #48]
	ands	r3, r2
	orrs	r3, r0
	strh	r3, [r5, #8]
	ldr	r3, [pc, #48]
	ldr	r0, [r3, #0]
	movs	r3, #4
	ands	r0, r3
	movs	r3, #255
	lsrs	r2, r0, #1
	lsls	r3, r3, #8
	adds	r3, #252
	add	r2, r8
	adds	r2, r2, r3
	ldr	r3, [pc, #24]
	lsrs	r0, r0, #2
	ands	r2, r3
	ldrh	r3, [r5, #6]
	ands	r3, r1
	orrs	r3, r2
	strh	r3, [r5, #6]
	b.n	.L_08120114
	.4byte 0x000003ff
	.4byte 0xfffffc00
	.4byte 0xfffffe00
	.4byte 0x000001ff
	.2byte 0x122c
	.2byte 0x0300
.L_08120114:
	ldr	r3, [sp, #4]
	movs	r1, #240
	subs	r0, r3, r0
	adds	r0, #248
	strb	r0, [r5, #4]
	adds	r0, r5, #0
	bl	sub_08014128
	ldr	r3, [pc, #44]
	movs	r2, #129
	ldr	r3, [r3, #4]
	lsls	r2, r2, #2
	adds	r2, #255
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_0812013c
	movs	r0, #1
	bl	sub_08013560
	b.n	.L_081200a0
.L_0812013c:
	adds	r0, r6, #0
	bl	sub_08014274
	movs	r0, #1
	bl	sub_08013560
	movs	r0, #1
	add	sp, #20
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x1150
	.2byte 0x0300
	push	{lr}
	b.n	.L_08120162
.L_0812015c:
	movs	r0, #1
	bl	sub_08013560
.L_08120162:
	bl	sub_08038048
	cmp	r0, #0
	beq.n	.L_0812015c
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #148
	ldr	r2, [r3, #0]
	movs	r3, #1
	str	r3, [r2, #8]
	pop	{pc}
	push	{r5, lr}
	adds	r5, r0, #0
	bl	sub_08016ca4
	movs	r2, #149
	lsls	r2, r2, #1
	adds	r3, r0, r2
	ldrb	r3, [r3, #0]
	cmp	r3, #1
	bne.n	.L_081201b2
	movs	r1, #0
	bl	sub_080ad0d8
	adds	r0, r5, #0
	bl	sub_0811f3b8
	adds	r0, r5, #0
	bl	sub_0811bc64
	adds	r0, r5, #0
	bl	sub_0811be3c
	adds	r5, r0, #0
	ldr	r0, [r5, #0]
	bl	sub_080200c8
	movs	r3, #0
	str	r3, [r5, #0]
	strh	r3, [r5, #40]
.L_081201b2:
	pop	{r5, pc}
