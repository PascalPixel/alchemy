.syntax unified
	.thumb
	.set sub_0800205c, 0x0800205c
	.set sub_0800206c, 0x0800206c
	.set sub_08013560, 0x08013560
	.set sub_08014274, 0x08014274
	.set sub_080145a8, 0x080145a8
	.set sub_08016ca4, 0x08016ca4
	.set sub_08038010, 0x08038010
	.set sub_08038018, 0x08038018
	.set sub_08038038, 0x08038038
	.set sub_08038048, 0x08038048
	.set sub_08038060, 0x08038060
	.set sub_08038068, 0x08038068
	.set sub_08038078, 0x08038078
	.set sub_08038080, 0x08038080
	.set sub_08038090, 0x08038090
	.set sub_080380b0, 0x080380b0
	.set sub_08038120, 0x08038120
	.set sub_08038260, 0x08038260
	.set sub_08038268, 0x08038268
	.set sub_08038270, 0x08038270
	.set sub_08038290, 0x08038290
	.set sub_080383d8, 0x080383d8
	.set sub_080ad008, 0x080ad008
	.set sub_080ad158, 0x080ad158
	.set sub_080ad160, 0x080ad160
	.set sub_080ad168, 0x080ad168
	.set sub_080ad170, 0x080ad170
	.set sub_080f8a44, 0x080f8a44
	.set sub_080f8ab4, 0x080f8ab4
	.set sub_08100e28, 0x08100e28
	.set sub_08100e5c, 0x08100e5c
	.set sub_08101a04, 0x08101a04
	.set sub_08101ac8, 0x08101ac8
	.set sub_08101c0c, 0x08101c0c
	.set sub_08101c18, 0x08101c18
	.set sub_08101c40, 0x08101c40
	.set sub_08104928, 0x08104928
	.set sub_08104d14, 0x08104d14
	.set sub_08104ef8, 0x08104ef8
	.set sub_081052ac, 0x081052ac
	.set sub_08105300, 0x08105300
	.set sub_0810532c, 0x0810532c
	.set sub_08105350, 0x08105350
	.set sub_081180f8, 0x081180f8
	.set sub_081c0010, 0x081c0010
	.global Overlay_08101c7c
