.syntax unified
	.thumb
	.set sub_08013164, 0x08013164
	.set sub_08013560, 0x08013560
	.set sub_08014274, 0x08014274
	.set sub_080143e0, 0x080143e0
	.set sub_080145a8, 0x080145a8
	.set sub_08014d00, 0x08014d00
	.set sub_08014d78, 0x08014d78
	.set sub_08016ca4, 0x08016ca4
	.set sub_08016ce4, 0x08016ce4
	.set sub_08039260, 0x08039260
	.set sub_0803939c, 0x0803939c
	.set sub_0803a3b8, 0x0803a3b8
	.set sub_0803aae4, 0x0803aae4
	.set sub_0803c274, 0x0803c274
	.set sub_0803c378, 0x0803c378
	.set sub_0803c9bc, 0x0803c9bc
	.set sub_08041b68, 0x08041b68
	.set sub_08041f70, 0x08041f70
	.set sub_08042010, 0x08042010
	.set sub_08042214, 0x08042214
	.set sub_0804297c, 0x0804297c
	.set sub_0804519c, 0x0804519c
	.set sub_080451bc, 0x080451bc
	.set sub_08045330, 0x08045330
	.set sub_080453d0, 0x080453d0
	.set sub_080454a0, 0x080454a0
	.set sub_080454dc, 0x080454dc
	.set sub_08045528, 0x08045528
	.set sub_080455dc, 0x080455dc
	.set sub_080457b0, 0x080457b0
	.set sub_080457d0, 0x080457d0
	.set sub_080461c8, 0x080461c8
	.set sub_08046b08, 0x08046b08
	.set sub_0804868c, 0x0804868c
	.set sub_08049154, 0x08049154
	.set sub_080499b4, 0x080499b4
	.set sub_08049a30, 0x08049a30
	.set sub_0804a134, 0x0804a134
	.set sub_0804a85c, 0x0804a85c
	.set sub_0804b7c0, 0x0804b7c0
	.set sub_0804b87c, 0x0804b87c
	.set sub_0804b8b8, 0x0804b8b8
	.set sub_0804babc, 0x0804babc
	.set sub_0804cb92, 0x0804cb92
	.set sub_0804cc24, 0x0804cc24
	.set sub_0804cc30, 0x0804cc30
	.set sub_080ad010, 0x080ad010
	.set sub_080ad078, 0x080ad078
	.set sub_080ad188, 0x080ad188
	.set sub_080ad190, 0x080ad190
	.set sub_080ad1b0, 0x080ad1b0
	.set sub_08118090, 0x08118090
	.set sub_081180e0, 0x081180e0
	.set sub_08118118, 0x08118118
	.set sub_08118130, 0x08118130
	.set sub_08118138, 0x08118138
	.set sub_08118158, 0x08118158
	.set sub_081c0010, 0x081c0010
	.global Func_0804bba8
	.thumb_func
