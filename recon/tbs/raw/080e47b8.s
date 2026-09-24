.syntax unified
	.thumb
	.set sub_080022ec, 0x080022ec
	.set sub_080022fc, 0x080022fc
	.set sub_0800231c, 0x0800231c
	.set sub_08002322, 0x08002322
	.set sub_08002dd8, 0x08002dd8
	.set sub_080030f8, 0x080030f8
	.set sub_080041d8, 0x080041d8
	.set sub_08004278, 0x08004278
	.set sub_08004458, 0x08004458
	.set sub_080049ac, 0x080049ac
	.set sub_08004bd4, 0x08004bd4
	.set sub_08004c1c, 0x08004c1c
	.set sub_08004c6c, 0x08004c6c
	.set sub_080051d8, 0x080051d8
	.set sub_080072f0, 0x080072f0
	.set sub_080072f4, 0x080072f4
	.set sub_080072f8, 0x080072f8
	.set sub_08009080, 0x08009080
	.set sub_08009088, 0x08009088
	.set sub_080b5070, 0x080b5070
	.set sub_080b5088, 0x080b5088
	.set sub_080b5098, 0x080b5098
	.set sub_080b50e8, 0x080b50e8
	.set sub_080cd52c, 0x080cd52c
	.set sub_080cd594, 0x080cd594
	.set sub_080cdb24, 0x080cdb24
	.set sub_080cdbc0, 0x080cdbc0
	.set sub_080cef64, 0x080cef64
	.set sub_080d4604, 0x080d4604
	.set sub_080d52a4, 0x080d52a4
	.set sub_080d6888, 0x080d6888
	.set sub_080d9ac4, 0x080d9ac4
	.set sub_080dea70, 0x080dea70
	.set sub_080df9d0, 0x080df9d0
	.set sub_080e0524, 0x080e0524
	.set sub_080e155c, 0x080e155c
	.set sub_080e38b8, 0x080e38b8
	.set sub_080e3944, 0x080e3944
	.set sub_080e396c, 0x080e396c
	.set sub_080e46f0, 0x080e46f0
	.set sub_080e67a2, 0x080e67a2
	.set sub_080ed408, 0x080ed408
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
	bne.n	.L_080e4e0c
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
	b.n	.L_080e4e54
