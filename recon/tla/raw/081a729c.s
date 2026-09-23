.syntax unified
	.thumb
	.set sub_0800206c, 0x0800206c
	.set sub_0801314c, 0x0801314c
	.set sub_08013300, 0x08013300
	.set sub_08013560, 0x08013560
	.set sub_08013ef8, 0x08013ef8
	.set sub_08013f3c, 0x08013f3c
	.set sub_08013fdc, 0x08013fdc
	.set sub_080140d8, 0x080140d8
	.set sub_080142d4, 0x080142d4
	.set sub_08014368, 0x08014368
	.set sub_080143ac, 0x080143ac
	.set sub_080144c0, 0x080144c0
	.set sub_08014878, 0x08014878
	.set sub_08014b70, 0x08014b70
	.set sub_08014bac, 0x08014bac
	.set sub_08014d00, 0x08014d00
	.set sub_0801587c, 0x0801587c
	.set sub_0801591c, 0x0801591c
	.global Func_081a729c
	.thumb_func
Func_081a729c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	mov	r9, r0
	movs	r0, #128
	lsls	r0, r0, #1
	sub	sp, #8
	adds	r0, #255
	movs	r1, #0
	str	r1, [sp, #4]
	mov	r8, r0
	movs	r1, #112
	movs	r0, #172
	bl	sub_08014d00
	mov	fp, r0
	bl	sub_080144c0
	movs	r0, #1
	bl	sub_08013ef8
	bl	sub_08014b70
	bl	sub_08014368
	movs	r0, #1
	bl	sub_08013560
	ldr	r3, [pc, #48]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #12
	strh	r3, [r2, #0]
	ldr	r3, [pc, #44]
	subs	r2, #12
	strh	r3, [r2, #0]
	add	r2, sp, #4
	ldr	r3, [pc, #40]
	ldrh	r2, [r2, #0]
	ldr	r0, [pc, #40]
	strh	r2, [r3, #10]
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
	b.n	.L_081a7320
	movs	r0, r0
	.4byte 0x00000681
	.4byte 0x00001440
	.4byte 0x03001120
	.2byte 0x0016
	.2byte 0x0000
.L_081a7320:
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r5, [pc, #252]
	movs	r3, #224
	lsls	r3, r3, #1
	adds	r4, r4, r3
	adds	r1, r5, #0
	adds	r0, r4, #0
	bl	sub_0801587c
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #212
	adds	r0, r5, #0
	ldr	r1, [pc, #232]
	ldr	r2, [pc, #236]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r1, [pc, #232]
	movs	r3, #208
	lsls	r3, r3, #1
	movs	r7, #0
.L_081a734c:
	movs	r6, #0
.L_081a734e:
	adds	r2, r3, #0
	movs	r0, #128
	lsls	r3, r2, #16
	lsls	r0, r0, #9
	adds	r3, r3, r0
	adds	r6, #1
	strh	r2, [r1, #0]
	asrs	r3, r3, #16
	adds	r1, #2
	cmp	r6, #29
	bls.n	.L_081a734e
	mov	r2, r8
	strh	r2, [r1, #0]
	mov	r0, r8
	adds	r1, #2
	adds	r7, #1
	strh	r0, [r1, #0]
	adds	r1, #2
	cmp	r7, #19
	bls.n	.L_081a734c
	ldr	r3, [pc, #188]
	movs	r7, #0
	movs	r2, #0
.L_081a737c:
	adds	r7, #1
	strh	r2, [r3, #2]
	strh	r2, [r3, #0]
	adds	r3, #4
	cmp	r7, #3
	bls.n	.L_081a737c
	movs	r3, #128
	movs	r1, #128
	movs	r2, #132
	lsls	r3, r3, #19
	lsls	r1, r1, #19
	lsls	r2, r2, #24
	adds	r3, #212
	ldr	r0, [pc, #156]
	adds	r1, #16
	adds	r2, #4
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	bl	sub_08014bac
	bl	sub_08014b70
	mov	r1, r9
	cmp	r1, #0
	beq.n	.L_081a73f2
	movs	r1, #128
	lsls	r1, r1, #3
	movs	r0, #56
	bl	sub_08014d00
	adds	r6, r0, #0
	adds	r1, r6, #0
	ldr	r0, [pc, #120]
	bl	sub_0801591c
	mov	r5, fp
	adds	r5, #16
	movs	r7, #0
.L_081a73c8:
	bl	sub_080143ac
	lsls	r2, r7, #8
	lsrs	r2, r2, #1
	adds	r2, r6, r2
	movs	r1, #128
	bl	sub_080142d4
	adds	r2, r5, #0
	movs	r3, #0
	stmia	r2!, {r3}
	ldr	r3, [pc, #92]
	adds	r7, #1
	stmia	r2!, {r3}
	adds	r5, #12
	str	r0, [r2, #0]
	cmp	r7, #4
	bls.n	.L_081a73c8
	movs	r0, #56
	bl	sub_0801314c
.L_081a73f2:
	movs	r0, #30
	bl	sub_08013f3c
	bl	sub_08013fdc
	ldr	r3, [pc, #32]
	movs	r2, #128
	lsls	r2, r2, #19
	strh	r3, [r2, #0]
	movs	r2, #165
	lsls	r2, r2, #2
	mov	r3, r9
	mov	sl, r2
	cmp	r3, #0
	beq.n	.L_081a7440
	movs	r0, #168
	lsls	r0, r0, #5
	adds	r0, #24
	mov	sl, r0
	b.n	.L_081a7440
.L_081a741a:
	movs	r1, #1
	str	r1, [sp, #4]
	b.n	.L_081a74e4
	.4byte 0x00001540
	.4byte 0x02010000
	.4byte 0x06006800
	.4byte 0x84002580
	.4byte 0x06003000
	.4byte 0x03001120
	.4byte 0x081a8288
	.2byte 0x4000
	.2byte 0x4000
.L_081a7440:
	movs	r7, #0
	cmp	r7, sl
	bcs.n	.L_081a74e4
.L_081a7446:
	cmp	r7, #120
	bls.n	.L_081a74d4
	mov	r2, r9
	cmp	r2, #0
	beq.n	.L_081a74bc
	ldr	r3, [pc, #68]
	mov	r5, fp
	adds	r5, #16
	movs	r6, #0
	movs	r4, #80
	mov	r8, r3
.L_081a745c:
	ldr	r3, [pc, #52]
	adds	r2, r4, #0
	ands	r2, r3
	ldrh	r3, [r5, #6]
	mov	r0, r8
	ands	r3, r0
	orrs	r3, r2
	strh	r3, [r5, #6]
	movs	r3, #124
	strb	r3, [r5, #4]
	adds	r0, r5, #0
	movs	r1, #0
	str	r4, [sp, #0]
	bl	sub_080140d8
	ldr	r4, [sp, #0]
	adds	r6, #1
	adds	r4, #32
	adds	r5, #12
	cmp	r6, #2
	bls.n	.L_081a745c
	movs	r1, #60
	adds	r0, r7, #0
	bl	sub_0800206c
	ldr	r2, [pc, #12]
	b.n	.L_081a74a0
	movs	r0, r0
	.4byte 0x000001ff
	.4byte 0xfffffe00
	.2byte 0x873e
	.2byte 0x081a
.L_081a74a0:
	ldr	r3, [pc, #40]
	ldrb	r1, [r2, r0]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #80
	strh	r3, [r2, #0]
	movs	r3, #16
	subs	r3, r3, r1
	movs	r0, #128
	lsls	r3, r3, #8
	lsls	r0, r0, #19
	adds	r3, r3, r1
	adds	r0, #82
	strh	r3, [r0, #0]
.L_081a74bc:
	ldr	r3, [pc, #16]
	movs	r2, #9
	ldr	r3, [r3, #4]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_081a741a
	b.n	.L_081a74d4
	movs	r0, r0
	.4byte 0x00002f50
	.2byte 0x1150
	.2byte 0x0300
.L_081a74d4:
	bl	sub_08014878
	adds	r7, #1
	movs	r0, #1
	bl	sub_08013560
	cmp	r7, sl
	bcc.n	.L_081a7446
.L_081a74e4:
	movs	r0, #172
	bl	sub_0801314c
	ldr	r2, [pc, #24]
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #80
	strh	r2, [r3, #0]
	adds	r3, #2
	strh	r2, [r3, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r0, [sp, #4]
	add	sp, #8
	b.n	.L_081a7508
	.2byte 0x0000
	.2byte 0x0000
.L_081a7508:
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
