.syntax unified
	.thumb
	.set sub_080022ec, 0x080022ec
	.set sub_08002df0, 0x08002df0
	.set sub_08004970, 0x08004970
	.set sub_080072f0, 0x080072f0
	.set sub_08077008, 0x08077008
	.set sub_08077010, 0x08077010
	.global Func_080c1c54
	.thumb_func
Func_080c1c54:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #12
	str	r0, [sp, #8]
	movs	r0, #36
	mov	sl, r1
	bl	sub_08004970
	mov	r9, r0
	ldr	r0, [sp, #8]
	bl	sub_08077008
	adds	r7, r0, #0
	adds	r6, r7, #0
	adds	r6, #16
	movs	r2, #36
	ldr	r3, [pc, #360]
	adds	r1, r6, #0
	mov	r0, r9
	bl	sub_080072f0
	mov	r1, sl
	lsls	r3, r1, #1
	add	r3, sl
	movs	r0, #0
	ldrsh	r5, [r6, r0]
	lsls	r0, r3, #5
	movs	r1, #10
	add	r0, sl
	mov	fp, r3
	bl	sub_080022ec
	adds	r5, r5, r0
	mov	r0, r9
	movs	r2, #0
	ldrsh	r3, [r0, r2]
	lsls	r0, r3, #3
	subs	r0, r0, r3
	movs	r1, #10
	bl	sub_080022ec
	cmp	r5, r0
	bge.n	.L_080c1cb6
	adds	r5, r0, #0
.L_080c1cb6:
	ldr	r1, [pc, #308]
	mov	r8, r1
	cmp	r5, r8
	ble.n	.L_080c1cc0
	mov	r5, r8
.L_080c1cc0:
	mov	r3, sl
	lsls	r3, r3, #4
	strh	r5, [r6, #0]
	mov	r1, sl
	subs	r0, r3, r1
	movs	r1, #10
	movs	r2, #18
	ldrsh	r5, [r7, r2]
	str	r3, [sp, #4]
	bl	sub_080022ec
	adds	r5, r5, r0
	mov	r0, r9
	movs	r2, #2
	ldrsh	r3, [r0, r2]
	lsls	r0, r3, #3
	subs	r0, r0, r3
	movs	r1, #10
	bl	sub_080022ec
	cmp	r5, r0
	bge.n	.L_080c1cee
	adds	r5, r0, #0
.L_080c1cee:
	cmp	r5, r8
	ble.n	.L_080c1cf4
	mov	r5, r8
.L_080c1cf4:
	mov	r1, sl
	lsls	r1, r1, #5
	mov	r2, sl
	subs	r0, r1, r2
	lsls	r0, r0, #2
	strh	r5, [r7, #18]
	mov	r8, r1
	subs	r0, r0, r2
	movs	r1, #10
	bl	sub_080022ec
	ldrh	r5, [r7, #24]
	adds	r5, r5, r0
	mov	r0, r9
	ldrh	r3, [r0, #8]
	lsls	r0, r3, #3
	subs	r0, r0, r3
	movs	r1, #10
	bl	sub_080022ec
	cmp	r5, r0
	bge.n	.L_080c1d22
	adds	r5, r0, #0
.L_080c1d22:
	ldr	r6, [pc, #204]
	cmp	r5, r6
	ble.n	.L_080c1d2a
	adds	r5, r6, #0
.L_080c1d2a:
	mov	r0, r8
	strh	r5, [r7, #24]
	movs	r1, #10
	add	r0, sl
	bl	sub_080022ec
	mov	r1, r9
	ldrh	r3, [r1, #10]
	ldrh	r5, [r7, #26]
	adds	r5, r5, r0
	lsls	r0, r3, #3
	subs	r0, r0, r3
	movs	r1, #10
	bl	sub_080022ec
	cmp	r5, r0
	bge.n	.L_080c1d4e
	adds	r5, r0, #0
.L_080c1d4e:
	cmp	r5, r6
	ble.n	.L_080c1d54
	adds	r5, r6, #0
.L_080c1d54:
	mov	r2, fp
	lsls	r0, r2, #4
	strh	r5, [r7, #26]
	movs	r1, #10
	add	r0, fp
	bl	sub_080022ec
	ldrh	r5, [r7, #28]
	adds	r5, r5, r0
	mov	r0, r9
	ldrh	r3, [r0, #12]
	lsls	r0, r3, #3
	subs	r0, r0, r3
	movs	r1, #10
	bl	sub_080022ec
	cmp	r5, r0
	bge.n	.L_080c1d7a
	adds	r5, r0, #0
.L_080c1d7a:
	cmp	r5, r6
	ble.n	.L_080c1d80
	adds	r5, r6, #0
.L_080c1d80:
	movs	r1, #20
	strh	r5, [r7, #28]
	mov	r8, r1
	movs	r6, #36
	movs	r4, #3
.L_080c1d8a:
	ldr	r0, [sp, #4]
	ldrsh	r2, [r6, r7]
	mov	r1, sl
	subs	r3, r0, r1
	adds	r5, r2, r3
	mov	r1, r9
	mov	r2, r8
	ldrsh	r3, [r2, r1]
	lsls	r0, r3, #3
	subs	r0, r0, r3
	movs	r1, #10
	str	r4, [sp, #0]
	bl	sub_080022ec
	ldr	r4, [sp, #0]
	cmp	r5, r0
	bge.n	.L_080c1dae
	adds	r5, r0, #0
.L_080c1dae:
	cmp	r5, #200
	ble.n	.L_080c1db4
	movs	r5, #200
.L_080c1db4:
	movs	r2, #4
	subs	r4, #1
	strh	r5, [r6, r7]
	add	r8, r2
	adds	r6, #4
	cmp	r4, #0
	bge.n	.L_080c1d8a
	ldrb	r3, [r7, #15]
	add	r3, sl
	strb	r3, [r7, #15]
	ldr	r0, [sp, #8]
	bl	sub_08077010
	mov	r0, r9
	bl	sub_08002df0
	add	sp, #12
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001388
	.4byte 0x0000270f
	.4byte 0x000003e7