Func_0804bba8:
.L_0804bba8:
	push	{r5, lr}
	mov	r5, r9
	push	{r5}
	sub	sp, #8
	mov	r5, sp
	mov	r3, r9
	str	r3, [sp, #4]
	adds	r0, r5, #0
	movs	r3, #255
	strh	r3, [r5, #0]
	bl	sub_080461c8
	adds	r0, r5, #0
	movs	r1, #1
	bl	sub_081180e0
	add	sp, #8
	pop	{r3}
	mov	r9, r3
	pop	{r5, pc}
	push	{r5, r6, lr}
	mov	r6, r9
	push	{r6}
	sub	sp, #132
	mov	r2, r9
	mov	r6, sp
	add	r3, sp, #128
	adds	r5, r2, #0
	str	r2, [r3, #0]
	adds	r1, r6, #0
	movs	r2, #52
	subs	r5, #8
	ldr	r0, [pc, #24]
	bl	sub_0803c9bc
	ldr	r3, [r5, #0]
	adds	r0, r6, #0
	ldr	r1, [r3, #68]
	movs	r2, #0
	movs	r3, #4
	bl	sub_0803aae4
	add	sp, #132
	pop	{r3}
	mov	r9, r3
	pop	{r5, r6, pc}
	.2byte 0x0c59
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #108
	str	r0, [sp, #88]
	movs	r0, #128
	lsls	r0, r0, #1
	str	r0, [sp, #68]
	movs	r0, #128
	movs	r3, #0
	movs	r6, #0
	lsls	r0, r0, #3
	str	r1, [sp, #84]
	str	r2, [sp, #80]
	str	r3, [sp, #96]
	str	r6, [sp, #60]
	str	r6, [sp, #56]
	bl	sub_080143e0
	str	r0, [sp, #52]
	movs	r0, #128
	lsls	r0, r0, #2
	bl	sub_080143e0
	movs	r1, #136
	str	r0, [sp, #48]
	lsls	r1, r1, #1
	movs	r0, #228
	bl	sub_08014d00
	mov	r1, sp
	adds	r1, #100
	str	r1, [sp, #36]
	str	r0, [r1, #0]
	ldr	r0, [sp, #52]
	movs	r1, #0
	str	r6, [sp, #92]
	bl	sub_080455dc
	ldr	r0, [pc, #168]
	bl	sub_080453d0
	ldr	r0, [pc, #164]
	bl	sub_08045330
	bl	sub_080451bc
	ldr	r2, [sp, #36]
	movs	r4, #130
	ldr	r1, [r2, #0]
	lsls	r4, r4, #1
	movs	r2, #1
	movs	r0, #134
	negs	r2, r2
	adds	r3, r1, r4
	lsls	r0, r0, #1
	str	r2, [r3, #0]
	adds	r3, r1, r0
	str	r2, [r3, #0]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #36]
	movs	r0, #128
	mov	ip, r3
	adds	r3, r1, #0
	lsls	r0, r0, #24
	adds	r3, #228
	movs	r2, #7
.L_0804bc98:
	subs	r2, #1
	stmia	r3!, {r0}
	cmp	r2, #0
	bge.n	.L_0804bc98
	ldr	r2, [sp, #36]
	movs	r1, #0
	ldr	r3, [r2, #0]
	movs	r2, #2
	adds	r3, #36
.L_0804bcaa:
	subs	r2, #1
	strb	r1, [r3, #0]
	adds	r3, #1
	cmp	r2, #0
	bge.n	.L_0804bcaa
	ldr	r4, [sp, #36]
	movs	r2, #1
	ldr	r1, [r4, #0]
	movs	r3, #0
	negs	r2, r2
	str	r3, [r1, #40]
	str	r3, [r1, #44]
	str	r3, [r1, #60]
	str	r3, [r1, #64]
	str	r3, [r1, #80]
	str	r3, [r1, #72]
	str	r2, [r1, #76]
	str	r3, [r1, #68]
	mov	r3, ip
	adds	r3, #68
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_0804bcda
	b.n	.L_0804bdb8
.L_0804bcda:
	movs	r3, #1
	ldr	r2, [pc, #48]
	str	r3, [r1, #80]
	ldr	r3, [pc, #24]
	movs	r5, #0
	strh	r3, [r2, #8]
	ldr	r3, [pc, #24]
	strh	r3, [r2, #10]
	strh	r3, [r2, #12]
	ldr	r3, [pc, #20]
	strh	r3, [r2, #14]
	mov	r7, ip
	mov	r6, ip
	adds	r7, #80
	adds	r6, #82
	b.n	.L_0804bd94
	movs	r0, r0
	.4byte 0x00000056
	.4byte 0x00000053
	.4byte 0x00000054
	.4byte 0x06006000
	.4byte 0x06006680
	.2byte 0x3a74
	.2byte 0x0200
.L_0804bd14:
	ldr	r3, [pc, #680]
	ldrh	r2, [r3, #0]
	movs	r3, #3
	ands	r3, r2
	cmp	r3, #3
	beq.n	.L_0804bd34
	adds	r5, #1
	cmp	r5, #24
	ble.n	.L_0804bd8e
	movs	r1, #1
	negs	r1, r1
	str	r1, [sp, #80]
	ldr	r2, [sp, #36]
	ldr	r3, [r2, #0]
	str	r0, [r3, #80]
	b.n	.L_0804bdb8
.L_0804bd34:
	ldrh	r2, [r1, #8]
	movs	r5, #0
	adds	r3, r2, #0
	cmp	r3, #86
	bne.n	.L_0804bd50
	ldrh	r3, [r1, #10]
	cmp	r3, #83
	bne.n	.L_0804bd50
	ldrh	r3, [r1, #12]
	cmp	r3, #83
	bne.n	.L_0804bd50
	ldrh	r3, [r1, #14]
	cmp	r3, #84
	beq.n	.L_0804bdb8
.L_0804bd50:
	adds	r3, r2, #0
	cmp	r3, #69
	bne.n	.L_0804bd68
	ldrh	r3, [r1, #10]
	cmp	r3, #68
	bne.n	.L_0804bd68
	ldrh	r3, [r1, #12]
	cmp	r3, #86
	bne.n	.L_0804bd68
	ldrh	r3, [r1, #14]
	cmp	r3, #83
	beq.n	.L_0804bdb8
.L_0804bd68:
	ldrh	r3, [r1, #0]
	cmp	r3, #69
	bne.n	.L_0804bd80
	ldrh	r3, [r1, #2]
	cmp	r3, #88
	bne.n	.L_0804bd80
	ldrh	r3, [r1, #4]
	cmp	r3, #69
	bne.n	.L_0804bd80
	ldrh	r3, [r1, #6]
	cmp	r3, #67
	beq.n	.L_0804bd8e
.L_0804bd80:
	movs	r3, #1
	negs	r3, r3
	str	r3, [sp, #80]
	ldr	r4, [sp, #36]
	movs	r2, #0
	ldr	r3, [r4, #0]
	b.n	.L_0804bdb6
.L_0804bd8e:
	movs	r0, #1
	bl	sub_08013560
.L_0804bd94:
	ldrb	r2, [r7, #0]
	movs	r3, #1
	eors	r3, r2
	lsls	r2, r3, #1
	adds	r2, r2, r3
	ldrb	r0, [r6, #0]
	ldr	r3, [pc, #544]
	lsls	r2, r2, #3
	adds	r1, r2, r3
	cmp	r0, #0
	beq.n	.L_0804bd14
	movs	r0, #1
	negs	r0, r0
	str	r0, [sp, #80]
	ldr	r1, [sp, #36]
	movs	r2, #0
	ldr	r3, [r1, #0]
.L_0804bdb6:
	str	r2, [r3, #80]
.L_0804bdb8:
	movs	r1, #144
	ldr	r0, [pc, #524]
	lsls	r1, r1, #3
	bl	sub_080145a8
.L_0804bdc2:
	ldr	r2, [sp, #88]
	ldr	r3, [sp, #80]
	str	r2, [sp, #96]
	cmp	r3, #0
	blt.n	.L_0804bddc
	movs	r3, #0
	add	r1, sp, #96
	add	r2, sp, #92
	ldr	r0, [sp, #84]
	str	r3, [sp, #92]
	bl	sub_0804babc
	str	r0, [sp, #80]
.L_0804bddc:
	add	r4, sp, #108
	mov	r9, r4
	bl	.L_0804bba8
	ldr	r1, [sp, #36]
	ldr	r0, [r1, #0]
	movs	r1, #0
	adds	r3, r0, #0
	adds	r3, #38
	strb	r1, [r3, #0]
	adds	r2, r0, #0
	movs	r3, #1
	negs	r3, r3
	adds	r2, #224
	str	r3, [r2, #0]
	adds	r3, r0, #0
	adds	r3, #216
	movs	r0, #183
	str	r1, [r3, #0]
	lsls	r0, r0, #1
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_0804be1e
	ldr	r3, [sp, #36]
	ldr	r2, [r3, #0]
	movs	r3, #1
	adds	r1, r2, #0
	adds	r1, #216
	str	r3, [r1, #0]
	adds	r2, #220
	movs	r3, #60
	str	r3, [r2, #0]
.L_0804be1e:
	ldr	r4, [sp, #80]
	cmp	r4, #0
	blt.n	.L_0804be30
	movs	r0, #0
	adds	r1, r4, #0
	bl	sub_080457d0
	adds	r6, r0, #0
	b.n	.L_0804be32
.L_0804be30:
	movs	r6, #14
.L_0804be32:
	movs	r0, #1
	negs	r0, r0
	cmp	r6, r0
	bne.n	.L_0804beb4
	ldr	r2, [sp, #36]
	movs	r4, #130
	ldr	r1, [r2, #0]
	lsls	r4, r4, #1
	adds	r3, r1, r4
	ldr	r0, [r3, #0]
	cmp	r0, r6
	beq.n	.L_0804bdc2
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r4, #4
	ldr	r2, [r3, #36]
	adds	r3, r1, r4
	ldr	r3, [r3, #0]
	lsls	r3, r3, #1
	adds	r3, #88
	ldrsh	r4, [r2, r3]
	strh	r0, [r2, r3]
	lsls	r3, r0, #16
	asrs	r1, r3, #16
	ldr	r3, [pc, #360]
	movs	r2, #134
	lsls	r2, r2, #2
	movs	r0, #7
	adds	r3, r3, r2
.L_0804be6c:
	ldrb	r2, [r3, #0]
	cmp	r2, r1
	bne.n	.L_0804be76
	strb	r4, [r3, #0]
	b.n	.L_0804be7c
.L_0804be76:
	cmp	r2, r4
	bne.n	.L_0804be7c
	strb	r1, [r3, #0]
.L_0804be7c:
	subs	r0, #1
	adds	r3, #1
	cmp	r0, #0
	bge.n	.L_0804be6c
	movs	r5, #192
	lsls	r5, r5, #18
	ldr	r3, [r5, #36]
	adds	r5, #228
	adds	r3, #65
	ldrb	r0, [r3, #0]
	bl	sub_0804297c
	bl	sub_08118118
	ldr	r4, [sp, #36]
	movs	r0, #130
	ldr	r3, [r4, #0]
	lsls	r0, r0, #1
	movs	r2, #1
	adds	r3, r3, r0
	negs	r2, r2
	str	r2, [r3, #0]
	bl	sub_08118158
	ldr	r2, [r5, #0]
	movs	r3, #1
	str	r3, [r2, #60]
	b.n	.L_0804bdc2
.L_0804beb4:
	cmp	r6, #49
	bne.n	.L_0804bef2
	movs	r0, #112
	bl	sub_081c0010
.L_0804bebe:
	movs	r0, #0
	movs	r1, #2
	movs	r2, #1
	movs	r3, #8
	bl	sub_0804a85c
	movs	r5, #1
	adds	r6, r0, #0
	negs	r5, r5
	cmp	r6, r5
	bne.n	.L_0804bed6
	b.n	.L_0804bdc2
.L_0804bed6:
	bl	sub_0804b8b8
	adds	r6, r0, #0
	cmp	r6, r5
	beq.n	.L_0804bebe
	bl	sub_08118158
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #228
	ldr	r2, [r3, #0]
	movs	r3, #0
	str	r3, [r2, #60]
	b.n	.L_0804bdc2
.L_0804bef2:
	cmp	r6, #7
	bne.n	.L_0804bf46
	movs	r0, #18
	bl	sub_08014d78
	ldr	r3, [pc, #208]
	adds	r7, r0, #0
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0804bf14
	ldr	r3, [pc, #204]
	movs	r6, #2
	ldr	r2, [r3, #0]
	movs	r3, #8
	ands	r2, r3
	cmp	r2, #0
	bne.n	.L_0804bf16
.L_0804bf14:
	movs	r6, #1
.L_0804bf16:
	adds	r0, r6, #0
	adds	r1, r7, #0
	bl	sub_08118090
	adds	r5, r0, #0
	cmp	r6, #1
	bne.n	.L_0804bf2e
	lsls	r0, r5, #1
	adds	r0, r7, r0
	bl	sub_08118138
	adds	r5, r5, r0
.L_0804bf2e:
	movs	r0, #1
	bl	sub_08013560
	ldrh	r2, [r7, #0]
	adds	r0, r7, #0
	adds	r1, r5, #0
	bl	sub_08046b08
	adds	r0, r7, #0
	bl	sub_08013164
	b.n	.L_0804bdc2
.L_0804bf46:
	cmp	r6, #4
	bne.n	.L_0804bf84
	bl	sub_0804b7c0
	cmp	r0, #0
	beq.n	.L_0804bf54
	b.n	.L_0804bdc2
.L_0804bf54:
	ldr	r2, [sp, #88]
	movs	r1, #1
	str	r2, [sp, #76]
	str	r1, [sp, #80]
	ldr	r4, [sp, #84]
	adds	r1, r2, #0
	ldrh	r3, [r4, #0]
	str	r0, [sp, #92]
	strh	r3, [r1, #0]
	ldr	r2, [sp, #76]
	movs	r3, #254
	lsls	r3, r3, #7
	adds	r3, #254
	strh	r3, [r2, #4]
	ldr	r4, [sp, #76]
	movs	r3, #99
	strh	r3, [r4, #6]
	ldr	r1, [sp, #76]
	adds	r3, #157
	strh	r0, [r1, #8]
	ldr	r2, [sp, #76]
	strh	r3, [r2, #10]
	bl	sub_0804cc30
.L_0804bf84:
	cmp	r6, #14
	beq.n	.L_0804bf8c
	bl	sub_0804cc30
.L_0804bf8c:
	movs	r0, #154
	bl	sub_081c0010
	ldr	r4, [sp, #80]
	movs	r3, #0
	str	r3, [sp, #44]
	cmp	r3, r4
	blt.n	.L_0804bfa0
	bl	sub_0804cc30
.L_0804bfa0:
	mov	r0, sp
	adds	r0, #104
	str	r3, [sp, #24]
	str	r3, [sp, #20]
	str	r3, [sp, #28]
	str	r0, [sp, #32]
.L_0804bfac:
	ldr	r1, [sp, #44]
	cmp	r1, #0
	bne.n	.L_0804bfd8
	ldr	r2, [sp, #36]
	movs	r0, #0
	ldr	r1, [r2, #0]
	adds	r1, #84
	bl	sub_08118130
	b.n	.L_0804bff2
	.4byte 0x0300124c
	.4byte 0x02003874
	.4byte 0x0804b6a1
	.4byte 0x02000240
	.4byte 0x03001238
	.2byte 0x1150
	.2byte 0x0300
.L_0804bfd8:
	ldr	r4, [sp, #36]
	ldr	r0, [sp, #24]
	ldr	r3, [r4, #0]
	movs	r1, #3
	adds	r3, r0, r3
	adds	r2, r3, #0
	adds	r2, #80
.L_0804bfe6:
	ldrb	r3, [r2, #0]
	subs	r1, #1
	strb	r3, [r2, #4]
	adds	r2, #1
	cmp	r1, #0
	bge.n	.L_0804bfe6
.L_0804bff2:
	ldr	r3, [sp, #96]
	ldr	r1, [sp, #20]
	ldr	r2, [sp, #28]
	adds	r3, r3, r1
	str	r3, [sp, #76]
	ldr	r3, [sp, #84]
	ldrh	r2, [r2, r3]
	adds	r0, r2, #0
	str	r2, [sp, #64]
	bl	sub_08016ca4
	movs	r3, #192
	str	r0, [sp, #72]
	lsls	r3, r3, #18
	adds	r3, #228
	ldr	r5, [r3, #0]
	ldr	r4, [sp, #64]
	adds	r3, r5, #0
	adds	r3, #224
	str	r4, [r3, #0]
	ldr	r3, [pc, #80]
	movs	r2, #0
	str	r2, [r5, #64]
	adds	r5, #24
	str	r3, [r5, #4]
	str	r2, [r5, #8]
	ldr	r0, [sp, #72]
	movs	r1, #165
	lsls	r1, r1, #1
	adds	r3, r0, r1
	ldrh	r0, [r3, #0]
	ldr	r1, [sp, #48]
	bl	sub_08045528
	ldr	r3, [pc, #48]
	ldrh	r2, [r5, #8]
	ands	r0, r3
	ldr	r3, [pc, #48]
	ands	r3, r2
	orrs	r3, r0
	strh	r3, [r5, #8]
	ldrh	r2, [r5, #6]
	ldr	r3, [pc, #44]
	ands	r3, r2
	ldrb	r2, [r5, #9]
	strh	r3, [r5, #6]
	movs	r3, #128
	strb	r3, [r5, #4]
	movs	r3, #15
	ands	r3, r2
	movs	r2, #224
	orrs	r3, r2
	strb	r3, [r5, #9]
	ldr	r3, [sp, #36]
	ldr	r2, [r3, #0]
	movs	r3, #1
	adds	r2, #38
	strb	r3, [r2, #0]
	b.n	.L_0804c078
	.4byte 0x000003ff
	.4byte 0x80000400
	.4byte 0xfffffc00
	.2byte 0xfe00
	.2byte 0xffff
.L_0804c078:
	bl	sub_08041b68
	movs	r5, #192
	lsls	r5, r5, #18
	adds	r5, #228
	ldr	r1, [r5, #0]
	movs	r7, #0
	adds	r3, r1, #0
	adds	r3, #36
	strb	r7, [r3, #0]
	ldr	r2, [sp, #24]
	movs	r3, #128
	adds	r2, #228
	lsls	r3, r3, #24
	str	r3, [r1, r2]
	add	r0, sp, #64
	ldr	r1, [sp, #32]
	ldrh	r0, [r0, #0]
	ldr	r4, [pc, #56]
	strh	r0, [r1, #0]
	ldr	r1, [sp, #32]
	movs	r3, #255
	strh	r3, [r1, #2]
	ldr	r0, [sp, #32]
	movs	r1, #1
	mov	r8, r4
	bl	sub_081180e0
	ldr	r0, [sp, #32]
	bl	sub_080461c8
	movs	r0, #1
	bl	sub_08013560
	movs	r1, #0
	movs	r0, #1
	bl	sub_080457d0
	adds	r6, r0, #0
	movs	r0, #1
	bl	sub_08013560
	movs	r2, #2
	negs	r2, r2
	cmp	r6, r2
	bne.n	.L_0804c11a
	b.n	.L_0804c0dc
	movs	r0, r0
	.2byte 0x0000
	.2byte 0x0000
.L_0804c0dc:
	movs	r0, #12
	bl	sub_08014d78
	adds	r5, r0, #0
	adds	r1, r5, #0
	movs	r0, #1
	bl	sub_08118090
	ldr	r4, [sp, #36]
	adds	r6, r0, #0
	ldr	r3, [r4, #0]
	mov	r0, r8
	adds	r3, #38
	strb	r0, [r3, #0]
	movs	r0, #1
	bl	sub_08013560
	adds	r1, r6, #0
	ldr	r2, [sp, #64]
	adds	r0, r5, #0
	bl	sub_08046b08
	ldr	r1, [sp, #36]
	movs	r2, #1
	ldr	r3, [r1, #0]
	adds	r0, r5, #0
	adds	r3, #38
	strb	r2, [r3, #0]
	bl	sub_08013164
	b.n	.L_0804bfac
.L_0804c11a:
	ldr	r0, [sp, #32]
	movs	r1, #0
	bl	sub_081180e0
	movs	r2, #1
	negs	r2, r2
	cmp	r6, r2
	bne.n	.L_0804c146
	ldr	r3, [sp, #44]
	cmp	r3, #0
	bne.n	.L_0804c134
	bl	sub_0804cc24
.L_0804c134:
	subs	r3, #1
	lsls	r4, r3, #2
	lsls	r0, r3, #4
	lsls	r1, r3, #1
	str	r3, [sp, #44]
	str	r4, [sp, #24]
	str	r0, [sp, #20]
	str	r1, [sp, #28]
	b.n	.L_0804bfac
.L_0804c146:
	ldr	r5, [r5, #0]
	ldr	r3, [r5, #76]
	cmp	r3, #0
	bne.n	.L_0804c150
	movs	r6, #3
.L_0804c150:
	ldr	r3, [pc, #48]
	str	r7, [r5, #8]
	str	r3, [r5, #4]
	ldr	r0, [sp, #52]
	adds	r1, r6, #0
	bl	sub_080455dc
	ldr	r3, [pc, #28]
	ldrh	r2, [r5, #8]
	ands	r0, r3
	ldr	r3, [pc, #32]
	ands	r3, r2
	orrs	r3, r0
	strh	r3, [r5, #8]
	cmp	r6, #15
	bne.n	.L_0804c190
	ldrh	r3, [r5, #6]
	ldr	r2, [pc, #24]
	ands	r2, r3
	ldr	r3, [pc, #8]
	b.n	.L_0804c198
	movs	r0, r0
	.4byte 0x000003ff
	.4byte 0x00000080
	.4byte 0x80002400
	.4byte 0xfffffc00
	.2byte 0xfe00
	.2byte 0xffff
.L_0804c190:
	ldrh	r3, [r5, #6]
	ldr	r2, [pc, #44]
	ands	r2, r3
	ldr	r3, [pc, #36]
.L_0804c198:
	orrs	r2, r3
	strh	r2, [r5, #6]
	movs	r3, #136
	strb	r3, [r5, #4]
	ldr	r3, [sp, #36]
	ldr	r2, [r3, #0]
	movs	r3, #1
	adds	r2, #36
	strb	r3, [r2, #0]
	cmp	r6, #16
	bls.n	.L_0804c1b2
	bl	sub_0804cb92
.L_0804c1b2:
	ldr	r2, [pc, #16]
	lsls	r3, r6, #2
	ldr	r3, [r3, r2]
	b.n	.L_0804c1c8
	movs	r0, r0
	.4byte 0x00000060
	.4byte 0xfffffe00
	.2byte 0xc1cc
	.2byte 0x0804
.L_0804c1c8:
	mov	pc, r3
	movs	r0, r0
	stmia	r2!, {r4}
	lsrs	r4, r0, #32
	stmia	r2!, {r7}
	lsrs	r4, r0, #32
	ldmia	r1!, {r4, r5, r6}
	lsrs	r4, r0, #32
	ldmia	r3, {r1, r2, r3, r7}
	lsrs	r4, r0, #32
	ldmia	r3!, {r1, r4, r7}
	lsrs	r4, r0, #32
	ldmia	r3!, {r1, r4, r7}
	lsrs	r4, r0, #32
	ldmia	r3!, {r1, r4, r7}
	lsrs	r4, r0, #32
	ldmia	r3!, {r1, r4, r7}
	lsrs	r4, r0, #32
	ldmia	r3!, {r1, r4, r7}
	lsrs	r4, r0, #32
	ldmia	r3!, {r1, r4, r7}
	lsrs	r4, r0, #32
	ldmia	r3!, {r1, r4, r7}
	lsrs	r4, r0, #32
	ldmia	r3!, {r1, r4, r7}
	lsrs	r4, r0, #32
	ldmia	r3!, {r1, r4, r7}
	lsrs	r4, r0, #32
	ldmia	r3!, {r1, r4, r7}
	lsrs	r4, r0, #32
	ldmia	r3!, {r1, r4, r7}
	lsrs	r4, r0, #32
	stmia	r7!, {r5, r6}
	lsrs	r4, r0, #32
	stmia	r4!, {r5, r6, r7}
	lsrs	r4, r0, #32
	movs	r3, #6
	str	r3, [sp, #0]
	movs	r1, #17
	movs	r2, #11
	movs	r3, #3
	movs	r0, #11
	bl	sub_08039260
	mov	fp, r0
	mov	r1, fp
	ldr	r0, [pc, #80]
	movs	r2, #16
	movs	r3, #0
	bl	sub_08042010
	ldr	r4, [sp, #36]
	ldr	r3, [pc, #72]
	ldr	r1, [r4, #0]
	movs	r0, #112
	ldrh	r2, [r1, #6]
	ands	r3, r2
	ldr	r2, [pc, #56]
	orrs	r3, r2
	strh	r3, [r1, #6]
	bl	sub_081c0010
	movs	r1, #1
	ldr	r0, [sp, #64]
	movs	r2, #1
	movs	r3, #0
	bl	sub_0804a85c
	movs	r1, #1
	adds	r6, r0, #0
	mov	r0, fp
	bl	sub_0803939c
	movs	r0, #1
	negs	r0, r0
	cmp	r6, r0
	bne.n	.L_0804c264
	b.n	.L_0804c078
.L_0804c264:
	ldr	r2, [sp, #76]
	movs	r1, #0
	movs	r3, #1
	str	r1, [sp, #60]
	str	r6, [sp, #68]
	strh	r3, [r2, #12]
	bl	sub_0804cb92
	lsls	r0, r0, #1
	movs	r0, r0
	movs	r2, r7
	movs	r0, r0
	.2byte 0xfe00
	.2byte 0xffff
	movs	r0, #112
	bl	sub_081c0010
	movs	r6, #0
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #228
	ldr	r3, [r3, #0]
	str	r6, [r3, #52]
	str	r6, [r3, #48]
	str	r6, [r3, #56]
.L_0804c296:
	ldr	r3, [sp, #36]
	movs	r4, #150
	ldr	r1, [r3, #0]
	ldr	r3, [pc, #56]
	ldrh	r2, [r1, #6]
	movs	r0, #116
	ands	r3, r2
	ldr	r2, [pc, #44]
	adds	r4, r4, r1
	orrs	r3, r2
	strh	r3, [r1, #6]
	ldr	r2, [sp, #72]
	movs	r3, #88
	ldrh	r3, [r2, r3]
	movs	r2, #252
	lsls	r2, r2, #6
	adds	r2, #255
	adds	r5, r2, #0
	adds	r0, r0, r1
	ands	r5, r3
	mov	r8, r4
	mov	sl, r0
	movs	r4, #0
	movs	r1, #0
	cmp	r5, #0
	beq.n	.L_0804c310
	ldr	r7, [sp, #72]
	mov	r6, r8
	adds	r7, #88
	mov	r9, r2
	b.n	.L_0804c2dc
	.4byte 0x00000030
	.2byte 0xfe00
	.2byte 0xffff
.L_0804c2dc:
	adds	r0, r5, #0
	str	r1, [sp, #16]
	str	r4, [sp, #4]
	bl	sub_080ad078
	ldrb	r2, [r0, #1]
	movs	r3, #128
	ands	r3, r2
	ldr	r1, [sp, #16]
	ldr	r4, [sp, #4]
	cmp	r3, #0
	beq.n	.L_0804c2fe
	mov	r3, sl
	strb	r1, [r3, r4]
	strh	r5, [r6, #0]
	adds	r4, #1
	adds	r6, #2
.L_0804c2fe:
	adds	r1, #1
	cmp	r1, #32
	beq.n	.L_0804c310
	adds	r7, #4
	ldrh	r3, [r7, #0]
	mov	r5, r9
	ands	r5, r3
	cmp	r5, #0
	bne.n	.L_0804c2dc
.L_0804c310:
	movs	r3, #0
	mov	r0, sl
	strb	r3, [r0, r4]
	ldr	r3, [pc, #28]
	lsls	r2, r4, #1
	mov	r1, r8
	strh	r3, [r2, r1]
	ldr	r0, [sp, #64]
	adds	r2, r4, #0
	bl	sub_0804a134
	movs	r2, #1
	adds	r6, r0, #0
	negs	r2, r2
	cmp	r6, r2
	bne.n	.L_0804c332
	b.n	.L_0804c078
.L_0804c332:
	b.n	.L_0804c338
	.2byte 0x0000
	.2byte 0x0000
.L_0804c338:
	mov	r4, sl
	ldrb	r3, [r4, r6]
	ldr	r0, [sp, #72]
	lsls	r3, r3, #2
	adds	r3, #88
	ldrh	r3, [r0, r3]
	movs	r1, #252
	lsls	r1, r1, #6
	adds	r1, #255
	ands	r1, r3
	adds	r0, r1, #0
	str	r1, [sp, #56]
	bl	sub_080ad078
	adds	r6, r0, #0
	ldr	r3, [sp, #36]
	ldrb	r2, [r6, #8]
	movs	r0, #128
	mov	r8, r2
	ldr	r5, [r3, #0]
	bl	sub_080143e0
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r7, [r3, #60]
	movs	r3, #6
	str	r3, [sp, #0]
	mov	sl, r0
	movs	r1, #17
	movs	r2, #18
	movs	r3, #3
	movs	r0, #8
	bl	sub_08039260
	ldr	r4, [sp, #36]
	mov	fp, r0
	ldr	r0, [r4, #0]
	ldr	r1, [pc, #68]
	ldrh	r2, [r0, #6]
	adds	r3, r1, #0
	ands	r3, r2
	ldr	r2, [pc, #52]
	adds	r5, #12
	orrs	r3, r2
	strh	r3, [r0, #6]
	ldr	r3, [pc, #56]
	str	r3, [r5, #4]
	movs	r3, #0
	str	r3, [r5, #8]
	mov	r3, fp
	movs	r0, #12
	ldrsh	r2, [r3, r0]
	ldr	r3, [pc, #32]
	lsls	r2, r2, #3
	adds	r2, #8
	ands	r2, r3
	ldrh	r3, [r5, #6]
	mov	r0, fp
	ands	r1, r3
	movs	r4, #14
	ldrsh	r3, [r0, r4]
	orrs	r1, r2
	lsls	r3, r3, #3
	adds	r3, #4
	strh	r1, [r5, #6]
	strb	r3, [r5, #4]
	mov	r1, sl
	b.n	.L_0804c3d0
	.4byte 0x00000028
	.4byte 0x000001ff
	.4byte 0xfffffe00
	.2byte 0x0400
	.2byte 0x4000
.L_0804c3d0:
	ldr	r0, [sp, #56]
	bl	sub_080454dc
	ldr	r3, [pc, #48]
	ldrh	r2, [r5, #8]
	ands	r0, r3
	ldr	r3, [pc, #44]
	ands	r3, r2
	orrs	r3, r0
	strh	r3, [r5, #8]
	ldr	r1, [sp, #36]
	movs	r3, #1
	ldr	r2, [r1, #0]
	adds	r2, #37
	strb	r3, [r2, #0]
	movs	r3, #5
	strb	r3, [r7, #7]
	ldr	r0, [sp, #72]
	ldrb	r2, [r6, #9]
	movs	r4, #58
	ldrsh	r3, [r0, r4]
	cmp	r2, r3
	ble.n	.L_0804c410
	movs	r0, #2
	bl	sub_08041f70
	b.n	.L_0804c424
	movs	r0, r0
	.4byte 0x000003ff
	.2byte 0xfc00
	.2byte 0xffff
.L_0804c410:
	ldr	r1, [sp, #72]
	movs	r2, #62
	adds	r2, #255
	adds	r3, r1, r2
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0804c424
	movs	r0, #9
	bl	sub_08041f70
.L_0804c424:
	ldr	r3, [sp, #56]
	ldr	r0, [pc, #180]
	mov	r1, fp
	adds	r0, r3, r0
	movs	r2, #16
	movs	r3, #0
	bl	sub_08042010
	movs	r5, #0
	ldrb	r0, [r6, #9]
	movs	r1, #2
	mov	r2, fp
	movs	r3, #104
	str	r5, [sp, #0]
	bl	sub_08042214
	movs	r3, #15
	strb	r3, [r7, #7]
	movs	r0, #15
	bl	sub_08041f70
	movs	r1, #240
	lsls	r1, r1, #8
	adds	r1, #31
	mov	r0, fp
	movs	r2, #11
	movs	r3, #0
	str	r5, [sp, #0]
	bl	sub_0803c378
	movs	r1, #240
	lsls	r1, r1, #8
	movs	r3, #0
	adds	r1, #30
	mov	r0, fp
	movs	r2, #12
	str	r5, [sp, #0]
	bl	sub_0803c378
	ldrb	r3, [r6, #2]
	cmp	r3, #4
	beq.n	.L_0804c48e
	movs	r4, #160
	lsls	r4, r4, #7
	adds	r1, r3, #0
	adds	r4, #1
	adds	r1, r1, r4
	mov	r0, fp
	movs	r2, #15
	movs	r3, #0
	str	r5, [sp, #0]
	bl	sub_0803c378
.L_0804c48e:
	ldr	r1, [sp, #76]
	mov	r0, r8
	strh	r0, [r1, #12]
	movs	r0, #112
	bl	sub_081c0010
	adds	r0, r6, #0
	bl	sub_0804b87c
	ldrb	r1, [r6, #0]
	adds	r3, r0, #0
	mov	r2, r8
	ldr	r0, [sp, #64]
	bl	sub_0804a85c
	ldr	r2, [sp, #36]
	ldr	r5, [pc, #40]
	ldr	r3, [r2, #0]
	adds	r6, r0, #0
	adds	r3, #37
	strb	r5, [r3, #0]
	mov	r0, sl
	bl	sub_08014274
	mov	r0, fp
	movs	r1, #1
	bl	sub_0803939c
	movs	r3, #1
	negs	r3, r3
	cmp	r6, r3
	bne.n	.L_0804c4d0
	b.n	.L_0804c296
.L_0804c4d0:
	movs	r4, #1
	str	r4, [sp, #60]
.L_0804c4d4:
	str	r6, [sp, #68]
	b.n	sub_0804cb92
	.4byte 0x00000000
	.2byte 0x05a7
	.2byte 0x0000
	movs	r0, #112
	bl	sub_081c0010
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #228
	ldr	r2, [r3, #0]
	movs	r3, #0
	str	r3, [r2, #52]
	str	r3, [r2, #48]
	str	r3, [r2, #56]
.L_0804c4f6:
	ldr	r0, [sp, #36]
	ldr	r3, [pc, #60]
	ldr	r1, [r0, #0]
	ldrh	r2, [r1, #6]
	ands	r3, r2
	ldr	r2, [pc, #48]
	orrs	r3, r2
	strh	r3, [r1, #6]
	ldr	r0, [sp, #32]
	bl	sub_080461c8
	ldr	r1, [sp, #36]
	ldr	r3, [sp, #24]
	ldr	r2, [r1, #0]
	movs	r0, #0
	adds	r2, r2, r3
	adds	r2, #84
	movs	r1, #0
	bl	sub_08049154
	adds	r6, r0, #0
	ldr	r0, [sp, #32]
	bl	sub_080461c8
	movs	r4, #1
	negs	r4, r4
	cmp	r6, r4
	bne.n	.L_0804c530
	b.n	.L_0804c078
.L_0804c530:
	b.n	.L_0804c53c
	movs	r0, r0
	.4byte 0x00000050
	.2byte 0xfe00
	.2byte 0xffff
.L_0804c53c:
	movs	r0, #6
	str	r0, [sp, #60]
	movs	r0, #1
	str	r6, [sp, #56]
	bl	sub_08013560
	ldr	r1, [sp, #36]
	adds	r0, r6, #0
	ldr	r3, [r1, #0]
	movs	r7, #0
	adds	r3, #12
	mov	r8, r3
	bl	sub_080ad188
	mov	r9, r0
	ldrh	r0, [r0, #0]
	bl	sub_080ad078
	mov	sl, r0
	movs	r0, #128
	bl	sub_080143e0
	ldr	r2, [sp, #60]
	str	r0, [sp, #40]
	str	r2, [sp, #0]
	movs	r1, #17
	movs	r2, #17
	movs	r3, #3
	movs	r0, #10
	bl	sub_08039260
	mov	fp, r0
	ldr	r0, [sp, #36]
	ldr	r4, [sp, #24]
	ldr	r3, [r0, #0]
	mov	r1, r9
	adds	r1, #4
	adds	r4, #84
	ldrb	r2, [r1, #0]
	ldrb	r3, [r3, r4]
	cmp	r2, r3
	bhi.n	.L_0804c5aa
	adds	r5, r0, #0
	adds	r0, r1, #0
	adds	r1, r4, #0
.L_0804c596:
	adds	r7, #1
	adds	r1, #1
	cmp	r7, #3
	bgt.n	.L_0804c5aa
	ldr	r3, [r5, #0]
	adds	r0, #1
	ldrb	r2, [r0, #0]
	ldrb	r3, [r3, r1]
	cmp	r2, r3
	bls.n	.L_0804c596
.L_0804c5aa:
	ldr	r1, [sp, #36]
	movs	r3, #4
	eors	r3, r7
	ldr	r0, [r1, #0]
	negs	r2, r3
	orrs	r2, r3
	ldr	r1, [pc, #68]
	lsrs	r4, r2, #31
	ldrh	r2, [r0, #6]
	adds	r3, r1, #0
	ands	r3, r2
	ldr	r2, [pc, #48]
	movs	r5, #1
	orrs	r3, r2
	strh	r3, [r0, #6]
	ldr	r3, [pc, #52]
	mov	r2, r8
	str	r3, [r2, #4]
	movs	r3, #0
	subs	r4, r5, r4
	str	r3, [r2, #8]
	mov	r0, fp
	movs	r3, #12
	ldrsh	r2, [r0, r3]
	ldr	r3, [pc, #28]
	lsls	r2, r2, #3
	mov	r0, r8
	adds	r2, #8
	ands	r2, r3
	ldrh	r3, [r0, #6]
	str	r4, [sp, #4]
	ands	r1, r3
	orrs	r1, r2
	mov	r2, r8
	strh	r1, [r2, #6]
	mov	r1, fp
	b.n	.L_0804c604
	.4byte 0x00000038
	.4byte 0x000001ff
	.4byte 0xfffffe00
	.2byte 0x0400
	.2byte 0x4000
.L_0804c604:
	movs	r0, #14
	ldrsh	r3, [r1, r0]
	movs	r0, #252
	lsls	r3, r3, #3
	adds	r3, #4
	strb	r3, [r2, #4]
	mov	r2, r9
	ldrh	r3, [r2, #0]
	lsls	r0, r0, #6
	adds	r0, #255
	ands	r0, r3
	ldr	r1, [sp, #40]
	bl	sub_080454dc
	ldr	r3, [pc, #52]
	ldr	r4, [sp, #4]
	ands	r0, r3
	mov	r3, r8
	ldrh	r2, [r3, #8]
	ldr	r3, [pc, #48]
	ands	r3, r2
	orrs	r3, r0
	mov	r0, r8
	strh	r3, [r0, #8]
	ldr	r1, [sp, #36]
	ldr	r3, [r1, #0]
	adds	r3, #37
	strb	r5, [r3, #0]
	cmp	r4, #0
	bne.n	.L_0804c648
	movs	r0, #2
	bl	sub_08041f70
	ldr	r4, [sp, #4]
.L_0804c648:
	adds	r0, r6, #0
	str	r4, [sp, #4]
	bl	sub_080ad188
	ldr	r3, [pc, #12]
	ldrh	r0, [r0, #0]
	b.n	.L_0804c664
	movs	r0, r0
	.4byte 0x000003ff
	.4byte 0xfffffc00
	.2byte 0x05a7
	.2byte 0x0000
.L_0804c664:
	movs	r2, #16
	adds	r0, r0, r3
	mov	r1, fp
	movs	r3, #0
	bl	sub_08042010
	movs	r2, #0
	lsls	r3, r2, #1
	ldr	r4, [sp, #4]
	mov	r6, r9
	adds	r5, r3, #0
	movs	r7, #0
	mov	r8, r2
	adds	r6, #4
	adds	r5, #11
.L_0804c682:
	ldrb	r3, [r6, #0]
	cmp	r3, #0
	beq.n	.L_0804c6b6
	movs	r3, #160
	lsls	r3, r3, #7
	adds	r3, #1
	mov	r0, r8
	adds	r1, r7, r3
	str	r0, [sp, #0]
	adds	r2, r5, #0
	mov	r0, fp
	movs	r3, #0
	str	r4, [sp, #4]
	bl	sub_0803c378
	ldrb	r1, [r6, #0]
	mov	r3, r8
	adds	r2, r5, #1
	str	r3, [sp, #0]
	adds	r1, #48
	mov	r0, fp
	movs	r3, #0
	bl	sub_0803c274
	ldr	r4, [sp, #4]
	adds	r5, #2
.L_0804c6b6:
	adds	r7, #1
	adds	r6, #1
	cmp	r7, #3
	ble.n	.L_0804c682
	cmp	r4, #0
	beq.n	.L_0804c6ca
	movs	r0, #112
	bl	sub_081c0010
	b.n	.L_0804c6d0
.L_0804c6ca:
	movs	r0, #114
	bl	sub_081c0010
.L_0804c6d0:
	mov	r0, sl
	bl	sub_0804b87c
	mov	r4, sl
	adds	r3, r0, #0
	ldrb	r1, [r4, #0]
	ldrb	r2, [r4, #8]
	ldr	r0, [sp, #64]
	bl	sub_0804a85c
	adds	r6, r0, #0
	mov	r0, sl
	ldr	r1, [sp, #76]
	ldrb	r3, [r0, #8]
	ldr	r2, [pc, #36]
	strh	r3, [r1, #12]
	ldr	r4, [sp, #36]
	ldr	r3, [r4, #0]
	adds	r3, #37
	strb	r2, [r3, #0]
	ldr	r0, [sp, #40]
	bl	sub_08014274
	mov	r0, fp
	movs	r1, #1
	bl	sub_0803939c
	movs	r0, #1
	negs	r0, r0
	cmp	r6, r0
	bne.n	.L_0804c710
	b.n	.L_0804c4f6
.L_0804c710:
	b.n	.L_0804c718
	movs	r0, r0
	.2byte 0x0000
	.2byte 0x0000
.L_0804c718:
	ldr	r1, [sp, #36]
	movs	r2, #0
	mov	lr, r2
	ldr	r2, [sp, #24]
	mov	ip, r1
	ldr	r1, [r1, #0]
	mov	r0, r9
	adds	r2, #84
	adds	r0, #4
	adds	r5, r2, #0
	ldrb	r4, [r0, #0]
	ldrb	r3, [r1, r5]
	movs	r7, #0
	cmp	r4, r3
	bls.n	.L_0804c73c
	mov	r3, lr
	strb	r3, [r1, r5]
	b.n	.L_0804c4d4
.L_0804c73c:
	subs	r3, r3, r4
	adds	r7, #1
	strb	r3, [r1, r5]
	adds	r0, #1
	adds	r2, #1
	cmp	r7, #3
	ble.n	.L_0804c74c
	b.n	.L_0804c4d4
.L_0804c74c:
	mov	r3, ip
	ldr	r1, [r3, #0]
	adds	r5, r2, #0
	ldrb	r4, [r0, #0]
	ldrb	r3, [r1, r5]
	cmp	r4, r3
	bls.n	.L_0804c73c
	mov	r4, lr
	strb	r4, [r1, r5]
	b.n	.L_0804c4d4
	movs	r0, #112
	bl	sub_081c0010
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #228
	ldr	r2, [r3, #0]
	movs	r3, #0
	str	r3, [r2, #52]
	str	r3, [r2, #48]
	str	r3, [r2, #56]
.L_0804c776:
	ldr	r0, [sp, #36]
	ldr	r2, [pc, #68]
	ldr	r1, [r0, #0]
	mov	sl, r2
	ldrh	r3, [r1, #6]
	movs	r7, #192
	ands	r2, r3
	ldr	r3, [pc, #52]
	lsls	r7, r7, #18
	orrs	r2, r3
	strh	r2, [r1, #6]
	ldr	r5, [sp, #24]
	adds	r7, #228
	ldr	r2, [r7, #0]
	movs	r3, #128
	lsls	r3, r3, #24
	adds	r5, #228
	str	r3, [r2, r5]
	movs	r1, #1
	ldr	r0, [sp, #64]
	bl	sub_0804868c
	movs	r3, #0
	adds	r6, r0, #0
	movs	r1, #1
	ldr	r0, [sp, #76]
	mov	r8, r3
	negs	r1, r1
	mov	r4, r8
	mov	r9, r1
	strh	r4, [r0, #12]
	cmp	r6, r9
	bne.n	.L_0804c7ba
	b.n	.L_0804c078
.L_0804c7ba:
	b.n	.L_0804c7c4
	.4byte 0x0000008c
	.2byte 0xfe00
	.2byte 0xffff
.L_0804c7c4:
	movs	r2, #5
	str	r6, [sp, #56]
	str	r2, [sp, #60]
	ldr	r3, [r7, #0]
	movs	r4, #255
	str	r6, [r3, r5]
	ldr	r3, [sp, #56]
	ldr	r0, [sp, #56]
	asrs	r7, r3, #8
	movs	r3, #15
	ands	r4, r0
	ands	r7, r3
	adds	r2, r4, #0
	ldr	r0, [sp, #64]
	adds	r1, r7, #0
	str	r4, [sp, #4]
	bl	sub_080ad1b0
	adds	r5, r0, #0
	ldr	r4, [sp, #4]
	cmp	r5, #0
	beq.n	.L_0804c8da
	adds	r1, r4, #0
	adds	r0, r7, #0
	bl	sub_080ad190
	bl	sub_080ad078
	movs	r3, #6
	adds	r5, r0, #0
	ldrb	r6, [r5, #8]
	movs	r1, #17
	str	r3, [sp, #0]
	movs	r2, #10
	movs	r3, #3
	movs	r0, #11
	bl	sub_08039260
	ldr	r2, [sp, #36]
	mov	r3, sl
	ldr	r1, [r2, #0]
	mov	fp, r0
	ldrh	r2, [r1, #6]
	mov	r0, r8
	ands	r3, r2
	ldr	r2, [pc, #60]
	orrs	r3, r2
	strh	r3, [r1, #6]
	movs	r3, #160
	lsls	r3, r3, #7
	adds	r3, #1
	str	r0, [sp, #0]
	adds	r1, r7, r3
	mov	r0, fp
	movs	r2, #0
	movs	r3, #0
	bl	sub_0803c378
	lsls	r0, r7, #2
	ldr	r4, [sp, #4]
	ldr	r3, [pc, #32]
	adds	r0, r0, r7
	lsls	r0, r0, #2
	adds	r0, r0, r4
	movs	r2, #16
	adds	r0, r0, r3
	mov	r1, fp
	movs	r3, #0
	bl	sub_08042010
	ldr	r1, [sp, #76]
	movs	r0, #1
	strh	r6, [r1, #12]
	bl	sub_08013560
	b.n	.L_0804c864
	.4byte 0x00000040
	.2byte 0x06d3
	.2byte 0x0000
.L_0804c864:
	movs	r0, #112
	bl	sub_081c0010
	adds	r0, r5, #0
	bl	sub_0804b87c
	adds	r2, r6, #0
	adds	r3, r0, #0
	ldrb	r1, [r5, #0]
	ldr	r0, [sp, #64]
	bl	sub_0804a85c
	ldr	r2, [sp, #36]
	adds	r6, r0, #0
	ldr	r3, [r2, #0]
	adds	r3, #216
	ldr	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0804c8c6
	bl	sub_0804519c
	movs	r1, #15
	adds	r0, #14
	movs	r2, #8
	bl	sub_080457b0
	adds	r5, r0, #0
	b.n	.L_0804c8a2
.L_0804c89c:
	movs	r0, #1
	bl	sub_08013560
.L_0804c8a2:
	bl	sub_0803a3b8
	cmp	r0, #0
	beq.n	.L_0804c89c
	movs	r1, #1
	adds	r0, r5, #0
	bl	sub_0803939c
	ldr	r3, [sp, #36]
	ldr	r1, [r3, #0]
	adds	r2, r1, #0
	adds	r2, #216
	ldr	r3, [r2, #0]
	adds	r1, #220
	adds	r3, #1
	str	r3, [r2, #0]
	movs	r3, #45
	str	r3, [r1, #0]
.L_0804c8c6:
	mov	r0, fp
	movs	r1, #1
	bl	sub_0803939c
	movs	r4, #1
	negs	r4, r4
	cmp	r6, r4
	bne.n	.L_0804c8d8
	b.n	.L_0804c776
.L_0804c8d8:
	b.n	.L_0804c4d4
.L_0804c8da:
	movs	r3, #6
	str	r3, [sp, #0]
	movs	r1, #17
	movs	r2, #10
	movs	r3, #3
	movs	r0, #11
	str	r4, [sp, #4]
	bl	sub_08039260
	mov	fp, r0
	ldr	r0, [sp, #36]
	mov	r3, sl
	ldr	r1, [r0, #0]
	movs	r0, #2
	ldrh	r2, [r1, #6]
	ands	r3, r2
	ldr	r2, [pc, #60]
	orrs	r3, r2
	strh	r3, [r1, #6]
	bl	sub_08041f70
	movs	r2, #160
	lsls	r2, r2, #7
	adds	r2, #1
	adds	r1, r7, r2
	mov	r0, fp
	movs	r2, #0
	movs	r3, #0
	str	r5, [sp, #0]
	bl	sub_0803c378
	lsls	r0, r7, #2
	ldr	r4, [sp, #4]
	ldr	r3, [pc, #28]
	adds	r0, r0, r7
	lsls	r0, r0, #2
	adds	r0, r0, r4
	mov	r1, fp
	movs	r2, #16
	adds	r0, r0, r3
	movs	r3, #0
	bl	sub_08042010
	movs	r0, #15
	bl	sub_08041f70
	b.n	.L_0804c940
	.4byte 0x00000040
	.2byte 0x06d3
	.2byte 0x0000
.L_0804c940:
	ldr	r4, [sp, #76]
	movs	r3, #1
	strh	r3, [r4, #12]
	movs	r0, #1
	bl	sub_08013560
	movs	r0, #112
	bl	sub_081c0010
	movs	r1, #4
	ldr	r0, [sp, #64]
	movs	r2, #0
	movs	r3, #7
	bl	sub_0804a85c
	movs	r1, #1
	adds	r6, r0, #0
	mov	r0, fp
	bl	sub_0803939c
	cmp	r6, r9
	bne.n	.L_0804c96e
	b.n	.L_0804c776
.L_0804c96e:
	b.n	.L_0804c4d4
	movs	r0, #112
	bl	sub_081c0010
	movs	r6, #0
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #228
	ldr	r3, [r3, #0]
	str	r6, [r3, #52]
	str	r6, [r3, #48]
	str	r6, [r3, #56]
.L_0804c986:
	ldr	r0, [sp, #36]
	ldr	r3, [pc, #52]
	ldr	r1, [r0, #0]
	movs	r4, #0
	ldrh	r2, [r1, #6]
	ands	r3, r2
	ldr	r2, [pc, #40]
	orrs	r3, r2
	strh	r3, [r1, #6]
	ldr	r0, [sp, #72]
	movs	r3, #0
	mov	r8, r3
	movs	r3, #216
	ldrh	r5, [r0, r3]
	movs	r2, #116
	adds	r2, r2, r1
	adds	r1, #150
	mov	r9, r2
	mov	sl, r1
	cmp	r5, #0
	beq.n	.L_0804c9f8
	adds	r3, r0, #0
	adds	r3, #216
	mov	r7, r9
	mov	r6, sl
	b.n	.L_0804c9c4
	movs	r0, r0
	.4byte 0x00000060
	.2byte 0xfe00
	.2byte 0xffff
.L_0804c9c4:
	adds	r0, r5, #0
	str	r3, [sp, #8]
	str	r4, [sp, #4]
	bl	sub_080ad010
	adds	r1, r5, #0
	ldr	r0, [sp, #64]
	bl	sub_080499b4
	ldr	r3, [sp, #8]
	ldr	r4, [sp, #4]
	cmp	r0, #0
	bne.n	.L_0804c9ea
	movs	r1, #1
	strh	r5, [r6, #0]
	add	r8, r1
	strb	r4, [r7, #0]
	adds	r6, #2
	adds	r7, #1
.L_0804c9ea:
	adds	r4, #1
	cmp	r4, #15
	beq.n	.L_0804c9f8
	adds	r3, #2
	ldrh	r5, [r3, #0]
	cmp	r5, #0
	bne.n	.L_0804c9c4
.L_0804c9f8:
	ldr	r2, [sp, #72]
	movs	r3, #216
	ldrh	r5, [r2, r3]
	movs	r4, #0
	cmp	r5, #0
	beq.n	.L_0804ca46
	mov	r0, r8
	mov	r7, r8
	lsls	r3, r0, #1
	mov	r1, sl
	adds	r2, #216
	add	r7, r9
	adds	r6, r3, r1
.L_0804ca12:
	adds	r0, r5, #0
	str	r2, [sp, #12]
	str	r4, [sp, #4]
	bl	sub_080ad010
	adds	r1, r5, #0
	ldr	r0, [sp, #64]
	bl	sub_080499b4
	ldr	r2, [sp, #12]
	ldr	r4, [sp, #4]
	cmp	r0, #0
	beq.n	.L_0804ca38
	movs	r3, #1
	strh	r5, [r6, #0]
	add	r8, r3
	strb	r4, [r7, #0]
	adds	r6, #2
	adds	r7, #1
.L_0804ca38:
	adds	r4, #1
	cmp	r4, #15
	beq.n	.L_0804ca46
	adds	r2, #2
	ldrh	r5, [r2, #0]
	cmp	r5, #0
	bne.n	.L_0804ca12
.L_0804ca46:
	ldr	r1, [pc, #36]
	mov	r4, r8
	lsls	r3, r4, #1
	mov	r0, sl
	strh	r1, [r3, r0]
	ldr	r0, [sp, #64]
	mov	r1, sl
	mov	r2, r8
	bl	sub_08049a30
	movs	r7, #1
	adds	r6, r0, #0
	negs	r7, r7
	cmp	r6, r7
	bne.n	.L_0804ca68
	bl	.L_0804c078
.L_0804ca68:
	b.n	.L_0804ca70
	movs	r0, r0
	.2byte 0x0000
	.2byte 0x0000
.L_0804ca70:
	mov	r2, r9
	ldrb	r6, [r2, r6]
	ldr	r3, [sp, #72]
	str	r6, [sp, #56]
	lsls	r6, r6, #1
	adds	r6, #216
	ldrh	r0, [r3, r6]
	bl	sub_080ad010
	ldrh	r0, [r0, #40]
	bl	sub_080ad078
	ldrb	r4, [r0, #8]
	mov	r8, r0
	ldr	r0, [sp, #36]
	mov	sl, r4
	ldr	r5, [r0, #0]
	movs	r0, #128
	bl	sub_080143e0
	movs	r3, #6
	str	r3, [sp, #0]
	mov	r9, r0
	movs	r1, #17
	movs	r2, #16
	movs	r3, #3
	movs	r0, #9
	bl	sub_08039260
	ldr	r1, [sp, #36]
	mov	fp, r0
	ldr	r0, [r1, #0]
	ldr	r1, [pc, #68]
	ldrh	r2, [r0, #6]
	adds	r3, r1, #0
	ands	r3, r2
	ldr	r2, [pc, #52]
	adds	r5, #12
	orrs	r3, r2
	strh	r3, [r0, #6]
	ldr	r3, [pc, #56]
	str	r3, [r5, #4]
	movs	r3, #0
	str	r3, [r5, #8]
	mov	r4, fp
	movs	r2, #12
	ldrsh	r3, [r4, r2]
	ldr	r4, [pc, #36]
	ldrh	r2, [r5, #6]
	lsls	r3, r3, #3
	adds	r3, #8
	ands	r3, r4
	ands	r1, r2
	orrs	r1, r3
	strh	r1, [r5, #6]
	mov	r1, fp
	movs	r0, #14
	ldrsh	r3, [r1, r0]
	mov	r1, r9
	lsls	r3, r3, #3
	adds	r3, #4
	strb	r3, [r5, #4]
	b.n	.L_0804cb00
	movs	r0, r0
	.4byte 0x00000030
	.4byte 0x000001ff
	.4byte 0xfffffe00
	.2byte 0x0400
	.2byte 0x4000
.L_0804cb00:
	ldr	r2, [sp, #72]
	str	r4, [sp, #4]
	ldrh	r0, [r2, r6]
	bl	sub_080454a0
	ldr	r3, [pc, #56]
	ldrh	r2, [r5, #8]
	ands	r0, r3
	ldr	r3, [pc, #52]
	ldr	r4, [sp, #4]
	ands	r3, r2
	orrs	r3, r0
	strh	r3, [r5, #8]
	ldr	r3, [sp, #36]
	ldr	r2, [r3, #0]
	movs	r3, #1
	adds	r2, #37
	strb	r3, [r2, #0]
	ldr	r1, [sp, #72]
	ldr	r3, [pc, #36]
	ldrh	r0, [r1, r6]
	movs	r2, #24
	ands	r0, r4
	mov	r1, fp
	adds	r0, r0, r3
	movs	r3, #0
	bl	sub_08042010
	ldr	r3, [sp, #76]
	mov	r2, sl
	strh	r2, [r3, #12]
	movs	r0, #112
	b.n	.L_0804cb50
	movs	r0, r0
	.4byte 0x000003ff
	.4byte 0xfffffc00
	.2byte 0x025f
	.2byte 0x0000
.L_0804cb50:
	bl	sub_081c0010
	mov	r0, r8
	bl	sub_0804b87c
	mov	r4, r8
	adds	r3, r0, #0
	ldrb	r1, [r4, #0]
	ldr	r0, [sp, #64]
	mov	r2, sl
	bl	sub_0804a85c
	adds	r6, r0, #0
	ldr	r0, [sp, #36]
	movs	r1, #0
	ldr	r3, [r0, #0]
	mov	r0, r9
	adds	r3, #37
	strb	r1, [r3, #0]
	bl	sub_08014274
	mov	r0, fp
	movs	r1, #1
	bl	sub_0803939c
	cmp	r6, r7
	bne.n	.L_0804cb88
	b.n	.L_0804c986
.L_0804cb88:
	movs	r2, #2
	str	r2, [sp, #60]
	b.n	.L_0804c4d4
	movs	r3, #3
	str	r3, [sp, #60]
