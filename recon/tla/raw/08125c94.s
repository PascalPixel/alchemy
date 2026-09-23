.syntax unified
	.thumb
	.set sub_0801314c, 0x0801314c
	.set sub_08013438, 0x08013438
	.set sub_08013560, 0x08013560
	.set sub_08013ba4, 0x08013ba4
	.set sub_08013c58, 0x08013c58
	.set sub_08013d0c, 0x08013d0c
	.set sub_080145a8, 0x080145a8
	.set sub_08014644, 0x08014644
	.set sub_08014cc0, 0x08014cc0
	.set sub_08014d00, 0x08014d00
	.set sub_08016ca4, 0x08016ca4
	.set sub_08038128, 0x08038128
	.set sub_08118410, 0x08118410
	.set sub_08118bcc, 0x08118bcc
	.set sub_0811a038, 0x0811a038
	.set sub_0811a0b0, 0x0811a0b0
	.set sub_0811a31c, 0x0811a31c
	.set sub_0811b9d4, 0x0811b9d4
	.set sub_0811be3c, 0x0811be3c
	.set sub_08125b78, 0x08125b78
	.set sub_08125bb8, 0x08125bb8
	.set sub_08126804, 0x08126804
	.set sub_08126ae4, 0x08126ae4
	.set sub_08138028, 0x08138028
	.global Func_08125c94
	.thumb_func