Overlay_08101c7c:
.L_08101c7c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	adds	r5, r3, #0
	ldr	r3, [sp, #32]
	adds	r6, r2, #0
	mov	sl, r3
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #60]
	mov	r7, sl
	lsls	r7, r7, #12
	mov	r8, r3
	mov	sl, r7
	cmp	r0, #0
	bge.n	.L_08101ca8
	adds	r6, r6, r0
	movs	r0, #0
.L_08101ca8:
	adds	r3, r0, r6
	cmp	r3, #29
	ble.n	.L_08101cb2
	movs	r3, #30
	subs	r6, r3, r0
.L_08101cb2:
	cmp	r1, #0
	bge.n	.L_08101cba
	adds	r5, r5, r1
	movs	r1, #0
.L_08101cba:
	adds	r3, r1, r5
	cmp	r3, #29
	ble.n	.L_08101cc4
	movs	r3, #20
	subs	r5, r3, r1
.L_08101cc4:
	cmp	r6, #0
	ble.n	.L_08101d24
	cmp	r5, #0
	ble.n	.L_08101d24
	lsls	r3, r0, #1
	lsls	r2, r1, #6
	add	r3, r8
	adds	r2, r2, r3
	movs	r0, #2
	mov	ip, r2
	mov	r9, r0
.L_08101cda:
	mov	r0, ip
	adds	r4, r6, #0
	adds	r0, #8
	cmp	r4, #0
	beq.n	.L_08101d0a
	ldr	r7, [pc, #72]
	movs	r3, #15
	mov	lr, r3
	mov	fp, r7
.L_08101cec:
	ldrh	r2, [r0, #0]
	mov	r7, lr
	lsrs	r3, r2, #12
	ands	r3, r7
	cmp	r3, #15
	bne.n	.L_08101d02
	mov	r3, fp
	ands	r2, r3
	mov	r7, sl
	orrs	r2, r7
	strh	r2, [r0, #0]
.L_08101d02:
	subs	r4, #1
	adds	r0, #2
	cmp	r4, #0
	bne.n	.L_08101cec
.L_08101d0a:
	lsrs	r3, r1, #2
	mov	r0, r8
	mov	r2, r9
	lsls	r2, r3
	ldrb	r3, [r0, #3]
	orrs	r2, r3
	strb	r2, [r0, #3]
	subs	r5, #1
	movs	r3, #64
	add	ip, r3
	adds	r1, #1
	cmp	r5, #0
	bne.n	.L_08101cda
.L_08101d24:
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0fff
	.2byte 0xffff
.L_08101d34:
	.2byte 0xb5e0
	adds	r4, r0, #0
	movs	r7, #12
	ldrsh	r0, [r4, r7]
	sub	sp, #4
	adds	r0, r0, r1
	movs	r7, #14
	ldrsh	r1, [r4, r7]
	ldr	r5, [sp, #24]
	adds	r6, r3, #0
	adds	r1, r1, r2
	ldr	r3, [sp, #20]
	adds	r0, #1
	adds	r1, #1
	adds	r2, r6, #0
	str	r5, [sp, #0]
	bl	.L_08101c7c
	add	sp, #4
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r2, #192
	lsls	r2, r2, #18
	adds	r3, r2, #0
	adds	r3, #220
	ldr	r3, [r3, #0]
	sub	sp, #36
	str	r3, [sp, #24]
	movs	r6, #0
	ldr	r2, [r2, #60]
	str	r6, [sp, #16]
	str	r2, [sp, #20]
	str	r6, [sp, #12]
	ldr	r0, [r3, #52]
	bl	sub_08038268
	movs	r0, #1
	bl	sub_08013560
	ldr	r2, [sp, #24]
	adds	r2, #240
	str	r2, [sp, #8]
	ldr	r0, [r2, #0]
	bl	sub_08038060
	ldr	r5, [pc, #600]
	ldr	r3, [sp, #8]
	adds	r0, r5, #0
	ldr	r1, [r3, #0]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08038080
	ldr	r2, [sp, #8]
	adds	r5, #1
	ldr	r1, [r2, #0]
	adds	r0, r5, #0
	movs	r2, #0
	movs	r3, #16
	bl	sub_08038080
	movs	r5, #6
	movs	r0, #1
	movs	r1, #1
	movs	r2, #11
	movs	r3, #3
	str	r5, [sp, #0]
	bl	.L_08101c7c
	ldr	r3, [sp, #24]
	movs	r1, #0
	ldr	r0, [r3, #52]
	movs	r3, #10
	str	r3, [sp, #0]
	movs	r2, #0
	movs	r3, #28
	str	r5, [sp, #4]
	bl	.L_08101d34
	movs	r1, #9
	movs	r2, #8
	movs	r3, #10
	movs	r0, #0
	str	r5, [sp, #0]
	bl	sub_08038010
	movs	r5, #2
	movs	r1, #12
	movs	r2, #22
	movs	r3, #7
	mov	r8, r0
	movs	r0, #8
	str	r5, [sp, #0]
	bl	sub_08038010
	movs	r1, #9
	str	r0, [sp, #32]
	movs	r2, #22
	movs	r3, #3
	movs	r0, #8
	str	r5, [sp, #0]
	bl	sub_08038010
	str	r0, [sp, #28]
	bl	sub_08038290
	ldr	r7, [pc, #480]
	movs	r5, #0
.L_08101e18:
	lsls	r3, r5, #3
	adds	r0, r5, r7
	mov	r1, r8
	movs	r2, #0
	adds	r5, #1
	bl	sub_08038080
	cmp	r5, #6
	ble.n	.L_08101e18
	ldr	r7, [pc, #464]
	movs	r2, #1
	movs	r3, #0
	mov	fp, r2
	mov	sl, r3
.L_08101e34:
	ldr	r0, [sp, #28]
	bl	sub_08038060
	ldr	r0, [pc, #444]
	ldr	r1, [sp, #28]
	movs	r2, #0
	movs	r3, #0
	adds	r0, r6, r0
	bl	sub_08038078
	ldr	r1, [pc, #436]
	ldr	r0, [sp, #32]
	adds	r1, r6, r1
	bl	sub_080383d8
	mov	r2, fp
	movs	r3, #15
	str	r2, [sp, #0]
	str	r3, [sp, #4]
	ldr	r2, [sp, #12]
	movs	r1, #0
	movs	r3, #6
	mov	r9, r0
	mov	r0, r8
	bl	sub_08101c18
	mov	r3, fp
	str	r3, [sp, #0]
	movs	r3, #14
	str	r3, [sp, #4]
	mov	r0, r8
	movs	r1, #0
	adds	r2, r6, #0
	movs	r3, #6
	bl	sub_08101c18
	str	r6, [sp, #12]
	b.n	.L_08101efa
.L_08101e80:
	ldr	r2, [r7, #12]
	movs	r3, #96
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_08101e9e
	subs	r6, #1
	adds	r0, r6, #0
	movs	r1, #7
	bl	sub_08100e28
	adds	r6, r0, #0
	movs	r0, #111
	bl	sub_081c0010
	b.n	.L_08101f30
.L_08101e9e:
	ldr	r2, [r7, #4]
	movs	r3, #8
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_08101eb6
	movs	r0, #113
	bl	sub_081c0010
	movs	r2, #2
	negs	r2, r2
	str	r2, [sp, #16]
	b.n	.L_08101f30
.L_08101eb6:
	ldr	r2, [r7, #4]
	movs	r3, #6
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_08101ece
	movs	r0, #113
	bl	sub_081c0010
	movs	r3, #1
	negs	r3, r3
	str	r3, [sp, #16]
	b.n	.L_08101f30
.L_08101ece:
	ldr	r3, [r7, #4]
	mov	r2, fp
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08101efa
	bl	sub_08038048
	cmp	r0, #0
	beq.n	.L_08101ef4
	adds	r6, #1
	adds	r0, r6, #0
	movs	r1, #7
	bl	sub_08100e28
	adds	r6, r0, #0
	movs	r0, #112
	bl	sub_081c0010
	b.n	.L_08101f30
.L_08101ef4:
	movs	r0, #111
	bl	sub_081c0010
.L_08101efa:
	mov	r2, r8
	movs	r3, #14
	ldrsh	r1, [r2, r3]
	movs	r0, #12
	adds	r1, r1, r6
	lsls	r1, r1, #3
	adds	r1, #8
	negs	r0, r0
	bl	sub_080f8a44
	movs	r0, #1
	bl	sub_08013560
	ldr	r2, [r7, #12]
	movs	r3, #144
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_08101e80
	adds	r6, #1
	adds	r0, r6, #0
	movs	r1, #7
	bl	sub_08100e28
	adds	r6, r0, #0
	movs	r0, #111
	bl	sub_081c0010
.L_08101f30:
	ldr	r3, [sp, #20]
	movs	r2, #152
	lsls	r2, r2, #5
	adds	r2, #70
	adds	r5, r3, r2
	ldrh	r3, [r5, #0]
	cmp	r3, #99
	beq.n	.L_08101f4a
	adds	r0, r3, #0
	bl	sub_08014274
	movs	r3, #99
	strh	r3, [r5, #0]
.L_08101f4a:
	movs	r5, #192
	lsls	r5, r5, #18
	ldr	r3, [r5, #60]
	movs	r2, #152
	lsls	r2, r2, #5
	adds	r2, #136
	adds	r3, r3, r2
	mov	r2, sl
	strb	r2, [r3, #0]
	ldr	r0, [sp, #32]
	bl	sub_08038060
	mov	r2, r9
	ldr	r3, [r2, #0]
	mov	r2, sl
	strh	r2, [r3, #24]
	strh	r2, [r3, #26]
	strh	r2, [r3, #20]
	mov	r3, sl
	mov	r2, r9
	str	r3, [r2, #0]
	ldr	r3, [sp, #16]
	cmp	r3, #0
	bne.n	.L_08101f7c
	b.n	.L_08101e34
.L_08101f7c:
	ldr	r2, [r5, #60]
	movs	r3, #1
	strb	r3, [r2, #6]
	ldr	r0, [sp, #28]
	bl	sub_08038268
	ldr	r0, [sp, #32]
	bl	sub_08038268
	movs	r0, #1
	bl	sub_08013560
	movs	r1, #1
	ldr	r0, [sp, #28]
	bl	sub_08038018
	mov	r0, r8
	movs	r1, #1
	bl	sub_08038018
	movs	r1, #1
	ldr	r0, [sp, #32]
	bl	sub_08038018
	bl	sub_08038290
	ldr	r2, [sp, #16]
	movs	r3, #2
	negs	r3, r3
	cmp	r2, r3
	bne.n	.L_08101fd8
	ldr	r2, [sp, #8]
	ldr	r0, [r2, #0]
	bl	sub_08038060
	ldr	r3, [sp, #24]
	ldr	r0, [r3, #52]
	bl	sub_08038060
	ldr	r2, [sp, #24]
	ldr	r0, [r2, #16]
	bl	sub_08038060
	ldr	r2, [r5, #60]
	movs	r3, #0
	strb	r3, [r2, #6]
.L_08101fd8:
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #36]
	bl	sub_080145a8
	ldr	r0, [sp, #16]
	add	sp, #36
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x00001168
	.4byte 0x0000116a
	.4byte 0x03001150
	.4byte 0x00002fcb
	.2byte 0x4da9
	.2byte 0x0810
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #112
	str	r0, [sp, #84]
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r3, [r3, #0]
	movs	r0, #192
	str	r3, [sp, #80]
	lsls	r0, r0, #1
	adds	r3, r3, r0
	ldr	r3, [r3, #0]
	ldr	r1, [sp, #80]
	mov	sl, r3
	ldr	r3, [sp, #80]
	movs	r2, #4
	adds	r3, #240
	ldr	r3, [r3, #0]
	ldr	r7, [sp, #80]
	str	r3, [sp, #76]
	ldr	r3, [sp, #84]
	ldr	r1, [r1, #52]
	lsls	r3, r3, #1
	str	r1, [sp, #72]
	str	r2, [sp, #68]
	str	r3, [sp, #56]
	movs	r4, #180
	lsls	r4, r4, #1
	adds	r3, r3, r4
	ldrh	r5, [r7, r3]
	movs	r1, #10
	adds	r0, r5, #0
	bl	sub_0800206c
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	movs	r1, #10
	mov	fp, r0
	adds	r0, r5, #0
	bl	sub_0800205c
	mov	r2, sp
	adds	r2, #104
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	str	r2, [sp, #12]
	movs	r1, #1
	str	r0, [sp, #52]
	negs	r1, r1
	movs	r0, #0
	ldr	r4, [sp, #12]
	str	r0, [sp, #48]
	str	r0, [sp, #44]
	str	r0, [sp, #40]
	str	r0, [sp, #36]
	str	r0, [sp, #32]
	str	r0, [sp, #28]
	str	r0, [sp, #24]
	str	r0, [sp, #20]
	str	r1, [sp, #60]
	mov	r3, sp
	movs	r2, #0
	adds	r3, #111
	mov	ip, r4
.L_08102094:
	strb	r2, [r3, #0]
	subs	r3, #1
	cmp	r3, ip
	bge.n	.L_08102094
	ldr	r7, [sp, #84]
	cmp	r7, #0
	bne.n	.L_08102116
	ldr	r0, [sp, #52]
	ldr	r3, [sp, #80]
	movs	r4, #139
	str	r0, [sp, #48]
	lsls	r4, r4, #1
	adds	r4, #255
	adds	r2, r3, r4
	ldrb	r3, [r2, #0]
	movs	r1, #0
	mov	r9, r1
	cmp	r7, r3
	bge.n	.L_081020dc
	ldr	r1, [sp, #12]
	adds	r0, r2, #0
	mov	r2, sl
	movs	r4, #4
	adds	r2, #160
.L_081020c4:
	ldrb	r3, [r2, #0]
	adds	r2, #1
	lsls	r3, r3, #24
	cmp	r3, #0
	bne.n	.L_081020d0
	strb	r4, [r1, #0]
.L_081020d0:
	ldrb	r3, [r0, #0]
	movs	r7, #1
	add	r9, r7
	adds	r1, #1
	cmp	r9, r3
	blt.n	.L_081020c4
.L_081020dc:
	ldr	r1, [sp, #80]
	movs	r2, #139
	lsls	r2, r2, #1
	adds	r2, #255
	adds	r3, r1, r2
	ldrb	r3, [r3, #0]
	movs	r0, #0
	mov	r9, r0
	cmp	r9, r3
	bge.n	.L_08102180
	adds	r5, r1, r2
	adds	r6, r3, #0
.L_081020f4:
	ldr	r4, [sp, #12]
	mov	r7, fp
	ldrsb	r3, [r4, r7]
	cmp	r3, #4
	bne.n	.L_0810210c
	movs	r0, #1
	add	fp, r0
	mov	r0, fp
	ldrb	r1, [r5, #0]
	bl	sub_08100e28
	mov	fp, r0
.L_0810210c:
	movs	r1, #1
	add	r9, r1
	cmp	r9, r6
	blt.n	.L_081020f4
	b.n	.L_08102180
.L_08102116:
	ldr	r2, [sp, #80]
	add	r5, sp, #88
	movs	r1, #28
	ldrsb	r1, [r2, r1]
	adds	r0, r5, #0
	bl	sub_08104928
	movs	r7, #139
	ldr	r4, [sp, #80]
	lsls	r7, r7, #1
	adds	r7, #255
	movs	r3, #0
	adds	r2, r4, r7
	mov	r9, r3
	ldrb	r3, [r2, #0]
	cmp	r9, r3
	bge.n	.L_08102180
	ldr	r0, [sp, #12]
	mov	r1, sl
	adds	r4, r2, #0
	adds	r1, #160
	adds	r2, r0, #0
	movs	r6, #7
.L_08102144:
	ldr	r7, [sp, #80]
	movs	r3, #28
	ldrsb	r3, [r7, r3]
	cmp	r9, r3
	bne.n	.L_08102152
	strb	r6, [r2, #0]
	b.n	.L_08102170
.L_08102152:
	mov	r7, r9
	ldrb	r3, [r5, r7]
	cmp	r3, #0
	beq.n	.L_08102160
	movs	r3, #0
	strb	r3, [r2, #0]
	b.n	.L_08102170
.L_08102160:
	movs	r3, #3
	strb	r3, [r2, #0]
	movs	r3, #0
	ldrsb	r3, [r1, r3]
	cmp	r3, #0
	bne.n	.L_08102170
	movs	r3, #7
	strb	r3, [r0, #0]
.L_08102170:
	movs	r3, #1
	add	r9, r3
	ldrb	r3, [r4, #0]
	adds	r0, #1
	adds	r1, #1
	adds	r2, #1
	cmp	r9, r3
	blt.n	.L_08102144
.L_08102180:
	ldr	r4, [sp, #80]
	movs	r3, #1
	ldr	r2, [r4, #20]
	strb	r3, [r2, #5]
.L_08102188:
	ldr	r7, [sp, #68]
	cmp	r7, #0
	bne.n	.L_08102190
	b.n	.L_081025c0
.L_08102190:
	movs	r0, #1
	negs	r0, r0
	str	r0, [sp, #60]
	ldr	r1, [sp, #12]
	mov	r3, fp
	ldrb	r2, [r1, r3]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_081021aa
	ldr	r4, [sp, #52]
	str	r4, [sp, #60]
	b.n	.L_081021ca
.L_081021aa:
	mov	r3, fp
	cmp	r3, #0
	bge.n	.L_081021b2
	adds	r3, #3
.L_081021b2:
	asrs	r3, r3, #2
	lsls	r3, r3, #2
	mov	r7, fp
	subs	r3, r7, r3
	lsls	r0, r3, #3
	subs	r0, r0, r3
	lsls	r0, r0, #3
	subs	r0, #8
	movs	r1, #52
	bl	sub_080f8a44
	b.n	.L_081021ec
.L_081021ca:
	mov	r3, fp
	cmp	r3, #0
	bge.n	.L_081021d2
	adds	r3, #3
.L_081021d2:
	asrs	r3, r3, #2
	mov	r0, fp
	lsls	r3, r3, #2
	subs	r3, r0, r3
	ldr	r2, [sp, #52]
	lsls	r0, r3, #3
	subs	r0, r0, r3
	lsls	r0, r0, #3
	lsls	r1, r2, #3
	subs	r0, #8
	adds	r1, #60
	bl	sub_080f8a44
.L_081021ec:
	ldr	r4, [sp, #68]
	lsrs	r3, r4, #2
	cmp	r3, #0
	bne.n	.L_081021f6
	b.n	.L_0810230a
.L_081021f6:
	mov	r3, fp
	cmp	r3, #0
	bge.n	.L_081021fe
	adds	r3, #3
.L_081021fe:
	asrs	r3, r3, #2
	lsls	r7, r3, #2
	adds	r0, r7, #0
	bl	sub_08104ef8
	mov	r0, sl
	adds	r1, r7, #0
	bl	sub_08101ac8
	ldr	r0, [sp, #84]
	cmp	r0, #1
	bne.n	.L_081022f8
	ldr	r1, [sp, #80]
	movs	r2, #180
	lsls	r2, r2, #1
	adds	r3, r1, r2
	ldrh	r5, [r3, #0]
	movs	r1, #10
	adds	r0, r5, #0
	bl	sub_0800206c
	lsls	r0, r0, #16
	lsrs	r6, r0, #16
	movs	r1, #10
	adds	r0, r5, #0
	bl	sub_0800205c
	lsls	r0, r0, #16
	lsrs	r2, r0, #16
	adds	r3, r6, #0
	cmp	r6, #0
	bge.n	.L_08102240
	adds	r3, r6, #3
.L_08102240:
	asrs	r3, r3, #2
	lsls	r3, r3, #2
	cmp	r3, r7
	bne.n	.L_0810227c
	adds	r3, r6, #0
	mov	r4, sl
	adds	r3, #160
	ldrsb	r5, [r4, r3]
	ldr	r0, [sp, #72]
	adds	r1, r6, #0
	movs	r3, #1
	bl	sub_08101c40
	subs	r3, r6, r7
	lsls	r1, r3, #3
	subs	r1, r1, r3
	adds	r1, #1
	movs	r3, #6
	ldr	r0, [sp, #72]
	movs	r2, #2
	str	r3, [sp, #4]
	str	r5, [sp, #0]
	bl	.L_08101d34
	ldr	r0, [sp, #80]
	movs	r1, #142
	lsls	r1, r1, #2
	adds	r2, r0, r1
	movs	r3, #54
	b.n	.L_08102286
.L_0810227c:
	ldr	r3, [sp, #80]
	movs	r4, #142
	lsls	r4, r4, #2
	adds	r2, r3, r4
	movs	r3, #200
.L_08102286:
	strh	r3, [r2, #0]
	ldr	r5, [sp, #80]
	movs	r0, #0
	mov	r9, r0
	movs	r6, #8
	adds	r5, #248
.L_08102292:
	ldmia	r5!, {r3}
	cmp	r3, #0
	beq.n	.L_081022ec
	ldr	r4, [sp, #80]
	mov	r1, r9
	movs	r3, #28
	ldrsb	r3, [r4, r3]
	adds	r2, r7, r1
	cmp	r2, r3
	bne.n	.L_081022be
	movs	r0, #182
	lsls	r0, r0, #1
	adds	r3, r4, r0
	ldrh	r2, [r3, #0]
	ldr	r3, [pc, #32]
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_081022ba
	ldr	r0, [pc, #28]
	b.n	.L_081022e2
.L_081022ba:
	ldr	r0, [pc, #28]
	b.n	.L_081022e2
.L_081022be:
	ldr	r1, [sp, #12]
	movs	r3, #2
	ldrb	r2, [r1, r2]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_081022e0
	ldr	r0, [pc, #16]
	b.n	.L_081022e2
	movs	r0, r0
	.4byte 0x00008000
	.4byte 0x000010e1
	.4byte 0x000010e0
	.2byte 0x10df
	.2byte 0x0000
.L_081022e0:
	ldr	r0, [pc, #440]
.L_081022e2:
	ldr	r1, [sp, #72]
	adds	r2, r6, #0
	movs	r3, #8
	bl	sub_08038080
.L_081022ec:
	movs	r2, #1
	add	r9, r2
	mov	r3, r9
	adds	r6, #56
	cmp	r3, #3
	ble.n	.L_08102292
.L_081022f8:
	ldr	r4, [sp, #80]
	movs	r7, #139
	lsls	r7, r7, #1
	adds	r7, #255
	adds	r3, r4, r7
	ldrb	r1, [r3, #0]
	mov	r0, fp
	bl	sub_08104d14
.L_0810230a:
	ldr	r0, [sp, #12]
	mov	r1, fp
	ldrb	r2, [r0, r1]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_08102322
	ldr	r0, [sp, #72]
	ldr	r2, [sp, #52]
	movs	r3, #1
	bl	sub_08101c40
.L_08102322:
	ldr	r2, [sp, #60]
	movs	r5, #1
	negs	r5, r5
	cmp	r2, r5
	beq.n	.L_0810233e
	mov	r4, fp
	lsls	r3, r4, #2
	add	r3, fp
	lsls	r3, r3, #1
	adds	r3, r3, r2
	lsls	r3, r3, #1
	mov	r7, sl
	ldrh	r3, [r7, r3]
	str	r3, [sp, #44]
.L_0810233e:
	ldr	r0, [sp, #44]
	movs	r3, #240
	lsls	r3, r3, #4
	ands	r3, r0
	lsrs	r3, r3, #8
	str	r3, [sp, #32]
	movs	r1, #31
	movs	r3, #224
	ands	r3, r0
	ands	r0, r1
	lsrs	r3, r3, #5
	str	r0, [sp, #24]
	ldr	r0, [sp, #44]
	str	r3, [sp, #28]
	bl	sub_08101a04
	ldr	r2, [sp, #60]
	str	r0, [sp, #20]
	cmp	r2, r5
	bne.n	.L_08102372
	ldr	r0, [sp, #84]
	movs	r1, #0
	movs	r2, #200
	bl	sub_08105300
	b.n	.L_081023de
.L_08102372:
	ldr	r3, [sp, #20]
	movs	r6, #128
	movs	r5, #1
	lsls	r6, r6, #7
	cmp	r3, #0
	beq.n	.L_08102390
	cmp	r3, #1
	bne.n	.L_0810238a
	movs	r6, #128
	movs	r5, #2
	lsls	r6, r6, #7
	b.n	.L_08102390
.L_0810238a:
	movs	r6, #128
	movs	r5, #1
	lsls	r6, r6, #8
.L_08102390:
	mov	r2, fp
	cmp	r2, #0
	bge.n	.L_08102398
	adds	r2, #3
.L_08102398:
	asrs	r2, r2, #2
	lsls	r2, r2, #2
	mov	r4, fp
	subs	r2, r4, r2
	lsls	r3, r2, #3
	subs	r3, r3, r2
	ldr	r7, [sp, #84]
	lsls	r3, r3, #3
	adds	r1, r3, #0
	adds	r1, #48
	movs	r2, #62
	cmp	r7, #0
	beq.n	.L_081023b4
	movs	r2, #54
.L_081023b4:
	ldr	r0, [sp, #84]
	bl	sub_08105300
	adds	r1, r6, #0
	ldr	r0, [sp, #84]
	bl	sub_0810532c
	ldr	r0, [sp, #68]
	movs	r3, #2
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L_081023d4
	ldr	r0, [sp, #84]
	movs	r1, #0
	bl	sub_08105350
.L_081023d4:
	ldr	r0, [sp, #84]
	ldr	r1, [sp, #28]
	adds	r2, r5, #0
	bl	sub_081052ac
.L_081023de:
	ldr	r1, [sp, #80]
	mov	r2, fp
	movs	r4, #129
	lsls	r4, r4, #2
	lsls	r3, r2, #1
	adds	r3, r3, r4
	ldrh	r0, [r1, r3]
	ldr	r6, [r1, #16]
	bl	sub_08016ca4
	movs	r7, #42
	adds	r5, r0, #0
	adds	r0, r6, #0
	bl	sub_08038260
	adds	r7, #255
	adds	r0, r5, #0
	adds	r1, r6, #0
	movs	r2, #0
	movs	r3, #0
	bl	sub_08038090
	adds	r3, r5, r7
	ldrb	r0, [r3, #0]
	ldr	r3, [pc, #144]
	adds	r1, r6, #0
	adds	r0, r0, r3
	movs	r2, #0
	movs	r3, #8
	bl	sub_08038080
	ldr	r0, [pc, #132]
	adds	r1, r6, #0
	movs	r2, #48
	movs	r3, #0
	bl	sub_08038090
	movs	r3, #0
	ldrb	r0, [r5, #15]
	movs	r1, #2
	str	r3, [sp, #0]
	adds	r2, r6, #0
	movs	r3, #72
	bl	sub_080380b0
	ldr	r0, [sp, #84]
	cmp	r0, #0
	bne.n	.L_0810244a
	ldr	r0, [pc, #104]
	adds	r1, r6, #0
	movs	r2, #0
	movs	r3, #16
	bl	sub_08038080
.L_0810244a:
	ldr	r0, [sp, #76]
	bl	sub_08038260
	ldr	r1, [sp, #84]
	cmp	r1, #0
	bne.n	.L_081024e0
	ldr	r2, [sp, #60]
	movs	r3, #1
	negs	r3, r3
	cmp	r2, r3
	beq.n	.L_081024e0
	ldr	r4, [sp, #40]
	cmp	r4, #0
	beq.n	.L_081024b8
	ldr	r7, [sp, #36]
	cmp	r7, #0
	bne.n	.L_0810247a
	ldr	r0, [pc, #60]
	ldr	r1, [sp, #76]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08038080
	b.n	.L_08102486
.L_0810247a:
	ldr	r0, [pc, #52]
	ldr	r1, [sp, #76]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08038080
.L_08102486:
	ldr	r0, [sp, #20]
	cmp	r0, #2
	bne.n	.L_081024e0
	ldr	r0, [pc, #36]
	ldr	r1, [sp, #76]
	movs	r2, #0
	movs	r3, #16
	bl	sub_08038080
	b.n	.L_081024e0
	movs	r0, r0
	.4byte 0x000010e2
	.4byte 0x00000b63
	.4byte 0x081059d4
	.4byte 0x000010da
	.4byte 0x000010c9
	.4byte 0x000010ca
	.2byte 0x10cf
	.2byte 0x0000
.L_081024b8:
	ldr	r5, [pc, #904]
	ldr	r1, [sp, #76]
	adds	r0, r5, #0
	movs	r2, #0
	movs	r3, #0
	bl	sub_08038080
	ldr	r1, [sp, #20]
	adds	r0, r5, #3
	cmp	r1, #0
	beq.n	.L_081024d6
	adds	r0, r5, #2
	cmp	r1, #1
	beq.n	.L_081024d6
	ldr	r0, [pc, #880]
.L_081024d6:
	ldr	r1, [sp, #76]
	movs	r2, #0
	movs	r3, #16
	bl	sub_08038080
.L_081024e0:
	movs	r0, #1
	bl	sub_08013560
	ldr	r2, [sp, #84]
	cmp	r2, #1
	bne.n	.L_08102572
	ldr	r4, [sp, #80]
	movs	r7, #128
	lsls	r7, r7, #2
	adds	r7, #22
	adds	r3, r4, r7
	ldrb	r0, [r3, #0]
	movs	r1, #1
	bl	sub_08038120
	ldr	r4, [pc, #844]
	ldr	r1, [sp, #76]
	adds	r0, r4, #0
	movs	r2, #0
	movs	r3, #0
	str	r4, [sp, #8]
	bl	sub_08038080
	ldr	r0, [sp, #80]
	movs	r1, #182
	lsls	r1, r1, #1
	adds	r6, r0, r1
	ldrh	r2, [r6, #0]
	movs	r5, #224
	adds	r3, r5, #0
	ands	r3, r2
	lsrs	r3, r3, #5
	lsls	r0, r3, #2
	adds	r0, r0, r3
	movs	r3, #31
	ands	r3, r2
	lsls	r0, r0, #2
	movs	r2, #150
	adds	r0, r0, r3
	lsls	r2, r2, #1
	adds	r0, r0, r2
	movs	r1, #4
	bl	sub_08038120
	ldrh	r3, [r6, #0]
	ldr	r0, [sp, #76]
	ands	r5, r3
	movs	r3, #160
	lsls	r3, r3, #7
	adds	r3, #1
	lsrs	r5, r5, #5
	adds	r5, r5, r3
	adds	r1, r5, #0
	movs	r3, #0
	movs	r2, #6
	str	r3, [sp, #0]
	bl	sub_08038270
	ldr	r4, [sp, #8]
	ldr	r1, [sp, #76]
	adds	r0, r4, #1
	movs	r2, #56
	movs	r3, #0
	bl	sub_08038080
	ldr	r4, [sp, #8]
	ldr	r1, [sp, #76]
	adds	r4, #2
	adds	r0, r4, #0
	movs	r2, #0
	movs	r3, #8
	bl	sub_08038080
.L_08102572:
	movs	r0, #1
	bl	sub_08013560
	ldr	r0, [sp, #72]
	bl	sub_08038268
	ldr	r4, [sp, #60]
	movs	r7, #1
	negs	r7, r7
	cmp	r4, r7
	beq.n	.L_081025b2
	movs	r3, #104
	str	r3, [sp, #0]
	ldr	r0, [sp, #72]
	movs	r1, #0
	movs	r2, #96
	movs	r3, #224
	bl	sub_08038068
	ldr	r1, [sp, #28]
	ldr	r2, [sp, #24]
	lsls	r0, r1, #2
	adds	r0, r0, r1
	ldr	r3, [pc, #684]
	lsls	r0, r0, #2
	adds	r0, r0, r2
	adds	r0, r0, r3
	ldr	r1, [sp, #72]
	movs	r2, #0
	movs	r3, #96
	bl	sub_08038080
.L_081025b2:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #60]
	movs	r3, #1
	strb	r3, [r2, #3]
	movs	r3, #0
	str	r3, [sp, #68]
.L_081025c0:
	movs	r0, #1
	bl	sub_08013560
	ldr	r1, [pc, #652]
	movs	r2, #128
	ldr	r3, [r1, #0]
	lsls	r2, r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_081025dc
	ldr	r3, [r1, #28]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_081025ec
.L_081025dc:
	ldr	r4, [sp, #40]
	cmp	r4, #0
	beq.n	.L_081025e6
	movs	r7, #1
	str	r7, [sp, #68]
.L_081025e6:
	movs	r0, #0
	str	r0, [sp, #40]
	str	r0, [sp, #36]
.L_081025ec:
	ldr	r2, [r1, #12]
	str	r2, [sp, #16]
	ldr	r4, [r1, #4]
	movs	r1, #132
	lsls	r1, r1, #6
	adds	r1, #44
	add	r1, sl
	ldr	r3, [r1, #0]
	cmp	r3, #0
	bne.n	.L_08102602
	b.n	.L_08102b06
.L_08102602:
	movs	r4, #0
	movs	r3, #132
	str	r4, [sp, #16]
	lsls	r3, r3, #6
	adds	r3, #40
	add	r3, sl
	ldr	r2, [r3, #0]
	adds	r2, #1
	str	r2, [r3, #0]
	ldr	r3, [r1, #0]
	subs	r3, #1
	cmp	r3, #27
	bls.n	.L_0810261e
	b.n	.L_08102b06
.L_0810261e:
	ldr	r2, [pc, #568]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	movs	r0, r0
	.4byte 0x08102788
	.4byte 0x081027f4
	.4byte 0x0810281a
	.4byte 0x081027f4
	.4byte 0x08102b06
	.4byte 0x08102860
	.4byte 0x08102860
	.4byte 0x081027f4
	.4byte 0x081027f4
	.4byte 0x08102b06
	.4byte 0x08102b06
	.4byte 0x08102b06
	.4byte 0x081028ce
	.4byte 0x081028f4
	.4byte 0x08102918
	.4byte 0x081028f4
	.4byte 0x081028f4
	.4byte 0x08102a98
	.4byte 0x08102b06
	.4byte 0x08102a98
	.4byte 0x08102abe
	.4byte 0x081027f4
	.4byte 0x08102b06
	.4byte 0x08102ae2
	.4byte 0x08102b06
	.4byte 0x08102b06
	.4byte 0x081026c4
	.2byte 0x2698
	.2byte 0x0810
	ldr	r1, [pc, #440]
	movs	r3, #1
	ldr	r2, [r1, #4]
	ands	r2, r3
	cmp	r2, #0
	bne.n	.L_081026be
	adds	r6, r1, #0
	movs	r5, #1
.L_081026a8:
	movs	r0, #150
	movs	r1, #26
	bl	sub_080f8a44
	movs	r0, #1
	bl	sub_08013560
	ldr	r3, [r6, #4]
	ands	r3, r5
	cmp	r3, #0
	beq.n	.L_081026a8
.L_081026be:
	movs	r4, #2
	str	r4, [sp, #16]
	b.n	.L_08102b06
	movs	r3, #132
	lsls	r3, r3, #6
	adds	r3, #40
	add	r3, sl
	ldr	r3, [r3, #0]
	cmp	r3, #60
	beq.n	.L_081026d4
	b.n	.L_08102b06
.L_081026d4:
	bl	sub_08100e5c
	movs	r2, #9
	movs	r3, #1
	adds	r0, #12
	movs	r1, #9
	bl	sub_08038038
	ldr	r2, [pc, #372]
	movs	r3, #139
	lsls	r3, r3, #2
	adds	r2, r2, r3
	adds	r5, r0, #0
	movs	r3, #1
	strb	r3, [r2, #0]
	b.n	.L_081026fa
.L_081026f4:
	movs	r0, #1
	bl	sub_08013560
.L_081026fa:
	bl	sub_08038048
	cmp	r0, #0
	beq.n	.L_081026f4
	movs	r1, #1
	adds	r0, r5, #0
	bl	sub_08038018
	mov	r0, sl
	bl	sub_08101c0c
	movs	r0, #1
	bl	sub_08013560
	bl	sub_08100e5c
	movs	r2, #9
	movs	r3, #1
	adds	r0, #13
	movs	r1, #9
	bl	sub_08038038
	ldr	r2, [pc, #308]
	movs	r4, #139
	lsls	r4, r4, #2
	adds	r2, r2, r4
	movs	r3, #1
	adds	r5, r0, #0
	strb	r3, [r2, #0]
	b.n	.L_0810273c
.L_08102736:
	movs	r0, #1
	bl	sub_08013560
.L_0810273c:
	bl	sub_08038048
	cmp	r0, #0
	beq.n	.L_08102736
	movs	r1, #1
	adds	r0, r5, #0
	bl	sub_08038018
	mov	r0, sl
	bl	sub_08101c0c
	movs	r2, #132
	lsls	r2, r2, #6
	adds	r2, #40
	movs	r3, #0
	add	r2, sl
	str	r3, [r2, #0]
	bl	sub_081180f8
	bl	sub_081180f8
	bl	sub_081180f8
	movs	r1, #0
	movs	r2, #7
	movs	r0, #4
	bl	sub_080ad160
	movs	r1, #0
	movs	r2, #7
	movs	r0, #4
	bl	sub_080ad170
	movs	r0, #4
	bl	sub_080ad008
	movs	r7, #2
	b.n	.L_08102b02
	movs	r3, #132
	lsls	r3, r3, #6
	adds	r3, #40
	add	r3, sl
	ldr	r3, [r3, #0]
	cmp	r3, #60
	beq.n	.L_08102798
	b.n	.L_08102b06
.L_08102798:
	str	r4, [sp, #8]
	bl	sub_08100e5c
	movs	r1, #9
	movs	r2, #9
	movs	r3, #1
	bl	sub_08038038
	ldr	r2, [pc, #176]
	adds	r5, r0, #0
	movs	r0, #139
	lsls	r0, r0, #2
	adds	r2, r2, r0
	movs	r3, #1
	strb	r3, [r2, #0]
	b.n	.L_081027c0
.L_081027b8:
	movs	r0, #1
	str	r4, [sp, #8]
	bl	sub_08013560
.L_081027c0:
	ldr	r4, [sp, #8]
	str	r4, [sp, #8]
	bl	sub_08038048
	ldr	r4, [sp, #8]
	cmp	r0, #0
	beq.n	.L_081027b8
	adds	r0, r5, #0
	movs	r1, #1
	bl	sub_08038018
	mov	r0, sl
	bl	sub_08101c0c
	movs	r2, #132
	lsls	r2, r2, #6
	adds	r2, #40
	add	r2, sl
	movs	r3, #0
	str	r3, [r2, #0]
	movs	r2, #132
	lsls	r2, r2, #6
	adds	r2, #44
	add	r2, sl
	movs	r3, #2
	b.n	.L_08102a92
	movs	r2, #132
	lsls	r2, r2, #6
	adds	r2, #40
	add	r2, sl
	ldr	r3, [r2, #0]
	cmp	r3, #90
	beq.n	.L_08102804
	b.n	.L_08102b06
.L_08102804:
	movs	r3, #0
	movs	r1, #1
	str	r1, [sp, #16]
	str	r3, [r2, #0]
	movs	r2, #132
	lsls	r2, r2, #6
	adds	r2, #44
	add	r2, sl
	ldr	r3, [r2, #0]
	movs	r4, #1
	b.n	.L_08102912
	movs	r2, #132
	lsls	r2, r2, #6
	adds	r2, #40
	add	r2, sl
	ldr	r3, [r2, #0]
	cmp	r3, #90
	beq.n	.L_0810282a
	b.n	.L_08102b06
.L_0810282a:
	movs	r3, #16
	str	r3, [sp, #16]
	movs	r3, #0
	str	r3, [r2, #0]
	movs	r2, #132
	lsls	r2, r2, #6
	adds	r2, #44
	add	r2, sl
	movs	r3, #4
	movs	r4, #16
	str	r3, [r2, #0]
	b.n	.L_08102b06
	movs	r0, r0
	.4byte 0x000010cb
	.4byte 0x000010cf
	.4byte 0x000010e3
	.4byte 0x000009b1
	.4byte 0x03001150
	.4byte 0x08102628
	.2byte 0x0240
	.2byte 0x0200
	movs	r3, #132
	lsls	r3, r3, #6
	adds	r3, #40
	add	r3, sl
	ldr	r3, [r3, #0]
	cmp	r3, #60
	beq.n	.L_08102870
	b.n	.L_08102b06
.L_08102870:
	str	r4, [sp, #8]
	bl	sub_08100e5c
	movs	r1, #9
	movs	r2, #9
	movs	r3, #1
	adds	r0, #1
	bl	sub_08038038
	ldr	r2, [pc, #836]
	movs	r7, #139
	lsls	r7, r7, #2
	adds	r2, r2, r7
	movs	r3, #1
	adds	r5, r0, #0
	strb	r3, [r2, #0]
	b.n	.L_0810289a
.L_08102892:
	movs	r0, #1
	str	r4, [sp, #8]
	bl	sub_08013560
.L_0810289a:
	ldr	r4, [sp, #8]
	str	r4, [sp, #8]
	bl	sub_08038048
	ldr	r4, [sp, #8]
	cmp	r0, #0
	beq.n	.L_08102892
	adds	r0, r5, #0
	movs	r1, #1
	bl	sub_08038018
	mov	r0, sl
	bl	sub_08101c0c
	movs	r2, #132
	lsls	r2, r2, #6
	adds	r2, #40
	add	r2, sl
	movs	r3, #0
	str	r3, [r2, #0]
	movs	r2, #132
	lsls	r2, r2, #6
	adds	r2, #44
	add	r2, sl
	movs	r3, #8
	b.n	.L_08102a92
	movs	r2, #132
	lsls	r2, r2, #6
	adds	r2, #40
	add	r2, sl
	ldr	r3, [r2, #0]
	cmp	r3, #40
	beq.n	.L_081028de
	b.n	.L_08102b06
.L_081028de:
	movs	r3, #0
	movs	r0, #2
	str	r0, [sp, #16]
	str	r3, [r2, #0]
	movs	r2, #132
	lsls	r2, r2, #6
	adds	r2, #44
	add	r2, sl
	ldr	r3, [r2, #0]
	movs	r4, #2
	b.n	.L_08102912
	movs	r2, #132
	lsls	r2, r2, #6
	adds	r2, #40
	add	r2, sl
	ldr	r3, [r2, #0]
	cmp	r3, #40
	beq.n	.L_08102904
	b.n	.L_08102b06
.L_08102904:
	movs	r3, #0
	str	r3, [r2, #0]
	movs	r2, #132
	lsls	r2, r2, #6
	adds	r2, #44
	add	r2, sl
	ldr	r3, [r2, #0]
.L_08102912:
	adds	r3, #1
	str	r3, [r2, #0]
	b.n	.L_08102b06
	movs	r3, #132
	lsls	r3, r3, #6
	adds	r3, #40
	add	r3, sl
	ldr	r3, [r3, #0]
	cmp	r3, #60
	beq.n	.L_08102928
	b.n	.L_08102b06
.L_08102928:
	ldr	r2, [pc, #668]
	movs	r1, #139
	lsls	r1, r1, #2
	adds	r2, r2, r1
	movs	r3, #1
	strb	r3, [r2, #0]
	str	r4, [sp, #8]
	bl	sub_08100e5c
	movs	r2, #9
	movs	r3, #1
	movs	r1, #9
	adds	r0, #4
	bl	sub_08038038
	movs	r1, #146
	adds	r5, r0, #0
	movs	r0, #2
	bl	sub_080f8ab4
	b.n	.L_0810295a
.L_08102952:
	movs	r0, #1
	str	r4, [sp, #8]
	bl	sub_08013560
.L_0810295a:
	ldr	r4, [sp, #8]
	str	r4, [sp, #8]
	bl	sub_08038048
	ldr	r4, [sp, #8]
	cmp	r0, #0
	beq.n	.L_08102952
	ldr	r1, [pc, #608]
	movs	r3, #1
	ldr	r2, [r1, #4]
	ands	r2, r3
	cmp	r2, #0
	bne.n	.L_08102992
	adds	r7, r1, #0
	movs	r6, #1
.L_08102978:
	movs	r0, #2
	movs	r1, #146
	str	r4, [sp, #8]
	bl	sub_080f8a44
	movs	r0, #1
	bl	sub_08013560
	ldr	r3, [r7, #4]
	ldr	r4, [sp, #8]
	ands	r3, r6
	cmp	r3, #0
	beq.n	.L_08102978
.L_08102992:
	movs	r1, #1
	adds	r0, r5, #0
	str	r4, [sp, #8]
	bl	sub_08038018
	mov	r0, sl
	bl	sub_08101c0c
	movs	r0, #1
	bl	sub_08013560
	bl	sub_08100e5c
	movs	r1, #9
	movs	r2, #9
	movs	r3, #1
	adds	r0, #5
	bl	sub_08038038
	adds	r5, r0, #0
	b.n	.L_081029c4
.L_081029bc:
	movs	r0, #1
	str	r4, [sp, #8]
	bl	sub_08013560
.L_081029c4:
	ldr	r4, [sp, #8]
	str	r4, [sp, #8]
	bl	sub_08038048
	ldr	r4, [sp, #8]
	cmp	r0, #0
	beq.n	.L_081029bc
	ldr	r1, [pc, #504]
	movs	r3, #1
	ldr	r2, [r1, #4]
	ands	r2, r3
	cmp	r2, #0
	bne.n	.L_081029fc
	adds	r7, r1, #0
	movs	r6, #1
.L_081029e2:
	movs	r0, #2
	movs	r1, #146
	str	r4, [sp, #8]
	bl	sub_080f8a44
	movs	r0, #1
	bl	sub_08013560
	ldr	r3, [r7, #4]
	ldr	r4, [sp, #8]
	ands	r3, r6
	cmp	r3, #0
	beq.n	.L_081029e2
.L_081029fc:
	movs	r1, #1
	adds	r0, r5, #0
	str	r4, [sp, #8]
	bl	sub_08038018
	mov	r0, sl
	bl	sub_08101c0c
	movs	r0, #1
	bl	sub_08013560
	bl	sub_08100e5c
	movs	r1, #9
	movs	r2, #9
	movs	r3, #1
	adds	r0, #6
	bl	sub_08038038
	adds	r5, r0, #0
	b.n	.L_08102a2e
.L_08102a26:
	movs	r0, #1
	str	r4, [sp, #8]
	bl	sub_08013560
.L_08102a2e:
	ldr	r4, [sp, #8]
	str	r4, [sp, #8]
	bl	sub_08038048
	ldr	r4, [sp, #8]
	cmp	r0, #0
	beq.n	.L_08102a26
	ldr	r1, [pc, #396]
	movs	r3, #1
	ldr	r2, [r1, #4]
	ands	r2, r3
	cmp	r2, #0
	bne.n	.L_08102a66
	adds	r7, r1, #0
	movs	r6, #1
.L_08102a4c:
	movs	r0, #2
	movs	r1, #146
	str	r4, [sp, #8]
	bl	sub_080f8a44
	movs	r0, #1
	bl	sub_08013560
	ldr	r3, [r7, #4]
	ldr	r4, [sp, #8]
	ands	r3, r6
	cmp	r3, #0
	beq.n	.L_08102a4c
.L_08102a66:
	movs	r1, #1
	adds	r0, r5, #0
	str	r4, [sp, #8]
	bl	sub_08038018
	mov	r0, sl
	bl	sub_08101c0c
	movs	r0, #1
	bl	sub_08013560
	movs	r2, #132
	lsls	r2, r2, #6
	adds	r2, #40
	add	r2, sl
	movs	r3, #0
	str	r3, [r2, #0]
	movs	r2, #132
	lsls	r2, r2, #6
	adds	r2, #44
	add	r2, sl
	movs	r3, #16
.L_08102a92:
	str	r3, [r2, #0]
	ldr	r4, [sp, #8]
	b.n	.L_08102b06
	movs	r2, #132
	lsls	r2, r2, #6
	adds	r2, #40
	add	r2, sl
	ldr	r3, [r2, #0]
	cmp	r3, #90
	bne.n	.L_08102b06
	movs	r3, #1
	str	r3, [sp, #16]
	movs	r3, #0
	str	r3, [r2, #0]
	movs	r2, #132
	lsls	r2, r2, #6
	adds	r2, #44
	add	r2, sl
	movs	r3, #21
	movs	r4, #1
	str	r3, [r2, #0]
	b.n	.L_08102b06
	movs	r2, #132
	lsls	r2, r2, #6
	adds	r2, #40
	add	r2, sl
	ldr	r3, [r2, #0]
	cmp	r3, #90
	bne.n	.L_08102b06
	movs	r3, #0
	movs	r4, #32
	str	r4, [sp, #16]
	str	r3, [r2, #0]
	movs	r2, #132
	lsls	r2, r2, #6
	adds	r2, #44
	add	r2, sl
	movs	r3, #22
	str	r3, [r2, #0]
	b.n	.L_08102b06
	movs	r2, #132
	lsls	r2, r2, #6
	adds	r2, #40
	add	r2, sl
	ldr	r3, [r2, #0]
	cmp	r3, #60
	bne.n	.L_08102b06
	movs	r3, #0
	str	r3, [r2, #0]
	movs	r2, #132
	lsls	r2, r2, #6
	adds	r2, #44
	add	r2, sl
	movs	r3, #25
	movs	r7, #2
	str	r3, [r2, #0]
.L_08102b02:
	str	r7, [sp, #16]
	movs	r4, #2
.L_08102b06:
	ldr	r0, [sp, #84]
	cmp	r0, #0
	bne.n	.L_08102bd0
	movs	r3, #128
	lsls	r3, r3, #1
	ands	r3, r4
	cmp	r3, #0
	beq.n	.L_08102bac
	ldr	r1, [sp, #60]
	movs	r2, #1
	negs	r2, r2
	cmp	r1, r2
	bne.n	.L_08102b2a
	movs	r0, #114
	bl	sub_081c0010
	bl	.L_08102188
.L_08102b2a:
	movs	r3, #1
	str	r3, [sp, #40]
	ldr	r7, [sp, #84]
	ldr	r3, [pc, #152]
	str	r7, [r3, #28]
	ldr	r0, [sp, #20]
	cmp	r0, #0
	beq.n	.L_08102b4c
	cmp	r0, #1
	beq.n	.L_08102b6a
	movs	r0, #114
	bl	sub_081c0010
	movs	r1, #1
	str	r1, [sp, #68]
	bl	.L_08102188
.L_08102b4c:
	movs	r0, #139
	str	r4, [sp, #8]
	bl	sub_081c0010
	ldr	r1, [sp, #28]
	ldr	r2, [sp, #24]
	ldr	r0, [sp, #32]
	bl	sub_080ad158
	ldr	r0, [sp, #32]
	ldr	r1, [sp, #28]
	ldr	r2, [sp, #24]
	bl	sub_080ad168
	b.n	.L_08102b86
.L_08102b6a:
	movs	r0, #175
	str	r4, [sp, #8]
	bl	sub_081c0010
	ldr	r1, [sp, #28]
	ldr	r2, [sp, #24]
	ldr	r0, [sp, #32]
	bl	sub_080ad160
	ldr	r0, [sp, #32]
	ldr	r1, [sp, #28]
	ldr	r2, [sp, #24]
	bl	sub_080ad170
.L_08102b86:
	ldr	r4, [sp, #8]
	ldr	r0, [sp, #32]
	str	r4, [sp, #8]
	bl	sub_080ad008
	mov	r1, fp
	ldr	r4, [sp, #8]
	cmp	r1, #0
	bge.n	.L_08102b9a
	adds	r1, #3
.L_08102b9a:
	asrs	r1, r1, #2
	lsls	r1, r1, #2
	mov	r0, sl
	str	r4, [sp, #8]
	bl	sub_08101ac8
	movs	r2, #1
	str	r2, [sp, #68]
	ldr	r4, [sp, #8]
.L_08102bac:
	ldr	r3, [sp, #84]
	cmp	r3, #0
	bne.n	.L_08102bd0
	movs	r3, #128
	lsls	r3, r3, #2
	ands	r3, r4
	cmp	r3, #0
	beq.n	.L_08102bd0
	movs	r0, #112
	movs	r5, #7
	bl	sub_081c0010
	b.n	.L_08102fd4
	movs	r0, r0
	.4byte 0x02000240
	.2byte 0x1150
	.2byte 0x0300
.L_08102bd0:
	movs	r3, #1
	ands	r3, r4
	cmp	r3, #0
	bne.n	.L_08102be8
	ldr	r7, [sp, #84]
	cmp	r7, #1
	bne.n	.L_08102c6c
	movs	r3, #128
	lsls	r3, r3, #1
	ands	r3, r4
	cmp	r3, #0
	beq.n	.L_08102c6c
.L_08102be8:
	ldr	r0, [sp, #60]
	movs	r1, #1
	negs	r1, r1
	cmp	r0, r1
	beq.n	.L_08102c24
	ldr	r2, [sp, #20]
	cmp	r2, #2
	bne.n	.L_08102c24
	movs	r0, #114
	bl	sub_081c0010
	ldr	r0, [sp, #72]
	bl	sub_08038268
	movs	r3, #104
	str	r3, [sp, #0]
	ldr	r0, [sp, #72]
	movs	r1, #0
	movs	r2, #96
	movs	r3, #224
	bl	sub_08038068
	ldr	r0, [pc, #772]
	ldr	r1, [sp, #72]
	movs	r2, #0
	movs	r3, #96
	bl	sub_08038080
	bl	.L_08102188
.L_08102c24:
	ldr	r3, [sp, #84]
	movs	r5, #1
	cmp	r3, #1
	bne.n	.L_08102c64
	ldr	r4, [sp, #80]
	movs	r3, #28
	ldrsb	r3, [r4, r3]
	cmp	fp, r3
	bne.n	.L_08102c4e
	movs	r7, #182
	lsls	r7, r7, #1
	adds	r3, r4, r7
	ldrh	r2, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #8
	ands	r3, r2
	movs	r5, #2
	cmp	r3, #0
	bne.n	.L_08102c64
	movs	r5, #1
	b.n	.L_08102c64
.L_08102c4e:
	ldr	r0, [sp, #12]
	mov	r1, fp
	ldrb	r3, [r0, r1]
	ldr	r2, [sp, #84]
	ands	r2, r3
	negs	r3, r2
	orrs	r3, r2
	lsrs	r3, r3, #31
	adds	r5, r3, #0
	movs	r3, #4
	subs	r5, r3, r5
.L_08102c64:
	movs	r0, #112
	bl	sub_081c0010
	b.n	.L_08102fd4
.L_08102c6c:
	movs	r3, #8
	ands	r3, r4
	cmp	r3, #0
	beq.n	.L_08102c7a
	movs	r0, #113
	movs	r5, #2
	b.n	.L_08102c86
.L_08102c7a:
	movs	r3, #2
	ands	r3, r4
	cmp	r3, #0
	beq.n	.L_08102c8e
	movs	r0, #113
	movs	r5, #1
.L_08102c86:
	bl	sub_081c0010
	negs	r5, r5
	b.n	.L_08102fd4
.L_08102c8e:
	ldr	r2, [sp, #84]
	cmp	r2, #0
	beq.n	.L_08102c96
	b.n	.L_08102db4
.L_08102c96:
	movs	r3, #4
	ands	r3, r4
	cmp	r3, #0
	bne.n	.L_08102ca0
	b.n	.L_08102db4
.L_08102ca0:
	ldr	r3, [sp, #40]
	cmp	r3, #0
	beq.n	.L_08102d9e
	ldr	r4, [sp, #36]
	movs	r3, #1
	eors	r4, r3
	str	r4, [sp, #36]
	cmp	r4, #0
	beq.n	.L_08102cba
	movs	r0, #139
	bl	sub_081c0010
	b.n	.L_08102cc0
.L_08102cba:
	movs	r0, #175
	bl	sub_081c0010
.L_08102cc0:
	ldr	r0, [sp, #80]
	movs	r1, #139
	lsls	r1, r1, #1
	adds	r1, #255
	adds	r3, r0, r1
	ldrb	r3, [r3, #0]
	movs	r7, #0
	mov	r9, r7
	cmp	r9, r3
	bge.n	.L_08102d86
	movs	r2, #160
	add	r2, sl
	mov	r8, r2
.L_08102cda:
	movs	r3, #0
	str	r3, [sp, #64]
	mov	r4, r8
	movs	r3, #0
	ldrsb	r3, [r4, r3]
	movs	r7, #0
	cmp	r7, r3
	bge.n	.L_08102d70
	mov	r0, r9
	lsls	r3, r0, #2
	add	r3, r9
	lsls	r3, r3, #2
	mov	r1, sl
	adds	r4, r3, r1
.L_08102cf6:
	ldrh	r0, [r4, #0]
	movs	r3, #240
	lsls	r3, r3, #4
	ands	r3, r0
	lsrs	r6, r3, #8
	movs	r3, #224
	adds	r4, #2
	ands	r3, r0
	movs	r7, #31
	str	r4, [sp, #8]
	lsrs	r5, r3, #5
	ands	r7, r0
	bl	sub_08101a04
	ldr	r4, [sp, #8]
	cmp	r0, #0
	beq.n	.L_08102d1e
	cmp	r0, #1
	beq.n	.L_08102d3c
	b.n	.L_08102d60
.L_08102d1e:
	ldr	r2, [sp, #36]
	cmp	r2, #0
	beq.n	.L_08102d60
	adds	r1, r5, #0
	adds	r2, r7, #0
	adds	r0, r6, #0
	str	r4, [sp, #8]
	bl	sub_080ad158
	adds	r1, r5, #0
	adds	r2, r7, #0
	adds	r0, r6, #0
	bl	sub_080ad168
	b.n	.L_08102d58
.L_08102d3c:
	ldr	r3, [sp, #36]
	cmp	r3, #0
	bne.n	.L_08102d60
	adds	r1, r5, #0
	adds	r2, r7, #0
	adds	r0, r6, #0
	str	r4, [sp, #8]
	bl	sub_080ad160
	adds	r1, r5, #0
	adds	r2, r7, #0
	adds	r0, r6, #0
	bl	sub_080ad170
.L_08102d58:
	adds	r0, r6, #0
	bl	sub_080ad008
	ldr	r4, [sp, #8]
.L_08102d60:
	ldr	r7, [sp, #64]
	mov	r0, r8
	adds	r7, #1
	str	r7, [sp, #64]
	movs	r3, #0
	ldrsb	r3, [r0, r3]
	cmp	r7, r3
	blt.n	.L_08102cf6
.L_08102d70:
	ldr	r2, [sp, #80]
	movs	r4, #139
	lsls	r4, r4, #1
	adds	r4, #255
	adds	r3, r2, r4
	ldrb	r3, [r3, #0]
	movs	r1, #1
	add	r9, r1
	add	r8, r1
	cmp	r9, r3
	blt.n	.L_08102cda
.L_08102d86:
	mov	r1, fp
	cmp	r1, #0
	bge.n	.L_08102d8e
	adds	r1, #3
.L_08102d8e:
	asrs	r1, r1, #2
	lsls	r1, r1, #2
	mov	r0, sl
	bl	sub_08101ac8
	movs	r7, #1
	str	r7, [sp, #68]
	b.n	.L_08102db4
.L_08102d9e:
	ldr	r0, [sp, #72]
	mov	r1, fp
	ldr	r2, [sp, #52]
	movs	r3, #0
	bl	sub_08101c40
	movs	r0, #112
	movs	r5, #10
	bl	sub_081c0010
	b.n	.L_08102fd4
.L_08102db4:
	ldr	r0, [sp, #16]
	movs	r3, #64
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L_08102e4c
	ldr	r1, [sp, #12]
	mov	r3, fp
	ldrb	r2, [r1, r3]
	movs	r3, #4
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08102dd0
	bl	.L_08102188
.L_08102dd0:
	mov	r5, fp
	adds	r5, #160
	mov	r4, sl
	ldrsb	r3, [r4, r5]
	cmp	r3, #0
	bne.n	.L_08102de0
	bl	.L_08102188
.L_08102de0:
	ldr	r0, [sp, #72]
	mov	r1, fp
	ldr	r2, [sp, #52]
	movs	r3, #0
	bl	sub_08101c40
	movs	r0, #111
	bl	sub_081c0010
	ldr	r7, [sp, #12]
	mov	r0, fp
	ldrb	r2, [r7, r0]
	movs	r3, #1
	ands	r3, r2
	movs	r1, #1
	cmp	r3, #0
	beq.n	.L_08102e10
	movs	r3, #2
	negs	r3, r3
	ands	r3, r2
	movs	r1, #0
	strb	r3, [r7, r0]
	str	r1, [sp, #52]
	b.n	.L_08102e30
.L_08102e10:
	ldr	r3, [sp, #52]
	cmp	r3, #0
	bne.n	.L_08102e30
	movs	r3, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08102e30
	ldr	r4, [sp, #12]
	adds	r3, r2, #0
	orrs	r3, r1
	mov	r7, fp
	movs	r0, #2
	strb	r3, [r4, r7]
	str	r0, [sp, #68]
	bl	.L_08102188
.L_08102e30:
	ldr	r1, [sp, #52]
	mov	r2, sl
	subs	r1, #1
	str	r1, [sp, #52]
	ldr	r0, [sp, #52]
	ldrsb	r1, [r2, r5]
	bl	sub_08100e28
	movs	r3, #2
	str	r0, [sp, #52]
	str	r0, [sp, #48]
	str	r3, [sp, #68]
	bl	.L_08102188
.L_08102e4c:
	ldr	r4, [sp, #16]
	movs	r3, #128
	ands	r3, r4
	cmp	r3, #0
	beq.n	.L_08102edc
	ldr	r7, [sp, #12]
	mov	r0, fp
	ldrb	r2, [r7, r0]
	movs	r3, #4
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08102e68
	bl	.L_08102188
.L_08102e68:
	mov	r5, fp
	adds	r5, #160
	mov	r1, sl
	ldrsb	r3, [r1, r5]
	cmp	r3, #0
	bne.n	.L_08102e78
	bl	.L_08102188
.L_08102e78:
	ldr	r0, [sp, #72]
	ldr	r2, [sp, #52]
	mov	r1, fp
	movs	r3, #0
	bl	sub_08101c40
	movs	r0, #111
	bl	sub_081c0010
	ldr	r2, [sp, #52]
	mov	r3, sl
	adds	r2, #1
	str	r2, [sp, #52]
	adds	r0, r2, #0
	ldrsb	r1, [r3, r5]
	bl	sub_08100e28
	mov	r4, fp
	str	r0, [sp, #52]
	ldrb	r2, [r7, r4]
	movs	r3, #1
	ands	r3, r2
	movs	r1, #1
	cmp	r3, #0
	beq.n	.L_08102eb8
	movs	r3, #2
	negs	r3, r3
	ands	r3, r2
	strb	r3, [r7, r4]
	movs	r7, #0
	str	r7, [sp, #52]
	b.n	.L_08102ed0
.L_08102eb8:
	ldr	r0, [sp, #52]
	cmp	r0, #0
	bne.n	.L_08102ed0
	movs	r3, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08102ed0
	adds	r3, r2, #0
	orrs	r3, r1
	ldr	r1, [sp, #12]
	mov	r2, fp
	strb	r3, [r1, r2]
.L_08102ed0:
	ldr	r3, [sp, #52]
	movs	r4, #2
	str	r3, [sp, #48]
	str	r4, [sp, #68]
	bl	.L_08102188
.L_08102edc:
	ldr	r7, [sp, #16]
	movs	r3, #48
	ands	r3, r7
	cmp	r3, #0
	bne.n	.L_08102eea
	bl	.L_08102188
.L_08102eea:
	movs	r0, #111
	bl	sub_081c0010
	ldr	r0, [sp, #12]
	mov	r1, fp
	ldrb	r2, [r0, r1]
	movs	r3, #4
	ands	r3, r2
	mov	r8, fp
	cmp	r3, #0
	bne.n	.L_08102f0a
	ldr	r0, [sp, #72]
	ldr	r2, [sp, #52]
	movs	r3, #0
	bl	sub_08101c40
.L_08102f0a:
	ldr	r2, [sp, #16]
	movs	r3, #32
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08102f20
	movs	r3, #1
	negs	r3, r3
	add	fp, r3
	b.n	.L_08102f24
	.2byte 0x10ef
	.2byte 0x0000
.L_08102f20:
	movs	r4, #1
	add	fp, r4
.L_08102f24:
	mov	r7, fp
	cmp	r7, #0
	bge.n	.L_08102f2e
	movs	r0, #0
	mov	fp, r0
.L_08102f2e:
	ldr	r1, [sp, #80]
	movs	r3, #139
	lsls	r3, r3, #1
	adds	r3, #255
	adds	r2, r1, r3
	ldrb	r1, [r2, #0]
	subs	r3, r1, #1
	cmp	fp, r3
	ble.n	.L_08102f42
	mov	fp, r3
.L_08102f42:
	ldr	r4, [sp, #84]
	cmp	r4, #0
	bne.n	.L_08102f8c
	adds	r0, r2, #0
	ldr	r2, [sp, #16]
	mov	ip, r1
	movs	r1, #32
	ands	r1, r2
	ldr	r2, [sp, #12]
	movs	r7, #0
	mov	r9, r7
	add	r2, fp
.L_08102f5a:
	cmp	r9, ip
	bge.n	.L_08102f8c
	movs	r3, #0
	ldrsb	r3, [r2, r3]
	cmp	r3, #4
	bne.n	.L_08102f8c
	cmp	r1, #0
	beq.n	.L_08102f74
	movs	r3, #1
	negs	r3, r3
	subs	r2, #1
	add	fp, r3
	b.n	.L_08102f7a
.L_08102f74:
	movs	r4, #1
	adds	r2, #1
	add	fp, r4
.L_08102f7a:
	mov	r7, fp
	cmp	r7, #0
	blt.n	.L_08102f8a
	ldrb	r3, [r0, #0]
	movs	r4, #1
	add	r9, r4
	cmp	fp, r3
	blt.n	.L_08102f5a
.L_08102f8a:
	mov	fp, r8
.L_08102f8c:
	ldr	r7, [sp, #48]
	mov	r3, fp
	str	r7, [sp, #52]
	adds	r3, #160
	mov	r0, sl
	ldrsb	r5, [r0, r3]
	cmp	r5, #0
	bne.n	.L_08102f9e
	movs	r5, #1
.L_08102f9e:
	ldr	r0, [sp, #52]
	adds	r1, r5, #0
	bl	sub_08100e28
	movs	r1, #2
	mov	r3, r8
	str	r0, [sp, #52]
	str	r1, [sp, #68]
	cmp	r3, #0
	bge.n	.L_08102fb4
	adds	r3, #3
.L_08102fb4:
	asrs	r3, r3, #2
	lsls	r2, r3, #2
	mov	r3, fp
	cmp	r3, #0
	bge.n	.L_08102fc0
	adds	r3, #3
.L_08102fc0:
	asrs	r3, r3, #2
	lsls	r3, r3, #2
	cmp	r2, r3
	bne.n	.L_08102fcc
	bl	.L_08102188
.L_08102fcc:
	movs	r2, #6
	str	r2, [sp, #68]
	bl	.L_08102188
.L_08102fd4:
	ldr	r3, [sp, #84]
	ldr	r4, [sp, #80]
	adds	r3, #28
	mov	r7, fp
	strb	r7, [r4, r3]
	ldr	r0, [sp, #60]
	movs	r1, #1
	negs	r1, r1
	cmp	r0, r1
	beq.n	.L_08103032
	ldr	r2, [sp, #56]
	movs	r3, #182
	lsls	r3, r3, #1
	mov	r4, fp
	adds	r1, r2, r3
	lsls	r3, r4, #2
	add	r3, fp
	lsls	r3, r3, #1
	adds	r3, r3, r0
	lsls	r3, r3, #1
	mov	r7, sl
	ldrh	r2, [r7, r3]
	ldr	r0, [sp, #80]
	movs	r4, #150
	strh	r2, [r0, r1]
	ldr	r3, [sp, #84]
	lsls	r4, r4, #2
	adds	r1, r3, r4
	movs	r3, #31
	ands	r3, r2
	strb	r3, [r0, r1]
	ldr	r0, [sp, #80]
	movs	r3, #224
	ands	r3, r2
	adds	r0, #2
	lsrs	r3, r3, #5
	strb	r3, [r0, r1]
	movs	r3, #240
	ldr	r7, [sp, #84]
	lsls	r3, r3, #4
	ands	r3, r2
	movs	r0, #151
	ldr	r2, [sp, #80]
	lsls	r0, r0, #2
	adds	r1, r7, r0
	lsrs	r3, r3, #8
	strb	r3, [r2, r1]
.L_08103032:
	ldr	r0, [sp, #52]
	ldr	r4, [sp, #56]
	lsls	r2, r0, #2
	ldr	r1, [sp, #80]
	adds	r2, r2, r0
	movs	r7, #180
	lsls	r7, r7, #1
	lsls	r2, r2, #1
	adds	r3, r4, r7
	add	r2, fp
	strh	r2, [r1, r3]
	movs	r1, #128
	ldr	r0, [sp, #84]
	lsls	r1, r1, #7
	bl	sub_0810532c
	adds	r0, r5, #0
	add	sp, #112
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
