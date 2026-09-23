.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_08013164, 0x08013164
	.set sub_08013560, 0x08013560
	.set sub_08013e70, 0x08013e70
	.set sub_08014878, 0x08014878
	.set sub_08014dac, 0x08014dac
	.set sub_08016ca4, 0x08016ca4
	.set sub_08020090, 0x08020090
	.set sub_08020098, 0x08020098
	.set sub_08038120, 0x08038120
	.set sub_080381c8, 0x080381c8
	.set sub_080ad000, 0x080ad000
	.set sub_080ad008, 0x080ad008
	.set sub_080ad0c0, 0x080ad0c0
	.set sub_080ad0c8, 0x080ad0c8
	.set sub_080ad148, 0x080ad148
	.set sub_080ad158, 0x080ad158
	.set sub_080ad160, 0x080ad160
	.set sub_080ad168, 0x080ad168
	.set sub_080ad170, 0x080ad170
	.set sub_080ad1b0, 0x080ad1b0
	.set sub_080ad1b8, 0x080ad1b8
	.set sub_0811a0b0, 0x0811a0b0
	.set sub_0811a188, 0x0811a188
	.set sub_0811a31c, 0x0811a31c
	.set sub_0811b4d8, 0x0811b4d8
	.set sub_0811b6a4, 0x0811b6a4
	.set sub_0811be3c, 0x0811be3c
	.set sub_0811ff08, 0x0811ff08
	.set sub_081201c4, 0x081201c4
	.set sub_08120360, 0x08120360
	.set sub_081203a8, 0x081203a8
	.set sub_08122c88, 0x08122c88
	.set sub_081234a4, 0x081234a4
	.set sub_081234f0, 0x081234f0
	.set sub_08123534, 0x08123534
	.set sub_08124854, 0x08124854
	.set sub_081248b4, 0x081248b4
	.set sub_08124914, 0x08124914
	.set sub_08124974, 0x08124974
	.set sub_081249b0, 0x081249b0
	.set sub_081249ec, 0x081249ec
	.set sub_08124a28, 0x08124a28
	.set sub_08124a64, 0x08124a64
	.set sub_08124aa0, 0x08124aa0
	.set sub_08124af8, 0x08124af8
	.set sub_08124ba8, 0x08124ba8
	.set sub_08124bcc, 0x08124bcc
	.set sub_0812628c, 0x0812628c
	.set sub_08127308, 0x08127308
	.set sub_081c0010, 0x081c0010
	.global Func_08124cc4
	.thumb_func
