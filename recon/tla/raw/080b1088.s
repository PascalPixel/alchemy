.syntax unified
	.thumb
	.set sub_08016ca4, 0x08016ca4
	.set sub_08016cfc, 0x08016cfc
	.set sub_08016d18, 0x08016d18
	.set sub_080ad3f8, 0x080ad3f8
	.set sub_080ae16c, 0x080ae16c
	.set sub_080aec04, 0x080aec04
	.set sub_080b0298, 0x080b0298
	.set sub_080b106c, 0x080b106c
	.global Func_080b1088
	.thumb_func
Func_080b1088:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r0, [pc, #196]
	movs	r2, #206
	ldrh	r3, [r0, #0]
	lsls	r2, r2, #7
	adds	r2, #116
	sub	sp, #4
	mov	r8, r0
	cmp	r3, r2
	bne.n	.L_080b10aa
	b.n	.L_080b11c0
.L_080b10aa:
	mov	r1, r8
	ldr	r3, [pc, #176]
	strh	r2, [r1, #0]
	movs	r0, #144
	lsls	r0, r0, #2
	movs	r2, #2
	add	r8, r2
	adds	r2, r3, r0
	movs	r0, #0
	ldrsh	r1, [r2, r0]
	mov	fp, r1
	movs	r1, #128
	lsls	r1, r1, #2
	adds	r1, #66
	adds	r3, r3, r1
	movs	r0, #0
	ldrsh	r2, [r3, r0]
	movs	r1, #4
	str	r2, [sp, #0]
	mov	sl, r1
.L_080b10d2:
	mov	r0, sl
	bl	sub_08016ca4
	adds	r7, r0, #0
	adds	r2, r7, #0
	adds	r2, #216
	movs	r6, #14
.L_080b10e0:
	ldrh	r3, [r2, #0]
	mov	r0, r8
	movs	r1, #2
	subs	r6, #1
	adds	r2, #2
	strh	r3, [r0, #0]
	add	r8, r1
	cmp	r6, #0
	bge.n	.L_080b10e0
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r5, r7, #0
	mov	r9, r2
	movs	r6, #14
	adds	r5, #216
.L_080b10fe:
	ldrh	r0, [r5, #0]
	bl	sub_080aec04
	ldrb	r3, [r0, #2]
	movs	r0, #192
	adds	r3, #255
	lsls	r3, r3, #24
	lsls	r0, r0, #18
	cmp	r3, r0
	bhi.n	.L_080b1122
	ldrh	r3, [r5, #0]
	mov	r1, r9
	ands	r3, r1
	lsls	r3, r3, #16
	lsrs	r3, r3, #16
	cmp	r3, #0
	bne.n	.L_080b1122
	strh	r3, [r5, #0]
.L_080b1122:
	ldrh	r0, [r5, #0]
	bl	sub_080b106c
	cmp	r0, #0
	beq.n	.L_080b1130
	ldr	r3, [pc, #40]
	strh	r3, [r5, #0]
.L_080b1130:
	subs	r6, #1
	adds	r5, #2
	cmp	r6, #0
	bge.n	.L_080b10fe
	adds	r0, r7, #0
	adds	r0, #216
	movs	r5, #0
	adds	r4, r0, #0
	adds	r1, r0, #0
	movs	r6, #14
.L_080b1144:
	ldrh	r2, [r4, #0]
	adds	r4, #2
	lsls	r3, r2, #16
	cmp	r3, #0
	beq.n	.L_080b1164
	strh	r2, [r1, #0]
	adds	r5, #1
	adds	r1, #2
	b.n	.L_080b1164
	movs	r0, r0
	.4byte 0x00000000
	.4byte 0x020023c4
	.2byte 0x0240
	.2byte 0x0200
.L_080b1164:
	subs	r6, #1
	cmp	r6, #0
	bge.n	.L_080b1144
	cmp	r5, #14
	bgt.n	.L_080b1188
	lsls	r3, r5, #1
	ldr	r2, [pc, #16]
	adds	r0, r3, r0
	movs	r3, #15
	subs	r5, r3, r5
.L_080b1178:
	subs	r5, #1
	strh	r2, [r0, #0]
	adds	r0, #2
	cmp	r5, #0
	bne.n	.L_080b1178
	b.n	.L_080b1188
	.2byte 0x0000
	.2byte 0x0000
.L_080b1188:
	mov	r0, sl
	bl	sub_080b0298
	mov	r0, sl
	bl	sub_080ad3f8
	movs	r2, #1
	add	sl, r2
	mov	r3, sl
	cmp	r3, #7
	ble.n	.L_080b10d2
	mov	r3, sp
	movs	r2, #2
	ldrh	r3, [r3, #0]
	mov	r1, r8
	mov	r0, fp
	add	r8, r2
	strh	r0, [r1, #0]
	mov	r0, r8
	strh	r3, [r0, #0]
	ldr	r0, [pc, #64]
	add	r8, r2
	ldrh	r3, [r0, #0]
	mov	r1, r8
	strh	r3, [r1, #0]
	mov	r2, r8
	ldrh	r3, [r0, #2]
	strh	r3, [r2, #2]
.L_080b11c0:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #255
	bl	sub_08016cfc
	movs	r0, #1
	bl	sub_080ae16c
	movs	r6, #229
	lsls	r6, r6, #4
.L_080b11d4:
	adds	r0, r6, #0
	bl	sub_08016d18
	movs	r3, #224
	lsls	r3, r3, #4
	adds	r6, #1
	adds	r3, #89
	cmp	r6, r3
	ble.n	.L_080b11d4
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0458
	.2byte 0x0200
	push	{r5, r6, lr}
	ldr	r5, [pc, #120]
	movs	r0, #206
	ldrh	r3, [r5, #0]
	lsls	r0, r0, #7
	mov	ip, r3
	adds	r0, #116
	adds	r5, #2
	cmp	ip, r0
	bne.n	.L_080b1268
	movs	r6, #4
.L_080b120e:
	adds	r0, r6, #0
	bl	sub_08016ca4
	movs	r2, #14
	adds	r0, #216
.L_080b1218:
	ldrh	r3, [r5, #0]
	subs	r2, #1
	strh	r3, [r0, #0]
	adds	r5, #2
	adds	r0, #2
	cmp	r2, #0
	bge.n	.L_080b1218
	adds	r0, r6, #0
	bl	sub_080b0298
	adds	r0, r6, #0
	adds	r6, #1
	bl	sub_080ad3f8
	cmp	r6, #7
	ble.n	.L_080b120e
	ldr	r1, [pc, #60]
	ldrh	r2, [r5, #0]
	movs	r4, #144
	lsls	r4, r4, #2
	adds	r3, r1, r4
	strh	r2, [r3, #0]
	adds	r5, #2
	ldrh	r2, [r5, #0]
	adds	r4, #2
	adds	r3, r1, r4
	strh	r2, [r3, #0]
	movs	r3, #134
	adds	r5, #2
	lsls	r3, r3, #2
	adds	r2, r1, r3
	ldrh	r3, [r5, #0]
	subs	r4, #40
	strh	r3, [r2, #0]
	movs	r0, #0
	ldrh	r3, [r5, #2]
	ldr	r5, [pc, #16]
	adds	r2, r1, r4
	strh	r3, [r2, #0]
	strh	r0, [r5, #0]
.L_080b1268:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #255
	bl	sub_08016d18
	pop	{r5, r6, pc}
	.4byte 0x020023c4
	.4byte 0x02000240
