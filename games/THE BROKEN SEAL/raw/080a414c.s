.syntax unified
	.thumb
	.set sub_080022ec, 0x080022ec
	.set sub_080022fc, 0x080022fc
	.set sub_080030f8, 0x080030f8
	.set sub_080041d8, 0x080041d8
	.set sub_08015070, 0x08015070
	.set sub_08015080, 0x08015080
	.set sub_08015270, 0x08015270
	.set sub_080770c0, 0x080770c0
	.set sub_080a112c, 0x080a112c
	.set sub_080a1a40, 0x080a1a40
	.set sub_080a1ac0, 0x080a1ac0
	.set sub_080a345c, 0x080a345c
	.set sub_080a3c98, 0x080a3c98
	.set sub_080a3ef0, 0x080a3ef0
	.set sub_080a4110, 0x080a4110
	.set sub_080a413c, 0x080a413c
	.set sub_080a448c, 0x080a448c
	.set sub_080a45cc, 0x080a45cc
	.set sub_080a4eb8, 0x080a4eb8
	.set sub_080a51d0, 0x080a51d0
	.set sub_080f9010, 0x080f9010
	.global Overlay_080a414c
Overlay_080a414c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r1, #0
	sub	sp, #16
	mov	sl, r1
	movs	r1, #8
	ldr	r3, [pc, #772]
	add	r1, sp
	mov	fp, r1
	movs	r2, #0
	ldr	r6, [r3, #0]
	mov	r0, fp
	movs	r3, #1
	mov	r8, r2
	mov	r9, r3
	bl	sub_080a448c
	movs	r2, #136
	lsls	r2, r2, #2
	adds	r2, r6, r2
	str	r2, [sp, #4]
	ldrh	r3, [r2, #0]
	movs	r7, #0
	cmp	r3, #1
	beq.n	.L_080a41e0
	bl	sub_080a345c
	ldr	r0, [r6, #52]
	bl	sub_08015270
	movs	r1, #134
	lsls	r1, r1, #1
	adds	r3, r6, r1
	ldr	r5, [r3, #0]
	bl	sub_080a4eb8
	adds	r0, r5, #0
	bl	sub_08015270
	movs	r3, #3
	str	r3, [sp, #0]
	movs	r2, #3
	movs	r3, #16
	adds	r0, r5, #0
	movs	r1, #0
	bl	sub_08015070
	bl	sub_080a51d0
	adds	r1, r5, #0
	mov	r0, fp
	bl	sub_080a45cc
	ldr	r0, [r6, #44]
	bl	sub_08015270
	movs	r2, #188
	lsls	r2, r2, #1
	adds	r3, r6, r2
	ldrh	r3, [r3, #0]
	ldr	r0, [pc, #668]
	ands	r0, r3
	ldr	r3, [pc, #668]
	ldr	r1, [r6, #44]
	adds	r0, r0, r3
	movs	r2, #0
	movs	r3, #0
	bl	sub_08015080
.L_080a41e0:
	ldr	r1, [sp, #4]
	mov	r3, sl
	ldr	r2, [pc, #652]
	strh	r3, [r1, #0]
	adds	r3, r6, r2
	movs	r5, #0
	ldrsb	r5, [r3, r5]
	movs	r3, #1
	negs	r3, r3
	cmp	r5, r3
	bne.n	.L_080a4258
	mov	r1, fp
	movs	r3, #2
	ldrsb	r3, [r1, r3]
	cmp	r3, #1
	bne.n	.L_080a4206
	movs	r2, #0
	movs	r7, #2
	mov	r8, r2
.L_080a4206:
	mov	r1, fp
	movs	r3, #3
	ldrsb	r3, [r1, r3]
	cmp	r3, #1
	bne.n	.L_080a4216
	movs	r2, #1
	movs	r7, #0
	mov	r8, r2
.L_080a4216:
	mov	r1, fp
	movs	r3, #1
	ldrsb	r3, [r1, r3]
	cmp	r3, #1
	bne.n	.L_080a4226
	movs	r2, #0
	movs	r7, #1
	mov	r8, r2
.L_080a4226:
	mov	r1, fp
	movs	r3, #4
	ldrsb	r3, [r1, r3]
	cmp	r3, #1
	bne.n	.L_080a4236
	movs	r2, #1
	movs	r7, #1
	mov	r8, r2
.L_080a4236:
	mov	r1, fp
	movs	r3, #0
	ldrsb	r3, [r1, r3]
	cmp	r3, #1
	bne.n	.L_080a427a
	movs	r2, #0
	movs	r7, #0
	mov	r8, r2
	b.n	.L_080a427a
.L_080a4248:
	movs	r0, #113
	bl	sub_080f9010
	movs	r3, #1
	negs	r3, r3
	ldr	r1, [pc, #544]
	mov	sl, r3
	b.n	.L_080a43c0
.L_080a4258:
	movs	r1, #3
	adds	r0, r5, #0
	bl	sub_080022fc
	lsls	r0, r0, #24
	asrs	r7, r0, #24
	movs	r1, #3
	adds	r0, r5, #0
	bl	sub_080022ec
	lsls	r0, r0, #24
	asrs	r0, r0, #24
	mov	r8, r0
	lsls	r3, r0, #1
	add	r3, r8
	adds	r3, r3, r7
	mov	sl, r3
.L_080a427a:
	mov	r1, r8
	adds	r0, r7, #0
	bl	sub_080a4110
	mov	r1, r8
	adds	r5, r0, #0
	adds	r0, r7, #0
	bl	sub_080a413c
	adds	r1, r0, #0
	adds	r0, r5, #0
	bl	sub_080a1ac0
	b.n	.L_080a4436
.L_080a4296:
	mov	r3, r9
	cmp	r3, #0
	beq.n	.L_080a4330
	movs	r1, #0
	adds	r0, r7, #3
	mov	r9, r1
	movs	r1, #3
	bl	sub_080022fc
	mov	r2, r8
	adds	r2, #2
	lsrs	r3, r2, #31
	adds	r3, r2, r3
	asrs	r3, r3, #1
	lsls	r3, r3, #1
	subs	r2, r2, r3
	mov	r8, r2
	lsls	r3, r2, #1
	adds	r7, r0, #0
	add	r3, r8
	adds	r3, r3, r7
	mov	sl, r3
	bl	sub_080a3c98
	mov	r2, sl
	cmp	r2, #2
	ble.n	.L_080a42fc
	movs	r3, #151
	lsls	r3, r3, #2
	adds	r2, r6, r3
	ldr	r1, [pc, #420]
	movs	r3, #1
	strb	r3, [r2, #0]
	adds	r3, r6, r1
	ldrb	r3, [r3, #0]
	subs	r1, #166
	adds	r2, r6, r1
	ldrh	r1, [r2, #0]
	adds	r0, r3, #0
	movs	r2, #0
	bl	sub_080a3ef0
	mov	r2, sl
	cmp	r2, #3
	bne.n	.L_080a4330
	movs	r1, #200
	ldr	r0, [pc, #392]
	lsls	r1, r1, #4
	bl	sub_080041d8
	b.n	.L_080a4330
.L_080a42fc:
	mov	r3, sl
	cmp	r3, #0
	beq.n	.L_080a4320
	movs	r1, #151
	lsls	r1, r1, #2
	adds	r3, r6, r1
	ldr	r2, [pc, #364]
	strb	r5, [r3, #0]
	adds	r3, r6, r2
	subs	r1, #232
	ldrb	r3, [r3, #0]
	adds	r2, r6, r1
	ldrh	r1, [r2, #0]
	adds	r0, r3, #0
	movs	r2, #0
	bl	sub_080a3ef0
	b.n	.L_080a4330
.L_080a4320:
	ldr	r2, [pc, #340]
	adds	r3, r6, r2
	ldrb	r1, [r3, #0]
	ldr	r0, [r6, #36]
	movs	r2, #0
	movs	r3, #0
	bl	sub_080a112c
.L_080a4330:
	mov	r1, r8
	adds	r0, r7, #0
	bl	sub_080a4110
	mov	r1, r8
	adds	r5, r0, #0
	adds	r0, r7, #0
	bl	sub_080a413c
	adds	r1, r0, #0
	adds	r0, r5, #0
	bl	sub_080a1a40
	movs	r0, #1
	bl	sub_080030f8
	ldr	r5, [pc, #300]
	ldr	r2, [r5, #0]
	movs	r3, #1
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_080a43c8
	mov	r1, fp
	mov	r2, sl
	ldrsb	r3, [r1, r2]
	movs	r1, #1
	negs	r1, r1
	cmp	r3, r1
	bne.n	.L_080a4372
	movs	r0, #114
	bl	sub_080f9010
	b.n	.L_080a43c8
.L_080a4372:
	mov	r2, sl
	cmp	r2, #5
	bhi.n	.L_080a43b8
	lsls	r3, r2, #2
	ldr	r2, [pc, #264]
	ldr	r3, [r3, r2]
	mov	pc, r3
	bics	r0, r3
	lsrs	r2, r1, #32
	bics	r0, r4
	lsrs	r2, r1, #32
	bics	r0, r5
	lsrs	r2, r1, #32
	bics	r0, r5
	lsrs	r2, r1, #32
	bics	r0, r6
	lsrs	r2, r1, #32
	bics	r0, r5
	lsrs	r2, r1, #32
	movs	r0, #174
	bl	sub_080f9010
	b.n	.L_080a43be
	movs	r0, #175
	bl	sub_080f9010
	b.n	.L_080a43be
	movs	r0, #112
	bl	sub_080f9010
	b.n	.L_080a43be
	movs	r0, #117
	bl	sub_080f9010
	b.n	.L_080a43be
.L_080a43b8:
	movs	r0, #112
	bl	sub_080f9010
.L_080a43be:
	ldr	r1, [pc, #180]
.L_080a43c0:
	mov	r2, sl
	adds	r3, r6, r1
	strb	r2, [r3, #0]
	b.n	.L_080a4446
.L_080a43c8:
	ldr	r2, [r5, #0]
	movs	r3, #2
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_080a43d4
	b.n	.L_080a4248
.L_080a43d4:
	ldr	r1, [pc, #176]
	ldr	r2, [r1, #0]
	movs	r3, #64
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_080a43f0
	subs	r3, #65
	movs	r1, #1
	movs	r0, #111
	add	r8, r3
	mov	r9, r1
	bl	sub_080f9010
	b.n	.L_080a4436
.L_080a43f0:
	ldr	r2, [r1, #0]
	movs	r3, #128
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_080a4408
	movs	r2, #1
	movs	r0, #111
	add	r8, r2
	mov	r9, r2
	bl	sub_080f9010
	b.n	.L_080a4436
.L_080a4408:
	ldr	r2, [r1, #0]
	movs	r3, #16
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_080a4420
	movs	r3, #1
	movs	r0, #111
	adds	r7, #1
	mov	r9, r3
	bl	sub_080f9010
	b.n	.L_080a4436
.L_080a4420:
	ldr	r3, [r1, #0]
	movs	r2, #32
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080a4436
	movs	r1, #1
	movs	r0, #111
	subs	r7, #1
	mov	r9, r1
	bl	sub_080f9010
.L_080a4436:
	movs	r0, #168
	lsls	r0, r0, #1
	bl	sub_080770c0
	adds	r5, r0, #0
	cmp	r5, #0
	bne.n	.L_080a4446
	b.n	.L_080a4296
.L_080a4446:
	movs	r3, #151
	lsls	r3, r3, #2
	adds	r2, r6, r3
	movs	r3, #0
	strb	r3, [r2, #0]
	bl	sub_080a3c98
	mov	r0, sl
	add	sp, #16
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x03001f2c
	.4byte 0x000001ff
	.4byte 0x00000075
	.4byte 0x0000025d
	.4byte 0x0000021a
	.4byte 0x080a3c09
	.4byte 0x03001c94
	.4byte 0x080a4380
	.4byte 0x03001b04
