.syntax unified
	.thumb
	.set sub_080022ec, 0x080022ec
	.set sub_08002dd8, 0x08002dd8
	.set sub_08002f40, 0x08002f40
	.set sub_080030f8, 0x080030f8
	.set sub_080041d8, 0x080041d8
	.set sub_08004278, 0x08004278
	.set sub_08005340, 0x08005340
	.set sub_080072f4, 0x080072f4
	.set sub_080072fc, 0x080072fc
	.set sub_080b50e8, 0x080b50e8
	.set sub_080cd52c, 0x080cd52c
	.set sub_080cd594, 0x080cd594
	.set sub_080cdbc0, 0x080cdbc0
	.set sub_080d6888, 0x080d6888
	.set sub_080e155c, 0x080e155c
	.set sub_080e3980, 0x080e3980
	.set sub_080ed408, 0x080ed408
	.set sub_080f9010, 0x080f9010
	.global Func_080ceff8
	.thumb_func
Func_080ceff8:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #148]
	ldmia	r3!, {r2}
	ldr	r7, [pc, #148]
	ldr	r3, [r3, #0]
	mov	r8, r2
	sub	sp, #32
	add	r7, r8
	str	r3, [sp, #16]
	str	r0, [r7, #0]
	movs	r0, #0
	mov	sl, r1
	bl	sub_080cd594
	ldr	r0, [pc, #128]
	bl	sub_08002f40
	adds	r5, r0, #0
	movs	r0, #160
	movs	r2, #128
	adds	r1, r5, #0
	ldr	r6, [pc, #120]
	adds	r5, #128
	lsls	r0, r0, #19
	bl	sub_080072fc
	mov	r1, r8
	adds	r0, r5, #0
	bl	sub_08005340
	ldr	r0, [pc, #104]
	bl	sub_08002f40
	adds	r5, r0, #0
	movs	r0, #160
	lsls	r0, r0, #19
	adds	r1, r5, #0
	movs	r2, #128
	bl	sub_080072fc
	mov	r3, sl
	cmp	r3, #2
	bne.n	.L_080cf06e
	ldr	r0, [pc, #84]
	bl	sub_08002f40
	adds	r5, r0, #0
	movs	r0, #160
	lsls	r0, r0, #19
	adds	r1, r5, #0
	movs	r2, #128
	bl	sub_080072fc
.L_080cf06e:
	movs	r2, #20
	ldr	r3, [r7, #0]
	add	r2, sp
	mov	r9, r2
	movs	r4, #36
	ldrsh	r0, [r3, r4]
	mov	r1, r9
	bl	sub_080e3980
	mov	r3, sl
	cmp	r3, #0
	bne.n	.L_080cf0bc
	ldr	r2, [pc, #44]
	ldr	r3, [pc, #12]
	mov	r4, r9
	strh	r3, [r2, #0]
	ldr	r2, [r4, #0]
	movs	r3, #64
	subs	r0, r3, r2
	ldr	r2, [pc, #32]
	b.n	.L_080cf0d4
	.4byte 0x00000100
	.4byte 0x03001eec
	.4byte 0x00007828
	.4byte 0x0000007b
	.4byte 0x03001388
	.4byte 0x0000008d
	.4byte 0x00000068
	.4byte 0x04000020
	.2byte 0x0028
	.2byte 0x0400
.L_080cf0bc:
	ldr	r3, [pc, #60]
	ldr	r2, [pc, #64]
	strh	r3, [r2, #0]
	mov	r2, r9
	ldr	r0, [r2, #0]
	negs	r0, r0
	lsls	r0, r0, #2
	movs	r1, #5
	bl	sub_080022ec
	ldr	r2, [pc, #48]
	adds	r0, #64
.L_080cf0d4:
	lsls	r3, r0, #8
	str	r3, [r2, #0]
	movs	r6, #2
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	movs	r0, #46
	str	r6, [sp, #0]
	bl	sub_080ed408
	ldr	r5, [pc, #28]
	adds	r3, r5, #0
	adds	r3, #184
	ldr	r3, [r3, #0]
	movs	r1, #7
	str	r3, [sp, #8]
	movs	r2, #7
	movs	r3, #7
	b.n	.L_080cf10c
	movs	r0, r0
	.4byte 0x000000cc
	.4byte 0x04000020
	.4byte 0x04000028
	.2byte 0x1e50
	.2byte 0x0300
.L_080cf10c:
	movs	r0, #47
	str	r6, [sp, #0]
	bl	sub_080ed408
	movs	r3, #239
	lsls	r3, r3, #7
	ldr	r2, [pc, #356]
	add	r3, r8
	adds	r5, #188
	str	r6, [r3, #0]
	ldr	r5, [r5, #0]
	movs	r3, #50
	add	r2, r8
	movs	r1, #144
	str	r3, [r2, #0]
	ldr	r0, [pc, #344]
	lsls	r1, r1, #3
	str	r5, [sp, #12]
	bl	sub_080041d8
	mov	r3, sl
	cmp	r3, #2
	bne.n	.L_080cf140
	ldr	r2, [pc, #332]
	movs	r3, #0
	b.n	.L_080cf14a
.L_080cf140:
	mov	r4, sl
	cmp	r4, #1
	bne.n	.L_080cf156
	ldr	r2, [pc, #320]
	movs	r3, #8
.L_080cf14a:
	add	r2, r8
	str	r3, [r2, #0]
	movs	r0, #212
	bl	sub_080f9010
	b.n	.L_080cf15e
.L_080cf156:
	ldr	r2, [pc, #304]
	movs	r3, #32
	add	r2, r8
	str	r3, [r2, #0]
.L_080cf15e:
	ldr	r2, [pc, #300]
	movs	r7, #0
	mov	fp, r2
.L_080cf164:
	cmp	r7, #0
	bne.n	.L_080cf1a4
	mov	r3, sl
	cmp	r3, #2
	bne.n	.L_080cf18a
	ldr	r3, [pc, #288]
	add	r3, r8
	ldr	r3, [r3, #0]
	movs	r2, #1
	movs	r4, #36
	ldrsh	r0, [r3, r4]
	movs	r3, #32
	str	r3, [sp, #0]
	movs	r1, #7
	negs	r2, r2
	movs	r3, #0
	bl	sub_080d6888
	b.n	.L_080cf1a4
.L_080cf18a:
	ldr	r3, [pc, #260]
	add	r3, r8
	ldr	r3, [r3, #0]
	movs	r2, #36
	ldrsh	r0, [r3, r2]
	movs	r3, #32
	movs	r2, #1
	str	r3, [sp, #0]
	movs	r1, #10
	negs	r2, r2
	movs	r3, #0
	bl	sub_080d6888
.L_080cf1a4:
	cmp	r7, #24
	bne.n	.L_080cf1ae
	movs	r0, #0
	bl	sub_080b50e8
.L_080cf1ae:
	cmp	r7, #8
	bne.n	.L_080cf1be
	mov	r3, sl
	cmp	r3, #0
	bne.n	.L_080cf1be
	movs	r0, #126
	bl	sub_080f9010
.L_080cf1be:
	cmp	r7, #31
	bgt.n	.L_080cf226
	adds	r3, r7, #0
	cmp	r7, #0
	bge.n	.L_080cf1ca
	adds	r3, r7, #3
.L_080cf1ca:
	asrs	r6, r3, #2
	cmp	r6, #2
	ble.n	.L_080cf1d6
	movs	r3, #1
	ands	r3, r6
	adds	r6, r3, #1
.L_080cf1d6:
	cmp	r7, #27
	bgt.n	.L_080cf226
	mov	r3, fp
	ldrb	r0, [r3, r6]
	ldr	r4, [pc, #180]
	ldr	r2, [pc, #180]
	mov	r3, r9
	ldr	r3, [r3, #4]
	lsls	r5, r6, #1
	ldrh	r1, [r4, r5]
	ldrb	r4, [r2, r6]
	subs	r3, r3, r0
	movs	r2, #64
	subs	r2, r2, r4
	add	r1, r8
	adds	r3, #8
	str	r4, [sp, #0]
	str	r0, [sp, #4]
	ldr	r4, [sp, #8]
	ldr	r0, [sp, #16]
	bl	sub_080072f4
	mov	r3, fp
	mov	r4, r9
	ldrb	r0, [r3, r6]
	ldr	r2, [pc, #136]
	ldr	r3, [r4, #4]
	ldr	r4, [pc, #136]
	ldrh	r1, [r2, r5]
	ldrb	r2, [r4, r6]
	subs	r3, r3, r0
	str	r2, [sp, #0]
	str	r0, [sp, #4]
	add	r1, r8
	adds	r3, #8
	ldr	r0, [sp, #16]
	movs	r2, #64
	ldr	r4, [sp, #12]
	bl	sub_080072f4
.L_080cf226:
	mov	r2, sl
	cmp	r2, #0
	bne.n	.L_080cf236
	movs	r0, #2
	movs	r1, #2
	bl	sub_080e155c
	b.n	.L_080cf23e
.L_080cf236:
	movs	r0, #16
	movs	r1, #16
	bl	sub_080e155c
.L_080cf23e:
	bl	sub_080cd52c
	ldr	r2, [pc, #88]
	movs	r3, #1
	add	r2, r8
	str	r3, [r2, #0]
	movs	r0, #1
	adds	r7, #1
	bl	sub_080030f8
	cmp	r7, #48
	bne.n	.L_080cf164
	ldr	r0, [pc, #44]
	bl	sub_08004278
	movs	r0, #47
	bl	sub_08002dd8
	movs	r0, #46
	bl	sub_08002dd8
	bl	sub_080cdbc0
	add	sp, #32
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x00007784
	.4byte 0x080cd261
	.4byte 0x000077a8
	.4byte 0x080ee09f
	.4byte 0x00007828
	.4byte 0x080ee096
	.4byte 0x080ee09c
	.4byte 0x00007824
