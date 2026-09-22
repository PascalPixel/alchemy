.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_08002064, 0x08002064
	.set sub_0800206c, 0x0800206c
	.set sub_08002090, 0x08002090
	.set sub_08002096, 0x08002096
	.set sub_0801314c, 0x0801314c
	.set sub_080132fc, 0x080132fc
	.set sub_08013300, 0x08013300
	.set sub_08013560, 0x08013560
	.set sub_080144c0, 0x080144c0
	.set sub_08014878, 0x08014878
	.set sub_08014cc0, 0x08014cc0
	.set sub_08014d00, 0x08014d00
	.set sub_08014de4, 0x08014de4
	.set sub_080156e8, 0x080156e8
	.set sub_08015778, 0x08015778
	.set sub_0801587c, 0x0801587c
	.set sub_08038000, 0x08038000
	.set sub_08038010, 0x08038010
	.set sub_08038018, 0x08038018
	.set sub_08038080, 0x08038080
	.set sub_080380b0, 0x080380b0
	.set sub_080ad1d8, 0x080ad1d8
	.set sub_080ad290, 0x080ad290
	.set sub_081ac028, 0x081ac028
	.set sub_081c0010, 0x081c0010
	.global Overlay_081ac0f8
Overlay_081ac0f8:
.L_081ac0f8:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r7, r0, #0
	adds	r6, r1, #0
	adds	r5, r2, #0
	cmp	r3, #0
	ble.n	.L_081ac154
	movs	r1, #31
	mov	r8, r1
	movs	r2, #248
	movs	r1, #248
	lsls	r2, r2, #2
	lsls	r1, r1, #7
	mov	lr, r2
	mov	ip, r1
	adds	r0, r3, #0
