.syntax unified
	.thumb
	.set sub_080022fc, 0x080022fc
	.set sub_080030f8, 0x080030f8
	.set sub_080041d8, 0x080041d8
	.set sub_08004278, 0x08004278
	.set sub_08015080, 0x08015080
	.set sub_08015270, 0x08015270
	.set sub_08015278, 0x08015278
	.set sub_08077008, 0x08077008
	.set sub_080770c0, 0x080770c0
	.set sub_080a10d0, 0x080a10d0
	.set sub_080a1804, 0x080a1804
	.set sub_080a1a40, 0x080a1a40
	.set sub_080a1e38, 0x080a1e38
	.set sub_080a1fd4, 0x080a1fd4
	.set sub_080a33d4, 0x080a33d4
	.set sub_080a345c, 0x080a345c
	.set sub_080a3ddc, 0x080a3ddc
	.set sub_080a3e28, 0x080a3e28
	.set sub_080a8b8c, 0x080a8b8c
	.set sub_080a93a4, 0x080a93a4
	.set sub_080a9598, 0x080a9598
	.set sub_080a9a5c, 0x080a9a5c
	.set sub_080a9cbc, 0x080a9cbc
	.set sub_080f9010, 0x080f9010
	.global Overlay_080a96d8
Overlay_080a96d8:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #684]
	ldr	r0, [pc, #684]
	ldr	r7, [r3, #0]
	adds	r3, r7, r0
	ldrb	r3, [r3, #0]
	sub	sp, #84
	movs	r2, #0
	str	r2, [sp, #20]
	subs	r0, #18
	lsls	r3, r3, #1
	adds	r3, r3, r0
	ldrh	r0, [r7, r3]
	bl	sub_08077008
	movs	r3, #10
	str	r3, [sp, #0]
	adds	r0, r7, #0
	movs	r3, #2
	str	r3, [sp, #4]
	movs	r1, #0
	movs	r2, #10
	movs	r3, #15
	adds	r0, #48
	bl	sub_080a10d0
	bl	sub_080a9cbc
	adds	r2, r7, #0
	movs	r1, #240
	adds	r2, #72
	movs	r4, #31
.L_080a9724:
	ldmia	r2!, {r3}
	cmp	r3, #0
	beq.n	.L_080a972c
	strb	r1, [r3, #15]
.L_080a972c:
	subs	r4, #1
	cmp	r4, #0
	bge.n	.L_080a9724
	ldr	r0, [pc, #616]
	bl	sub_08004278
	movs	r6, #24
	ldr	r5, [pc, #612]
	negs	r6, r6
	adds	r0, r5, #0
	ldr	r1, [r7, #36]
	movs	r2, #64
	adds	r3, r6, #0
	adds	r5, #3
	bl	sub_08015080
	ldr	r1, [r7, #36]
	movs	r2, #0
	adds	r3, r6, #0
	adds	r0, r5, #0
	bl	sub_08015080
	bl	sub_080a9cbc
	movs	r0, #1
	bl	sub_080030f8
	movs	r2, #134
	lsls	r2, r2, #1
	adds	r3, r7, r2
	ldr	r1, [r3, #0]
	adds	r0, r7, #0
	bl	sub_080a33d4
	movs	r0, #228
	lsls	r0, r0, #1
	movs	r3, #0
	adds	r0, r7, r0
	str	r3, [sp, #16]
	str	r0, [sp, #12]
	b.n	.L_080a9926
.L_080a977e:
	movs	r0, #112
	bl	sub_080f9010
	movs	r2, #1
	str	r2, [sp, #20]
	str	r2, [sp, #16]
	b.n	.L_080a9926
.L_080a978c:
	movs	r0, #113
	bl	sub_080f9010
	movs	r3, #1
	negs	r3, r3
	movs	r0, #1
	str	r3, [sp, #20]
	str	r0, [sp, #16]
	b.n	.L_080a9926
.L_080a979e:
	lsls	r2, r4, #1
	adds	r3, r2, #0
	adds	r3, #216
	mov	r0, r9
	ldrh	r3, [r0, r3]
	add	r1, sp, #52
	strh	r3, [r1, r2]
	adds	r4, #1
.L_080a97ae:
	cmp	r4, #14
	ble.n	.L_080a979e
	ldr	r1, [sp, #12]
	movs	r2, #0
	mov	r0, r9
	bl	sub_080a3ddc
	movs	r2, #134
	lsls	r2, r2, #2
	adds	r3, r7, r2
	strb	r0, [r3, #0]
	ldr	r0, [sp, #12]
	movs	r1, #0
	bl	sub_080a1e38
	add	r3, sp, #24
	mov	r8, r3
	movs	r1, #0
	mov	r0, r8
	bl	sub_080a8b8c
	ldr	r0, [r7, #48]
	bl	sub_08015270
	ldr	r2, [pc, #440]
	adds	r3, r7, r2
	ldrb	r1, [r3, #0]
	movs	r2, #1
	ldr	r0, [r7, #48]
	bl	sub_080a9a5c
	movs	r0, #1
	bl	sub_080030f8
	movs	r1, #0
	ldr	r0, [sp, #12]
	bl	sub_080a3e28
	movs	r5, #1
	mov	sl, r5
	mov	fp, r8
	b.n	.L_080a9918
.L_080a9802:
	cmp	r5, #0
	beq.n	.L_080a982a
	ldr	r0, [r7, #44]
	bl	sub_08015278
	mov	r3, sl
	movs	r5, #0
	cmp	r3, #0
	beq.n	.L_080a9820
	ldr	r0, [r7, #36]
	movs	r1, #0
	mov	r2, r8
	mov	sl, r5
	bl	sub_080a9598
.L_080a9820:
	ldr	r0, [r7, #36]
	movs	r1, #0
	mov	r2, r8
	bl	sub_080a93a4
.L_080a982a:
	mov	r0, fp
	ldr	r1, [r0, #16]
	ldr	r3, [r7, #20]
	movs	r6, #1
	lsls	r1, r1, #4
	strb	r6, [r3, #5]
	adds	r1, #52
	movs	r0, #96
	bl	sub_080a1a40
	movs	r0, #1
	bl	sub_080030f8
	mov	r2, fp
	add	r3, sp, #32
	ldr	r1, [r2, #20]
	movs	r0, #0
	str	r3, [sp, #0]
	movs	r2, #5
	add	r3, sp, #40
	bl	sub_080a1fd4
	cmp	r0, #1
	bne.n	.L_080a9860
	movs	r3, #1
	mov	sl, r3
	movs	r5, #1
.L_080a9860:
	cmp	r0, #0
	bne.n	.L_080a9866
	movs	r5, #1
.L_080a9866:
	movs	r2, #1
	negs	r2, r2
	cmp	r0, r2
	bne.n	.L_080a9870
	movs	r5, #0
.L_080a9870:
	ldr	r2, [pc, #304]
	ldr	r3, [r2, #0]
	ands	r3, r6
	cmp	r3, #0
	beq.n	.L_080a987c
	b.n	.L_080a977e
.L_080a987c:
	ldr	r2, [r2, #0]
	movs	r3, #2
	ands	r2, r3
	cmp	r2, #0
	bne.n	.L_080a978c
	ldr	r6, [pc, #288]
	movs	r1, #128
	ldr	r3, [r6, #0]
	lsls	r1, r1, #1
	ands	r3, r1
	cmp	r3, #0
	bne.n	.L_080a98a0
	ldr	r2, [r6, #0]
	movs	r3, #128
	lsls	r3, r3, #2
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_080a9918
.L_080a98a0:
	movs	r0, #111
	str	r1, [sp, #8]
	bl	sub_080f9010
	movs	r5, #28
	ldrsb	r5, [r7, r5]
	movs	r0, #130
	lsls	r0, r0, #2
	lsls	r3, r5, #1
	adds	r3, r3, r0
	ldrh	r3, [r7, r3]
	movs	r2, #152
	lsls	r2, r2, #2
	mov	r0, r8
	adds	r3, r3, r2
	ldr	r2, [r0, #24]
	strb	r2, [r7, r3]
	ldr	r1, [sp, #8]
	ldr	r3, [r6, #0]
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_080a98d0
	adds	r5, #1
	b.n	.L_080a98d2
.L_080a98d0:
	subs	r5, #1
.L_080a98d2:
	mov	r0, r9
	add	r1, sp, #52
	adds	r0, #216
	movs	r2, #0
	movs	r4, #14
.L_080a98dc:
	ldrh	r3, [r2, r1]
	subs	r4, #1
	strh	r3, [r0, #0]
	adds	r2, #2
	adds	r0, #2
	cmp	r4, #0
	bge.n	.L_080a98dc
	ldr	r2, [pc, #192]
	adds	r3, r7, r2
	ldrb	r1, [r3, #0]
	adds	r0, r5, r1
	bl	sub_080022fc
	movs	r3, #130
	adds	r5, r0, #0
	lsls	r3, r3, #2
	lsls	r2, r5, #1
	adds	r2, r2, r3
	ldrh	r3, [r7, r2]
	ldr	r0, [pc, #148]
	str	r3, [r7, #8]
	ldrh	r1, [r7, r2]
	adds	r3, r7, r0
	strb	r1, [r3, #0]
	strb	r5, [r7, #28]
	adds	r0, r7, #0
	ldrh	r1, [r7, r2]
	bl	sub_080a1804
	b.n	.L_080a9926
.L_080a9918:
	movs	r0, #168
	lsls	r0, r0, #1
	bl	sub_080770c0
	cmp	r0, #0
	bne.n	.L_080a9926
	b.n	.L_080a9802
.L_080a9926:
	ldr	r2, [sp, #16]
	cmp	r2, #0
	bne.n	.L_080a9952
	movs	r0, #168
	lsls	r0, r0, #1
	bl	sub_080770c0
	cmp	r0, #0
	bne.n	.L_080a9952
	bl	sub_080a9cbc
	ldr	r0, [r7, #36]
	bl	sub_08015270
	ldr	r0, [pc, #84]
	adds	r3, r7, r0
	ldrb	r0, [r3, #0]
	bl	sub_08077008
	movs	r4, #0
	mov	r9, r0
	b.n	.L_080a97ae
.L_080a9952:
	ldr	r0, [r7, #44]
	bl	sub_08015278
	ldr	r0, [r7, #44]
	bl	sub_08015270
	movs	r2, #134
	lsls	r2, r2, #1
	adds	r3, r7, r2
	ldr	r0, [r3, #0]
	bl	sub_08015278
	bl	sub_080a345c
	ldr	r0, [r7, #36]
	bl	sub_08015270
	movs	r1, #200
	lsls	r1, r1, #4
	ldr	r0, [pc, #32]
	bl	sub_080041d8
	ldr	r0, [sp, #20]
	add	sp, #84
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001f2c
	.4byte 0x0000021a
	.4byte 0x080a19a1
	.4byte 0x00000b06
	.4byte 0x03001c94
	.4byte 0x03001b04
	.4byte 0x00000219
