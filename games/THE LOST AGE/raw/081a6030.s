.syntax unified
	.thumb
	.set sub_08002064, 0x08002064
	.set sub_0801314c, 0x0801314c
	.set sub_08013300, 0x08013300
	.set sub_08013560, 0x08013560
	.set sub_08013fdc, 0x08013fdc
	.set sub_08014368, 0x08014368
	.set sub_080144c0, 0x080144c0
	.set sub_080145a8, 0x080145a8
	.set sub_08014644, 0x08014644
	.set sub_08014878, 0x08014878
	.set sub_08014b70, 0x08014b70
	.set sub_08014bac, 0x08014bac
	.set sub_08014cc0, 0x08014cc0
	.set sub_0801591c, 0x0801591c
	.set sub_081a814c, 0x081a814c
	.set sub_081a81d4, 0x081a81d4
	.set sub_081a81f4, 0x081a81f4
	.set sub_081a8228, 0x081a8228
	.global Overlay_081a6030
Overlay_081a6030:
.L_081a6030:
	push	{r5, lr}
	ldr	r5, [pc, #56]
	movs	r3, #128
	lsls	r3, r3, #19
	strh	r5, [r3, #0]
	ldr	r0, [pc, #52]
	bl	sub_08013300
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	movs	r1, #160
	lsls	r2, r2, #24
	adds	r4, r0, #0
	adds	r3, #212
	lsls	r1, r1, #19
	adds	r2, #128
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r3, #160
	lsls	r3, r3, #19
	strh	r5, [r3, #0]
	ldr	r5, [pc, #20]
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r4, r4, r3
	adds	r1, r5, #0
	adds	r0, r4, #0
	b.n	.L_081a6078
	movs	r0, r0
	.4byte 0x00000000
	.4byte 0x00000018
	.2byte 0x0000
	.2byte 0x0201
.L_081a6078:
	bl	sub_0801591c
	movs	r3, #128
	lsls	r3, r3, #19
	movs	r1, #192
	adds	r3, #212
	adds	r0, r5, #0
	lsls	r1, r1, #19
	ldr	r2, [pc, #4]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	pop	{r5, pc}
	.2byte 0x2700
	.2byte 0x8400
.L_081a6094:
	push	{r5, r6, r7, lr}
	adds	r6, r0, #0
	movs	r5, #0
	cmp	r5, r6
	bcs.n	.L_081a60b8
	ldr	r7, [pc, #28]
.L_081a60a0:
	movs	r0, #1
	bl	sub_08013560
	bl	sub_08014878
	ldr	r3, [r7, #4]
	movs	r0, #1
	cmp	r3, #0
	bne.n	.L_081a60ba
	adds	r5, #1
	cmp	r5, r6
	bcc.n	.L_081a60a0
.L_081a60b8:
	movs	r0, #0
.L_081a60ba:
	pop	{r5, r6, r7, pc}
	.2byte 0x1150
	.2byte 0x0300
.L_081a60c0:
	push	{r5, r6, r7, lr}
	adds	r5, r0, #0
	ldr	r3, [r5, #0]
	movs	r1, #0
	sub	sp, #4
	cmp	r1, r3
	bge.n	.L_081a6144
	ldr	r7, [pc, #124]
	ldr	r6, [pc, #124]
.L_081a60d2:
	ldr	r2, [r5, #20]
	cmp	r2, #0
	beq.n	.L_081a60e0
	ldr	r3, [r7, #0]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_081a60e8
.L_081a60e0:
	ldrh	r3, [r6, #6]
	ldr	r2, [r5, #4]
	adds	r3, r3, r2
	strh	r3, [r6, #6]
.L_081a60e8:
	ldr	r2, [r5, #24]
	cmp	r2, #0
	beq.n	.L_081a60f6
	ldr	r3, [r7, #0]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_081a60fe
.L_081a60f6:
	ldrh	r3, [r6, #4]
	ldr	r2, [r5, #8]
	adds	r3, r3, r2
	strh	r3, [r6, #4]
.L_081a60fe:
	ldr	r2, [r5, #28]
	cmp	r2, #0
	beq.n	.L_081a610c
	ldr	r3, [r7, #0]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_081a6114
.L_081a610c:
	ldrh	r3, [r6, #10]
	ldr	r2, [r5, #12]
	adds	r3, r3, r2
	strh	r3, [r6, #10]
.L_081a6114:
	ldr	r2, [r5, #32]
	cmp	r2, #0
	beq.n	.L_081a6122
	ldr	r3, [r7, #0]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_081a612a
.L_081a6122:
	ldrh	r3, [r6, #8]
	ldr	r2, [r5, #16]
	adds	r3, r3, r2
	strh	r3, [r6, #8]
.L_081a612a:
	movs	r0, #1
	str	r1, [sp, #0]
	bl	.L_081a6094
	ldr	r1, [sp, #0]
	cmp	r0, #0
	beq.n	.L_081a613c
	movs	r0, #0
	b.n	.L_081a6146
.L_081a613c:
	ldr	r3, [r5, #0]
	adds	r1, #1
	cmp	r1, r3
	blt.n	.L_081a60d2
.L_081a6144:
	movs	r0, #1
.L_081a6146:
	add	sp, #4
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x0300122c
	.2byte 0x1120
	.2byte 0x0300
.L_081a6154:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #20]
	movs	r2, #128
	ldr	r1, [pc, #20]
	lsls	r2, r2, #19
	strh	r3, [r2, #0]
	sub	sp, #36
	adds	r6, r0, #0
	movs	r3, #0
	movs	r7, #0
.L_081a6174:
	movs	r5, #31
	b.n	.L_081a6180
	.4byte 0x00000000
	.2byte 0xf800
	.2byte 0x0600
.L_081a6180:
	adds	r2, r3, #0
	movs	r0, #128
	lsls	r3, r2, #16
	lsls	r0, r0, #9
	adds	r3, r3, r0
	subs	r5, #1
	strh	r2, [r1, #0]
	asrs	r3, r3, #16
	adds	r1, #2
	cmp	r5, #0
	bge.n	.L_081a6180
	adds	r7, #1
	cmp	r7, #15
	ble.n	.L_081a6174
	movs	r3, #0
	cmp	r7, #31
	bgt.n	.L_081a61c0
.L_081a61a2:
	movs	r5, #31
.L_081a61a4:
	adds	r2, r3, #0
	movs	r4, #128
	lsls	r3, r2, #16
	lsls	r4, r4, #9
	adds	r3, r3, r4
	subs	r5, #1
	strh	r2, [r1, #0]
	asrs	r3, r3, #16
	adds	r1, #2
	cmp	r5, #0
	bge.n	.L_081a61a4
	adds	r7, #1
	cmp	r7, #31
	ble.n	.L_081a61a2
.L_081a61c0:
	ldr	r1, [pc, #12]
	ldr	r0, [pc, #8]
	movs	r3, #0
	movs	r7, #0
.L_081a61c8:
	movs	r5, #31
	b.n	.L_081a61d4
	.4byte 0x00001000
	.2byte 0xf000
	.2byte 0x0600
.L_081a61d4:
	adds	r2, r3, #0
	lsls	r3, r2, #16
	movs	r4, #128
	lsls	r2, r2, #16
	lsls	r4, r4, #9
	lsrs	r2, r2, #16
	adds	r3, r3, r4
	orrs	r2, r0
	subs	r5, #1
	strh	r2, [r1, #0]
	asrs	r3, r3, #16
	adds	r1, #2
	cmp	r5, #0
	bge.n	.L_081a61d4
	adds	r7, #1
	cmp	r7, #15
	ble.n	.L_081a61c8
	movs	r3, #0
	cmp	r7, #31
	bgt.n	.L_081a6222
	ldr	r0, [pc, #44]
.L_081a61fe:
	movs	r5, #31
.L_081a6200:
	adds	r2, r3, #0
	lsls	r3, r2, #16
	movs	r4, #128
	lsls	r2, r2, #16
	lsls	r4, r4, #9
	lsrs	r2, r2, #16
	adds	r3, r3, r4
	orrs	r2, r0
	subs	r5, #1
	strh	r2, [r1, #0]
	asrs	r3, r3, #16
	adds	r1, #2
	cmp	r5, #0
	bge.n	.L_081a6200
	adds	r7, #1
	cmp	r7, #31
	ble.n	.L_081a61fe
.L_081a6222:
	ldr	r3, [pc, #12]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #10
	b.n	.L_081a6234
	.4byte 0x00001000
	.2byte 0x1f02
	.2byte 0x0000
.L_081a6234:
	strh	r3, [r2, #0]
	ldr	r3, [pc, #44]
	adds	r2, #2
	strh	r3, [r2, #0]
	ldr	r3, [pc, #40]
	adds	r2, #68
	strh	r3, [r2, #0]
	ldr	r3, [pc, #40]
	adds	r2, #2
	strh	r3, [r2, #0]
	ldr	r3, [pc, #36]
	movs	r2, #0
	movs	r5, #3
.L_081a624e:
	subs	r5, #1
	strh	r2, [r3, #2]
	strh	r2, [r3, #0]
	adds	r3, #4
	cmp	r5, #0
	bge.n	.L_081a624e
	cmp	r6, #0
	bne.n	.L_081a6274
	bl	sub_081a814c
	b.n	.L_081a6274
	.4byte 0x00001e05
	.4byte 0x00000244
	.4byte 0x00000a0b
	.2byte 0x1120
	.2byte 0x0300
.L_081a6274:
	movs	r0, #2
	movs	r1, #0
	bl	sub_081a81f4
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #0
	bl	sub_081a81d4
	ldr	r3, [pc, #20]
	movs	r2, #128
	lsls	r2, r2, #19
	strh	r3, [r2, #0]
	cmp	r6, #0
	bne.n	.L_081a62a0
	movs	r0, #1
	bl	sub_081a8228
	b.n	.L_081a62a6
	movs	r0, r0
	.2byte 0x0600
	.2byte 0x0000
.L_081a62a0:
	movs	r0, #10
	bl	sub_081a8228
.L_081a62a6:
	cmp	r6, #0
	beq.n	.L_081a62ac
	b.n	.L_081a6424
.L_081a62ac:
	mov	r5, sp
	movs	r3, #30
	str	r3, [r5, #0]
	movs	r7, #2
	subs	r3, #33
	str	r7, [r5, #4]
	str	r6, [r5, #8]
	str	r3, [r5, #12]
	str	r6, [r5, #16]
	str	r6, [r5, #20]
	str	r6, [r5, #24]
	str	r6, [r5, #28]
	str	r6, [r5, #32]
	adds	r0, r5, #0
	bl	.L_081a60c0
	cmp	r0, #0
	bne.n	.L_081a62d2
	b.n	.L_081a6562
.L_081a62d2:
	movs	r3, #2
	movs	r0, #10
	negs	r3, r3
	mov	sl, r0
	str	r0, [r5, #0]
	movs	r2, #0
	str	r7, [r5, #4]
	str	r6, [r5, #8]
	str	r3, [r5, #12]
	str	r6, [r5, #16]
	str	r6, [r5, #20]
	str	r6, [r5, #24]
	str	r6, [r5, #28]
	str	r6, [r5, #32]
	adds	r0, r5, #0
	mov	fp, r2
	mov	r8, r3
	bl	.L_081a60c0
	cmp	r0, #0
	bne.n	.L_081a62fe
	b.n	.L_081a6562
.L_081a62fe:
	mov	r0, r8
	mov	r4, sl
	movs	r7, #1
	str	r0, [r5, #12]
	str	r4, [r5, #0]
	str	r7, [r5, #4]
	str	r6, [r5, #8]
	str	r6, [r5, #16]
	str	r6, [r5, #20]
	str	r6, [r5, #24]
	str	r6, [r5, #28]
	str	r6, [r5, #32]
	adds	r0, r5, #0
	bl	.L_081a60c0
	cmp	r0, #0
	bne.n	.L_081a6322
	b.n	.L_081a6562
.L_081a6322:
	movs	r3, #1
	negs	r3, r3
	mov	r2, sl
	str	r2, [r5, #0]
	str	r7, [r5, #4]
	str	r6, [r5, #8]
	str	r3, [r5, #12]
	str	r6, [r5, #16]
	str	r6, [r5, #20]
	str	r6, [r5, #24]
	str	r6, [r5, #28]
	str	r6, [r5, #32]
	adds	r0, r5, #0
	mov	r8, r3
	bl	.L_081a60c0
	cmp	r0, #0
	bne.n	.L_081a6348
	b.n	.L_081a6562
.L_081a6348:
	mov	r0, r8
	mov	r4, sl
	str	r0, [r5, #12]
	str	r4, [r5, #0]
	str	r7, [r5, #4]
	str	r6, [r5, #8]
	str	r6, [r5, #16]
	str	r7, [r5, #20]
	str	r6, [r5, #24]
	str	r6, [r5, #28]
	str	r6, [r5, #32]
	adds	r0, r5, #0
	bl	.L_081a60c0
	cmp	r0, #0
	bne.n	.L_081a636a
	b.n	.L_081a6562
.L_081a636a:
	mov	r2, sl
	mov	r3, r8
	str	r2, [r5, #0]
	str	r6, [r5, #4]
	str	r6, [r5, #8]
	str	r3, [r5, #12]
	str	r6, [r5, #16]
	str	r6, [r5, #20]
	str	r6, [r5, #24]
	str	r7, [r5, #28]
	str	r6, [r5, #32]
	adds	r0, r5, #0
	bl	.L_081a60c0
	cmp	r0, #0
	bne.n	.L_081a638c
	b.n	.L_081a6562
.L_081a638c:
	movs	r4, #20
	mov	r0, r8
	str	r0, [r5, #4]
	str	r4, [r5, #0]
	str	r6, [r5, #8]
	str	r6, [r5, #12]
	str	r6, [r5, #16]
	str	r7, [r5, #20]
	str	r6, [r5, #24]
	str	r6, [r5, #28]
	str	r6, [r5, #32]
	adds	r0, r5, #0
	mov	sl, r4
	bl	.L_081a60c0
	cmp	r0, #0
	bne.n	.L_081a63b0
	b.n	.L_081a6562
.L_081a63b0:
	movs	r4, #3
	mov	r2, sl
	mov	r3, r8
	str	r2, [r5, #0]
	str	r3, [r5, #4]
	str	r6, [r5, #8]
	str	r7, [r5, #12]
	str	r6, [r5, #16]
	str	r4, [r5, #20]
	str	r6, [r5, #24]
	str	r7, [r5, #28]
	str	r6, [r5, #32]
	adds	r0, r5, #0
	mov	r9, r4
	bl	.L_081a60c0
	cmp	r0, #0
	bne.n	.L_081a63d6
	b.n	.L_081a6562
.L_081a63d6:
	mov	r0, sl
	mov	r2, r8
	mov	r3, r9
	str	r0, [r5, #0]
	str	r2, [r5, #4]
	str	r6, [r5, #8]
	str	r7, [r5, #12]
	str	r6, [r5, #16]
	str	r3, [r5, #20]
	str	r6, [r5, #24]
	str	r7, [r5, #28]
	str	r6, [r5, #32]
	adds	r0, r5, #0
	bl	.L_081a60c0
	cmp	r0, #0
	bne.n	.L_081a63fa
	b.n	.L_081a6562
.L_081a63fa:
	movs	r0, #2
	movs	r1, #0
	bl	sub_081a81d4
	movs	r0, #60
	bl	sub_081a8228
	mov	r0, fp
	movs	r3, #60
	mov	r4, r8
	mov	r2, r9
	str	r0, [r5, #8]
	str	r0, [r5, #16]
	str	r0, [r5, #24]
	str	r0, [r5, #32]
	str	r3, [r5, #0]
	str	r4, [r5, #4]
	str	r7, [r5, #12]
	str	r2, [r5, #20]
	str	r7, [r5, #28]
	b.n	.L_081a6558
.L_081a6424:
	cmp	r6, #1
	beq.n	.L_081a642a
	b.n	.L_081a6566
.L_081a642a:
	mov	r5, sp
	movs	r3, #40
	movs	r7, #0
	str	r3, [r5, #0]
	subs	r3, #41
	str	r6, [r5, #4]
	str	r6, [r5, #8]
	str	r3, [r5, #12]
	str	r3, [r5, #16]
	str	r6, [r5, #20]
	str	r7, [r5, #24]
	str	r6, [r5, #28]
	str	r7, [r5, #32]
	adds	r0, r5, #0
	mov	r8, r3
	bl	.L_081a60c0
	cmp	r0, #0
	bne.n	.L_081a6452
	b.n	.L_081a6562
.L_081a6452:
	mov	r0, r8
	movs	r4, #20
	movs	r2, #3
	str	r0, [r5, #12]
	str	r0, [r5, #16]
	str	r4, [r5, #0]
	str	r6, [r5, #4]
	str	r6, [r5, #8]
	str	r2, [r5, #20]
	str	r7, [r5, #24]
	str	r6, [r5, #28]
	str	r7, [r5, #32]
	adds	r0, r5, #0
	mov	fp, r4
	mov	sl, r2
	bl	.L_081a60c0
	cmp	r0, #0
	beq.n	.L_081a6562
	movs	r3, #10
	mov	r4, r8
	mov	r0, sl
	mov	r9, r3
	str	r3, [r5, #0]
	movs	r3, #7
	str	r0, [r5, #24]
	str	r6, [r5, #4]
	str	r6, [r5, #8]
	str	r4, [r5, #12]
	str	r4, [r5, #16]
	str	r3, [r5, #20]
	str	r6, [r5, #28]
	str	r7, [r5, #32]
	adds	r0, r5, #0
	bl	.L_081a60c0
	cmp	r0, #0
	beq.n	.L_081a6562
	mov	r2, r9
	mov	r3, r8
	mov	r4, sl
	str	r2, [r5, #0]
	str	r7, [r5, #4]
	str	r6, [r5, #8]
	str	r7, [r5, #12]
	str	r3, [r5, #16]
	str	r7, [r5, #20]
	str	r4, [r5, #24]
	str	r7, [r5, #28]
	str	r7, [r5, #32]
	adds	r0, r5, #0
	bl	.L_081a60c0
	cmp	r0, #0
	beq.n	.L_081a6562
	mov	r0, r8
	movs	r2, #15
	movs	r3, #30
	str	r0, [r5, #4]
	str	r0, [r5, #16]
	str	r3, [r5, #0]
	str	r7, [r5, #8]
	str	r7, [r5, #12]
	str	r2, [r5, #20]
	str	r6, [r5, #24]
	str	r7, [r5, #28]
	str	r7, [r5, #32]
	adds	r0, r5, #0
	mov	sl, r2
	bl	.L_081a60c0
	cmp	r0, #0
	beq.n	.L_081a6562
	mov	r4, r8
	mov	r0, sl
	mov	r3, fp
	str	r0, [r5, #20]
	str	r3, [r5, #0]
	str	r4, [r5, #4]
	str	r4, [r5, #8]
	str	r7, [r5, #12]
	str	r4, [r5, #16]
	str	r6, [r5, #24]
	str	r7, [r5, #28]
	str	r7, [r5, #32]
	adds	r0, r5, #0
	bl	.L_081a60c0
	cmp	r0, #0
	beq.n	.L_081a6562
	movs	r4, #2
	movs	r2, #60
	mov	r3, r8
	negs	r4, r4
	mov	r0, sl
	str	r0, [r5, #20]
	str	r2, [r5, #0]
	str	r3, [r5, #4]
	str	r3, [r5, #8]
	str	r7, [r5, #12]
	str	r4, [r5, #16]
	str	r6, [r5, #24]
	str	r7, [r5, #28]
	str	r7, [r5, #32]
	adds	r0, r5, #0
	mov	fp, r2
	mov	r9, r4
	bl	.L_081a60c0
	cmp	r0, #0
	beq.n	.L_081a6562
	movs	r0, #2
	movs	r1, #0
	bl	sub_081a81d4
	movs	r0, #60
	bl	sub_081a8228
	mov	r3, r8
	mov	r0, sl
	mov	r2, fp
	mov	r4, r9
	str	r0, [r5, #20]
	str	r2, [r5, #0]
	str	r3, [r5, #4]
	str	r3, [r5, #8]
	str	r7, [r5, #12]
	str	r4, [r5, #16]
	str	r6, [r5, #24]
	str	r7, [r5, #28]
	str	r7, [r5, #32]
.L_081a6558:
	adds	r0, r5, #0
	bl	.L_081a60c0
	cmp	r0, #0
	bne.n	.L_081a6624
.L_081a6562:
	movs	r0, #0
	b.n	.L_081a6626
.L_081a6566:
	mov	r5, sp
	movs	r3, #60
	str	r3, [r5, #0]
	subs	r3, #61
	movs	r6, #0
	str	r3, [r5, #8]
	str	r3, [r5, #12]
	movs	r3, #1
	str	r6, [r5, #4]
	str	r3, [r5, #16]
	str	r6, [r5, #20]
	str	r6, [r5, #24]
	str	r6, [r5, #28]
	str	r6, [r5, #32]
	adds	r0, r5, #0
	bl	.L_081a60c0
	cmp	r0, #0
	beq.n	.L_081a6562
	movs	r3, #16
	movs	r2, #8
	str	r3, [r5, #0]
	subs	r3, #24
	str	r2, [r5, #4]
	str	r3, [r5, #8]
	str	r2, [r5, #12]
	str	r2, [r5, #16]
	str	r6, [r5, #20]
	str	r6, [r5, #24]
	str	r6, [r5, #28]
	str	r6, [r5, #32]
	adds	r0, r5, #0
	bl	.L_081a60c0
	cmp	r0, #0
	beq.n	.L_081a6562
	movs	r2, #128
	lsls	r2, r2, #19
	ldr	r6, [pc, #128]
	adds	r2, #82
	movs	r7, #2
	movs	r5, #11
	mov	r8, r2
.L_081a65bc:
	lsls	r3, r5, #8
	orrs	r3, r5
	mov	r4, r8
	strh	r3, [r4, #0]
	movs	r0, #1
	ldrh	r3, [r6, #6]
	adds	r3, r3, r7
	adds	r3, #16
	strh	r3, [r6, #6]
	ldrh	r3, [r6, #10]
	adds	r3, r3, r7
	adds	r3, #8
	strh	r3, [r6, #10]
	adds	r7, #1
	bl	.L_081a6094
	cmp	r0, #0
	bne.n	.L_081a6562
	ldrh	r3, [r6, #6]
	movs	r0, #1
	adds	r3, r3, r7
	adds	r3, #16
	strh	r3, [r6, #6]
	ldrh	r3, [r6, #10]
	adds	r3, r3, r7
	adds	r3, #8
	strh	r3, [r6, #10]
	adds	r7, #1
	bl	.L_081a6094
	cmp	r0, #0
	bne.n	.L_081a6562
	adds	r5, #1
	cmp	r5, #15
	ble.n	.L_081a65bc
	movs	r0, #254
	lsls	r0, r0, #7
	adds	r0, #255
	movs	r1, #0
	bl	sub_081a81d4
	movs	r0, #1
	bl	sub_081a8228
	movs	r0, #2
	bl	.L_081a6094
	cmp	r0, #0
	beq.n	.L_081a6624
	bl	sub_08013fdc
	b.n	.L_081a6562
.L_081a6624:
	movs	r0, #1
.L_081a6626:
	add	sp, #36
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x1120
	.2byte 0x0300
.L_081a6638:
	push	{r5, r6, lr}
	ldr	r3, [pc, #28]
	movs	r2, #128
	lsls	r2, r2, #19
	strh	r3, [r2, #0]
	ldr	r0, [pc, #28]
	movs	r6, #0
	movs	r4, #0
.L_081a6648:
	adds	r3, r4, #0
	subs	r3, #9
	cmp	r3, #1
	bhi.n	.L_081a6682
	ldr	r5, [pc, #8]
	movs	r1, #0
	b.n	.L_081a6664
	movs	r0, r0
	.4byte 0x00000000
	.4byte 0x00002000
	.2byte 0xe800
	.2byte 0x0600
.L_081a6664:
	adds	r2, r6, #0
	lsls	r3, r2, #16
	movs	r6, #128
	lsls	r2, r2, #16
	lsls	r6, r6, #9
	lsrs	r2, r2, #16
	adds	r3, r3, r6
	orrs	r2, r5
	adds	r1, #1
	strh	r2, [r0, #0]
	asrs	r6, r3, #16
	adds	r0, #2
	cmp	r1, #31
	ble.n	.L_081a6664
	b.n	.L_081a6690
.L_081a6682:
	ldr	r3, [pc, #48]
	movs	r1, #31
.L_081a6686:
	subs	r1, #1
	strh	r3, [r0, #0]
	adds	r0, #2
	cmp	r1, #0
	bge.n	.L_081a6686
.L_081a6690:
	adds	r4, #1
	cmp	r4, #19
	ble.n	.L_081a6648
	ldr	r3, [pc, #32]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #14
	strh	r3, [r2, #0]
	ldr	r2, [pc, #24]
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #80
	strh	r2, [r3, #0]
	adds	r3, #2
	strh	r2, [r3, #0]
	ldr	r3, [pc, #16]
	movs	r2, #0
	b.n	.L_081a66c4
	.4byte 0x00002000
	.4byte 0x00001d0a
	.4byte 0x00000000
	.2byte 0x1120
	.2byte 0x0300
.L_081a66c4:
	movs	r1, #3
.L_081a66c6:
	subs	r1, #1
	strh	r2, [r3, #2]
	strh	r2, [r3, #0]
	adds	r3, #4
	cmp	r1, #0
	bge.n	.L_081a66c6
	movs	r0, #0
	movs	r1, #0
	bl	sub_081a81f4
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #0
	bl	sub_081a81d4
	ldr	r3, [pc, #48]
	movs	r2, #128
	lsls	r2, r2, #19
	movs	r0, #40
	strh	r3, [r2, #0]
	bl	sub_081a8228
	movs	r0, #150
	bl	.L_081a6094
	cmp	r0, #0
	bne.n	.L_081a671c
	movs	r0, #2
	movs	r1, #0
	bl	sub_081a81d4
	movs	r0, #40
	bl	sub_081a8228
	movs	r0, #60
	bl	.L_081a6094
	cmp	r0, #0
	beq.n	.L_081a6720
	b.n	.L_081a671c
	movs	r0, r0
	.2byte 0x0800
	.2byte 0x0000
.L_081a671c:
	movs	r0, #0
	b.n	.L_081a6722
.L_081a6720:
	movs	r0, #1
.L_081a6722:
	pop	{r5, r6, pc}
.L_081a6724:
	push	{r5, r6, r7, lr}
	ldr	r3, [pc, #24]
	movs	r2, #128
	lsls	r2, r2, #19
	strh	r3, [r2, #0]
	ldr	r1, [pc, #24]
	movs	r6, #64
	movs	r0, #0
.L_081a6734:
	subs	r3, r0, #7
	cmp	r3, #2
	bhi.n	.L_081a676a
	ldr	r4, [pc, #8]
	movs	r5, #0
	b.n	.L_081a674c
	.4byte 0x00000000
	.4byte 0x00003000
	.2byte 0xe800
	.2byte 0x0600
.L_081a674c:
	adds	r2, r6, #0
	lsls	r3, r2, #16
	movs	r6, #128
	lsls	r2, r2, #16
	lsls	r6, r6, #9
	lsrs	r2, r2, #16
	adds	r3, r3, r6
	orrs	r2, r4
	adds	r5, #1
	strh	r2, [r1, #0]
	asrs	r6, r3, #16
	adds	r1, #2
	cmp	r5, #31
	ble.n	.L_081a674c
	b.n	.L_081a6796
.L_081a676a:
	adds	r3, r0, #0
	subs	r3, #10
	cmp	r3, #1
	bhi.n	.L_081a6788
	ldr	r3, [pc, #16]
	movs	r5, #0
.L_081a6776:
	adds	r5, #1
	strh	r3, [r1, #0]
	adds	r1, #2
	cmp	r5, #31
	ble.n	.L_081a6776
	b.n	.L_081a6796
	movs	r0, r0
	.2byte 0x3060
	.2byte 0x0000
.L_081a6788:
	ldr	r3, [pc, #36]
	movs	r5, #31
.L_081a678c:
	subs	r5, #1
	strh	r3, [r1, #0]
	adds	r1, #2
	cmp	r5, #0
	bge.n	.L_081a678c
.L_081a6796:
	adds	r0, #1
	cmp	r0, #19
	ble.n	.L_081a6734
	ldr	r1, [pc, #24]
	movs	r6, #160
	movs	r0, #0
.L_081a67a2:
	adds	r3, r0, #0
	subs	r3, #10
	cmp	r3, #1
	bhi.n	.L_081a67da
	ldr	r4, [pc, #8]
	movs	r5, #0
	b.n	.L_081a67bc
	.4byte 0x00003040
	.4byte 0x00003000
	.2byte 0xe000
	.2byte 0x0600
.L_081a67bc:
	adds	r2, r6, #0
	lsls	r3, r2, #16
	movs	r6, #128
	lsls	r2, r2, #16
	lsls	r6, r6, #9
	lsrs	r2, r2, #16
	adds	r3, r3, r6
	orrs	r2, r4
	adds	r5, #1
	strh	r2, [r1, #0]
	asrs	r6, r3, #16
	adds	r1, #2
	cmp	r5, #31
	ble.n	.L_081a67bc
	b.n	.L_081a67e8
.L_081a67da:
	ldr	r3, [pc, #44]
	movs	r5, #31
.L_081a67de:
	subs	r5, #1
	strh	r3, [r1, #0]
	adds	r1, #2
	cmp	r5, #0
	bge.n	.L_081a67de
.L_081a67e8:
	adds	r0, #1
	cmp	r0, #19
	ble.n	.L_081a67a2
	ldr	r3, [pc, #28]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #14
	strh	r3, [r2, #0]
	ldr	r3, [pc, #20]
	subs	r2, #6
	strh	r3, [r2, #0]
	ldr	r3, [pc, #20]
	adds	r2, #72
	strh	r3, [r2, #0]
	ldr	r3, [pc, #16]
	b.n	.L_081a681c
	.4byte 0x00003040
	.4byte 0x00001d0a
	.4byte 0x00001c09
	.4byte 0x00000841
	.2byte 0x1000
	.2byte 0x0000
.L_081a681c:
	adds	r2, #2
	strh	r3, [r2, #0]
	ldr	r3, [pc, #76]
	movs	r2, #0
	movs	r5, #3
.L_081a6826:
	subs	r5, #1
	strh	r2, [r3, #2]
	strh	r2, [r3, #0]
	adds	r3, #4
	cmp	r5, #0
	bge.n	.L_081a6826
	movs	r0, #0
	movs	r1, #0
	bl	sub_081a81f4
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #0
	bl	sub_081a81d4
	ldr	r3, [pc, #32]
	movs	r2, #128
	lsls	r2, r2, #19
	movs	r0, #40
	strh	r3, [r2, #0]
	bl	sub_081a8228
	movs	r0, #60
	bl	.L_081a6094
	cmp	r0, #0
	bne.n	.L_081a68ac
	movs	r7, #128
	ldr	r6, [pc, #12]
	lsls	r7, r7, #19
	movs	r5, #1
	adds	r7, #82
	b.n	.L_081a6874
	.4byte 0x00000900
	.4byte 0x00001000
	.2byte 0x1120
	.2byte 0x0300
.L_081a6874:
	adds	r3, r5, #0
	orrs	r3, r6
	strh	r3, [r7, #0]
	movs	r0, #3
	bl	.L_081a6094
	cmp	r0, #0
	bne.n	.L_081a68ac
	adds	r5, #1
	cmp	r5, #16
	ble.n	.L_081a6874
	movs	r0, #150
	bl	.L_081a6094
	cmp	r0, #0
	bne.n	.L_081a68ac
	movs	r0, #2
	movs	r1, #0
	bl	sub_081a81d4
	movs	r0, #40
	bl	sub_081a8228
	movs	r0, #60
	bl	.L_081a6094
	cmp	r0, #0
	beq.n	.L_081a68b0
.L_081a68ac:
	movs	r0, #0
	b.n	.L_081a68b2
.L_081a68b0:
	movs	r0, #1
.L_081a68b2:
	pop	{r5, r6, r7, pc}
.L_081a68b4:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #172
	ldr	r3, [r3, #0]
	adds	r5, r0, #0
	mov	r8, r3
	movs	r0, #4
	ldrsh	r3, [r5, r0]
	ldrh	r1, [r5, #4]
	cmp	r3, #0
	bgt.n	.L_081a68d2
	b.n	.L_081a69f6
.L_081a68d2:
	movs	r1, #0
	ldrsh	r7, [r5, r1]
	movs	r2, #2
	ldrsh	r3, [r5, r2]
	cmp	r7, r3
	blt.n	.L_081a68e0
	b.n	.L_081a6a9c
.L_081a68e0:
	ldr	r0, [r5, #28]
	bl	.L_081a6094
	cmp	r0, #0
	beq.n	.L_081a68ec
	b.n	.L_081a69f2
.L_081a68ec:
	movs	r0, #22
	ldrsh	r3, [r5, r0]
	adds	r2, r3, #0
	cmp	r3, #0
	ble.n	.L_081a6978
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #212
	ldr	r0, [r5, #8]
	ldr	r1, [r5, #12]
	ldr	r2, [pc, #108]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r3, [r5, #8]
	movs	r2, #240
	lsls	r2, r2, #3
	adds	r3, r3, r2
	str	r3, [r5, #8]
	ldr	r3, [r5, #12]
	adds	r6, r5, #0
	adds	r3, r3, r2
	str	r3, [r5, #12]
	adds	r6, #16
	adds	r0, r6, #0
	movs	r4, #29
.L_081a691e:
	ldrh	r2, [r5, #20]
	ldr	r1, [r0, #0]
	adds	r3, r2, #1
	strh	r3, [r5, #20]
	subs	r4, #1
	strh	r2, [r1, #0]
	adds	r1, #2
	str	r1, [r0, #0]
	cmp	r4, #0
	bge.n	.L_081a691e
	ldr	r3, [r6, #0]
	ldr	r2, [pc, #52]
	movs	r1, #128
	strh	r2, [r3, #0]
	adds	r3, #2
	strh	r2, [r3, #0]
	adds	r3, #2
	str	r3, [r6, #0]
	lsls	r1, r1, #14
	ldrh	r3, [r5, #24]
	movs	r2, #0
	adds	r3, #1
	strh	r3, [r5, #24]
	lsls	r3, r3, #16
	cmp	r3, r1
	bne.n	.L_081a6960
	movs	r3, #192
	lsls	r3, r3, #19
	str	r3, [r5, #12]
	ldr	r3, [pc, #24]
	strh	r2, [r5, #24]
	strh	r2, [r5, #20]
	str	r3, [r6, #0]
.L_081a6960:
	ldrh	r3, [r5, #22]
	subs	r3, #8
	strh	r3, [r5, #22]
	adds	r2, r3, #0
	b.n	.L_081a6978
	movs	r0, r0
	.4byte 0x000001ff
	.4byte 0x840001e0
	.2byte 0xf800
	.2byte 0x0600
.L_081a6978:
	ldrh	r1, [r5, #4]
	adds	r3, r2, r1
	ldr	r2, [pc, #296]
	strh	r3, [r5, #22]
	ldrh	r3, [r2, #6]
	adds	r3, r3, r1
	strh	r3, [r2, #6]
	mov	r3, r8
	ldrh	r2, [r3, #10]
	movs	r0, #10
	ldrsh	r3, [r3, r0]
	cmp	r3, #0
	beq.n	.L_081a69b4
	subs	r3, r2, #1
	mov	r1, r8
	movs	r2, #128
	strh	r3, [r1, #10]
	lsls	r2, r2, #9
	lsls	r3, r3, #16
	cmp	r3, r2
	bne.n	.L_081a69b4
	movs	r0, #254
	lsls	r0, r0, #7
	adds	r0, #255
	movs	r1, #0
	bl	sub_081a81d4
	movs	r0, #10
	bl	sub_081a8228
.L_081a69b4:
	mov	r3, r8
	ldrh	r2, [r3, #12]
	movs	r0, #12
	ldrsh	r3, [r3, r0]
	cmp	r3, #0
	beq.n	.L_081a69e0
	subs	r3, r2, #1
	mov	r1, r8
	movs	r2, #128
	strh	r3, [r1, #12]
	lsls	r2, r2, #9
	lsls	r3, r3, #16
	cmp	r3, r2
	bne.n	.L_081a69e0
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #0
	bl	sub_081a81d4
	movs	r0, #10
	bl	sub_081a8228
.L_081a69e0:
	movs	r0, #4
	ldrsh	r3, [r5, r0]
	adds	r7, r7, r3
	movs	r1, #2
	ldrsh	r3, [r5, r1]
	cmp	r7, r3
	bge.n	.L_081a69f0
	b.n	.L_081a68e0
.L_081a69f0:
	b.n	.L_081a6a9c
.L_081a69f2:
	movs	r0, #1
	b.n	.L_081a6aa0
.L_081a69f6:
	movs	r2, #0
	ldrsh	r7, [r5, r2]
	movs	r0, #2
	ldrsh	r3, [r5, r0]
	cmp	r7, r3
	ble.n	.L_081a6a9c
.L_081a6a02:
	ldr	r3, [pc, #164]
	ldrh	r2, [r3, #6]
	adds	r2, r2, r1
	strh	r2, [r3, #6]
	ldrh	r3, [r5, #22]
	subs	r3, r3, r1
	movs	r1, #128
	strh	r3, [r5, #22]
	lsls	r1, r1, #12
	lsls	r3, r3, #16
	cmp	r3, r1
	ble.n	.L_081a6a80
	ldrh	r3, [r5, #24]
	movs	r2, #255
	lsls	r2, r2, #8
	adds	r2, #255
	adds	r3, r3, r2
	strh	r3, [r5, #24]
	lsls	r3, r3, #16
	cmp	r3, #0
	bne.n	.L_081a6a7a
	movs	r3, #1
	strh	r3, [r5, #24]
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #212
	ldr	r0, [r5, #8]
	ldr	r1, [r5, #12]
	ldr	r2, [pc, #112]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r3, [r5, #8]
	ldr	r2, [pc, #108]
	adds	r0, r5, #0
	adds	r3, r3, r2
	str	r3, [r5, #8]
	ldr	r3, [r5, #12]
	adds	r0, #16
	adds	r3, r3, r2
	str	r3, [r5, #12]
	movs	r4, #29
.L_081a6a54:
	ldrh	r2, [r5, #20]
	ldr	r1, [r0, #0]
	adds	r3, r2, #1
	strh	r3, [r5, #20]
	subs	r4, #1
	strh	r2, [r1, #0]
	adds	r1, #2
	str	r1, [r0, #0]
	cmp	r4, #0
	bge.n	.L_081a6a54
	ldr	r3, [r5, #16]
	movs	r0, #255
	subs	r3, #60
	str	r3, [r5, #16]
	ldrh	r3, [r5, #20]
	lsls	r0, r0, #8
	adds	r0, #226
	adds	r3, r3, r0
	strh	r3, [r5, #20]
.L_081a6a7a:
	ldrh	r3, [r5, #22]
	subs	r3, #8
	strh	r3, [r5, #22]
.L_081a6a80:
	ldr	r0, [r5, #28]
	bl	.L_081a6094
	cmp	r0, #0
	bne.n	.L_081a69f2
	ldrh	r3, [r5, #4]
	adds	r1, r3, #0
	lsls	r3, r1, #16
	asrs	r3, r3, #16
	adds	r7, r7, r3
	movs	r2, #2
	ldrsh	r3, [r5, r2]
	cmp	r7, r3
	bgt.n	.L_081a6a02
.L_081a6a9c:
	strh	r7, [r5, #0]
	movs	r0, #0
.L_081a6aa0:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x03001120
	.4byte 0x840001e0
	.2byte 0xf880
	.2byte 0xffff
.L_081a6ab4:
	push	{r5, r6, lr}
	sub	sp, #12
	adds	r6, r0, #0
	movs	r3, #0
	add	r0, sp, #8
	mov	ip, r2
	str	r3, [r0, #0]
	movs	r2, #133
	movs	r3, #128
	mov	r4, sp
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r5, r1, #0
	adds	r3, #212
	adds	r1, r4, #0
	adds	r2, #2
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldrb	r3, [r4, #1]
	movs	r2, #63
	ands	r2, r3
	ldrb	r3, [r4, #3]
	movs	r1, #192
	orrs	r3, r1
	strb	r3, [r4, #3]
	movs	r3, #32
	orrs	r2, r3
	movs	r3, #13
	negs	r3, r3
	ands	r2, r3
	movs	r3, #4
	orrs	r2, r3
	movs	r3, #3
	orrs	r2, r3
	ldr	r3, [pc, #52]
	strb	r2, [r4, #1]
	adds	r2, r6, #0
	ldrh	r1, [r4, #4]
	ands	r2, r3
	ldr	r3, [pc, #52]
	subs	r5, #32
	ands	r3, r1
	orrs	r3, r2
	strh	r3, [r4, #4]
	ldr	r3, [pc, #36]
	ldrh	r2, [r4, #2]
	ands	r5, r3
	ldr	r3, [pc, #40]
	lsrs	r6, r6, #4
	ands	r3, r2
	movs	r2, #224
	orrs	r3, r5
	add	ip, r2
	strh	r3, [r4, #2]
	lsls	r2, r2, #19
	mov	r3, ip
	strb	r3, [r4, #0]
	adds	r1, r6, r2
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	b.n	.L_081a6b40
	.4byte 0x000003ff
	.4byte 0x000001ff
	.4byte 0xfffffc00
	.2byte 0xfe00
	.2byte 0xffff
.L_081a6b40:
	lsls	r2, r2, #24
	adds	r3, #212
	adds	r0, r4, #0
	adds	r2, #2
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	add	sp, #12
	pop	{r5, r6, pc}
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	sub	sp, #28
	adds	r3, #172
	add	r5, sp, #24
	ldr	r6, [r3, #0]
	movs	r3, #0
	str	r3, [r5, #0]
	movs	r3, #128
	lsls	r3, r3, #19
	movs	r1, #224
	adds	r3, #212
	adds	r0, r5, #0
	lsls	r1, r1, #19
	ldr	r2, [pc, #216]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r0, #0
	ldrsh	r3, [r6, r0]
	ldrh	r2, [r6, #0]
	cmp	r3, #0
	beq.n	.L_081a6b8c
	b.n	.L_081a6d62
.L_081a6b8c:
	movs	r1, #4
	ldrsh	r3, [r6, r1]
	ldrh	r2, [r6, #4]
	cmp	r3, #0
	bne.n	.L_081a6bbc
	movs	r0, #2
	ldrsh	r3, [r6, r0]
	ldr	r1, [pc, #188]
	lsls	r3, r3, #1
	adds	r3, #10
	ldrsh	r3, [r1, r3]
	movs	r0, #1
	negs	r0, r0
	ldrh	r2, [r6, #2]
	cmp	r3, r0
	beq.n	.L_081a6bb2
	adds	r3, r2, #5
	strh	r3, [r6, #2]
	adds	r2, r3, #0
.L_081a6bb2:
	lsls	r3, r2, #16
	asrs	r3, r3, #15
	adds	r3, #4
	ldrh	r3, [r1, r3]
	b.n	.L_081a6bbe
.L_081a6bbc:
	subs	r3, r2, #1
.L_081a6bbe:
	strh	r3, [r6, #4]
	movs	r1, #8
	ldrsh	r3, [r6, r1]
	ldrh	r2, [r6, #8]
	cmp	r3, #0
	bne.n	.L_081a6bdc
	ldrh	r3, [r6, #6]
	ldr	r2, [pc, #140]
	adds	r3, #3
	strh	r3, [r6, #6]
	lsls	r3, r3, #16
	asrs	r3, r3, #15
	adds	r3, #4
	ldrh	r3, [r2, r3]
	b.n	.L_081a6bde
.L_081a6bdc:
	subs	r3, r2, #1
.L_081a6bde:
	strh	r3, [r6, #8]
	movs	r2, #6
	ldrsh	r3, [r6, r2]
	ldr	r7, [pc, #116]
	lsls	r4, r3, #1
	ldrsh	r0, [r7, r4]
	movs	r1, #1
	negs	r1, r1
	cmp	r0, r1
	bne.n	.L_081a6c78
	ldr	r0, [pc, #108]
	bl	sub_08013300
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	ldr	r1, [pc, #96]
	adds	r2, #128
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r5, [pc, #92]
	ldr	r0, [pc, #92]
	bl	sub_08013300
	adds	r1, r5, #0
	bl	sub_0801591c
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #212
	adds	r0, r5, #0
	ldr	r1, [pc, #76]
	ldr	r2, [pc, #80]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r3, #1
	movs	r2, #0
	strh	r3, [r6, #0]
	strh	r3, [r6, #2]
	movs	r3, #160
	strh	r2, [r6, #4]
	strh	r3, [r6, #6]
	strh	r2, [r6, #8]
	ldr	r3, [pc, #16]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #80
	strh	r3, [r2, #0]
	ldr	r3, [pc, #12]
	adds	r2, #2
	strh	r3, [r2, #0]
	b.n	.L_081a6e82
	movs	r0, r0
	.4byte 0x00000250
	.4byte 0x00001001
	.4byte 0x85000100
	.4byte 0x081a8378
	.4byte 0x081a84e6
	.4byte 0x0000001b
	.4byte 0x05000200
	.4byte 0x02026800
	.4byte 0x0000001c
	.4byte 0x06010000
	.2byte 0x0c00
	.2byte 0x8400
.L_081a6c78:
	movs	r2, #2
	ldrsh	r3, [r6, r2]
	ldr	r1, [pc, #164]
	lsls	r3, r3, #1
	ldrsh	r6, [r1, r3]
	str	r6, [sp, #12]
	adds	r6, r3, #2
	ldrsh	r6, [r1, r6]
	adds	r2, r3, #6
	str	r6, [sp, #4]
	adds	r3, #8
	ldrsh	r2, [r1, r2]
	ldrsh	r1, [r1, r3]
	adds	r2, r0, r2
	subs	r2, #28
	str	r2, [sp, #8]
	adds	r3, r4, #2
	ldrsh	r3, [r7, r3]
	ldr	r2, [sp, #4]
	adds	r3, r3, r1
	movs	r1, #0
	mov	r9, r1
	cmp	r9, r2
	blt.n	.L_081a6caa
	b.n	.L_081a6e82
.L_081a6caa:
	movs	r6, #16
	str	r1, [sp, #0]
	add	r6, sp
	mov	ip, r6
	mov	sl, r5
	mov	r8, r3
.L_081a6cb6:
	ldr	r1, [sp, #12]
	ldr	r2, [sp, #0]
	ldr	r6, [sp, #8]
	mov	r0, r9
	movs	r3, #7
	lsls	r7, r0, #6
	adds	r5, r1, r2
	mov	fp, r8
	mov	lr, r3
.L_081a6cc8:
	movs	r3, #0
	mov	r0, sl
	str	r3, [r0, #0]
	movs	r2, #133
	movs	r3, #128
	mov	r4, ip
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	adds	r1, r4, #0
	adds	r2, #2
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r3, [pc, #56]
	adds	r2, r5, #0
	ldrh	r1, [r4, #4]
	ands	r2, r3
	ldr	r3, [pc, #60]
	adds	r0, r4, #0
	ands	r3, r1
	orrs	r3, r2
	strh	r3, [r4, #4]
	ldr	r3, [pc, #40]
	adds	r2, r6, #0
	ldrh	r1, [r4, #2]
	ands	r2, r3
	ldr	r3, [pc, #44]
	ands	r3, r1
	orrs	r3, r2
	add	r1, sp, #16
	mov	r2, fp
	strh	r3, [r4, #2]
	strb	r2, [r1, #0]
	movs	r2, #224
	lsls	r2, r2, #19
	mov	ip, r1
	movs	r3, #128
	adds	r1, r7, r2
	movs	r2, #132
	lsls	r3, r3, #19
	b.n	.L_081a6d30
	movs	r0, r0
	.4byte 0x000003ff
	.4byte 0x000001ff
	.4byte 0x081a8378
	.4byte 0xfffffc00
	.2byte 0xfe00
	.2byte 0xffff
.L_081a6d30:
	lsls	r2, r2, #24
	adds	r3, #212
	adds	r2, #2
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r3, #1
	negs	r3, r3
	add	lr, r3
	mov	r0, lr
	adds	r7, #8
	adds	r6, #8
	adds	r5, #1
	cmp	r0, #0
	bge.n	.L_081a6cc8
	ldr	r2, [sp, #0]
	ldr	r6, [sp, #4]
	movs	r3, #1
	movs	r1, #8
	adds	r2, #8
	add	r9, r3
	add	r8, r1
	str	r2, [sp, #0]
	cmp	r9, r6
	blt.n	.L_081a6cb6
	b.n	.L_081a6e82
.L_081a6d62:
	movs	r0, #128
	lsls	r3, r2, #16
	lsls	r0, r0, #9
	cmp	r3, r0
	beq.n	.L_081a6d6e
	b.n	.L_081a6e82
.L_081a6d6e:
	movs	r2, #6
	ldrsh	r1, [r6, r2]
	ldr	r0, [pc, #144]
	mov	ip, r1
	add	r0, ip
	adds	r3, r0, #0
	cmp	r0, #0
	bge.n	.L_081a6d82
	mov	r3, ip
	subs	r3, #253
.L_081a6d82:
	asrs	r3, r3, #2
	adds	r1, r3, #0
	adds	r3, r0, #0
	adds	r1, #30
	cmp	r3, #0
	bge.n	.L_081a6d92
	mov	r3, ip
	subs	r3, #225
.L_081a6d92:
	asrs	r3, r3, #5
	adds	r5, r3, #0
	adds	r5, #50
	movs	r0, #0
	adds	r2, r5, #0
	bl	.L_081a6ab4
	movs	r0, #128
	movs	r1, #83
	adds	r2, r5, #0
	bl	.L_081a6ab4
	movs	r0, #6
	ldrsh	r3, [r6, r0]
	ldr	r1, [pc, #84]
	adds	r2, r3, r1
	cmp	r2, #0
	bge.n	.L_081a6dba
	adds	r2, r3, #0
	subs	r2, #253
.L_081a6dba:
	movs	r7, #128
	movs	r3, #140
	asrs	r2, r2, #2
	lsls	r7, r7, #1
	subs	r1, r3, r2
	adds	r0, r7, #0
	adds	r2, r5, #0
	bl	.L_081a6ab4
	ldrh	r2, [r6, #6]
	movs	r3, #224
	lsls	r3, r3, #19
	adds	r3, #6
	strh	r2, [r3, #0]
	ldr	r2, [pc, #40]
	adds	r3, #8
	strh	r2, [r3, #0]
	adds	r3, #8
	strh	r2, [r3, #0]
	ldrh	r2, [r6, #6]
	strh	r2, [r3, #8]
	movs	r0, #6
	ldrsh	r3, [r6, r0]
	ldrh	r2, [r6, #6]
	cmp	r3, #255
	bgt.n	.L_081a6e08
	adds	r3, r2, #2
	strh	r3, [r6, #6]
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	cmp	r3, r7
	ble.n	.L_081a6e08
	strh	r7, [r6, #6]
	b.n	.L_081a6e08
	movs	r0, r0
	.4byte 0x00000000
	.2byte 0xff00
	.2byte 0xffff
.L_081a6e08:
	movs	r1, #2
	ldrsh	r3, [r6, r1]
	ldrh	r5, [r6, #2]
	cmp	r3, #15
	bgt.n	.L_081a6e82
	ldrh	r0, [r6, #4]
	movs	r1, #3
	adds	r0, #1
	strh	r0, [r6, #4]
	lsls	r0, r0, #16
	asrs	r0, r0, #16
	bl	sub_08002064
	lsls	r0, r0, #16
	cmp	r0, #0
	bne.n	.L_081a6e38
	ldr	r2, [pc, #36]
	movs	r1, #128
	adds	r3, r5, #1
	lsls	r1, r1, #19
	strh	r3, [r6, #2]
	adds	r1, #82
	orrs	r3, r2
	strh	r3, [r1, #0]
.L_081a6e38:
	movs	r2, #4
	ldrsh	r3, [r6, r2]
	cmp	r3, #8
	bne.n	.L_081a6e46
	ldr	r5, [pc, #16]
	ldr	r0, [pc, #20]
	b.n	.L_081a6e68
.L_081a6e46:
	cmp	r3, #16
	bne.n	.L_081a6e60
	ldr	r5, [pc, #8]
	ldr	r0, [pc, #12]
	b.n	.L_081a6e68
	.4byte 0x00001000
	.4byte 0x02026800
	.4byte 0x0000001d
	.2byte 0x001e
	.2byte 0x0000
.L_081a6e60:
	cmp	r3, #24
	bne.n	.L_081a6e82
	ldr	r5, [pc, #40]
	ldr	r0, [pc, #44]
.L_081a6e68:
	bl	sub_08013300
	adds	r1, r5, #0
	bl	sub_0801591c
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #212
	adds	r0, r5, #0
	ldr	r1, [pc, #28]
	ldr	r2, [pc, #28]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
.L_081a6e82:
	add	sp, #28
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x02026800
	.4byte 0x0000001f
	.4byte 0x06010000
	.2byte 0x0c00
	.2byte 0x8400
.L_081a6ea0:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r1, #112
	movs	r0, #172
	sub	sp, #36
	bl	sub_08014cc0
	ldr	r3, [pc, #56]
	movs	r2, #128
	lsls	r2, r2, #19
	strh	r3, [r2, #0]
	movs	r3, #0
	str	r3, [sp, #0]
	movs	r3, #128
	mov	fp, sp
	lsls	r3, r3, #19
	movs	r1, #224
	mov	r9, r0
	adds	r3, #212
	mov	r0, fp
	lsls	r1, r1, #19
	ldr	r2, [pc, #32]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r0, [pc, #32]
	bl	sub_08013300
	ldr	r5, [pc, #28]
	adds	r6, r0, #0
	adds	r6, #32
	adds	r1, r5, #0
	adds	r0, r6, #0
	bl	sub_0801591c
	movs	r3, #128
	b.n	.L_081a6f04
	movs	r0, r0
	.4byte 0x00000000
	.4byte 0x85000100
	.4byte 0x0000001a
	.2byte 0x0000
	.2byte 0x0201
.L_081a6f04:
	lsls	r3, r3, #19
	adds	r3, #212
	adds	r0, r5, #0
	ldr	r1, [pc, #100]
	ldr	r2, [pc, #100]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r0, #1
	bl	sub_08013560
	ldr	r0, [pc, #92]
	bl	sub_08013300
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	movs	r1, #160
	lsls	r2, r2, #24
	adds	r6, r0, #0
	adds	r3, #212
	lsls	r1, r1, #19
	adds	r2, #128
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r3, [pc, #52]
	movs	r2, #160
	lsls	r2, r2, #19
	strh	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r6, r6, r0
	adds	r1, r5, #0
	adds	r0, r6, #0
	bl	sub_0801591c
	movs	r3, #128
	lsls	r3, r3, #19
	movs	r1, #192
	adds	r3, #212
	adds	r0, r5, #0
	lsls	r1, r1, #19
	ldr	r2, [pc, #28]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r0, #1
	bl	sub_08013560
	ldr	r7, [pc, #20]
	b.n	.L_081a6f84
	.4byte 0x00007fff
	.4byte 0x06010000
	.4byte 0x84001b00
	.4byte 0x00000019
	.4byte 0x84002580
	.2byte 0xf800
	.2byte 0x0600
.L_081a6f84:
	ldr	r1, [pc, #8]
	movs	r2, #0
	mov	r8, r2
	mov	sl, r2
.L_081a6f8c:
	movs	r6, #29
	b.n	.L_081a6f94
	.2byte 0x01ff
	.2byte 0x0000
.L_081a6f94:
	mov	r2, r8
	movs	r0, #128
	lsls	r3, r2, #16
	lsls	r0, r0, #9
	adds	r3, r3, r0
	asrs	r3, r3, #16
	subs	r6, #1
	strh	r2, [r7, #0]
	mov	r8, r3
	adds	r7, #2
	cmp	r6, #0
	bge.n	.L_081a6f94
	movs	r2, #1
	add	sl, r2
	strh	r1, [r7, #0]
	mov	r3, sl
	adds	r7, #2
	strh	r1, [r7, #0]
	adds	r7, #2
	cmp	r3, #19
	ble.n	.L_081a6f8c
	ldr	r3, [pc, #32]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #10
	strh	r3, [r2, #0]
	ldr	r2, [pc, #24]
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #80
	strh	r2, [r3, #0]
	adds	r3, #2
	strh	r2, [r3, #0]
	ldr	r3, [pc, #16]
	movs	r2, #0
	movs	r6, #3
	b.n	.L_081a6fec
	movs	r0, r0
	.4byte 0x00001f83
	.4byte 0x00000000
	.2byte 0x1120
	.2byte 0x0300
.L_081a6fec:
	subs	r6, #1
	strh	r2, [r3, #2]
	strh	r2, [r3, #0]
	adds	r3, #4
	cmp	r6, #0
	bge.n	.L_081a6fec
	movs	r2, #192
	lsls	r2, r2, #18
	adds	r2, #128
	ldr	r1, [r2, #0]
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	movs	r0, #160
	lsls	r2, r2, #24
	movs	r6, #4
	adds	r3, #212
	lsls	r0, r0, #19
	adds	r2, #128
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r0, #1
	bl	sub_08013560
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #0
	bl	sub_081a81d4
	ldr	r3, [pc, #60]
	movs	r2, #128
	lsls	r2, r2, #19
	strh	r3, [r2, #0]
	movs	r0, #10
	bl	sub_081a8228
	movs	r3, #176
	add	r5, sp, #4
	strh	r3, [r5, #2]
	ldr	r3, [pc, #44]
	movs	r2, #0
	str	r3, [r5, #8]
	ldr	r3, [pc, #40]
	mov	r0, r8
	str	r3, [r5, #12]
	movs	r3, #1
	strh	r2, [r5, #0]
	strh	r0, [r5, #20]
	strh	r2, [r5, #22]
	mov	r0, r9
	mov	r2, sl
	mov	r8, r3
	str	r3, [r5, #28]
	movs	r3, #10
	strh	r3, [r0, #10]
	strh	r2, [r5, #24]
	movs	r3, #25
	mov	r2, r9
	strh	r6, [r5, #4]
	b.n	.L_081a7070
	.4byte 0x00001240
	.4byte 0x02019600
	.2byte 0x9600
	.2byte 0x0600
.L_081a7070:
	str	r7, [r5, #16]
	strh	r3, [r2, #12]
	adds	r0, r5, #0
	bl	.L_081a68b4
	cmp	r0, #0
	bne.n	.L_081a715e
	movs	r3, #200
	strh	r3, [r5, #2]
	movs	r3, #3
	strh	r3, [r5, #4]
	adds	r0, r5, #0
	bl	.L_081a68b4
	cmp	r0, #0
	bne.n	.L_081a715e
	movs	r3, #216
	strh	r3, [r5, #2]
	movs	r3, #2
	strh	r3, [r5, #4]
	adds	r0, r5, #0
	bl	.L_081a68b4
	adds	r7, r0, #0
	cmp	r7, #0
	bne.n	.L_081a715e
	ldr	r0, [pc, #260]
	bl	sub_08013300
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	ldr	r1, [pc, #248]
	adds	r2, #8
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r0, #255
	lsls	r0, r0, #8
	adds	r0, #251
	mov	sl, r0
	mov	r3, r9
	mov	r2, sl
	strh	r2, [r3, #2]
	strh	r7, [r3, #0]
	movs	r3, #255
	lsls	r3, r3, #8
	mov	r0, r9
	adds	r3, #253
	mov	r2, r9
	strh	r7, [r0, #4]
	strh	r3, [r2, #6]
	mov	r3, r9
	strh	r7, [r3, #8]
	mov	r0, r8
	movs	r3, #224
	strh	r0, [r5, #4]
	strh	r3, [r5, #2]
	adds	r0, r5, #0
	bl	.L_081a68b4
	adds	r6, r0, #0
	cmp	r6, #0
	bne.n	.L_081a715e
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #188]
	bl	sub_080145a8
	movs	r3, #160
	strh	r3, [r5, #2]
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	strh	r3, [r5, #4]
	ldr	r3, [pc, #172]
	strh	r6, [r5, #22]
	str	r3, [r5, #8]
	ldr	r3, [pc, #172]
	adds	r0, r5, #0
	str	r3, [r5, #12]
	ldr	r3, [pc, #168]
	str	r3, [r5, #16]
	movs	r3, #225
	lsls	r3, r3, #1
	strh	r3, [r5, #20]
	movs	r3, #11
	strh	r3, [r5, #24]
	movs	r3, #8
	str	r3, [r5, #28]
	bl	.L_081a68b4
	adds	r6, r0, #0
	cmp	r6, #0
	bne.n	.L_081a715e
	movs	r0, #254
	lsls	r0, r0, #7
	adds	r0, #255
	movs	r1, #0
	bl	sub_081a81d4
	movs	r0, #20
	bl	sub_081a8228
	mov	r2, sl
	mov	r3, r8
	strh	r6, [r5, #2]
	strh	r2, [r5, #4]
	str	r3, [r5, #28]
	adds	r0, r5, #0
	bl	.L_081a68b4
	cmp	r0, #0
	bne.n	.L_081a7172
	movs	r0, #120
	bl	.L_081a6094
	b.n	.L_081a7172
.L_081a715e:
	movs	r0, #0
	movs	r1, #0
	bl	sub_081a81d4
	movs	r0, #10
	bl	sub_081a8228
	movs	r0, #10
	bl	sub_08013560
.L_081a7172:
	movs	r3, #0
	mov	r0, fp
	str	r3, [r0, #0]
	movs	r3, #128
	lsls	r3, r3, #19
	movs	r1, #224
	adds	r3, #212
	lsls	r1, r1, #19
	ldr	r2, [pc, #64]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r0, [pc, #40]
	bl	sub_08014644
	movs	r0, #172
	bl	sub_0801314c
	movs	r0, #1
	bl	sub_08013560
	movs	r0, #1
	add	sp, #36
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x0000001a
	.4byte 0x05000200
	.4byte 0x081a6b51
	.4byte 0x02017080
	.4byte 0x06007800
	.4byte 0x0600f9c2
	.2byte 0x0100
	.2byte 0x8500
	push	{r5, r6, lr}
	ldr	r5, [pc, #192]
	sub	sp, #4
	ldrb	r3, [r5, #0]
	mov	r6, sp
	adds	r6, #3
	strb	r3, [r6, #0]
	bl	sub_08014b70
	bl	sub_08014368
	movs	r0, #1
	bl	sub_08013560
	bl	sub_080144c0
	ldr	r2, [pc, #164]
	movs	r3, #0
	strb	r3, [r2, #0]
	strb	r3, [r5, #0]
	bl	.L_081a6030
	movs	r0, #0
	bl	.L_081a6154
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_081a722c
	bl	.L_081a6638
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_081a722c
	movs	r0, #1
	bl	.L_081a6154
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_081a722c
	bl	.L_081a6724
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_081a722c
	movs	r0, #2
	bl	.L_081a6154
	adds	r5, r0, #0
	cmp	r5, #0
	bne.n	.L_081a7244
.L_081a722c:
	movs	r0, #0
	movs	r1, #0
	bl	sub_081a81d4
	movs	r0, #10
	bl	sub_081a8228
	movs	r0, #10
	bl	sub_08013560
	cmp	r5, #0
	beq.n	.L_081a7248
.L_081a7244:
	bl	.L_081a6ea0
.L_081a7248:
	ldr	r2, [pc, #68]
	movs	r3, #1
	strb	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r3, [pc, #60]
	movs	r2, #0
	movs	r5, #3
.L_081a725a:
	subs	r5, #1
	strh	r2, [r3, #2]
	strh	r2, [r3, #0]
	adds	r3, #4
	cmp	r5, #0
	bge.n	.L_081a725a
	ldr	r3, [pc, #32]
	movs	r2, #128
	lsls	r2, r2, #19
	strh	r3, [r2, #0]
	bl	sub_08014bac
	bl	sub_08014b70
	bl	sub_081a814c
	ldr	r2, [pc, #16]
	ldrb	r3, [r6, #0]
	movs	r0, #1
	strb	r3, [r2, #0]
	add	sp, #4
	b.n	.L_081a7298
	movs	r0, r0
	.4byte 0x00000000
	.4byte 0x0300123c
	.4byte 0x0300120c
	.2byte 0x1120
	.2byte 0x0300
.L_081a7298:
	pop	{r5, r6, pc}
