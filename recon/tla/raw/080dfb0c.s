.syntax unified
	.thumb
	.global Func_080dfb0c
	.thumb_func
Func_080dfb0c:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	sub	sp, #68
	mov	sl, r0
	add	r0, sp, #28
	movs	r3, #0
	str	r3, [r0, #4]
	ldr	r3, [pc, #96]
	mov	r8, r0
	str	r3, [r0, #36]
	movs	r3, #204
	lsls	r3, r3, #8
	adds	r3, #204
	str	r3, [r0, #8]
	str	r3, [r0, #12]
	movs	r7, #0
	add	r6, sp, #16
.L_080dfb32:
	lsls	r5, r7, #12
	adds	r0, r5, #0
	bl	Math_Cosine
	lsls	r3, r0, #1
	adds	r3, r3, r0
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [r6, #0]
	adds	r0, r5, #0
	movs	r3, #0
	str	r3, [r6, #4]
	bl	Math_Sine
	str	r0, [r6, #8]
	mov	r2, sl
	ldr	r5, [r2, #8]
	ldr	r1, [r2, #12]
	ldr	r3, [r6, #0]
	ldr	r2, [r2, #16]
	ldr	r4, [r6, #4]
	str	r0, [sp, #4]
	ldr	r0, [pc, #32]
	adds	r7, #1
	str	r0, [sp, #8]
	mov	r0, r8
	str	r0, [sp, #12]
	adds	r0, r5, #0
	str	r4, [sp, #0]
	bl	Func_080df8e0
	cmp	r7, #16
	bls.n	.L_080dfb32
	add	sp, #68
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.4byte 0x080dfab5
	.2byte 0x0001
	.2byte 0x0109
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	adds	r7, r0, #0
	bl	Random16
	ldrh	r6, [r7, #6]
	movs	r1, #128
	lsls	r1, r1, #10
	adds	r5, r0, #0
	adds	r0, r6, #0
	adds	r5, r5, r1
	bl	Math_Cosine
	ldr	r2, [pc, #140]
	adds	r1, r0, #0
	mov	r8, r2
	adds	r0, r5, #0
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x4682
	adds	r0, r6, #0
	bl	Math_Sine
	adds	r1, r0, #0
	adds	r0, r5, #0
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x68bb
	movs	r1, #255
	add	r3, sl
	str	r3, [r7, #8]
	ldr	r3, [r7, #16]
	lsls	r1, r1, #8
	adds	r3, r3, r0
	str	r3, [r7, #16]
	ldrh	r3, [r7, #6]
	adds	r1, #240
	adds	r3, r3, r1
	strh	r3, [r7, #6]
	adds	r5, r7, #0
	adds	r5, #102
	movs	r1, #0
	ldrsh	r3, [r5, r1]
	ldrh	r2, [r5, #0]
	cmp	r3, #0
	beq.n	.L_080dfbf8
	subs	r3, r2, #1
	strh	r3, [r5, #0]
	ldrh	r3, [r7, #6]
	movs	r2, #128
	lsls	r2, r2, #4
	adds	r3, r3, r2
	strh	r3, [r7, #6]
	b.n	.L_080dfc10
.L_080dfbf8:
	bl	Random16
	lsls	r0, r0, #5
	lsrs	r0, r0, #16
	cmp	r0, #0
	bne.n	.L_080dfc10
	bl	Random16
	lsls	r0, r0, #4
	lsrs	r0, r0, #16
	adds	r0, #8
	strh	r0, [r5, #0]
.L_080dfc10:
	adds	r2, r7, #0
	adds	r2, #100
	ldrh	r3, [r2, #0]
	movs	r1, #202
	adds	r3, #1
	strh	r3, [r2, #0]
	lsls	r1, r1, #15
	lsls	r3, r3, #16
	cmp	r3, r1
	bne.n	.L_080dfc2c
	ldr	r1, [pc, #16]
	adds	r0, r7, #0
	bl	Object_SetCallback
.L_080dfc2c:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.4byte 0x0300021c
	.2byte 0x0e54
	.2byte 0x080f
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	adds	r7, r0, #0
	ldr	r0, [r7, #104]
	ldrh	r5, [r7, #6]
	movs	r2, #128
	lsls	r2, r2, #12
	mov	r9, r0
	adds	r0, r5, #0
	mov	r8, r2
	bl	Math_Cosine
	ldr	r6, [pc, #112]
	adds	r1, r0, #0
	mov	r0, r8
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x4682
	adds	r0, r5, #0
	bl	Math_Sine
	adds	r1, r0, #0
	mov	r0, r8
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x464a
	ldr	r3, [r2, #8]
	movs	r1, #0
	add	r3, sl
	str	r3, [r7, #8]
	ldr	r3, [r2, #16]
	adds	r2, r7, #0
	adds	r3, r3, r0
	str	r3, [r7, #16]
	ldrh	r3, [r7, #6]
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r3, r3, r0
	strh	r3, [r7, #6]
	adds	r2, #100
	ldrh	r3, [r2, #0]
	movs	r0, #242
	adds	r3, #1
	strh	r3, [r2, #0]
	lsls	r0, r0, #15
	lsls	r3, r3, #16
	cmp	r3, r0
	bne.n	.L_080dfcc2
	ldr	r3, [pc, #44]
	str	r3, [r7, #108]
	adds	r3, r7, #0
	adds	r3, #102
	strh	r1, [r2, #0]
	strh	r1, [r3, #0]
	movs	r3, #200
	lsls	r3, r3, #5
	adds	r3, #153
	str	r3, [r7, #72]
	movs	r3, #192
	lsls	r3, r3, #10
	str	r3, [r7, #40]
	bl	Random16
	strh	r0, [r7, #6]
.L_080dfcc2:
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.4byte 0x0300021c
	.2byte 0xfb89
	.2byte 0x080d
