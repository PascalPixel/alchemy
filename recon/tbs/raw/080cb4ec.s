.syntax unified
	.thumb
	.set sub_08002304, 0x08002304
	.set sub_08002dd8, 0x08002dd8
	.set sub_080030f8, 0x080030f8
	.set sub_080041d8, 0x080041d8
	.set sub_08004278, 0x08004278
	.set sub_08004458, 0x08004458
	.set sub_080072f4, 0x080072f4
	.set sub_080cd52c, 0x080cd52c
	.set sub_080cd594, 0x080cd594
	.set sub_080cdbc0, 0x080cdbc0
	.set sub_080d6888, 0x080d6888
	.set sub_080e0524, 0x080e0524
	.set sub_080e155c, 0x080e155c
	.set sub_080e396c, 0x080e396c
	.set sub_080ed408, 0x080ed408
	.set sub_080f9010, 0x080f9010
	.global Unnamed_080cb4ec
	.global Func_080cb4ec
	.thumb_func
Unnamed_080cb4ec:
Func_080cb4ec:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r6, [pc, #96]
	adds	r3, r6, #0
	ldmia	r3!, {r2}
	ldr	r3, [r3, #0]
	sub	sp, #36
	str	r3, [sp, #12]
	ldr	r3, [pc, #88]
	mov	fp, r2
	add	r3, fp
	str	r0, [r3, #0]
	movs	r0, #1
	mov	r8, r3
	bl	sub_080cd594
	ldr	r2, [pc, #76]
	ldr	r3, [pc, #56]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #56]
	adds	r2, #50
	strh	r3, [r2, #0]
	movs	r5, #1
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	movs	r0, #46
	str	r5, [sp, #0]
	bl	sub_080ed408
	ldr	r3, [r6, #28]
	movs	r1, #7
	str	r3, [sp, #16]
	movs	r2, #7
	movs	r3, #7
	movs	r0, #47
	str	r5, [sp, #0]
	bl	sub_080ed408
	ldr	r3, [r6, #32]
	mov	r0, sp
	adds	r0, #16
	str	r0, [sp, #8]
	mov	r1, fp
	str	r3, [r0, #4]
	b.n	.L_080cb568
	movs	r0, r0
	.4byte 0x00000100
	.4byte 0x00001000
	.4byte 0x03001eec
	.4byte 0x00007828
	.2byte 0x0020
	.2byte 0x0400
.L_080cb568:
	movs	r2, #1
	ldr	r0, [pc, #152]
	movs	r3, #1
	bl	sub_080e0524
	movs	r3, #239
	lsls	r3, r3, #7
	ldr	r2, [pc, #144]
	add	r3, fp
	str	r5, [r3, #0]
	add	r2, fp
	movs	r3, #0
	movs	r1, #144
	str	r3, [r2, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #132]
	bl	sub_080041d8
	mov	r2, r8
	ldr	r3, [r2, #0]
	add	r5, sp, #24
	movs	r2, #36
	ldrsh	r0, [r3, r2]
	adds	r1, r5, #0
	bl	sub_080e396c
	ldr	r2, [r5, #0]
	movs	r3, #64
	ldr	r1, [pc, #108]
	subs	r3, r3, r2
	lsls	r3, r3, #8
	movs	r5, #225
	str	r3, [r1, #0]
	lsls	r5, r5, #7
	movs	r3, #0
	mov	r9, r3
	add	r5, fp
.L_080cb5b2:
	bl	sub_08004458
	movs	r1, #96
	bl	sub_08002304
	mov	r2, r9
	adds	r0, #16
	str	r0, [r5, #0]
	cmp	r2, #0
	bge.n	.L_080cb5c8
	adds	r2, #3
.L_080cb5c8:
	asrs	r2, r2, #2
	movs	r3, #24
	subs	r3, r3, r2
	lsls	r3, r3, #16
	str	r3, [r5, #4]
	cmp	r0, #43
	bgt.n	.L_080cb5da
	movs	r3, #3
	b.n	.L_080cb618
.L_080cb5da:
	cmp	r0, #51
	bgt.n	.L_080cb5e2
	movs	r3, #2
	b.n	.L_080cb618
.L_080cb5e2:
	cmp	r0, #59
	bgt.n	.L_080cb5ea
	movs	r3, #1
	b.n	.L_080cb618
.L_080cb5ea:
	cmp	r0, #67
	bgt.n	.L_080cb5f2
	movs	r3, #0
	b.n	.L_080cb618
.L_080cb5f2:
	cmp	r0, #75
	bgt.n	.L_080cb5fa
	movs	r3, #1
	b.n	.L_080cb616
.L_080cb5fa:
	cmp	r0, #83
	bgt.n	.L_080cb614
	movs	r3, #2
	b.n	.L_080cb616
	movs	r0, r0
	.4byte 0x00000078
	.4byte 0x00007784
	.4byte 0x080cd261
	.2byte 0x0028
	.2byte 0x0400
.L_080cb614:
	movs	r3, #3
.L_080cb616:
	negs	r3, r3
.L_080cb618:
	str	r3, [r5, #12]
	ldr	r3, [r5, #12]
	lsls	r3, r3, #17
	str	r3, [r5, #12]
	movs	r3, #128
	lsls	r3, r3, #12
	str	r3, [r5, #16]
	movs	r0, #1
	ldr	r3, [r5, #0]
	add	r9, r0
	lsls	r3, r3, #16
	mov	r2, r9
	str	r3, [r5, #0]
	adds	r5, #28
	cmp	r2, #64
	bne.n	.L_080cb5b2
	movs	r0, #212
	bl	sub_080f9010
	movs	r3, #0
	mov	sl, r3
.L_080cb642:
	mov	r0, sl
	cmp	r0, #16
	bgt.n	.L_080cb65c
	ldr	r2, [pc, #40]
	mov	r3, sl
	orrs	r3, r2
	ldr	r2, [pc, #48]
	strh	r3, [r2, #0]
	cmp	r0, #16
	bne.n	.L_080cb65c
	ldr	r3, [pc, #32]
	subs	r2, #2
	strh	r3, [r2, #0]
.L_080cb65c:
	mov	r3, sl
	cmp	r3, #103
	ble.n	.L_080cb68e
	ldr	r3, [pc, #24]
	ldr	r2, [pc, #12]
	mov	r0, sl
	subs	r3, r3, r0
	orrs	r3, r2
	ldr	r2, [pc, #16]
	strh	r3, [r2, #0]
	b.n	.L_080cb684
	movs	r0, r0
	.4byte 0x00001000
	.4byte 0x00000000
	.4byte 0x00000078
	.2byte 0x0052
	.2byte 0x0400
.L_080cb684:
	cmp	r0, #104
	bne.n	.L_080cb68e
	ldr	r3, [pc, #44]
	subs	r2, #2
	strh	r3, [r2, #0]
.L_080cb68e:
	ldr	r7, [pc, #44]
	movs	r3, #15
	mov	r9, r3
	add	r7, fp
.L_080cb696:
	ldr	r6, [r7, #12]
	adds	r3, r6, #0
	cmp	r6, #0
	bge.n	.L_080cb6a0
	negs	r3, r6
.L_080cb6a0:
	mov	r0, r9
	lsls	r0, r0, #2
	mov	r8, r0
	asrs	r4, r3, #17
	mov	r3, r8
	adds	r3, #25
	cmp	sl, r3
	bge.n	.L_080cb70a
	ldr	r2, [pc, #12]
	lsls	r3, r4, #1
	ldrh	r1, [r2, r3]
	b.n	.L_080cb6c4
	.4byte 0x00003f44
	.4byte 0x00007224
	.2byte 0xdf88
	.2byte 0x080e
.L_080cb6c4:
	movs	r3, #2
	ldrsh	r2, [r7, r3]
	ldr	r3, [pc, #272]
	ldrb	r5, [r3, r4]
	lsrs	r3, r5, #1
	subs	r2, r2, r3
	movs	r0, #6
	ldrsh	r3, [r7, r0]
	ldr	r0, [pc, #264]
	ldrb	r4, [r0, r4]
	lsrs	r0, r4, #1
	subs	r3, r3, r0
	str	r5, [sp, #0]
	ldr	r0, [sp, #8]
	str	r4, [sp, #4]
	lsrs	r6, r6, #31
	lsls	r6, r6, #2
	ldr	r4, [r6, r0]
	add	r1, fp
	ldr	r0, [sp, #12]
	bl	sub_080072f4
	mov	r3, r8
	adds	r3, #16
	cmp	sl, r3
	blt.n	.L_080cb73e
	ldr	r3, [r7, #0]
	ldr	r2, [r7, #12]
	adds	r3, r3, r2
	str	r3, [r7, #0]
	ldr	r2, [r7, #16]
	ldr	r3, [r7, #4]
	adds	r3, r3, r2
	str	r3, [r7, #4]
	b.n	.L_080cb73e
.L_080cb70a:
	ldr	r2, [pc, #216]
	lsls	r3, r4, #1
	ldrh	r1, [r2, r3]
	movs	r3, #2
	ldrsh	r2, [r7, r3]
	ldr	r3, [pc, #196]
	ldrb	r5, [r3, r4]
	lsrs	r3, r5, #1
	subs	r2, r2, r3
	movs	r0, #6
	ldrsh	r3, [r7, r0]
	ldr	r0, [pc, #188]
	ldrb	r4, [r0, r4]
	lsrs	r0, r4, #1
	subs	r4, #4
	lsrs	r6, r6, #31
	subs	r3, r3, r0
	str	r5, [sp, #0]
	ldr	r0, [sp, #8]
	str	r4, [sp, #4]
	lsls	r6, r6, #2
	ldr	r4, [r6, r0]
	add	r1, fp
	ldr	r0, [sp, #12]
	bl	sub_080072f4
.L_080cb73e:
	movs	r2, #1
	negs	r2, r2
	add	r9, r2
	subs	r7, #28
	cmp	r9, r2
	bne.n	.L_080cb696
	mov	r3, sl
	subs	r3, #23
	cmp	r3, #64
	bhi.n	.L_080cb78c
	movs	r3, #3
	mov	r0, sl
	ands	r3, r0
	cmp	r3, #0
	bne.n	.L_080cb78c
	ldr	r3, [pc, #136]
	add	r3, fp
	ldr	r3, [r3, #0]
	movs	r2, #36
	ldrsh	r0, [r3, r2]
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r2, #5
	movs	r3, #0
	movs	r1, #7
	bl	sub_080d6888
	ldr	r2, [pc, #116]
	movs	r3, #1
	add	r2, fp
	str	r3, [r2, #0]
	mov	r0, sl
	movs	r3, #7
	ands	r3, r0
	cmp	r3, #0
	bne.n	.L_080cb78c
	movs	r0, #133
	bl	sub_080f9010
.L_080cb78c:
	movs	r0, #8
	movs	r1, #8
	bl	sub_080e155c
	bl	sub_080cd52c
	ldr	r2, [pc, #84]
	movs	r3, #1
	add	r2, fp
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_080030f8
	movs	r2, #1
	add	sl, r2
	mov	r3, sl
	cmp	r3, #120
	beq.n	.L_080cb7b2
	b.n	.L_080cb642
.L_080cb7b2:
	ldr	r0, [pc, #64]
	bl	sub_08004278
	movs	r0, #47
	bl	sub_08002dd8
	movs	r0, #46
	bl	sub_08002dd8
	bl	sub_080cdbc0
	add	sp, #36
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x080edf7f
	.4byte 0x080edf83
	.4byte 0x080edf88
	.4byte 0x00007828
	.4byte 0x000077a8
	.4byte 0x00007824
	.4byte 0x080cd261
