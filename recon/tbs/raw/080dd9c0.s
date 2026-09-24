.syntax unified
	.thumb
	.set sub_08002dd8, 0x08002dd8
	.set sub_080030f8, 0x080030f8
	.set sub_080041d8, 0x080041d8
	.set sub_08004278, 0x08004278
	.set sub_08004458, 0x08004458
	.set sub_080072f4, 0x080072f4
	.set sub_080072fc, 0x080072fc
	.set sub_080b50e8, 0x080b50e8
	.set sub_080cd52c, 0x080cd52c
	.set sub_080cd594, 0x080cd594
	.set sub_080cdbc0, 0x080cdbc0
	.set sub_080d6888, 0x080d6888
	.set sub_080e0524, 0x080e0524
	.set sub_080e155c, 0x080e155c
	.set sub_080ed408, 0x080ed408
	.set sub_080f9010, 0x080f9010
	.global FunctionHead_080dd9c0
	.global Func_080dd9c0
	.thumb_func
FunctionHead_080dd9c0:
Func_080dd9c0:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r7, [pc, #84]
	adds	r3, r7, #0
	ldmia	r3!, {r1}
	ldr	r6, [pc, #80]
	mov	r9, r1
	ldr	r3, [r3, #0]
	sub	sp, #52
	add	r6, r9
	str	r3, [sp, #48]
	str	r0, [r6, #0]
	movs	r0, #1
	bl	sub_080cd594
	ldr	r2, [pc, #64]
	ldr	r3, [pc, #44]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #44]
	adds	r2, #48
	strh	r3, [r2, #0]
	ldr	r3, [pc, #40]
	adds	r2, #2
	strh	r3, [r2, #0]
	ldr	r0, [pc, #52]
	movs	r3, #1
	mov	r1, r9
	movs	r2, #1
	bl	sub_080e0524
	ldr	r3, [r6, #0]
	ldr	r3, [r3, #4]
	cmp	r3, #1
	bne.n	.L_080dda3c
	ldr	r2, [pc, #36]
	ldr	r3, [pc, #36]
	str	r3, [r2, #0]
	b.n	.L_080dda3c
	movs	r0, r0
	.4byte 0x00000100
	.4byte 0x00000000
	.4byte 0x00001010
	.4byte 0x03001eec
	.4byte 0x00007828
	.4byte 0x04000020
	.4byte 0x0000007e
	.4byte 0x04000028
	.2byte 0x9000
	.2byte 0xffff
.L_080dda3c:
	.2byte 0x2501
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	movs	r0, #46
	str	r5, [sp, #0]
	bl	sub_080ed408
	ldr	r2, [r7, #28]
	movs	r1, #7
	str	r2, [sp, #36]
	movs	r3, #7
	movs	r2, #7
	movs	r0, #47
	str	r5, [sp, #0]
	bl	sub_080ed408
	movs	r3, #239
	lsls	r3, r3, #7
	ldr	r2, [pc, #176]
	add	r3, r9
	str	r5, [r3, #0]
	add	r2, r9
	movs	r3, #0
	ldr	r7, [r7, #32]
	movs	r1, #144
	str	r3, [r2, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #160]
	str	r7, [sp, #40]
	bl	sub_080041d8
	ldr	r3, [r6, #0]
	ldr	r2, [pc, #156]
	ldr	r3, [r3, #24]
	ldrb	r3, [r2, r3]
	lsls	r3, r3, #3
	adds	r3, #56
	str	r3, [sp, #32]
	movs	r3, #0
	mov	fp, r3
	movs	r1, #1
	movs	r2, #128
	ldr	r3, [pc, #140]
	negs	r1, r1
	lsls	r2, r2, #3
.L_080dda98:
	movs	r4, #1
	add	fp, r4
	str	r1, [r3, #0]
	adds	r3, #28
	cmp	fp, r2
	bne.n	.L_080dda98
	ldr	r0, [sp, #32]
	movs	r6, #0
	str	r6, [sp, #44]
	cmp	r0, #0
	bne.n	.L_080ddab0
	b.n	.L_080ddd90
.L_080ddab0:
	ldr	r1, [sp, #32]
	subs	r0, #64
	subs	r1, #16
	str	r0, [sp, #28]
	str	r1, [sp, #24]
.L_080ddaba:
	ldr	r2, [sp, #44]
	ldr	r3, [sp, #28]
	cmp	r2, r3
	bne.n	.L_080ddac8
	movs	r0, #132
	bl	sub_080b50e8
.L_080ddac8:
	ldr	r4, [sp, #44]
	ldr	r6, [sp, #24]
	cmp	r4, r6
	blt.n	.L_080ddae4
	ldr	r2, [pc, #80]
	ldr	r3, [pc, #56]
	strh	r3, [r2, #0]
	ldr	r0, [sp, #32]
	ldr	r2, [pc, #52]
	subs	r3, r0, r4
	ldr	r1, [pc, #72]
	subs	r3, #1
	orrs	r3, r2
	strh	r3, [r1, #0]
.L_080ddae4:
	ldr	r3, [pc, #68]
	add	r3, r9
	ldr	r3, [r3, #0]
	ldr	r2, [pc, #48]
	ldr	r3, [r3, #24]
	ldrb	r3, [r2, r3]
	movs	r1, #0
	mov	fp, r1
	cmp	r3, #0
	bne.n	.L_080ddafa
	b.n	.L_080ddcda
.L_080ddafa:
	ldr	r4, [pc, #52]
	ldr	r6, [sp, #44]
	movs	r3, #12
	movs	r0, #8
	subs	r6, #8
	str	r3, [sp, #20]
	str	r4, [sp, #16]
	str	r0, [sp, #12]
	b.n	.L_080ddb34
	.4byte 0x00003f44
	.4byte 0x00001000
	.4byte 0x00007784
	.4byte 0x080cd261
	.4byte 0x080eebb6
	.4byte 0x02010018
	.4byte 0x04000050
	.4byte 0x04000052
	.4byte 0x00007828
	.2byte 0xebae
	.2byte 0x080e
.L_080ddb34:
	mov	r8, r6
.L_080ddb36:
	ldr	r1, [sp, #44]
	ldr	r2, [sp, #12]
	cmp	r1, r2
	bgt.n	.L_080ddb40
	b.n	.L_080ddc6c
.L_080ddb40:
	ldr	r3, [pc, #628]
	mov	r4, fp
	ldrb	r2, [r3, r4]
	adds	r3, r2, #0
	cmp	r3, #1
	bhi.n	.L_080ddba6
	mov	r6, r8
	lsls	r3, r6, #1
	add	r3, r8
	lsls	r0, r6, #4
	lsls	r1, r3, #1
	cmp	r0, #80
	ble.n	.L_080ddb5c
	movs	r0, #80
.L_080ddb5c:
	cmp	r1, #30
	ble.n	.L_080ddb62
	movs	r1, #30
.L_080ddb62:
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080ddb88
	ldr	r3, [sp, #16]
	movs	r2, #0
	ldrsb	r2, [r3, r2]
	movs	r4, #108
	subs	r2, r2, r1
	movs	r1, #48
	subs	r3, r4, r0
	str	r1, [sp, #0]
	str	r0, [sp, #4]
	mov	r1, r9
	ldr	r0, [sp, #48]
	ldr	r6, [sp, #40]
	bl	sub_080072fc
	b.n	.L_080ddc02
.L_080ddb88:
	ldr	r3, [sp, #16]
	movs	r2, #0
	ldrsb	r2, [r3, r2]
	movs	r4, #108
	adds	r2, r2, r1
	movs	r1, #48
	subs	r3, r4, r0
	str	r1, [sp, #0]
	str	r0, [sp, #4]
	mov	r1, r9
	ldr	r0, [sp, #48]
	ldr	r6, [sp, #36]
	bl	sub_080072fc
	b.n	.L_080ddc02
.L_080ddba6:
	mov	r1, r8
	lsls	r0, r1, #3
	cmp	r0, #64
	ble.n	.L_080ddbb0
	movs	r0, #64
.L_080ddbb0:
	mov	r3, r8
	cmp	r3, #8
	ble.n	.L_080ddbb8
	movs	r1, #8
.L_080ddbb8:
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080ddbe2
	ldr	r4, [sp, #16]
	movs	r2, #0
	ldrsb	r2, [r4, r2]
	subs	r2, r2, r1
	movs	r1, #32
	str	r1, [sp, #0]
	movs	r1, #240
	movs	r6, #108
	lsls	r1, r1, #4
	subs	r3, r6, r0
	str	r0, [sp, #4]
	add	r1, r9
	ldr	r0, [sp, #48]
	ldr	r4, [sp, #40]
	bl	sub_080072f4
	b.n	.L_080ddc02
.L_080ddbe2:
	ldr	r6, [sp, #16]
	movs	r2, #0
	ldrsb	r2, [r6, r2]
	adds	r2, r2, r1
	movs	r1, #108
	subs	r3, r1, r0
	movs	r1, #32
	str	r1, [sp, #0]
	movs	r1, #240
	lsls	r1, r1, #4
	str	r0, [sp, #4]
	add	r1, r9
	ldr	r0, [sp, #48]
	ldr	r4, [sp, #36]
	bl	sub_080072f4
.L_080ddc02:
	ldr	r3, [sp, #12]
	ldr	r6, [sp, #44]
	adds	r3, #1
	cmp	r6, r3
	bne.n	.L_080ddc14
	ldr	r2, [pc, #428]
	movs	r3, #3
	add	r2, r9
	str	r3, [r2, #0]
.L_080ddc14:
	ldr	r3, [sp, #12]
	ldr	r0, [sp, #44]
	adds	r3, #3
	cmp	r0, r3
	bge.n	.L_080ddc6c
	bl	sub_08004458
	movs	r3, #31
	ands	r3, r0
	adds	r1, r3, #0
	ldr	r7, [pc, #404]
	adds	r1, #72
	movs	r6, #0
	b.n	.L_080ddc36
.L_080ddc30:
	adds	r7, r5, #0
	adds	r7, #28
	adds	r6, #1
.L_080ddc36:
	cmp	r6, #64
	beq.n	.L_080ddc6c
	adds	r5, r7, #0
	movs	r2, #1
	ldr	r3, [r5, #24]
	negs	r2, r2
	cmp	r3, r2
	bne.n	.L_080ddc30
	str	r1, [sp, #8]
	bl	sub_08004458
	ldr	r3, [sp, #16]
	movs	r2, #0
	ldrsb	r2, [r3, r2]
	movs	r3, #31
	ands	r3, r0
	adds	r2, r2, r3
	adds	r2, #32
	str	r2, [r5, #0]
	ldr	r1, [sp, #8]
	cmp	r2, #96
	ble.n	.L_080ddc66
	movs	r3, #96
	str	r3, [r5, #0]
.L_080ddc66:
	movs	r3, #0
	str	r1, [r7, #4]
	str	r3, [r7, #24]
.L_080ddc6c:
	ldr	r2, [pc, #340]
	add	r2, r9
	ldr	r3, [r2, #0]
	ldr	r3, [r3, #20]
	movs	r6, #0
	cmp	r3, #0
	beq.n	.L_080ddcac
	ldr	r4, [sp, #20]
	adds	r5, r2, #0
	mov	sl, r4
	movs	r7, #36
.L_080ddc82:
	ldr	r0, [sp, #44]
	cmp	r0, sl
	bne.n	.L_080ddca0
	movs	r0, #132
	bl	sub_080f9010
	ldr	r3, [r5, #0]
	ldrsh	r0, [r3, r7]
	movs	r3, #3
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #5
	adds	r3, r6, #0
	bl	sub_080d6888
.L_080ddca0:
	ldr	r3, [r5, #0]
	ldr	r3, [r3, #20]
	adds	r6, #1
	adds	r7, #2
	cmp	r6, r3
	bne.n	.L_080ddc82
.L_080ddcac:
	ldr	r3, [sp, #16]
	ldr	r2, [sp, #20]
	ldr	r6, [sp, #12]
	adds	r3, #1
	str	r3, [sp, #16]
	adds	r2, #8
	adds	r6, #8
	ldr	r3, [pc, #264]
	str	r2, [sp, #20]
	str	r6, [sp, #12]
	add	r3, r9
	ldr	r3, [r3, #0]
	ldr	r1, [pc, #256]
	ldr	r3, [r3, #24]
	movs	r4, #8
	movs	r0, #1
	ldrb	r3, [r1, r3]
	negs	r4, r4
	add	fp, r0
	add	r8, r4
	cmp	fp, r3
	beq.n	.L_080ddcda
	b.n	.L_080ddb36
.L_080ddcda:
	ldr	r3, [pc, #240]
	movs	r2, #0
	ldr	r7, [pc, #224]
	mov	fp, r2
	mov	sl, r3
.L_080ddce4:
	ldr	r3, [r7, #24]
	cmp	r3, #0
	blt.n	.L_080ddd5c
	lsrs	r5, r3, #31
	adds	r5, r3, r5
	asrs	r5, r5, #1
	ldr	r6, [pc, #220]
	ldr	r0, [pc, #224]
	lsls	r4, r5, #1
	mov	r3, sl
	ldrh	r1, [r6, r4]
	ldrsb	r6, [r0, r5]
	ldrb	r0, [r3, r5]
	lsls	r0, r0, #24
	mov	r8, r4
	asrs	r4, r0, #24
	lsrs	r0, r0, #31
	ldr	r2, [r7, #0]
	ldr	r3, [r7, #4]
	adds	r0, r4, r0
	asrs	r0, r0, #1
	subs	r2, r2, r6
	subs	r3, r3, r0
	str	r6, [sp, #0]
	str	r4, [sp, #4]
	add	r1, r9
	ldr	r4, [sp, #36]
	ldr	r0, [sp, #48]
	bl	sub_080072f4
	ldr	r6, [pc, #172]
	mov	r0, r8
	mov	r3, sl
	ldrh	r1, [r6, r0]
	ldrb	r0, [r3, r5]
	lsls	r0, r0, #24
	asrs	r4, r0, #24
	lsrs	r0, r0, #31
	ldr	r3, [r7, #4]
	adds	r0, r4, r0
	ldr	r6, [pc, #156]
	asrs	r0, r0, #1
	subs	r3, r3, r0
	ldrsb	r0, [r6, r5]
	ldr	r2, [r7, #0]
	add	r1, r9
	str	r0, [sp, #0]
	str	r4, [sp, #4]
	ldr	r0, [sp, #48]
	ldr	r4, [sp, #40]
	bl	sub_080072f4
	ldr	r3, [r7, #24]
	adds	r3, #1
	str	r3, [r7, #24]
	cmp	r3, #14
	bne.n	.L_080ddd5c
	movs	r3, #1
	negs	r3, r3
	str	r3, [r7, #24]
.L_080ddd5c:
	movs	r6, #1
	add	fp, r6
	mov	r0, fp
	adds	r7, #28
	cmp	r0, #64
	bne.n	.L_080ddce4
	movs	r1, #8
	movs	r0, #8
	bl	sub_080e155c
	bl	sub_080cd52c
	ldr	r2, [pc, #96]
	movs	r3, #1
	add	r2, r9
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_080030f8
	ldr	r1, [sp, #44]
	ldr	r2, [sp, #32]
	adds	r1, #1
	str	r1, [sp, #44]
	cmp	r1, r2
	beq.n	.L_080ddd90
	b.n	.L_080ddaba
.L_080ddd90:
	ldr	r0, [pc, #72]
	bl	sub_08004278
	movs	r0, #47
	bl	sub_08002dd8
	movs	r0, #46
	bl	sub_08002dd8
	bl	sub_080cdbc0
	add	sp, #52
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x080eeba6
	.4byte 0x000077a8
	.4byte 0x02010000
	.4byte 0x00007828
	.4byte 0x080eebb6
	.4byte 0x080eebc0
	.4byte 0x080eebc8
	.4byte 0x080eebb9
	.4byte 0x00007824
	.4byte 0x080cd261
