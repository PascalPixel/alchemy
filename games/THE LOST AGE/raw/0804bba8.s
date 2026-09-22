.syntax unified
	.thumb
	.set sub_08013164, 0x08013164
	.set sub_08013560, 0x08013560
	.set sub_080143e0, 0x080143e0
	.set sub_080145a8, 0x080145a8
	.set sub_08014d00, 0x08014d00
	.set sub_08014d78, 0x08014d78
	.set sub_08016ca4, 0x08016ca4
	.set sub_08016ce4, 0x08016ce4
	.set sub_0803aae4, 0x0803aae4
	.set sub_0803c9bc, 0x0803c9bc
	.set sub_0804297c, 0x0804297c
	.set sub_080451bc, 0x080451bc
	.set sub_08045330, 0x08045330
	.set sub_080453d0, 0x080453d0
	.set sub_08045528, 0x08045528
	.set sub_080455dc, 0x080455dc
	.set sub_080457d0, 0x080457d0
	.set sub_080461c8, 0x080461c8
	.set sub_08046b08, 0x08046b08
	.set sub_0804a85c, 0x0804a85c
	.set sub_0804b7c0, 0x0804b7c0
	.set sub_0804b8b8, 0x0804b8b8
	.set sub_0804babc, 0x0804babc
	.set sub_0804c078, 0x0804c078
	.set sub_0804cc30, 0x0804cc30
	.set sub_08118090, 0x08118090
	.set sub_081180e0, 0x081180e0
	.set sub_08118118, 0x08118118
	.set sub_08118130, 0x08118130
	.set sub_08118138, 0x08118138
	.set sub_08118158, 0x08118158
	.set sub_081c0010, 0x081c0010
	.global Overlay_0804bba8
Overlay_0804bba8:
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
	b.n	sub_0804c078
	.4byte 0x000003ff
	.4byte 0x80000400
	.4byte 0xfffffc00
	.4byte 0xfffffe00
