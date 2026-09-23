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
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #48
	str	r1, [sp, #8]
	ldr	r3, [pc, #288]
	ldr	r3, [r3, #0]
	mov	r8, r3
	movs	r3, #0
	mov	r9, r3
	add	r3, sp, #24
	mov	sl, r3
	mov	r3, r9
	str	r3, [sp, #0]
	mov	fp, r0
	movs	r1, #1
	movs	r0, #1
	movs	r2, #28
	movs	r3, #5
	bl	sub_080162d4
	movs	r6, #0
	mov	r9, r0
	cmp	r0, #0
	beq.n	.L_080215aa
	movs	r1, #8
	movs	r3, #4
	movs	r2, #0
	str	r3, [sp, #0]
	bl	sub_0801e41c
	ldr	r2, [pc, #240]
	movs	r3, #1
	add	r2, r8
	strb	r3, [r2, #0]
	mov	r0, fp
	bl	sub_08021360
	bl	sub_08019d2c
	movs	r3, #14
	add	r5, sp, #16
	add	r2, sp, #20
	str	r3, [sp, #0]
	movs	r1, #0
	adds	r3, r5, #0
	str	r6, [sp, #4]
	bl	sub_0801a4fc
	mov	r3, sl
	str	r6, [r3, #0]
	ldr	r3, [pc, #204]
	movs	r2, #224
	str	r3, [sp, #28]
	ldr	r3, [sp, #16]
	lsls	r2, r2, #8
	ldr	r0, [sp, #8]
	orrs	r3, r2
	str	r3, [sp, #32]
	add	r7, sp, #36
	bl	sub_08021360
	bl	sub_08019d2c
	movs	r3, #15
	add	r2, sp, #12
	str	r3, [sp, #0]
	movs	r1, #0
	adds	r3, r5, #0
	str	r6, [sp, #4]
	bl	sub_0801a4fc
	ldr	r3, [pc, #164]
	str	r6, [r7, #0]
	movs	r2, #240
	str	r3, [sp, #40]
	ldr	r3, [sp, #16]
	lsls	r2, r2, #8
	orrs	r3, r2
	str	r3, [sp, #44]
	ldr	r3, [pc, #152]
	add	r3, r8
	strh	r6, [r3, #0]
	ldr	r3, [pc, #152]
	add	r3, r8
	strh	r6, [r3, #0]
	mov	r0, fp
	movs	r1, #1
	bl	sub_08019908
	movs	r1, #1
	ldr	r0, [sp, #8]
	bl	sub_08019908
	ldr	r0, [pc, #132]
	bl	sub_08019ba0
	movs	r2, #68
	adds	r1, r0, #0
	movs	r3, #2
	mov	r0, r9
	str	r6, [sp, #0]
	bl	sub_080165d8
	movs	r0, #81
	bl	sub_080f9010
.L_08021566:
	mov	r0, sl
	movs	r1, #250
	bl	sub_08003dec
	adds	r0, r7, #0
	movs	r1, #250
	bl	sub_08003dec
	movs	r0, #1
	bl	sub_080030f8
	bl	sub_080f9048
	cmp	r0, #0
	beq.n	.L_08021590
	ldr	r3, [pc, #80]
	ldr	r2, [pc, #84]
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08021566
.L_08021590:
	movs	r1, #2
	mov	r0, r9
	bl	sub_08016418
	movs	r0, #1
	bl	sub_080030f8
	ldr	r0, [sp, #20]
	bl	sub_08003f3c
	ldr	r0, [sp, #12]
	bl	sub_08003f3c
.L_080215aa:
	add	sp, #48
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001e8c
	.4byte 0x00000ea3
	.4byte 0x800c000c
	.4byte 0x802c000c
	.4byte 0x000012f4
	.4byte 0x000012f6
	.4byte 0x0000001d
	.4byte 0x03001c94
	.4byte 0x00000303
