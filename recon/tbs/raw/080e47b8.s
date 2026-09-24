.syntax unified
	.thumb
	.set sub_080030f8, 0x080030f8
	.set sub_080041d8, 0x080041d8
	.set sub_08004458, 0x08004458
	.set sub_080049ac, 0x080049ac
	.set sub_08004bd4, 0x08004bd4
	.set sub_08004c1c, 0x08004c1c
	.set sub_08004c6c, 0x08004c6c
	.set sub_080051d8, 0x080051d8
	.set sub_080072f4, 0x080072f4
	.set sub_08009088, 0x08009088
	.set sub_080b5098, 0x080b5098
	.set sub_080cd594, 0x080cd594
	.set sub_080cdb24, 0x080cdb24
	.set sub_080cef64, 0x080cef64
	.set sub_080df9d0, 0x080df9d0
	.set sub_080e0524, 0x080e0524
	.set sub_080e3944, 0x080e3944
	.set sub_080e396c, 0x080e396c
	.set sub_080e4e0c, 0x080e4e0c
	.set sub_080e4e54, 0x080e4e54
	.set sub_080e4e58, 0x080e4e58
	.set sub_080f9010, 0x080f9010
	.global FunctionHead_080e47b8
	.global Func_080e47b8
	.thumb_func
FunctionHead_080e47b8:
Func_080e47b8:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #184
	ldr	r3, [pc, #56]
	str	r1, [sp, #96]
	str	r0, [sp, #100]
	adds	r2, r3, #0
	ldmia	r2!, {r0}
	str	r0, [sp, #92]
	ldr	r2, [r2, #0]
	str	r2, [sp, #88]
	adds	r2, r3, #0
	subs	r2, #108
	ldr	r2, [r2, #0]
	str	r2, [sp, #80]
	ldr	r1, [pc, #36]
	ldr	r3, [r3, #8]
	ldr	r2, [sp, #100]
	str	r3, [sp, #76]
	adds	r3, r0, r1
	str	r2, [r3, #0]
	ldr	r3, [sp, #96]
	cmp	r3, #11
	beq.n	.L_080e47fa
	cmp	r3, #8
	beq.n	.L_080e47fa
	cmp	r3, #32
	bne.n	.L_080e480c
.L_080e47fa:
	movs	r0, #0
	bl	sub_080cdb24
	b.n	.L_080e4812
	movs	r0, r0
	.4byte 0x03001eec
	.2byte 0x7828
	.2byte 0x0000
.L_080e480c:
	movs	r0, #0
	bl	sub_080cd594
.L_080e4812:
	ldr	r2, [pc, #68]
	ldr	r3, [pc, #60]
	strh	r3, [r2, #0]
	ldr	r1, [sp, #76]
	ldr	r0, [pc, #64]
	movs	r2, #0
	movs	r3, #0
	bl	sub_080e0524
	ldr	r0, [pc, #56]
	ldr	r1, [sp, #92]
	movs	r2, #1
	movs	r3, #0
	bl	sub_080e0524
	ldr	r0, [pc, #48]
	ldr	r1, [pc, #52]
	movs	r2, #1
	movs	r3, #0
	bl	sub_080e0524
	movs	r0, #162
	ldr	r5, [sp, #92]
	lsls	r0, r0, #7
	movs	r3, #144
	adds	r1, r5, r0
	lsls	r3, r3, #1
	ldr	r0, [pc, #28]
	movs	r2, #40
	bl	sub_080df9d0
	b.n	.L_080e486c
	movs	r0, r0
	.4byte 0x00001010
	.4byte 0x04000052
	.4byte 0x00000073
	.4byte 0x00000096
	.4byte 0x00000099
	.2byte 0x0000
	.2byte 0x0201
.L_080e486c:
	ldr	r1, [sp, #96]
	cmp	r1, #5
	beq.n	.L_080e4876
	cmp	r1, #23
	bne.n	.L_080e487a
.L_080e4876:
	ldr	r0, [pc, #644]
	b.n	.L_080e48f0
.L_080e487a:
	ldr	r2, [sp, #96]
	cmp	r2, #12
	bne.n	.L_080e4884
	ldr	r0, [pc, #636]
	b.n	.L_080e48f0
.L_080e4884:
	ldr	r3, [sp, #96]
	cmp	r3, #6
	beq.n	.L_080e488e
	cmp	r3, #27
	bne.n	.L_080e48a0
.L_080e488e:
	ldr	r0, [pc, #628]
	ldr	r1, [pc, #628]
	movs	r2, #1
	movs	r3, #0
	bl	sub_080e0524
	ldr	r0, [pc, #624]
	ldr	r1, [pc, #624]
	b.n	.L_080e48f2
.L_080e48a0:
	ldr	r5, [sp, #96]
	cmp	r5, #31
	bne.n	.L_080e48aa
	ldr	r0, [pc, #620]
	b.n	.L_080e48b2
.L_080e48aa:
	ldr	r0, [sp, #96]
	cmp	r0, #8
	bne.n	.L_080e48be
	ldr	r0, [pc, #612]
.L_080e48b2:
	ldr	r1, [pc, #596]
	movs	r2, #1
	movs	r3, #1
	bl	sub_080e0524
	b.n	.L_080e4912
.L_080e48be:
	ldr	r1, [sp, #96]
	cmp	r1, #14
	bne.n	.L_080e48c8
	ldr	r0, [pc, #596]
	b.n	.L_080e48f0
.L_080e48c8:
	ldr	r2, [sp, #96]
	cmp	r2, #30
	bne.n	.L_080e48d2
	ldr	r0, [pc, #564]
	b.n	.L_080e48f0
.L_080e48d2:
	ldr	r3, [sp, #96]
	cmp	r3, #16
	bne.n	.L_080e48dc
	ldr	r0, [pc, #580]
	b.n	.L_080e48f0
.L_080e48dc:
	ldr	r5, [sp, #96]
	cmp	r5, #20
	bne.n	.L_080e48e6
	ldr	r0, [pc, #576]
	b.n	.L_080e48f0
.L_080e48e6:
	ldr	r3, [sp, #96]
	subs	r3, #33
	cmp	r3, #1
	bhi.n	.L_080e48fc
	ldr	r0, [pc, #568]
.L_080e48f0:
	ldr	r1, [pc, #532]
.L_080e48f2:
	movs	r2, #1
	movs	r3, #0
	bl	sub_080e0524
	b.n	.L_080e4912
.L_080e48fc:
	ldr	r0, [sp, #96]
	cmp	r0, #11
	beq.n	.L_080e4912
	cmp	r0, #32
	beq.n	.L_080e4912
	ldr	r0, [pc, #548]
	ldr	r1, [pc, #508]
	movs	r2, #1
	movs	r3, #0
	bl	sub_080e0524
.L_080e4912:
	ldr	r1, [sp, #96]
	cmp	r1, #100
	bls.n	.L_080e491a
	b.n	.L_080e4ade
.L_080e491a:
	ldr	r2, [pc, #532]
	lsls	r3, r1, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	movs	r0, r0
	.4byte 0x080e4ab8
	.4byte 0x080e4ac4
	.4byte 0x080e4abc
	.4byte 0x080e4ac0
	.4byte 0x080e4ab8
	.4byte 0x080e4ac0
	.4byte 0x080e4ac4
	.4byte 0x080e4ab8
	.4byte 0x080e4ab8
	.4byte 0x080e4ab8
	.4byte 0x080e4ab8
	.4byte 0x080e4ab8
	.4byte 0x080e4ab8
	.4byte 0x080e4ab8
	.4byte 0x080e4abc
	.4byte 0x080e4abc
	.4byte 0x080e4abc
	.4byte 0x080e4abc
	.4byte 0x080e4abc
	.4byte 0x080e4abc
	.4byte 0x080e4ac0
	.4byte 0x080e4ac0
	.4byte 0x080e4ac0
	.4byte 0x080e4ac0
	.4byte 0x080e4ac0
	.4byte 0x080e4ac0
	.4byte 0x080e4ac4
	.4byte 0x080e4ac4
	.4byte 0x080e4ac4
	.4byte 0x080e4ac4
	.4byte 0x080e4ac4
	.4byte 0x080e4ac4
	.4byte 0x080e4ac4
	.4byte 0x080e4ab8
	.4byte 0x080e4ac0
	.4byte 0x080e4ac0
	.4byte 0x080e4ade
	.4byte 0x080e4ade
	.4byte 0x080e4ade
	.4byte 0x080e4ade
	.4byte 0x080e4ade
	.4byte 0x080e4ade
	.4byte 0x080e4ade
	.4byte 0x080e4ade
	.4byte 0x080e4ade
	.4byte 0x080e4ade
	.4byte 0x080e4ade
	.4byte 0x080e4ade
	.4byte 0x080e4ade
	.4byte 0x080e4ade
	.4byte 0x080e4ade
	.4byte 0x080e4ade
	.4byte 0x080e4ade
	.4byte 0x080e4ade
	.4byte 0x080e4ade
	.4byte 0x080e4ade
	.4byte 0x080e4ade
	.4byte 0x080e4ade
	.4byte 0x080e4ade
	.4byte 0x080e4ade
	.4byte 0x080e4ade
	.4byte 0x080e4ade
	.4byte 0x080e4ade
	.4byte 0x080e4ade
	.4byte 0x080e4ade
	.4byte 0x080e4ade
	.4byte 0x080e4ade
	.4byte 0x080e4ade
	.4byte 0x080e4ade
	.4byte 0x080e4ade
	.4byte 0x080e4ade
	.4byte 0x080e4ade
	.4byte 0x080e4ade
	.4byte 0x080e4ade
	.4byte 0x080e4ade
	.4byte 0x080e4ade
	.4byte 0x080e4ade
	.4byte 0x080e4ade
	.4byte 0x080e4ade
	.4byte 0x080e4ade
	.4byte 0x080e4ade
	.4byte 0x080e4ade
	.4byte 0x080e4ade
	.4byte 0x080e4ade
	.4byte 0x080e4ade
	.4byte 0x080e4ade
	.4byte 0x080e4ade
	.4byte 0x080e4ade
	.4byte 0x080e4ade
	.4byte 0x080e4ade
	.4byte 0x080e4ade
	.4byte 0x080e4ade
	.4byte 0x080e4ade
	.4byte 0x080e4ade
	.4byte 0x080e4ade
	.4byte 0x080e4ade
	.4byte 0x080e4ade
	.4byte 0x080e4ade
	.4byte 0x080e4ade
	.4byte 0x080e4ade
	.2byte 0x4ad2
	.2byte 0x080e
	ldr	r0, [pc, #120]
	b.n	.L_080e4ac6
	ldr	r0, [pc, #120]
	b.n	.L_080e4ac6
	ldr	r0, [pc, #120]
	b.n	.L_080e4ac6
	ldr	r0, [pc, #120]
.L_080e4ac6:
	ldr	r1, [pc, #124]
	movs	r2, #1
	movs	r3, #1
	bl	sub_080e0524
	b.n	.L_080e4ade
	ldr	r0, [pc, #100]
	ldr	r1, [pc, #108]
	movs	r2, #1
	movs	r3, #1
	bl	sub_080e0524
.L_080e4ade:
	ldr	r3, [sp, #92]
	movs	r5, #239
	lsls	r5, r5, #7
	adds	r2, r3, r5
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r0, [sp, #96]
	cmp	r0, #12
	bne.n	.L_080e4b4c
	ldr	r1, [sp, #92]
	ldr	r3, [pc, #84]
	adds	r2, r1, r3
	movs	r3, #75
	b.n	.L_080e4b54
	movs	r0, r0
	.4byte 0x0000007d
	.4byte 0x000000a9
	.4byte 0x000000ce
	.4byte 0x02010000
	.4byte 0x000000c4
	.4byte 0x02010c56
	.4byte 0x00000079
	.4byte 0x000000c3
	.4byte 0x0000006f
	.4byte 0x000000b8
	.4byte 0x000000b4
	.4byte 0x00000053
	.4byte 0x0000009e
	.4byte 0x080e4924
	.4byte 0x00000094
	.4byte 0x00000092
	.4byte 0x0000008e
	.4byte 0x00000090
	.4byte 0x02013c56
	.2byte 0x7784
	.2byte 0x0000
.L_080e4b4c:
	ldr	r5, [sp, #92]
	ldr	r0, [pc, #656]
	movs	r3, #50
	adds	r2, r5, r0
.L_080e4b54:
	str	r3, [r2, #0]
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #648]
	bl	sub_080041d8
	ldr	r2, [pc, #644]
	ldr	r1, [sp, #92]
	adds	r5, r1, r2
	ldr	r3, [r5, #0]
	mov	r2, sp
	adds	r2, #160
	movs	r1, #36
	ldrsh	r0, [r3, r1]
	adds	r1, r2, #0
	str	r2, [sp, #60]
	bl	sub_080e396c
	ldr	r3, [r5, #0]
	ldr	r0, [r3, #8]
	mov	r3, sp
	adds	r3, #172
	adds	r1, r3, #0
	str	r3, [sp, #56]
	bl	sub_080e396c
	ldr	r3, [r5, #0]
	mov	r1, sp
	ldr	r0, [r3, #4]
	adds	r1, #104
	str	r1, [sp, #52]
	bl	sub_080cef64
	ldr	r0, [pc, #596]
	ldr	r3, [sp, #92]
	adds	r2, r3, r0
	movs	r3, #24
	str	r3, [r2, #0]
	ldr	r1, [sp, #92]
	ldr	r3, [pc, #588]
	adds	r2, r1, r3
	movs	r3, #0
	str	r3, [r2, #0]
	ldr	r3, [r5, #0]
	ldr	r0, [r3, #8]
	bl	sub_080b5098
	movs	r1, #225
	ldr	r7, [r0, #0]
	ldr	r0, [sp, #92]
	movs	r5, #0
	lsls	r1, r1, #7
	ldr	r6, [pc, #564]
	mov	sl, r5
	mov	r8, r5
	adds	r5, r0, r1
.L_080e4bc4:
	bl	sub_08004458
	movs	r3, #63
	ands	r3, r0
	adds	r3, #32
	mov	r2, r8
	str	r3, [r5, #0]
	str	r2, [r5, #4]
	str	r2, [r5, #8]
	bl	sub_08004458
	ands	r0, r6
	str	r0, [r5, #12]
	bl	sub_08004458
	ands	r0, r6
	str	r0, [r5, #16]
	bl	sub_08004458
	movs	r3, #1
	ands	r0, r6
	add	sl, r3
	str	r0, [r5, #20]
	mov	r0, sl
	adds	r5, #28
	cmp	r0, #64
	bne.n	.L_080e4bc4
	adds	r0, r7, #0
	movs	r1, #0
	bl	sub_08009088
	mov	r1, sp
	adds	r1, #148
	str	r1, [sp, #8]
	ldr	r3, [r7, #8]
	str	r3, [r1, #0]
	ldr	r3, [r7, #12]
	movs	r2, #160
	lsls	r2, r2, #15
	adds	r3, r3, r2
	str	r3, [r1, #4]
	ldr	r3, [r7, #16]
	str	r3, [r1, #8]
	ldr	r3, [r7, #36]
	str	r3, [sp, #48]
	ldr	r5, [r7, #40]
	str	r5, [sp, #44]
	ldr	r0, [r7, #44]
	str	r0, [sp, #40]
	ldr	r1, [r7, #52]
	str	r1, [sp, #36]
	ldr	r2, [r7, #72]
	movs	r3, #0
	str	r2, [sp, #32]
	str	r3, [r7, #36]
	str	r3, [r7, #40]
	str	r3, [r7, #44]
	str	r3, [r7, #52]
	str	r3, [r7, #72]
	ldr	r5, [sp, #92]
	ldr	r0, [pc, #424]
	adds	r3, r5, r0
	ldr	r3, [r3, #0]
	ldr	r1, [sp, #56]
	ldr	r0, [r3, #8]
	bl	sub_080e396c
	ldr	r1, [sp, #56]
	ldr	r3, [r1, #0]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [r1, #0]
	movs	r0, #212
	bl	sub_080f9010
	movs	r2, #0
	add	r3, sp, #124
	str	r2, [sp, #84]
	mov	r8, r3
.L_080e4c64:
	ldr	r0, [sp, #92]
	movs	r1, #225
	movs	r5, #0
	lsls	r1, r1, #7
	mov	r9, r5
	mov	sl, r5
	adds	r6, r0, r1
.L_080e4c72:
	ldr	r3, [r6, #0]
	cmp	r3, #0
	blt.n	.L_080e4d34
	mov	r3, sl
	cmp	r3, #0
	bge.n	.L_080e4c80
	adds	r3, #3
.L_080e4c80:
	ldr	r2, [sp, #84]
	asrs	r3, r3, #2
	cmp	r2, r3
	blt.n	.L_080e4d30
	movs	r3, #5
	mov	fp, r3
	bl	sub_080049ac
	ldr	r0, [r6, #20]
	bl	sub_08004c6c
	ldr	r0, [r6, #12]
	bl	sub_08004bd4
	ldr	r0, [r6, #16]
	bl	sub_08004c1c
	add	r5, sp, #124
	adds	r1, r5, #0
	adds	r0, r6, #0
	bl	sub_080e3944
	ldr	r3, [r5, #0]
	ldr	r0, [sp, #56]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	ldr	r2, [r0, #0]
	asrs	r3, r3, #1
	adds	r3, r3, r2
	str	r3, [r5, #0]
	ldr	r1, [sp, #96]
	cmp	r1, #7
	bgt.n	.L_080e4ccc
	ldr	r3, [r5, #4]
	ldr	r2, [r0, #4]
	adds	r3, r3, r2
	subs	r3, #8
	b.n	.L_080e4ce8
.L_080e4ccc:
	ldr	r2, [sp, #96]
	cmp	r2, #35
	bne.n	.L_080e4cde
	ldr	r0, [sp, #56]
	ldr	r3, [r5, #4]
	ldr	r2, [r0, #4]
	adds	r3, r3, r2
	adds	r3, #44
	b.n	.L_080e4ce8
.L_080e4cde:
	ldr	r1, [sp, #56]
	ldr	r3, [r5, #4]
	ldr	r2, [r1, #4]
	adds	r3, r3, r2
	adds	r3, #12
.L_080e4ce8:
	str	r3, [r5, #4]
	movs	r2, #60
	ldr	r3, [r5, #8]
	negs	r2, r2
	cmp	r3, r2
	bge.n	.L_080e4cf8
	str	r2, [r5, #8]
	adds	r3, r2, #0
.L_080e4cf8:
	cmp	r3, #60
	ble.n	.L_080e4d00
	movs	r3, #60
	str	r3, [r5, #8]
.L_080e4d00:
	adds	r3, #60
	str	r3, [r5, #8]
	ldr	r2, [pc, #240]
	movs	r3, #10
	subs	r3, #2
	ldrh	r1, [r2, r3]
	ldr	r2, [sp, #76]
	ldr	r3, [r5, #4]
	adds	r1, r2, r1
	movs	r0, #10
	ldr	r2, [r5, #0]
	mov	r5, fp
	str	r5, [sp, #0]
	str	r0, [sp, #4]
	ldr	r5, [sp, #52]
	subs	r3, #5
	subs	r2, #2
	ldr	r4, [r5, #4]
	ldr	r0, [sp, #88]
	bl	sub_080072f4
	ldr	r3, [r6, #0]
	subs	r3, #4
	str	r3, [r6, #0]
.L_080e4d30:
	movs	r0, #1
	add	r9, r0
.L_080e4d34:
	movs	r1, #1
	add	sl, r1
	mov	r2, sl
	adds	r6, #28
	cmp	r2, #64
	bne.n	.L_080e4c72
	ldr	r3, [sp, #96]
	cmp	r3, #7
	bgt.n	.L_080e4d8a
	mov	r5, r9
	cmp	r5, #63
	bgt.n	.L_080e4d8a
	bl	sub_080049ac
	ldr	r0, [sp, #80]
	adds	r1, r0, #0
	adds	r1, #12
	bl	sub_080051d8
	add	r1, sp, #148
	adds	r0, r1, #0
	mov	r1, r8
	bl	sub_080e3944
	mov	r3, r8
	ldr	r2, [r3, #0]
	lsrs	r3, r2, #31
	adds	r2, r2, r3
	mov	r5, r8
	ldr	r3, [r5, #4]
	asrs	r2, r2, #1
	movs	r1, #20
	str	r2, [r5, #0]
	str	r1, [sp, #0]
	movs	r1, #40
	str	r1, [sp, #4]
	subs	r2, #10
	subs	r3, #4
	ldr	r4, [sp, #104]
	ldr	r0, [sp, #88]
	ldr	r1, [pc, #116]
	bl	sub_080072f4
.L_080e4d8a:
	ldr	r0, [sp, #92]
	ldr	r1, [pc, #112]
	movs	r3, #1
	adds	r2, r0, r1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_080030f8
	ldr	r2, [sp, #84]
	adds	r2, #1
	str	r2, [sp, #84]
	cmp	r2, #32
	beq.n	.L_080e4da6
	b.n	.L_080e4c64
.L_080e4da6:
	ldr	r3, [sp, #96]
	cmp	r3, #11
	bne.n	sub_080e4e0c
	ldr	r3, [pc, #44]
	ldr	r2, [pc, #84]
	strh	r3, [r2, #0]
	ldr	r5, [sp, #92]
	ldr	r0, [pc, #48]
	adds	r3, r5, r0
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #4]
	cmp	r3, #0
	bne.n	.L_080e4dcc
	ldr	r1, [sp, #60]
	ldr	r5, [sp, #84]
	ldr	r3, [r1, #0]
	subs	r3, r5, r3
	adds	r2, #8
	b.n	sub_080e4e54
.L_080e4dcc:
	ldr	r0, [sp, #60]
	ldr	r2, [r0, #0]
	movs	r3, #96
	ldr	r1, [pc, #52]
	subs	r3, r3, r2
	lsls	r3, r3, #8
	str	r3, [r1, #0]
	b.n	sub_080e4e58
