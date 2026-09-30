.syntax unified
	.thumb
	.global Func_080ebf68
	.thumb_func
Func_080ebf68:
	push	{r5, lr}
	adds	r5, r0, #0
	ldr	r0, [r5, #0]
	sub	sp, #4
	cmp	r0, #0
	beq.n	.L_080ebf78
	bl	0x08020048
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
	bl	Func_08014dac
	ldr	r7, [pc, #392]
	adds	r6, r0, #0
	adds	r1, r7, #0
	adds	r1, #32
	str	r1, [sp, #4]
	bl	Func_080cdf5c
	bl	Object_GetById
	movs	r5, #0
	str	r5, [sp, #12]
	str	r5, [sp, #8]
	bl	Resource_FindFreeEntry
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
	bl	VramBlock_LoadCached
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
	bl	Func_08013164
	movs	r0, #142
	lsls	r0, r0, #1
	bl	GameFlag_Test
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
	bl	Func_080ec1d0
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
	bl	0x08038398
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
	bl	UiWindow_CreateFar
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
	bl	Func_08014274
	ldrh	r0, [r5, #2]
	bl	Func_08014274
	ldr	r0, [r5, #28]
	movs	r1, #2
	bl	UiWork_FinalizeFar
	pop	{r5, pc}
	movs	r0, r0
	.2byte 0xa000
	.2byte 0x0202
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r7, r1, #0
	adds	r5, r0, #0
	bl	Func_080cdf5c
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #32]
	ldr	r3, [r3, #0]
	mov	r8, r3
	bl	ObjectTable_Get
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L_080ec1c4
	ldr	r2, [pc, #60]
	asrs	r5, r5, #16
	movs	r1, #192
	lsls	r0, r5, #14
	lsls	r1, r1, #2
	adds	r0, r0, r2
	adds	r1, #85
	bl	Math_Div
	asrs	r7, r7, #16
	adds	r5, r0, #0
	movs	r3, #224
	lsls	r3, r3, #13
	lsls	r5, r5, #16
	lsls	r0, r7, #14
	movs	r1, #160
	adds	r0, r0, r3
	str	r5, [r6, #8]
	lsls	r1, r1, #2
	bl	Math_Div
	mov	r2, r8
	lsls	r0, r0, #16
	str	r0, [r6, #16]
	str	r5, [r2, #0]
	ldr	r3, [r6, #16]
	str	r3, [r2, #8]
.L_080ec1c4:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x00434000
