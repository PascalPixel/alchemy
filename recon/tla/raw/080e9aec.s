.syntax unified
	.thumb
	.set sub_0801314c, 0x0801314c
	.set sub_08013300, 0x08013300
	.set sub_08013560, 0x08013560
	.set sub_08014274, 0x08014274
	.set sub_080142d4, 0x080142d4
	.set sub_080143ac, 0x080143ac
	.set sub_080145a8, 0x080145a8
	.set sub_08014644, 0x08014644
	.set sub_08014cc0, 0x08014cc0
	.set sub_0801587c, 0x0801587c
	.set sub_08020090, 0x08020090
	.set sub_080200e8, 0x080200e8
	.set sub_080cdf5c, 0x080cdf5c
	.set sub_080ce31c, 0x080ce31c
	.set sub_080ce458, 0x080ce458
	.set sub_080ceafc, 0x080ceafc
	.set sub_080d2d84, 0x080d2d84
	.set sub_080dbcc0, 0x080dbcc0
	.set sub_080dbe80, 0x080dbe80
	.set sub_080e1420, 0x080e1420
	.set sub_080eaf98, 0x080eaf98
	.set sub_081c0010, 0x081c0010
	.global Func_080e9aec
	.thumb_func
Func_080e9aec:
	push	{lr}
	bl	.L_080e9af4
	pop	{pc}