.L_081ac11a:
	ldrh	r4, [r7, #0]
	mov	r2, r8
	adds	r3, r4, #0
	ands	r3, r2
	mov	r1, lr
	adds	r2, r4, #0
	ands	r2, r1
	muls	r3, r5
	mov	r1, ip
	muls	r2, r5
	ands	r1, r4
	muls	r1, r5
	lsrs	r4, r3, #16
	mov	r3, r8
	ands	r4, r3
	lsrs	r2, r2, #16
	mov	r3, lr
	ands	r2, r3
	orrs	r4, r2
	lsrs	r1, r1, #16
	mov	r2, ip
	ands	r1, r2
	orrs	r4, r1
	subs	r0, #1
	strh	r4, [r6, #0]
	adds	r7, #2
	adds	r6, #2
	cmp	r0, #0
	bne.n	.L_081ac11a
.L_081ac154:
	movs	r0, #0
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r5, [pc, #372]
	movs	r1, #192
	lsls	r1, r1, #3
	adds	r1, #14
	movs	r0, #100
	add	sp, r5
	bl	sub_08014cc0
	movs	r1, #128
	lsls	r1, r1, #2
	movs	r0, #96
	bl	sub_08014cc0
	movs	r1, #246
	lsls	r1, r1, #7
	str	r0, [sp, #116]
	adds	r1, #124
	movs	r0, #92
	bl	sub_08014d00
	movs	r1, #192
	lsls	r1, r1, #3
	str	r0, [sp, #112]
	adds	r1, #20
	movs	r0, #180
	bl	sub_08014d00
	movs	r1, #76
	str	r0, [sp, #108]
	movs	r0, #48
	bl	sub_08014d00
	str	r0, [sp, #104]
	ldr	r0, [pc, #308]
	bl	sub_080132fc
	bl	sub_081ac028
	bl	sub_080144c0
	ldr	r2, [pc, #300]
	movs	r3, #0
	strb	r3, [r2, #0]
	ldr	r6, [pc, #296]
	movs	r0, #0
	mov	fp, r0
	movs	r5, #0
	movs	r4, #0
.L_081ac1ca:
	movs	r1, #0
	adds	r0, r4, #0
.L_081ac1ce:
	mov	r3, fp
	adds	r2, r3, r6
	movs	r7, #2
	adds	r3, r1, r0
	adds	r1, #1
	strh	r3, [r2, #0]
	add	fp, r7
	cmp	r1, #32
	bne.n	.L_081ac1ce
	adds	r5, #1
	adds	r4, #30
	cmp	r5, #20
	bne.n	.L_081ac1ca
	ldr	r0, [pc, #260]
	bl	sub_08013300
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	movs	r1, #160
	lsls	r2, r2, #24
	adds	r4, r0, #0
	adds	r3, #212
	lsls	r1, r1, #19
	adds	r2, #112
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r2, #132
	movs	r0, #160
	lsls	r2, r2, #24
	lsls	r0, r0, #19
	add	r1, sp, #256
	adds	r2, #128
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r0, #224
	lsls	r0, r0, #1
	adds	r4, r4, r0
	adds	r0, r4, #0
	ldr	r1, [pc, #212]
	bl	sub_0801587c
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #212
	ldr	r0, [pc, #200]
	ldr	r1, [pc, #204]
	ldr	r2, [pc, #204]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r0, [pc, #204]
	bl	sub_08013300
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r4, r0, #0
	adds	r3, #212
	ldr	r1, [sp, #116]
	adds	r2, #128
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r2, #132
	lsls	r2, r2, #24
	adds	r0, r4, #0
	ldr	r1, [pc, #176]
	adds	r2, #128
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r1, #128
	lsls	r1, r1, #2
	adds	r4, r4, r1
	adds	r0, r4, #0
	ldr	r1, [pc, #144]
	bl	sub_0801587c
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #212
	ldr	r0, [pc, #132]
	ldr	r1, [pc, #148]
	ldr	r2, [pc, #152]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r5, #128
	lsls	r5, r5, #1
	adds	r3, r5, #0
	ldr	r1, [pc, #132]
	ldr	r0, [sp, #116]
	movs	r2, #0
	bl	.L_081ac0f8
	movs	r1, #160
	adds	r3, r5, #0
	lsls	r1, r1, #19
	movs	r2, #0
	add	r0, sp, #256
	bl	.L_081ac0f8
	ldr	r3, [pc, #48]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #10
	strh	r3, [r2, #0]
	ldr	r3, [pc, #40]
	adds	r2, #62
	strh	r3, [r2, #0]
	ldr	r3, [pc, #40]
	adds	r2, #2
	strh	r3, [r2, #0]
	ldr	r3, [pc, #36]
	subs	r2, #74
	strh	r3, [r2, #0]
	ldr	r3, [pc, #36]
	adds	r2, #80
	strh	r3, [r2, #0]
	ldr	r3, [pc, #32]
	adds	r2, #2
	strh	r3, [r2, #0]
	ldr	r3, [pc, #80]
	movs	r2, #0
	strh	r2, [r3, #4]
	strh	r2, [r3, #6]
	b.n	.L_081ac314
	.4byte 0x00000686
	.4byte 0x00003737
	.4byte 0x00002723
	.4byte 0x00003340
	.4byte 0x00003f44
	.4byte 0x00000810
	.4byte 0xfffffd00
	.4byte 0x0000000c
	.4byte 0x0300120c
	.4byte 0x06003000
	.4byte 0x00000084
	.4byte 0x02010000
	.4byte 0x06004000
	.4byte 0x84002580
	.4byte 0x00000086
	.4byte 0x05000200
	.4byte 0x06010000
	.4byte 0x84001f00
	.2byte 0x1120
	.2byte 0x0300
.L_081ac314:
	movs	r3, #128
	ldr	r1, [pc, #56]
	lsls	r3, r3, #19
	adds	r3, #20
	strh	r2, [r3, #0]
	adds	r3, #2
	strh	r1, [r3, #0]
	adds	r3, #2
	strh	r2, [r3, #0]
	adds	r3, #2
	strh	r1, [r3, #0]
	ldr	r1, [pc, #40]
	ldr	r2, [pc, #40]
	adds	r3, #38
	strh	r1, [r3, #0]
	adds	r3, #4
	strh	r2, [r3, #0]
	subs	r3, #2
	strh	r1, [r3, #0]
	adds	r3, #4
	strh	r2, [r3, #0]
	mov	r4, sp
	movs	r3, #1
	movs	r2, #0
	adds	r4, #152
	negs	r3, r3
	str	r2, [sp, #96]
	str	r2, [sp, #88]
	str	r3, [sp, #84]
	b.n	.L_081ac35c
	.4byte 0x0000ff60
	.4byte 0x000000f0
	.2byte 0x00a0
	.2byte 0x0000
.L_081ac35c:
	str	r2, [sp, #80]
	str	r2, [sp, #76]
	str	r4, [sp, #44]
	str	r4, [sp, #20]
	ldr	r5, [sp, #112]
	mov	fp, r2
	movs	r6, #127
	movs	r7, #31
.L_081ac36c:
	bl	sub_08014878
	ands	r0, r6
	subs	r0, #64
	lsls	r0, r0, #16
	str	r0, [r5, #0]
	bl	sub_08014878
	ands	r0, r7
	lsls	r0, r0, #16
	str	r0, [r5, #4]
	bl	sub_08014878
	ands	r0, r6
	subs	r0, #64
	lsls	r0, r0, #16
	str	r0, [r5, #8]
	bl	sub_08014878
	ands	r0, r6
	subs	r0, #64
	lsls	r0, r0, #14
	str	r0, [r5, #12]
	bl	sub_08014878
	ands	r0, r7
	adds	r0, #32
	lsls	r0, r0, #12
	str	r0, [r5, #16]
	bl	sub_08014878
	ands	r0, r6
	subs	r0, #64
	lsls	r0, r0, #14
	movs	r2, #0
	str	r0, [r5, #20]
	str	r2, [r5, #24]
	ldr	r1, [sp, #20]
	movs	r3, #150
	lsls	r3, r3, #1
	stmia	r1!, {r3}
	movs	r3, #1
	add	fp, r3
	adds	r0, r1, #0
	mov	r4, fp
	str	r0, [sp, #20]
	adds	r5, #28
	cmp	r4, #2
	bne.n	.L_081ac36c
	mov	r5, sp
	add	r3, sp, #224
	adds	r5, #144
	str	r2, [r3, #12]
	str	r2, [r3, #8]
	str	r2, [r3, #4]
	str	r2, [r3, #0]
	str	r5, [sp, #56]
	str	r2, [r5, #4]
	str	r2, [sp, #144]
	bl	sub_08014878
	movs	r1, #6
	bl	sub_0800206c
	mov	r6, sp
	adds	r6, #136
	str	r0, [sp, #136]
	str	r6, [sp, #48]
.L_081ac3f4:
	bl	sub_08014878
	movs	r1, #6
	bl	sub_0800206c
	ldr	r7, [sp, #48]
	str	r0, [r7, #4]
	ldr	r3, [sp, #136]
	cmp	r3, r0
	beq.n	.L_081ac3f4
	ldr	r0, [sp, #112]
	movs	r3, #160
	lsls	r3, r3, #14
	ldr	r2, [pc, #772]
	str	r3, [r0, #0]
	ldr	r3, [pc, #772]
	movs	r1, #192
	lsls	r1, r1, #15
	str	r1, [r0, #4]
	str	r2, [r0, #8]
	str	r3, [r0, #28]
	str	r1, [r0, #32]
	str	r2, [r0, #36]
	bl	sub_08038000
	movs	r1, #6
	str	r1, [sp, #0]
	mov	r8, r1
	movs	r2, #12
	movs	r1, #0
	movs	r3, #3
	movs	r0, #18
	bl	sub_08038010
	movs	r3, #128
	ldr	r2, [sp, #108]
	ldr	r5, [pc, #732]
	lsls	r3, r3, #3
	adds	r3, #196
	adds	r6, r2, r3
	adds	r1, r0, #0
	str	r1, [r6, #0]
	adds	r0, r5, #0
	movs	r2, #48
	movs	r3, #0
	bl	sub_08038080
	ldr	r3, [pc, #716]
	ldr	r2, [r6, #0]
	ldr	r0, [r3, #16]
	movs	r1, #6
	movs	r3, #0
	str	r3, [sp, #0]
	bl	sub_080380b0
	mov	r4, r8
	movs	r1, #16
	movs	r2, #8
	movs	r3, #4
	movs	r0, #22
	str	r4, [sp, #0]
	bl	sub_08038010
	adds	r1, r0, #0
	ldr	r7, [sp, #108]
	movs	r0, #128
	lsls	r0, r0, #3
	adds	r0, #204
	adds	r6, r7, r0
	adds	r0, r5, #0
	str	r1, [r6, #0]
	adds	r0, #10
	movs	r2, #0
	movs	r3, #0
	adds	r5, #11
	bl	sub_08038080
	ldr	r1, [r6, #0]
	movs	r2, #0
	adds	r0, r5, #0
	movs	r3, #8
	bl	sub_08038080
	ldr	r2, [sp, #104]
	movs	r1, #0
	adds	r2, #12
	str	r1, [sp, #100]
	str	r2, [sp, #40]
.L_081ac4a4:
	ldr	r4, [sp, #100]
	movs	r3, #0
	mov	r8, r3
	cmp	r4, #16
	bgt.n	.L_081ac4ce
	movs	r6, #128
	lsls	r5, r4, #12
	lsls	r6, r6, #1
	ldr	r1, [pc, #620]
	adds	r2, r5, #0
	adds	r3, r6, #0
	ldr	r0, [sp, #116]
	bl	.L_081ac0f8
	movs	r1, #160
	add	r0, sp, #256
	lsls	r1, r1, #19
	adds	r2, r5, #0
	adds	r3, r6, #0
	bl	.L_081ac0f8
.L_081ac4ce:
	bl	sub_08014de4
	ldr	r0, [sp, #104]
	ldr	r1, [sp, #40]
	bl	sub_080156e8
	ldr	r3, [pc, #588]
	ldr	r5, [sp, #108]
	movs	r2, #196
	str	r3, [r5, r2]
	movs	r3, #214
	movs	r2, #200
	lsls	r3, r3, #2
	str	r3, [r5, r2]
	ldr	r3, [pc, #576]
	movs	r2, #204
	str	r3, [r5, r2]
	movs	r3, #230
	movs	r2, #208
	lsls	r3, r3, #2
	str	r3, [r5, r2]
	ldr	r3, [pc, #564]
	movs	r2, #212
	str	r3, [r5, r2]
	movs	r3, #232
	movs	r2, #216
	lsls	r3, r3, #2
	str	r3, [r5, r2]
	ldr	r3, [pc, #556]
	movs	r2, #220
	movs	r1, #234
	str	r3, [r5, r2]
	lsls	r1, r1, #2
	movs	r3, #224
	str	r1, [r5, r3]
	ldr	r3, [pc, #544]
	movs	r2, #228
	str	r3, [r5, r2]
	movs	r3, #232
	str	r1, [r5, r3]
	movs	r6, #0
	mov	fp, r6
	ldr	r2, [sp, #48]
	ldr	r6, [pc, #532]
	movs	r5, #236
	lsls	r5, r5, #2
	movs	r4, #32
	movs	r0, #240
	movs	r1, #236
.L_081ac530:
	ldr	r7, [sp, #108]
	adds	r3, r4, #0
	orrs	r3, r6
	str	r3, [r7, r1]
	adds	r4, #16
	ldmia	r2!, {r3}
	adds	r1, #8
	lsls	r3, r3, #3
	adds	r3, r3, r5
	str	r3, [r7, r0]
	movs	r3, #1
	add	fp, r3
	mov	r7, fp
	adds	r0, #8
	cmp	r7, #2
	bne.n	.L_081ac530
	ldr	r3, [sp, #96]
	movs	r0, #7
	subs	r3, #2
	str	r0, [sp, #68]
	cmp	r3, #1
	bhi.n	.L_081ac62a
	movs	r1, #0
	mov	fp, r1
	movs	r2, #128
	movs	r1, #224
	lsls	r1, r1, #14
	movs	r6, #0
	lsls	r2, r2, #1
	movs	r7, #252
.L_081ac56c:
	ldr	r4, [sp, #88]
	mov	r3, fp
	lsls	r5, r3, #2
	cmp	r4, r6
	bne.n	.L_081ac586
	movs	r0, #48
	adds	r0, #255
	str	r1, [sp, #16]
	str	r2, [sp, #12]
	bl	sub_081c0010
	ldr	r2, [sp, #12]
	ldr	r1, [sp, #16]
.L_081ac586:
	ldr	r0, [sp, #88]
	cmp	r0, r6
	blt.n	.L_081ac5a8
	ldr	r3, [pc, #432]
	ldr	r4, [sp, #108]
	orrs	r3, r1
	str	r3, [r4, r7]
	add	r3, sp, #224
	ldr	r3, [r3, r5]
	adds	r7, #8
	lsls	r3, r3, #5
	adds	r3, #152
	str	r3, [r4, r2]
	ldr	r5, [sp, #68]
	adds	r2, #8
	adds	r5, #1
	str	r5, [sp, #68]
.L_081ac5a8:
	movs	r3, #1
	movs	r0, #128
	add	fp, r3
	lsls	r0, r0, #14
	mov	r4, fp
	adds	r1, r1, r0
	adds	r6, #5
	cmp	r4, #4
	bne.n	.L_081ac56c
	ldr	r5, [sp, #88]
	cmp	r5, #19
	ble.n	.L_081ac62a
	ldr	r6, [sp, #84]
	cmp	r6, #0
	blt.n	.L_081ac62a
	ldr	r3, [pc, #380]
	movs	r7, #0
	ldrb	r2, [r3, r6]
	mov	fp, r7
	cmp	r2, #0
	beq.n	.L_081ac62a
	ldr	r3, [pc, #372]
	ldr	r1, [sp, #68]
	ldrb	r3, [r3, r6]
	movs	r0, #96
	lsls	r3, r3, #3
	adds	r4, r3, #0
	lsls	r3, r1, #3
	adds	r1, r3, #0
	mov	ip, r0
	adds	r0, r3, #0
	ldr	r3, [pc, #356]
	adds	r5, r2, #0
	movs	r7, #128
	mov	lr, r3
	movs	r3, #158
	lsls	r2, r5, #4
	lsls	r7, r7, #1
	lsls	r3, r3, #2
	adds	r4, #152
	adds	r0, #200
	adds	r1, #196
	adds	r7, #255
	subs	r2, r3, r2
.L_081ac600:
	adds	r3, r2, #0
	ands	r3, r7
	mov	r6, ip
	lsls	r3, r3, #16
	orrs	r3, r6
	mov	r6, lr
	orrs	r3, r6
	ldr	r6, [sp, #108]
	adds	r2, #32
	str	r3, [r6, r1]
	str	r4, [r6, r0]
	ldr	r3, [sp, #68]
	movs	r6, #1
	adds	r3, #1
	add	fp, r6
	adds	r0, #8
	adds	r1, #8
	str	r3, [sp, #68]
	adds	r4, #32
	cmp	fp, r5
	bne.n	.L_081ac600
.L_081ac62a:
	ldr	r7, [sp, #96]
	cmp	r7, #2
	beq.n	.L_081ac632
	b.n	.L_081ac758
.L_081ac632:
	ldr	r0, [sp, #88]
	cmp	r0, #20
	bne.n	.L_081ac6d0
	ldr	r1, [sp, #80]
	cmp	r1, #0
	ble.n	.L_081ac6d0
	adds	r0, r1, #0
	bl	sub_080ad1d8
	ldr	r3, [sp, #80]
	movs	r2, #1
	str	r2, [sp, #72]
	cmp	r3, #9
	ble.n	.L_081ac67c
	ldr	r5, [sp, #80]
	movs	r4, #2
	str	r4, [sp, #72]
	cmp	r5, #99
	ble.n	.L_081ac67c
	movs	r0, #186
	ldr	r7, [sp, #80]
	lsls	r0, r0, #2
	movs	r6, #3
	adds	r0, #255
	str	r6, [sp, #72]
	cmp	r7, r0
	ble.n	.L_081ac67c
	movs	r3, #156
	ldr	r2, [sp, #80]
	lsls	r3, r3, #6
	movs	r1, #4
	adds	r3, #15
	str	r1, [sp, #72]
	cmp	r2, r3
	ble.n	.L_081ac67c
	movs	r4, #5
	str	r4, [sp, #72]
.L_081ac67c:
	ldr	r5, [sp, #72]
	movs	r0, #8
	lsrs	r3, r5, #31
	adds	r3, r5, r3
	asrs	r3, r3, #1
	subs	r0, r0, r3
	adds	r2, r5, #0
	movs	r3, #6
	str	r3, [sp, #0]
	adds	r2, #13
	movs	r1, #16
	movs	r3, #3
	bl	sub_08038010
	ldr	r6, [sp, #108]
	movs	r7, #153
	lsls	r7, r7, #3
	adds	r5, r6, r7
	ldr	r6, [pc, #172]
	adds	r1, r0, #0
	str	r1, [r5, #0]
	adds	r0, r6, #0
	movs	r2, #0
	movs	r3, #0
	bl	sub_08038080
	ldr	r0, [sp, #72]
	ldr	r1, [r5, #0]
	lsls	r2, r0, #3
	adds	r2, #56
	subs	r0, r6, #1
	movs	r3, #0
	bl	sub_08038080
	movs	r1, #0
	ldr	r2, [r5, #0]
	ldr	r0, [sp, #80]
	str	r1, [sp, #0]
	movs	r3, #56
	ldr	r1, [sp, #72]
	bl	sub_080380b0
.L_081ac6d0:
	ldr	r2, [sp, #76]
	cmp	r2, #0
	ble.n	.L_081ac6da
	subs	r2, #1
	str	r2, [sp, #76]
.L_081ac6da:
	ldr	r3, [sp, #88]
	cmp	r3, #19
	ble.n	.L_081ac70a
	ldr	r3, [pc, #112]
	movs	r2, #1
	ldr	r3, [r3, #12]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_081ac70a
	movs	r0, #112
	bl	sub_081c0010
	ldr	r4, [sp, #80]
	cmp	r4, #0
	ble.n	.L_081ac702
	movs	r5, #3
	movs	r6, #0
	str	r5, [sp, #96]
	str	r6, [sp, #76]
	b.n	.L_081ac70a
.L_081ac702:
	movs	r0, #0
	str	r0, [sp, #96]
	movs	r7, #1
	mov	r8, r7
.L_081ac70a:
	ldr	r1, [sp, #88]
	adds	r1, #1
	str	r1, [sp, #88]
	b.n	.L_081ac980
	movs	r0, r0
	.4byte 0xff600000
	.4byte 0xffd80000
	.4byte 0x00000d69
	.4byte 0x02000240
	.4byte 0x05000200
	.4byte 0xc0006000
	.4byte 0x40102020
	.4byte 0x40102030
	.4byte 0x40042020
	.4byte 0x40042030
	.4byte 0x40202000
	.4byte 0x80002040
	.4byte 0x081ad368
	.4byte 0x081ad36c
	.4byte 0x80002000
	.4byte 0x00000d6a
	.2byte 0x1150
	.2byte 0x0300
.L_081ac758:
	ldr	r2, [sp, #96]
	cmp	r2, #3
	bne.n	.L_081ac7f4
	ldr	r3, [sp, #76]
	cmp	r3, #0
	bne.n	.L_081ac798
	movs	r4, #0
	ldr	r5, [sp, #108]
	str	r4, [sp, #80]
	movs	r6, #153
	lsls	r6, r6, #3
	adds	r3, r5, r6
	ldr	r2, [r3, #0]
	movs	r0, #0
	ldr	r1, [sp, #72]
	movs	r3, #56
	str	r4, [sp, #0]
	bl	sub_080380b0
	movs	r7, #128
	ldr	r3, [pc, #704]
	ldr	r1, [sp, #80]
	lsls	r7, r7, #3
	adds	r7, #196
	ldr	r0, [r3, #16]
	adds	r3, r5, r7
	ldr	r2, [r3, #0]
	str	r1, [sp, #0]
	movs	r3, #0
	movs	r1, #6
	bl	sub_080380b0
.L_081ac798:
	ldr	r2, [sp, #76]
	cmp	r2, #15
	bne.n	.L_081ac7ec
	ldr	r4, [sp, #108]
	movs	r3, #0
	str	r3, [sp, #96]
	movs	r5, #153
	lsls	r5, r5, #3
	adds	r3, r4, r5
	ldr	r0, [r3, #0]
	movs	r1, #1
	bl	sub_08038018
	movs	r3, #6
	str	r3, [sp, #0]
	movs	r1, #16
	movs	r2, #8
	movs	r3, #4
	movs	r0, #22
	bl	sub_08038010
	adds	r1, r0, #0
	ldr	r7, [sp, #108]
	movs	r0, #128
	ldr	r5, [pc, #636]
	lsls	r0, r0, #3
	adds	r0, #204
	adds	r6, r7, r0
	str	r1, [r6, #0]
	adds	r0, r5, #0
	movs	r2, #0
	movs	r3, #0
	bl	sub_08038080
	ldr	r1, [r6, #0]
	adds	r0, r5, #1
	movs	r2, #0
	movs	r3, #8
	bl	sub_08038080
	movs	r1, #1
	mov	r8, r1
.L_081ac7ec:
	ldr	r2, [sp, #76]
	adds	r2, #1
	str	r2, [sp, #76]
	b.n	.L_081ac980
.L_081ac7f4:
	ldr	r3, [sp, #96]
	cmp	r3, #0
	beq.n	.L_081ac7fc
	b.n	.L_081ac980
.L_081ac7fc:
	ldr	r4, [sp, #100]
	cmp	r4, #17
	bgt.n	.L_081ac804
	b.n	.L_081ac980
.L_081ac804:
	ldr	r6, [pc, #580]
	movs	r2, #64
	ldr	r3, [r6, #12]
	ldr	r3, [r6, #12]
	ldr	r3, [r6, #0]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_081ac826
	ldr	r5, [sp, #112]
	movs	r2, #128
	ldr	r3, [r5, #4]
	lsls	r2, r2, #11
	adds	r3, r3, r2
	str	r3, [r5, #4]
	ldr	r3, [r5, #32]
	adds	r3, r3, r2
	str	r3, [r5, #32]
.L_081ac826:
	ldr	r3, [r6, #0]
	movs	r2, #128
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_081ac840
	ldr	r7, [sp, #112]
	ldr	r2, [pc, #540]
	ldr	r3, [r7, #4]
	adds	r3, r3, r2
	str	r3, [r7, #4]
	ldr	r3, [r7, #32]
	adds	r3, r3, r2
	str	r3, [r7, #32]
.L_081ac840:
	ldr	r0, [sp, #112]
	ldr	r1, [pc, #528]
	ldr	r3, [r0, #4]
	cmp	r3, r1
	bgt.n	.L_081ac850
	movs	r3, #128
	lsls	r3, r3, #14
	str	r3, [r0, #4]
.L_081ac850:
	movs	r2, #160
	lsls	r2, r2, #16
	cmp	r3, r2
	ble.n	.L_081ac85c
	ldr	r3, [sp, #112]
	str	r2, [r3, #4]
.L_081ac85c:
	ldr	r4, [sp, #112]
	ldr	r3, [r4, #32]
	cmp	r3, r1
	bgt.n	.L_081ac86a
	movs	r3, #128
	lsls	r3, r3, #14
	str	r3, [r4, #32]
.L_081ac86a:
	cmp	r3, r2
	ble.n	.L_081ac872
	ldr	r5, [sp, #112]
	str	r2, [r5, #32]
.L_081ac872:
	ldr	r7, [pc, #464]
	bl	sub_080ad290
	ldr	r2, [r7, #16]
	lsls	r3, r0, #2
	adds	r3, r3, r0
	lsls	r3, r3, #1
	cmp	r2, r3
	bcs.n	.L_081ac890
	ldr	r6, [sp, #108]
	movs	r7, #128
	lsls	r7, r7, #3
	adds	r7, #204
	adds	r3, r6, r7
	b.n	.L_081ac8aa
.L_081ac890:
	ldr	r5, [r6, #12]
	movs	r3, #2
	ands	r5, r3
	cmp	r5, #0
	beq.n	.L_081ac8b6
	movs	r0, #113
	bl	sub_081c0010
	movs	r1, #128
	ldr	r0, [sp, #108]
	lsls	r1, r1, #3
	adds	r1, #204
	adds	r3, r0, r1
.L_081ac8aa:
	ldr	r0, [r3, #0]
	movs	r1, #1
	bl	sub_08038018
	bl	.L_081ad2d2
.L_081ac8b6:
	ldr	r3, [r6, #12]
	movs	r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_081ac980
	movs	r0, #151
	lsls	r0, r0, #1
	bl	sub_081c0010
	movs	r4, #128
	ldr	r2, [sp, #108]
	lsls	r4, r4, #3
	adds	r4, #204
	adds	r3, r2, r4
	ldr	r0, [r3, #0]
	movs	r1, #1
	bl	sub_08038018
	movs	r6, #1
	str	r6, [sp, #96]
	bl	sub_080ad290
	lsls	r3, r0, #2
	adds	r3, r3, r0
	lsls	r3, r3, #1
	str	r3, [sp, #92]
	ldr	r3, [r7, #16]
	ldr	r0, [sp, #92]
	cmp	r0, r3
	bls.n	.L_081ac8f4
	str	r3, [sp, #92]
.L_081ac8f4:
	ldr	r1, [sp, #92]
	negs	r0, r1
	bl	sub_080ad1d8
	movs	r4, #128
	ldr	r2, [sp, #108]
	lsls	r4, r4, #3
	adds	r4, #196
	adds	r3, r2, r4
	ldr	r0, [r7, #16]
	ldr	r2, [r3, #0]
	movs	r1, #6
	movs	r3, #0
	str	r5, [sp, #0]
	bl	sub_080380b0
	ldr	r7, [sp, #44]
	ldr	r6, [sp, #112]
	movs	r5, #0
	mov	fp, r5
.L_081ac91c:
	mov	r0, fp
	cmp	r0, #1
	ble.n	.L_081ac92c
	ldr	r3, [pc, #308]
	str	r3, [r6, #8]
	movs	r3, #192
	lsls	r3, r3, #15
	str	r3, [r6, #4]
.L_081ac92c:
	bl	sub_08014878
	movs	r3, #127
	ands	r3, r0
	subs	r3, #64
	lsls	r3, r3, #12
	str	r3, [r6, #12]
	bl	sub_08014878
	movs	r3, #15
	ands	r3, r0
	adds	r3, #48
	lsls	r3, r3, #13
	str	r3, [r6, #16]
	bl	sub_08014878
	movs	r5, #63
	movs	r1, #6
	ands	r5, r0
	ldr	r0, [r6, #4]
	bl	sub_08002054
	adds	r5, #140
	lsls	r5, r5, #12
	adds	r5, r5, r0
	str	r5, [r6, #20]
	bl	sub_08014878
	movs	r1, #144
	lsls	r1, r1, #7
	bl	sub_0800206c
	movs	r1, #1
	movs	r3, #150
	add	fp, r1
	lsls	r3, r3, #1
	mov	r2, fp
	str	r0, [r6, #24]
	stmia	r7!, {r3}
	adds	r6, #28
	cmp	r2, #2
	bne.n	.L_081ac91c
.L_081ac980:
	mov	r3, r8
	cmp	r3, #1
	bne.n	.L_081ac9c0
	ldr	r4, [sp, #112]
	movs	r3, #160
	lsls	r3, r3, #14
	ldr	r2, [pc, #204]
	str	r3, [r4, #0]
	ldr	r3, [pc, #204]
	movs	r1, #192
	lsls	r1, r1, #15
	str	r1, [r4, #4]
	str	r1, [r4, #32]
	str	r2, [r4, #8]
	str	r3, [r4, #28]
	str	r2, [r4, #36]
	bl	sub_08014878
	movs	r1, #6
	bl	sub_0800206c
	str	r0, [sp, #136]
.L_081ac9ac:
	bl	sub_08014878
	movs	r1, #6
	bl	sub_0800206c
	ldr	r5, [sp, #48]
	str	r0, [r5, #4]
	ldr	r3, [sp, #136]
	cmp	r3, r0
	beq.n	.L_081ac9ac
.L_081ac9c0:
	mov	r7, sp
	mov	r1, sp
	adds	r7, #128
	movs	r0, #1
	movs	r6, #0
	adds	r1, #132
	str	r7, [sp, #52]
	str	r6, [r7, #0]
	str	r0, [sp, #132]
	str	r1, [sp, #24]
	mov	fp, r0
	mov	r8, r7
	mov	lr, r7
.L_081ac9da:
	ldr	r3, [sp, #24]
	mov	r6, fp
	ldmia	r3!, {r5}
	subs	r6, #1
	adds	r2, r3, #0
	str	r2, [sp, #24]
	cmp	r6, #0
	blt.n	.L_081aca64
	ldr	r4, [sp, #52]
	lsls	r0, r6, #2
	ldr	r3, [r0, r4]
	ldr	r7, [sp, #112]
	lsls	r2, r3, #3
	subs	r2, r2, r3
	lsls	r3, r5, #3
	subs	r3, r3, r5
	lsls	r3, r3, #2
	adds	r1, r3, #0
	lsls	r2, r2, #2
	adds	r2, #8
	adds	r1, #8
	ldr	r2, [r7, r2]
	ldr	r3, [r7, r1]
	cmp	r2, r3
	ble.n	.L_081aca66
	mov	sl, r1
	adds	r1, r0, #0
	adds	r4, r1, #4
	mov	ip, lr
.L_081aca14:
	mov	r0, ip
	ldr	r3, [r1, r0]
	subs	r6, #1
	str	r3, [r4, r0]
	subs	r1, #4
	subs	r4, #4
	cmp	r6, #0
	blt.n	.L_081aca64
	mov	r3, ip
	adds	r0, r1, #0
	ldr	r2, [r0, r3]
	ldr	r7, [sp, #112]
	lsls	r3, r2, #3
	subs	r3, r3, r2
	lsls	r3, r3, #2
	adds	r3, #8
	ldr	r2, [r7, r3]
	mov	r3, sl
	ldr	r3, [r7, r3]
	mov	r9, r3
	cmp	r2, r9
	bgt.n	.L_081aca14
	b.n	.L_081aca66
	movs	r0, r0
	.4byte 0x02000240
	.4byte 0x00000d73
	.4byte 0x03001150
	.4byte 0xfffc0000
	.4byte 0x001fffff
	.4byte 0xff4c0000
	.4byte 0xff600000
	.2byte 0x0000
	.2byte 0xffd8
.L_081aca64:
	.2byte 0x00b0
.L_081aca66:
	adds	r3, r0, #4
	mov	r4, r8
	str	r5, [r3, r4]
	movs	r5, #1
	add	fp, r5
	mov	r6, fp
	cmp	r6, #2
	bne.n	.L_081ac9da
	ldr	r1, [sp, #68]
	movs	r7, #0
	lsls	r3, r1, #3
	mov	fp, r7
	movs	r0, #240
	movs	r2, #200
	ldr	r7, [sp, #52]
	adds	r2, r2, r3
	add	r0, sp
	adds	r3, #196
	add	r6, sp, #200
	mov	r9, r0
	mov	sl, r2
	mov	r8, r3
.L_081aca92:
	ldr	r2, [r7, #0]
	ldr	r5, [sp, #112]
	lsls	r3, r2, #3
	subs	r3, r3, r2
	lsls	r3, r3, #2
	adds	r4, r5, r3
	adds	r0, r4, #0
	adds	r1, r6, #0
	str	r4, [sp, #8]
	bl	sub_08015778
	ldr	r2, [r6, #8]
	movs	r0, #94
	adds	r0, #255
	ldr	r4, [sp, #8]
	cmp	r2, r0
	bgt.n	.L_081acabc
	movs	r3, #175
	lsls	r3, r3, #1
	str	r3, [r6, #8]
	adds	r2, r3, #0
.L_081acabc:
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #138
	cmp	r2, r3
	ble.n	.L_081acaca
	str	r3, [r6, #8]
	adds	r2, r3, #0
.L_081acaca:
	ldr	r3, [r7, #0]
	ldr	r1, [pc, #908]
	lsls	r3, r3, #2
	adds	r2, r2, r1
	mov	r5, r9
	str	r2, [r5, r3]
	ldr	r5, [r7, #0]
	mov	r0, r9
	lsls	r3, r5, #2
	ldr	r1, [r0, r3]
	movs	r0, #128
	lsrs	r3, r1, #31
	adds	r1, r1, r3
	asrs	r1, r1, #1
	adds	r1, #128
	lsls	r0, r0, #4
	str	r4, [sp, #8]
	bl	sub_08002054
	ldr	r3, [r6, #0]
	movs	r1, #16
	subs	r2, r3, r0
	ldr	r3, [r6, #4]
	negs	r1, r1
	subs	r0, r3, r0
	adds	r3, r0, #0
	adds	r3, #96
	ldr	r4, [sp, #8]
	cmp	r3, r1
	ble.n	.L_081acb5c
	lsls	r1, r5, #25
	movs	r5, #158
	lsls	r5, r5, #2
	adds	r3, r2, r5
	movs	r2, #128
	lsls	r2, r2, #1
	adds	r2, #255
	ands	r3, r2
	lsls	r3, r3, #16
	subs	r2, #159
	orrs	r1, r3
	adds	r3, r0, r2
	movs	r2, #255
	ands	r3, r2
	orrs	r1, r3
	ldr	r3, [pc, #824]
	mov	r5, r8
	orrs	r1, r3
	ldr	r3, [sp, #108]
	ldr	r2, [pc, #820]
	str	r1, [r3, r5]
	ldr	r0, [r4, #24]
	cmp	r0, #0
	bge.n	.L_081acb3e
	movs	r1, #192
	lsls	r1, r1, #2
	adds	r1, #255
	adds	r0, r0, r1
.L_081acb3e:
	asrs	r3, r0, #10
	ldrb	r3, [r2, r3]
	movs	r2, #128
	lsls	r2, r2, #3
	lsls	r3, r3, #3
	orrs	r3, r2
	ldr	r2, [sp, #108]
	mov	r4, sl
	str	r3, [r2, r4]
	ldr	r0, [sp, #68]
	movs	r5, #8
	adds	r0, #1
	str	r0, [sp, #68]
	add	sl, r5
	add	r8, r5
.L_081acb5c:
	movs	r1, #1
	add	fp, r1
	mov	r2, fp
	adds	r7, #4
	cmp	r2, #2
	bne.n	.L_081aca92
	ldr	r3, [sp, #96]
	cmp	r3, #0
	bne.n	.L_081acc2c
	ldr	r1, [sp, #68]
	ldr	r7, [sp, #52]
	mov	r6, sp
	adds	r6, #240
	ldr	r0, [sp, #112]
	lsls	r3, r1, #3
	movs	r5, #200
	movs	r2, #200
	str	r6, [sp, #64]
	movs	r4, #0
	add	r5, sp
	str	r7, [sp, #28]
	adds	r2, r2, r3
	adds	r7, r3, #0
	mov	fp, r4
	mov	r9, r5
	mov	sl, r0
	mov	r8, r2
	adds	r7, #196
.L_081acb94:
	mov	r1, r9
	mov	r0, sl
	bl	sub_08015778
	ldr	r5, [sp, #28]
	ldr	r0, [sp, #64]
	ldmia	r5!, {r3}
	adds	r4, r5, #0
	str	r4, [sp, #28]
	lsls	r3, r3, #2
	ldr	r6, [r0, r3]
	movs	r0, #128
	lsrs	r3, r6, #31
	adds	r6, r6, r3
	asrs	r6, r6, #1
	adds	r6, #128
	adds	r1, r6, #0
	lsls	r0, r0, #4
	bl	sub_08002054
	mov	r1, r9
	ldr	r5, [r1, #0]
	adds	r1, r6, #0
	subs	r5, r5, r0
	movs	r0, #192
	lsls	r0, r0, #2
	bl	sub_08002054
	mov	r3, r9
	ldr	r2, [r3, #4]
	movs	r4, #158
	movs	r3, #128
	lsls	r4, r4, #2
	lsls	r3, r3, #1
	movs	r6, #176
	adds	r2, r2, r0
	adds	r3, #255
	adds	r5, r5, r4
	lsls	r6, r6, #1
	ands	r5, r3
	adds	r2, r2, r6
	movs	r3, #255
	ands	r2, r3
	ldr	r3, [pc, #636]
	lsls	r5, r5, #16
	ldr	r0, [sp, #108]
	orrs	r5, r2
	orrs	r5, r3
	mov	r1, fp
	str	r5, [r0, r7]
	cmp	r1, #1
	bne.n	.L_081acc06
	ldr	r3, [r0, r7]
	movs	r2, #128
	lsls	r2, r2, #21
	orrs	r3, r2
	str	r3, [r0, r7]
.L_081acc06:
	ldr	r2, [sp, #108]
	movs	r3, #144
	lsls	r3, r3, #4
	mov	r4, r8
	adds	r3, #88
	str	r3, [r2, r4]
	ldr	r6, [sp, #68]
	movs	r1, #1
	add	fp, r1
	movs	r5, #8
	adds	r6, #1
	movs	r0, #28
	mov	r2, fp
	add	r8, r5
	adds	r7, #8
	str	r6, [sp, #68]
	add	sl, r0
	cmp	r2, #2
	bne.n	.L_081acb94
.L_081acc2c:
	ldr	r0, [sp, #68]
	movs	r6, #128
	movs	r3, #0
	lsls	r6, r6, #19
	mov	fp, r3
	mov	r9, r6
	lsls	r3, r0, #3
	movs	r1, #200
	ldr	r6, [sp, #112]
	adds	r1, r1, r3
	adds	r3, #196
	add	r4, sp, #212
	add	r5, sp, #200
	add	r7, sp, #248
	mov	sl, r1
	mov	r8, r3
.L_081acc4c:
	ldr	r3, [r6, #0]
	movs	r2, #0
	str	r3, [r4, #0]
	str	r2, [r4, #4]
	adds	r0, r4, #0
	ldr	r3, [r6, #8]
	adds	r1, r5, #0
	str	r3, [r4, #8]
	str	r4, [sp, #8]
	bl	sub_08015778
	ldr	r3, [r5, #8]
	movs	r0, #94
	adds	r0, #255
	ldr	r4, [sp, #8]
	cmp	r3, r0
	bgt.n	.L_081acc74
	movs	r3, #175
	lsls	r3, r3, #1
	str	r3, [r5, #8]
.L_081acc74:
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #138
	cmp	r3, r2
	ble.n	.L_081acc82
	str	r2, [r5, #8]
	adds	r3, r2, #0
.L_081acc82:
	ldr	r1, [pc, #472]
	adds	r3, r3, r1
	str	r3, [r7, #0]
	movs	r0, #6
	ldrsh	r2, [r6, r0]
	movs	r0, #16
	adds	r3, r3, r2
	str	r3, [r7, #0]
	negs	r0, r0
	ldr	r1, [r5, #4]
	ldr	r2, [r5, #0]
	adds	r3, r1, #0
	adds	r3, #88
	cmp	r3, r0
	ble.n	.L_081accde
	movs	r0, #156
	lsls	r0, r0, #2
	adds	r3, r2, r0
	movs	r2, #128
	lsls	r2, r2, #1
	adds	r2, #255
	ands	r3, r2
	movs	r0, #172
	mov	r2, r9
	lsls	r0, r0, #1
	lsls	r3, r3, #16
	orrs	r3, r2
	adds	r2, r1, r0
	movs	r1, #255
	ands	r2, r1
	orrs	r3, r2
	ldr	r2, [pc, #412]
	ldr	r1, [sp, #108]
	orrs	r3, r2
	mov	r2, r8
	str	r3, [r1, r2]
	movs	r3, #137
	mov	r0, sl
	lsls	r3, r3, #4
	str	r3, [r1, r0]
	ldr	r2, [sp, #68]
	movs	r1, #8
	adds	r2, #1
	str	r2, [sp, #68]
	add	sl, r1
	add	r8, r1
.L_081accde:
	movs	r0, #1
	movs	r3, #128
	add	fp, r0
	lsls	r3, r3, #18
	mov	r1, fp
	add	r9, r3
	adds	r7, #4
	adds	r6, #28
	cmp	r1, #2
	bne.n	.L_081acc4c
	ldr	r2, [sp, #96]
	cmp	r2, #1
	beq.n	.L_081accfa
	b.n	.L_081ad1ce
.L_081accfa:
	ldr	r4, [sp, #44]
	movs	r3, #0
	str	r3, [sp, #60]
	str	r4, [sp, #36]
	str	r3, [sp, #32]
	ldr	r7, [sp, #112]
	mov	fp, r3
.L_081acd08:
	ldr	r1, [r7, #0]
	ldr	r3, [r7, #12]
	ldr	r2, [r7, #16]
	adds	r1, r1, r3
	ldr	r3, [r7, #4]
	str	r1, [r7, #0]
	adds	r3, r3, r2
	str	r3, [r7, #4]
	ldr	r2, [r7, #20]
	ldr	r3, [r7, #8]
	movs	r6, #0
	adds	r3, r3, r2
	str	r3, [r7, #8]
	ldr	r5, [sp, #112]
	mov	r9, r5
.L_081acd26:
	cmp	fp, r6
	beq.n	.L_081acda0
	mov	r0, r9
	ldr	r3, [r0, #0]
	ldr	r2, [r7, #4]
	subs	r3, r1, r3
	asrs	r4, r3, #16
	ldr	r3, [r0, #4]
	subs	r2, r2, r3
	asrs	r2, r2, #16
	mov	r8, r2
	ldr	r3, [r0, #8]
	ldr	r2, [r7, #8]
	mov	r5, r8
	subs	r2, r2, r3
	asrs	r2, r2, #16
	mov	sl, r2
	mov	r3, r8
	muls	r3, r5
	adds	r2, r4, #0
	muls	r2, r4
	mov	r0, sl
	adds	r2, r2, r3
	mov	r3, sl
	muls	r3, r0
	adds	r0, r2, r3
	movs	r2, #144
	lsls	r2, r2, #4
	adds	r2, #195
	cmp	r0, r2
	bgt.n	.L_081acda0
	str	r4, [sp, #8]
	ldr	r3, [pc, #260]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9c02
	adds	r5, r0, #0
	adds	r1, r5, #0
	lsls	r0, r4, #15
	bl	sub_08002054
	ldr	r3, [r7, #12]
	adds	r1, r5, #0
	adds	r3, r3, r0
	str	r3, [r7, #12]
	mov	r3, r8
	lsls	r0, r3, #15
	bl	sub_08002054
	ldr	r3, [r7, #16]
	mov	r4, sl
	adds	r3, r3, r0
	str	r3, [r7, #16]
	adds	r1, r5, #0
	lsls	r0, r4, #15
	bl	sub_08002054
	ldr	r3, [r7, #20]
	ldr	r1, [r7, #0]
	adds	r3, r3, r0
	str	r3, [r7, #20]
.L_081acda0:
	movs	r5, #28
	adds	r6, #1
	add	r9, r5
	cmp	r6, #2
	bne.n	.L_081acd26
	ldr	r0, [pc, #196]
	cmp	r1, r0
	bge.n	.L_081acdc8
	ldr	r2, [r7, #12]
	str	r0, [r7, #0]
	negs	r2, r2
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #4
	cmp	r3, #0
	bge.n	.L_081acdc2
	adds	r3, #63
.L_081acdc2:
	asrs	r3, r3, #6
	str	r3, [r7, #12]
	adds	r1, r0, #0
.L_081acdc8:
	movs	r3, #200
	lsls	r3, r3, #16
	cmp	r1, r3
	ble.n	.L_081acde6
	ldr	r2, [r7, #12]
	str	r3, [r7, #0]
	negs	r2, r2
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #4
	cmp	r3, #0
	bge.n	.L_081acde2
	adds	r3, #63
.L_081acde2:
	asrs	r3, r3, #6
	str	r3, [r7, #12]
.L_081acde6:
	ldr	r2, [r7, #8]
	ldr	r1, [pc, #136]
	cmp	r2, r1
	bge.n	.L_081ace06
	ldr	r2, [r7, #20]
	str	r1, [r7, #8]
	negs	r2, r2
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #4
	cmp	r3, #0
	bge.n	.L_081ace00
	adds	r3, #63
.L_081ace00:
	asrs	r3, r3, #6
	str	r3, [r7, #20]
	adds	r2, r1, #0
.L_081ace06:
	ldr	r3, [r7, #4]
	movs	r6, #180
	lsls	r6, r6, #16
	adds	r3, r3, r6
	cmp	r2, r3
	ble.n	.L_081ace38
	ldr	r1, [r7, #20]
	str	r3, [r7, #8]
	cmp	r1, #0
	ble.n	.L_081ace38
	ldr	r2, [r7, #16]
	lsrs	r3, r1, #31
	adds	r3, r1, r3
	asrs	r3, r3, #1
	adds	r2, r2, r3
	str	r2, [r7, #16]
	negs	r2, r1
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r2, r3, #4
	cmp	r2, #0
	bge.n	.L_081ace34
	adds	r2, #63
.L_081ace34:
	asrs	r3, r2, #6
	str	r3, [r7, #20]
.L_081ace38:
	ldr	r3, [r7, #16]
	ldr	r0, [pc, #60]
	adds	r2, r3, r0
	ldr	r3, [r7, #4]
	str	r2, [r7, #16]
	cmp	r3, #0
	ble.n	.L_081ace48
	b.n	.L_081acf5c
.L_081ace48:
	negs	r2, r2
	lsls	r3, r2, #1
	adds	r3, r3, r2
	movs	r1, #0
	lsls	r2, r3, #4
	str	r1, [r7, #4]
	cmp	r2, #0
	bge.n	.L_081ace7c
	adds	r2, #63
	b.n	.L_081ace7c
	.4byte 0xfffffea2
	.4byte 0x40002300
	.4byte 0x081ad374
	.4byte 0x80002000
	.4byte 0x030002d4
	.4byte 0xff380000
	.4byte 0xff6a0000
	.2byte 0x8000
	.2byte 0xffff
.L_081ace7c:
	.2byte 0x1193
	str	r3, [r7, #16]
	bl	sub_08014878
	movs	r3, #3
	ands	r0, r3
	cmp	r0, #0
	bne.n	.L_081acede
	ldr	r2, [r7, #12]
	ldr	r3, [r7, #20]
	asrs	r2, r2, #8
	asrs	r3, r3, #8
	adds	r0, r2, #0
	muls	r0, r2
	adds	r2, r3, #0
	muls	r2, r3
	adds	r3, r2, #0
	adds	r0, r0, r3
	ldr	r3, [pc, #536]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x00c3
	subs	r3, r3, r0
	lsls	r3, r3, #3
	cmp	r3, #0
	bge.n	.L_081aceb2
	adds	r3, #63
.L_081aceb2:
	asrs	r6, r3, #6
	bl	sub_08014878
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	adds	r5, r0, #0
	ands	r5, r3
	adds	r0, r5, #0
	bl	sub_08002096
	adds	r3, r6, #0
	muls	r3, r0
	asrs	r3, r3, #8
	str	r3, [r7, #12]
	adds	r0, r5, #0
	bl	sub_08002090
	adds	r3, r6, #0
	muls	r3, r0
	asrs	r3, r3, #8
	b.n	.L_081acf00
.L_081acede:
	ldr	r2, [r7, #12]
	lsls	r3, r2, #3
	subs	r3, r3, r2
	lsls	r3, r3, #3
	cmp	r3, #0
	bge.n	.L_081aceec
	adds	r3, #63
.L_081aceec:
	ldr	r2, [r7, #20]
	asrs	r3, r3, #6
	str	r3, [r7, #12]
	lsls	r3, r2, #3
	subs	r3, r3, r2
	lsls	r3, r3, #3
	cmp	r3, #0
	bge.n	.L_081acefe
	adds	r3, #63
.L_081acefe:
	asrs	r3, r3, #6
.L_081acf00:
	str	r3, [r7, #20]
	ldr	r5, [sp, #36]
	ldr	r4, [sp, #32]
	ldr	r3, [r5, #0]
	cmp	r3, #19
	ble.n	.L_081acf10
	subs	r3, #20
	str	r3, [r5, #0]
.L_081acf10:
	ldr	r3, [r7, #16]
	ldr	r6, [pc, #428]
	cmp	r3, r6
	bgt.n	.L_081acf60
	movs	r0, #0
	str	r0, [r7, #12]
	str	r0, [r7, #16]
	str	r0, [r7, #20]
	ldr	r1, [sp, #36]
	str	r0, [r1, #0]
	ldr	r5, [r7, #24]
	adds	r0, r5, #0
	cmp	r5, #0
	bge.n	.L_081acf34
	movs	r2, #192
	lsls	r2, r2, #2
	adds	r2, #255
	adds	r0, r5, r2
.L_081acf34:
	asrs	r0, r0, #10
	movs	r1, #3
	str	r4, [sp, #8]
	bl	sub_08002064
	ldr	r4, [sp, #8]
	cmp	r0, #1
	bne.n	.L_081acf4e
	movs	r6, #128
	lsls	r6, r6, #4
	adds	r3, r5, r6
	str	r3, [r7, #24]
	b.n	.L_081acf60
.L_081acf4e:
	cmp	r0, #2
	bne.n	.L_081acf60
	movs	r0, #128
	lsls	r0, r0, #3
	adds	r3, r5, r0
	str	r3, [r7, #24]
	b.n	.L_081acf60
.L_081acf5c:
	mov	r1, fp
	lsls	r4, r1, #2
.L_081acf60:
	ldr	r3, [r7, #12]
	lsls	r2, r3, #6
	subs	r2, r2, r3
	cmp	r2, #0
	bge.n	.L_081acf6c
	adds	r2, #63
.L_081acf6c:
	asrs	r3, r2, #6
	str	r3, [r7, #12]
	ldr	r3, [r7, #16]
	lsls	r2, r3, #6
	subs	r2, r2, r3
	cmp	r2, #0
	bge.n	.L_081acf7c
	adds	r2, #63
.L_081acf7c:
	asrs	r3, r2, #6
	str	r3, [r7, #16]
	ldr	r3, [r7, #20]
	lsls	r2, r3, #6
	subs	r2, r2, r3
	cmp	r2, #0
	bge.n	.L_081acf8c
	adds	r2, #63
.L_081acf8c:
	asrs	r3, r2, #6
	str	r3, [r7, #20]
	ldr	r5, [sp, #32]
	ldr	r6, [sp, #44]
	ldr	r2, [r7, #24]
	ldr	r3, [r5, r6]
	movs	r0, #142
	lsls	r0, r0, #7
	adds	r2, r2, r3
	adds	r0, #255
	str	r2, [r7, #24]
	cmp	r2, r0
	ble.n	.L_081acfac
	ldr	r1, [pc, #284]
	adds	r3, r2, r1
	str	r3, [r7, #24]
.L_081acfac:
	ldr	r2, [sp, #36]
	ldr	r3, [r2, #0]
	cmp	r3, #0
	ble.n	.L_081acfb8
	subs	r3, #1
	str	r3, [r2, #0]
.L_081acfb8:
	ldr	r3, [r7, #12]
	cmp	r3, #0
	bne.n	.L_081ad016
	ldr	r3, [r7, #16]
	cmp	r3, #0
	bne.n	.L_081ad016
	ldr	r3, [r7, #20]
	cmp	r3, #0
	bne.n	.L_081ad016
	ldr	r3, [r7, #4]
	cmp	r3, #0
	bne.n	.L_081ad016
	movs	r1, #192
	ldr	r0, [r7, #24]
	lsls	r1, r1, #4
	add	r5, sp, #224
	str	r4, [sp, #8]
	bl	sub_08002054
	ldr	r1, [pc, #232]
	ldr	r4, [sp, #8]
	movs	r6, #0
	str	r0, [r5, r4]
	ldrsh	r3, [r1, r6]
	ldr	r2, [r7, #8]
	lsls	r3, r3, #16
	cmp	r2, r3
	ble.n	.L_081acff8
	adds	r3, r4, #0
	adds	r3, #8
	str	r6, [r5, r3]
	b.n	.L_081ad010
.L_081acff8:
	adds	r6, #1
	cmp	r6, #6
	beq.n	.L_081ad010
	lsls	r3, r6, #1
	ldrsh	r3, [r1, r3]
	lsls	r3, r3, #16
	cmp	r2, r3
	ble.n	.L_081acff8
	ldr	r3, [sp, #32]
	add	r2, sp, #224
	adds	r3, #8
	str	r6, [r2, r3]
.L_081ad010:
	ldr	r5, [sp, #60]
	adds	r5, #1
	str	r5, [sp, #60]
.L_081ad016:
	ldr	r6, [sp, #36]
	ldr	r0, [sp, #32]
	movs	r1, #1
	add	fp, r1
	adds	r6, #4
	adds	r0, #4
	mov	r2, fp
	str	r6, [sp, #36]
	str	r0, [sp, #32]
	adds	r7, #28
	cmp	r2, #2
	beq.n	.L_081ad030
	b.n	.L_081acd08
.L_081ad030:
	ldr	r3, [sp, #60]
	cmp	r3, #2
	beq.n	.L_081ad038
	b.n	.L_081ad1ce
.L_081ad038:
	movs	r6, #1
	movs	r5, #0
	movs	r4, #2
	negs	r6, r6
	movs	r7, #60
	str	r4, [sp, #96]
	str	r5, [sp, #88]
	str	r6, [sp, #84]
	str	r5, [sp, #80]
	str	r7, [sp, #76]
	add	r1, sp, #224
	ldr	r2, [r1, #0]
	ldr	r3, [r1, #4]
	cmp	r2, r3
	bne.n	.L_081ad0d0
	ldr	r3, [r1, #8]
	cmp	r2, r3
	bne.n	.L_081ad0d0
	ldr	r3, [r1, #12]
	cmp	r2, r3
	bne.n	.L_081ad0d0
	ldr	r0, [sp, #56]
	movs	r3, #120
	str	r3, [r0, #4]
	str	r3, [sp, #144]
	str	r5, [sp, #84]
	ldr	r3, [sp, #136]
	ldr	r2, [r1, #0]
	cmp	r2, r3
	bne.n	.L_081ad07c
	ldr	r1, [sp, #92]
	lsls	r3, r1, #4
	subs	r3, r3, r1
	b.n	.L_081ad096
.L_081ad07c:
	ldr	r4, [sp, #48]
	ldr	r3, [r4, #4]
	cmp	r2, r3
	bne.n	.L_081ad090
	ldr	r5, [sp, #92]
	lsls	r3, r5, #1
	adds	r3, r3, r5
	lsls	r3, r3, #3
	adds	r3, r3, r5
	b.n	.L_081ad096
.L_081ad090:
	ldr	r6, [sp, #92]
	lsls	r3, r6, #2
	adds	r3, r3, r6
.L_081ad096:
	str	r3, [sp, #80]
	ldr	r3, [pc, #48]
	ldr	r7, [sp, #108]
	movs	r1, #128
	lsls	r1, r1, #3
	adds	r1, #196
	ldr	r0, [r3, #16]
	adds	r3, r7, r1
	ldr	r2, [r3, #0]
	movs	r1, #6
	movs	r3, #0
	str	r3, [sp, #0]
	bl	sub_080380b0
	movs	r0, #93
	bl	sub_081c0010
	b.n	.L_081ad1ce
	movs	r0, r0
	.4byte 0x030002d4
	.4byte 0x0002ffff
	.4byte 0xffffb800
	.4byte 0x081ad386
	.2byte 0x0240
	.2byte 0x0200
.L_081ad0d0:
	ldr	r5, [pc, #400]
	movs	r4, #0
	add	r6, sp, #768
	mov	r8, r4
	mov	fp, r4
	mov	lr, r5
	adds	r7, r6, #0
.L_081ad0de:
	mov	r5, fp
	adds	r5, #1
	adds	r6, r5, #0
	cmp	r5, #4
	beq.n	.L_081ad118
	mov	r1, fp
	add	r0, sp, #768
	lsls	r2, r1, #2
	ldr	r1, [pc, #376]
	add	r0, lr
	add	r4, sp, #768
	lsls	r3, r5, #2
	mov	ip, r0
	adds	r3, r3, r4
	mov	r0, lr
	adds	r4, r3, r0
	adds	r0, r7, r1
.L_081ad100:
	mov	r3, ip
	ldr	r1, [r3, r2]
	ldmia	r4!, {r3}
	cmp	r1, r3
	bne.n	.L_081ad112
	stmia	r0!, {r1}
	movs	r1, #1
	adds	r7, #4
	add	r8, r1
.L_081ad112:
	adds	r6, #1
	cmp	r6, #4
	bne.n	.L_081ad100
.L_081ad118:
	mov	fp, r5
	cmp	r5, #3
	bne.n	.L_081ad0de
	mov	r2, r8
	cmp	r2, #1
	bne.n	.L_081ad148
	ldr	r3, [sp, #92]
	ldr	r4, [sp, #108]
	str	r3, [sp, #80]
	movs	r5, #128
	ldr	r3, [pc, #316]
	lsls	r5, r5, #3
	adds	r5, #196
	ldr	r0, [r3, #16]
	adds	r3, r4, r5
	ldr	r2, [r3, #0]
	movs	r6, #0
	movs	r1, #6
	movs	r3, #0
	str	r6, [sp, #0]
	bl	sub_080380b0
	movs	r7, #3
	str	r7, [sp, #84]
.L_081ad148:
	mov	r0, r8
	cmp	r0, #2
	bne.n	.L_081ad18a
	ldr	r1, [sp, #92]
	ldr	r2, [sp, #108]
	lsls	r1, r1, #1
	str	r1, [sp, #80]
	ldr	r3, [pc, #276]
	movs	r4, #128
	lsls	r4, r4, #3
	adds	r4, #196
	ldr	r0, [r3, #16]
	adds	r3, r2, r4
	ldr	r2, [r3, #0]
	movs	r1, #6
	movs	r3, #0
	movs	r5, #0
	str	r5, [sp, #0]
	bl	sub_080380b0
	bl	sub_08014878
	movs	r3, #1
	ldr	r6, [sp, #56]
	ands	r3, r0
	lsls	r3, r3, #2
	movs	r2, #60
	str	r2, [r3, r6]
	movs	r7, #2
	movs	r0, #91
	str	r7, [sp, #84]
	bl	sub_081c0010
.L_081ad18a:
	mov	r0, r8
	cmp	r0, #3
	bne.n	.L_081ad1ce
	ldr	r1, [sp, #92]
	ldr	r2, [sp, #108]
	lsls	r3, r1, #1
	adds	r3, r3, r1
	str	r3, [sp, #80]
	movs	r4, #128
	ldr	r3, [pc, #204]
	lsls	r4, r4, #3
	adds	r4, #196
	ldr	r0, [r3, #16]
	adds	r3, r2, r4
	ldr	r2, [r3, #0]
	movs	r1, #6
	movs	r3, #0
	movs	r5, #0
	str	r5, [sp, #0]
	bl	sub_080380b0
	bl	sub_08014878
	movs	r3, #1
	ldr	r6, [sp, #56]
	ands	r3, r0
	lsls	r3, r3, #2
	movs	r2, #60
	str	r2, [r3, r6]
	movs	r7, #1
	movs	r0, #92
	str	r7, [sp, #84]
	bl	sub_081c0010
.L_081ad1ce:
	ldr	r0, [sp, #68]
	cmp	r0, #128
	beq.n	.L_081ad1f6
	ldr	r2, [sp, #68]
	ldr	r0, [pc, #152]
	lsls	r3, r2, #3
	adds	r2, r3, #0
	movs	r1, #0
	adds	r2, #200
	adds	r3, #196
.L_081ad1e2:
	ldr	r4, [sp, #108]
	str	r0, [r4, r3]
	str	r1, [r4, r2]
	ldr	r5, [sp, #68]
	adds	r2, #8
	adds	r5, #1
	adds	r3, #8
	str	r5, [sp, #68]
	cmp	r5, #128
	bne.n	.L_081ad1e2
.L_081ad1f6:
	movs	r1, #224
	mov	r8, r1
	mov	r1, sp
	adds	r1, #240
	str	r1, [sp, #4]
	movs	r6, #0
	mov	r7, sp
	mov	r4, sp
	movs	r2, #216
	movs	r3, #208
	mov	fp, r6
	adds	r7, #166
	add	r5, sp, #164
	adds	r4, #162
	add	r0, sp, #160
	mov	lr, r2
	mov	ip, r3
	movs	r6, #200
.L_081ad21a:
	ldr	r1, [sp, #4]
	ldmia	r1!, {r3}
	adds	r2, r1, #0
	str	r2, [sp, #4]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	ldr	r2, [pc, #52]
	ldr	r1, [pc, #52]
	adds	r3, #128
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	strh	r3, [r7, #0]
	strh	r2, [r5, #0]
	strh	r1, [r4, #0]
	strh	r3, [r0, #0]
	ldrh	r2, [r7, #0]
	lsls	r2, r2, #16
	mov	sl, r2
	ldr	r2, [sp, #108]
	mov	r1, sl
	ldr	r3, [r2, r6]
	orrs	r3, r1
	str	r3, [r2, r6]
	ldrh	r2, [r5, #0]
	mov	r1, ip
	lsls	r2, r2, #16
	mov	sl, r2
	ldr	r2, [sp, #108]
	adds	r6, #32
	ldr	r3, [r2, r1]
	mov	r2, sl
	orrs	r3, r2
	ldr	r2, [sp, #108]
	b.n	.L_081ad274
	.4byte 0x00000000
	.4byte 0xfffffde0
	.4byte 0xfffffd78
	.4byte 0x02000240
	.2byte 0x2000
	.2byte 0x40f0
.L_081ad274:
	str	r3, [r2, r1]
	ldrh	r2, [r4, #0]
	ldr	r1, [sp, #108]
	lsls	r2, r2, #16
	mov	sl, r2
	mov	r2, lr
	ldr	r3, [r1, r2]
	mov	r1, sl
	orrs	r3, r1
	ldr	r1, [sp, #108]
	str	r3, [r1, r2]
	ldrh	r2, [r0, #0]
	lsls	r2, r2, #16
	mov	sl, r2
	mov	r2, r8
	ldr	r3, [r1, r2]
	mov	r1, sl
	orrs	r3, r1
	ldr	r1, [sp, #108]
	str	r3, [r1, r2]
	movs	r3, #1
	add	fp, r3
	movs	r2, #32
	mov	r1, fp
	add	r8, r2
	add	lr, r2
	add	ip, r2
	cmp	r1, #4
	bne.n	.L_081ad21a
	ldr	r0, [sp, #108]
	movs	r3, #128
	lsls	r3, r3, #19
	movs	r1, #224
	adds	r3, #212
	adds	r0, #196
	lsls	r1, r1, #19
	ldr	r2, [pc, #128]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r0, #1
	bl	sub_08013560
	ldr	r2, [sp, #100]
	adds	r2, #1
	str	r2, [sp, #100]
	bl	.L_081ac4a4
.L_081ad2d2:
	movs	r3, #0
	movs	r6, #128
	mov	fp, r3
	lsls	r6, r6, #1
.L_081ad2da:
	mov	r4, fp
	movs	r5, #128
	lsls	r3, r4, #12
	lsls	r5, r5, #9
	subs	r5, r5, r3
	ldr	r0, [sp, #116]
	adds	r2, r5, #0
	ldr	r1, [pc, #88]
	adds	r3, r6, #0
	bl	.L_081ac0f8
	adds	r2, r5, #0
	movs	r1, #160
	movs	r5, #1
	add	r0, sp, #256
	lsls	r1, r1, #19
	adds	r3, r6, #0
	add	fp, r5
	bl	.L_081ac0f8
	mov	r7, fp
	movs	r0, #1
	bl	sub_08013560
	cmp	r7, #17
	bne.n	.L_081ad2da
	movs	r0, #48
	bl	sub_0801314c
	movs	r0, #180
	bl	sub_0801314c
	movs	r0, #96
	bl	sub_0801314c
	movs	r0, #92
	bl	sub_0801314c
	movs	r0, #100
	bl	sub_0801314c
	movs	r3, #192
	lsls	r3, r3, #2
	add	sp, r3
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x84000100
	.4byte 0x05000200
