.syntax unified
	.thumb
	.set sub_08013560, 0x08013560
	.set sub_08014d00, 0x08014d00
	.set sub_080200e8, 0x080200e8
	.set sub_08038130, 0x08038130
	.set sub_080ad0e8, 0x080ad0e8
	.set sub_0811a0b0, 0x0811a0b0
	.set sub_0811a24c, 0x0811a24c
	.set sub_0811a31c, 0x0811a31c
	.set sub_0811a44c, 0x0811a44c
	.set sub_0811a7ac, 0x0811a7ac
	.set sub_0811b2c4, 0x0811b2c4
	.set sub_0811bc64, 0x0811bc64
	.set sub_0811bc98, 0x0811bc98
	.set sub_0811be3c, 0x0811be3c
	.set sub_08126cfc, 0x08126cfc
	.set sub_08127adc, 0x08127adc
	.set sub_08138050, 0x08138050
	.global Func_081195d4
	.thumb_func
Func_081195d4:
	push	{lr}
	ldr	r3, [pc, #12]
	movs	r1, #16
	ldr	r0, [pc, #12]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0xbd00
	movs	r0, r0
	.4byte 0x03000258
	.2byte 0x3a74
	.2byte 0x0200
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
	sub	sp, #40
	movs	r0, #1
	movs	r1, #0
	str	r3, [sp, #8]
	bl	sub_0811a24c
	cmp	r0, #0
	beq.n	.L_081196ea
	mov	r1, sp
	adds	r1, #28
	movs	r0, #1
	str	r1, [sp, #4]
	bl	sub_0811a31c
	mov	r2, sp
	adds	r2, #16
	mov	r8, r0
	str	r0, [sp, #12]
	adds	r0, r2, #0
	str	r2, [sp, #0]
	bl	sub_0811a0b0
	cmp	r8, r0
	ble.n	.L_08119632
	mov	r8, r0
.L_08119632:
	ldr	r0, [sp, #4]
	movs	r1, #4
	bl	sub_08126cfc
	movs	r0, #32
	bl	sub_08013560
	mov	r3, r8
	cmp	r3, #0
	ble.n	.L_081196a6
	ldr	r3, [pc, #176]
	movs	r1, #134
	lsls	r1, r1, #2
	adds	r1, r1, r3
	movs	r2, #88
	movs	r3, #0
	mov	fp, r1
	mov	sl, r2
	mov	r9, r3
	mov	r7, r8
.L_0811965a:
	ldr	r3, [sp, #8]
	mov	r1, sl
	ldrsh	r0, [r1, r3]
	ldr	r2, [sp, #0]
	mov	r1, r9
	ldrh	r6, [r1, r2]
	bl	sub_0811be3c
	adds	r5, r0, #0
	adds	r0, r6, #0
	bl	sub_0811be3c
	ldr	r3, [r0, #0]
	ldr	r4, [r5, #0]
	mov	ip, r3
	ldr	r3, [r5, #12]
	subs	r7, #1
	str	r3, [r0, #12]
	ldr	r3, [r5, #16]
	str	r3, [r0, #16]
	mov	r0, ip
	ldr	r1, [r4, #8]
	ldr	r2, [r4, #12]
	ldr	r3, [r4, #16]
	bl	sub_080200e8
	mov	r1, fp
	strb	r6, [r1, #0]
	ldr	r1, [sp, #8]
	movs	r2, #1
	mov	r3, sl
	add	fp, r2
	movs	r2, #2
	strh	r6, [r3, r1]
	add	sl, r2
	add	r9, r2
	cmp	r7, #0
	bne.n	.L_0811965a
.L_081196a6:
	mov	r3, r8
	cmp	r3, #0
	ble.n	.L_081196ca
	ldr	r1, [sp, #12]
	ldr	r3, [pc, #72]
	ldr	r0, [sp, #4]
	adds	r3, r1, r3
	movs	r1, #134
	lsls	r1, r1, #2
	adds	r2, r3, r1
	mov	r7, r8
.L_081196bc:
	ldrh	r3, [r0, #0]
	subs	r7, #1
	strb	r3, [r2, #0]
	adds	r0, #2
	adds	r2, #1
	cmp	r7, #0
	bne.n	.L_081196bc
.L_081196ca:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #36]
	adds	r3, #65
	ldrb	r0, [r3, #0]
	bl	sub_08038130
	bl	sub_0811a44c
	movs	r1, #6
	ldr	r0, [sp, #0]
	bl	sub_08126cfc
	movs	r0, #32
	bl	sub_08013560
.L_081196ea:
	add	sp, #40
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	movs	r1, #76
	movs	r0, #48
	bl	sub_08014d00
	movs	r1, #128
	adds	r3, r0, #0
	adds	r3, #12
	movs	r2, #0
	lsls	r1, r1, #15
	str	r2, [r3, #0]
	str	r1, [r3, #4]
	str	r2, [r3, #8]
	movs	r3, #180
	lsls	r3, r3, #16
	str	r3, [r0, #4]
	movs	r3, #160
	lsls	r3, r3, #6
	strh	r3, [r0, #54]
	movs	r3, #160
	lsls	r3, r3, #7
	strh	r3, [r0, #52]
	movs	r3, #128
	lsls	r3, r3, #17
	str	r2, [r0, #0]
	str	r1, [r0, #8]
	str	r3, [r0, #32]
	pop	{pc}
	push	{r5, r6, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	movs	r5, #118
	ldr	r6, [r3, #36]
	adds	r5, #255
	cmp	r0, #78
	beq.n	.L_08119748
	movs	r5, #188
	lsls	r5, r5, #1
.L_08119748:
	movs	r0, #128
	bl	sub_0811bc64
	bl	sub_0811bc98
	movs	r1, #1
	adds	r0, r5, #0
	bl	sub_08127adc
	adds	r1, r5, #0
	movs	r2, #255
	movs	r0, #128
	bl	sub_080ad0e8
	adds	r2, r6, #0
	adds	r2, #102
	movs	r3, #128
	strh	r3, [r2, #0]
	adds	r2, #2
	movs	r3, #255
	strh	r3, [r2, #0]
	bl	sub_0811b2c4
	movs	r0, #128
	bl	sub_0811be3c
	ldr	r2, [r0, #12]
	cmp	r2, #0
	bge.n	.L_0811978a
	movs	r1, #255
	lsls	r1, r1, #8
	adds	r1, #255
	adds	r2, r2, r1
.L_0811978a:
	ldr	r3, [r0, #16]
	asrs	r2, r2, #16
	cmp	r3, #0
	bge.n	.L_0811979a
	movs	r1, #255
	lsls	r1, r1, #8
	adds	r1, #255
	adds	r3, r3, r1
.L_0811979a:
	asrs	r3, r3, #16
	movs	r1, #128
	bl	sub_0811a7ac
	bl	sub_0811a44c
	pop	{r5, r6, pc}
	push	{lr}
	sub	sp, #88
	movs	r2, #0
	movs	r4, #128
	adds	r1, r0, #0
	movs	r3, #1
	mov	r0, sp
	str	r2, [r0, #16]
	str	r4, [r0, #8]
	str	r2, [r0, #12]
	str	r3, [r0, #20]
	strh	r4, [r0, #36]
	str	r2, [r0, #4]
	str	r2, [r0, #0]
	str	r2, [r0, #24]
	bl	sub_08138050
	add	sp, #88
	pop	{pc}
	.align 2, 0
