.syntax unified
	.thumb
	.set sub_08002304, 0x08002304
	.set sub_0800231c, 0x0800231c
	.set sub_08002322, 0x08002322
	.set sub_08002dd8, 0x08002dd8
	.set sub_080030f8, 0x080030f8
	.set sub_080041d8, 0x080041d8
	.set sub_08004278, 0x08004278
	.set sub_08004458, 0x08004458
	.set sub_080072f4, 0x080072f4
	.set sub_080072f8, 0x080072f8
	.set sub_080b5088, 0x080b5088
	.set sub_080b50e8, 0x080b50e8
	.set sub_080cd52c, 0x080cd52c
	.set sub_080cd594, 0x080cd594
	.set sub_080cdbc0, 0x080cdbc0
	.set sub_080d6888, 0x080d6888
	.set sub_080e0524, 0x080e0524
	.set sub_080e155c, 0x080e155c
	.set sub_080e3908, 0x080e3908
	.set sub_080e3980, 0x080e3980
	.set sub_080ed408, 0x080ed408
	.set sub_080f9010, 0x080f9010
	.global Region_080ddde0
	.global Func_080ddde0
	.thumb_func
Region_080ddde0:
Func_080ddde0:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r5, [pc, #668]
	adds	r3, r5, #0
	ldmia	r3!, {r1}
	ldr	r3, [r3, #0]
	sub	sp, #64
	str	r3, [sp, #48]
	ldr	r3, [pc, #660]
	mov	fp, r1
	ldr	r2, [r5, #8]
	add	r3, fp
	str	r2, [sp, #28]
	str	r0, [r3, #0]
	movs	r0, #1
	bl	sub_080cd594
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	movs	r0, #46
	bl	sub_080ed408
	ldr	r5, [r5, #28]
	ldr	r0, [pc, #628]
	mov	r1, fp
	movs	r2, #1
	movs	r3, #0
	str	r5, [sp, #32]
	bl	sub_080e0524
	ldr	r1, [pc, #616]
	ldr	r0, [pc, #620]
	add	r1, fp
	movs	r2, #1
	movs	r3, #1
	bl	sub_080e0524
	movs	r2, #0
	ldr	r1, [sp, #28]
	movs	r3, #0
	ldr	r0, [pc, #604]
	bl	sub_080e0524
	movs	r3, #0
	mov	sl, r3
	movs	r2, #128
	ldr	r3, [pc, #596]
	movs	r1, #0
	lsls	r2, r2, #3
.L_080dde52:
	movs	r4, #1
	add	sl, r4
	str	r1, [r3, #0]
	adds	r3, #28
	cmp	sl, r2
	bne.n	.L_080dde52
	ldr	r3, [pc, #584]
	movs	r0, #0
	movs	r2, #1
	mov	sl, r0
	negs	r2, r2
	add	r3, fp
.L_080dde6a:
	movs	r1, #1
	add	sl, r1
	mov	r4, sl
	str	r2, [r3, #0]
	adds	r3, #28
	cmp	r4, #64
	bne.n	.L_080dde6a
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, fp
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r2, [pc, #552]
	movs	r3, #75
	add	r2, fp
	movs	r1, #144
	str	r3, [r2, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #544]
	bl	sub_080041d8
	movs	r0, #138
	bl	sub_080f9010
	movs	r0, #0
	ldr	r3, [pc, #496]
	str	r0, [sp, #40]
	add	r3, fp
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #20]
	movs	r1, #40
	lsls	r3, r3, #3
	negs	r1, r1
	cmp	r3, r1
	bne.n	.L_080ddeb2
	b.n	.L_080de2a0
.L_080ddeb2:
	ldr	r2, [pc, #476]
	add	r2, fp
	str	r2, [sp, #20]
.L_080ddeb8:
	ldr	r3, [sp, #40]
	cmp	r3, #24
	bne.n	.L_080ddec4
	movs	r0, #133
	bl	sub_080b50e8
.L_080ddec4:
	movs	r4, #0
	str	r4, [sp, #44]
	ldr	r0, [sp, #20]
	ldr	r3, [r0, #0]
	ldr	r3, [r3, #20]
	cmp	r3, #0
	beq.n	.L_080ddefc
	ldr	r5, [pc, #480]
.L_080dded4:
	ldr	r1, [sp, #44]
	ldr	r2, [sp, #40]
	lsls	r3, r1, #3
	cmp	r2, r3
	bne.n	.L_080ddeea
	movs	r1, #128
	ldr	r0, [sp, #48]
	lsls	r1, r1, #7
	ldr	r2, [pc, #464]
	bl	sub_080072f8
.L_080ddeea:
	ldr	r3, [sp, #44]
	adds	r3, #1
	str	r3, [sp, #44]
	ldr	r4, [sp, #20]
	ldr	r3, [r4, #0]
	ldr	r0, [sp, #44]
	ldr	r3, [r3, #20]
	cmp	r0, r3
	bne.n	.L_080dded4
.L_080ddefc:
	movs	r1, #0
	str	r1, [sp, #44]
	ldr	r2, [pc, #396]
	mov	r4, fp
	ldr	r3, [r4, r2]
	ldr	r3, [r3, #20]
	cmp	r3, #0
	bne.n	.L_080ddf0e
	b.n	.L_080de1f0
.L_080ddf0e:
	mov	r0, sp
	adds	r0, #52
	movs	r1, #36
	movs	r3, #0
	str	r0, [sp, #24]
	str	r1, [sp, #16]
	str	r3, [sp, #12]
.L_080ddf1c:
	mov	r0, fp
	adds	r5, r0, r2
	ldr	r3, [r5, #0]
	ldr	r1, [sp, #16]
	ldr	r4, [sp, #44]
	ldrsh	r0, [r3, r1]
	lsls	r4, r4, #3
	ldr	r1, [sp, #24]
	mov	r8, r4
	bl	sub_080e3980
	ldr	r4, [sp, #24]
	ldr	r3, [r4, #0]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [r4, #0]
	ldr	r0, [sp, #40]
	mov	r3, r8
	adds	r3, #1
	cmp	r0, r3
	bne.n	.L_080ddf50
	ldr	r2, [pc, #368]
	movs	r3, #4
	add	r2, fp
	str	r3, [r2, #0]
.L_080ddf50:
	mov	r3, r8
	ldr	r1, [sp, #40]
	adds	r3, #4
	cmp	r1, r3
	bne.n	.L_080ddf7a
	ldr	r3, [r5, #0]
	ldr	r2, [sp, #16]
	ldrsh	r0, [r3, r2]
	movs	r3, #6
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #5
	ldr	r3, [sp, #44]
	bl	sub_080d6888
	ldr	r3, [r5, #0]
	ldr	r1, [sp, #16]
	ldrsh	r0, [r3, r1]
	movs	r1, #6
	bl	sub_080b5088
.L_080ddf7a:
	movs	r3, #2
	ldr	r4, [sp, #40]
	add	r3, r8
	mov	r9, r3
	cmp	r4, r8
	bge.n	.L_080ddf88
	b.n	.L_080de0da
.L_080ddf88:
	mov	r3, r8
	adds	r3, #16
	cmp	r4, r3
	blt.n	.L_080ddf92
	b.n	.L_080de0d4
.L_080ddf92:
	mov	r0, r8
	subs	r3, r4, r0
	lsls	r6, r3, #6
	cmp	r6, #104
	ble.n	.L_080ddf9e
	movs	r6, #104
.L_080ddf9e:
	ldr	r3, [r5, #0]
	ldr	r3, [r3, #24]
	ldr	r7, [pc, #284]
	lsls	r3, r3, #2
	adds	r3, #3
	adds	r2, r7, #0
	ldrb	r3, [r2, r3]
	movs	r1, #0
	mov	sl, r1
	cmp	r3, #0
	beq.n	.L_080de004
	ldr	r3, [sp, #44]
	ldr	r4, [sp, #40]
	mov	r9, r7
	adds	r5, r3, r4
.L_080ddfbc:
	mov	r0, sl
	adds	r3, r5, r0
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	movs	r2, #3
	ands	r3, r2
	lsls	r2, r3, #1
	adds	r2, r2, r3
	lsls	r1, r2, #4
	subs	r1, r1, r2
	lsls	r1, r1, #6
	ldr	r2, [pc, #192]
	add	r1, fp
	adds	r1, r1, r2
	ldr	r2, [sp, #52]
	movs	r3, #24
	subs	r2, #12
	str	r3, [sp, #0]
	str	r6, [sp, #4]
	movs	r3, #0
	ldr	r0, [sp, #48]
	ldr	r4, [sp, #32]
	bl	sub_080072f4
	ldr	r1, [sp, #20]
	ldr	r3, [r1, #0]
	ldr	r3, [r3, #24]
	lsls	r3, r3, #2
	adds	r3, #3
	mov	r2, r9
	movs	r0, #1
	ldrb	r3, [r2, r3]
	add	sl, r0
	cmp	sl, r3
	bne.n	.L_080ddfbc
.L_080de004:
	movs	r3, #2
	add	r3, r8
	ldr	r4, [sp, #40]
	mov	r9, r3
	cmp	r4, r9
	bne.n	.L_080de0da
	ldr	r1, [sp, #20]
	ldr	r3, [r1, #0]
	ldr	r3, [r3, #24]
	lsls	r3, r3, #2
	ldrb	r3, [r7, r3]
	movs	r0, #0
	mov	sl, r0
	cmp	r3, #0
	beq.n	.L_080de0da
	ldr	r2, [sp, #12]
	ldr	r3, [pc, #156]
	adds	r7, r2, r3
.L_080de028:
	bl	sub_08004458
	ldr	r6, [pc, #152]
	ands	r6, r0
	bl	sub_08004458
	ldr	r3, [sp, #52]
	ldr	r5, [pc, #148]
	ldr	r4, [pc, #148]
	lsls	r3, r3, #16
	str	r3, [r7, #0]
	ands	r5, r0
	movs	r3, #208
	adds	r5, r5, r4
	lsls	r3, r3, #15
	str	r3, [r7, #4]
	adds	r0, r5, #0
	bl	sub_08002322
	adds	r6, #64
	adds	r3, r6, #0
	muls	r3, r0
	asrs	r3, r3, #5
	str	r3, [r7, #12]
	adds	r0, r5, #0
	bl	sub_0800231c
	adds	r3, r6, #0
	muls	r3, r0
	negs	r3, r3
	asrs	r3, r3, #6
	str	r3, [r7, #16]
	bl	sub_08004458
	movs	r3, #7
	ands	r3, r0
	adds	r3, #32
	str	r3, [r7, #24]
	ldr	r1, [sp, #20]
	ldr	r3, [r1, #0]
	ldr	r3, [r3, #24]
	ldr	r2, [pc, #68]
	lsls	r3, r3, #2
	movs	r0, #1
	ldrb	r3, [r2, r3]
	add	sl, r0
	adds	r7, #28
	cmp	sl, r3
	bne.n	.L_080de028
	b.n	.L_080de0da
	.4byte 0x03001eec
	.4byte 0x00007828
	.4byte 0x000000ce
	.4byte 0x00000c56
	.4byte 0x000000c4
	.4byte 0x00000073
	.4byte 0x02010018
	.4byte 0x00007098
	.4byte 0x00007784
	.4byte 0x080cd261
	.4byte 0x03000168
	.4byte 0x10101010
	.4byte 0x000077a8
	.4byte 0x080eebd6
	.4byte 0x02010000
	.4byte 0x000001ff
	.4byte 0x00007fff
	.2byte 0xc000
	.2byte 0xffff
.L_080de0d4:
	.2byte 0x2302
	add	r3, r8
	mov	r9, r3
.L_080de0da:
	ldr	r4, [sp, #40]
	cmp	r4, r9
	blt.n	.L_080de1ca
	mov	r3, r8
	adds	r3, #24
	cmp	r4, r3
	bge.n	.L_080de1ca
	ldr	r1, [sp, #20]
	ldr	r3, [r1, #0]
	ldr	r3, [r3, #24]
	ldr	r2, [pc, #468]
	lsls	r3, r3, #2
	adds	r3, #1
	ldrb	r3, [r2, r3]
	movs	r0, #0
	mov	sl, r0
	cmp	r3, #0
	beq.n	.L_080de1ca
	ldr	r7, [pc, #456]
	movs	r3, #3
	mov	r9, r3
	add	r7, fp
.L_080de106:
	mov	r4, sl
	mov	r0, r9
	ands	r4, r0
	str	r4, [sp, #8]
	bl	sub_08004458
	ldr	r3, [r7, #0]
	ldr	r3, [r3, #24]
	ldr	r1, [pc, #428]
	lsls	r3, r3, #2
	adds	r3, #2
	ldrb	r5, [r1, r3]
	adds	r1, r5, #0
	bl	sub_08002304
	ldr	r2, [sp, #24]
	ldr	r2, [r2, #4]
	mov	r8, r2
	mov	r3, r8
	ldr	r4, [sp, #8]
	subs	r3, r3, r0
	subs	r5, r5, r0
	ldr	r0, [pc, #408]
	mov	r8, r3
	ldrb	r3, [r0, r4]
	mov	r1, r8
	lsrs	r3, r3, #1
	subs	r1, r1, r3
	movs	r2, #8
	mov	r8, r1
	add	r8, r2
	bl	sub_08004458
	adds	r5, #1
	ldr	r3, [sp, #24]
	adds	r1, r5, #0
	ldr	r6, [r3, #0]
	bl	sub_08002304
	ldr	r4, [sp, #8]
	adds	r6, r6, r0
	ldr	r0, [pc, #372]
	lsrs	r3, r5, #31
	adds	r5, r5, r3
	ldrb	r3, [r0, r4]
	asrs	r5, r5, #1
	lsrs	r3, r3, #1
	subs	r6, r6, r5
	subs	r6, r6, r3
	bl	sub_08004458
	ldr	r3, [pc, #356]
	mov	r1, r9
	ands	r0, r1
	ldrb	r2, [r3, r0]
	mov	r3, r9
	orrs	r3, r2
	ldr	r2, [r7, #0]
	ldr	r1, [pc, #348]
	ldr	r2, [r2, #24]
	ldrb	r2, [r1, r2]
	movs	r0, #47
	str	r2, [sp, #0]
	movs	r1, #7
	movs	r2, #7
	bl	sub_080ed408
	ldr	r4, [sp, #8]
	ldr	r2, [pc, #332]
	lsls	r3, r4, #1
	ldrh	r1, [r2, r3]
	ldr	r2, [pc, #312]
	ldrb	r3, [r2, r4]
	ldr	r0, [pc, #304]
	str	r3, [sp, #0]
	ldrb	r3, [r0, r4]
	ldr	r2, [pc, #320]
	str	r3, [sp, #4]
	add	r1, fp
	ldr	r4, [r2, #0]
	mov	r3, r8
	ldr	r0, [sp, #48]
	adds	r2, r6, #0
	bl	sub_080072f4
	movs	r0, #47
	bl	sub_08002dd8
	movs	r3, #1
	add	sl, r3
	ldr	r3, [r7, #0]
	ldr	r3, [r3, #24]
	ldr	r4, [pc, #260]
	lsls	r3, r3, #2
	adds	r3, #1
	ldrb	r3, [r4, r3]
	cmp	sl, r3
	bne.n	.L_080de106
.L_080de1ca:
	ldr	r0, [sp, #16]
	ldr	r1, [sp, #12]
	ldr	r3, [sp, #44]
	movs	r2, #224
	lsls	r2, r2, #4
	adds	r0, #2
	adds	r1, r1, r2
	adds	r3, #1
	str	r0, [sp, #16]
	str	r3, [sp, #44]
	str	r1, [sp, #12]
	ldr	r2, [pc, #228]
	mov	r4, fp
	ldr	r3, [r4, r2]
	ldr	r0, [sp, #44]
	ldr	r3, [r3, #20]
	cmp	r0, r3
	beq.n	.L_080de1f0
	b.n	.L_080ddf1c
.L_080de1f0:
	movs	r1, #0
	ldr	r6, [pc, #240]
	mov	sl, r1
.L_080de1f6:
	ldr	r3, [r6, #24]
	cmp	r3, #0
	ble.n	.L_080de262
	subs	r3, #1
	movs	r2, #128
	str	r3, [r6, #24]
	lsls	r2, r2, #5
	adds	r0, r6, #0
	movs	r1, #60
	bl	sub_080e3908
	movs	r2, #208
	ldr	r3, [r6, #4]
	lsls	r2, r2, #15
	cmp	r3, r2
	ble.n	.L_080de224
	ldr	r3, [r6, #16]
	negs	r3, r3
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [r6, #16]
	b.n	.L_080de262
.L_080de224:
	ldr	r2, [r6, #0]
	ldr	r4, [pc, #192]
	cmp	r2, r4
	bhi.n	.L_080de262
	cmp	r3, #0
	blt.n	.L_080de262
	ldr	r4, [r6, #24]
	cmp	r4, #0
	bge.n	.L_080de238
	adds	r4, #15
.L_080de238:
	asrs	r4, r4, #4
	adds	r4, #1
	lsls	r5, r4, #1
	ldr	r0, [pc, #172]
	subs	r1, r5, #2
	ldrh	r1, [r0, r1]
	ldr	r0, [sp, #28]
	adds	r1, r0, r1
	lsrs	r0, r4, #31
	adds	r0, r4, r0
	asrs	r0, r0, #1
	asrs	r2, r2, #16
	asrs	r3, r3, #16
	subs	r2, r2, r0
	subs	r3, r3, r4
	str	r4, [sp, #0]
	str	r5, [sp, #4]
	ldr	r0, [sp, #48]
	ldr	r4, [sp, #32]
	bl	sub_080072f4
.L_080de262:
	movs	r0, #1
	movs	r1, #128
	add	sl, r0
	lsls	r1, r1, #3
	adds	r6, #28
	cmp	sl, r1
	bne.n	.L_080de1f6
	movs	r0, #2
	movs	r1, #8
	bl	sub_080e155c
	bl	sub_080cd52c
	ldr	r2, [pc, #112]
	movs	r3, #1
	add	r2, fp
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_080030f8
	ldr	r2, [sp, #40]
	adds	r2, #1
	str	r2, [sp, #40]
	ldr	r4, [sp, #20]
	ldr	r3, [r4, #0]
	ldr	r3, [r3, #20]
	lsls	r3, r3, #3
	adds	r3, #40
	cmp	r2, r3
	beq.n	.L_080de2a0
	b.n	.L_080ddeb8
.L_080de2a0:
	ldr	r0, [pc, #80]
	bl	sub_08004278
	movs	r0, #46
	bl	sub_08002dd8
	bl	sub_080cdbc0
	add	sp, #64
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x080eebd6
	.4byte 0x00007828
	.4byte 0x080eded0
	.4byte 0x080edeca
	.4byte 0x080eebe2
	.4byte 0x080eebe6
	.4byte 0x080edebe
	.4byte 0x03001f0c
	.4byte 0x02010000
	.4byte 0x007effff
	.4byte 0x080ede48
	.4byte 0x00007824
	.4byte 0x080cd261
