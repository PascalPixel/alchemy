.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_08002064, 0x08002064
	.set sub_08002096, 0x08002096
	.set sub_0801314c, 0x0801314c
	.set sub_08013300, 0x08013300
	.set sub_08013560, 0x08013560
	.set sub_08014274, 0x08014274
	.set sub_080142d4, 0x080142d4
	.set sub_080143ac, 0x080143ac
	.set sub_08014878, 0x08014878
	.set sub_0801489c, 0x0801489c
	.set sub_08014cc0, 0x08014cc0
	.set sub_0801587c, 0x0801587c
	.set sub_080202c8, 0x080202c8
	.set sub_08020330, 0x08020330
	.set sub_080cad84, 0x080cad84
	.set sub_080cdf5c, 0x080cdf5c
	.set sub_080ce31c, 0x080ce31c
	.set sub_080ce458, 0x080ce458
	.set sub_080db9c0, 0x080db9c0
	.set sub_080db9cc, 0x080db9cc
	.set sub_080dbcc0, 0x080dbcc0
	.set sub_080dc294, 0x080dc294
	.set sub_080dc384, 0x080dc384
	.set sub_080dc390, 0x080dc390
	.set sub_080e1420, 0x080e1420
	.set sub_080e43a4, 0x080e43a4
	.set sub_080e89e4, 0x080e89e4
	.set sub_080e8b44, 0x080e8b44
	.set sub_080e8c9c, 0x080e8c9c
	.set sub_080e8cfc, 0x080e8cfc
	.set sub_080e9688, 0x080e9688
	.set sub_080eaf98, 0x080eaf98
	.set sub_080eb01c, 0x080eb01c
	.set sub_080eb298, 0x080eb298
	.set sub_081c0010, 0x081c0010
	.global Func_080e8d80
	.thumb_func
