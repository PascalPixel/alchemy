.syntax unified
	.thumb
	.set sub_080022fc, 0x080022fc
	.set sub_080030f8, 0x080030f8
	.set sub_08015080, 0x08015080
	.set sub_08015270, 0x08015270
	.set sub_08077008, 0x08077008
	.set sub_080770c0, 0x080770c0
	.set sub_080a1804, 0x080a1804
	.set sub_080a1870, 0x080a1870
	.set sub_080a195c, 0x080a195c
	.set sub_080a1a40, 0x080a1a40
	.set sub_080a2144, 0x080a2144
	.set sub_080a3d24, 0x080a3d24
	.set sub_080a68a8, 0x080a68a8
	.set sub_080a68ec, 0x080a68ec
	.set sub_080a7850, 0x080a7850
	.set sub_080a7f44, 0x080a7f44
	.set sub_080a8088, 0x080a8088
	.set sub_080a9b94, 0x080a9b94
	.set sub_080f9010, 0x080f9010
	.global Overlay_080a7a34
Overlay_080a7a34:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #112]
	ldr	r3, [r3, #0]
	movs	r1, #28
	ldrsb	r1, [r3, r1]
	movs	r2, #30
	ldrsb	r2, [r3, r2]
	mov	r8, r3
	sub	sp, #12
	movs	r3, #1
	str	r3, [sp, #8]
	mov	sl, r1
	movs	r3, #136
	movs	r1, #0
	str	r1, [sp, #4]
	lsls	r3, r3, #2
	add	r3, r8
	ldrh	r3, [r3, #0]
	mov	fp, r2
	movs	r1, #130
	mov	r2, sl
	lsls	r1, r1, #2
	lsls	r7, r2, #1
	mov	r9, r3
	mov	r2, r8
	adds	r3, r7, r1
	ldrh	r0, [r2, r3]
	bl	sub_08077008
	movs	r3, #141
	lsls	r3, r3, #2
	ldr	r0, [pc, #48]
	add	r3, r8
	movs	r1, #130
	movs	r2, #3
.L_080a7a86:
	subs	r2, #1
	strh	r1, [r3, #0]
	strh	r0, [r3, #8]
	adds	r1, #32
	adds	r3, #2
	cmp	r2, #0
	bge.n	.L_080a7a86
	movs	r0, #14
	bl	sub_080a2144
	movs	r1, #160
	ldr	r3, [pc, #24]
	ldr	r0, [pc, #28]
	lsls	r1, r1, #19
	ldr	r2, [pc, #28]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r1, #28
	ldr	r0, [pc, #24]
	ldr	r2, [pc, #24]
	b.n	.L_080a7acc
	.4byte 0x00000080
	.4byte 0x03001f2c
	.4byte 0x040000d4
	.4byte 0x05000200
	.4byte 0x80000010
	.4byte 0x050001c8
	.2byte 0x0001
	.2byte 0x8000
.L_080a7acc:
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r1, #4
	ldr	r0, [pc, #192]
	ldr	r2, [pc, #192]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r1, #28
	ldr	r0, [pc, #188]
	ldr	r2, [pc, #192]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	b.n	.L_080a7d26
.L_080a7ae6:
	ldr	r3, [sp, #8]
	cmp	r3, #0
	beq.n	.L_080a7bac
	movs	r1, #0
	movs	r5, #134
	str	r1, [sp, #8]
	lsls	r5, r5, #1
	add	r5, r8
	ldr	r0, [r5, #0]
	bl	sub_08015270
	ldr	r6, [pc, #164]
	ldr	r1, [r5, #0]
	adds	r0, r6, #0
	movs	r2, #0
	movs	r3, #0
	bl	sub_08015080
	movs	r0, #48
	bl	sub_080770c0
	cmp	r0, #0
	beq.n	.L_080a7b20
	ldr	r1, [r5, #0]
	ldr	r0, [pc, #144]
	movs	r2, #0
	movs	r3, #16
	bl	sub_08015080
.L_080a7b20:
	ldr	r1, [r5, #0]
	subs	r0, r6, #3
	movs	r2, #0
	movs	r3, #8
	bl	sub_08015080
	mov	r0, sl
	mov	r1, fp
	add	r0, fp
	bl	sub_080022fc
	movs	r2, #130
	lsls	r2, r2, #2
	lsls	r7, r0, #1
	adds	r5, r7, r2
	mov	r3, r8
	mov	sl, r0
	ldrh	r0, [r3, r5]
	bl	sub_08077008
	mov	r0, r9
	movs	r1, #3
	adds	r0, #3
	bl	sub_080022fc
	mov	r1, r8
	mov	r9, r0
	ldrh	r0, [r1, r5]
	mov	r1, r9
	bl	sub_080a8088
	mov	r2, r8
	ldrh	r1, [r2, r5]
	mov	r0, r8
	bl	sub_080a1804
	movs	r3, #169
	lsls	r3, r3, #1
	ldr	r1, [pc, #28]
	adds	r6, r7, #0
	movs	r2, #7
	add	r3, r8
.L_080a7b74:
	subs	r2, #1
	strh	r1, [r3, #0]
	subs	r3, #2
	cmp	r2, #0
	bge.n	.L_080a7b74
	movs	r3, #162
	lsls	r3, r3, #1
	adds	r2, r6, r3
	ldr	r3, [pc, #8]
	mov	r1, r8
	strh	r3, [r1, r2]
	b.n	.L_080a7bb0
	.4byte 0x0000001e
	.4byte 0x0000001a
	.4byte 0x05000200
	.4byte 0x80000010
	.4byte 0x050001e8
	.4byte 0x80000001
	.4byte 0x00000b0d
	.2byte 0x0b16
	.2byte 0x0000
.L_080a7bac:
	mov	r2, sl
	lsls	r7, r2, #1
.L_080a7bb0:
	mov	r3, sl
	adds	r0, r7, r3
	lsls	r0, r0, #3
	movs	r1, #16
	subs	r0, #10
	bl	sub_080a1a40
	movs	r0, #1
	bl	sub_080030f8
	ldr	r1, [pc, #136]
	ldr	r2, [r1, #0]
	movs	r3, #1
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_080a7bdc
	movs	r0, #112
	bl	sub_080f9010
	movs	r1, #1
	str	r1, [sp, #4]
	b.n	.L_080a7d38
.L_080a7bdc:
	ldr	r6, [r1, #0]
	movs	r3, #2
	ands	r6, r3
	cmp	r6, #0
	beq.n	.L_080a7bf4
	movs	r0, #113
	bl	sub_080f9010
	movs	r2, #1
	negs	r2, r2
	str	r2, [sp, #4]
	b.n	.L_080a7d38
.L_080a7bf4:
	ldr	r7, [pc, #92]
	movs	r3, #128
	ldr	r5, [r7, #0]
	lsls	r3, r3, #1
	ands	r5, r3
	cmp	r5, #0
	beq.n	.L_080a7c58
	mov	r0, sl
	movs	r1, #1
	bl	sub_080a7f44
	cmp	r0, #0
	beq.n	.L_080a7cb6
	movs	r0, #112
	bl	sub_080f9010
	movs	r3, #1
	add	sl, r3
	bl	sub_080a195c
	mov	r1, r8
	ldr	r0, [r1, #16]
	movs	r2, #2
	movs	r1, #2
	movs	r3, #8
	str	r6, [sp, #0]
	bl	sub_080a1870
	movs	r3, #169
	lsls	r3, r3, #1
	ldr	r1, [pc, #24]
	movs	r2, #7
	add	r3, r8
.L_080a7c36:
	subs	r2, #1
	strh	r1, [r3, #0]
	subs	r3, #2
	cmp	r2, #0
	bge.n	.L_080a7c36
	mov	r2, sl
	movs	r3, #162
	lsls	r3, r3, #1
	lsls	r7, r2, #1
	b.n	.L_080a7cac
	movs	r0, r0
	.4byte 0x0000001e
	.4byte 0x03001c94
	.2byte 0x1b04
	.2byte 0x0300
.L_080a7c58:
	ldr	r2, [r7, #0]
	movs	r3, #128
	lsls	r3, r3, #2
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_080a7ccc
	mov	r0, sl
	movs	r1, #0
	bl	sub_080a7f44
	cmp	r0, #0
	beq.n	.L_080a7cb6
	movs	r0, #112
	bl	sub_080f9010
	movs	r2, #1
	negs	r2, r2
	add	sl, r2
	bl	sub_080a195c
	mov	r3, r8
	ldr	r0, [r3, #16]
	movs	r1, #2
	movs	r2, #2
	movs	r3, #8
	str	r5, [sp, #0]
	bl	sub_080a1870
	movs	r3, #169
	lsls	r3, r3, #1
	ldr	r1, [pc, #44]
	movs	r2, #7
	add	r3, r8
.L_080a7c9a:
	subs	r2, #1
	strh	r1, [r3, #0]
	subs	r3, #2
	cmp	r2, #0
	bge.n	.L_080a7c9a
	mov	r1, sl
	movs	r3, #162
	lsls	r7, r1, #1
	lsls	r3, r3, #1
.L_080a7cac:
	adds	r2, r7, r3
	ldr	r3, [pc, #24]
	mov	r1, r8
	strh	r3, [r1, r2]
	b.n	.L_080a7cbc
.L_080a7cb6:
	movs	r0, #114
	bl	sub_080f9010
.L_080a7cbc:
	movs	r0, #1
	bl	sub_080030f8
	b.n	.L_080a7d26
	.4byte 0x0000001e
	.2byte 0x001a
	.2byte 0x0000
.L_080a7ccc:
	ldr	r2, [r1, #0]
	movs	r3, #4
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_080a7cea
	movs	r0, #48
	bl	sub_080770c0
	cmp	r0, #0
	beq.n	.L_080a7cea
	bl	sub_080a7850
	movs	r2, #1
	str	r2, [sp, #8]
	b.n	.L_080a7d26
.L_080a7cea:
	ldr	r2, [r7, #0]
	movs	r3, #32
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_080a7d0a
	movs	r0, #111
	bl	sub_080f9010
	mov	r3, fp
	cmp	r3, #1
	ble.n	.L_080a7d0a
	movs	r1, #1
	movs	r2, #1
	negs	r1, r1
	str	r2, [sp, #8]
	add	sl, r1
.L_080a7d0a:
	ldr	r2, [r7, #0]
	movs	r3, #16
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_080a7d26
	movs	r0, #111
	bl	sub_080f9010
	mov	r3, fp
	cmp	r3, #1
	ble.n	.L_080a7d26
	movs	r1, #1
	str	r1, [sp, #8]
	add	sl, r1
.L_080a7d26:
	movs	r0, #168
	lsls	r0, r0, #1
	bl	sub_080770c0
	cmp	r0, #0
	bne.n	.L_080a7d34
	b.n	.L_080a7ae6
.L_080a7d34:
	mov	r2, sl
	lsls	r7, r2, #1
.L_080a7d38:
	mov	r1, r8
	mov	r3, sl
	strb	r3, [r1, #28]
	movs	r3, #130
	lsls	r3, r3, #2
	adds	r2, r7, r3
	ldrh	r3, [r1, r2]
	str	r3, [r1, #8]
	ldr	r3, [pc, #24]
	ldrh	r2, [r1, r2]
	add	r3, r8
	strb	r2, [r3, #0]
	ldr	r0, [sp, #4]
	add	sp, #12
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.2byte 0x021a
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #232]
	ldr	r7, [r3, #0]
	movs	r0, #28
	ldrsb	r0, [r7, r0]
	sub	sp, #8
	movs	r2, #1
	mov	r8, r0
	movs	r0, #136
	movs	r1, #30
	ldrsb	r1, [r7, r1]
	lsls	r0, r0, #2
	str	r2, [sp, #4]
	adds	r3, r7, r0
	ldrh	r3, [r3, #0]
	mov	r9, r1
	mov	r1, r8
	str	r3, [sp, #0]
	lsls	r1, r1, #1
	movs	r3, #130
	mov	sl, r1
	lsls	r3, r3, #2
	add	r3, sl
	ldrh	r0, [r7, r3]
	bl	sub_08077008
	movs	r2, #134
	lsls	r2, r2, #1
	adds	r6, r7, r2
	ldr	r0, [r6, #0]
	bl	sub_08015270
	ldr	r5, [pc, #172]
	ldr	r1, [r6, #0]
	adds	r0, r5, #0
	movs	r2, #0
	movs	r3, #0
	adds	r5, #1
	bl	sub_08015080
	movs	r3, #16
	ldr	r1, [r6, #0]
	adds	r0, r5, #0
	movs	r2, #0
	bl	sub_08015080
	movs	r3, #228
	lsls	r3, r3, #1
	adds	r3, r3, r7
	mov	fp, r3
.L_080a7dd8:
	ldr	r0, [sp, #4]
	cmp	r0, #0
	beq.n	.L_080a7e84
	movs	r1, #0
	mov	r0, r8
	str	r1, [sp, #4]
	add	r0, r9
	mov	r1, r9
	bl	sub_080022fc
	mov	r8, r0
	mov	r2, r8
	lsls	r2, r2, #1
	movs	r5, #130
	mov	sl, r2
	lsls	r5, r5, #2
	add	r5, sl
	ldrh	r0, [r7, r5]
	bl	sub_08077008
	ldr	r0, [sp, #0]
	movs	r1, #3
	adds	r0, #3
	bl	sub_080022fc
	str	r0, [sp, #0]
	ldr	r1, [sp, #0]
	ldrh	r0, [r7, r5]
	bl	sub_080a8088
	ldrh	r1, [r7, r5]
	adds	r0, r7, #0
	bl	sub_080a1804
	movs	r0, #169
	lsls	r0, r0, #1
	ldr	r1, [pc, #52]
	mov	r6, sl
	movs	r2, #7
	adds	r3, r7, r0
.L_080a7e28:
	subs	r2, #1
	strh	r1, [r3, #0]
	subs	r3, #2
	cmp	r2, #0
	bge.n	.L_080a7e28
	movs	r1, #162
	lsls	r1, r1, #1
	ldr	r3, [pc, #36]
	adds	r2, r6, r1
	strh	r3, [r7, r2]
	movs	r2, #130
	lsls	r2, r2, #2
	adds	r3, r6, r2
	ldrh	r0, [r7, r3]
	bl	sub_08077008
	movs	r2, #0
	mov	r1, fp
	bl	sub_080a68ec
	movs	r1, #134
	lsls	r1, r1, #2
	b.n	.L_080a7e68
	movs	r0, r0
	.4byte 0x0000001e
	.4byte 0x0000001a
	.4byte 0x03001f2c
	.2byte 0x0c05
	.2byte 0x0000
.L_080a7e68:
	adds	r3, r7, r1
	strb	r0, [r3, #0]
	mov	r0, fp
	bl	sub_080a68a8
	movs	r0, #96
	movs	r1, #96
	movs	r2, #8
	bl	sub_080a9b94
	mov	r0, fp
	bl	sub_080a3d24
	b.n	.L_080a7e8a
.L_080a7e84:
	mov	r2, r8
	lsls	r2, r2, #1
	mov	sl, r2
.L_080a7e8a:
	mov	r0, sl
	add	r0, r8
	lsls	r0, r0, #3
	movs	r1, #16
	subs	r0, #10
	bl	sub_080a1a40
	movs	r0, #1
	bl	sub_080030f8
	ldr	r1, [pc, #152]
	ldr	r2, [r1, #0]
	movs	r3, #1
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_080a7eb4
	movs	r0, #112
	bl	sub_080f9010
	movs	r0, #1
	b.n	.L_080a7f0e
.L_080a7eb4:
	ldr	r2, [r1, #0]
	movs	r3, #2
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_080a7eca
	movs	r0, #113
	bl	sub_080f9010
	movs	r0, #1
	negs	r0, r0
	b.n	.L_080a7f0e
.L_080a7eca:
	ldr	r5, [pc, #112]
	ldr	r2, [r5, #0]
	movs	r3, #32
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_080a7eec
	movs	r0, #111
	bl	sub_080f9010
	mov	r3, r9
	cmp	r3, #1
	ble.n	.L_080a7eec
	movs	r0, #1
	movs	r1, #1
	negs	r0, r0
	str	r1, [sp, #4]
	add	r8, r0
.L_080a7eec:
	ldr	r2, [r5, #0]
	movs	r3, #16
	ands	r2, r3
	cmp	r2, #0
	bne.n	.L_080a7ef8
	b.n	.L_080a7dd8
.L_080a7ef8:
	movs	r0, #111
	bl	sub_080f9010
	mov	r2, r9
	cmp	r2, #1
	bgt.n	.L_080a7f06
	b.n	.L_080a7dd8
.L_080a7f06:
	movs	r3, #1
	add	r8, r3
	str	r3, [sp, #4]
	b.n	.L_080a7dd8
.L_080a7f0e:
	mov	r1, r8
	movs	r2, #130
	strb	r1, [r7, #28]
	lsls	r2, r2, #2
	add	r2, sl
	ldrh	r3, [r7, r2]
	str	r3, [r7, #8]
	ldr	r1, [pc, #32]
	ldrh	r2, [r7, r2]
	adds	r3, r7, r1
	strb	r2, [r3, #0]
	add	sp, #8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001c94
	.4byte 0x03001b04
	.4byte 0x0000021a
