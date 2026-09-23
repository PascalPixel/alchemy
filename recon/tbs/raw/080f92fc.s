.syntax unified
	.thumb
	.set sub_080022fc, 0x080022fc
	.set sub_080030f8, 0x080030f8
	.set sub_080037d4, 0x080037d4
	.set sub_080f9080, 0x080f9080
	.global AudioTest_RunParameterController
	.global Func_080f92fc
	.thumb_func
AudioTest_RunParameterController:
Func_080f92fc:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #12
	mov	r9, sp
	ldr	r2, [pc, #276]
	mov	r3, r9
	ldmia	r2!, {r0, r1, r4}
	stmia	r3!, {r0, r1, r4}
	movs	r0, #2
	movs	r7, #0
	mov	fp, r0
	ldr	r3, [pc, #264]
	str	r7, [r3, #0]
	movs	r1, #20
	movs	r2, #0
	mov	sl, r1
	mov	r8, r2
	mov	r6, r9
.L_080f932a:
	mov	r3, sl
	cmp	r3, #0
	beq.n	.L_080f9336
	movs	r4, #1
	negs	r4, r4
	add	sl, r4
.L_080f9336:
	ldr	r2, [pc, #244]
	ldr	r3, [r2, #0]
	cmp	r3, #0
	beq.n	.L_080f9346
	movs	r3, #0
	movs	r0, #20
	str	r3, [r2, #0]
	mov	sl, r0
.L_080f9346:
	ldr	r5, [pc, #232]
	ldr	r3, [r5, #0]
	movs	r2, #4
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080f9362
	mov	r0, fp
	adds	r0, #1
	movs	r1, #5
	bl	sub_080022fc
	mov	fp, r0
	bl	sub_080037d4
.L_080f9362:
	ldr	r3, [r5, #0]
	movs	r2, #128
	lsls	r2, r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080f9374
	ldr	r3, [r6, #0]
	adds	r3, #10
	str	r3, [r6, #0]
.L_080f9374:
	ldr	r3, [r5, #0]
	movs	r2, #128
	lsls	r2, r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080f9386
	ldr	r3, [r6, #0]
	subs	r3, #10
	str	r3, [r6, #0]
.L_080f9386:
	ldr	r3, [r5, #0]
	movs	r2, #16
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080f9396
	ldr	r3, [r6, #0]
	adds	r3, #1
	str	r3, [r6, #0]
.L_080f9396:
	ldr	r3, [r5, #0]
	movs	r2, #32
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080f93a6
	ldr	r3, [r6, #0]
	subs	r3, #1
	str	r3, [r6, #0]
.L_080f93a6:
	ldr	r3, [r5, #0]
	movs	r2, #64
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080f93be
	cmp	r7, #0
	ble.n	.L_080f93be
	movs	r1, #4
	negs	r1, r1
	subs	r6, #4
	add	r8, r1
	subs	r7, #1
.L_080f93be:
	ldr	r5, [pc, #112]
	ldr	r3, [r5, #0]
	movs	r2, #128
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080f93d6
	cmp	r7, #1
	bgt.n	.L_080f93d6
	movs	r2, #4
	adds	r6, #4
	add	r8, r2
	adds	r7, #1
.L_080f93d6:
	ldr	r3, [r5, #0]
	movs	r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080f93ea
	mov	r3, r8
	mov	r4, r9
	ldr	r0, [r3, r4]
	bl	sub_080f9080
.L_080f93ea:
	ldr	r3, [r5, #0]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080f93fa
	movs	r0, #19
	bl	sub_080f9080
.L_080f93fa:
	ldr	r3, [r5, #0]
	movs	r2, #8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080f940a
	movs	r0, #17
	bl	sub_080f9080
.L_080f940a:
	ldr	r3, [r5, #0]
	movs	r2, #4
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080f941a
	ldr	r0, [pc, #28]
	bl	sub_080f9080
.L_080f941a:
	movs	r0, #1
	bl	sub_080030f8
	b.n	.L_080f932a
	movs	r0, r0
	.4byte 0x080fb794
	.4byte 0x03007804
	.4byte 0x03000b5c
	.4byte 0x03001b04
	.4byte 0x00000121