Func_080e8d80:
.L_080e8d80:
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #92]
	movs	r4, #0
	mov	ip, r3
	ldr	r3, [r3, #80]
	cmp	r4, r3
	bge.n	.L_080e8dae
	mov	r1, ip
	mov	r0, ip
	adds	r1, #64
	adds	r0, #32
.L_080e8d9a:
	ldmia	r0!, {r3}
	adds	r4, #1
	ldr	r2, [r3, #80]
	ldrh	r3, [r1, #0]
	adds	r1, #2
	strh	r3, [r2, #18]
	mov	r2, ip
	ldr	r3, [r2, #80]
	cmp	r4, r3
	blt.n	.L_080e8d9a
.L_080e8dae:
	pop	{pc}
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r1, #132
	lsls	r1, r1, #6
	adds	r1, #84
	movs	r0, #92
	sub	sp, #56
	bl	sub_08014cc0
	str	r0, [sp, #52]
	movs	r2, #192
	lsls	r2, r2, #18
	adds	r3, r2, #0
	ldr	r2, [r2, #108]
	adds	r3, #224
	ldr	r3, [r3, #0]
	str	r2, [sp, #48]
	mov	sl, r3
	ldr	r1, [r3, #20]
	str	r1, [sp, #44]
	ldr	r2, [r1, #80]
	str	r2, [sp, #40]
	cmp	r1, #0
	beq.n	.L_080e8dfe
	cmp	r2, #0
	beq.n	.L_080e8dfe
	ldrb	r3, [r2, #20]
	ldrb	r2, [r2, #21]
	movs	r1, #128
	muls	r3, r2
	lsls	r1, r1, #4
	cmp	r3, r1
	ble.n	.L_080e8dfe
	b.n	.L_080e9064
.L_080e8dfe:
	bl	sub_080dc294
	ldr	r0, [pc, #628]
	bl	sub_08013300
	ldr	r7, [sp, #52]
	adds	r7, #84
	adds	r1, r7, #0
	bl	sub_0801587c
	bl	sub_080143ac
	movs	r1, #64
	adds	r2, r7, #0
	str	r0, [sp, #36]
	bl	sub_080142d4
	movs	r3, #208
	ldr	r2, [sp, #52]
	movs	r1, #128
	lsls	r3, r3, #5
	lsls	r1, r1, #5
	adds	r3, #84
	adds	r1, #84
	mov	r8, r0
	movs	r4, #0
	adds	r6, r2, r3
	adds	r5, r2, r1
.L_080e8e36:
	movs	r3, #1
	ands	r3, r4
	add	r3, r8
	str	r3, [sp, #0]
	adds	r0, r5, #0
	movs	r1, #4
	movs	r2, #4
	movs	r3, #0
	str	r4, [sp, #4]
	bl	sub_080eaf98
	ldrb	r3, [r5, #5]
	movs	r1, #33
	negs	r1, r1
	adds	r2, r1, #0
	ands	r3, r2
	ldrb	r2, [r5, #9]
	strb	r3, [r5, #5]
	movs	r3, #15
	ands	r3, r2
	adds	r1, #20
	movs	r2, #224
	orrs	r3, r2
	ldr	r4, [sp, #4]
	adds	r2, r1, #0
	ands	r3, r2
	strb	r3, [r5, #9]
	movs	r3, #240
	strh	r3, [r5, #30]
	adds	r4, #1
	subs	r3, #241
	str	r3, [r6, #24]
	adds	r5, #40
	adds	r6, #28
	cmp	r4, #63
	ble.n	.L_080e8e36
	mov	r2, sl
	ldr	r0, [r2, #16]
	movs	r1, #0
	bl	sub_080e1420
	ldr	r3, [sp, #44]
	cmp	r3, #0
	beq.n	.L_080e8eae
	ldr	r1, [sp, #40]
	cmp	r1, #0
	beq.n	.L_080e8eae
	adds	r0, r1, #0
	movs	r1, #0
	bl	sub_080202c8
	ldr	r2, [sp, #40]
	ldr	r1, [sp, #40]
	ldr	r0, [r2, #40]
	ldrb	r3, [r1, #21]
	ldrb	r2, [r2, #20]
	adds	r1, r7, #0
	muls	r2, r3
	bl	sub_080e43a4
.L_080e8eae:
	movs	r0, #1
	bl	sub_080e89e4
	bl	sub_080e8cfc
	movs	r0, #10
	bl	sub_08013560
	movs	r2, #1
	negs	r2, r2
	str	r2, [sp, #32]
	ldr	r3, [sp, #40]
	movs	r1, #0
	ldrb	r3, [r3, #20]
	movs	r2, #128
	str	r3, [sp, #28]
	str	r1, [sp, #24]
	str	r1, [sp, #16]
	str	r1, [sp, #20]
	str	r1, [sp, #8]
	lsls	r2, r2, #10
	mov	r8, r2
	mov	sl, r1
	mov	r9, r1
.L_080e8ede:
	ldr	r1, [sp, #20]
	movs	r3, #15
	ands	r3, r1
	cmp	r3, #15
	bne.n	.L_080e8eee
	movs	r0, #152
	bl	sub_081c0010
.L_080e8eee:
	ldr	r2, [sp, #48]
	movs	r1, #197
	lsls	r1, r1, #1
	adds	r3, r2, r1
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #2
	bne.n	.L_080e8f62
	ldr	r2, [sp, #52]
	movs	r4, #0
	ldr	r3, [r2, #80]
	cmp	r4, r3
	bge.n	.L_080e8f62
	adds	r7, r2, #0
	ldr	r3, [sp, #8]
	adds	r2, #32
	str	r2, [sp, #12]
	mov	fp, r3
	adds	r7, #64
.L_080e8f16:
	ldr	r2, [sp, #12]
	ldmia	r2!, {r3}
	adds	r1, r2, #0
	str	r1, [sp, #12]
	ldr	r6, [r3, #80]
	movs	r3, #128
	lsls	r3, r3, #11
	ldrh	r5, [r7, #0]
	adds	r7, #2
	cmp	r8, r3
	ble.n	.L_080e8f56
	mov	r1, fp
	lsls	r0, r1, #11
	str	r4, [sp, #4]
	bl	sub_08002096
	ldr	r1, [pc, #324]
	ldr	r4, [sp, #4]
	add	r1, r8
	cmp	r1, #0
	bge.n	.L_080e8f44
	ldr	r1, [pc, #316]
	add	r1, r8
.L_080e8f44:
	str	r4, [sp, #4]
	asrs	r1, r1, #7
	ldr	r2, [pc, #312]
	mov	lr, r2
	.2byte 0xf800
	.2byte 0x1828
	strh	r0, [r6, #18]
	ldr	r4, [sp, #4]
	b.n	.L_080e8f58
.L_080e8f56:
	strh	r5, [r6, #18]
.L_080e8f58:
	ldr	r1, [sp, #52]
	adds	r4, #1
	ldr	r3, [r1, #80]
	cmp	r4, r3
	blt.n	.L_080e8f16
.L_080e8f62:
	ldr	r1, [sp, #16]
	ldr	r2, [sp, #8]
	adds	r3, r1, #0
	adds	r2, #3
	str	r2, [sp, #8]
	adds	r3, #1
	movs	r0, #1
	mov	r2, r8
	str	r3, [sp, #16]
	bl	sub_080e8b44
	mov	r1, sl
	cmp	r1, #1
	beq.n	.L_080e8fb2
	cmp	r1, #1
	bgt.n	.L_080e8f88
	cmp	r1, #0
	beq.n	.L_080e8f94
	b.n	.L_080e8ff4
.L_080e8f88:
	mov	r2, sl
	cmp	r2, #2
	beq.n	.L_080e8fbe
	cmp	r2, #3
	beq.n	.L_080e8fe2
	b.n	.L_080e8ff4
.L_080e8f94:
	mov	r3, r9
	cmp	r3, #40
	bne.n	.L_080e8f9e
	movs	r1, #1
	str	r1, [sp, #24]
.L_080e8f9e:
	movs	r2, #200
	lsls	r2, r2, #5
	ldr	r3, [pc, #228]
	adds	r2, #153
	add	r8, r2
	cmp	r8, r3
	ble.n	.L_080e8ff4
	movs	r2, #1
	movs	r1, #1
	b.n	.L_080e8fda
.L_080e8fb2:
	mov	r3, r9
	cmp	r3, #40
	bne.n	.L_080e8ff4
	movs	r2, #1
	movs	r1, #2
	b.n	.L_080e8fda
.L_080e8fbe:
	mov	r3, r9
	cmp	r3, #50
	bne.n	.L_080e8fca
	movs	r1, #1
	negs	r1, r1
	str	r1, [sp, #24]
.L_080e8fca:
	ldr	r2, [pc, #192]
	movs	r3, #128
	add	r8, r2
	lsls	r3, r3, #10
	cmp	r8, r3
	bgt.n	.L_080e8ff4
	movs	r2, #1
	movs	r1, #3
.L_080e8fda:
	negs	r2, r2
	mov	sl, r1
	mov	r9, r2
	b.n	.L_080e8ff4
.L_080e8fe2:
	movs	r3, #0
	mov	r1, r9
	mov	r8, r3
	cmp	r1, #0
	bne.n	.L_080e8ff4
	movs	r2, #186
	lsls	r2, r2, #2
	adds	r2, #255
	mov	sl, r2
.L_080e8ff4:
	ldr	r3, [sp, #24]
	cmp	r3, #1
	bne.n	.L_080e9006
	ldr	r1, [sp, #32]
	ldr	r2, [sp, #28]
	cmp	r1, r2
	bgt.n	.L_080e9006
	adds	r1, #1
	str	r1, [sp, #32]
.L_080e9006:
	ldr	r3, [sp, #24]
	movs	r1, #1
	negs	r1, r1
	cmp	r3, r1
	bne.n	.L_080e901a
	ldr	r2, [sp, #32]
	cmp	r2, #0
	blt.n	.L_080e901a
	subs	r2, #2
	str	r2, [sp, #32]
.L_080e901a:
	movs	r0, #1
	bl	sub_08013560
	ldr	r1, [sp, #20]
	movs	r2, #186
	lsls	r2, r2, #2
	movs	r3, #1
	adds	r1, #1
	adds	r2, #255
	add	r9, r3
	str	r1, [sp, #20]
	cmp	sl, r2
	beq.n	.L_080e9036
	b.n	.L_080e8ede
.L_080e9036:
	ldr	r3, [sp, #44]
	cmp	r3, #0
	beq.n	.L_080e9052
	ldr	r1, [sp, #40]
	cmp	r1, #0
	beq.n	.L_080e9052
	adds	r0, r1, #0
	movs	r1, #16
	bl	sub_080202c8
	ldr	r3, [sp, #44]
	ldr	r2, [r3, #80]
	movs	r3, #1
	strb	r3, [r2, #25]
.L_080e9052:
	bl	sub_080e8c9c
	bl	.L_080e8d80
	ldr	r0, [sp, #36]
	bl	sub_08014274
	bl	sub_080dc384
.L_080e9064:
	movs	r0, #92
	bl	sub_0801314c
	add	sp, #56
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x000001ee
	.4byte 0xfffc0000
	.4byte 0xfffc007f
	.4byte 0x0300021c
	.4byte 0x000bffff
	.2byte 0xe667
	.2byte 0xffff
	.2byte 0xb5e0
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r1, #132
	lsls	r1, r1, #6
	adds	r1, #84
	movs	r0, #92
	sub	sp, #32
	bl	sub_08014cc0
	movs	r2, #192
	str	r0, [sp, #28]
	lsls	r2, r2, #18
	adds	r3, r2, #0
	adds	r3, #224
	ldr	r5, [r3, #0]
	ldr	r2, [r2, #108]
	str	r2, [sp, #24]
	bl	sub_080dc294
	movs	r1, #0
	ldr	r0, [r5, #16]
	bl	sub_080e1420
	movs	r0, #1
	bl	sub_080e89e4
	bl	sub_080e8cfc
	movs	r0, #10
	bl	sub_08013560
	movs	r0, #0
	movs	r1, #128
	str	r0, [sp, #16]
	str	r0, [sp, #20]
	str	r0, [sp, #4]
	lsls	r1, r1, #10
	mov	r9, r1
	mov	sl, r0
	mov	fp, r0
.L_080e90ea:
	ldr	r2, [sp, #20]
	movs	r3, #15
	ands	r3, r2
	cmp	r3, #15
	bne.n	.L_080e90fa
	movs	r0, #152
	bl	sub_081c0010
.L_080e90fa:
	ldr	r0, [sp, #24]
	movs	r1, #197
	lsls	r1, r1, #1
	adds	r3, r0, r1
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #2
	bne.n	.L_080e9170
	ldr	r0, [sp, #28]
	movs	r2, #0
	ldr	r3, [r0, #80]
	cmp	r2, r3
	bge.n	.L_080e9170
	ldr	r3, [sp, #28]
	ldr	r1, [sp, #4]
	adds	r3, #32
	str	r1, [sp, #12]
	str	r3, [sp, #8]
	adds	r0, #64
	mov	r8, r0
.L_080e9124:
	ldr	r1, [sp, #8]
	ldmia	r1!, {r3}
	adds	r0, r1, #0
	str	r0, [sp, #8]
	mov	r1, r9
	ldr	r7, [r3, #80]
	movs	r0, #2
	mov	r3, r8
	ldrh	r6, [r3, #0]
	add	r8, r0
	cmp	r1, #0
	ble.n	.L_080e9164
	ldr	r3, [sp, #12]
	str	r2, [sp, #0]
	lsls	r0, r3, #11
	bl	sub_08002096
	movs	r1, #192
	adds	r5, r0, #0
	lsls	r1, r1, #1
	mov	r0, r9
	bl	sub_08002054
	ldr	r3, [pc, #240]
	adds	r1, r0, #0
	adds	r0, r5, #0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x1830
	strh	r0, [r7, #18]
	ldr	r2, [sp, #0]
	b.n	.L_080e9166
.L_080e9164:
	strh	r6, [r7, #18]
.L_080e9166:
	ldr	r0, [sp, #28]
	adds	r2, #1
	ldr	r3, [r0, #80]
	cmp	r2, r3
	blt.n	.L_080e9124
.L_080e9170:
	ldr	r1, [sp, #16]
	ldr	r2, [sp, #4]
	adds	r3, r1, #0
	adds	r2, #3
	str	r2, [sp, #4]
	adds	r3, #1
	movs	r0, #1
	mov	r2, r9
	str	r3, [sp, #16]
	bl	sub_080e8b44
	mov	r0, sl
	cmp	r0, #1
	beq.n	.L_080e91b6
	cmp	r0, #1
	bgt.n	.L_080e9196
	cmp	r0, #0
	beq.n	.L_080e91a2
	b.n	.L_080e9206
.L_080e9196:
	mov	r1, sl
	cmp	r1, #2
	beq.n	.L_080e91dc
	cmp	r1, #3
	beq.n	.L_080e91f4
	b.n	.L_080e9206
.L_080e91a2:
	movs	r2, #152
	lsls	r2, r2, #6
	ldr	r3, [pc, #160]
	adds	r2, #102
	add	r9, r2
	cmp	r9, r3
	ble.n	.L_080e9206
	movs	r1, #1
	movs	r0, #1
	b.n	.L_080e91c8
.L_080e91b6:
	ldr	r2, [sp, #28]
	ldr	r3, [r2, #80]
	cmp	r3, #0
	beq.n	.L_080e91d0
	mov	r3, fp
	cmp	r3, #50
	bne.n	.L_080e9206
	movs	r1, #1
	movs	r0, #2
.L_080e91c8:
	negs	r1, r1
	mov	sl, r0
	mov	fp, r1
	b.n	.L_080e9206
.L_080e91d0:
	mov	r2, fp
	cmp	r2, #10
	bne.n	.L_080e9206
	movs	r0, #1
	movs	r3, #2
	b.n	.L_080e91ec
.L_080e91dc:
	ldr	r1, [pc, #108]
	movs	r2, #128
	add	r9, r1
	lsls	r2, r2, #10
	cmp	r9, r2
	bgt.n	.L_080e9206
	movs	r0, #1
	movs	r3, #3
.L_080e91ec:
	negs	r0, r0
	mov	sl, r3
	mov	fp, r0
	b.n	.L_080e9206
.L_080e91f4:
	movs	r1, #0
	mov	r2, fp
	mov	r9, r1
	cmp	r2, #0
	bne.n	.L_080e9206
	movs	r3, #186
	lsls	r3, r3, #2
	adds	r3, #255
	mov	sl, r3
.L_080e9206:
	movs	r0, #1
	bl	sub_08013560
	ldr	r1, [sp, #20]
	movs	r2, #186
	lsls	r2, r2, #2
	movs	r0, #1
	adds	r1, #1
	adds	r2, #255
	add	fp, r0
	str	r1, [sp, #20]
	cmp	sl, r2
	beq.n	.L_080e9222
	b.n	.L_080e90ea
.L_080e9222:
	bl	sub_080e8c9c
	bl	.L_080e8d80
	bl	sub_080dc384
	movs	r0, #92
	bl	sub_0801314c
	add	sp, #32
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x0300021c
	.4byte 0x0005ffff
	.2byte 0xd99a
	.2byte 0xffff
.L_080e9250:
	.2byte 0xb5e0
	mov	r7, r8
	push	{r7}
	adds	r4, r1, #0
	subs	r3, r2, #2
	muls	r3, r4
	subs	r2, #1
	adds	r7, r0, r3
	adds	r3, r4, #0
	muls	r3, r2
	mov	ip, r0
	add	r3, ip
	adds	r5, r2, #0
	mov	r8, r3
	cmp	r5, #1
	beq.n	.L_080e929e
	movs	r1, #132
	lsls	r1, r1, #24
	mov	lr, r1
.L_080e9276:
	adds	r2, r4, #0
	cmp	r4, #0
	bge.n	.L_080e927e
	adds	r2, r4, #3
.L_080e927e:
	movs	r3, #128
	mov	r6, lr
	asrs	r2, r2, #2
	lsls	r3, r3, #19
	adds	r3, #212
	adds	r0, r7, #0
	mov	r1, r8
	orrs	r2, r6
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	subs	r1, r1, r4
	subs	r5, #1
	subs	r7, r7, r4
	mov	r8, r1
	cmp	r5, #1
	bne.n	.L_080e9276
.L_080e929e:
	ldr	r3, [pc, #16]
	mov	r0, ip
	adds	r1, r4, #0
	movs	r2, #0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0xbc08
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0260
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r1, #201
	lsls	r1, r1, #5
	movs	r0, #92
	sub	sp, #88
	bl	sub_08014cc0
	movs	r3, #192
	str	r0, [sp, #84]
	lsls	r3, r3, #18
	adds	r3, #224
	ldr	r3, [r3, #0]
	movs	r2, #0
	str	r3, [sp, #80]
	ldr	r0, [r3, #20]
	ldr	r5, [r3, #16]
	str	r0, [sp, #76]
	movs	r3, #224
	lsls	r3, r3, #12
	ldr	r1, [r0, #80]
	str	r3, [sp, #32]
	str	r2, [sp, #56]
	str	r2, [sp, #52]
	str	r2, [sp, #48]
	str	r2, [sp, #44]
	str	r2, [sp, #36]
	ldr	r2, [sp, #80]
	mov	fp, r1
	movs	r1, #30
	ldrsh	r0, [r2, r1]
	bl	sub_080ce31c
	str	r0, [sp, #28]
	ldr	r2, [sp, #80]
	movs	r0, #160
	lsls	r0, r0, #23
	movs	r3, #30
	ldrsh	r1, [r2, r3]
	adds	r0, #5
	ldr	r2, [sp, #28]
	bl	sub_080ce458
	ldr	r3, [sp, #76]
	str	r0, [sp, #24]
	cmp	r3, #0
	beq.n	.L_080e9334
	mov	r0, fp
	cmp	r0, #0
	beq.n	.L_080e9356
	ldrb	r3, [r0, #20]
	ldrb	r2, [r0, #21]
	movs	r1, #128
	muls	r3, r2
	lsls	r1, r1, #4
	cmp	r3, r1
	ble.n	.L_080e9332
	b.n	.L_080e97a6
.L_080e9332:
	b.n	.L_080e9356
.L_080e9334:
	ldr	r2, [sp, #80]
	ldr	r3, [r5, #8]
	movs	r0, #128
	str	r3, [r2, #4]
	lsls	r0, r0, #12
	ldr	r3, [r5, #16]
	ldrh	r1, [r2, #0]
	adds	r3, r3, r0
	str	r3, [r2, #12]
	movs	r0, #128
	ldr	r3, [r5, #12]
	lsls	r0, r0, #14
	str	r3, [r2, #8]
	ldr	r2, [sp, #80]
	adds	r2, #4
	bl	sub_0801489c
.L_080e9356:
	movs	r1, #0
	ldr	r0, [sp, #76]
	str	r1, [sp, #40]
	bl	sub_08020330
	movs	r2, #212
	lsls	r2, r2, #1
	cmp	r0, r2
	bne.n	.L_080e936e
	movs	r3, #1
	str	r3, [sp, #40]
	str	r3, [sp, #36]
.L_080e936e:
	ldr	r0, [sp, #76]
	cmp	r0, #0
	bne.n	.L_080e9378
	movs	r1, #2
	str	r1, [sp, #36]
.L_080e9378:
	ldr	r2, [sp, #36]
	cmp	r2, #1
	bne.n	.L_080e9384
	movs	r3, #224
	lsls	r3, r3, #12
	str	r3, [sp, #32]
.L_080e9384:
	ldr	r0, [sp, #36]
	cmp	r0, #2
	bne.n	.L_080e9390
	movs	r1, #168
	lsls	r1, r1, #13
	str	r1, [sp, #32]
.L_080e9390:
	bl	sub_080dc294
	ldr	r0, [pc, #416]
	bl	sub_08013300
	ldr	r2, [sp, #84]
	adds	r2, #32
	adds	r1, r2, #0
	str	r2, [sp, #20]
	bl	sub_0801587c
	bl	sub_080143ac
	movs	r1, #128
	lsls	r1, r1, #3
	ldr	r2, [sp, #20]
	str	r0, [sp, #72]
	bl	sub_080142d4
	ldr	r3, [sp, #84]
	str	r0, [sp, #68]
	movs	r1, #242
	movs	r0, #130
	lsls	r0, r0, #4
	lsls	r1, r1, #4
	movs	r2, #13
	adds	r6, r3, r0
	adds	r5, r3, r1
	negs	r2, r2
	movs	r3, #63
	adds	r7, r2, #0
	mov	r9, r3
.L_080e93d0:
	ldr	r0, [sp, #68]
	movs	r3, #128
	str	r0, [sp, #0]
	movs	r1, #8
	adds	r0, r5, #0
	movs	r2, #8
	lsls	r3, r3, #23
	bl	sub_080eaf98
	ldrb	r3, [r5, #5]
	movs	r2, #32
	orrs	r3, r2
	ldrb	r2, [r5, #9]
	strb	r3, [r5, #5]
	movs	r3, #15
	ands	r3, r2
	ands	r3, r7
	strb	r3, [r5, #9]
	movs	r3, #240
	strh	r3, [r5, #30]
	subs	r3, #241
	add	r9, r3
	mov	r1, r9
	str	r3, [r6, #24]
	adds	r5, #40
	adds	r6, #28
	cmp	r1, #0
	bge.n	.L_080e93d0
	ldr	r2, [sp, #76]
	cmp	r2, #0
	beq.n	.L_080e9432
	mov	r3, fp
	cmp	r3, #0
	beq.n	.L_080e9432
	ldr	r0, [sp, #40]
	cmp	r0, #1
	bne.n	.L_080e9432
	mov	r0, fp
	movs	r1, #0
	bl	sub_080202c8
	mov	r1, fp
	ldrb	r2, [r1, #20]
	ldrb	r3, [r1, #21]
	ldr	r0, [r1, #40]
	muls	r2, r3
	ldr	r1, [sp, #20]
	bl	sub_080e43a4
.L_080e9432:
	movs	r0, #2
	bl	sub_080e89e4
	movs	r0, #1
	bl	sub_08013560
	movs	r0, #238
	bl	sub_081c0010
	movs	r3, #128
	movs	r2, #0
	lsls	r3, r3, #10
	str	r2, [sp, #16]
	str	r3, [sp, #12]
	str	r2, [sp, #64]
	str	r2, [sp, #60]
.L_080e9452:
	ldr	r0, [sp, #48]
	cmp	r0, #0
	beq.n	.L_080e9480
	ldr	r1, [sp, #36]
	cmp	r1, #1
	bne.n	.L_080e9470
	ldr	r2, [sp, #32]
	movs	r3, #160
	lsls	r3, r3, #11
	cmp	r2, r3
	ble.n	.L_080e9480
	ldr	r0, [pc, #208]
	adds	r2, r2, r0
	str	r2, [sp, #32]
	b.n	.L_080e9480
.L_080e9470:
	ldr	r1, [sp, #32]
	movs	r2, #160
	lsls	r2, r2, #11
	cmp	r1, r2
	ble.n	.L_080e9480
	ldr	r3, [pc, #196]
	adds	r1, r1, r3
	str	r1, [sp, #32]
.L_080e9480:
	ldr	r0, [sp, #48]
	cmp	r0, #0
	beq.n	.L_080e957e
	mov	r9, r0
.L_080e9488:
	bl	sub_08014878
	ldr	r1, [sp, #36]
	adds	r7, r0, #0
	cmp	r1, #2
	bne.n	.L_080e94a4
	bl	sub_08014878
	lsls	r3, r0, #2
	adds	r3, r3, r0
	ldr	r2, [pc, #164]
	lsls	r3, r3, #1
	lsrs	r3, r3, #4
	adds	r7, r3, r2
.L_080e94a4:
	ldr	r0, [sp, #44]
	ldr	r1, [sp, #84]
	lsls	r3, r0, #2
	adds	r3, r3, r0
	lsls	r3, r3, #3
	movs	r2, #242
	adds	r3, r1, r3
	lsls	r2, r2, #4
	adds	r5, r3, r2
	lsls	r3, r0, #3
	subs	r3, r3, r0
	lsls	r3, r3, #2
	adds	r3, r1, r3
	ldr	r1, [sp, #76]
	movs	r0, #130
	lsls	r0, r0, #4
	adds	r6, r3, r0
	cmp	r1, #0
	beq.n	.L_080e9502
	adds	r0, r1, #0
	bl	sub_080db9c0
	movs	r3, #3
	ands	r0, r3
	movs	r1, #13
	ldrb	r3, [r5, #9]
	negs	r1, r1
	adds	r2, r1, #0
	ands	r3, r2
	movs	r2, #254
	lsls	r0, r0, #2
	lsls	r2, r2, #7
	orrs	r3, r0
	adds	r2, #255
	strb	r3, [r5, #9]
	cmp	r7, r2
	bgt.n	.L_080e94f8
	ldr	r0, [sp, #76]
	bl	sub_080db9cc
	adds	r0, #15
	b.n	.L_080e9500
.L_080e94f8:
	ldr	r0, [sp, #76]
	bl	sub_080db9cc
	subs	r0, #15
.L_080e9500:
	strh	r0, [r5, #30]
.L_080e9502:
	movs	r3, #0
	str	r3, [r6, #24]
	ldr	r0, [sp, #80]
	movs	r2, #160
	ldr	r3, [r0, #4]
	lsls	r2, r2, #11
	str	r3, [r6, #0]
	ldr	r3, [r0, #8]
	adds	r3, r3, r2
	str	r3, [r6, #4]
	ldr	r3, [r0, #12]
	str	r3, [r6, #8]
	ldr	r1, [sp, #32]
	cmp	r1, r2
	ble.n	.L_080e9548
	bl	sub_08014878
	adds	r3, r0, #0
	ldr	r2, [sp, #32]
	lsls	r0, r3, #2
	adds	r0, r0, r3
	adds	r0, r2, r0
	adds	r1, r7, #0
	adds	r2, r6, #0
	bl	sub_0801489c
	b.n	.L_080e955a
	.4byte 0x000001f1
	.4byte 0xfffff5c3
	.4byte 0xffffeb86
	.2byte 0xf000
	.2byte 0xffff
.L_080e9548:
	bl	sub_08014878
	ldr	r3, [sp, #32]
	lsls	r0, r0, #2
	subs	r0, r3, r0
	adds	r1, r7, #0
	adds	r2, r6, #0
	bl	sub_0801489c
.L_080e955a:
	ldr	r0, [sp, #44]
	adds	r0, #1
	str	r0, [sp, #44]
	adds	r3, r0, #0
	cmp	r0, #0
	bge.n	.L_080e9568
	adds	r3, #63
.L_080e9568:
	ldr	r1, [sp, #44]
	movs	r2, #1
	asrs	r3, r3, #6
	negs	r2, r2
	lsls	r3, r3, #6
	add	r9, r2
	subs	r1, r1, r3
	mov	r3, r9
	str	r1, [sp, #44]
	cmp	r3, #0
	bne.n	.L_080e9488
.L_080e957e:
	ldr	r2, [sp, #68]
	ldr	r0, [sp, #84]
	ldr	r3, [pc, #52]
	movs	r1, #130
	mov	r8, r2
	lsls	r1, r1, #4
	adds	r6, r0, r1
	mov	r0, r8
	ands	r0, r3
	adds	r4, r2, #0
	ldr	r2, [sp, #84]
	mov	r8, r0
	movs	r0, #242
	lsls	r0, r0, #4
	ldr	r1, [pc, #32]
	adds	r5, r2, r0
	ldr	r7, [sp, #68]
	ldr	r2, [sp, #68]
	mov	sl, r1
	adds	r4, #8
	adds	r2, #16
	adds	r7, #24
	movs	r1, #63
	ands	r4, r3
	ands	r2, r3
	ands	r7, r3
	mov	r9, r1
	b.n	.L_080e95c0
	movs	r0, r0
	.4byte 0x000003ff
	.2byte 0xfc00
	.2byte 0xffff
.L_080e95c0:
	ldr	r3, [r6, #24]
	cmp	r3, #23
	bhi.n	.L_080e961c
	cmp	r3, #0
	bne.n	.L_080e95d8
	ldrh	r3, [r5, #8]
	mov	r0, sl
	ands	r3, r0
	mov	r1, r8
	orrs	r3, r1
	strh	r3, [r5, #8]
	ldr	r3, [r6, #24]
.L_080e95d8:
	cmp	r3, #18
	bne.n	.L_080e95e8
	ldrh	r3, [r5, #8]
	mov	r0, sl
	ands	r3, r0
	orrs	r3, r4
	strh	r3, [r5, #8]
	ldr	r3, [r6, #24]
.L_080e95e8:
	cmp	r3, #20
	bne.n	.L_080e95f8
	ldrh	r3, [r5, #8]
	mov	r1, sl
	ands	r3, r1
	orrs	r3, r2
	strh	r3, [r5, #8]
	ldr	r3, [r6, #24]
.L_080e95f8:
	cmp	r3, #22
	bne.n	.L_080e9606
	ldrh	r3, [r5, #8]
	mov	r0, sl
	ands	r3, r0
	orrs	r3, r7
	strh	r3, [r5, #8]
.L_080e9606:
	adds	r0, r5, #0
	adds	r1, r6, #0
	str	r2, [sp, #8]
	str	r4, [sp, #4]
	bl	sub_080eb298
	ldr	r3, [r6, #24]
	ldr	r4, [sp, #4]
	adds	r3, #1
	str	r3, [r6, #24]
	ldr	r2, [sp, #8]
.L_080e961c:
	movs	r1, #1
	negs	r1, r1
	add	r9, r1
	mov	r3, r9
	adds	r5, #40
	adds	r6, #28
	cmp	r3, #0
	bge.n	.L_080e95c0
	ldr	r0, [sp, #76]
	cmp	r0, #0
	beq.n	sub_080e9688
	mov	r1, fp
	cmp	r1, #0
	beq.n	sub_080e9688
	ldr	r2, [sp, #52]
	cmp	r2, #0
	beq.n	sub_080e9688
	ldrb	r2, [r1, #20]
	ldrb	r3, [r1, #21]
	adds	r1, r3, #0
	muls	r1, r2
	mov	r3, fp
	ldrb	r0, [r3, #16]
	movs	r2, #0
	bl	sub_080142d4
	ldr	r1, [pc, #360]
	lsls	r0, r0, #5
	adds	r5, r0, r1
	ldr	r0, [sp, #56]
	movs	r1, #3
	bl	sub_08002064
	cmp	r0, #0
	bne.n	.L_080e966e
	mov	r2, fp
	ldrb	r1, [r2, #20]
	ldr	r0, [sp, #20]
	ldrb	r2, [r2, #21]
	bl	.L_080e9250
.L_080e966e:
	ldr	r3, [sp, #56]
	mov	r0, fp
	adds	r3, #1
	str	r3, [sp, #56]
	movs	r3, #192
	lsls	r3, r3, #18
	ldrb	r1, [r0, #20]
	ldrb	r2, [r0, #21]
	ldr	r4, [r3, #84]
	ldr	r0, [sp, #20]
	adds	r3, r5, #0
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x9904
	movs	r0, #2
	adds	r2, r1, #0
	adds	r2, #1
	str	r2, [sp, #16]
	ldr	r2, [sp, #12]
	bl	sub_080e8b44
	ldr	r3, [sp, #64]
	cmp	r3, #1
	beq.n	.L_080e96e4
	cmp	r3, #1
	bgt.n	.L_080e96a8
	cmp	r3, #0
	beq.n	.L_080e96b4
	b.n	.L_080e9776
.L_080e96a8:
	ldr	r0, [sp, #64]
	cmp	r0, #2
	beq.n	.L_080e972c
	cmp	r0, #3
	beq.n	.L_080e9764
	b.n	.L_080e9776
.L_080e96b4:
	ldr	r1, [sp, #60]
	cmp	r1, #40
	bne.n	.L_080e96c4
	ldr	r2, [sp, #36]
	cmp	r2, #0
	beq.n	.L_080e96c4
	movs	r3, #1
	str	r3, [sp, #48]
.L_080e96c4:
	ldr	r0, [sp, #12]
	movs	r1, #200
	lsls	r1, r1, #5
	ldr	r2, [pc, #244]
	adds	r1, #153
	adds	r0, r0, r1
	str	r0, [sp, #12]
	cmp	r0, r2
	ble.n	.L_080e9776
	ldr	r3, [sp, #64]
	movs	r0, #1
	adds	r3, #1
	negs	r0, r0
	str	r3, [sp, #64]
	str	r0, [sp, #60]
	b.n	.L_080e9776
.L_080e96e4:
	ldr	r1, [sp, #40]
	cmp	r1, #0
	bne.n	.L_080e970e
	ldr	r2, [sp, #60]
	cmp	r2, #50
	bne.n	.L_080e96fa
	movs	r0, #1
	movs	r3, #2
	negs	r0, r0
	str	r3, [sp, #64]
	str	r0, [sp, #60]
.L_080e96fa:
	ldr	r1, [sp, #24]
	cmp	r1, #0
	beq.n	.L_080e9776
	ldr	r3, [r1, #8]
	movs	r0, #0
	ldr	r1, [sp, #60]
	ldr	r2, [sp, #28]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0xe033
.L_080e970e:
	mov	r3, fp
	ldrb	r2, [r3, #21]
	ldr	r0, [sp, #60]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	cmp	r0, r3
	bne.n	.L_080e9726
	movs	r2, #1
	movs	r1, #2
	negs	r2, r2
	str	r1, [sp, #64]
	str	r2, [sp, #60]
.L_080e9726:
	movs	r3, #1
	str	r3, [sp, #52]
	b.n	.L_080e9776
.L_080e972c:
	ldr	r0, [sp, #40]
	cmp	r0, #1
	bne.n	.L_080e9736
	movs	r1, #0
	str	r1, [sp, #52]
.L_080e9736:
	ldr	r2, [sp, #60]
	cmp	r2, #5
	bne.n	.L_080e9746
	ldr	r3, [sp, #36]
	cmp	r3, #0
	beq.n	.L_080e9746
	movs	r0, #0
	str	r0, [sp, #48]
.L_080e9746:
	ldr	r1, [sp, #12]
	ldr	r2, [pc, #120]
	movs	r3, #128
	adds	r1, r1, r2
	lsls	r3, r3, #10
	str	r1, [sp, #12]
	cmp	r1, r3
	bgt.n	.L_080e9776
	ldr	r0, [sp, #64]
	movs	r1, #1
	adds	r0, #1
	negs	r1, r1
	str	r0, [sp, #64]
	str	r1, [sp, #60]
	b.n	.L_080e9776
.L_080e9764:
	ldr	r3, [sp, #60]
	movs	r2, #0
	str	r2, [sp, #12]
	cmp	r3, #0
	bne.n	.L_080e9776
	movs	r0, #186
	lsls	r0, r0, #2
	adds	r0, #255
	str	r0, [sp, #64]
.L_080e9776:
	movs	r0, #1
	bl	sub_08013560
	ldr	r1, [sp, #60]
	movs	r3, #186
	ldr	r2, [sp, #64]
	lsls	r3, r3, #2
	adds	r1, #1
	adds	r3, #255
	str	r1, [sp, #60]
	cmp	r2, r3
	beq.n	.L_080e9790
	b.n	.L_080e9452
.L_080e9790:
	movs	r0, #195
	lsls	r0, r0, #1
	bl	sub_081c0010
	bl	sub_080e8c9c
	bl	sub_080dc384
	ldr	r0, [sp, #72]
	bl	sub_08014274
.L_080e97a6:
	movs	r0, #92
	bl	sub_0801314c
	add	sp, #88
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x06010000
	.4byte 0x000bffff
	.2byte 0xd99a
	.2byte 0xffff
	.2byte 0xb5e0
	mov	r7, r8
	push	{r7}
	movs	r1, #201
	lsls	r1, r1, #5
	movs	r0, #92
	bl	sub_08014cc0
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #224
	ldr	r5, [r3, #0]
	bl	sub_080dc294
	ldr	r0, [r5, #16]
	movs	r1, #0
	bl	sub_080e1420
	movs	r0, #2
	bl	sub_080e89e4
	movs	r0, #10
	bl	sub_08013560
	movs	r6, #128
	movs	r3, #0
	mov	r8, r3
	lsls	r6, r6, #10
	movs	r5, #0
	movs	r7, #0
.L_080e9804:
	mov	r1, r8
	movs	r3, #1
	movs	r0, #2
	adds	r2, r6, #0
	add	r8, r3
	bl	sub_080e8b44
	cmp	r5, #1
	beq.n	.L_080e9840
	cmp	r5, #1
	bgt.n	.L_080e9820
	cmp	r5, #0
	beq.n	.L_080e982a
	b.n	.L_080e986c
.L_080e9820:
	cmp	r5, #2
	beq.n	.L_080e984c
	cmp	r5, #3
	beq.n	.L_080e9860
	b.n	.L_080e986c
.L_080e982a:
	movs	r3, #152
	lsls	r3, r3, #6
	adds	r3, #102
	adds	r6, r6, r3
	ldr	r3, [pc, #96]
	cmp	r6, r3
	ble.n	.L_080e986c
	movs	r7, #1
	movs	r5, #1
	negs	r7, r7
	b.n	.L_080e986c
.L_080e9840:
	cmp	r7, #10
	bne.n	.L_080e986c
	movs	r7, #1
	movs	r5, #2
	negs	r7, r7
	b.n	.L_080e986c
.L_080e984c:
	ldr	r3, [pc, #72]
	adds	r6, r6, r3
	movs	r3, #128
	lsls	r3, r3, #10
	cmp	r6, r3
	bgt.n	.L_080e986c
	movs	r7, #1
	movs	r5, #3
	negs	r7, r7
	b.n	.L_080e986c
.L_080e9860:
	movs	r6, #0
	cmp	r7, #0
	bne.n	.L_080e986c
	movs	r5, #186
	lsls	r5, r5, #2
	adds	r5, #255
.L_080e986c:
	movs	r0, #1
	bl	sub_08013560
	movs	r3, #186
	lsls	r3, r3, #2
	adds	r3, #255
	adds	r7, #1
	cmp	r5, r3
	bne.n	.L_080e9804
	bl	sub_080e8c9c
	bl	sub_080dc384
	movs	r0, #92
	bl	sub_0801314c
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x0005ffff
	.2byte 0xd99a
	.2byte 0xffff
.L_080e989c:
	.2byte 0xb5e0
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	adds	r6, r1, #0
	adds	r0, #12
	movs	r1, #24
	bl	sub_08002064
	adds	r5, r0, #0
	ldr	r2, [pc, #136]
	lsls	r5, r5, #18
	asrs	r5, r5, #16
	adds	r0, r5, #0
	movs	r1, #96
	mov	r8, r2
	bl	sub_08002064
	lsls	r0, r0, #16
	mov	r2, r8
	asrs	r0, r0, #16
	ldrb	r3, [r2, r0]
	subs	r6, #7
	lsls	r6, r6, #16
	asrs	r6, r6, #16
	adds	r3, r3, r6
	adds	r0, r5, #0
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	movs	r1, #96
	adds	r0, #32
	mov	sl, r3
	bl	sub_08002064
	mov	r2, r8
	ldrb	r3, [r2, r0]
	adds	r5, #64
	adds	r3, r3, r6
	lsls	r3, r3, #16
	adds	r0, r5, #0
	movs	r1, #96
	asrs	r7, r3, #16
	bl	sub_08002064
	mov	r2, r8
	ldrb	r3, [r2, r0]
	adds	r3, r3, r6
	lsls	r3, r3, #16
	asrs	r0, r3, #16
	mov	r3, sl
	cmp	r3, #0
	bge.n	.L_080e9908
	movs	r2, #0
	mov	sl, r2
.L_080e9908:
	mov	r3, sl
	cmp	r3, #31
	ble.n	.L_080e9912
	movs	r2, #31
	mov	sl, r2
.L_080e9912:
	cmp	r7, #0
	bge.n	.L_080e9918
	movs	r7, #0
.L_080e9918:
	cmp	r7, #31
	ble.n	.L_080e991e
	movs	r7, #31
.L_080e991e:
	cmp	r0, #0
	bge.n	.L_080e9924
	movs	r0, #0
.L_080e9924:
	cmp	r0, #31
	ble.n	.L_080e992a
	movs	r0, #31
.L_080e992a:
	lsls	r3, r7, #5
	lsls	r0, r0, #10
	orrs	r0, r3
	mov	r3, sl
	orrs	r0, r3
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0fe0
	.2byte 0x080f
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	movs	r5, #192
	lsls	r5, r5, #18
	sub	sp, #12
	ldr	r7, [r5, #92]
	bl	sub_080cdf5c
	bl	sub_080cad84
	mov	sl, r0
	ldr	r5, [r5, #108]
	mov	r3, sl
	mov	r2, sl
	adds	r3, #34
	ldr	r1, [r2, #16]
	ldr	r0, [r0, #8]
	ldrb	r2, [r3, #0]
	mov	r8, r5
	bl	sub_080dbcc0
	cmp	r0, #0
	bne.n	.L_080e9992
	movs	r3, #208
	lsls	r3, r3, #4
	adds	r3, #70
	add	r3, r8
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	beq.n	.L_080e999e
	movs	r2, #208
	lsls	r2, r2, #4
	adds	r2, #68
	add	r2, r8
	ldrh	r3, [r2, #0]
	subs	r3, #1
	b.n	.L_080e999c
.L_080e9992:
	movs	r2, #208
	lsls	r2, r2, #4
	adds	r2, #68
	add	r2, r8
	movs	r3, #112
.L_080e999c:
	strh	r3, [r2, #0]
.L_080e999e:
	movs	r2, #128
	lsls	r2, r2, #5
	adds	r2, #52
	adds	r3, r7, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #1
	bne.n	.L_080e99f0
	movs	r3, #208
	lsls	r3, r3, #4
	adds	r3, #68
	add	r3, r8
	movs	r2, #0
	ldrsh	r6, [r3, r2]
	cmp	r6, #112
	ble.n	.L_080e99c0
	movs	r6, #112
.L_080e99c0:
	cmp	r6, #0
	bge.n	.L_080e99c6
	movs	r6, #0
.L_080e99c6:
	movs	r3, #128
	lsls	r3, r3, #5
	adds	r3, #40
	lsls	r0, r6, #15
	movs	r1, #112
	adds	r5, r7, r3
	bl	sub_08002054
	movs	r2, #128
	lsls	r2, r2, #8
	adds	r0, r0, r2
	str	r0, [r5, #0]
	cmp	r6, #0
	bne.n	.L_080e99f0
	movs	r2, #179
	movs	r3, #128
	lsls	r2, r2, #1
	lsls	r3, r3, #6
	add	r2, r8
	adds	r3, #153
	strh	r3, [r2, #0]
.L_080e99f0:
	ldr	r6, [pc, #188]
	movs	r5, #0
.L_080e99f4:
	movs	r2, #128
	lsls	r2, r2, #5
	adds	r2, #50
	adds	r3, r7, r2
	ldrh	r0, [r3, #0]
	lsrs	r3, r5, #31
	lsls	r0, r0, #16
	adds	r3, r5, r3
	asrs	r3, r3, #1
	asrs	r0, r0, #18
	movs	r1, #24
	adds	r0, r0, r3
	bl	sub_08002064
	adds	r1, r5, #0
	cmp	r5, #15
	bne.n	.L_080e9a18
	movs	r1, #14
.L_080e9a18:
	bl	.L_080e989c
	strh	r0, [r6, #0]
	adds	r6, #2
	adds	r5, #1
	cmp	r5, #15
	ble.n	.L_080e99f4
	movs	r3, #128
	lsls	r3, r3, #5
	adds	r3, #50
	adds	r2, r7, r3
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	movs	r2, #128
	lsls	r2, r2, #5
	adds	r2, #40
	adds	r3, r7, r2
	ldr	r3, [r3, #0]
	mov	r9, r3
	lsls	r3, r3, #1
	add	r3, r9
	lsls	r3, r3, #1
	cmp	r3, #0
	bge.n	.L_080e9a4c
	adds	r3, #7
.L_080e9a4c:
	asrs	r3, r3, #3
	mov	r8, r3
	movs	r3, #128
	lsls	r3, r3, #5
	adds	r6, r7, r3
	ldr	r3, [pc, #92]
	movs	r2, #128
	ldr	r3, [r3, #0]
	lsls	r2, r2, #5
	adds	r2, #46
	adds	r1, r7, r2
	lsrs	r3, r3, #1
	movs	r2, #1
	ands	r3, r2
	ldrh	r2, [r1, #0]
	lsls	r3, r3, #5
	adds	r2, r2, r3
	ldr	r3, [pc, #60]
	ldrh	r1, [r6, #8]
	ands	r2, r3
	ldr	r3, [pc, #64]
	mov	r0, sl
	ands	r3, r1
	orrs	r3, r2
	strh	r3, [r6, #8]
	bl	sub_080db9c0
	ldrb	r2, [r6, #9]
	movs	r3, #3
	ands	r0, r3
	movs	r3, #13
	negs	r3, r3
	ands	r3, r2
	lsls	r0, r0, #2
	orrs	r3, r0
	strb	r3, [r6, #9]
	mov	r0, sl
	bl	sub_080db9cc
	subs	r0, #1
	strh	r0, [r6, #30]
	mov	r2, sl
	ldr	r3, [r2, #8]
	mov	r5, sp
	str	r3, [r5, #0]
	adds	r0, r5, #0
	b.n	.L_080e9abc
	movs	r0, r0
	.4byte 0x000003ff
	.4byte 0x050003c0
	.4byte 0x0300122c
	.2byte 0xfc00
	.2byte 0xffff
.L_080e9abc:
	ldr	r3, [r2, #12]
	str	r3, [r5, #4]
	ldr	r3, [r2, #16]
	str	r3, [r5, #8]
	bl	sub_080dc390
	ldr	r3, [r5, #0]
	mov	r2, r8
	str	r3, [r6, #12]
	adds	r0, r6, #0
	ldr	r3, [r5, #8]
	str	r2, [r6, #24]
	str	r3, [r6, #16]
	mov	r3, r9
	str	r3, [r6, #20]
	bl	sub_080eb01c
	add	sp, #12
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.align 2, 0