.L_080e9af4:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r1, #128
	lsls	r1, r1, #5
	adds	r1, #56
	movs	r0, #92
	sub	sp, #24
	bl	sub_08014cc0
	movs	r2, #192
	lsls	r2, r2, #18
	adds	r3, r2, #0
	adds	r3, #224
	ldr	r3, [r3, #0]
	ldr	r2, [r2, #108]
	mov	r8, r0
	movs	r1, #30
	ldrsh	r0, [r3, r1]
	ldr	r7, [r3, #16]
	mov	sl, r3
	mov	fp, r2
	bl	sub_080ce31c
	adds	r2, r0, #0
	movs	r0, #160
	mov	r4, sl
	lsls	r0, r0, #23
	movs	r3, #30
	ldrsh	r1, [r4, r3]
	adds	r0, #5
	bl	sub_080ce458
	str	r0, [sp, #8]
	ldr	r0, [pc, #612]
	bl	sub_08013300
	mov	r1, r8
	bl	sub_0801587c
	bl	sub_080143ac
	movs	r1, #128
	lsls	r1, r1, #4
	mov	r2, r8
	adds	r5, r0, #0
	bl	sub_080142d4
	movs	r3, #128
	lsls	r3, r3, #5
	adds	r3, #44
	add	r3, r8
	strh	r5, [r3, #0]
	movs	r2, #128
	movs	r3, #128
	lsls	r2, r2, #5
	lsls	r3, r3, #5
	adds	r2, #46
	adds	r3, #40
	movs	r6, #0
	add	r2, r8
	add	r3, r8
	strh	r0, [r2, #0]
	str	r6, [r3, #0]
	movs	r5, #128
	movs	r0, #0
	ldrsh	r2, [r2, r0]
	lsls	r5, r5, #5
	add	r5, r8
	movs	r3, #128
	str	r2, [sp, #0]
	adds	r0, r5, #0
	lsls	r3, r3, #24
	movs	r1, #15
	movs	r2, #15
	bl	sub_080eaf98
	ldrb	r3, [r5, #5]
	movs	r2, #32
	orrs	r3, r2
	ldrb	r2, [r5, #9]
	strb	r3, [r5, #5]
	movs	r3, #15
	ands	r3, r2
	strb	r3, [r5, #9]
	ldr	r2, [r7, #12]
	ldr	r3, [r7, #16]
	ldr	r1, [r7, #8]
	adds	r0, r7, #0
	bl	sub_080200e8
	adds	r0, r7, #0
	movs	r1, #0
	bl	sub_080e1420
	movs	r1, #5
	adds	r0, r7, #0
	bl	sub_08020090
	movs	r0, #130
	bl	sub_081c0010
	movs	r0, #1
	bl	sub_08013560
	movs	r3, #128
	lsls	r3, r3, #5
	adds	r3, #48
	add	r3, r8
	strh	r6, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #5
	adds	r3, #50
	add	r3, r8
	strh	r6, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #5
	adds	r3, #52
	add	r3, r8
	movs	r1, #144
	strh	r6, [r3, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #440]
	bl	sub_080145a8
	adds	r3, r7, #0
	movs	r1, #0
	adds	r3, #34
	mov	r9, r1
	ldr	r0, [r7, #8]
	ldr	r1, [r7, #16]
	ldrb	r2, [r3, #0]
	bl	sub_080dbcc0
	mov	r6, fp
	movs	r4, #0
	add	r5, sp, #12
	adds	r6, #20
	cmp	r0, #0
	bne.n	.L_080e9c20
	movs	r2, #1
	mov	r9, r2
	b.n	.L_080e9c6e
.L_080e9c1a:
	movs	r3, #1
	mov	r9, r3
	b.n	.L_080e9c6e
.L_080e9c20:
	ldmia	r6!, {r0}
	cmp	r0, #0
	beq.n	.L_080e9c68
	ldr	r3, [r0, #0]
	cmp	r3, #0
	beq.n	.L_080e9c68
	adds	r3, r0, #0
	adds	r3, #89
	ldrb	r2, [r3, #0]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080e9c68
	mov	r1, sl
	ldr	r3, [r1, #16]
	cmp	r0, r3
	beq.n	.L_080e9c68
	ldr	r3, [r0, #8]
	adds	r2, r0, #0
	str	r3, [r5, #0]
	adds	r2, #8
	ldr	r3, [r0, #20]
	str	r4, [sp, #4]
	str	r3, [r5, #4]
	ldr	r3, [r0, #16]
	str	r3, [r5, #8]
	ldrh	r3, [r0, #32]
	adds	r0, r7, #0
	ldrh	r1, [r7, #32]
	subs	r3, #2
	adds	r0, #8
	bl	sub_080dbe80
	ldr	r4, [sp, #4]
	cmp	r0, #0
	bge.n	.L_080e9c1a
.L_080e9c68:
	adds	r4, #1
	cmp	r4, #79
	ble.n	.L_080e9c20
.L_080e9c6e:
	mov	r2, r9
	cmp	r2, #0
	beq.n	.L_080e9cf4
	movs	r0, #5
	bl	sub_08013560
	movs	r0, #246
	bl	sub_081c0010
	movs	r6, #128
	lsls	r6, r6, #5
	adds	r6, #40
	movs	r5, #0
	add	r6, r8
.L_080e9c8a:
	lsls	r3, r5, #11
	str	r3, [r6, #0]
	movs	r0, #1
	adds	r5, #1
	bl	sub_08013560
	cmp	r5, #31
	ble.n	.L_080e9c8a
	movs	r0, #10
	bl	sub_08013560
	ldr	r3, [sp, #8]
	cmp	r3, #0
	beq.n	.L_080e9cb6
	mov	r0, sl
	movs	r4, #24
	ldrsh	r1, [r0, r4]
	movs	r3, #26
	ldrsh	r2, [r0, r3]
	ldr	r0, [sp, #8]
	bl	sub_080ceafc
.L_080e9cb6:
	movs	r6, #128
	lsls	r6, r6, #5
	adds	r6, #40
	movs	r5, #31
	add	r6, r8
.L_080e9cc0:
	lsls	r3, r5, #11
	str	r3, [r6, #0]
	movs	r0, #1
	subs	r5, #1
	bl	sub_08013560
	cmp	r5, #0
	bne.n	.L_080e9cc0
	movs	r0, #5
	bl	sub_08013560
	ldr	r0, [pc, #212]
	bl	sub_08014644
	movs	r3, #128
	lsls	r3, r3, #5
	adds	r3, #44
	add	r3, r8
	movs	r4, #0
	ldrsh	r0, [r3, r4]
	bl	sub_08014274
	movs	r0, #92
	bl	sub_0801314c
	b.n	.L_080e9d9a
.L_080e9cf4:
	movs	r0, #85
	adds	r0, r0, r7
	mov	r1, r9
	strb	r1, [r0, #0]
	ldr	r3, [r7, #20]
	mov	r2, r9
	str	r3, [r7, #12]
	movs	r3, #128
	lsls	r3, r3, #5
	adds	r3, #40
	add	r3, r8
	str	r2, [r3, #0]
	movs	r3, #208
	lsls	r3, r3, #4
	movs	r2, #208
	adds	r3, #70
	lsls	r2, r2, #4
	add	r3, fp
	mov	r4, r9
	adds	r2, #68
	strh	r4, [r3, #0]
	add	r2, fp
	movs	r3, #100
	movs	r6, #128
	lsls	r6, r6, #5
	strh	r3, [r2, #0]
	adds	r6, #40
	mov	sl, r0
	movs	r5, #0
	add	r6, r8
.L_080e9d30:
	cmp	r5, #10
	bne.n	.L_080e9d3a
	movs	r0, #246
	bl	sub_081c0010
.L_080e9d3a:
	ldr	r3, [r7, #12]
	movs	r0, #128
	lsls	r0, r0, #9
	adds	r3, r3, r0
	str	r3, [r7, #12]
	lsls	r3, r5, #11
	str	r3, [r6, #0]
	movs	r0, #1
	adds	r5, #1
	bl	sub_08013560
	cmp	r5, #31
	ble.n	.L_080e9d30
	movs	r3, #4
	mov	r1, sl
	strb	r3, [r1, #0]
	ldr	r3, [r7, #20]
	movs	r2, #146
	lsls	r2, r2, #14
	adds	r3, r3, r2
	str	r3, [r7, #12]
	movs	r3, #192
	lsls	r3, r3, #10
	str	r3, [r7, #72]
	movs	r4, #128
	ldr	r3, [pc, #64]
	lsls	r4, r4, #2
	adds	r4, #18
	adds	r3, r3, r4
	movs	r2, #10
	strb	r2, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #5
	adds	r3, #52
	movs	r2, #1
	add	r3, r8
	strh	r2, [r3, #0]
	movs	r3, #208
	lsls	r3, r3, #4
	adds	r3, #70
	add	r3, fp
	strh	r2, [r3, #0]
	movs	r2, #208
	lsls	r2, r2, #4
	adds	r2, #68
	add	r2, fp
	movs	r3, #112
	strh	r3, [r2, #0]
.L_080e9d9a:
	add	sp, #24
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x000001da
	.4byte 0x080e9941
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r7, [r3, #92]
	bl	sub_080cdf5c
	bl	sub_080d2d84
	adds	r5, r0, #0
	ldr	r0, [pc, #188]
	bl	sub_08014644
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #12]
	ldr	r3, [r5, #16]
	adds	r0, r5, #0
	bl	sub_080200e8
	adds	r1, r5, #0
	adds	r1, #85
	movs	r3, #3
	strb	r3, [r1, #0]
	ldr	r3, [pc, #164]
	movs	r1, #128
	lsls	r1, r1, #2
	adds	r1, #18
	movs	r2, #0
	adds	r3, r3, r1
	strb	r2, [r3, #0]
	movs	r6, #0
	b.n	.L_080e9dfe
.L_080e9df2:
	movs	r0, #1
	adds	r6, #1
	bl	sub_08013560
	cmp	r6, #59
	bgt.n	.L_080e9e06
.L_080e9dfe:
	ldr	r2, [r5, #12]
	ldr	r3, [r5, #20]
	cmp	r2, r3
	bgt.n	.L_080e9df2
.L_080e9e06:
	movs	r0, #132
	bl	sub_081c0010
	movs	r0, #1
	bl	sub_08013560
	ldr	r2, [r5, #12]
	ldr	r3, [r5, #20]
	movs	r6, #0
	cmp	r2, r3
	ble.n	.L_080e9e30
.L_080e9e1c:
	movs	r0, #1
	adds	r6, #1
	bl	sub_08013560
	cmp	r6, #19
	bgt.n	.L_080e9e30
	ldr	r2, [r5, #12]
	ldr	r3, [r5, #20]
	cmp	r2, r3
	bgt.n	.L_080e9e1c
.L_080e9e30:
	movs	r0, #1
	bl	sub_08013560
	ldr	r2, [r5, #12]
	ldr	r3, [r5, #20]
	movs	r6, #0
	cmp	r2, r3
	ble.n	.L_080e9e54
.L_080e9e40:
	movs	r0, #1
	adds	r6, #1
	bl	sub_08013560
	cmp	r6, #19
	bgt.n	.L_080e9e54
	ldr	r2, [r5, #12]
	ldr	r3, [r5, #20]
	cmp	r2, r3
	bgt.n	.L_080e9e40
.L_080e9e54:
	movs	r0, #1
	bl	sub_08013560
	movs	r3, #0
	str	r3, [r5, #68]
	str	r3, [r5, #40]
	ldr	r3, [r5, #20]
	movs	r2, #128
	str	r3, [r5, #12]
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r5, #72]
	lsls	r2, r2, #5
	adds	r2, #44
	adds	r3, r7, r2
	movs	r1, #0
	ldrsh	r0, [r3, r1]
	bl	sub_08014274
	movs	r0, #92
	bl	sub_0801314c
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x080e9941
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	ldr	r1, [r0, #80]
	adds	r0, #100
	ldrh	r3, [r0, #0]
	movs	r2, #3
	ands	r2, r3
	ldr	r4, [r1, #40]
	cmp	r2, #1
	beq.n	.L_080e9ec0
	cmp	r2, #1
	bgt.n	.L_080e9ea8
	cmp	r2, #0
	beq.n	.L_080e9eb2
	b.n	.L_080e9eda
.L_080e9ea8:
	cmp	r2, #2
	beq.n	.L_080e9ec4
	cmp	r2, #3
	beq.n	.L_080e9ed2
	b.n	.L_080e9eda
.L_080e9eb2:
	movs	r3, #7
	strb	r3, [r4, #5]
	movs	r3, #1
	strb	r3, [r1, #25]
	movs	r3, #2
	strb	r3, [r1, #26]
	b.n	.L_080e9eda
.L_080e9ec0:
	movs	r3, #0
	b.n	.L_080e9ecc
.L_080e9ec4:
	movs	r2, #7
	movs	r3, #0
	strb	r2, [r4, #5]
	movs	r2, #1
.L_080e9ecc:
	strb	r2, [r1, #25]
	strb	r3, [r1, #26]
	b.n	.L_080e9eda
.L_080e9ed2:
	movs	r3, #0
	strb	r3, [r4, #5]
	movs	r3, #1
	strb	r3, [r1, #25]
.L_080e9eda:
	ldrh	r3, [r0, #0]
	adds	r3, #1
	strh	r3, [r0, #0]
	pop	{pc}
	.align 2, 0