Func_08125c94:
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r2, r3, #0
	adds	r2, #168
	ldr	r0, [r2, #0]
	ldr	r4, [r3, #48]
	ldr	r1, [r0, #0]
	movs	r3, #52
	subs	r2, r3, r1
	cmp	r2, #32
	ble.n	.L_08125cae
	movs	r2, #32
.L_08125cae:
	cmp	r2, #0
	bge.n	.L_08125cb4
	movs	r2, #0
.L_08125cb4:
	ldr	r3, [pc, #64]
	strh	r2, [r3, #2]
	cmp	r1, #80
	bhi.n	.L_08125cd0
	lsls	r2, r1, #1
	adds	r2, r2, r1
	lsls	r3, r2, #4
	subs	r3, r3, r2
	movs	r2, #175
	lsls	r2, r2, #8
	lsls	r3, r3, #3
	adds	r2, #128
	adds	r3, r3, r2
	strh	r3, [r4, #54]
.L_08125cd0:
	ldr	r3, [r0, #0]
	adds	r2, r3, #1
	str	r2, [r0, #0]
	cmp	r2, #80
	bhi.n	.L_08125cea
	movs	r3, #180
	subs	r3, r3, r2
	movs	r0, #0
	movs	r1, #0
	movs	r2, #0
	bl	sub_08126804
	b.n	.L_08125cf6
.L_08125cea:
	movs	r0, #0
	movs	r1, #0
	movs	r2, #0
	movs	r3, #100
	bl	sub_08126804
.L_08125cf6:
	pop	{pc}
	.2byte 0x1120
	.2byte 0x0300
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #168
	ldr	r3, [r3, #0]
	ldr	r0, [r3, #0]
	cmp	r0, #79
	bhi.n	.L_08125d62
	movs	r2, #240
	movs	r3, #7
	lsls	r2, r2, #8
	ands	r3, r0
	adds	r2, #129
	adds	r4, r3, r2
	adds	r3, r0, #0
	cmp	r0, #0
	bge.n	.L_08125d20
	adds	r3, r0, #7
.L_08125d20:
	asrs	r3, r3, #3
	movs	r2, #13
	ldr	r5, [pc, #60]
	subs	r2, r2, r3
	lsls	r3, r2, #6
	movs	r1, #0
	adds	r2, r3, r5
.L_08125d2e:
	adds	r1, #1
	strh	r4, [r2, #0]
	adds	r2, #2
	cmp	r1, #32
	bne.n	.L_08125d2e
	movs	r3, #128
	lsls	r3, r3, #4
	orrs	r4, r3
	adds	r3, r0, #0
	cmp	r3, #0
	bge.n	.L_08125d46
	adds	r3, #7
.L_08125d46:
	asrs	r3, r3, #3
	adds	r2, r3, #0
	adds	r2, #13
	cmp	r2, #20
	bhi.n	.L_08125d62
	ldr	r0, [pc, #16]
	lsls	r3, r2, #6
	movs	r1, #0
	adds	r2, r3, r0
.L_08125d58:
	adds	r1, #1
	strh	r4, [r2, #0]
	adds	r2, #2
	cmp	r1, #32
	bne.n	.L_08125d58
.L_08125d62:
	pop	{r5, pc}
	.2byte 0x6000
	.2byte 0x0600
	movs	r3, #128
	lsls	r3, r3, #19
	movs	r2, #0
	adds	r3, #18
	strh	r2, [r3, #0]
	bx	lr
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #156
	movs	r5, #192
	str	r0, [sp, #8]
	lsls	r5, r5, #18
	adds	r3, r5, #0
	adds	r3, #176
	adds	r7, r1, #0
	movs	r0, #168
	movs	r1, #4
	ldr	r6, [r3, #0]
	bl	sub_08014cc0
	adds	r3, r7, #0
	subs	r3, #78
	str	r0, [sp, #4]
	cmp	r3, #2
	bhi.n	.L_08125da6
	b.n	.L_08125fc0
.L_08125da6:
	ldr	r0, [pc, #216]
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	mov	ip, r0
	adds	r3, #212
	ldr	r1, [pc, #204]
	adds	r2, #8
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r2, #132
	mov	r0, ip
	lsls	r2, r2, #24
	adds	r0, #32
	adds	r1, #32
	adds	r2, #8
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r2, #132
	mov	r0, ip
	lsls	r2, r2, #24
	adds	r0, #64
	adds	r1, #32
	adds	r2, #8
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r2, #132
	mov	r0, ip
	lsls	r2, r2, #24
	adds	r0, #96
	adds	r1, #32
	adds	r2, #8
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r2, #132
	mov	r0, ip
	lsls	r2, r2, #24
	adds	r0, #128
	adds	r1, #32
	adds	r2, #8
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r2, #132
	mov	r0, ip
	lsls	r2, r2, #24
	adds	r0, #160
	adds	r1, #32
	adds	r2, #8
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r2, #132
	mov	r0, ip
	lsls	r2, r2, #24
	adds	r0, #192
	adds	r1, #32
	adds	r2, #8
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r3, #0
	str	r3, [r6, #16]
	movs	r3, #128
	movs	r2, #1
	lsls	r3, r3, #19
	str	r2, [r6, #12]
	str	r2, [r6, #8]
	strh	r2, [r3, #0]
	ldr	r5, [pc, #88]
	movs	r1, #32
	ldr	r2, [pc, #88]
	ldr	r0, [pc, #92]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x4816
	movs	r1, #32
	movs	r2, #0
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x2280
	ldr	r3, [pc, #48]
	lsls	r2, r2, #19
	adds	r2, #10
	strh	r3, [r2, #0]
	movs	r1, #128
	lsls	r1, r1, #19
	adds	r1, #8
	ldrh	r3, [r1, #0]
	ldr	r2, [pc, #36]
	orrs	r3, r2
	ldr	r2, [pc, #60]
	strh	r3, [r1, #0]
	movs	r3, #2
	str	r3, [r6, #8]
	movs	r6, #0
.L_08125e62:
	movs	r1, #240
	lsls	r1, r1, #8
	adds	r1, #128
	cmp	r6, #20
	bls.n	.L_08125e72
	movs	r1, #240
	lsls	r1, r1, #8
	adds	r1, #136
.L_08125e72:
	movs	r3, #0
	b.n	.L_08125e9c
	movs	r0, r0
	.4byte 0x00000c04
	.4byte 0x00000002
	.4byte 0x0812cd74
	.4byte 0x06005020
	.4byte 0x03000260
	.4byte 0x33333333
	.4byte 0x06005000
	.4byte 0x06005100
	.2byte 0x6000
	.2byte 0x0600
.L_08125e9c:
	adds	r3, #1
	strh	r1, [r2, #0]
	adds	r2, #2
	cmp	r3, #31
	bls.n	.L_08125e9c
	adds	r6, #1
	cmp	r6, #31
	bls.n	.L_08125e62
	ldr	r5, [pc, #92]
	movs	r6, #32
	movs	r3, #8
	movs	r1, #0
	strh	r3, [r5, #4]
	strh	r6, [r5, #2]
	strh	r6, [r5, #6]
	movs	r0, #1
	mov	r8, r1
	bl	sub_08013560
	ldr	r1, [pc, #56]
	movs	r3, #128
	ldr	r2, [pc, #56]
	lsls	r3, r3, #19
	adds	r3, #64
	strh	r1, [r3, #0]
	adds	r3, #4
	strh	r2, [r3, #0]
	subs	r3, #2
	strh	r1, [r3, #0]
	adds	r3, #4
	strh	r2, [r3, #0]
	ldr	r3, [pc, #40]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #72
	strh	r3, [r2, #0]
	ldr	r3, [pc, #32]
	movs	r1, #238
	adds	r2, #2
	movs	r0, #128
	lsls	r1, r1, #7
	strh	r3, [r2, #0]
	lsls	r0, r0, #19
	adds	r1, #65
	bl	sub_08013ba4
	b.n	.L_08125f10
	movs	r0, r0
	.4byte 0x000000f0
	.4byte 0x00000088
	.4byte 0x00003537
	.4byte 0x00003f21
	.2byte 0x1120
	.2byte 0x0300
.L_08125f10:
	movs	r0, #0
	movs	r1, #0
	movs	r2, #0
	movs	r3, #180
	bl	sub_08126804
	ldr	r3, [sp, #4]
	mov	r2, r8
	movs	r1, #144
	str	r2, [r3, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #136]
	bl	sub_080145a8
	movs	r1, #200
	lsls	r1, r1, #4
	ldr	r0, [pc, #128]
	bl	sub_080145a8
	ldr	r2, [pc, #128]
	movs	r1, #32
	movs	r0, #2
	bl	sub_08013438
	strh	r6, [r5, #2]
	movs	r0, #1
	bl	sub_08013560
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #36]
	movs	r5, #128
	adds	r3, #65
	ldrb	r0, [r3, #0]
	lsls	r5, r5, #19
	bl	sub_08038128
	adds	r5, #8
	movs	r0, #20
	bl	sub_08013560
	adds	r0, r5, #0
	movs	r1, #2
	bl	sub_08013d0c
	adds	r0, r5, #0
	movs	r1, #0
	bl	sub_08013c58
	adds	r3, r7, #0
	subs	r3, #74
	cmp	r3, #2
	bls.n	.L_08125f84
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #126
	cmp	r7, r0
	bne.n	.L_08125f8c
.L_08125f84:
	movs	r0, #60
	bl	sub_08013560
	b.n	.L_08125f92
.L_08125f8c:
	ldr	r0, [sp, #8]
	bl	sub_08118bcc
.L_08125f92:
	ldr	r0, [pc, #28]
	bl	sub_08014644
	ldr	r0, [pc, #24]
	bl	sub_08014644
	ldr	r2, [pc, #28]
	movs	r3, #0
	strh	r3, [r2, #2]
	movs	r0, #2
	movs	r1, #0
	movs	r2, #0
	bl	sub_08013438
	b.n	.L_081261b8
	.4byte 0x08125c95
	.4byte 0x08125cfd
	.4byte 0x08125d69
	.2byte 0x1120
	.2byte 0x0300
.L_08125fc0:
	ldr	r5, [r5, #36]
	movs	r3, #1
	str	r5, [sp, #0]
	add	r5, sp, #40
	str	r3, [r6, #12]
	movs	r3, #0
	str	r3, [r6, #16]
	adds	r0, r5, #0
	bl	sub_0811a038
	mov	r8, r0
	lsls	r0, r0, #1
	adds	r0, r5, r0
	bl	sub_0811a0b0
	add	r8, r0
	mov	r1, r8
	movs	r6, #0
	cmp	r1, #0
	beq.n	.L_0812603a
	movs	r2, #102
	movs	r3, #114
	adds	r2, #255
	adds	r3, #255
	mov	fp, r2
	mov	sl, r5
	mov	r9, r3
.L_08125ff6:
	mov	r0, sl
	ldrh	r5, [r0, #0]
	movs	r1, #2
	adds	r0, r5, #0
	add	sl, r1
	bl	sub_0811be3c
	adds	r7, r0, #0
	adds	r0, r5, #0
	bl	sub_08016ca4
	movs	r2, #165
	lsls	r2, r2, #1
	adds	r3, r0, r2
	ldrh	r0, [r3, #0]
	cmp	r0, fp
	beq.n	.L_08126034
	cmp	r0, r9
	beq.n	.L_08126034
	movs	r3, #118
	adds	r3, #255
	cmp	r0, r3
	beq.n	.L_08126034
	movs	r1, #188
	lsls	r1, r1, #1
	cmp	r0, r1
	beq.n	.L_08126034
	movs	r3, #179
	lsls	r3, r3, #8
	adds	r3, #51
	str	r3, [r7, #24]
.L_08126034:
	adds	r6, #1
	cmp	r6, r8
	bne.n	.L_08125ff6
.L_0812603a:
	ldr	r1, [pc, #148]
	ldr	r0, [pc, #148]
	ldrh	r3, [r0, #0]
	adds	r4, r3, #0
	strh	r0, [r0, #0]
	ldrh	r2, [r1, #0]
	cmp	r2, #31
	bgt.n	.L_0812606c
	lsls	r3, r2, #1
	adds	r3, r3, r2
	adds	r2, #1
	lsls	r3, r3, #2
	strh	r2, [r1, #0]
	movs	r2, #192
	adds	r3, r3, r1
	lsls	r2, r2, #7
	adds	r3, #4
	adds	r2, #65
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #19
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #10
	str	r2, [r3, #0]
.L_0812606c:
	strh	r4, [r0, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r0, [sp, #0]
	ldr	r2, [pc, #96]
	movs	r1, #206
	lsls	r1, r1, #3
	add	r6, sp, #104
	adds	r3, r0, r1
	movs	r0, #0
	strh	r2, [r3, #0]
	mov	r8, r0
	adds	r1, r6, #0
	movs	r0, #2
	add	r5, sp, #68
	bl	sub_0811a31c
	ldr	r1, [pc, #56]
	str	r0, [r5, #20]
	mov	sl, r1
	lsls	r0, r0, #1
	mov	r2, sl
	adds	r0, #36
	strh	r2, [r5, r0]
	movs	r1, #0
	adds	r0, r6, #0
	bl	sub_0811b9d4
	adds	r0, r5, #0
	bl	sub_08138028
	movs	r0, #0
	movs	r1, #0
	movs	r2, #0
	movs	r3, #100
	bl	sub_08126804
	ldr	r0, [sp, #4]
	mov	r3, r8
	ldr	r2, [pc, #28]
	str	r3, [r0, #0]
	movs	r1, #32
	movs	r0, #2
	bl	sub_08013438
	movs	r0, #1
	b.n	.L_081260e0
	.4byte 0x000000ff
	.4byte 0x020038e0
	.4byte 0x04000208
	.4byte 0x00000064
	.2byte 0x5d69
	.2byte 0x0812
.L_081260e0:
	bl	sub_08013560
	movs	r0, #20
	bl	sub_08013560
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #36]
	movs	r5, #128
	adds	r3, #65
	lsls	r5, r5, #19
	ldrb	r0, [r3, #0]
	adds	r5, #8
	bl	sub_08038128
	adds	r0, r5, #0
	movs	r1, #2
	bl	sub_08013d0c
	adds	r0, r5, #0
	movs	r1, #0
	bl	sub_08013c58
	ldr	r3, [pc, #56]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #80
	add	r1, sp, #12
	strh	r3, [r2, #0]
	movs	r0, #3
	mov	r8, r1
	bl	sub_0811a31c
	adds	r7, r0, #0
	lsls	r3, r7, #1
	mov	r2, r8
	mov	r0, sl
	strh	r0, [r2, r3]
	movs	r1, #0
	mov	r0, r8
	bl	sub_0811b9d4
	movs	r0, #1
	mov	r1, r8
	bl	sub_0811a31c
	adds	r7, r0, #0
	movs	r6, #0
	cmp	r7, #0
	beq.n	.L_0812615e
	b.n	.L_0812614c
	movs	r0, r0
	.2byte 0x3f40
	.2byte 0x0000
.L_0812614c:
	mov	r5, r8
.L_0812614e:
	ldrh	r0, [r5, #0]
	movs	r1, #1
	adds	r6, #1
	adds	r5, #2
	bl	sub_08126ae4
	cmp	r6, r7
	bne.n	.L_0812614e
.L_0812615e:
	movs	r1, #128
	lsls	r1, r1, #19
	ldr	r5, [pc, #36]
	adds	r1, #82
	movs	r6, #0
	mov	sl, r1
.L_0812616a:
	adds	r3, r6, #0
	orrs	r3, r5
	mov	r2, sl
	strh	r3, [r2, #0]
	movs	r0, #1
	adds	r6, #1
	bl	sub_08013560
	cmp	r6, #16
	bne.n	.L_0812616a
	movs	r6, #0
	cmp	r7, #0
	beq.n	.L_0812619c
	mov	r5, r8
	b.n	.L_0812618c
	.2byte 0x1000
	.2byte 0x0000
.L_0812618c:
	ldrh	r0, [r5, #0]
	movs	r1, #0
	adds	r6, #1
	adds	r5, #2
	bl	sub_08126ae4
	cmp	r6, r7
	bne.n	.L_0812618c
.L_0812619c:
	ldr	r0, [sp, #8]
	bl	sub_08118bcc
	ldr	r2, [pc, #88]
	movs	r3, #0
	strh	r3, [r2, #2]
	movs	r0, #1
	bl	sub_08013560
	movs	r0, #2
	movs	r1, #0
	movs	r2, #0
	bl	sub_08013438
.L_081261b8:
	ldr	r5, [pc, #56]
	movs	r6, #128
	lsls	r6, r6, #19
	movs	r1, #0
	movs	r2, #0
	adds	r6, #10
	movs	r0, #2
	bl	sub_08013438
	strh	r5, [r6, #0]
	movs	r0, #1
	bl	sub_08013560
	strh	r5, [r6, #0]
	movs	r1, #128
	lsls	r1, r1, #19
	adds	r1, #8
	ldrh	r2, [r1, #0]
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #253
	ands	r3, r2
	ldr	r2, [pc, #20]
	strh	r3, [r1, #0]
	movs	r3, #8
	strh	r3, [r2, #4]
	ldr	r3, [pc, #8]
	movs	r2, #128
	b.n	.L_08126200
	movs	r0, r0
	.4byte 0x00001f83
	.4byte 0x00001541
	.2byte 0x1120
	.2byte 0x0300
.L_08126200:
	lsls	r2, r2, #19
	strh	r3, [r2, #0]
	movs	r0, #168
	bl	sub_0801314c
	add	sp, #156
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	push	{r5, r6, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r0, [r3, #36]
	movs	r3, #160
	lsls	r3, r3, #3
	adds	r3, #108
	sub	sp, #4
	adds	r4, r1, #0
	adds	r5, r0, r3
	mov	r6, sp
	ldr	r3, [pc, #88]
	ldrh	r2, [r3, #0]
	str	r2, [r6, #0]
	strh	r3, [r3, #0]
	cmp	r4, #0
	bne.n	.L_08126254
	movs	r3, #128
	movs	r1, #160
	movs	r2, #128
	lsls	r3, r3, #19
	lsls	r1, r1, #19
	lsls	r2, r2, #24
	adds	r3, #212
	adds	r0, r5, #0
	adds	r1, #192
	adds	r2, #128
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	b.n	.L_0812627c
.L_08126254:
	movs	r3, #192
	lsls	r3, r3, #3
	adds	r3, #108
	adds	r0, r0, r3
	lsls	r3, r4, #4
	adds	r3, r3, r4
	lsls	r3, r3, #4
	adds	r3, r3, r4
	movs	r2, #128
	lsls	r3, r3, #2
	movs	r1, #160
	lsls	r2, r2, #9
	subs	r2, r2, r3
	lsls	r1, r1, #19
	str	r2, [r0, #0]
	adds	r1, #192
	adds	r0, r5, #0
	movs	r3, #128
	bl	sub_08118410
.L_0812627c:
	ldr	r2, [r6, #0]
	ldr	r3, [pc, #8]
	add	sp, #4
	strh	r2, [r3, #0]
	pop	{r5, r6, pc}
	movs	r0, r0
	.2byte 0x0208
	.2byte 0x0400
	push	{r5, r6, r7, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #176
	ldr	r6, [r3, #0]
	adds	r7, r0, #0
	ldr	r3, [r6, #8]
	adds	r5, r2, #0
	cmp	r3, #0
	bne.n	.L_081262ac
	movs	r1, #192
	lsls	r1, r1, #4
	ldr	r0, [pc, #200]
	adds	r1, #255
	bl	sub_080145a8
.L_081262ac:
	str	r7, [r6, #8]
	cmp	r7, #1
	bne.n	.L_081262e8
	ldr	r1, [pc, #192]
	ldr	r0, [pc, #192]
	ldrh	r3, [r0, #0]
	adds	r4, r3, #0
	strh	r0, [r0, #0]
	ldrh	r2, [r1, #0]
	cmp	r2, #31
	bgt.n	.L_081262e6
	lsls	r3, r2, #1
	adds	r3, r3, r2
	adds	r2, #1
	lsls	r3, r3, #2
	strh	r2, [r1, #0]
	movs	r2, #248
	adds	r3, r3, r1
	lsls	r2, r2, #5
	adds	r3, #4
	adds	r2, #131
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #10
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #10
	str	r2, [r3, #0]
.L_081262e6:
	strh	r4, [r0, #0]
.L_081262e8:
	movs	r3, #128
	movs	r1, #160
	movs	r2, #128
	lsls	r3, r3, #19
	lsls	r1, r1, #19
	lsls	r2, r2, #24
	adds	r3, #212
	ldr	r0, [pc, #132]
	adds	r1, #160
	adds	r2, #16
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r2, [pc, #124]
	movs	r3, #160
	ldrh	r2, [r2, #0]
	lsls	r3, r3, #19
	adds	r3, #188
	strh	r2, [r3, #0]
	cmp	r5, #128
	bne.n	.L_08126332
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r0, [r3, #36]
	movs	r2, #160
	lsls	r2, r2, #3
	adds	r2, #108
	adds	r0, r0, r2
	movs	r3, #128
	movs	r2, #128
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	adds	r1, #32
	adds	r2, #128
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	b.n	.L_081263ae
.L_08126332:
	cmp	r5, #0
	beq.n	.L_081263ae
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #36]
	movs	r2, #160
	lsls	r2, r2, #3
	adds	r2, #108
	movs	r4, #160
	ldr	r7, [pc, #36]
	adds	r2, r2, r3
	lsls	r4, r4, #19
	mov	ip, r2
	movs	r6, #0
	adds	r4, #192
	movs	r0, #0
.L_08126352:
	mov	r2, ip
	ldrh	r3, [r0, r2]
	movs	r1, #31
	ands	r1, r3
	lsls	r3, r3, #16
	lsrs	r2, r3, #21
	lsrs	r3, r3, #26
	ands	r2, r7
	ands	r3, r7
	cmp	r1, r5
	ble.n	.L_08126384
	subs	r1, r1, r5
	b.n	.L_08126386
	.4byte 0x0000001f
	.4byte 0x08125c0d
	.4byte 0x020038e0
	.4byte 0x04000208
	.4byte 0x05000200
	.2byte 0x01e8
	.2byte 0x0500
.L_08126384:
	movs	r1, #0
.L_08126386:
	cmp	r2, r5
	ble.n	.L_0812638e
	subs	r2, r2, r5
	b.n	.L_08126390
.L_0812638e:
	movs	r2, #0
.L_08126390:
	cmp	r3, r5
	ble.n	.L_08126398
	subs	r3, r3, r5
	b.n	.L_0812639a
.L_08126398:
	movs	r3, #0
.L_0812639a:
	lsls	r3, r3, #10
	lsls	r2, r2, #5
	orrs	r3, r2
	orrs	r3, r1
	adds	r6, #1
	strh	r3, [r4, #0]
	adds	r0, #2
	adds	r4, #2
	cmp	r6, #128
	bne.n	.L_08126352
.L_081263ae:
	ldr	r0, [pc, #12]
	bl	sub_08125b78
	ldr	r0, [pc, #8]
	bl	sub_08125bb8
	pop	{r5, r6, r7, pc}
	.4byte 0x06003800
	.2byte 0xf800
	.2byte 0x0600
	push	{r5, r6, lr}
	movs	r5, #168
	lsls	r5, r5, #2
	adds	r1, r5, #0
	movs	r0, #40
	bl	sub_08014d00
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #176
	ldr	r6, [r3, #0]
	ldr	r3, [pc, #16]
	movs	r2, #0
	adds	r1, r5, #0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2300
	str	r3, [r6, #8]
	pop	{r5, r6, pc}
	movs	r0, r0
	.2byte 0x0260
	.2byte 0x0300
	push	{lr}
	movs	r0, #40
	bl	sub_0801314c
	pop	{pc}
	.2byte 0x0000
