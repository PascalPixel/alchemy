.syntax unified
	.thumb
	.set sub_0801314c, 0x0801314c
	.set sub_08013560, 0x08013560
	.set sub_08014cc0, 0x08014cc0
	.set sub_08016ca4, 0x08016ca4
	.set sub_08118010, 0x08118010
	.set sub_08118040, 0x08118040
	.set sub_0813fd84, 0x0813fd84
	.set sub_0814153c, 0x0814153c
	.set sub_08146e00, 0x08146e00
	.set sub_08147ae0, 0x08147ae0
	.set sub_08148458, 0x08148458
	.set sub_0814bee0, 0x0814bee0
	.set sub_0814ce30, 0x0814ce30
	.set sub_0814db04, 0x0814db04
	.set sub_08153ebc, 0x08153ebc
	.set sub_08158dc8, 0x08158dc8
	.set sub_0815b764, 0x0815b764
	.set sub_0815cde8, 0x0815cde8
	.set sub_08164bf4, 0x08164bf4
	.set sub_08165ab4, 0x08165ab4
	.set sub_0816729c, 0x0816729c
	.set sub_08168a40, 0x08168a40
	.set sub_081693d0, 0x081693d0
	.set sub_08170364, 0x08170364
	.set sub_08175f74, 0x08175f74
	.set sub_08179f18, 0x08179f18
	.set sub_0817ea58, 0x0817ea58
	.set sub_08180c94, 0x08180c94
	.set sub_08182aa8, 0x08182aa8
	.set sub_081843fc, 0x081843fc
	.set sub_08186dfc, 0x08186dfc
	.set sub_0818cb74, 0x0818cb74
	.set sub_0818df5c, 0x0818df5c
	.set sub_08191d60, 0x08191d60
	.set sub_08192c9c, 0x08192c9c
	.set sub_081c0010, 0x081c0010
	.global Func_0814c994
	.thumb_func
