.syntax unified
	.thumb
	.set sub_080022fc, 0x080022fc
	.set sub_0800231c, 0x0800231c
	.set sub_08002322, 0x08002322
	.set sub_08002dd8, 0x08002dd8
	.set sub_080030f8, 0x080030f8
	.set sub_080041d8, 0x080041d8
	.set sub_08004278, 0x08004278
	.set sub_08004458, 0x08004458
	.set sub_080049ac, 0x080049ac
	.set sub_080051d8, 0x080051d8
	.set sub_080072f4, 0x080072f4
	.set sub_080b5028, 0x080b5028
	.set sub_080b5070, 0x080b5070
	.set sub_080b5098, 0x080b5098
	.set sub_080b50e8, 0x080b50e8
	.set sub_080cd52c, 0x080cd52c
	.set sub_080cd594, 0x080cd594
	.set sub_080cdbc0, 0x080cdbc0
	.set sub_080d6888, 0x080d6888
	.set sub_080e0524, 0x080e0524
	.set sub_080e155c, 0x080e155c
	.set sub_080e3944, 0x080e3944
	.set sub_080ed408, 0x080ed408
	.set sub_080f9010, 0x080f9010
	.global Unnamed_080da6cc
	.global Func_080da6cc
	.thumb_func
Unnamed_080da6cc:
Func_080da6cc:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r2, [pc, #44]
	adds	r3, r2, #0
	ldmia	r3!, {r1}
	ldr	r3, [r3, #0]
	sub	sp, #56
	str	r3, [sp, #32]
	adds	r3, r2, #0
	subs	r3, #108
	ldr	r3, [r3, #0]
	str	r3, [sp, #24]
	ldr	r2, [r2, #8]
	ldr	r3, [pc, #24]
	str	r2, [sp, #20]
	mov	r9, r1
	add	r3, r9
	str	r0, [r3, #0]
	ldr	r3, [r0, #4]
	cmp	r3, #1
	bne.n	.L_080da710
	movs	r0, #1
	bl	sub_080cd594
	b.n	.L_080da716
	.4byte 0x03001eec
	.2byte 0x7828
	.2byte 0x0000
.L_080da710:
	movs	r0, #0
	bl	sub_080cd594
.L_080da716:
	ldr	r0, [pc, #128]
	mov	r1, r9
	movs	r2, #1
	movs	r3, #1
	bl	sub_080e0524
	ldr	r0, [pc, #120]
	ldr	r1, [sp, #20]
	movs	r2, #0
	movs	r3, #0
	bl	sub_080e0524
	movs	r3, #3
	movs	r1, #7
	movs	r2, #7
	movs	r0, #46
	str	r3, [sp, #0]
	bl	sub_080ed408
	ldr	r5, [pc, #96]
	adds	r3, r5, #0
	adds	r3, #184
	ldr	r3, [r3, #0]
	str	r3, [sp, #36]
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	movs	r0, #47
	bl	sub_080ed408
	adds	r5, #188
	ldr	r3, [r5, #0]
	mov	r2, sp
	adds	r2, #36
	ldr	r5, [pc, #68]
	str	r2, [sp, #12]
	str	r3, [r2, #4]
	ldr	r2, [pc, #64]
	ldr	r3, [pc, #44]
	add	r5, r9
	strh	r3, [r2, #0]
	ldr	r3, [r5, #0]
	ldr	r0, [r3, #8]
	bl	sub_080b5098
	ldr	r3, [r5, #0]
	ldr	r0, [r0, #0]
	mov	r8, r0
	ldr	r0, [r3, #8]
	bl	sub_080b5070
	mov	r5, r8
	ldr	r3, [r5, #12]
	adds	r3, r3, r0
	movs	r1, #127
	movs	r0, #0
	str	r3, [sp, #16]
	ldr	r7, [pc, #28]
	mov	sl, r0
	mov	fp, r1
	b.n	.L_080da7b0
	.4byte 0x00001010
	.4byte 0x000000b4
	.4byte 0x00000073
	.4byte 0x03001e50
	.4byte 0x00007828
	.4byte 0x04000052
	.2byte 0x0000
	.2byte 0x0201
.L_080da7b0:
	bl	sub_08004458
	adds	r6, r0, #0
	bl	sub_08004458
	mov	r2, fp
	adds	r5, r0, #0
	adds	r0, r6, #0
	ands	r5, r2
	bl	sub_08002322
	adds	r5, #127
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #6
	str	r3, [r7, #12]
	bl	sub_08004458
	mov	r3, fp
	ands	r0, r3
	subs	r0, #16
	lsls	r0, r0, #16
	asrs	r0, r0, #6
	str	r0, [r7, #16]
	adds	r0, r6, #0
	bl	sub_0800231c
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #6
	str	r3, [r7, #20]
	mov	r5, r8
	ldr	r3, [r5, #8]
	str	r3, [r7, #0]
	ldr	r0, [sp, #16]
	str	r0, [r7, #4]
	ldr	r3, [r5, #16]
	movs	r1, #1
	str	r3, [r7, #8]
	add	sl, r1
	movs	r3, #1
	negs	r3, r3
	mov	r2, sl
	str	r3, [r7, #24]
	adds	r7, #28
	cmp	r2, #64
	bne.n	.L_080da7b0
	ldr	r3, [pc, #804]
	movs	r2, #0
	add	r3, r9
	str	r2, [r3, #0]
	ldr	r3, [pc, #800]
	movs	r5, #144
	add	r3, r9
	lsls	r5, r5, #3
	str	r2, [r3, #0]
	adds	r1, r5, #0
	ldr	r0, [pc, #792]
	bl	sub_080041d8
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, r9
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r2, [pc, #780]
	movs	r3, #75
	add	r2, r9
	str	r3, [r2, #0]
	adds	r1, r5, #0
	ldr	r0, [pc, #772]
	bl	sub_080041d8
	movs	r3, #0
	str	r3, [sp, #28]
	ldr	r3, [pc, #768]
	add	r3, r9
	ldr	r3, [r3, #0]
	ldr	r2, [pc, #764]
	ldr	r3, [r3, #24]
	ldrb	r3, [r2, r3]
	movs	r5, #132
	lsrs	r3, r3, #1
	negs	r5, r5
	cmp	r3, r5
	bne.n	.L_080da85e
	b.n	.L_080dab06
.L_080da85e:
	ldr	r0, [sp, #24]
	ldr	r1, [pc, #740]
	adds	r0, #12
	add	r1, r9
	str	r0, [sp, #8]
	mov	fp, r1
.L_080da86a:
	ldr	r3, [sp, #28]
	subs	r3, #17
	cmp	r3, #62
	bhi.n	.L_080da87c
	ldr	r2, [pc, #704]
	movs	r3, #128
	add	r2, r9
	lsls	r3, r3, #1
	b.n	.L_080da882
.L_080da87c:
	ldr	r2, [pc, #692]
	movs	r3, #0
	add	r2, r9
.L_080da882:
	str	r3, [r2, #0]
	ldr	r5, [pc, #704]
	add	r5, r9
	ldr	r3, [r5, #0]
	ldr	r6, [pc, #704]
	ldr	r3, [r3, #24]
	ldrb	r3, [r6, r3]
	ldr	r2, [sp, #28]
	lsrs	r3, r3, #1
	adds	r3, #108
	cmp	r2, r3
	bne.n	.L_080da8a0
	movs	r0, #133
	bl	sub_080b50e8
.L_080da8a0:
	movs	r2, #0
	movs	r3, #100
	movs	r0, #0
	movs	r1, #0
	bl	sub_080b5028
	bl	sub_080049ac
	ldr	r0, [sp, #24]
	ldr	r1, [sp, #8]
	bl	sub_080051d8
	movs	r3, #0
	mov	sl, r3
	ldr	r3, [r5, #0]
	ldr	r3, [r3, #24]
	ldrb	r3, [r6, r3]
	adds	r2, r6, #0
	cmp	r3, #0
	bne.n	.L_080da8ca
	b.n	.L_080daa5c
.L_080da8ca:
	ldr	r6, [pc, #644]
.L_080da8cc:
	mov	r5, sl
	lsrs	r3, r5, #31
	add	r3, sl
	asrs	r3, r3, #1
	mov	r8, r3
	ldr	r0, [sp, #28]
	mov	r7, r8
	adds	r7, #48
	cmp	r0, r8
	ble.n	.L_080da978
	movs	r1, #1
	ldr	r3, [r6, #24]
	negs	r1, r1
	cmp	r3, r1
	bne.n	.L_080da974
	add	r5, sp, #44
	adds	r0, r6, #0
	adds	r1, r5, #0
	bl	sub_080e3944
	ldr	r3, [r5, #0]
	asrs	r3, r3, #1
	str	r3, [r5, #0]
	ldr	r3, [r5, #8]
	cmp	r3, #159
	bgt.n	.L_080da904
	movs	r3, #160
	str	r3, [r5, #8]
.L_080da904:
	ldr	r2, [pc, #588]
	cmp	r3, r2
	ble.n	.L_080da90e
	str	r2, [r5, #8]
	adds	r3, r2, #0
.L_080da90e:
	adds	r2, r3, #0
	subs	r2, #160
	cmp	r2, #0
	bge.n	.L_080da918
	adds	r2, #63
.L_080da918:
	asrs	r2, r2, #6
	movs	r3, #10
	subs	r4, r3, r2
	mov	r7, r8
	ldr	r3, [sp, #28]
	movs	r2, #4
	adds	r7, #48
	mov	ip, r2
	cmp	r3, r7
	blt.n	.L_080da930
	movs	r0, #0
	mov	ip, r0
.L_080da930:
	lsls	r0, r4, #1
	ldr	r2, [pc, #548]
	subs	r3, r0, #2
	ldrh	r1, [r2, r3]
	ldr	r2, [sp, #20]
	lsrs	r3, r4, #31
	adds	r1, r2, r1
	adds	r3, r4, r3
	ldr	r2, [r5, #0]
	asrs	r3, r3, #1
	subs	r2, r2, r3
	ldr	r3, [r5, #4]
	str	r0, [sp, #4]
	str	r4, [sp, #0]
	ldr	r0, [sp, #12]
	mov	r5, ip
	subs	r3, r3, r4
	ldr	r4, [r5, r0]
	ldr	r0, [sp, #32]
	bl	sub_080072f4
	ldr	r3, [r6, #0]
	ldr	r2, [r6, #12]
	adds	r3, r3, r2
	str	r3, [r6, #0]
	ldr	r2, [r6, #16]
	ldr	r3, [r6, #4]
	adds	r3, r3, r2
	str	r3, [r6, #4]
	ldr	r2, [r6, #20]
	ldr	r3, [r6, #8]
	adds	r3, r3, r2
	str	r3, [r6, #8]
	b.n	.L_080da978
.L_080da974:
	mov	r7, r8
	adds	r7, #48
.L_080da978:
	ldr	r1, [sp, #28]
	cmp	r1, r7
	ble.n	.L_080daa46
	movs	r2, #1
	ldr	r3, [r6, #24]
	negs	r2, r2
	cmp	r3, r2
	bne.n	.L_080daa46
	mov	r3, fp
	ldr	r5, [r3, #0]
	mov	r0, sl
	ldr	r1, [r5, #20]
	bl	sub_080022fc
	lsls	r0, r0, #1
	adds	r0, #36
	ldrsh	r0, [r5, r0]
	bl	sub_080b5098
	ldr	r1, [r0, #0]
	ldr	r2, [r6, #0]
	ldr	r3, [r1, #8]
	subs	r3, r3, r2
	ldr	r2, [r6, #12]
	asrs	r3, r3, #9
	adds	r0, r2, r3
	str	r0, [r6, #12]
	ldr	r2, [r6, #4]
	ldr	r3, [r1, #12]
	subs	r3, r3, r2
	ldr	r2, [r6, #16]
	asrs	r3, r3, #9
	adds	r4, r2, r3
	str	r4, [r6, #16]
	ldr	r2, [r6, #8]
	ldr	r3, [r1, #16]
	subs	r3, r3, r2
	ldr	r2, [r6, #20]
	asrs	r3, r3, #9
	adds	r1, r2, r3
	str	r1, [r6, #20]
	mov	r3, r8
	ldr	r2, [sp, #28]
	adds	r3, #85
	cmp	r2, r3
	bge.n	.L_080daa04
	lsls	r3, r0, #4
	subs	r3, r3, r0
	lsls	r2, r3, #2
	cmp	r2, #0
	bge.n	.L_080da9e0
	adds	r2, #63
.L_080da9e0:
	asrs	r3, r2, #6
	str	r3, [r6, #12]
	lsls	r3, r4, #4
	subs	r3, r3, r4
	lsls	r2, r3, #2
	cmp	r2, #0
	bge.n	.L_080da9f0
	adds	r2, #63
.L_080da9f0:
	asrs	r3, r2, #6
	str	r3, [r6, #16]
	lsls	r3, r1, #4
	subs	r3, r3, r1
	lsls	r2, r3, #2
	cmp	r2, #0
	bge.n	.L_080daa00
	adds	r2, #63
.L_080daa00:
	asrs	r3, r2, #6
	str	r3, [r6, #20]
.L_080daa04:
	ldr	r3, [r6, #4]
	cmp	r3, #0
	bge.n	.L_080daa46
	movs	r3, #0
	str	r3, [r6, #24]
	add	r2, sp, #44
	ldr	r3, [r2, #0]
	str	r3, [r6, #0]
	ldr	r3, [r2, #4]
	movs	r0, #136
	str	r3, [r6, #4]
	bl	sub_080f9010
	mov	r3, fp
	ldr	r5, [r3, #0]
	mov	r0, sl
	ldr	r1, [r5, #20]
	bl	sub_080022fc
	adds	r3, r0, #0
	lsls	r2, r3, #1
	adds	r2, #36
	ldrsh	r0, [r5, r2]
	movs	r2, #4
	str	r2, [sp, #0]
	movs	r1, #10
	movs	r2, #5
	bl	sub_080d6888
	ldr	r2, [pc, #284]
	movs	r3, #2
	add	r2, r9
	str	r3, [r2, #0]
.L_080daa46:
	mov	r5, fp
	ldr	r3, [r5, #0]
	movs	r2, #1
	add	sl, r2
	ldr	r3, [r3, #24]
	ldr	r2, [pc, #248]
	ldrb	r3, [r2, r3]
	adds	r6, #28
	cmp	sl, r3
	beq.n	.L_080daa5c
	b.n	.L_080da8cc
.L_080daa5c:
	mov	r1, fp
	ldr	r3, [r1, #0]
	ldr	r3, [r3, #24]
	ldrb	r3, [r2, r3]
	movs	r0, #0
	mov	sl, r0
	cmp	r3, #0
	beq.n	.L_080daac2
	ldr	r6, [pc, #224]
.L_080daa6e:
	ldr	r3, [r6, #24]
	cmp	r3, #11
	bhi.n	.L_080daab0
	lsrs	r4, r3, #31
	adds	r4, r3, r4
	asrs	r4, r4, #1
	ldr	r2, [pc, #228]
	lsls	r3, r4, #1
	ldrh	r1, [r2, r3]
	ldr	r3, [pc, #224]
	ldrb	r5, [r3, r4]
	ldr	r2, [r6, #0]
	lsrs	r3, r5, #1
	subs	r2, r2, r3
	ldr	r3, [pc, #220]
	ldrb	r0, [r3, r4]
	ldr	r3, [r6, #4]
	str	r5, [sp, #0]
	adds	r3, r3, r0
	ldr	r0, [pc, #212]
	ldrb	r0, [r0, r4]
	ldr	r5, [sp, #12]
	str	r0, [sp, #4]
	subs	r3, #56
	add	r1, r9
	ldr	r4, [r5, #4]
	ldr	r0, [sp, #32]
	bl	sub_080072f4
	ldr	r3, [r6, #24]
	adds	r3, #1
	str	r3, [r6, #24]
	ldr	r2, [pc, #156]
.L_080daab0:
	mov	r1, fp
	ldr	r3, [r1, #0]
	ldr	r3, [r3, #24]
	movs	r0, #1
	ldrb	r3, [r2, r3]
	add	sl, r0
	adds	r6, #28
	cmp	sl, r3
	bne.n	.L_080daa6e
.L_080daac2:
	ldr	r2, [pc, #116]
	add	r2, r9
	ldr	r3, [r2, #0]
	cmp	r3, #0
	bne.n	.L_080daad0
	movs	r3, #1
	str	r3, [r2, #0]
.L_080daad0:
	movs	r0, #8
	movs	r1, #8
	bl	sub_080e155c
	bl	sub_080cd52c
	ldr	r2, [pc, #144]
	movs	r3, #1
	add	r2, r9
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_080030f8
	ldr	r2, [sp, #28]
	adds	r2, #1
	str	r2, [sp, #28]
	mov	r5, fp
	ldr	r3, [r5, #0]
	ldr	r2, [pc, #84]
	ldr	r3, [r3, #24]
	ldrb	r3, [r2, r3]
	ldr	r0, [sp, #28]
	lsrs	r3, r3, #1
	adds	r3, #132
	cmp	r0, r3
	beq.n	.L_080dab06
	b.n	.L_080da86a
.L_080dab06:
	ldr	r0, [pc, #60]
	bl	sub_08004278
	ldr	r0, [pc, #44]
	bl	sub_08004278
	movs	r0, #47
	bl	sub_08002dd8
	movs	r0, #46
	bl	sub_08002dd8
	bl	sub_080cdbc0
	add	sp, #56
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x000077ac
	.4byte 0x000077b0
	.4byte 0x080d6505
	.4byte 0x00007784
	.4byte 0x080cd261
	.4byte 0x00007828
	.4byte 0x080eea41
	.4byte 0x02010000
	.4byte 0x0000031f
	.4byte 0x080ede48
	.4byte 0x000077a8
	.4byte 0x080eea56
	.4byte 0x080eea44
	.4byte 0x080eea50
	.4byte 0x080eea4a
	.4byte 0x00007824
