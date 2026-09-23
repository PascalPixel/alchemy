.syntax unified
	.thumb
	.set sub_08002dd8, 0x08002dd8
	.set sub_080044d0, 0x080044d0
	.set sub_080048b0, 0x080048b0
	.set sub_080049ac, 0x080049ac
	.set sub_080051d8, 0x080051d8
	.set sub_080051e8, 0x080051e8
	.set sub_080072f0, 0x080072f0
	.set sub_0800b388, 0x0800b388
	.set sub_080770c0, 0x080770c0
	.global ObjectSystem_UpdateCameraFixed
	.global Func_0800c880
	.thumb_func
ObjectSystem_UpdateCameraFixed:
Func_0800c880:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #372]
	ldr	r7, [r3, #0]
	subs	r3, #24
	ldr	r3, [r3, #0]
	sub	sp, #52
	mov	r8, r3
	ldr	r5, [pc, #364]
	movs	r0, #52
	adds	r1, r5, #0
	bl	sub_080048b0
	movs	r2, #132
	lsrs	r5, r5, #2
	lsls	r2, r2, #24
	adds	r1, r0, #0
	ldr	r3, [pc, #348]
	ldr	r0, [pc, #352]
	orrs	r2, r5
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r5, r7, #0
	ldr	r3, [r5, #24]
	adds	r6, r5, #0
	adds	r6, #12
	cmp	r3, #0
	beq.n	.L_0800c8c4
	adds	r5, r3, #0
.L_0800c8c4:
	ldr	r3, [r7, #28]
	cmp	r3, #0
	beq.n	.L_0800c8cc
	adds	r6, r3, #0
.L_0800c8cc:
	ldr	r3, [r6, #0]
	ldr	r0, [r5, #0]
	ldr	r1, [r5, #8]
	subs	r0, r0, r3
	ldr	r3, [r6, #8]
	subs	r1, r1, r3
	asrs	r1, r1, #16
	asrs	r0, r0, #16
	bl	sub_080044d0
	movs	r3, #0
	mov	r2, r8
	lsls	r0, r0, #16
	asrs	r0, r0, #16
	strh	r3, [r2, #0]
	mov	r9, r0
	bl	sub_080049ac
	ldr	r0, [pc, #288]
	bl	sub_080770c0
	cmp	r0, #0
	beq.n	.L_0800c910
	ldr	r3, [pc, #284]
	ldr	r0, [pc, #284]
	add	r9, r3
	ldr	r3, [pc, #284]
	bl	sub_080072f0
	adds	r0, r5, #0
	adds	r1, r6, #0
	bl	sub_080051e8
	b.n	.L_0800c918
.L_0800c910:
	adds	r0, r5, #0
	adds	r1, r6, #0
	bl	sub_080051d8
.L_0800c918:
	ldr	r3, [pc, #264]
	ldr	r3, [r3, #0]
	ldr	r2, [pc, #264]
	mov	r8, r3
	add	r8, r2
	movs	r3, #128
	movs	r2, #63
	lsls	r3, r3, #9
	mov	r7, r8
	str	r2, [sp, #8]
	mov	fp, r3
	adds	r7, #24
.L_0800c930:
	mov	r2, r8
	ldr	r3, [r2, #0]
	cmp	r3, #0
	beq.n	.L_0800c9d8
	mov	r3, r8
	adds	r3, #84
	ldrb	r3, [r3, #0]
	movs	r2, #15
	ands	r2, r3
	cmp	r2, #1
	beq.n	.L_0800c950
	cmp	r2, #1
	ble.n	.L_0800c9d8
	cmp	r2, #2
	beq.n	.L_0800c988
	b.n	.L_0800c9d8
.L_0800c950:
	movs	r3, #8
	add	r3, r8
	mov	sl, r3
	ldr	r3, [r7, #0]
	str	r3, [sp, #20]
	ldr	r3, [r7, #4]
	add	r6, sp, #20
	str	r3, [r6, #4]
	ldr	r0, [pc, #176]
	ldr	r5, [r7, #56]
	bl	sub_080770c0
	cmp	r0, #0
	beq.n	.L_0800c972
	mov	r2, fp
	str	r2, [sp, #20]
	str	r2, [r6, #4]
.L_0800c972:
	mov	r2, r8
	ldrh	r3, [r2, #6]
	ldrb	r2, [r7, #10]
	add	r3, r9
	str	r2, [sp, #0]
	adds	r0, r5, #0
	mov	r1, sl
	adds	r2, r6, #0
	bl	sub_0800b388
	b.n	.L_0800c9d8
.L_0800c988:
	ldr	r3, [r7, #0]
	str	r3, [sp, #12]
	ldr	r3, [r7, #4]
	add	r4, sp, #12
	str	r3, [r4, #4]
	ldr	r0, [pc, #128]
	str	r4, [sp, #4]
	bl	sub_080770c0
	ldr	r4, [sp, #4]
	cmp	r0, #0
	beq.n	.L_0800c9a6
	mov	r3, fp
	str	r3, [sp, #12]
	str	r3, [r4, #4]
.L_0800c9a6:
	movs	r2, #3
	ldr	r6, [r7, #56]
	mov	sl, r2
.L_0800c9ac:
	ldmia	r6!, {r5}
	cmp	r5, #0
	beq.n	.L_0800c9cc
	mov	r2, r8
	ldrh	r3, [r2, #6]
	ldrb	r2, [r7, #10]
	mov	r1, r8
	str	r2, [sp, #0]
	add	r3, r9
	adds	r2, r4, #0
	adds	r0, r5, #0
	adds	r1, #8
	str	r4, [sp, #4]
	bl	sub_0800b388
	ldr	r4, [sp, #4]
.L_0800c9cc:
	movs	r3, #1
	negs	r3, r3
	add	sl, r3
	mov	r2, sl
	cmp	r2, #0
	bge.n	.L_0800c9ac
.L_0800c9d8:
	ldr	r3, [sp, #8]
	movs	r2, #112
	subs	r3, #1
	negs	r2, r2
	str	r3, [sp, #8]
	subs	r7, #112
	add	r8, r2
	cmp	r3, #0
	bge.n	.L_0800c930
	movs	r0, #52
	bl	sub_08002dd8
	add	sp, #52
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001e80
	.4byte 0x000002c4
	.4byte 0x040000d4
	.4byte 0x08009bb8
	.4byte 0x0000016b
	.4byte 0xffffe000
	.4byte 0x08013190
	.4byte 0x030002c0
	.4byte 0x03001e64
	.4byte 0x00001b90