Func_0814c994:
	push	{r5, lr}
	movs	r1, #192
	lsls	r1, r1, #2
	adds	r5, r0, #0
	adds	r1, #2
	movs	r0, #100
	bl	sub_08014cc0
	movs	r1, #246
	lsls	r1, r1, #7
	adds	r1, #124
	movs	r0, #92
	bl	sub_08014cc0
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r0, #96
	bl	sub_08014cc0
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #92]
	movs	r1, #240
	lsls	r1, r1, #7
	adds	r1, #240
	adds	r2, r3, r1
	str	r5, [r2, #0]
	movs	r2, #240
	lsls	r2, r2, #7
	adds	r2, #228
	adds	r3, r3, r2
	movs	r2, #1
	str	r2, [r3, #0]
	ldr	r3, [r5, #0]
	cmp	r3, #0
	beq.n	.L_0814cab0
	subs	r3, #1
	cmp	r3, #28
	bls.n	.L_0814c9e4
	b.n	.L_0814cb46
.L_0814c9e4:
	ldr	r2, [pc, #372]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x0814ca60
	.4byte 0x0814ca68
	.4byte 0x0814ca70
	.4byte 0x0814ca78
	.4byte 0x0814ca80
	.4byte 0x0814ca88
	.4byte 0x0814ca90
	.4byte 0x0814ca98
	.4byte 0x0814caa0
	.4byte 0x0814caa8
	.4byte 0x0814cab0
	.4byte 0x0814cab8
	.4byte 0x0814cac0
	.4byte 0x0814cac8
	.4byte 0x0814cad0
	.4byte 0x0814cad8
	.4byte 0x0814cae0
	.4byte 0x0814cae8
	.4byte 0x0814caf0
	.4byte 0x0814caf8
	.4byte 0x0814cb00
	.4byte 0x0814cb08
	.4byte 0x0814cb10
	.4byte 0x0814cb18
	.4byte 0x0814cb20
	.4byte 0x0814cb28
	.4byte 0x0814cb30
	.4byte 0x0814cb38
	.2byte 0xcb40
	.2byte 0x0814
	adds	r0, r5, #0
	bl	sub_08165ab4
	b.n	.L_0814cb46
	adds	r0, r5, #0
	bl	sub_08148458
	b.n	.L_0814cb46
	adds	r0, r5, #0
	bl	sub_08168a40
	b.n	.L_0814cb46
	adds	r0, r5, #0
	bl	sub_08153ebc
	b.n	.L_0814cb46
	adds	r0, r5, #0
	bl	sub_0814ce30
	b.n	.L_0814cb46
	adds	r0, r5, #0
	bl	sub_081693d0
	b.n	.L_0814cb46
	adds	r0, r5, #0
	bl	sub_08147ae0
	b.n	.L_0814cb46
	adds	r0, r5, #0
	bl	sub_08146e00
	b.n	.L_0814cb46
	adds	r0, r5, #0
	bl	sub_0816729c
	b.n	.L_0814cb46
	adds	r0, r5, #0
	bl	sub_0814db04
	b.n	.L_0814cb46
.L_0814cab0:
	adds	r0, r5, #0
	bl	sub_08164bf4
	b.n	.L_0814cb46
	adds	r0, r5, #0
	bl	sub_08158dc8
	b.n	.L_0814cb46
	adds	r0, r5, #0
	bl	sub_0814bee0
	b.n	.L_0814cb46
	adds	r0, r5, #0
	bl	sub_08170364
	b.n	.L_0814cb46
	adds	r0, r5, #0
	bl	sub_08179f18
	b.n	.L_0814cb46
	adds	r0, r5, #0
	bl	sub_08191d60
	b.n	.L_0814cb46
	adds	r0, r5, #0
	bl	sub_0815b764
	b.n	.L_0814cb46
	adds	r0, r5, #0
	bl	sub_08186dfc
	b.n	.L_0814cb46
	adds	r0, r5, #0
	bl	sub_0814153c
	b.n	.L_0814cb46
	adds	r0, r5, #0
	bl	sub_08182aa8
	b.n	.L_0814cb46
	adds	r0, r5, #0
	bl	sub_0817ea58
	b.n	.L_0814cb46
	adds	r0, r5, #0
	bl	sub_0815cde8
	b.n	.L_0814cb46
	adds	r0, r5, #0
	bl	sub_081843fc
	b.n	.L_0814cb46
	adds	r0, r5, #0
	bl	sub_08175f74
	b.n	.L_0814cb46
	adds	r0, r5, #0
	bl	sub_08192c9c
	b.n	.L_0814cb46
	adds	r0, r5, #0
	bl	sub_08180c94
	b.n	.L_0814cb46
	adds	r0, r5, #0
	bl	sub_0818cb74
	b.n	.L_0814cb46
	adds	r0, r5, #0
	bl	sub_0813fd84
	b.n	.L_0814cb46
	adds	r0, r5, #0
	bl	sub_0818df5c
.L_0814cb46:
	movs	r0, #96
	bl	sub_0801314c
	movs	r0, #92
	bl	sub_0801314c
	movs	r0, #100
	bl	sub_0801314c
	pop	{r5, pc}
	movs	r0, r0
	.2byte 0xc9ec
	.2byte 0x0814
	push	{r5, lr}
	movs	r1, #192
	lsls	r1, r1, #2
	adds	r5, r0, #0
	adds	r1, #2
	movs	r0, #100
	bl	sub_08014cc0
	movs	r1, #246
	lsls	r1, r1, #7
	adds	r1, #124
	movs	r0, #92
	bl	sub_08014cc0
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r0, #96
	bl	sub_08014cc0
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #92]
	movs	r1, #240
	lsls	r1, r1, #7
	adds	r1, #240
	adds	r2, r3, r1
	str	r5, [r2, #0]
	movs	r2, #240
	lsls	r2, r2, #7
	adds	r2, #228
	adds	r3, r3, r2
	movs	r2, #1
	str	r2, [r3, #0]
	ldr	r2, [pc, #36]
	ldr	r3, [r5, #0]
	adds	r0, r5, #0
	lsls	r3, r3, #2
	subs	r3, #4
	ldr	r3, [r2, r3]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2060
	bl	sub_0801314c
	movs	r0, #92
	bl	sub_0801314c
	movs	r0, #100
	bl	sub_0801314c
	pop	{r5, pc}
	movs	r0, r0
	.2byte 0x7aa0
	.2byte 0x0819
	push	{r5, r6, lr}
	ldr	r6, [pc, #44]
	ldr	r5, [pc, #36]
	ldr	r1, [pc, #44]
	ldr	r4, [pc, #44]
	movs	r0, #0
.L_0814cbd8:
	adds	r3, r0, #0
	subs	r3, #8
	cmp	r3, #127
	bhi.n	.L_0814cc08
	ldrh	r2, [r6, #0]
	ldrb	r3, [r4, #0]
	subs	r2, r2, r3
	cmp	r2, #0
	bge.n	.L_0814cbec
	movs	r2, #0
.L_0814cbec:
	cmp	r2, #240
	ble.n	.L_0814cbf2
	movs	r2, #240
.L_0814cbf2:
	strh	r2, [r1, #0]
	b.n	.L_0814cc0a
	movs	r0, r0
	.4byte 0x0000fff1
	.4byte 0x02010000
	.4byte 0x02010082
	.2byte 0xfffa
	.2byte 0x0200
.L_0814cc08:
	strh	r5, [r1, #0]
.L_0814cc0a:
	adds	r0, #1
	adds	r1, #2
	adds	r4, #1
	cmp	r0, #160
	bne.n	.L_0814cbd8
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #176
	ldrh	r1, [r3, #10]
	movs	r2, #197
	lsls	r2, r2, #8
	adds	r2, #255
	ands	r2, r1
	strh	r2, [r3, #10]
	movs	r2, #254
	ldrh	r1, [r3, #10]
	lsls	r2, r2, #7
	adds	r2, #255
	ands	r2, r1
	strh	r2, [r3, #10]
	movs	r1, #128
	lsls	r1, r1, #19
	ldrh	r2, [r3, #10]
	ldr	r0, [pc, #8]
	adds	r1, #64
	ldr	r2, [pc, #8]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	pop	{r5, r6, pc}
	.4byte 0x02010082
	.2byte 0x0001
	.2byte 0xa260
	push	{r5, r6, r7, lr}
	adds	r5, r0, #0
	ldr	r3, [r5, #20]
	sub	sp, #40
	movs	r4, #0
	movs	r2, #0
	cmp	r3, #0
	beq.n	.L_0814cc90
	add	r1, sp, #12
	movs	r6, #36
	adds	r7, r1, #0
.L_0814cc62:
	ldrsh	r0, [r6, r5]
	str	r1, [sp, #8]
	str	r2, [sp, #4]
	str	r4, [sp, #0]
	bl	sub_08016ca4
	movs	r3, #56
	ldrsh	r0, [r0, r3]
	ldr	r1, [sp, #8]
	ldr	r2, [sp, #4]
	ldr	r4, [sp, #0]
	cmp	r0, #0
	ble.n	.L_0814cc84
	ldrh	r3, [r6, r5]
	adds	r4, #1
	strh	r3, [r7, #0]
	adds	r7, #2
.L_0814cc84:
	ldr	r3, [r5, #20]
	adds	r2, #1
	adds	r6, #2
	cmp	r2, r3
	bne.n	.L_0814cc62
	b.n	.L_0814cc92
.L_0814cc90:
	add	r1, sp, #12
.L_0814cc92:
	ldr	r3, [pc, #16]
	lsls	r2, r4, #1
	strh	r3, [r1, r2]
	adds	r0, r1, #0
	movs	r1, #0
	bl	sub_08118010
	add	sp, #40
	pop	{r5, r6, r7, pc}
	.2byte 0x00ff
	.2byte 0x0000
	push	{r5, r6, lr}
	mov	r6, r8
	push	{r6}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #36]
	ldr	r6, [r3, #96]
	ldr	r3, [pc, #60]
	movs	r2, #128
	lsls	r2, r2, #19
	strh	r3, [r2, #0]
	mov	r8, r2
	ldr	r2, [pc, #52]
	movs	r3, #32
	strh	r3, [r2, #6]
	movs	r3, #206
	lsls	r3, r3, #3
	adds	r1, r1, r3
	ldrh	r1, [r1, #0]
	movs	r0, #1
	movs	r2, #24
	bl	sub_08118040
	ldr	r5, [pc, #36]
	movs	r1, #128
	adds	r0, r6, #0
	lsls	r1, r1, #7
	movs	r2, #0
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x4806
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r2, #0
	mov	lr, r5
	.2byte 0xf800
	.2byte 0xe008
	movs	r0, r0
	.4byte 0x00000001
	.4byte 0x03001120
	.4byte 0x03000260
	.2byte 0x4000
	.2byte 0x0600
	movs	r0, #1
	bl	sub_08013560
	ldr	r3, [pc, #36]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	ldr	r3, [pc, #28]
	subs	r2, #2
	strh	r3, [r2, #0]
	ldr	r3, [pc, #28]
	mov	r2, r8
	strh	r3, [r2, #0]
	ldr	r2, [pc, #24]
	movs	r3, #120
	movs	r0, #195
	str	r3, [r2, #16]
	lsls	r0, r0, #1
	bl	sub_081c0010
	b.n	.L_0814cd40
	.4byte 0x0000100e
	.4byte 0x00003f46
	.4byte 0x00007741
	.2byte 0x11e0
	.2byte 0x0300
.L_0814cd40:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, pc}
	.align 2, 0
