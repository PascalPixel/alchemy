.syntax unified
	.thumb
	.balign 4
	.global Func_0811bfd0
	.thumb_func
Func_0811bfd0:
	push	{r5, r6, lr}
	bl	0x0811be3c
	adds	r6, r0, #0
	ldr	r5, [r6, #0]
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r5, #52]
	movs	r3, #128
	lsls	r3, r3, #11
	str	r3, [r5, #48]
	movs	r3, #192
	lsls	r3, r3, #10
	str	r3, [r5, #40]
	movs	r3, #153
	lsls	r3, r3, #8
	adds	r3, #153
	adds	r2, r5, #0
	str	r3, [r5, #72]
	adds	r2, #90
	movs	r3, #0
	str	r3, [r5, #68]
	adds	r0, r5, #0
	strb	r3, [r2, #0]
	bl	Object_ResetMotion
	ldr	r3, [r6, #12]
	adds	r0, r5, #0
	lsls	r1, r3, #1
	adds	r1, r1, r3
	lsrs	r3, r1, #31
	adds	r1, r1, r3
	asrs	r1, r1, #1
	ldr	r3, [r6, #16]
	movs	r2, #0
	bl	Object_SetPosition
	pop	{r5, r6, pc}
	push	{r5, r6, lr}
	bl	0x0811be3c
	adds	r6, r0, #0
	ldr	r5, [r6, #0]
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r5, #52]
	movs	r3, #128
	lsls	r3, r3, #11
	str	r3, [r5, #48]
	str	r3, [r5, #40]
	movs	r3, #153
	lsls	r3, r3, #8
	adds	r3, #153
	adds	r2, r5, #0
	str	r3, [r5, #72]
	adds	r2, #90
	movs	r3, #0
	str	r3, [r5, #68]
	adds	r0, r5, #0
	strb	r3, [r2, #0]
	bl	Object_ResetMotion
	movs	r1, #166
	lsls	r1, r1, #9
	ldr	r0, [r6, #12]
	ldr	r3, [pc, #28]
	adds	r1, #204
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x6933
	adds	r1, r0, #0
	movs	r2, #0
	adds	r0, r5, #0
	bl	Object_SetPosition
	adds	r0, r5, #0
	movs	r1, #5
	bl	Object_SetMode
	pop	{r5, r6, pc}
	.2byte 0x021c
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r5, r0, #0
	mov	r8, r1
	bl	0x0811be3c
	adds	r7, r0, #0
	adds	r0, r5, #0
	ldr	r6, [r7, #0]
	bl	Owner_GetState
	movs	r2, #165
	lsls	r2, r2, #1
	adds	r3, r0, r2
	ldrh	r0, [r3, #0]
	movs	r3, #102
	adds	r3, #255
	cmp	r0, r3
	beq.n	.L_0811c0f0
	adds	r2, #39
	cmp	r0, r2
	beq.n	.L_0811c0f0
	adds	r3, #16
	cmp	r0, r3
	beq.n	.L_0811c0f0
	adds	r2, #7
	cmp	r0, r2
	beq.n	.L_0811c0f0
	ldr	r3, [pc, #96]
	mov	r2, r8
	lsls	r5, r2, #2
	ldr	r3, [r3, r5]
	str	r3, [r6, #52]
	ldr	r3, [pc, #88]
	ldr	r3, [r3, r5]
	str	r3, [r6, #48]
	ldr	r3, [r6, #12]
	cmp	r3, #0
	beq.n	.L_0811c0c8
	cmp	r2, #4
	ble.n	.L_0811c0ce
.L_0811c0c8:
	ldr	r3, [pc, #76]
	ldr	r3, [r3, r5]
	str	r3, [r6, #40]
.L_0811c0ce:
	adds	r0, r6, #0
	bl	Object_ResetMotion
	ldr	r3, [pc, #68]
	ldr	r2, [r7, #12]
	ldr	r3, [r3, r5]
	movs	r1, #100
	adds	r0, r3, #0
	muls	r0, r2
	bl	Math_Div
	ldr	r3, [r7, #16]
	adds	r1, r0, #0
	movs	r2, #0
	adds	r0, r6, #0
	bl	Object_SetPosition
.L_0811c0f0:
	movs	r3, #153
	lsls	r3, r3, #8
	adds	r3, #153
	adds	r2, r6, #0
	str	r3, [r6, #72]
	adds	r2, #90
	movs	r3, #0
	str	r3, [r6, #68]
	adds	r0, r6, #0
	strb	r3, [r2, #0]
	movs	r1, #5
	bl	Object_SetMode
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.4byte 0x0812cbb0
	.4byte 0x0812cbd8
	.4byte 0x0812cc00
	.2byte 0xcc28
	.2byte 0x0812
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	adds	r6, r1, #0
	mov	fp, r2
	mov	r9, r3
	bl	0x0811be3c
	adds	r5, r0, #0
	adds	r0, r6, #0
	bl	0x0811be3c
	ldr	r7, [r5, #0]
	ldr	r6, [r0, #0]
	movs	r2, #75
	mov	r8, r2
	ldr	r3, [r6, #8]
	ldr	r2, [r7, #8]
	movs	r1, #100
	subs	r3, r3, r2
	mov	r0, r8
	muls	r0, r3
	mov	sl, r2
	bl	Math_Div
	ldr	r3, [r6, #16]
	ldr	r6, [r7, #16]
	adds	r5, r0, #0
	subs	r3, r3, r6
	mov	r0, r8
	muls	r0, r3
	movs	r1, #100
	bl	Math_Div
	mov	r3, sl
	adds	r3, r3, r5
	adds	r6, r6, r0
	asrs	r5, r5, #8
	asrs	r0, r0, #8
	mov	r8, r3
	adds	r2, r5, #0
	muls	r2, r5
	adds	r3, r0, #0
	muls	r3, r0
	adds	r0, r2, r3
	cmp	r0, #0
	beq.n	.L_0811c190
	ldr	r3, [pc, #108]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x0205
	b.n	.L_0811c192
.L_0811c190:
	movs	r5, #0
.L_0811c192:
	adds	r0, r5, #0
	mov	r1, fp
	bl	Math_Div
	adds	r3, r7, #0
	adds	r5, r0, #0
	adds	r3, #88
	movs	r1, #1
	str	r5, [r7, #52]
	str	r5, [r7, #48]
	strb	r1, [r3, #0]
	subs	r3, #3
	ldrb	r2, [r3, #0]
	movs	r3, #4
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0811c1b8
	mov	r2, r9
	str	r2, [r7, #40]
.L_0811c1b8:
	mov	r3, r9
	str	r3, [r7, #40]
	movs	r3, #171
	lsls	r3, r3, #8
	adds	r3, #133
	str	r3, [r7, #72]
	adds	r3, r7, #0
	adds	r3, #90
	strb	r1, [r3, #0]
	adds	r0, r7, #0
	bl	Object_ResetMotion
	cmp	r5, #0
	beq.n	.L_0811c1e0
	adds	r0, r7, #0
	mov	r1, r8
	movs	r2, #0
	adds	r3, r6, #0
	bl	Object_SetPosition
.L_0811c1e0:
	adds	r0, r7, #0
	movs	r1, #2
	bl	Object_SetMode
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x02d4
	.2byte 0x0300
	push	{r5, lr}
	bl	0x0811be3c
	ldr	r5, [r0, #0]
	adds	r0, r5, #0
	bl	Object_ResetMotion
	adds	r0, r5, #0
	movs	r1, #2
	bl	Object_SetMode
	pop	{r5, pc}
