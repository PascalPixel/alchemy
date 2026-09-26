.syntax unified
	.thumb
	.set sub_080030f8, 0x080030f8
	.set sub_08003dec, 0x08003dec
	.set sub_08003f3c, 0x08003f3c
	.set sub_080162d4, 0x080162d4
	.set sub_08016418, 0x08016418
	.set sub_080165d8, 0x080165d8
	.set sub_08019908, 0x08019908
	.set sub_08019ba0, 0x08019ba0
	.set sub_08019d2c, 0x08019d2c
	.set sub_0801a4fc, 0x0801a4fc
	.set sub_0801e41c, 0x0801e41c
	.set sub_08021360, 0x08021360
	.set sub_080f9010, 0x080f9010
	.set sub_080f9048, 0x080f9048
	.global Func_08021390
	.thumb_func
Func_08021390:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #208]
	sub	sp, #28
	movs	r2, #0
	ldr	r5, [r3, #0]
	mov	sl, r0
	str	r2, [sp, #0]
	movs	r0, #2
	movs	r1, #1
	movs	r2, #26
	movs	r3, #5
	add	r7, sp, #16
	bl	sub_080162d4
	movs	r6, #0
	mov	r8, r0
	cmp	r0, #0
	beq.n	.L_0802145c
	movs	r1, #4
	movs	r3, #4
	movs	r2, #0
	str	r3, [sp, #0]
	bl	sub_0801e41c
	ldr	r3, [pc, #168]
	adds	r2, r5, r3
	movs	r3, #1
	strb	r3, [r2, #0]
	mov	r0, sl
	bl	sub_08021360
	bl	sub_08019d2c
	movs	r1, #14
	add	r2, sp, #12
	add	r3, sp, #8
	str	r1, [sp, #0]
	movs	r1, #0
	str	r6, [sp, #4]
	bl	sub_0801a4fc
	ldr	r3, [pc, #136]
	str	r6, [r7, #0]
	movs	r2, #224
	str	r3, [sp, #20]
	ldr	r3, [sp, #8]
	lsls	r2, r2, #8
	orrs	r3, r2
	ldr	r2, [pc, #128]
	str	r3, [sp, #24]
	adds	r3, r5, r2
	adds	r2, #2
	strh	r6, [r3, #0]
	adds	r3, r5, r2
	strh	r6, [r3, #0]
	movs	r1, #1
	mov	r0, sl
	bl	sub_08019908
	ldr	r0, [pc, #108]
	bl	sub_08019ba0
	movs	r2, #36
	adds	r1, r0, #0
	movs	r3, #2
	mov	r0, r8
	str	r6, [sp, #0]
	bl	sub_080165d8
	movs	r0, #81
	bl	sub_080f9010
	ldr	r5, [pc, #88]
	ldr	r6, [pc, #88]
.L_0802142a:
	adds	r0, r7, #0
	movs	r1, #250
	bl	sub_08003dec
	movs	r0, #1
	bl	sub_080030f8
	bl	sub_080f9048
	cmp	r0, #0
	beq.n	.L_08021448
	ldr	r3, [r6, #0]
	ands	r3, r5
	cmp	r3, #0
	beq.n	.L_0802142a
.L_08021448:
	mov	r0, r8
	movs	r1, #2
	bl	sub_08016418
	movs	r0, #1
	bl	sub_080030f8
	ldr	r0, [sp, #12]
	bl	sub_08003f3c
.L_0802145c:
	add	sp, #28
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001e8c
	.4byte 0x00000ea3
	.4byte 0x8014000c
	.4byte 0x000012f4
	.4byte 0x0000001b
	.4byte 0x00000303
	.2byte 0x1c94
	.2byte 0x0300
