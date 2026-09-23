.syntax unified
	.thumb
	.set sub_08013164, 0x08013164
	.set sub_08014274, 0x08014274
	.set sub_080142d4, 0x080142d4
	.set sub_080143ac, 0x080143ac
	.set sub_08014dac, 0x08014dac
	.set sub_08016ce4, 0x08016ce4
	.set sub_08020048, 0x08020048
	.set sub_08038010, 0x08038010
	.set sub_08038018, 0x08038018
	.set sub_08038398, 0x08038398
	.set sub_080cdf5c, 0x080cdf5c
	.set sub_080d2d84, 0x080d2d84
	.set sub_080ec1d0, 0x080ec1d0
	.global Func_080ebf68
	.thumb_func
Func_080ebf68:
	push	{r5, lr}
	adds	r5, r0, #0
	ldr	r0, [r5, #0]
	sub	sp, #4
	cmp	r0, #0
	beq.n	.L_080ebf78
	bl	sub_08020048
.L_080ebf78:
	mov	r0, sp
	movs	r3, #0
	str	r3, [r0, #0]
	movs	r2, #133
	movs	r3, #128
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	adds	r1, r5, #0
	adds	r2, #18
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	add	sp, #4
	pop	{r5, pc}
	push	{r5, r6, r7, lr}
	movs	r0, #32
	sub	sp, #20
	bl	sub_08014dac
	ldr	r7, [pc, #392]
	adds	r6, r0, #0
	adds	r1, r7, #0
	adds	r1, #32
	str	r1, [sp, #4]
	bl	sub_080cdf5c
	bl	sub_080d2d84
	movs	r5, #0
	str	r5, [sp, #12]
	str	r5, [sp, #8]
	bl	sub_080143ac
	movs	r2, #192
	lsls	r2, r2, #3
	adds	r2, #68
	movs	r4, #201
	adds	r3, r7, r2
	lsls	r4, r4, #3
	strh	r0, [r7, #0]
	str	r5, [r3, #0]
	adds	r3, r7, r4
	str	r5, [r3, #0]
	movs	r2, #133
	movs	r3, #128
	add	r0, sp, #16
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	str	r5, [r0, #0]
	adds	r3, #212
	adds	r1, r6, #0
	adds	r2, #32
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r3, #255
	str	r3, [r6, #0]
	movs	r3, #128
	lsls	r3, r3, #1
	adds	r3, #255
	str	r3, [r6, #4]
	movs	r3, #68
	str	r3, [r6, #32]
	movs	r3, #162
	lsls	r3, r3, #1
	str	r3, [r6, #36]
	movs	r3, #119
	str	r3, [r6, #64]
	movs	r3, #120
	adds	r3, #255
	str	r3, [r6, #68]
	movs	r3, #255
	lsls	r3, r3, #4
	str	r3, [r6, #96]
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	str	r3, [r6, #100]
	ldr	r3, [pc, #280]
	movs	r2, #136
	str	r3, [r6, #104]
	ldr	r3, [pc, #276]
	lsls	r2, r2, #1
	str	r3, [r6, #108]
	movs	r3, #136
	lsls	r3, r3, #5
	str	r2, [r6, #8]
	str	r2, [r6, #40]
	str	r2, [r6, #72]
	str	r3, [r6, #112]
	movs	r1, #128
	adds	r2, r6, #0
	ldrh	r0, [r7, #0]
	bl	sub_080142d4
	movs	r3, #128
	lsls	r3, r3, #3
	orrs	r0, r3
	ldr	r3, [sp, #4]
	movs	r2, #0
	movs	r1, #0
.L_080ec040:
	str	r1, [r3, #0]
	str	r1, [r3, #4]
	str	r0, [r3, #8]
	ldr	r4, [sp, #4]
	adds	r2, #1
	adds	r4, #12
	adds	r3, #12
	str	r4, [sp, #4]
	cmp	r2, #127
	bls.n	.L_080ec040
	adds	r0, r6, #0
	bl	sub_08013164
	movs	r0, #142
	lsls	r0, r0, #1
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_080ec072
	movs	r3, #132
	lsls	r3, r3, #17
	str	r3, [r7, #4]
	movs	r3, #128
	lsls	r3, r3, #13
	b.n	.L_080ec084
.L_080ec072:
	add	r0, sp, #12
	add	r1, sp, #8
	bl	sub_080ec1d0
	ldr	r3, [sp, #12]
	lsls	r3, r3, #16
	str	r3, [r7, #4]
	ldr	r3, [sp, #8]
	lsls	r3, r3, #16
.L_080ec084:
	str	r3, [r7, #8]
	ldr	r2, [r7, #4]
	ldr	r1, [pc, #168]
	ldr	r4, [pc, #172]
	adds	r2, r2, r1
	str	r2, [sp, #12]
	ldr	r3, [r7, #8]
	movs	r1, #136
	adds	r3, r3, r4
	movs	r0, #224
	str	r3, [sp, #8]
	movs	r6, #0
	lsls	r1, r1, #17
	lsls	r0, r0, #16
	cmp	r2, #0
	bge.n	.L_080ec0a8
	str	r6, [sp, #12]
	movs	r2, #0
.L_080ec0a8:
	cmp	r2, r1
	ble.n	.L_080ec0ae
	str	r1, [sp, #12]
.L_080ec0ae:
	cmp	r3, #0
	bge.n	.L_080ec0b6
	str	r6, [sp, #8]
	movs	r3, #0
.L_080ec0b6:
	cmp	r3, r0
	ble.n	.L_080ec0bc
	str	r0, [sp, #8]
.L_080ec0bc:
	ldr	r3, [r7, #4]
	ldr	r2, [pc, #124]
	str	r3, [r2, #0]
	ldr	r3, [r7, #8]
	adds	r2, #4
	str	r3, [r2, #0]
	bl	sub_08038398
	strh	r0, [r7, #2]
	ldr	r2, [pc, #112]
	ldrh	r3, [r7, #2]
	movs	r1, #0
	lsls	r3, r3, #2
	adds	r3, r3, r2
	ldrh	r5, [r3, #2]
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r2, #0
	movs	r3, #0
	movs	r0, #0
	bl	sub_08038010
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	strh	r3, [r7, #18]
	adds	r3, #1
	str	r3, [r7, #24]
	str	r0, [r7, #28]
	ldr	r2, [sp, #4]
	movs	r3, #128
	stmia	r2!, {r6}
	lsls	r3, r3, #23
	adds	r1, r2, #0
	str	r1, [sp, #4]
	stmia	r2!, {r3}
	movs	r3, #128
	lsrs	r5, r5, #5
	lsls	r3, r3, #3
	adds	r4, r2, #0
	orrs	r3, r5
	str	r4, [sp, #4]
	str	r3, [r2, #0]
	ldr	r2, [pc, #48]
	ldr	r3, [pc, #48]
	str	r3, [r2, #0]
	ldr	r3, [sp, #12]
	adds	r2, #4
	str	r3, [r2, #0]
	ldr	r3, [sp, #8]
	adds	r2, #4
	str	r3, [r2, #0]
	add	sp, #20
	pop	{r5, r6, r7, pc}
	.4byte 0x0202a000
	.4byte 0x0001ffff
	.4byte 0x00011ff0
	.4byte 0xff880000
	.4byte 0xffb00000
	.4byte 0x0202a64c
	.4byte 0x020036e0
	.4byte 0x0202a62c
	.2byte 0xa004
	.2byte 0x0202
	push	{r5, lr}
	ldr	r5, [pc, #24]
	ldrh	r0, [r5, #0]
	bl	sub_08014274
	ldrh	r0, [r5, #2]
	bl	sub_08014274
	ldr	r0, [r5, #28]
	movs	r1, #2
	bl	sub_08038018
	pop	{r5, pc}
	movs	r0, r0
	.4byte 0x0202a000