.L_080e4dcc:
	ldr	r0, [sp, #60]
	ldr	r2, [r0, #0]
	movs	r3, #96
	ldr	r1, [pc, #52]
	subs	r3, r3, r2
	lsls	r3, r3, #8
	str	r3, [r1, #0]
	b.n	.L_080e4e58
	.4byte 0x00000100
	.4byte 0x00007784
	.4byte 0x080cd261
	.4byte 0x00007828
	.4byte 0x000077b4
	.4byte 0x000077b8
	.4byte 0x0000ffff
	.4byte 0x080ede48
	.4byte 0x02013c56
	.4byte 0x00007824
	.4byte 0x04000020
	.2byte 0x0028
	.2byte 0x0400
.L_080e4e0c:
	ldr	r1, [sp, #96]
	cmp	r1, #32
	bne.n	.L_080e4e58
	ldr	r2, [pc, #36]
	ldr	r3, [pc, #28]
	strh	r3, [r2, #0]
	ldr	r2, [sp, #92]
	ldr	r5, [pc, #32]
	adds	r3, r2, r5
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #4]
	cmp	r3, #0
	bne.n	.L_080e4e44
	ldr	r0, [pc, #24]
	movs	r1, #192
	lsls	r1, r1, #12
	str	r0, [sp, #72]
	str	r1, [sp, #68]
	b.n	.L_080e4e4e
	movs	r0, r0
	.4byte 0x00000100
	.4byte 0x04000020
	.4byte 0x00007828
	.2byte 0x0000
	.2byte 0xff80
.L_080e4e44:
	.2byte 0x2280
	ldr	r3, [pc, #88]
	lsls	r2, r2, #12
	str	r2, [sp, #72]
	str	r3, [sp, #68]
.L_080e4e4e:
	ldr	r5, [sp, #72]
	ldr	r2, [pc, #80]
	asrs	r3, r5, #16
.L_080e4e54:
	lsls	r3, r3, #8
	str	r3, [r2, #0]
.L_080e4e58:
	ldr	r0, [sp, #96]
	cmp	r0, #8
	bne.n	.L_080e4ec0
	ldr	r2, [pc, #72]
	ldr	r3, [pc, #56]
	strh	r3, [r2, #0]
	ldr	r3, [sp, #60]
	ldr	r2, [r3, #0]
	movs	r3, #64
	ldr	r1, [pc, #56]
	subs	r3, r3, r2
	lsls	r3, r3, #8
	str	r3, [r1, #0]
	ldr	r5, [sp, #92]
	movs	r0, #239
	lsls	r0, r0, #7
	ldr	r1, [pc, #48]
	adds	r2, r5, r0
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r6, #0
	adds	r3, r5, r1
	movs	r1, #128
	str	r6, [r3, #0]
	ldr	r5, [pc, #36]
	lsls	r1, r1, #7
	ldr	r0, [pc, #36]
	bl	sub_080072f8
	movs	r1, #128
	ldr	r0, [sp, #88]
	lsls	r1, r1, #7
	b.n	.L_080e4eb8
	movs	r0, r0
	.4byte 0x00000100
	.4byte 0xfff40000
	.4byte 0x04000028
	.4byte 0x04000020
	.4byte 0x00007784
	.4byte 0x03000164
	.2byte 0x4000
	.2byte 0x0600
.L_080e4eb8:
	bl	sub_080072f8
	ldr	r3, [pc, #40]
	strh	r6, [r3, #0]
.L_080e4ec0:
	ldr	r2, [sp, #96]
	cmp	r2, #31
	bne.n	.L_080e4f02
	ldr	r3, [pc, #28]
	ldr	r2, [pc, #32]
	strh	r3, [r2, #0]
	ldr	r5, [sp, #92]
	ldr	r0, [pc, #32]
	adds	r3, r5, r0
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #4]
	cmp	r3, #0
	bne.n	.L_080e4ef4
	ldr	r3, [sp, #60]
	ldr	r2, [r3, #0]
	movs	r3, #32
	b.n	.L_080e4efa
	movs	r0, r0
	.4byte 0x00000100
	.4byte 0x04000050
	.4byte 0x04000020
	.2byte 0x7828
	.2byte 0x0000
.L_080e4ef4:
	ldr	r5, [sp, #60]
	ldr	r2, [r5, #0]
	movs	r3, #96
.L_080e4efa:
	ldr	r1, [pc, #448]
	subs	r3, r3, r2
	lsls	r3, r3, #8
	str	r3, [r1, #0]
.L_080e4f02:
	ldr	r0, [sp, #96]
	cmp	r0, #15
	beq.n	.L_080e4f14
	cmp	r0, #17
	beq.n	.L_080e4f14
	cmp	r0, #24
	beq.n	.L_080e4f14
	cmp	r0, #26
	bne.n	.L_080e4f86
.L_080e4f14:
	movs	r1, #128
	ldr	r5, [pc, #424]
	lsls	r1, r1, #7
	ldr	r0, [pc, #424]
	bl	sub_080072f8
	movs	r1, #128
	ldr	r0, [sp, #88]
	lsls	r1, r1, #7
	bl	sub_080072f8
	ldr	r2, [pc, #412]
	ldr	r1, [sp, #92]
	adds	r3, r1, r2
	ldr	r2, [r3, #0]
	movs	r3, #0
	str	r3, [r2, #28]
	ldr	r0, [pc, #404]
	bl	sub_08004278
	ldr	r0, [pc, #400]
	bl	sub_08004278
	movs	r0, #47
	bl	sub_08002dd8
	movs	r0, #46
	bl	sub_08002dd8
	adds	r0, r7, #0
	movs	r1, #3
	bl	sub_08009080
	ldr	r3, [sp, #96]
	cmp	r3, #15
	bne.n	.L_080e4f64
	ldr	r0, [sp, #100]
	movs	r1, #9
	bl	sub_080dea70
.L_080e4f64:
	ldr	r5, [sp, #96]
	cmp	r5, #24
	bne.n	.L_080e4f70
	ldr	r0, [sp, #100]
	bl	sub_080d52a4
.L_080e4f70:
	ldr	r0, [sp, #96]
	cmp	r0, #26
	beq.n	.L_080e4f7a
	bl	.L_080e65f8
.L_080e4f7a:
	ldr	r0, [sp, #100]
	movs	r1, #8
	bl	sub_080dea70
	bl	.L_080e65f8
.L_080e4f86:
	adds	r0, r7, #0
	movs	r1, #16
	bl	sub_08009088
	ldr	r1, [sp, #48]
	str	r1, [r7, #36]
	ldr	r2, [sp, #44]
	str	r2, [r7, #40]
	ldr	r3, [sp, #40]
	str	r3, [r7, #44]
	ldr	r5, [sp, #36]
	str	r5, [r7, #52]
	ldr	r0, [sp, #32]
	str	r0, [r7, #72]
	ldr	r1, [sp, #96]
	cmp	r1, #35
	bne.n	.L_080e4ff4
	movs	r1, #128
	ldr	r5, [pc, #276]
	lsls	r1, r1, #7
	ldr	r0, [pc, #276]
	bl	sub_080072f8
	movs	r1, #128
	lsls	r1, r1, #7
	ldr	r0, [sp, #88]
	bl	sub_080072f8
	ldr	r5, [pc, #264]
	ldr	r2, [sp, #92]
	adds	r3, r2, r5
	ldr	r2, [r3, #0]
	movs	r3, #0
	str	r3, [r2, #28]
	ldr	r0, [pc, #256]
	bl	sub_08004278
	ldr	r0, [pc, #252]
	bl	sub_08004278
	movs	r0, #47
	bl	sub_08002dd8
	movs	r0, #46
	bl	sub_08002dd8
	ldr	r0, [sp, #100]
	movs	r3, #3
	str	r3, [r0, #24]
	ldr	r0, [sp, #100]
	movs	r1, #2
	bl	sub_080d4604
	bl	.L_080e65f8
.L_080e4ff4:
	ldr	r1, [sp, #92]
	ldr	r2, [pc, #208]
	adds	r3, r1, r2
	ldr	r3, [r3, #0]
	movs	r5, #36
	ldrsh	r0, [r3, r5]
	bl	sub_080b5098
	ldr	r0, [r0, #0]
	str	r0, [sp, #28]
	mov	r0, sp
	adds	r0, #136
	ldr	r1, [sp, #28]
	ldr	r2, [sp, #8]
	str	r0, [sp, #24]
	ldr	r3, [r2, #0]
	ldr	r0, [r1, #8]
	movs	r1, #6
	subs	r0, r0, r3
	bl	sub_080022ec
	ldr	r3, [sp, #24]
	str	r0, [r3, #0]
	ldr	r5, [sp, #28]
	ldr	r1, [sp, #8]
	ldr	r0, [r5, #12]
	ldr	r3, [r1, #4]
	movs	r2, #240
	lsls	r2, r2, #13
	subs	r0, r0, r3
	adds	r0, r0, r2
	movs	r1, #6
	bl	sub_080022ec
	ldr	r3, [sp, #24]
	str	r0, [r3, #4]
	ldr	r0, [r5, #16]
	ldr	r5, [sp, #8]
	ldr	r3, [r5, #8]
	movs	r1, #6
	subs	r0, r0, r3
	bl	sub_080022ec
	ldr	r1, [sp, #24]
	str	r0, [r1, #8]
	ldr	r5, [sp, #92]
	ldr	r0, [pc, #128]
	movs	r2, #0
	mov	sl, r2
	adds	r3, r5, r0
.L_080e5058:
	movs	r1, #1
	add	sl, r1
	mov	r5, sl
	str	r2, [r3, #0]
	adds	r3, #28
	cmp	r5, #64
	bne.n	.L_080e5058
	ldr	r0, [sp, #96]
	cmp	r0, #14
	beq.n	.L_080e5114
	ldr	r1, [sp, #92]
	ldr	r2, [pc, #88]
	adds	r3, r1, r2
	ldr	r3, [r3, #0]
	movs	r5, #36
	ldrsh	r0, [r3, r5]
	bl	sub_080b5070
	lsrs	r3, r0, #31
	adds	r0, r0, r3
	ldr	r1, [sp, #92]
	movs	r2, #225
	asrs	r7, r0, #1
	lsls	r2, r2, #7
	movs	r0, #0
	mov	sl, r0
	movs	r6, #255
	adds	r5, r1, r2
.L_080e5090:
	ldr	r0, [sp, #28]
	ldr	r3, [r0, #8]
	str	r7, [r5, #4]
	str	r3, [r5, #0]
	ldr	r3, [r0, #16]
	str	r3, [r5, #8]
	ldr	r1, [sp, #96]
	cmp	r1, #31
	bne.n	.L_080e50d8
	bl	sub_08004458
	ands	r0, r6
	subs	r0, #127
	lsls	r0, r0, #12
	str	r0, [r5, #12]
	bl	sub_08004458
	ands	r0, r6
	subs	r0, #64
	lsls	r0, r0, #10
	b.n	.L_080e50ee
	movs	r0, r0
	.4byte 0x04000028
	.4byte 0x03000164
	.4byte 0x06004000
	.4byte 0x00007828
	.4byte 0x080cd4b5
	.4byte 0x080cd261
	.2byte 0x7098
	.2byte 0x0000
.L_080e50d8:
	bl	sub_08004458
	ands	r0, r6
	subs	r0, #127
	lsls	r0, r0, #12
	str	r0, [r5, #12]
	bl	sub_08004458
	ands	r0, r6
	subs	r0, #64
	lsls	r0, r0, #12
.L_080e50ee:
	str	r0, [r5, #16]
	bl	sub_08004458
	ands	r0, r6
	subs	r0, #127
	lsls	r0, r0, #12
	str	r0, [r5, #20]
	mov	r2, sl
	lsrs	r3, r2, #31
	add	r3, sl
	asrs	r3, r3, #1
	adds	r3, #32
	str	r3, [r5, #24]
	movs	r3, #1
	add	sl, r3
	mov	r0, sl
	adds	r5, #28
	cmp	r0, #32
	bne.n	.L_080e5090
.L_080e5114:
	ldr	r1, [sp, #96]
	cmp	r1, #11
	bne.n	.L_080e5138
	ldr	r1, [sp, #92]
	ldr	r0, [pc, #68]
	movs	r2, #1
	movs	r3, #1
	bl	sub_080e0524
	movs	r2, #1
	movs	r3, #0
	ldr	r0, [pc, #60]
	ldr	r1, [pc, #60]
	bl	sub_080e0524
	ldr	r2, [pc, #60]
	ldr	r3, [pc, #40]
	strh	r3, [r2, #0]
.L_080e5138:
	ldr	r2, [sp, #96]
	cmp	r2, #32
	bne.n	.L_080e517c
	ldr	r1, [sp, #92]
	ldr	r0, [pc, #48]
	movs	r2, #1
	movs	r3, #1
	bl	sub_080e0524
	movs	r2, #1
	movs	r3, #0
	ldr	r0, [pc, #40]
	ldr	r1, [pc, #24]
	bl	sub_080e0524
	ldr	r2, [pc, #24]
	ldr	r3, [pc, #4]
	strh	r3, [r2, #0]
	b.n	.L_080e517c
	movs	r0, r0
	.4byte 0x00000e10
	.4byte 0x000000ab
	.4byte 0x000000ac
	.4byte 0x02010000
	.4byte 0x04000052
	.4byte 0x000000ad
	.2byte 0x00ae
	.2byte 0x0000
.L_080e517c:
	ldr	r3, [sp, #96]
	cmp	r3, #7
	beq.n	.L_080e523e
	cmp	r3, #13
	beq.n	.L_080e523e
	cmp	r3, #18
	beq.n	.L_080e523e
	cmp	r3, #11
	beq.n	.L_080e523e
	cmp	r3, #32
	beq.n	.L_080e523e
	cmp	r3, #19
	beq.n	.L_080e523e
	movs	r7, #0
	cmp	r3, #12
	beq.n	.L_080e51a0
	movs	r7, #160
	lsls	r7, r7, #13
.L_080e51a0:
	movs	r5, #0
	mov	sl, r5
	ldr	r5, [pc, #804]
	movs	r6, #255
.L_080e51a8:
	ldr	r0, [sp, #28]
	ldr	r3, [r0, #8]
	str	r7, [r5, #4]
	str	r3, [r5, #0]
	ldr	r3, [r0, #16]
	str	r3, [r5, #8]
	ldr	r1, [sp, #96]
	cmp	r1, #5
	beq.n	.L_080e51be
	cmp	r1, #23
	bne.n	.L_080e51e0
.L_080e51be:
	bl	sub_08004458
	ands	r0, r6
	subs	r0, #127
	lsls	r0, r0, #11
	str	r0, [r5, #12]
	bl	sub_08004458
	ands	r0, r6
	lsls	r0, r0, #11
	str	r0, [r5, #16]
	bl	sub_08004458
	ands	r0, r6
	subs	r0, #127
	lsls	r0, r0, #11
	b.n	.L_080e522c
.L_080e51e0:
	ldr	r2, [sp, #96]
	cmp	r2, #25
	bne.n	.L_080e520a
	bl	sub_08004458
	ands	r0, r6
	subs	r0, #127
	lsls	r0, r0, #11
	str	r0, [r5, #12]
	bl	sub_08004458
	movs	r3, #127
	ands	r3, r0
	lsls	r3, r3, #10
	str	r3, [r5, #16]
	bl	sub_08004458
	ands	r0, r6
	subs	r0, #127
	lsls	r0, r0, #11
	b.n	.L_080e522c
.L_080e520a:
	bl	sub_08004458
	ands	r0, r6
	subs	r0, #127
	lsls	r0, r0, #10
	str	r0, [r5, #12]
	bl	sub_08004458
	movs	r3, #127
	ands	r3, r0
	lsls	r3, r3, #10
	str	r3, [r5, #16]
	bl	sub_08004458
	ands	r0, r6
	subs	r0, #127
	lsls	r0, r0, #10
.L_080e522c:
	str	r0, [r5, #20]
	movs	r3, #0
	str	r3, [r5, #24]
	movs	r3, #1
	add	sl, r3
	mov	r0, sl
	adds	r5, #28
	cmp	r0, #64
	bne.n	.L_080e51a8
.L_080e523e:
	ldr	r1, [sp, #96]
	subs	r1, #2
	str	r1, [sp, #20]
	cmp	r1, #1
	bls.n	.L_080e525a
	ldr	r2, [sp, #96]
	cmp	r2, #12
	beq.n	.L_080e525a
	cmp	r2, #22
	beq.n	.L_080e525a
	cmp	r2, #29
	beq.n	.L_080e525a
	cmp	r2, #28
	bne.n	.L_080e5264
.L_080e525a:
	movs	r1, #144
	ldr	r0, [pc, #624]
	lsls	r1, r1, #3
	bl	sub_080041d8
.L_080e5264:
	ldr	r3, [sp, #96]
	subs	r3, #4
	str	r3, [sp, #16]
	cmp	r3, #2
	bls.n	.L_080e5288
	ldr	r5, [sp, #96]
	cmp	r5, #23
	beq.n	.L_080e5288
	cmp	r5, #30
	beq.n	.L_080e5288
	cmp	r5, #27
	beq.n	.L_080e5288
	cmp	r5, #33
	beq.n	.L_080e5288
	cmp	r5, #34
	beq.n	.L_080e5288
	cmp	r5, #100
	bne.n	.L_080e528e
.L_080e5288:
	movs	r0, #32
	str	r0, [sp, #64]
	b.n	.L_080e52ea
.L_080e528e:
	ldr	r1, [sp, #96]
	cmp	r1, #3
	bls.n	.L_080e52b4
	cmp	r1, #8
	beq.n	.L_080e52b4
	cmp	r1, #9
	beq.n	.L_080e52b4
	cmp	r1, #10
	beq.n	.L_080e52b4
	cmp	r1, #22
	beq.n	.L_080e52b4
	cmp	r1, #25
	beq.n	.L_080e52b4
	cmp	r1, #29
	beq.n	.L_080e52b4
	cmp	r1, #31
	beq.n	.L_080e52b4
	cmp	r1, #14
	bne.n	.L_080e52ba
.L_080e52b4:
	movs	r2, #48
	str	r2, [sp, #64]
	b.n	.L_080e52ea
.L_080e52ba:
	ldr	r5, [sp, #96]
	movs	r3, #20
	str	r3, [sp, #64]
	cmp	r5, #21
	beq.n	.L_080e52ea
	ldr	r0, [sp, #96]
	cmp	r0, #11
	beq.n	.L_080e52d2
	cmp	r0, #32
	beq.n	.L_080e52d2
	cmp	r0, #20
	bne.n	.L_080e52d8
.L_080e52d2:
	movs	r1, #40
	str	r1, [sp, #64]
	b.n	.L_080e52ea
.L_080e52d8:
	ldr	r2, [sp, #96]
	cmp	r2, #28
	beq.n	.L_080e52e6
	movs	r3, #80
	str	r3, [sp, #64]
	cmp	r2, #12
	bne.n	.L_080e52ea
.L_080e52e6:
	movs	r5, #64
	str	r5, [sp, #64]
.L_080e52ea:
	ldr	r1, [sp, #64]
	movs	r0, #0
	str	r0, [sp, #84]
	cmp	r1, #0
	bne.n	.L_080e52f8
	bl	.L_080e657c
.L_080e52f8:
	ldr	r2, [sp, #96]
	cmp	r2, #11
	beq.n	.L_080e5334
	cmp	r2, #32
	beq.n	.L_080e5334
	ldr	r3, [sp, #92]
	movs	r5, #211
	ldr	r1, [sp, #84]
	lsls	r5, r5, #7
	movs	r0, #0
	movs	r7, #128
	adds	r6, r3, r5
	mov	sl, r0
	lsls	r7, r7, #11
	lsls	r5, r1, #12
.L_080e5316:
	adds	r0, r5, #0
	bl	sub_08002322
	lsls	r0, r0, #2
	subs	r0, r7, r0
	movs	r3, #1
	asrs	r0, r0, #10
	movs	r2, #128
	add	sl, r3
	stmia	r6!, {r0}
	lsls	r2, r2, #4
	mov	r0, sl
	adds	r5, r5, r2
	cmp	r0, #160
	bne.n	.L_080e5316
.L_080e5334:
	ldr	r1, [sp, #84]
	cmp	r1, #2
	bgt.n	.L_080e535c
	ldr	r2, [sp, #92]
	ldr	r5, [pc, #404]
	adds	r3, r2, r5
	ldr	r3, [r3, #0]
	ldr	r1, [sp, #56]
	ldr	r0, [r3, #8]
	bl	sub_080e396c
	ldr	r0, [sp, #56]
	ldr	r3, [r0, #0]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [r0, #0]
	ldr	r3, [r0, #4]
	adds	r3, #16
	str	r3, [r0, #4]
.L_080e535c:
	ldr	r1, [sp, #96]
	cmp	r1, #11
	beq.n	.L_080e53e2
	cmp	r1, #8
	beq.n	.L_080e53e2
	cmp	r1, #32
	beq.n	.L_080e53e2
	cmp	r1, #33
	beq.n	.L_080e53ea
	cmp	r1, #34
	beq.n	.L_080e53e2
	ldr	r2, [sp, #84]
	cmp	r2, #11
	bgt.n	.L_080e53e2
	ldr	r5, [sp, #92]
	ldr	r0, [pc, #344]
	adds	r3, r5, r0
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #4]
	cmp	r3, #0
	bne.n	.L_080e53b4
	lsrs	r3, r2, #31
	adds	r3, r2, r3
	asrs	r3, r3, #1
	lsls	r1, r3, #3
	subs	r1, r1, r3
	lsls	r1, r1, #2
	subs	r1, r1, r3
	ldr	r3, [sp, #56]
	movs	r0, #48
	ldr	r2, [r3, #0]
	ldr	r3, [r3, #4]
	lsls	r1, r1, #7
	str	r0, [sp, #0]
	movs	r0, #72
	str	r0, [sp, #4]
	adds	r1, r5, r1
	subs	r2, #32
	subs	r3, #40
	ldr	r4, [sp, #104]
	ldr	r0, [sp, #88]
	bl	sub_080072f4
	b.n	.L_080e53e2
.L_080e53b4:
	ldr	r5, [sp, #84]
	lsrs	r3, r5, #31
	adds	r3, r5, r3
	asrs	r3, r3, #1
	lsls	r1, r3, #3
	subs	r1, r1, r3
	lsls	r1, r1, #2
	subs	r1, r1, r3
	ldr	r0, [sp, #92]
	ldr	r3, [sp, #56]
	lsls	r1, r1, #7
	ldr	r2, [r3, #0]
	adds	r1, r0, r1
	ldr	r3, [r3, #4]
	movs	r0, #48
	str	r0, [sp, #0]
	movs	r0, #72
	str	r0, [sp, #4]
	subs	r3, #40
	ldr	r4, [sp, #104]
	ldr	r0, [sp, #88]
	bl	sub_080072f4
.L_080e53e2:
	ldr	r5, [sp, #96]
	cmp	r5, #33
	bls.n	.L_080e53ea
	b.n	.L_080e551a
.L_080e53ea:
	ldr	r0, [sp, #96]
	ldr	r2, [pc, #232]
	lsls	r3, r0, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	strb	r4, [r3, r2]
	lsrs	r6, r1, #32
	strb	r4, [r0, r3]
	lsrs	r6, r1, #32
	strb	r4, [r7, r2]
	lsrs	r6, r1, #32
	strb	r4, [r0, r4]
	lsrs	r6, r1, #32
	strb	r2, [r3, r4]
	lsrs	r6, r1, #32
	strb	r4, [r2, r4]
	lsrs	r6, r1, #32
	strb	r2, [r3, r4]
	lsrs	r6, r1, #32
	strb	r2, [r3, r4]
	lsrs	r6, r1, #32
	strb	r4, [r2, r2]
	lsrs	r6, r1, #32
	strb	r4, [r1, r4]
	lsrs	r6, r1, #32
	strb	r4, [r3, r2]
	lsrs	r6, r1, #32
	strb	r2, [r3, r4]
	lsrs	r6, r1, #32
	strb	r4, [r4, r2]
	lsrs	r6, r1, #32
	strb	r4, [r4, r2]
	lsrs	r6, r1, #32
	strb	r4, [r0, r2]
	lsrs	r6, r1, #32
	strb	r2, [r3, r4]
	lsrs	r6, r1, #32
	strb	r2, [r3, r4]
	lsrs	r6, r1, #32
	strb	r2, [r3, r4]
	lsrs	r6, r1, #32
	strb	r4, [r5, r2]
	lsrs	r6, r1, #32
	strb	r4, [r6, r2]
	lsrs	r6, r1, #32
	strb	r4, [r0, r4]
	lsrs	r6, r1, #32
	strb	r2, [r3, r4]
	lsrs	r6, r1, #32
	strb	r4, [r0, r4]
	lsrs	r6, r1, #32
	strb	r4, [r2, r4]
	lsrs	r6, r1, #32
	strb	r2, [r3, r4]
	lsrs	r6, r1, #32
	strb	r4, [r4, r2]
	lsrs	r6, r1, #32
	strb	r2, [r3, r4]
	lsrs	r6, r1, #32
	strb	r2, [r3, r4]
	lsrs	r6, r1, #32
	strb	r4, [r0, r3]
	lsrs	r6, r1, #32
	strb	r4, [r7, r2]
	lsrs	r6, r1, #32
	strb	r2, [r3, r4]
	lsrs	r6, r1, #32
	strb	r4, [r1, r2]
	lsrs	r6, r1, #32
	strb	r2, [r3, r4]
	lsrs	r6, r1, #32
	strb	r4, [r7, r1]
	lsrs	r6, r1, #32
	ldr	r0, [pc, #92]
	bl	sub_080e46f0
	b.n	.L_080e551a
	ldr	r0, [pc, #88]
	bl	sub_080e46f0
	b.n	.L_080e551a
	ldr	r0, [pc, #84]
	bl	sub_080e46f0
	b.n	.L_080e551a
	ldr	r0, [pc, #80]
	bl	sub_080e46f0
	b.n	.L_080e551a
	ldr	r0, [pc, #76]
	bl	sub_080e46f0
	b.n	.L_080e551a
	ldr	r0, [pc, #72]
	bl	sub_080e46f0
	b.n	.L_080e551a
	ldr	r0, [pc, #68]
	bl	sub_080e46f0
	b.n	.L_080e551a
	ldr	r0, [pc, #64]
	bl	sub_080e46f0
	b.n	.L_080e551a
	ldr	r0, [pc, #60]
	bl	sub_080e46f0
	b.n	.L_080e551a
	ldr	r0, [pc, #56]
	bl	sub_080e46f0
	b.n	.L_080e551a
	.4byte 0x02014000
	.4byte 0x080dbb9d
	.4byte 0x00007828
	.4byte 0x080e53f4
	.4byte 0x00000053
	.4byte 0x0000006f
	.4byte 0x00000079
	.4byte 0x000000c3
	.4byte 0x0000008d
	.4byte 0x000000bb
	.4byte 0x000000b9
	.4byte 0x000000c0
	.4byte 0x000000a4
	.2byte 0x00a3
	.2byte 0x0000
	ldr	r0, [pc, #648]
	bl	sub_080e46f0
	b.n	.L_080e551a
	ldr	r0, [pc, #644]
	bl	sub_080e46f0
	b.n	.L_080e551a
	ldr	r0, [pc, #640]
	bl	sub_080e46f0
.L_080e551a:
	ldr	r1, [sp, #96]
	cmp	r1, #11
	beq.n	.L_080e5608
	cmp	r1, #8
	beq.n	.L_080e5608
	cmp	r1, #32
	beq.n	.L_080e5608
	ldr	r2, [sp, #84]
	subs	r2, #4
	cmp	r2, #11
	bhi.n	.L_080e556a
	lsrs	r3, r2, #31
	adds	r3, r2, r3
	asrs	r3, r3, #1
	lsls	r1, r3, #4
	subs	r1, r1, r3
	ldr	r2, [sp, #92]
	ldr	r5, [sp, #60]
	lsls	r1, r1, #6
	adds	r1, r2, r1
	movs	r3, #162
	ldr	r2, [r5, #0]
	lsls	r3, r3, #7
	ldr	r0, [sp, #56]
	adds	r1, r1, r3
	lsrs	r3, r2, #31
	adds	r2, r2, r3
	ldr	r3, [r0, #4]
	movs	r0, #20
	str	r0, [sp, #0]
	movs	r0, #48
	str	r0, [sp, #4]
	ldr	r5, [sp, #52]
	asrs	r2, r2, #1
	subs	r2, #8
	subs	r3, #24
	ldr	r4, [r5, #4]
	ldr	r0, [sp, #88]
	bl	sub_080072f4
.L_080e556a:
	bl	sub_080049ac
	ldr	r1, [sp, #80]
	ldr	r0, [sp, #80]
	adds	r1, #12
	bl	sub_080051d8
	ldr	r0, [sp, #84]
	cmp	r0, #3
	ble.n	.L_080e5608
	movs	r2, #112
	movs	r1, #0
	add	r2, sp
	mov	sl, r1
	mov	r9, r2
.L_080e5588:
	mov	r5, sl
	lsrs	r3, r5, #31
	add	r3, sl
	asrs	r7, r3, #1
	lsls	r3, r7, #3
	subs	r3, r3, r7
	ldr	r0, [sp, #92]
	lsls	r3, r3, #2
	movs	r1, #225
	adds	r3, r0, r3
	lsls	r1, r1, #7
	adds	r1, r1, r3
	ldr	r5, [r1, #24]
	mov	r8, r1
	cmp	r5, #0
	ble.n	.L_080e55fe
	mov	r6, r9
	adds	r1, r6, #0
	mov	r0, r8
	bl	sub_080e3944
	ldr	r2, [r6, #0]
	asrs	r5, r5, #4
	lsrs	r3, r2, #31
	adds	r5, #1
	adds	r2, r2, r3
	lsls	r4, r5, #1
	asrs	r2, r2, #1
	ldr	r1, [pc, #472]
	str	r2, [r6, #0]
	subs	r3, r4, #2
	ldrh	r1, [r1, r3]
	ldr	r3, [sp, #76]
	adds	r1, r3, r1
	lsrs	r3, r5, #31
	adds	r3, r5, r3
	asrs	r3, r3, #1
	subs	r2, r2, r3
	ldr	r3, [r6, #4]
	movs	r0, #1
	subs	r3, r3, r5
	str	r5, [sp, #0]
	str	r4, [sp, #4]
	ldr	r5, [sp, #52]
	ands	r0, r7
	lsls	r0, r0, #2
	ldr	r4, [r0, r5]
	ldr	r0, [sp, #88]
	bl	sub_080072f4
	mov	r0, r8
	movs	r1, #60
	ldr	r2, [pc, #428]
	bl	sub_080e38b8
	mov	r0, r8
	ldr	r3, [r0, #24]
	subs	r3, #1
	str	r3, [r0, #24]
.L_080e55fe:
	movs	r1, #1
	add	sl, r1
	mov	r2, sl
	cmp	r2, #128
	bne.n	.L_080e5588
.L_080e5608:
	ldr	r3, [sp, #96]
	cmp	r3, #7
	beq.n	.L_080e561c
	cmp	r3, #13
	beq.n	.L_080e561c
	cmp	r3, #18
	beq.n	.L_080e561c
	cmp	r3, #19
	beq.n	.L_080e561c
	b.n	.L_080e57b4
.L_080e561c:
	ldr	r5, [sp, #84]
	cmp	r5, #50
	bne.n	.L_080e563c
	ldr	r0, [sp, #92]
	ldr	r1, [pc, #380]
	adds	r3, r0, r1
	ldr	r3, [r3, #0]
	ldr	r0, [r3, #8]
	movs	r3, #1
	movs	r2, #0
	negs	r3, r3
	str	r2, [sp, #0]
	movs	r1, #7
	adds	r2, r3, #0
	bl	sub_080d6888
.L_080e563c:
	ldr	r2, [sp, #84]
	cmp	r2, #79
	bne.n	.L_080e565c
	ldr	r0, [pc, #352]
	ldr	r5, [sp, #92]
	adds	r3, r5, r0
	ldr	r3, [r3, #0]
	ldr	r0, [r3, #8]
	movs	r3, #1
	movs	r2, #0
	negs	r3, r3
	str	r2, [sp, #0]
	movs	r1, #0
	adds	r2, r3, #0
	bl	sub_080d6888
.L_080e565c:
	ldr	r1, [sp, #84]
	cmp	r1, #12
	bne.n	.L_080e56ae
	movs	r2, #0
	ldr	r5, [pc, #320]
	mov	sl, r2
	movs	r6, #255
.L_080e566a:
	ldr	r0, [sp, #28]
	ldr	r3, [r0, #8]
	str	r3, [r5, #0]
	movs	r3, #160
	lsls	r3, r3, #13
	str	r3, [r5, #4]
	ldr	r3, [r0, #16]
	str	r3, [r5, #8]
	bl	sub_08004458
	ands	r0, r6
	subs	r0, #128
	lsls	r0, r0, #10
	str	r0, [r5, #12]
	bl	sub_08004458
	ands	r0, r6
	subs	r0, #128
	lsls	r0, r0, #10
	str	r0, [r5, #16]
	bl	sub_08004458
	movs	r1, #1
	ands	r0, r6
	subs	r0, #128
	add	sl, r1
	lsls	r0, r0, #10
	movs	r3, #0
	mov	r2, sl
	str	r0, [r5, #20]
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r2, #64
	bne.n	.L_080e566a
.L_080e56ae:
	ldr	r3, [sp, #84]
	cmp	r3, #11
	bgt.n	.L_080e56b8
	bl	.L_080e640e
.L_080e56b8:
	ldr	r0, [sp, #92]
	ldr	r1, [pc, #232]
	adds	r5, r0, r1
	ldr	r3, [r5, #0]
	ldr	r0, [r3, #8]
	bl	sub_080b5098
	ldr	r3, [r5, #0]
	ldr	r0, [r0, #0]
	mov	r8, r0
	ldr	r0, [r3, #8]
	bl	sub_080b5070
	lsrs	r3, r0, #31
	adds	r0, r0, r3
	movs	r3, #112
	asrs	r0, r0, #1
	movs	r2, #0
	add	r3, sp
	ldr	r7, [pc, #200]
	mov	fp, r0
	mov	sl, r2
	mov	r9, r3
.L_080e56e6:
	ldr	r3, [r7, #24]
	cmp	r3, #0
	blt.n	.L_080e577e
	mov	r6, r9
	mov	r0, sl
	movs	r5, #1
	ands	r5, r0
	adds	r1, r6, #0
	adds	r0, r7, #0
	bl	sub_080e3944
	ldr	r2, [r6, #0]
	adds	r5, #6
	lsls	r0, r5, #1
	asrs	r2, r2, #1
	ldr	r1, [pc, #148]
	str	r2, [r6, #0]
	subs	r3, r0, #2
	ldrh	r1, [r1, r3]
	ldr	r3, [sp, #76]
	adds	r1, r3, r1
	lsrs	r3, r5, #1
	subs	r2, r2, r3
	ldr	r3, [r6, #4]
	ldr	r4, [sp, #104]
	subs	r3, r3, r5
	str	r5, [sp, #0]
	str	r0, [sp, #4]
	ldr	r0, [sp, #88]
	bl	sub_080072f4
	adds	r0, r7, #0
	movs	r1, #62
	movs	r2, #0
	bl	sub_080e38b8
	mov	r3, sl
	ldr	r5, [sp, #84]
	adds	r3, #22
	cmp	r5, r3
	ble.n	.L_080e577e
	mov	r1, r8
	ldr	r0, [r1, #8]
	ldr	r3, [r7, #0]
	ldr	r1, [r1, #12]
	subs	r0, r0, r3
	ldr	r3, [r7, #4]
	mov	r2, r8
	add	r1, fp
	subs	r1, r1, r3
	ldr	r3, [r2, #16]
	ldr	r2, [r7, #8]
	subs	r3, r3, r2
	asrs	r4, r3, #8
	ldr	r3, [r7, #12]
	asrs	r0, r0, #8
	adds	r3, r3, r0
	str	r3, [r7, #12]
	ldr	r3, [r7, #16]
	asrs	r1, r1, #8
	adds	r3, r3, r1
	str	r3, [r7, #16]
	ldr	r3, [r7, #20]
	adds	r3, r3, r4
	str	r3, [r7, #20]
	ldr	r3, [pc, #64]
	ldr	r2, [pc, #68]
	adds	r0, r0, r3
	cmp	r0, r2
	bhi.n	.L_080e577e
	adds	r3, r4, r3
	cmp	r3, r2
	bhi.n	.L_080e577e
	movs	r3, #1
	negs	r3, r3
	str	r3, [r7, #24]
.L_080e577e:
	movs	r5, #1
	add	sl, r5
	mov	r0, sl
	adds	r7, #28
	cmp	r0, #32
	bne.n	.L_080e56e6
	bl	.L_080e640e
	movs	r0, r0
	lsls	r4, r6, #2
	movs	r0, r0
	lsls	r0, r4, #2
	movs	r0, r0
	lsls	r5, r7, #1
	movs	r0, r0
	udf	#72
	lsrs	r6, r1, #32
	bl	sub_080e67a2
	ldrb	r0, [r5, #0]
	movs	r0, r0
	ands	r0, r0
	lsls	r1, r0, #8
	lsrs	r7, r7, #31
	movs	r0, r0
	subs	r6, r7, #7
	movs	r0, r0
.L_080e57b4:
	ldr	r1, [sp, #96]
	cmp	r1, #21
	bne.n	.L_080e57be
	bl	.L_080e640e
.L_080e57be:
	cmp	r1, #6
	beq.n	.L_080e57c8
	cmp	r1, #27
	beq.n	.L_080e57c8
	b.n	.L_080e58f2
.L_080e57c8:
	ldr	r3, [sp, #84]
	subs	r3, #6
	cmp	r3, #13
	bhi.n	.L_080e5818
	movs	r2, #0
	ldr	r5, [sp, #84]
	mov	sl, r2
.L_080e57d6:
	lsrs	r3, r5, #31
	adds	r3, r5, r3
	movs	r2, #3
	asrs	r3, r3, #1
	ands	r3, r2
	lsls	r2, r3, #1
	adds	r2, r2, r3
	ldr	r0, [sp, #60]
	lsls	r1, r2, #4
	subs	r1, r1, r2
	ldr	r3, [pc, #736]
	ldr	r2, [r0, #0]
	lsls	r1, r1, #6
	adds	r1, r1, r3
	lsrs	r3, r2, #31
	adds	r2, r2, r3
	movs	r3, #24
	str	r3, [sp, #0]
	asrs	r2, r2, #1
	movs	r3, #104
	subs	r2, #8
	str	r3, [sp, #4]
	ldr	r4, [sp, #104]
	ldr	r0, [sp, #88]
	movs	r3, #0
	bl	sub_080072f4
	movs	r1, #1
	add	sl, r1
	mov	r2, sl
	adds	r5, #3
	cmp	r2, #2
	bne.n	.L_080e57d6
.L_080e5818:
	ldr	r3, [sp, #84]
	subs	r3, #8
	cmp	r3, #15
	bls.n	.L_080e5824
	bl	.L_080e640e
.L_080e5824:
	ldr	r5, [pc, #680]
	ldr	r0, [pc, #684]
	movs	r3, #0
	mov	sl, r3
	movs	r7, #3
	mov	fp, r5
	mov	r9, r0
.L_080e5832:
	mov	r8, sl
	mov	r1, r8
	ands	r1, r7
	mov	r8, r1
	bl	sub_08004458
	ldr	r3, [pc, #664]
	adds	r5, r0, #0
	ands	r5, r3
	adds	r0, r5, #0
	bl	sub_08002322
	ldr	r2, [sp, #60]
	ldr	r3, [r2, #0]
	adds	r6, r0, #0
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	lsls	r6, r6, #3
	mov	r1, r8
	asrs	r3, r3, #1
	mov	r0, fp
	asrs	r6, r6, #16
	adds	r6, r6, r3
	ldrb	r3, [r0, r1]
	adds	r0, r5, #0
	lsrs	r3, r3, #1
	subs	r6, r6, r3
	bl	sub_0800231c
	mov	r2, r9
	adds	r5, r0, #0
	mov	r0, r8
	ldrb	r3, [r2, r0]
	lsls	r5, r5, #5
	lsrs	r3, r3, #1
	movs	r0, #47
	asrs	r5, r5, #16
	subs	r5, r5, r3
	bl	sub_08002dd8
	movs	r0, #46
	bl	sub_08002dd8
	bl	sub_08004458
	ldr	r3, [pc, #588]
	ands	r0, r7
	ldrb	r2, [r3, r0]
	adds	r3, r7, #0
	orrs	r3, r2
	movs	r2, #2
	str	r2, [sp, #0]
	movs	r1, #7
	movs	r2, #7
	movs	r0, #47
	bl	sub_080ed408
	ldr	r2, [pc, #568]
	mov	r1, r8
	lsls	r3, r1, #1
	ldrh	r1, [r2, r3]
	ldr	r2, [pc, #564]
	mov	r0, fp
	adds	r1, r1, r2
	mov	r2, r8
	ldrb	r3, [r0, r2]
	str	r3, [sp, #0]
	mov	r0, r9
	ldrb	r3, [r0, r2]
	ldr	r2, [pc, #552]
	str	r3, [sp, #4]
	adds	r5, #56
	ldr	r4, [r2, #0]
	adds	r3, r5, #0
	adds	r2, r6, #0
	ldr	r0, [sp, #88]
	bl	sub_080072f4
	movs	r0, #47
	bl	sub_08002dd8
	ldr	r0, [pc, #532]
	ldr	r5, [sp, #92]
	adds	r3, r5, r0
	ldr	r3, [r3, #0]
	ldr	r1, [sp, #52]
	ldr	r0, [r3, #4]
	bl	sub_080cef64
	movs	r1, #1
	add	sl, r1
	mov	r2, sl
	cmp	r2, #3
	bne.n	.L_080e5832
	bl	.L_080e640e
.L_080e58f2:
	ldr	r3, [sp, #96]
	cmp	r3, #14
	beq.n	.L_080e58fa
	b.n	.L_080e5a2e
.L_080e58fa:
	movs	r0, #47
	bl	sub_08002dd8
	movs	r0, #46
	bl	sub_08002dd8
	ldr	r5, [sp, #84]
	cmp	r5, #23
	bls.n	.L_080e590e
	b.n	.L_080e5ab6
.L_080e590e:
	ldr	r0, [sp, #60]
	ldr	r3, [r0, #0]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [sp, #12]
	lsls	r3, r5, #5
	subs	r3, #232
	mov	fp, r3
	lsls	r3, r5, #4
	adds	r7, r3, #0
	mov	r1, fp
	subs	r7, #48
	cmp	r1, #0
	ble.n	.L_080e5930
	movs	r2, #0
	mov	fp, r2
.L_080e5930:
	cmp	r7, #104
	ble.n	.L_080e593a
.L_080e5934:
	subs	r7, #104
	cmp	r7, #104
	bgt.n	.L_080e5934
.L_080e593a:
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	movs	r0, #47
	bl	sub_080ed408
	ldr	r3, [pc, #420]
	ldr	r0, [sp, #12]
	mov	r8, r3
	movs	r5, #188
	mov	r1, fp
	add	r8, r5
	adds	r1, r1, r7
	movs	r6, #17
	movs	r5, #104
	str	r5, [sp, #4]
	str	r6, [sp, #0]
	mov	r9, r1
	subs	r0, #8
	mov	sl, r0
	mov	r2, r8
	mov	r3, r9
	ldr	r0, [sp, #88]
	ldr	r4, [r2, #0]
	subs	r3, #104
	ldr	r1, [pc, #368]
	mov	r2, sl
	bl	sub_080072f4
	subs	r5, r5, r7
	str	r5, [sp, #4]
	str	r6, [sp, #0]
	mov	r3, r8
	ldr	r4, [r3, #0]
	ldr	r0, [sp, #88]
	ldr	r1, [pc, #348]
	mov	r2, sl
	mov	r3, r9
	bl	sub_080072f4
	movs	r0, #34
	str	r0, [sp, #0]
	movs	r0, #65
	str	r0, [sp, #4]
	ldr	r2, [sp, #12]
	mov	r3, fp
	mov	r5, r8
	ldr	r1, [pc, #340]
	subs	r2, #17
	adds	r3, #47
	ldr	r4, [r5, #0]
	ldr	r0, [sp, #88]
	bl	sub_080072f4
	movs	r0, #47
	bl	sub_08002dd8
	ldr	r0, [sp, #84]
	cmp	r0, #8
	bne.n	.L_080e59be
	ldr	r1, [sp, #92]
	ldr	r2, [pc, #316]
	adds	r3, r1, r2
	str	r0, [r3, #0]
.L_080e59be:
	ldr	r3, [sp, #84]
	cmp	r3, #1
	ble.n	.L_080e5ab6
	ldr	r0, [sp, #92]
	movs	r1, #225
	movs	r5, #0
	lsls	r1, r1, #7
	mov	sl, r5
	movs	r7, #0
	movs	r6, #255
	adds	r5, r0, r1
.L_080e59d4:
	ldr	r3, [r5, #24]
	cmp	r3, #0
	bne.n	.L_080e5a20
	ldr	r2, [sp, #28]
	ldr	r3, [r2, #8]
	str	r3, [r5, #0]
	movs	r3, #160
	lsls	r3, r3, #13
	str	r3, [r5, #4]
	ldr	r3, [r2, #16]
	str	r3, [r5, #8]
	bl	sub_08004458
	ands	r0, r6
	subs	r0, #127
	lsls	r0, r0, #12
	str	r0, [r5, #12]
	bl	sub_08004458
	ands	r0, r6
	subs	r0, #64
	lsls	r0, r0, #10
	str	r0, [r5, #16]
	bl	sub_08004458
	ands	r0, r6
	subs	r0, #127
	lsls	r0, r0, #12
	str	r0, [r5, #20]
	mov	r0, sl
	lsrs	r3, r0, #31
	add	r3, sl
	asrs	r3, r3, #1
	adds	r3, #32
	adds	r7, #1
	str	r3, [r5, #24]
	cmp	r7, #4
	beq.n	.L_080e5ab6
.L_080e5a20:
	movs	r1, #1
	add	sl, r1
	mov	r2, sl
	adds	r5, #28
	cmp	r2, #64
	bne.n	.L_080e59d4
	b.n	.L_080e5ab6
.L_080e5a2e:
	ldr	r1, [sp, #96]
	cmp	r1, #31
	bne.n	.L_080e5afc
	movs	r0, #47
	bl	sub_08002dd8
	movs	r0, #46
	bl	sub_08002dd8
	ldr	r3, [sp, #84]
	subs	r3, #4
	cmp	r3, #19
	bhi.n	.L_080e5ab6
	ldr	r2, [sp, #60]
	ldr	r5, [r2, #0]
	lsrs	r3, r5, #31
	movs	r0, #2
	adds	r5, r5, r3
	movs	r3, #48
	str	r0, [sp, #0]
	mov	r9, r3
	mov	sl, r0
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	movs	r0, #47
	bl	sub_080ed408
	mov	r3, r9
	movs	r1, #24
	ldr	r6, [pc, #132]
	str	r1, [sp, #0]
	str	r3, [sp, #4]
	asrs	r5, r5, #1
	adds	r6, #188
	adds	r2, r5, #0
	ldr	r4, [r6, #0]
	ldr	r0, [sp, #88]
	subs	r2, #24
	mov	r8, r1
	ldr	r1, [pc, #100]
	bl	sub_080072f4
	movs	r0, #47
	bl	sub_08002dd8
	mov	r0, sl
	str	r0, [sp, #0]
	movs	r1, #7
	movs	r2, #7
	movs	r3, #7
	movs	r0, #47
	bl	sub_080ed408
	mov	r1, r8
	mov	r2, r9
	str	r1, [sp, #0]
	str	r2, [sp, #4]
	ldr	r0, [sp, #88]
	ldr	r4, [r6, #0]
	ldr	r1, [pc, #60]
	adds	r2, r5, #0
	movs	r3, #48
	bl	sub_080072f4
	movs	r0, #47
	bl	sub_08002dd8
.L_080e5ab6:
	ldr	r0, [pc, #52]
	ldr	r5, [sp, #92]
	adds	r3, r5, r0
	ldr	r3, [r3, #0]
	ldr	r1, [sp, #52]
	ldr	r0, [r3, #4]
	bl	sub_080cef64
	bl	.L_080e640e
	movs	r0, r0
	lsrs	r6, r2, #17
	lsls	r1, r0, #8
	udf	#202
	lsrs	r6, r1, #32
	udf	#208
	lsrs	r6, r1, #32
	.2byte 0xffff
	.2byte 0x0000
	.2byte 0xedd0
	.2byte 0x080e
	udf	#190
	lsrs	r6, r1, #32
	movs	r0, r0
	lsls	r1, r0, #8
	subs	r4, r1, #4
	lsls	r0, r0, #12
	ldrb	r0, [r5, #0]
	movs	r0, r0
	subs	r0, r2, #1
	lsls	r0, r0, #12
	lsls	r0, r5, #27
	lsls	r1, r0, #8
	strb	r0, [r5, #30]
	movs	r0, r0
.L_080e5afc:
	ldr	r1, [sp, #96]
	cmp	r1, #30
	bne.n	.L_080e5bac
	ldr	r2, [sp, #84]
	cmp	r2, #15
	ble.n	.L_080e5b16
	ldr	r5, [sp, #84]
	ldr	r2, [pc, #60]
	ldr	r1, [pc, #60]
	ldr	r3, [pc, #64]
	subs	r2, r2, r5
	orrs	r2, r1
	strh	r2, [r3, #0]
.L_080e5b16:
	ldr	r0, [sp, #84]
	cmp	r0, #5
	bgt.n	.L_080e5b20
	bl	.L_080e640e
.L_080e5b20:
	ldr	r1, [sp, #60]
	ldr	r2, [sp, #84]
	ldr	r6, [r1, #0]
	lsrs	r0, r0, #31
	adds	r0, r2, r0
	lsrs	r3, r6, #31
	movs	r1, #3
	asrs	r0, r0, #1
	adds	r6, r6, r3
	bl	sub_080022fc
	lsls	r5, r0, #2
	adds	r5, r5, r0
	lsls	r3, r5, #9
	asrs	r6, r6, #1
	ldr	r1, [pc, #20]
	subs	r6, #20
	mov	r9, r3
	b.n	.L_080e5b58
	movs	r0, r0
	.4byte 0x00000020
	.4byte 0x00001000
	.4byte 0x04000052
	.2byte 0x0c56
	.2byte 0x0201
.L_080e5b58:
	movs	r0, #40
	movs	r2, #32
	add	r1, r9
	mov	sl, r0
	str	r0, [sp, #0]
	mov	r8, r2
	str	r2, [sp, #4]
	ldr	r4, [sp, #104]
	ldr	r0, [sp, #88]
	adds	r2, r6, #0
	movs	r3, #16
	bl	sub_080072f4
	ldr	r3, [pc, #636]
	lsls	r5, r5, #8
	adds	r5, r5, r3
	mov	r0, sl
	mov	r1, r8
	str	r0, [sp, #0]
	str	r1, [sp, #4]
	ldr	r4, [sp, #104]
	adds	r1, r5, #0
	adds	r2, r6, #0
	movs	r3, #48
	ldr	r0, [sp, #88]
	bl	sub_080072f4
	ldr	r2, [pc, #612]
	mov	r3, sl
	add	r9, r2
	mov	r5, r8
	str	r3, [sp, #0]
	str	r5, [sp, #4]
	ldr	r4, [sp, #104]
	ldr	r0, [sp, #88]
	mov	r1, r9
	adds	r2, r6, #0
	movs	r3, #80
	bl	sub_080072f4
	bl	.L_080e640e
.L_080e5bac:
	ldr	r0, [sp, #96]
	cmp	r0, #5
	beq.n	.L_080e5bb6
	cmp	r0, #23
	bne.n	.L_080e5c32
.L_080e5bb6:
	movs	r2, #112
	movs	r1, #0
	add	r2, sp
	ldr	r7, [pc, #568]
	mov	sl, r1
	mov	r8, r2
.L_080e5bc2:
	mov	r5, sl
	lsrs	r3, r5, #31
	add	r3, sl
	asrs	r3, r3, #1
	ldr	r0, [sp, #84]
	adds	r3, #4
	cmp	r0, r3
	blt.n	.L_080e5c22
	ldr	r3, [r7, #24]
	cmp	r3, #11
	bgt.n	.L_080e5c22
	mov	r6, r8
	lsrs	r5, r3, #31
	adds	r1, r6, #0
	adds	r0, r7, #0
	adds	r5, r3, r5
	bl	sub_080e3944
	ldr	r2, [r6, #0]
	ldr	r1, [pc, #528]
	lsrs	r3, r2, #31
	asrs	r5, r5, #1
	adds	r2, r2, r3
	lsls	r5, r5, #11
	ldr	r3, [r6, #4]
	asrs	r2, r2, #1
	adds	r5, r5, r1
	movs	r1, #32
	str	r2, [r6, #0]
	str	r1, [sp, #0]
	movs	r1, #64
	subs	r3, #32
	str	r1, [sp, #4]
	subs	r2, #16
	ldr	r4, [sp, #104]
	ldr	r0, [sp, #88]
	adds	r1, r5, #0
	bl	sub_080072f4
	movs	r2, #128
	adds	r0, r7, #0
	movs	r1, #60
	lsls	r2, r2, #5
	bl	sub_080e38b8
	ldr	r3, [r7, #24]
	adds	r3, #1
	str	r3, [r7, #24]
.L_080e5c22:
	movs	r2, #1
	add	sl, r2
	mov	r3, sl
	adds	r7, #28
	cmp	r3, #16
	bne.n	.L_080e5bc2
	bl	.L_080e640e
.L_080e5c32:
	ldr	r5, [sp, #96]
	cmp	r5, #4
	bne.n	.L_080e5c3a
	b.n	.L_080e640e
.L_080e5c3a:
	cmp	r5, #11
	beq.n	.L_080e5c40
	b.n	.L_080e5e28
.L_080e5c40:
	ldr	r0, [sp, #84]
	lsls	r5, r0, #9
	adds	r0, r5, #0
	bl	sub_08002322
	adds	r0, r5, #0
	bl	sub_0800231c
	ldr	r2, [sp, #60]
	lsls	r0, r0, #2
	movs	r1, #6
	ldrsh	r3, [r2, r1]
	asrs	r0, r0, #16
	adds	r3, r3, r0
	adds	r5, r3, #0
	ldr	r3, [sp, #84]
	adds	r5, #16
	cmp	r3, #3
	bgt.n	.L_080e5c92
	ldr	r0, [sp, #92]
	ldr	r2, [pc, #404]
	adds	r3, r0, r2
	ldr	r3, [r3, #0]
	ldr	r2, [r3, #4]
	ldr	r1, [pc, #400]
	lsls	r3, r2, #3
	subs	r3, r3, r2
	ldrb	r2, [r1, r3]
	ldr	r3, [pc, #396]
	movs	r1, #57
	ldrb	r3, [r3, #0]
	str	r1, [sp, #0]
	movs	r1, #98
	str	r1, [sp, #4]
	adds	r3, r5, r3
	ldr	r4, [sp, #104]
	ldr	r0, [sp, #88]
	ldr	r1, [sp, #92]
	bl	sub_080072f4
	b.n	.L_080e640e
.L_080e5c92:
	ldr	r3, [sp, #84]
	cmp	r3, #7
	bgt.n	.L_080e5cc2
	ldr	r0, [sp, #92]
	ldr	r2, [pc, #356]
	adds	r3, r0, r2
	ldr	r3, [r3, #0]
	ldr	r2, [r3, #4]
	ldr	r1, [pc, #352]
	lsls	r3, r2, #3
	subs	r3, r3, r2
	ldrb	r2, [r1, r3]
	ldr	r3, [pc, #348]
	movs	r1, #57
	ldrb	r3, [r3, #0]
	str	r1, [sp, #0]
	movs	r1, #98
	str	r1, [sp, #4]
	adds	r3, r5, r3
	ldr	r4, [sp, #104]
	ldr	r0, [sp, #88]
	ldr	r1, [sp, #92]
	bl	sub_080072f4
.L_080e5cc2:
	ldr	r3, [sp, #92]
	ldr	r0, [pc, #324]
	adds	r1, r3, r0
	ldr	r0, [pc, #308]
	adds	r6, r3, r0
	ldr	r3, [r6, #0]
	ldr	r2, [pc, #308]
	mov	r8, r2
	ldr	r2, [r3, #4]
	lsls	r3, r2, #3
	subs	r3, r3, r2
	ldr	r7, [pc, #300]
	adds	r3, #1
	mov	r0, r8
	ldrb	r2, [r0, r3]
	ldrb	r3, [r7, #1]
	movs	r0, #99
	str	r0, [sp, #0]
	movs	r0, #69
	adds	r3, r5, r3
	str	r0, [sp, #4]
	ldr	r4, [sp, #104]
	ldr	r0, [sp, #88]
	bl	sub_080072f4
	ldr	r3, [sp, #84]
	subs	r3, #4
	cmp	r3, #1
	bhi.n	.L_080e5d0a
	movs	r1, #128
	ldr	r3, [pc, #272]
	ldr	r0, [sp, #88]
	lsls	r1, r1, #7
	ldr	r2, [pc, #268]
	bl	sub_080072f0
.L_080e5d0a:
	ldr	r3, [sp, #84]
	subs	r3, #6
	cmp	r3, #1
	bhi.n	.L_080e5d3a
	ldr	r2, [sp, #92]
	ldr	r3, [pc, #256]
	adds	r1, r2, r3
	ldr	r3, [r6, #0]
	ldr	r2, [r3, #4]
	lsls	r3, r2, #3
	subs	r3, r3, r2
	adds	r3, #2
	mov	r0, r8
	ldrb	r2, [r0, r3]
	ldrb	r3, [r7, #2]
	movs	r0, #128
	str	r0, [sp, #0]
	movs	r0, #91
	str	r0, [sp, #4]
	adds	r3, r5, r3
	ldr	r4, [sp, #104]
	ldr	r0, [sp, #88]
	bl	sub_080072f4
.L_080e5d3a:
	ldr	r3, [sp, #84]
	subs	r3, #8
	cmp	r3, #1
	bhi.n	.L_080e5d66
	ldr	r3, [r6, #0]
	ldr	r2, [r3, #4]
	lsls	r3, r2, #3
	subs	r3, r3, r2
	adds	r3, #3
	mov	r1, r8
	ldrb	r2, [r1, r3]
	ldrb	r3, [r7, #3]
	movs	r1, #128
	str	r1, [sp, #0]
	movs	r1, #91
	str	r1, [sp, #4]
	adds	r3, r5, r3
	ldr	r4, [sp, #104]
	ldr	r0, [sp, #88]
	ldr	r1, [pc, #152]
	bl	sub_080072f4
.L_080e5d66:
	ldr	r3, [sp, #84]
	subs	r3, #10
	cmp	r3, #1
	bhi.n	.L_080e5d92
	ldr	r3, [r6, #0]
	ldr	r2, [r3, #4]
	lsls	r3, r2, #3
	subs	r3, r3, r2
	adds	r3, #4
	mov	r0, r8
	ldrb	r2, [r0, r3]
	ldrb	r3, [r7, #4]
	movs	r0, #128
	str	r0, [sp, #0]
	movs	r0, #59
	str	r0, [sp, #4]
	ldr	r1, [pc, #148]
	adds	r3, r5, r3
	ldr	r4, [sp, #104]
	ldr	r0, [sp, #88]
	bl	sub_080072f4
.L_080e5d92:
	ldr	r3, [sp, #84]
	subs	r3, #12
	cmp	r3, #1
	bhi.n	.L_080e5dbe
	ldr	r3, [r6, #0]
	ldr	r2, [r3, #4]
	lsls	r3, r2, #3
	subs	r3, r3, r2
	adds	r3, #5
	mov	r0, r8
	ldrb	r2, [r0, r3]
	ldrb	r3, [r7, #5]
	movs	r0, #122
	str	r0, [sp, #0]
	movs	r0, #29
	str	r0, [sp, #4]
	ldr	r1, [pc, #108]
	adds	r3, r5, r3
	ldr	r4, [sp, #104]
	ldr	r0, [sp, #88]
	bl	sub_080072f4
.L_080e5dbe:
	ldr	r3, [sp, #84]
	subs	r3, #14
	cmp	r3, #1
	bls.n	.L_080e5dc8
	b.n	.L_080e640e
.L_080e5dc8:
	ldr	r3, [r6, #0]
	ldr	r2, [r3, #4]
	lsls	r3, r2, #3
	subs	r3, r3, r2
	adds	r3, #6
	mov	r0, r8
	ldrb	r2, [r0, r3]
	ldrb	r3, [r7, #6]
	movs	r0, #76
	str	r0, [sp, #0]
	movs	r0, #25
	str	r0, [sp, #4]
	ldr	r1, [pc, #64]
	adds	r3, r5, r3
	ldr	r4, [sp, #104]
	ldr	r0, [sp, #88]
	bl	sub_080072f4
	b.n	.L_080e640e
	movs	r0, r0
	.4byte 0x02012a56
	.4byte 0x02011156
	.4byte 0x02014000
	.4byte 0x02010000
	.4byte 0x00007828
	.4byte 0x080eedd4
	.4byte 0x080eede2
	.4byte 0x000015d2
	.4byte 0x03000168
	.4byte 0x3f3f3f3f
	.4byte 0x00003081
	.4byte 0x02012d80
	.4byte 0x02014b00
	.2byte 0x58d2
	.2byte 0x0201
.L_080e5e28:
	ldr	r1, [sp, #96]
	cmp	r1, #32
	beq.n	.L_080e5e30
	b.n	.L_080e600a
.L_080e5e30:
	ldr	r2, [sp, #72]
	ldr	r3, [sp, #68]
	ldr	r5, [sp, #84]
	adds	r2, r2, r3
	str	r2, [sp, #72]
	cmp	r5, #6
	ble.n	.L_080e5e50
	ldr	r0, [sp, #68]
	lsls	r3, r3, #1
	adds	r3, r3, r0
	lsls	r3, r3, #4
	cmp	r3, #0
	bge.n	.L_080e5e4c
	adds	r3, #63
.L_080e5e4c:
	asrs	r3, r3, #6
	str	r3, [sp, #68]
.L_080e5e50:
	ldr	r1, [sp, #72]
	ldr	r2, [pc, #76]
	asrs	r3, r1, #16
	lsls	r3, r3, #8
	str	r3, [r2, #0]
	ldr	r1, [sp, #84]
	subs	r1, #16
	cmp	r1, #15
	bhi.n	.L_080e5e6e
	ldr	r2, [pc, #52]
	subs	r2, r2, r1
	ldr	r1, [pc, #52]
	ldr	r3, [pc, #56]
	orrs	r2, r1
	strh	r2, [r3, #0]
.L_080e5e6e:
	ldr	r3, [sp, #84]
	subs	r3, #4
	cmp	r3, #1
	bhi.n	.L_080e5e84
	movs	r1, #128
	ldr	r3, [pc, #44]
	ldr	r0, [sp, #88]
	lsls	r1, r1, #7
	ldr	r2, [pc, #44]
	bl	sub_080072f0
.L_080e5e84:
	ldr	r2, [sp, #84]
	cmp	r2, #3
	bgt.n	.L_080e5ee2
	ldr	r5, [sp, #92]
	ldr	r0, [pc, #32]
	adds	r3, r5, r0
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #4]
	b.n	.L_080e5eb4
	movs	r0, r0
	.4byte 0x00000010
	.4byte 0x00001000
	.4byte 0x04000028
	.4byte 0x04000052
	.4byte 0x03000168
	.4byte 0x3f3f3f3f
	.2byte 0x7828
	.2byte 0x0000
.L_080e5eb4:
	cmp	r3, #1
	bne.n	.L_080e5eca
	movs	r3, #80
	str	r3, [sp, #0]
	movs	r3, #104
	str	r3, [sp, #4]
	ldr	r4, [sp, #104]
	ldr	r0, [sp, #88]
	adds	r1, r5, #0
	movs	r2, #0
	b.n	.L_080e5eda
.L_080e5eca:
	movs	r3, #80
	str	r3, [sp, #0]
	movs	r3, #104
	str	r3, [sp, #4]
	ldr	r4, [sp, #104]
	ldr	r0, [sp, #88]
	ldr	r1, [sp, #92]
	movs	r2, #48
.L_080e5eda:
	movs	r3, #24
	bl	sub_080072f4
	b.n	.L_080e640e
.L_080e5ee2:
	ldr	r1, [sp, #84]
	cmp	r1, #7
	bgt.n	.L_080e5f24
	ldr	r2, [sp, #92]
	ldr	r5, [pc, #872]
	adds	r3, r2, r5
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #4]
	cmp	r3, #1
	bne.n	.L_080e5f0e
	movs	r3, #80
	str	r3, [sp, #0]
	movs	r3, #104
	str	r3, [sp, #4]
	adds	r1, r2, #0
	ldr	r4, [sp, #104]
	ldr	r0, [sp, #88]
	movs	r2, #0
	movs	r3, #24
	bl	sub_080072f4
	b.n	.L_080e5f24
.L_080e5f0e:
	movs	r3, #80
	str	r3, [sp, #0]
	movs	r3, #104
	str	r3, [sp, #4]
	ldr	r4, [sp, #104]
	ldr	r0, [sp, #88]
	ldr	r1, [sp, #92]
	movs	r2, #48
	movs	r3, #24
	bl	sub_080072f4
.L_080e5f24:
	ldr	r0, [sp, #92]
	ldr	r1, [pc, #812]
	adds	r3, r0, r1
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #4]
	cmp	r3, #1
	bne.n	.L_080e5f4e
	movs	r3, #80
	movs	r2, #240
	lsls	r2, r2, #5
	str	r3, [sp, #0]
	movs	r3, #104
	adds	r1, r0, r2
	str	r3, [sp, #4]
	ldr	r4, [sp, #104]
	ldr	r0, [sp, #88]
	movs	r2, #16
	movs	r3, #16
	bl	sub_080072f4
	b.n	.L_080e5f6a
.L_080e5f4e:
	ldr	r3, [sp, #92]
	movs	r5, #240
	lsls	r5, r5, #5
	adds	r1, r3, r5
	movs	r3, #80
	str	r3, [sp, #0]
	movs	r3, #104
	str	r3, [sp, #4]
	ldr	r4, [sp, #104]
	ldr	r0, [sp, #88]
	movs	r2, #32
	movs	r3, #16
	bl	sub_080072f4
.L_080e5f6a:
	ldr	r3, [sp, #84]
	subs	r3, #6
	cmp	r3, #1
	bhi.n	.L_080e5f8e
	ldr	r0, [sp, #92]
	movs	r3, #128
	movs	r2, #250
	lsls	r2, r2, #6
	str	r3, [sp, #0]
	movs	r3, #91
	adds	r1, r0, r2
	str	r3, [sp, #4]
	ldr	r4, [sp, #104]
	ldr	r0, [sp, #88]
	movs	r2, #0
	movs	r3, #16
	bl	sub_080072f4
.L_080e5f8e:
	ldr	r3, [sp, #84]
	subs	r3, #8
	cmp	r3, #1
	bhi.n	.L_080e5fac
	movs	r3, #128
	str	r3, [sp, #0]
	movs	r3, #91
	str	r3, [sp, #4]
	ldr	r4, [sp, #104]
	ldr	r0, [sp, #88]
	ldr	r1, [pc, #692]
	movs	r2, #0
	movs	r3, #16
	bl	sub_080072f4
.L_080e5fac:
	ldr	r3, [sp, #84]
	subs	r3, #10
	cmp	r3, #1
	bhi.n	.L_080e5fca
	movs	r3, #128
	str	r3, [sp, #0]
	movs	r3, #59
	str	r3, [sp, #4]
	ldr	r1, [pc, #668]
	ldr	r4, [sp, #104]
	ldr	r0, [sp, #88]
	movs	r2, #0
	movs	r3, #16
	bl	sub_080072f4
.L_080e5fca:
	ldr	r3, [sp, #84]
	subs	r3, #12
	cmp	r3, #1
	bhi.n	.L_080e5fe8
	movs	r3, #128
	str	r3, [sp, #0]
	movs	r3, #29
	str	r3, [sp, #4]
	ldr	r1, [pc, #644]
	ldr	r4, [sp, #104]
	ldr	r0, [sp, #88]
	movs	r2, #0
	movs	r3, #16
	bl	sub_080072f4
.L_080e5fe8:
	ldr	r3, [sp, #84]
	subs	r3, #14
	cmp	r3, #1
	bls.n	.L_080e5ff2
	b.n	.L_080e640e
.L_080e5ff2:
	movs	r3, #128
	str	r3, [sp, #0]
	movs	r3, #26
	str	r3, [sp, #4]
	ldr	r1, [pc, #616]
	ldr	r4, [sp, #104]
	ldr	r0, [sp, #88]
	movs	r2, #0
	movs	r3, #16
	bl	sub_080072f4
	b.n	.L_080e640e
.L_080e600a:
	ldr	r3, [sp, #96]
	cmp	r3, #20
	bne.n	.L_080e60ba
	movs	r5, #0
	mov	sl, r5
.L_080e6014:
	mov	r3, sl
	ldr	r0, [sp, #84]
	mov	r5, sl
	adds	r3, #6
	adds	r5, #1
	cmp	r0, r3
	blt.n	.L_080e60b2
	adds	r3, #12
	cmp	r0, r3
	bge.n	.L_080e60ae
	mov	r1, sl
	subs	r3, r0, r1
	subs	r3, #6
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r4, r3, #1
	ldr	r3, [sp, #60]
	ldr	r2, [r3, #0]
	ldr	r7, [pc, #556]
	lsrs	r3, r2, #31
	adds	r2, r2, r3
	ldrb	r3, [r7, r4]
	asrs	r2, r2, #1
	lsrs	r3, r3, #1
	subs	r6, r2, r3
	movs	r3, #1
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_080e605c
	lsrs	r3, r5, #31
	adds	r3, r5, r3
	asrs	r3, r3, #1
	lsls	r2, r3, #1
	adds	r2, r2, r3
	adds	r6, r6, r2
	b.n	.L_080e606c
.L_080e605c:
	mov	r5, sl
	adds	r5, #1
	lsrs	r3, r5, #31
	adds	r3, r5, r3
	asrs	r3, r3, #1
	lsls	r2, r3, #1
	adds	r2, r2, r3
	subs	r6, r6, r2
.L_080e606c:
	mov	r1, sl
	movs	r0, #1
	cmp	r1, #0
	beq.n	.L_080e6084
	mov	r3, sl
	subs	r3, #1
	movs	r2, #3
	ands	r3, r2
	movs	r0, #0
	cmp	r3, #1
	ble.n	.L_080e6084
	movs	r0, #1
.L_080e6084:
	ldr	r2, [pc, #484]
	lsls	r3, r4, #1
	ldrh	r1, [r2, r3]
	ldr	r2, [pc, #460]
	ldr	r3, [pc, #480]
	adds	r1, r1, r2
	ldrb	r2, [r7, r4]
	ldrb	r3, [r3, r4]
	str	r2, [sp, #0]
	ldr	r2, [pc, #476]
	ldrb	r2, [r2, r4]
	str	r2, [sp, #4]
	ldr	r2, [sp, #52]
	lsls	r0, r0, #2
	ldr	r4, [r0, r2]
	adds	r3, #48
	ldr	r0, [sp, #88]
	adds	r2, r6, #0
	bl	sub_080072f4
	b.n	.L_080e60b2
.L_080e60ae:
	mov	r5, sl
	adds	r5, #1
.L_080e60b2:
	mov	sl, r5
	cmp	r5, #12
	bne.n	.L_080e6014
	b.n	.L_080e640e
.L_080e60ba:
	ldr	r3, [sp, #96]
	cmp	r3, #16
	beq.n	.L_080e60c2
	b.n	.L_080e61d4
.L_080e60c2:
	ldr	r5, [sp, #84]
	cmp	r5, #0
	bne.n	.L_080e610c
	movs	r0, #0
	ldr	r6, [pc, #428]
	ldr	r5, [pc, #428]
	mov	sl, r0
	movs	r7, #0
.L_080e60d2:
	bl	sub_08004458
	movs	r3, #127
	ands	r3, r0
	adds	r3, #32
	str	r3, [r5, #0]
	str	r7, [r5, #4]
	str	r7, [r5, #8]
	bl	sub_08004458
	ands	r0, r6
	str	r0, [r5, #12]
	bl	sub_08004458
	ands	r0, r6
	str	r0, [r5, #16]
	bl	sub_08004458
	movs	r1, #1
	add	sl, r1
	ands	r0, r6
	mov	r2, sl
	str	r0, [r5, #20]
	adds	r5, #28
	cmp	r2, #64
	bne.n	.L_080e60d2
	ldr	r2, [pc, #376]
	movs	r3, #159
	str	r3, [r2, #4]
.L_080e610c:
	movs	r5, #112
	ldr	r0, [sp, #60]
	movs	r3, #0
	add	r5, sp
	ldr	r7, [pc, #356]
	mov	sl, r3
	mov	r9, r5
	mov	r8, r0
.L_080e611c:
	ldr	r3, [r7, #0]
	cmp	r3, #0
	blt.n	.L_080e61c6
	mov	r1, sl
	lsrs	r3, r1, #31
	add	r3, sl
	ldr	r2, [sp, #84]
	asrs	r3, r3, #1
	cmp	r2, r3
	blt.n	.L_080e61c6
	movs	r5, #3
	ands	r5, r1
	bl	sub_080049ac
	ldr	r0, [r7, #12]
	bl	sub_08004bd4
	mov	r6, r9
	ldr	r0, [r7, #16]
	bl	sub_08004c1c
	adds	r1, r6, #0
	adds	r0, r7, #0
	bl	sub_080e3944
	ldr	r2, [r6, #0]
	mov	r0, r8
	lsrs	r3, r2, #31
	adds	r2, r2, r3
	ldr	r3, [r0, #0]
	lsrs	r1, r3, #31
	adds	r3, r3, r1
	asrs	r3, r3, #1
	asrs	r2, r2, #1
	adds	r2, r2, r3
	str	r2, [r6, #0]
	ldr	r3, [r6, #4]
	ldr	r1, [r0, #4]
	adds	r3, r3, r1
	adds	r1, r3, #0
	adds	r1, #32
	str	r1, [r6, #4]
	ldr	r1, [pc, #272]
	movs	r0, #8
	lsls	r5, r5, #1
	ldrh	r1, [r1, r5]
	str	r0, [sp, #0]
	str	r0, [sp, #4]
	ldr	r5, [pc, #216]
	ldr	r0, [sp, #52]
	adds	r3, #28
	ldr	r4, [r0, #4]
	adds	r1, r1, r5
	subs	r2, #4
	ldr	r0, [sp, #88]
	bl	sub_080072f4
	ldr	r3, [r7, #0]
	subs	r3, #6
	str	r3, [r7, #0]
	cmp	r3, #0
	bge.n	.L_080e61c6
	movs	r3, #7
	mov	r1, sl
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_080e61a6
	cmp	r1, #63
	bne.n	.L_080e61c6
.L_080e61a6:
	movs	r0, #133
	bl	sub_080f9010
	ldr	r5, [pc, #164]
	ldr	r2, [sp, #92]
	adds	r3, r2, r5
	ldr	r3, [r3, #0]
	movs	r1, #36
	ldrsh	r0, [r3, r1]
	movs	r3, #4
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #5
	movs	r3, #0
	bl	sub_080d6888
.L_080e61c6:
	movs	r2, #1
	add	sl, r2
	mov	r3, sl
	adds	r7, #28
	cmp	r3, #64
	bne.n	.L_080e611c
	b.n	.L_080e640e
.L_080e61d4:
	ldr	r5, [sp, #96]
	cmp	r5, #8
	bne.n	.L_080e6218
	ldr	r3, [sp, #84]
	subs	r3, #5
	cmp	r3, #44
	bls.n	.L_080e61e4
	b.n	.L_080e640e
.L_080e61e4:
	ldr	r0, [sp, #84]
	cmp	r0, #25
	ble.n	.L_080e61f2
	lsls	r2, r0, #2
	movs	r3, #196
	subs	r1, r3, r2
	b.n	.L_080e61fa
.L_080e61f2:
	ldr	r1, [sp, #84]
	lsls	r3, r1, #4
	adds	r1, r3, #0
	subs	r1, #64
.L_080e61fa:
	cmp	r1, #96
	ble.n	.L_080e6200
	movs	r1, #96
.L_080e6200:
	movs	r2, #32
	movs	r3, #104
	subs	r3, r3, r1
	str	r2, [sp, #0]
	str	r1, [sp, #4]
	ldr	r4, [sp, #104]
	ldr	r0, [sp, #88]
	ldr	r1, [pc, #72]
	movs	r2, #48
	bl	sub_080072f4
	b.n	.L_080e640e
.L_080e6218:
	ldr	r3, [sp, #96]
	subs	r3, #33
	cmp	r3, #1
	bhi.n	.L_080e62cc
	ldr	r2, [sp, #84]
	cmp	r2, #5
	ble.n	.L_080e6228
	b.n	.L_080e640e
.L_080e6228:
	ldr	r5, [sp, #92]
	ldr	r0, [pc, #40]
	adds	r3, r5, r0
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #4]
	cmp	r3, #0
	bne.n	.L_080e6288
	ldr	r2, [sp, #60]
	ldr	r1, [r2, #0]
	ldr	r5, [sp, #84]
	lsrs	r3, r1, #31
	adds	r1, r1, r3
	movs	r3, #6
	subs	r3, r3, r5
	lsls	r2, r3, #1
	adds	r2, r2, r3
	ldr	r0, [sp, #60]
	lsls	r3, r2, #1
	asrs	r1, r1, #1
	adds	r1, r1, r3
	b.n	.L_080e62a2
	movs	r0, r0
	.4byte 0x00007828
	.4byte 0x02010000
	.4byte 0x02012d80
	.4byte 0x02014b00
	.4byte 0x02015980
	.4byte 0x080ede9f
	.4byte 0x080edeb2
	.4byte 0x080edeab
	.4byte 0x080edea5
	.4byte 0x0000ffff
	.4byte 0x02014000
	.4byte 0x020146e4
	.2byte 0xedea
	.2byte 0x080e
.L_080e6288:
	ldr	r2, [sp, #60]
	ldr	r1, [r2, #0]
	ldr	r5, [sp, #84]
	lsrs	r3, r1, #31
	adds	r1, r1, r3
	movs	r3, #6
	subs	r3, r3, r5
	lsls	r2, r3, #1
	adds	r2, r2, r3
	ldr	r0, [sp, #60]
	lsls	r3, r2, #1
	asrs	r1, r1, #1
	subs	r1, r1, r3
.L_080e62a2:
	ldr	r3, [r0, #4]
	lsls	r2, r2, #2
	subs	r3, r3, r2
	adds	r3, #24
	adds	r2, r1, #0
	movs	r1, #32
	str	r1, [sp, #0]
	movs	r1, #64
	str	r1, [sp, #4]
	ldr	r1, [sp, #52]
	subs	r2, #16
	ldr	r4, [r1, #4]
	subs	r3, #32
	ldr	r0, [sp, #88]
	ldr	r1, [pc, #8]
	bl	sub_080072f4
	b.n	.L_080e640e
	movs	r0, r0
	.2byte 0x0000
	.2byte 0x0201
.L_080e62cc:
	ldr	r2, [sp, #96]
	cmp	r2, #12
	bne.n	.L_080e6374
	ldr	r3, [sp, #84]
	cmp	r3, #47
	ble.n	.L_080e62e6
	ldr	r5, [sp, #84]
	ldr	r2, [pc, #28]
	ldr	r1, [pc, #28]
	ldr	r3, [pc, #32]
	subs	r2, r2, r5
	orrs	r2, r1
	strh	r2, [r3, #0]
.L_080e62e6:
	movs	r0, #0
	movs	r1, #3
	movs	r2, #1
	ldr	r7, [pc, #20]
	mov	sl, r0
	mov	r9, r1
	add	r6, sp, #112
	mov	r8, r2
	b.n	.L_080e6308
	.4byte 0x00000040
	.4byte 0x00001000
	.4byte 0x04000052
	.2byte 0x4000
	.2byte 0x0201
.L_080e6308:
	mov	r0, sl
	movs	r1, #3
	bl	sub_080022fc
	adds	r1, r6, #0
	adds	r5, r0, #0
	adds	r0, r7, #0
	bl	sub_080e3944
	ldr	r2, [r6, #0]
	lsrs	r3, r2, #31
	adds	r2, r2, r3
	asrs	r2, r2, #1
	str	r2, [r6, #0]
	movs	r0, #24
	mov	r4, sl
	mov	r3, r8
	lsls	r1, r5, #3
	ands	r4, r3
	adds	r1, r1, r5
	ldr	r3, [r6, #4]
	ldr	r5, [pc, #728]
	str	r0, [sp, #0]
	str	r0, [sp, #4]
	ldr	r0, [sp, #52]
	lsls	r4, r4, #2
	lsls	r1, r1, #6
	ldr	r4, [r4, r0]
	adds	r1, r1, r5
	subs	r2, #12
	subs	r3, #12
	ldr	r0, [sp, #88]
	bl	sub_080072f4
	mov	r3, sl
	mov	r1, r9
	ands	r3, r1
	adds	r3, #11
	mov	r2, r8
	lsls	r2, r3
	adds	r0, r7, #0
	movs	r1, #60
	bl	sub_080e38b8
	ldr	r3, [r7, #24]
	movs	r2, #1
	adds	r3, #1
	add	sl, r2
	str	r3, [r7, #24]
	mov	r3, sl
	adds	r7, #28
	cmp	r3, #16
	bne.n	.L_080e6308
	b.n	.L_080e640e
.L_080e6374:
	ldr	r5, [sp, #96]
	cmp	r5, #100
	beq.n	.L_080e640e
	movs	r1, #112
	movs	r0, #0
	add	r1, sp
	ldr	r7, [pc, #652]
	mov	sl, r0
	mov	r8, r1
.L_080e6386:
	mov	r3, sl
	ldr	r2, [sp, #84]
	adds	r3, #4
	cmp	r2, r3
	blt.n	.L_080e6402
	ldr	r5, [r7, #24]
	cmp	r5, #23
	bgt.n	.L_080e6402
	cmp	r5, #0
	bge.n	.L_080e639c
	adds	r5, #3
.L_080e639c:
	mov	r6, r8
	adds	r1, r6, #0
	adds	r0, r7, #0
	bl	sub_080e3944
	ldr	r2, [r6, #0]
	lsrs	r3, r2, #31
	adds	r2, r2, r3
	asrs	r5, r5, #2
	asrs	r2, r2, #1
	mov	r3, sl
	movs	r0, #24
	movs	r4, #1
	str	r2, [r6, #0]
	ands	r4, r3
	lsls	r1, r5, #3
	ldr	r3, [r6, #4]
	str	r0, [sp, #0]
	movs	r0, #48
	str	r0, [sp, #4]
	adds	r1, r1, r5
	ldr	r0, [sp, #52]
	ldr	r5, [pc, #576]
	lsls	r4, r4, #2
	lsls	r1, r1, #7
	adds	r1, r1, r5
	ldr	r4, [r4, r0]
	subs	r2, #12
	subs	r3, #24
	ldr	r0, [sp, #88]
	bl	sub_080072f4
	ldr	r1, [sp, #96]
	cmp	r1, #25
	bne.n	.L_080e63f0
	movs	r2, #128
	adds	r0, r7, #0
	movs	r1, #60
	lsls	r2, r2, #3
	bl	sub_080e38b8
	b.n	.L_080e63fc
.L_080e63f0:
	movs	r2, #128
	adds	r0, r7, #0
	movs	r1, #60
	lsls	r2, r2, #5
	bl	sub_080e38b8
.L_080e63fc:
	ldr	r3, [r7, #24]
	adds	r3, #1
	str	r3, [r7, #24]
.L_080e6402:
	movs	r2, #1
	add	sl, r2
	mov	r3, sl
	adds	r7, #28
	cmp	r3, #16
	bne.n	.L_080e6386
.L_080e640e:
	ldr	r5, [sp, #96]
	cmp	r5, #7
	bgt.n	.L_080e6464
	ldr	r0, [sp, #84]
	cmp	r0, #5
	bgt.n	.L_080e6464
	add	r5, sp, #112
	adds	r1, r5, #0
	ldr	r0, [sp, #8]
	bl	sub_080e3944
	ldr	r2, [r5, #0]
	lsrs	r3, r2, #31
	adds	r2, r2, r3
	asrs	r2, r2, #1
	movs	r0, #20
	str	r2, [r5, #0]
	ldr	r3, [r5, #4]
	str	r0, [sp, #0]
	movs	r0, #40
	str	r0, [sp, #4]
	ldr	r5, [sp, #52]
	ldr	r1, [pc, #472]
	subs	r2, #10
	subs	r3, #4
	ldr	r4, [r5, #4]
	ldr	r0, [sp, #88]
	bl	sub_080072f4
	ldr	r0, [sp, #8]
	ldr	r1, [sp, #24]
	ldr	r3, [r0, #0]
	ldr	r2, [r1, #0]
	adds	r3, r3, r2
	str	r3, [r0, #0]
	ldr	r3, [r0, #4]
	ldr	r2, [r1, #4]
	adds	r3, r3, r2
	str	r3, [r0, #4]
	ldr	r3, [r0, #8]
	ldr	r2, [r1, #8]
	adds	r3, r3, r2
	str	r3, [r0, #8]
.L_080e6464:
	ldr	r2, [sp, #84]
	cmp	r2, #3
	bne.n	.L_080e6472
	movs	r0, #1
	negs	r0, r0
	bl	sub_080b50e8
.L_080e6472:
	ldr	r3, [sp, #84]
	cmp	r3, #4
	bne.n	.L_080e647e
	movs	r0, #134
	bl	sub_080f9010
.L_080e647e:
	ldr	r5, [sp, #84]
	cmp	r5, #6
	bne.n	.L_080e6530
	ldr	r0, [sp, #16]
	cmp	r0, #1
	bls.n	.L_080e64a8
	ldr	r1, [sp, #96]
	cmp	r1, #7
	beq.n	.L_080e64a8
	cmp	r1, #13
	beq.n	.L_080e64a8
	cmp	r1, #18
	beq.n	.L_080e64a8
	cmp	r1, #19
	beq.n	.L_080e64a8
	cmp	r1, #23
	beq.n	.L_080e64a8
	cmp	r1, #34
	beq.n	.L_080e64a8
	cmp	r1, #100
	bne.n	.L_080e64c2
.L_080e64a8:
	ldr	r2, [sp, #92]
	ldr	r5, [pc, #364]
	adds	r3, r2, r5
	ldr	r3, [r3, #0]
	movs	r1, #36
	ldrsh	r0, [r3, r1]
	movs	r1, #4
	bl	sub_080b5088
	ldr	r3, [sp, #92]
	subs	r5, #128
	adds	r2, r3, r5
	b.n	.L_080e650c
.L_080e64c2:
	ldr	r0, [sp, #96]
	cmp	r0, #20
	beq.n	.L_080e64d0
	cmp	r0, #14
	beq.n	.L_080e64d0
	cmp	r0, #33
	bne.n	.L_080e64ec
.L_080e64d0:
	ldr	r1, [sp, #92]
	ldr	r2, [pc, #324]
	adds	r3, r1, r2
	ldr	r3, [r3, #0]
	movs	r1, #1
	movs	r5, #36
	ldrsh	r0, [r3, r5]
	bl	sub_080b5088
	ldr	r1, [pc, #312]
	ldr	r0, [sp, #92]
	movs	r3, #2
	adds	r2, r0, r1
	b.n	.L_080e650e
.L_080e64ec:
	ldr	r2, [sp, #96]
	cmp	r2, #30
	beq.n	.L_080e64f6
	cmp	r2, #8
	bne.n	.L_080e6510
.L_080e64f6:
	ldr	r5, [sp, #92]
	ldr	r0, [pc, #284]
	adds	r3, r5, r0
	ldr	r3, [r3, #0]
	movs	r1, #36
	ldrsh	r0, [r3, r1]
	movs	r1, #3
	bl	sub_080b5088
	ldr	r3, [pc, #272]
	adds	r2, r5, r3
.L_080e650c:
	movs	r3, #8
.L_080e650e:
	str	r3, [r2, #0]
.L_080e6510:
	ldr	r5, [sp, #84]
	cmp	r5, #6
	bne.n	.L_080e6530
	ldr	r0, [sp, #92]
	ldr	r1, [pc, #252]
	adds	r3, r0, r1
	ldr	r3, [r3, #0]
	movs	r2, #36
	ldrsh	r0, [r3, r2]
	movs	r3, #4
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #5
	movs	r3, #0
	bl	sub_080d6888
.L_080e6530:
	ldr	r3, [sp, #84]
	cmp	r3, #14
	bne.n	.L_080e6550
	ldr	r0, [pc, #224]
	ldr	r5, [sp, #92]
	adds	r3, r5, r0
	ldr	r3, [r3, #0]
	movs	r1, #36
	ldrsh	r0, [r3, r1]
	movs	r3, #4
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #5
	movs	r3, #0
	bl	sub_080d6888
.L_080e6550:
	movs	r1, #8
	movs	r0, #8
	bl	sub_080e155c
	bl	sub_080cd52c
	ldr	r5, [pc, #192]
	ldr	r3, [sp, #92]
	adds	r2, r3, r5
	movs	r3, #1
	movs	r0, #1
	str	r3, [r2, #0]
	bl	sub_080030f8
	ldr	r0, [sp, #84]
	ldr	r1, [sp, #64]
	adds	r0, #1
	str	r0, [sp, #84]
	cmp	r0, r1
	beq.n	.L_080e657c
	bl	.L_080e52f8
.L_080e657c:
	ldr	r2, [sp, #96]
	cmp	r2, #21
	bne.n	.L_080e65c4
	movs	r1, #128
	ldr	r5, [pc, #156]
	lsls	r1, r1, #7
	ldr	r0, [pc, #156]
	bl	sub_080072f8
	movs	r1, #128
	ldr	r0, [sp, #88]
	lsls	r1, r1, #7
	bl	sub_080072f8
	ldr	r0, [pc, #124]
	ldr	r5, [sp, #92]
	adds	r3, r5, r0
	ldr	r2, [r3, #0]
	movs	r3, #0
	str	r3, [r2, #28]
	ldr	r0, [pc, #132]
	bl	sub_08004278
	ldr	r0, [pc, #132]
	bl	sub_08004278
	movs	r0, #47
	bl	sub_08002dd8
	movs	r0, #46
	bl	sub_08002dd8
	ldr	r0, [sp, #100]
	bl	sub_080d9ac4
	b.n	.L_080e65f8
.L_080e65c4:
	ldr	r1, [sp, #20]
	cmp	r1, #1
	bls.n	.L_080e65dc
	ldr	r2, [sp, #96]
	cmp	r2, #12
	beq.n	.L_080e65dc
	cmp	r2, #22
	beq.n	.L_080e65dc
	cmp	r2, #28
	beq.n	.L_080e65dc
	cmp	r2, #29
	bne.n	.L_080e65e2
.L_080e65dc:
	ldr	r0, [pc, #84]
	bl	sub_08004278
.L_080e65e2:
	ldr	r0, [pc, #76]
	bl	sub_08004278
	movs	r0, #47
	bl	sub_08002dd8
	movs	r0, #46
	bl	sub_08002dd8
	bl	sub_080cdbc0
.L_080e65f8:
	add	sp, #184
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x02010000
	.4byte 0x02014000
	.4byte 0x02013c56
	.4byte 0x00007828
	.4byte 0x000077a8
	.4byte 0x00007824
	.4byte 0x03000164
	.4byte 0x06004000
	.4byte 0x080cd4b5
	.4byte 0x080cd261
	.4byte 0x080dbb9d