Func_08124cc4:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	movs	r2, #0
	mov	r9, r2
	bl	sub_080ad000
	movs	r2, #8
	adds	r3, r0, #0
	adds	r2, r2, r3
	mov	r8, r2
	movs	r2, #0
	mov	sl, r2
	movs	r2, #148
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldr	r3, [r3, #0]
	cmp	r9, r3
	bge.n	.L_08124d2c
	mov	r5, r8
.L_08124cf0:
	movs	r3, #3
	ldrsb	r3, [r5, r3]
	cmp	r3, #0
	ble.n	.L_08124d1a
	ldrb	r0, [r5, #2]
	bl	sub_08123534
	cmp	r0, #0
	beq.n	.L_08124d1a
	ldrb	r0, [r5, #2]
	bl	sub_08016ca4
	movs	r2, #56
	ldrsh	r3, [r0, r2]
	cmp	r3, #0
	beq.n	.L_08124d1a
	ldrb	r3, [r5, #3]
	subs	r3, #1
	strb	r3, [r5, #3]
	movs	r3, #1
	mov	r9, r3
.L_08124d1a:
	movs	r3, #144
	lsls	r3, r3, #1
	add	r3, r8
	ldr	r3, [r3, #0]
	movs	r2, #1
	add	sl, r2
	adds	r5, #4
	cmp	sl, r3
	blt.n	.L_08124cf0
.L_08124d2c:
	movs	r3, #0
	mov	sl, r3
	movs	r3, #144
	lsls	r3, r3, #1
	add	r3, r8
	ldr	r3, [r3, #0]
	cmp	sl, r3
	bge.n	.L_08124e10
	mov	r6, r8
.L_08124d3e:
	movs	r3, #3
	ldrsb	r3, [r6, r3]
	cmp	r3, #0
	bne.n	.L_08124dfe
	ldrb	r5, [r6, #2]
	adds	r0, r5, #0
	bl	sub_08123534
	cmp	r0, #0
	beq.n	.L_08124e04
	movs	r2, #2
	mov	r9, r2
	bl	sub_081234a4
	movs	r0, #30
	bl	sub_08122c88
	movs	r0, #0
	adds	r1, r5, #0
	bl	sub_08120360
	ldrb	r3, [r6, #0]
	movs	r0, #3
	lsls	r1, r3, #2
	adds	r1, r1, r3
	ldrb	r3, [r6, #1]
	lsls	r1, r1, #2
	adds	r1, r1, r3
	movs	r3, #150
	lsls	r3, r3, #1
	adds	r1, r1, r3
	bl	sub_08120360
	movs	r0, #14
	movs	r1, #175
	bl	sub_08120360
	movs	r0, #10
	movs	r1, #0
	bl	sub_08120360
	movs	r0, #4
	ldr	r1, [pc, #136]
	bl	sub_08120360
	movs	r0, #11
	adds	r1, r5, #0
	bl	sub_08120360
	adds	r0, r5, #0
	bl	sub_08123534
	cmp	r0, #0
	beq.n	.L_08124dc6
	adds	r0, r5, #0
	bl	sub_0811be3c
	movs	r1, #3
	ldr	r0, [r0, #0]
	bl	sub_08020090
	adds	r0, r5, #0
	bl	sub_0811be3c
	movs	r1, #32
	ldr	r0, [r0, #0]
	bl	sub_08020098
.L_08124dc6:
	ldrb	r7, [r6, #0]
	ldrb	r2, [r6, #1]
	adds	r1, r7, #0
	adds	r0, r5, #0
	bl	sub_080ad158
	ldrb	r1, [r6, #0]
	ldrb	r2, [r6, #1]
	adds	r0, r5, #0
	bl	sub_080ad168
	adds	r0, r5, #0
	bl	sub_080ad008
	adds	r0, r5, #0
	bl	sub_08123534
	cmp	r0, #0
	beq.n	.L_08124df8
	adds	r0, r5, #0
	adds	r1, r7, #0
	movs	r2, #3
	movs	r3, #0
	bl	sub_08127308
.L_08124df8:
	bl	sub_081234f0
	b.n	.L_08124e04
.L_08124dfe:
	movs	r2, #1
	adds	r6, #4
	add	sl, r2
.L_08124e04:
	movs	r3, #144
	lsls	r3, r3, #1
	add	r3, r8
	ldr	r3, [r3, #0]
	cmp	sl, r3
	blt.n	.L_08124d3e
.L_08124e10:
	mov	r0, r9
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.2byte 0x0cf7
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #36]
	sub	sp, #52
	str	r3, [sp, #8]
	adds	r3, #68
	str	r3, [sp, #4]
	movs	r1, #1
	ldrb	r2, [r3, #0]
	negs	r3, r2
	orrs	r3, r2
	lsrs	r3, r3, #31
	mov	fp, r3
	movs	r2, #0
	add	fp, r1
	mov	r9, r2
	cmp	r9, fp
	blt.n	.L_08124e54
	b.n	.L_08124f9a
.L_08124e54:
	mov	r0, r9
	bl	sub_080ad000
	movs	r2, #148
	adds	r3, r0, #0
	movs	r5, #8
	lsls	r2, r2, #1
	adds	r5, r5, r3
	adds	r3, r3, r2
	ldr	r3, [r3, #0]
	movs	r1, #0
	mov	r8, r1
	mov	sl, r5
	cmp	r8, r3
	bge.n	.L_08124eaa
.L_08124e72:
	movs	r3, #3
	ldrsb	r3, [r5, r3]
	cmp	r3, #0
	ble.n	.L_08124e98
	ldrb	r0, [r5, #2]
	bl	sub_0811be3c
	cmp	r0, #0
	beq.n	.L_08124e98
	ldrb	r0, [r5, #2]
	bl	sub_08016ca4
	movs	r1, #56
	ldrsh	r3, [r0, r1]
	cmp	r3, #0
	beq.n	.L_08124e98
	ldrb	r3, [r5, #3]
	subs	r3, #1
	strb	r3, [r5, #3]
.L_08124e98:
	movs	r3, #144
	lsls	r3, r3, #1
	add	r3, sl
	ldr	r3, [r3, #0]
	movs	r2, #1
	add	r8, r2
	adds	r5, #4
	cmp	r8, r3
	blt.n	.L_08124e72
.L_08124eaa:
	movs	r3, #0
	mov	r8, r3
	movs	r3, #144
	lsls	r3, r3, #1
	add	r3, sl
	ldr	r3, [r3, #0]
	cmp	r8, r3
	bge.n	.L_08124f90
	mov	r6, sl
.L_08124ebc:
	movs	r3, #3
	ldrsb	r3, [r6, r3]
	cmp	r3, #0
	bne.n	.L_08124f7e
	ldrb	r5, [r6, #2]
	adds	r0, r5, #0
	bl	sub_0811be3c
	cmp	r0, #0
	beq.n	.L_08124f84
	bl	sub_081234a4
	movs	r0, #30
	bl	sub_08122c88
	movs	r0, #0
	adds	r1, r5, #0
	bl	sub_08120360
	ldrb	r3, [r6, #0]
	movs	r2, #150
	lsls	r1, r3, #2
	adds	r1, r1, r3
	ldrb	r3, [r6, #1]
	lsls	r1, r1, #2
	adds	r1, r1, r3
	lsls	r2, r2, #1
	adds	r1, r1, r2
	movs	r0, #3
	bl	sub_08120360
	movs	r0, #14
	movs	r1, #175
	bl	sub_08120360
	movs	r0, #10
	movs	r1, #0
	bl	sub_08120360
	movs	r0, #4
	ldr	r1, [pc, #508]
	bl	sub_08120360
	movs	r0, #11
	adds	r1, r5, #0
	bl	sub_08120360
	movs	r0, #212
	bl	sub_081c0010
	adds	r0, r5, #0
	bl	sub_08123534
	cmp	r0, #0
	beq.n	.L_08124f46
	adds	r0, r5, #0
	bl	sub_0811be3c
	movs	r1, #3
	ldr	r0, [r0, #0]
	bl	sub_08020090
	adds	r0, r5, #0
	bl	sub_0811be3c
	movs	r1, #32
	ldr	r0, [r0, #0]
	bl	sub_08020098
.L_08124f46:
	ldrb	r7, [r6, #0]
	ldrb	r2, [r6, #1]
	adds	r1, r7, #0
	adds	r0, r5, #0
	bl	sub_080ad158
	ldrb	r1, [r6, #0]
	ldrb	r2, [r6, #1]
	adds	r0, r5, #0
	bl	sub_080ad168
	adds	r0, r5, #0
	bl	sub_080ad008
	adds	r0, r5, #0
	bl	sub_08123534
	cmp	r0, #0
	beq.n	.L_08124f78
	adds	r0, r5, #0
	adds	r1, r7, #0
	movs	r2, #3
	movs	r3, #0
	bl	sub_08127308
.L_08124f78:
	bl	sub_081234f0
	b.n	.L_08124f84
.L_08124f7e:
	movs	r3, #1
	adds	r6, #4
	add	r8, r3
.L_08124f84:
	movs	r3, #144
	lsls	r3, r3, #1
	add	r3, sl
	ldr	r3, [r3, #0]
	cmp	r8, r3
	blt.n	.L_08124ebc
.L_08124f90:
	movs	r5, #1
	add	r9, r5
	cmp	r9, fp
	bge.n	.L_08124f9a
	b.n	.L_08124e54
.L_08124f9a:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #36]
	movs	r1, #206
	lsls	r1, r1, #3
	adds	r3, r3, r1
	ldrh	r1, [r3, #0]
	movs	r2, #0
	movs	r0, #2
	bl	sub_0812628c
	ldr	r3, [pc, #348]
	ldr	r2, [sp, #4]
	ldr	r4, [r3, #4]
	ldr	r3, [r3, #0]
	str	r3, [sp, #12]
	str	r4, [sp, #16]
	ldrb	r3, [r2, #0]
	cmp	r3, #0
	beq.n	.L_08124fd8
	ldr	r3, [sp, #8]
	adds	r3, #80
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_08124fd8
	movs	r3, #2
	str	r3, [sp, #12]
	add	r0, sp, #12
	movs	r3, #1
	str	r3, [r0, #4]
	b.n	.L_08124fda
.L_08124fd8:
	add	r0, sp, #12
.L_08124fda:
	movs	r5, #1
	movs	r3, #20
	str	r5, [sp, #0]
	add	r3, sp
	mov	fp, r3
	mov	r9, r0
.L_08124fe6:
	mov	r1, r9
	ldr	r0, [r1, #0]
	mov	r1, fp
	bl	sub_0811a31c
	mov	r2, r9
	ldr	r3, [r2, #0]
	mov	r8, r0
	cmp	r3, #1
	bne.n	.L_08125004
	lsls	r0, r0, #1
	add	r0, fp
	bl	sub_0811a0b0
	add	r8, r0
.L_08125004:
	movs	r3, #0
	mov	sl, r3
	cmp	sl, r8
	blt.n	.L_0812500e
	b.n	.L_0812533e
.L_0812500e:
	mov	r5, sl
	lsls	r3, r5, #1
	mov	r1, fp
	ldrh	r6, [r1, r3]
	movs	r5, #162
	adds	r0, r6, #0
	bl	sub_08016ca4
	movs	r3, #68
	adds	r7, r0, #0
	adds	r3, #255
	adds	r2, r7, r3
	movs	r3, #0
	strb	r3, [r2, #0]
	lsls	r5, r5, #1
	adds	r1, r7, r5
	ldrb	r2, [r1, #0]
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L_0812503a
	adds	r3, #255
	strb	r3, [r1, #0]
.L_0812503a:
	movs	r3, #56
	ldrsh	r2, [r7, r3]
	ldrh	r1, [r7, #56]
	cmp	r2, #0
	beq.n	.L_08125130
	adds	r0, r7, #0
	adds	r0, #68
	ldrb	r3, [r0, #0]
	cmp	r3, #0
	beq.n	.L_081250c0
	movs	r5, #52
	ldrsh	r3, [r7, r5]
	ldrh	r4, [r7, #52]
	cmp	r2, r3
	beq.n	.L_081250c0
	movs	r2, #165
	lsls	r2, r2, #1
	adds	r3, r7, r2
	ldrh	r3, [r3, #0]
	ldrb	r5, [r0, #0]
	cmp	r3, #91
	blt.n	.L_08125074
	cmp	r3, #97
	ble.n	.L_0812506e
	cmp	r3, #218
	bne.n	.L_08125074
.L_0812506e:
	lsls	r3, r5, #2
	adds	r3, r3, r5
	lsls	r5, r3, #1
.L_08125074:
	lsls	r3, r1, #16
	asrs	r1, r3, #16
	lsls	r3, r4, #16
	adds	r2, r1, r5
	asrs	r3, r3, #16
	cmp	r2, r3
	ble.n	.L_08125084
	subs	r5, r3, r1
.L_08125084:
	adds	r1, r5, #0
	adds	r0, r6, #0
	bl	sub_080ad0c0
	adds	r0, r6, #0
	movs	r1, #1
	bl	sub_08038120
	adds	r0, r5, #0
	movs	r1, #5
	bl	sub_08038120
	movs	r3, #56
	ldrsh	r2, [r7, r3]
	movs	r5, #52
	ldrsh	r3, [r7, r5]
	cmp	r2, r3
	bne.n	.L_081250b0
	ldr	r0, [pc, #104]
	bl	sub_080381c8
	b.n	.L_081250b6
.L_081250b0:
	ldr	r0, [pc, #100]
	bl	sub_080381c8
.L_081250b6:
	movs	r0, #175
	bl	sub_081c0010
	bl	sub_0811ff08
.L_081250c0:
	adds	r0, r7, #0
	adds	r0, #69
	ldrb	r3, [r0, #0]
	cmp	r3, #0
	beq.n	.L_08125130
	movs	r2, #58
	ldrsh	r1, [r7, r2]
	movs	r3, #54
	ldrsh	r2, [r7, r3]
	cmp	r1, r2
	beq.n	.L_08125130
	ldrb	r5, [r0, #0]
	adds	r3, r1, r5
	cmp	r3, r2
	ble.n	.L_081250e0
	subs	r5, r2, r1
.L_081250e0:
	adds	r1, r5, #0
	adds	r0, r6, #0
	bl	sub_080ad0c8
	adds	r0, r6, #0
	movs	r1, #1
	bl	sub_08038120
	adds	r0, r5, #0
	movs	r1, #5
	bl	sub_08038120
	movs	r5, #58
	ldrsh	r2, [r7, r5]
	movs	r1, #54
	ldrsh	r3, [r7, r1]
	cmp	r2, r3
	bne.n	.L_08125120
	ldr	r0, [pc, #20]
	bl	sub_080381c8
	b.n	.L_08125126
	.4byte 0x00000cf7
	.4byte 0x081297c4
	.4byte 0x00000c6c
	.4byte 0x00000c69
	.2byte 0x0c6d
	.2byte 0x0000
.L_08125120:
	ldr	r0, [pc, #568]
	bl	sub_080381c8
.L_08125126:
	movs	r0, #175
	bl	sub_081c0010
	bl	sub_0811ff08
.L_08125130:
	adds	r0, r6, #0
	bl	sub_08124bcc
	cmp	r0, #0
	beq.n	.L_0812515a
	adds	r0, r6, #0
	bl	sub_0811be3c
	adds	r1, r0, #0
	adds	r0, r6, #0
	bl	sub_0811b4d8
	adds	r0, r6, #0
	movs	r1, #1
	bl	sub_08038120
	ldr	r0, [pc, #524]
	bl	sub_080381c8
	bl	sub_0811ff08
.L_0812515a:
	adds	r0, r6, #0
	bl	sub_08124854
	cmp	r0, #0
	beq.n	.L_08125184
	adds	r0, r6, #0
	bl	sub_0811be3c
	adds	r1, r0, #0
	adds	r0, r6, #0
	bl	sub_0811b4d8
	adds	r0, r6, #0
	movs	r1, #1
	bl	sub_08038120
	ldr	r0, [pc, #488]
	bl	sub_080381c8
	bl	sub_0811ff08
.L_08125184:
	adds	r0, r6, #0
	bl	sub_081248b4
	cmp	r0, #0
	beq.n	.L_081251ae
	adds	r0, r6, #0
	bl	sub_0811be3c
	adds	r1, r0, #0
	adds	r0, r6, #0
	bl	sub_0811b4d8
	adds	r0, r6, #0
	movs	r1, #1
	bl	sub_08038120
	ldr	r0, [pc, #448]
	bl	sub_080381c8
	bl	sub_0811ff08
.L_081251ae:
	adds	r0, r6, #0
	bl	sub_08124914
	cmp	r0, #0
	beq.n	.L_081251d8
	adds	r0, r6, #0
	bl	sub_0811be3c
	adds	r1, r0, #0
	adds	r0, r6, #0
	bl	sub_0811b4d8
	adds	r0, r6, #0
	movs	r1, #1
	bl	sub_08038120
	ldr	r0, [pc, #412]
	bl	sub_080381c8
	bl	sub_0811ff08
.L_081251d8:
	adds	r0, r6, #0
	bl	sub_08124974
	cmp	r0, #0
	beq.n	.L_08125202
	adds	r0, r6, #0
	bl	sub_0811be3c
	adds	r1, r0, #0
	adds	r0, r6, #0
	bl	sub_0811b4d8
	adds	r0, r6, #0
	movs	r1, #1
	bl	sub_08038120
	ldr	r0, [pc, #372]
	bl	sub_080381c8
	bl	sub_0811ff08
.L_08125202:
	adds	r0, r6, #0
	bl	sub_081249b0
	cmp	r0, #0
	beq.n	.L_0812522c
	adds	r0, r6, #0
	bl	sub_0811be3c
	adds	r1, r0, #0
	adds	r0, r6, #0
	bl	sub_0811b4d8
	adds	r0, r6, #0
	movs	r1, #1
	bl	sub_08038120
	ldr	r0, [pc, #336]
	bl	sub_080381c8
	bl	sub_0811ff08
.L_0812522c:
	adds	r0, r6, #0
	bl	sub_081249ec
	cmp	r0, #0
	beq.n	.L_08125256
	adds	r0, r6, #0
	bl	sub_0811be3c
	adds	r1, r0, #0
	adds	r0, r6, #0
	bl	sub_0811b4d8
	adds	r0, r6, #0
	movs	r1, #1
	bl	sub_08038120
	ldr	r0, [pc, #296]
	bl	sub_080381c8
	bl	sub_0811ff08
.L_08125256:
	adds	r0, r6, #0
	bl	sub_08124a28
	cmp	r0, #0
	beq.n	.L_08125286
	adds	r0, r6, #0
	bl	sub_0811be3c
	adds	r1, r0, #0
	adds	r0, r6, #0
	bl	sub_0811b4d8
	movs	r1, #1
	adds	r0, r6, #0
	bl	sub_08038120
	adds	r0, r6, #0
	bl	sub_0811b6a4
	ldr	r0, [pc, #252]
	bl	sub_080381c8
	bl	sub_0811ff08
.L_08125286:
	adds	r0, r6, #0
	bl	sub_08124a64
	cmp	r0, #0
	beq.n	.L_081252b6
	adds	r0, r6, #0
	bl	sub_0811be3c
	adds	r1, r0, #0
	adds	r0, r6, #0
	bl	sub_0811b4d8
	movs	r1, #1
	adds	r0, r6, #0
	bl	sub_08038120
	adds	r0, r6, #0
	bl	sub_0811b6a4
	ldr	r0, [pc, #208]
	bl	sub_080381c8
	bl	sub_0811ff08
.L_081252b6:
	adds	r0, r6, #0
	bl	sub_08124aa0
	cmp	r0, #0
	beq.n	.L_081252e0
	adds	r0, r6, #0
	bl	sub_0811be3c
	adds	r1, r0, #0
	adds	r0, r6, #0
	bl	sub_0811b4d8
	adds	r0, r6, #0
	movs	r1, #1
	bl	sub_08038120
	ldr	r0, [pc, #172]
	bl	sub_080381c8
	bl	sub_0811ff08
.L_081252e0:
	adds	r0, r6, #0
	bl	sub_08124af8
	cmp	r0, #0
	beq.n	.L_0812530a
	adds	r0, r6, #0
	bl	sub_0811be3c
	adds	r1, r0, #0
	adds	r0, r6, #0
	bl	sub_0811b4d8
	adds	r0, r6, #0
	movs	r1, #1
	bl	sub_08038120
	ldr	r0, [pc, #132]
	bl	sub_080381c8
	bl	sub_0811ff08
.L_0812530a:
	adds	r0, r6, #0
	bl	sub_08124ba8
	cmp	r0, #0
	beq.n	.L_08125334
	adds	r0, r6, #0
	bl	sub_0811be3c
	adds	r1, r0, #0
	adds	r0, r6, #0
	bl	sub_0811b4d8
	adds	r0, r6, #0
	movs	r1, #1
	bl	sub_08038120
	ldr	r0, [pc, #96]
	bl	sub_080381c8
	bl	sub_0811ff08
.L_08125334:
	movs	r2, #1
	add	sl, r2
	cmp	sl, r8
	bge.n	.L_0812533e
	b.n	.L_0812500e
.L_0812533e:
	ldr	r5, [sp, #0]
	movs	r3, #4
	subs	r5, #1
	add	r9, r3
	str	r5, [sp, #0]
	cmp	r5, #0
	blt.n	.L_0812534e
	b.n	.L_08124fe6
.L_0812534e:
	add	sp, #52
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x00000c6a
	.4byte 0x00000ce9
	.4byte 0x00000ce7
	.4byte 0x00000ce8
	.4byte 0x00000ce6
	.4byte 0x00000ceb
	.4byte 0x00000cea
	.4byte 0x00000cee
	.4byte 0x00000ced
	.4byte 0x00000ce3
	.4byte 0x00000cec
	.4byte 0x00000cf1
	.2byte 0x0cf2
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #36
	str	r2, [sp, #4]
	movs	r2, #0
	str	r1, [sp, #8]
	str	r2, [sp, #0]
	mov	r9, r0
.L_081253aa:
	movs	r3, #0
	movs	r7, #0
	mov	fp, r3
	mov	sl, r3
.L_081253b2:
	add	r2, sp, #36
	adds	r3, r7, r2
	movs	r4, #0
	adds	r6, r3, #0
	adds	r5, r3, #0
	mov	r8, r4
	subs	r6, #12
	subs	r5, #24
.L_081253c2:
	mov	r0, r9
	mov	r1, sl
	mov	r2, r8
	bl	sub_080ad1b0
	cmp	r0, #0
	beq.n	.L_081253e6
	mov	r3, sl
	mov	r4, r8
	movs	r2, #1
	adds	r7, #1
	strb	r3, [r6, #0]
	mov	fp, r2
	strb	r4, [r5, #0]
	adds	r6, #1
	adds	r5, #1
	cmp	r7, #10
	beq.n	.L_08125442
.L_081253e6:
	movs	r3, #1
	add	r8, r3
	mov	r4, r8
	cmp	r4, #19
	ble.n	.L_081253c2
	add	sl, r3
	mov	r2, sl
	cmp	r2, #3
	ble.n	.L_081253b2
	cmp	r7, #0
	bne.n	.L_08125448
	movs	r3, #0
	mov	sl, r3
.L_08125400:
	add	r2, sp, #36
	adds	r3, r7, r2
	movs	r4, #0
	adds	r6, r3, #0
	adds	r5, r3, #0
	mov	r8, r4
	subs	r6, #12
	subs	r5, #24
.L_08125410:
	mov	r0, r9
	mov	r1, sl
	mov	r2, r8
	bl	sub_080ad1b8
	cmp	r0, #0
	beq.n	.L_08125430
	mov	r3, sl
	mov	r4, r8
	adds	r7, #1
	strb	r3, [r6, #0]
	strb	r4, [r5, #0]
	adds	r6, #1
	adds	r5, #1
	cmp	r7, #10
	beq.n	.L_08125442
.L_08125430:
	movs	r2, #1
	add	r8, r2
	mov	r3, r8
	cmp	r3, #19
	ble.n	.L_08125410
	add	sl, r2
	mov	r4, sl
	cmp	r4, #3
	ble.n	.L_08125400
.L_08125442:
	cmp	r7, #0
	bne.n	.L_08125448
	b.n	.L_08125600
.L_08125448:
	bl	sub_080ad148
	adds	r3, r7, #0
	muls	r3, r0
	lsrs	r7, r3, #16
	add	r3, sp, #24
	ldrb	r3, [r3, r7]
	mov	r2, fp
	mov	sl, r3
	add	r3, sp, #12
	ldrb	r3, [r3, r7]
	mov	r8, r3
	cmp	r2, #0
	beq.n	.L_081254a6
	mov	r1, sl
	mov	r2, r8
	mov	r0, r9
	bl	sub_080ad170
	mov	r0, r9
	mov	r1, sl
	mov	r2, r8
	bl	sub_080ad160
	ldr	r3, [sp, #4]
	cmp	r3, #0
	beq.n	.L_081254ae
	movs	r0, #0
	mov	r1, r9
	bl	sub_08120360
	mov	r4, sl
	lsls	r1, r4, #2
	add	r1, sl
	lsls	r1, r1, #2
	movs	r2, #150
	lsls	r2, r2, #1
	add	r1, r8
	adds	r1, r1, r2
	movs	r0, #3
	bl	sub_08120360
	ldr	r1, [pc, #368]
	movs	r0, #4
	bl	sub_08120360
	b.n	.L_08125600
.L_081254a6:
	ldr	r3, [sp, #4]
	cmp	r3, #0
	beq.n	.L_081254ae
	b.n	.L_08125600
.L_081254ae:
	mov	r4, r9
	movs	r0, #0
	cmp	r4, #7
	bls.n	.L_081254b8
	movs	r0, #1
.L_081254b8:
	bl	sub_080ad000
	movs	r4, #148
	adds	r3, r0, #0
	lsls	r4, r4, #1
	adds	r0, r3, r4
	adds	r2, r3, #0
	ldr	r3, [r0, #0]
	movs	r1, #0
	adds	r2, #8
	cmp	r1, r3
	bge.n	.L_081254f6
	movs	r5, #1
	negs	r5, r5
	movs	r4, #254
.L_081254d6:
	ldrb	r3, [r2, #0]
	cmp	r3, sl
	bne.n	.L_081254ec
	ldrb	r3, [r2, #1]
	cmp	r3, r8
	bne.n	.L_081254ec
	movs	r3, #3
	ldrsb	r3, [r2, r3]
	cmp	r3, r5
	bne.n	.L_081254ec
	strb	r4, [r2, #3]
.L_081254ec:
	ldr	r3, [r0, #0]
	adds	r1, #1
	adds	r2, #4
	cmp	r1, r3
	blt.n	.L_081254d6
.L_081254f6:
	mov	r2, r9
	movs	r0, #0
	cmp	r2, #7
	bls.n	.L_08125500
	movs	r0, #1
.L_08125500:
	bl	sub_080ad000
	movs	r4, #148
	adds	r3, r0, #0
	lsls	r4, r4, #1
	movs	r2, #2
	adds	r5, r3, #0
	adds	r0, r3, r4
	negs	r2, r2
	adds	r5, #8
	mov	lr, r2
	mov	fp, r0
.L_08125518:
	ldr	r3, [r0, #0]
	movs	r7, #1
	movs	r1, #0
	negs	r7, r7
	cmp	r1, r3
	bge.n	.L_08125546
	movs	r3, #3
	ldrsb	r3, [r5, r3]
	cmp	r3, lr
	bne.n	.L_08125530
	ldrb	r7, [r5, #2]
	b.n	.L_08125546
.L_08125530:
	ldr	r3, [r0, #0]
	adds	r1, #1
	cmp	r1, r3
	bge.n	.L_08125546
	lsls	r3, r1, #2
	adds	r2, r5, r3
	movs	r3, #3
	ldrsb	r3, [r2, r3]
	cmp	r3, lr
	bne.n	.L_08125530
	ldrb	r7, [r2, #2]
.L_08125546:
	movs	r3, #1
	negs	r3, r3
	cmp	r7, r3
	beq.n	.L_081255b0
	adds	r4, r3, #0
	ldr	r3, [r0, #0]
	cmp	r3, #0
	ble.n	.L_08125574
	mov	r3, fp
	ldr	r1, [r3, #0]
	adds	r2, r5, #0
.L_0812555c:
	ldrb	r3, [r2, #2]
	cmp	r3, r7
	bne.n	.L_0812556c
	movs	r3, #3
	ldrsb	r3, [r2, r3]
	cmp	r3, r4
	ble.n	.L_0812556c
	adds	r4, r3, #0
.L_0812556c:
	subs	r1, #1
	adds	r2, #4
	cmp	r1, #0
	bne.n	.L_0812555c
.L_08125574:
	adds	r4, #1
	cmp	r4, #1
	bgt.n	.L_0812557c
	movs	r4, #2
.L_0812557c:
	ldr	r3, [r0, #0]
	movs	r1, #0
	cmp	r1, r3
	bge.n	.L_08125518
	movs	r2, #2
	movs	r3, #144
	negs	r2, r2
	lsls	r3, r3, #1
	mov	ip, r2
	adds	r6, r5, r3
	adds	r2, r5, #0
.L_08125592:
	ldrb	r3, [r2, #2]
	cmp	r3, r7
	bne.n	.L_081255a4
	movs	r3, #3
	ldrsb	r3, [r2, r3]
	cmp	r3, ip
	bne.n	.L_081255a4
	strb	r4, [r2, #3]
	adds	r4, #1
.L_081255a4:
	ldr	r3, [r6, #0]
	adds	r1, #1
	adds	r2, #4
	cmp	r1, r3
	blt.n	.L_08125592
	b.n	.L_08125518
.L_081255b0:
	ldr	r4, [sp, #0]
	cmp	r4, #0
	bne.n	.L_081255f8
	movs	r2, #1
	movs	r0, #0
	mov	r1, r9
	str	r2, [sp, #0]
	bl	sub_08120360
	ldr	r3, [sp, #8]
	cmp	r3, #0
	bne.n	.L_081255f0
	mov	r4, sl
	lsls	r1, r4, #2
	add	r1, sl
	lsls	r1, r1, #2
	movs	r2, #150
	lsls	r2, r2, #1
	add	r1, r8
	adds	r1, r1, r2
	movs	r0, #3
	bl	sub_08120360
	mov	r3, r9
	ldr	r1, [pc, #48]
	cmp	r3, #7
	bls.n	.L_081255e8
	adds	r1, #1
.L_081255e8:
	movs	r0, #4
	bl	sub_08120360
	b.n	.L_081255f8
.L_081255f0:
	ldr	r1, [pc, #36]
	movs	r0, #4
	bl	sub_08120360
.L_081255f8:
	ldr	r4, [sp, #8]
	cmp	r4, #0
	beq.n	.L_08125600
	b.n	.L_081253aa
.L_08125600:
	add	sp, #36
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x00000c97
	.4byte 0x00000c94
	.2byte 0x0c96
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #32
	str	r0, [sp, #12]
	movs	r1, #0
	ldrb	r0, [r0, #0]
	str	r1, [sp, #4]
	mov	r8, r0
	bl	sub_08016ca4
	mov	r2, r8
	str	r0, [sp, #8]
	cmp	r2, #7
	bls.n	.L_08125650
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #36]
	adds	r3, #68
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_081256a4
.L_08125650:
	mov	r3, r8
	movs	r0, #0
	cmp	r3, #7
	bls.n	.L_0812565a
	movs	r0, #1
.L_0812565a:
	bl	sub_080ad000
	adds	r3, r0, #0
	movs	r0, #148
	lsls	r0, r0, #1
	adds	r6, r3, #0
	adds	r3, r3, r0
	ldr	r3, [r3, #0]
	ldr	r1, [sp, #4]
	adds	r6, #8
	movs	r7, #0
	cmp	r1, r3
	bge.n	.L_081256a4
	movs	r2, #1
	negs	r2, r2
	mov	sl, r2
	adds	r5, r6, #0
.L_0812567c:
	ldrb	r3, [r5, #2]
	cmp	r3, r8
	bne.n	.L_08125694
	movs	r3, #3
	ldrsb	r3, [r5, r3]
	cmp	r3, sl
	bne.n	.L_08125694
	ldrb	r1, [r5, #0]
	ldrb	r2, [r5, #1]
	mov	r0, r8
	bl	sub_080ad160
.L_08125694:
	movs	r0, #144
	lsls	r0, r0, #1
	adds	r3, r6, r0
	ldr	r3, [r3, #0]
	adds	r7, #1
	adds	r5, #4
	cmp	r7, r3
	blt.n	.L_0812567c
.L_081256a4:
	movs	r0, #1
	movs	r1, #0
	bl	sub_0811a188
	cmp	r0, #0
	beq.n	.L_081256c0
	movs	r0, #2
	movs	r1, #0
	bl	sub_0811a188
	cmp	r0, #0
	beq.n	.L_081256c0
	movs	r1, #1
	str	r1, [sp, #4]
.L_081256c0:
	mov	r2, r8
	movs	r0, #0
	cmp	r2, #7
	bls.n	.L_081256d8
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #36]
	adds	r3, #68
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_081256d8
	movs	r0, #1
.L_081256d8:
	bl	sub_080ad000
	add	r3, sp, #16
	mov	fp, r3
	adds	r6, r0, #0
	adds	r6, #8
	movs	r2, #0
	add	r3, sp, #28
	mov	ip, fp
.L_081256ea:
	str	r2, [r3, #0]
	subs	r3, #4
	cmp	r3, ip
	bge.n	.L_081256ea
	movs	r3, #144
	lsls	r3, r3, #1
	adds	r7, r6, r3
	movs	r0, #2
	str	r7, [sp, #0]
	negs	r0, r0
	mov	r9, r0
.L_08125700:
	ldr	r3, [r7, #0]
	movs	r1, #1
	negs	r1, r1
	movs	r4, #0
	mov	ip, r1
	cmp	r4, r3
	bge.n	.L_08125732
	movs	r3, #3
	ldrsb	r3, [r6, r3]
	cmp	r3, r9
	bne.n	.L_0812571a
	ldrb	r2, [r6, #2]
	b.n	.L_08125730
.L_0812571a:
	ldr	r3, [r7, #0]
	adds	r4, #1
	cmp	r4, r3
	bge.n	.L_08125732
	lsls	r3, r4, #2
	adds	r2, r6, r3
	movs	r3, #3
	ldrsb	r3, [r2, r3]
	cmp	r3, r9
	bne.n	.L_0812571a
	ldrb	r2, [r2, #2]
.L_08125730:
	mov	ip, r2
.L_08125732:
	movs	r3, #1
	negs	r3, r3
	cmp	ip, r3
	beq.n	.L_081257ac
	adds	r5, r3, #0
	ldr	r3, [r7, #0]
	cmp	r3, #0
	ble.n	.L_08125760
	ldr	r3, [sp, #0]
	adds	r2, r6, #0
	ldr	r4, [r3, #0]
.L_08125748:
	ldrb	r3, [r2, #2]
	cmp	r3, ip
	bne.n	.L_08125758
	movs	r3, #3
	ldrsb	r3, [r2, r3]
	cmp	r3, r5
	ble.n	.L_08125758
	adds	r5, r3, #0
.L_08125758:
	subs	r4, #1
	adds	r2, #4
	cmp	r4, #0
	bne.n	.L_08125748
.L_08125760:
	adds	r5, #1
	cmp	r5, #1
	bgt.n	.L_08125768
	movs	r5, #2
.L_08125768:
	ldr	r3, [r7, #0]
	movs	r4, #0
	cmp	r4, r3
	bge.n	.L_08125700
	movs	r1, #144
	movs	r0, #2
	lsls	r1, r1, #1
	negs	r0, r0
	adds	r1, r1, r6
	mov	sl, r0
	mov	lr, r1
	mov	r0, fp
	adds	r1, r6, #0
.L_08125782:
	ldrb	r3, [r1, #2]
	cmp	r3, ip
	bne.n	.L_0812579e
	movs	r3, #3
	ldrsb	r3, [r1, r3]
	cmp	r3, sl
	bne.n	.L_0812579e
	ldrb	r2, [r1, #0]
	strb	r5, [r1, #3]
	lsls	r2, r2, #2
	ldr	r3, [r0, r2]
	adds	r5, #1
	adds	r3, #1
	str	r3, [r0, r2]
.L_0812579e:
	mov	r2, lr
	ldr	r3, [r2, #0]
	adds	r4, #1
	adds	r1, #4
	cmp	r4, r3
	blt.n	.L_08125782
	b.n	.L_08125700
.L_081257ac:
	ldr	r3, [sp, #4]
	cmp	r3, #0
	bne.n	.L_081257b4
	b.n	.L_08125a64
.L_081257b4:
	movs	r5, #166
	lsls	r5, r5, #1
	adds	r0, r5, #0
	bl	sub_08014dac
	adds	r2, r5, #0
	ldr	r3, [pc, #704]
	ldr	r1, [sp, #8]
	mov	sl, r0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9a02
	movs	r3, #150
	lsls	r3, r3, #1
	movs	r6, #0
	mov	r5, fp
	movs	r4, #4
	movs	r0, #0
	adds	r1, r2, r3
.L_081257da:
	ldr	r2, [r0, r5]
	cmp	r2, #4
	ble.n	.L_081257e6
	mov	r2, fp
	str	r4, [r0, r2]
	adds	r2, r4, #0
.L_081257e6:
	movs	r3, #0
	ldrsb	r3, [r1, r3]
	cmp	r3, r2
	bge.n	.L_081257f0
	strb	r2, [r1, #0]
.L_081257f0:
	adds	r6, #1
	adds	r1, #1
	adds	r0, #4
	cmp	r6, #3
	ble.n	.L_081257da
	mov	r0, r8
	bl	sub_080ad008
	movs	r6, #0
	movs	r7, #72
.L_08125804:
	ldr	r3, [sp, #8]
	mov	r1, sl
	ldrsh	r2, [r7, r3]
	ldrsh	r3, [r7, r1]
	subs	r5, r2, r3
	cmp	r5, #0
	ble.n	.L_0812587a
	bl	sub_081234a4
	movs	r0, #25
	bl	sub_08122c88
	movs	r0, #0
	mov	r1, r8
	bl	sub_08120360
	movs	r0, #1
	adds	r1, r5, #0
	bl	sub_08120360
	movs	r0, #14
	movs	r1, #175
	bl	sub_08120360
	ldr	r1, [pc, #592]
	movs	r0, #4
	adds	r1, r6, r1
	bl	sub_08120360
	mov	r1, r8
	movs	r0, #11
	bl	sub_08120360
	movs	r0, #212
	bl	sub_081c0010
	mov	r0, r8
	bl	sub_0811be3c
	movs	r1, #3
	ldr	r0, [r0, #0]
	bl	sub_08020090
	mov	r0, r8
	bl	sub_0811be3c
	movs	r1, #32
	ldr	r0, [r0, #0]
	bl	sub_08020098
	movs	r3, #1
	mov	r0, r8
	adds	r1, r6, #0
	movs	r2, #2
	negs	r3, r3
	bl	sub_08127308
	bl	sub_081234f0
.L_0812587a:
	adds	r6, #1
	adds	r7, #4
	cmp	r6, #3
	ble.n	.L_08125804
	mov	r0, sl
	bl	sub_08013164
	ldr	r1, [sp, #4]
	cmp	r1, #0
	bne.n	.L_08125890
	b.n	.L_08125a64
.L_08125890:
	mov	r0, r8
	bl	sub_0811be3c
	ldr	r3, [r0, #0]
	cmp	r3, #0
	bne.n	.L_0812589e
	b.n	.L_08125a64
.L_0812589e:
	bl	sub_081234a4
	ldr	r2, [sp, #12]
	ldr	r3, [r2, #96]
	cmp	r3, #0
	beq.n	.L_08125924
	movs	r0, #8
	mov	r1, r8
	bl	sub_08120360
	movs	r0, #0
	mov	r1, r8
	bl	sub_08120360
	ldr	r3, [sp, #12]
	movs	r0, #1
	ldr	r1, [r3, #96]
	bl	sub_08120360
	ldr	r1, [pc, #452]
	movs	r0, #4
	bl	sub_08120360
	ldr	r0, [sp, #12]
	ldr	r1, [r0, #96]
	mov	r0, r8
	negs	r1, r1
	bl	sub_080ad0c0
	cmp	r0, #0
	bne.n	.L_0812591c
	mov	r0, r8
	bl	sub_081203a8
	cmp	r0, #0
	beq.n	.L_081258f8
	movs	r1, #1
	mov	r0, r8
	bl	sub_080ad0c0
	movs	r0, #11
	mov	r1, r8
	bl	sub_08120360
	b.n	.L_08125924
.L_081258f8:
	movs	r0, #9
	mov	r1, r8
	bl	sub_08120360
	mov	r1, r8
	movs	r0, #0
	bl	sub_08120360
	mov	r1, r8
	cmp	r1, #7
	bhi.n	.L_08125912
	ldr	r1, [pc, #384]
	b.n	.L_08125914
.L_08125912:
	ldr	r1, [pc, #384]
.L_08125914:
	movs	r0, #4
	bl	sub_08120360
	b.n	.L_08125924
.L_0812591c:
	movs	r0, #11
	mov	r1, r8
	bl	sub_08120360
.L_08125924:
	bl	sub_081201c4
	bl	sub_081234a4
	ldr	r2, [sp, #8]
	movs	r3, #50
	adds	r3, #255
	adds	r6, r2, r3
	movs	r0, #0
	ldrsb	r0, [r6, r0]
	cmp	r0, #0
	beq.n	.L_081259e4
	movs	r1, #52
	ldrsh	r3, [r2, r1]
	movs	r1, #10
	muls	r0, r3
	bl	sub_08002054
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r7, r0, #0
	mov	r1, r8
	movs	r0, #8
	ldr	r5, [r3, #36]
	bl	sub_08120360
	movs	r0, #0
	mov	r1, r8
	bl	sub_08120360
	movs	r0, #1
	adds	r1, r7, #0
	bl	sub_08120360
	ldr	r1, [pc, #300]
	movs	r0, #4
	bl	sub_08120360
	movs	r3, #0
	ldrsb	r3, [r6, r3]
	cmp	r3, #0
	beq.n	.L_08125984
	movs	r3, #128
	lsls	r3, r3, #4
	adds	r3, #76
	adds	r2, r5, r3
	movs	r3, #134
	b.n	.L_0812598e
.L_08125984:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #76
	adds	r2, r5, r0
	movs	r3, #133
.L_0812598e:
	strh	r3, [r2, #0]
	negs	r1, r7
	mov	r0, r8
	bl	sub_080ad0c0
	cmp	r0, #0
	bne.n	.L_081259dc
	mov	r0, r8
	bl	sub_081203a8
	cmp	r0, #0
	beq.n	.L_081259b8
	movs	r1, #1
	mov	r0, r8
	bl	sub_080ad0c0
	movs	r0, #11
	mov	r1, r8
	bl	sub_08120360
	b.n	.L_081259e4
.L_081259b8:
	movs	r0, #9
	mov	r1, r8
	bl	sub_08120360
	mov	r1, r8
	movs	r0, #0
	bl	sub_08120360
	mov	r1, r8
	cmp	r1, #7
	bhi.n	.L_081259d2
	ldr	r1, [pc, #192]
	b.n	.L_081259d4
.L_081259d2:
	ldr	r1, [pc, #192]
.L_081259d4:
	movs	r0, #4
	bl	sub_08120360
	b.n	.L_081259e4
.L_081259dc:
	movs	r0, #11
	mov	r1, r8
	bl	sub_08120360
.L_081259e4:
	bl	sub_081201c4
	bl	sub_081234a4
	ldr	r2, [sp, #8]
	movs	r3, #66
	adds	r3, #255
	adds	r5, r2, r3
	ldrb	r3, [r5, #0]
	cmp	r3, #0
	beq.n	.L_08125a60
	mov	r0, r8
	bl	sub_081203a8
	cmp	r0, #0
	bne.n	.L_08125a60
	ldrb	r3, [r5, #0]
	adds	r3, #255
	strb	r3, [r5, #0]
	lsls	r3, r3, #24
	cmp	r3, #0
	bne.n	.L_08125a60
	movs	r1, #192
	lsls	r1, r1, #24
	mov	r0, r8
	bl	sub_080ad0c0
	cmp	r0, #0
	bne.n	.L_08125a60
	movs	r0, #0
	mov	r1, r8
	bl	sub_08120360
	ldr	r5, [pc, #116]
	movs	r0, #4
	adds	r1, r5, #0
	bl	sub_08120360
	movs	r0, #8
	mov	r1, r8
	bl	sub_08120360
	movs	r0, #9
	mov	r1, r8
	bl	sub_08120360
	movs	r0, #0
	mov	r1, r8
	bl	sub_08120360
	mov	r0, r8
	cmp	r0, #7
	bhi.n	.L_08125a58
	subs	r1, r5, #3
	movs	r0, #4
	bl	sub_08120360
	b.n	.L_08125a60
.L_08125a58:
	adds	r1, r5, #3
	movs	r0, #4
	bl	sub_08120360
.L_08125a60:
	bl	sub_081201c4
.L_08125a64:
	mov	r0, r8
	bl	sub_0811be3c
	ldr	r3, [r0, #0]
	cmp	r3, #0
	beq.n	.L_08125a76
	mov	r0, r8
	bl	sub_080ad008
.L_08125a76:
	add	sp, #32
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x03000730
	.4byte 0x00000cd5
	.4byte 0x00000ca0
	.4byte 0x00000c71
	.4byte 0x00000c77
	.4byte 0x00000ca8
	.2byte 0x0c74
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	movs	r2, #128
	lsls	r2, r2, #19
	sub	sp, #8
	adds	r2, #8
	movs	r0, #6
	ldrh	r3, [r2, #0]
	add	r0, sp
	mov	r9, r0
	mov	r1, r9
	strh	r3, [r1, #0]
	ldr	r1, [pc, #52]
	mov	r7, sp
	orrs	r3, r1
	strh	r3, [r2, #0]
	add	r3, sp, #4
	adds	r2, #2
	mov	sl, r3
	ldrh	r3, [r2, #0]
	mov	r0, sl
	strh	r3, [r0, #0]
	orrs	r3, r1
	strh	r3, [r2, #0]
	movs	r3, #2
	add	r3, sp
	adds	r2, #2
	mov	r8, r3
	ldrh	r3, [r2, #0]
	mov	r0, r8
	strh	r3, [r0, #0]
	orrs	r3, r1
	strh	r3, [r2, #0]
	adds	r2, #2
	ldrh	r3, [r2, #0]
	movs	r0, #16
	strh	r3, [r7, #0]
	orrs	r3, r1
	b.n	.L_08125af8
	.2byte 0x0040
	.2byte 0x0000
.L_08125af8:
	strh	r3, [r2, #0]
	ldr	r3, [pc, #56]
	adds	r2, #66
	strh	r3, [r2, #0]
	bl	sub_08013e70
	movs	r6, #128
	lsls	r6, r6, #19
	movs	r5, #0
	adds	r6, #76
.L_08125b0c:
	bl	sub_08014878
	bl	sub_08014878
	bl	sub_08014878
	bl	sub_08014878
	lsls	r3, r5, #8
	orrs	r3, r5
	strh	r3, [r6, #0]
	movs	r0, #1
	adds	r5, #1
	bl	sub_08013560
	cmp	r5, #15
	ble.n	.L_08125b0c
	ldr	r3, [pc, #8]
	b.n	.L_08125b3c
	movs	r0, r0
	.4byte 0x00003eee
	.2byte 0x0001
	.2byte 0x0000
.L_08125b3c:
	movs	r2, #128
	lsls	r2, r2, #19
	strh	r3, [r2, #0]
	movs	r0, #4
	bl	sub_08013560
	mov	r1, r9
	ldrh	r3, [r1, #0]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #8
	strh	r3, [r2, #0]
	mov	r0, sl
	ldrh	r3, [r0, #0]
	adds	r2, #2
	strh	r3, [r2, #0]
	mov	r1, r8
	ldrh	r3, [r1, #0]
	adds	r2, #2
	strh	r3, [r2, #0]
	ldrh	r3, [r7, #0]
	adds	r2, #2
	movs	r0, #0
	strh	r3, [r2, #0]
	add	sp, #8
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	push	{lr}
	ldr	r2, [pc, #48]
	ldr	r1, [pc, #48]
	movs	r3, #0
.L_08125b80:
	adds	r3, #1
	stmia	r0!, {r2}
	adds	r2, r2, r1
	cmp	r3, #63
	bls.n	.L_08125b80
	ldr	r2, [pc, #32]
	ldr	r1, [pc, #32]
	movs	r3, #0
.L_08125b90:
	adds	r3, #1
	stmia	r0!, {r2}
	adds	r2, r2, r1
	cmp	r3, #55
	bls.n	.L_08125b90
	movs	r1, #136
	movs	r2, #1
	ldr	r3, [pc, #20]
	lsls	r1, r1, #2
	negs	r2, r2
	mov	lr, r3
	.2byte 0xf800
	.2byte 0xbd00
	movs	r0, r0
	.4byte 0x03020100
	.4byte 0x04040404
	.2byte 0x0260
	.2byte 0x0300
	push	{r5, r6, lr}
	movs	r1, #128
	movs	r2, #1
	ldr	r5, [pc, #64]
	lsls	r1, r1, #1
	negs	r2, r2
	adds	r6, r0, #0
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x2380
	lsls	r3, r3, #1
	adds	r6, r6, r3
	adds	r0, r6, #0
	movs	r1, #128
	ldr	r2, [pc, #44]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x2180
	ldr	r2, [pc, #40]
	lsls	r1, r1, #10
	adds	r6, #128
	movs	r3, #0
	adds	r1, #2
.L_08125be6:
	adds	r3, #1
	stmia	r6!, {r2}
	adds	r2, r2, r1
	cmp	r3, #239
	bls.n	.L_08125be6
	movs	r1, #160
	ldr	r3, [pc, #12]
	adds	r0, r6, #0
	lsls	r1, r1, #2
	ldr	r2, [pc, #8]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0xbd60
	.4byte 0x03000260
	.4byte 0x03ff03ff
	.2byte 0x0200
	.2byte 0x0201
	push	{lr}
	movs	r2, #192
	lsls	r2, r2, #18
	adds	r3, r2, #0
	adds	r3, #176
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #8]
	cmp	r3, #2
	bne.n	.L_08125c54
	ldr	r4, [r2, #40]
	ldr	r3, [r4, #0]
	lsls	r0, r3, #2
	adds	r0, r0, r3
	lsls	r0, r0, #6
	adds	r0, r4, r0
	ldrh	r3, [r0, #32]
	movs	r1, #128
	lsls	r1, r1, #19
	adds	r1, #12
	strh	r3, [r1, #0]
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #176
	adds	r0, #34
	ldr	r2, [pc, #24]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r2, #132
	adds	r0, r4, #0
	lsls	r2, r2, #24
	adds	r3, #36
	adds	r0, #16
	adds	r1, #20
	adds	r2, #4
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
.L_08125c54:
	pop	{pc}
	movs	r0, r0
	.2byte 0x0001
	.2byte 0xa260
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #168
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #0]
	subs	r0, r3, #1
	cmp	r0, #31
	bhi.n	.L_08125c88
	ldr	r2, [pc, #28]
	lsrs	r0, r0, #2
	lsls	r0, r0, #5
	adds	r0, r0, r2
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	ldr	r1, [pc, #12]
	adds	r2, #8
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
.L_08125c88:
	pop	{pc}
	movs	r0, r0
	.4byte 0x0812cc74
	.4byte 0x06005000
