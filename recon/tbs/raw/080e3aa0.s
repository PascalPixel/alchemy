.syntax unified
	.thumb
	.set sub_080022ec, 0x080022ec
	.set sub_08002dd8, 0x08002dd8
	.set sub_08002f40, 0x08002f40
	.set sub_080030f8, 0x080030f8
	.set sub_080041d8, 0x080041d8
	.set sub_08004278, 0x08004278
	.set sub_08004458, 0x08004458
	.set sub_080049ac, 0x080049ac
	.set sub_080051d8, 0x080051d8
	.set sub_08005268, 0x08005268
	.set sub_080072f0, 0x080072f0
	.set sub_080072f4, 0x080072f4
	.set sub_08077008, 0x08077008
	.set sub_080b5058, 0x080b5058
	.set sub_080b5070, 0x080b5070
	.set sub_080b5088, 0x080b5088
	.set sub_080b5098, 0x080b5098
	.set sub_080b50e8, 0x080b50e8
	.set sub_080c9048, 0x080c9048
	.set sub_080cdd14, 0x080cdd14
	.set sub_080cdd58, 0x080cdd58
	.set sub_080e0524, 0x080e0524
	.set sub_080e38b8, 0x080e38b8
	.set sub_080e3958, 0x080e3958
	.set sub_080e396c, 0x080e396c
	.set sub_080ed408, 0x080ed408
	.global BattlePres_RunBeamSequence
	.global Func_080e3aa0
	.thumb_func
BattlePres_RunBeamSequence:
Func_080e3aa0:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r2, [pc, #88]
	adds	r3, r2, #0
	ldmia	r3!, {r1}
	ldr	r3, [r3, #0]
	sub	sp, #92
	str	r3, [sp, #44]
	ldr	r3, [r2, #8]
	str	r3, [sp, #36]
	subs	r2, #108
	ldr	r2, [r2, #0]
	str	r2, [sp, #32]
	ldr	r5, [r0, #0]
	str	r5, [sp, #28]
	ldr	r5, [pc, #64]
	mov	fp, r1
	add	r5, fp
	str	r0, [r5, #0]
	ldr	r0, [r0, #8]
	bl	sub_08077008
	str	r0, [sp, #24]
	movs	r0, #1
	bl	sub_080030f8
	bl	sub_080c9048
	bl	sub_080cdd58
	ldr	r2, [pc, #40]
	ldr	r3, [pc, #24]
	movs	r0, #1
	strh	r3, [r2, #0]
	bl	sub_080030f8
	ldr	r6, [sp, #28]
	cmp	r6, #5
	bne.n	.L_080e3b58
	ldr	r3, [r5, #0]
	ldr	r3, [r3, #4]
	cmp	r3, #0
	bne.n	.L_080e3b36
	b.n	.L_080e3b14
	movs	r0, r0
	.4byte 0x00001f80
	.4byte 0x03001eec
	.4byte 0x00007828
	.2byte 0x000a
	.2byte 0x0400
.L_080e3b14:
	movs	r3, #3
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #7
	movs	r3, #11
	movs	r0, #46
	bl	sub_080ed408
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r0, #47
	movs	r1, #7
	movs	r2, #7
	movs	r3, #11
	bl	sub_080ed408
	b.n	.L_080e3ba0
.L_080e3b36:
	movs	r3, #3
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #7
	movs	r3, #15
	movs	r0, #46
	bl	sub_080ed408
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r0, #47
	movs	r1, #7
	movs	r2, #7
	movs	r3, #15
	bl	sub_080ed408
	b.n	.L_080e3ba0
.L_080e3b58:
	ldr	r3, [r5, #0]
	ldr	r3, [r3, #4]
	cmp	r3, #0
	bne.n	.L_080e3b80
	movs	r3, #3
	movs	r1, #7
	movs	r2, #7
	movs	r0, #46
	str	r3, [sp, #0]
	bl	sub_080ed408
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r0, #47
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	bl	sub_080ed408
	b.n	.L_080e3ba0
.L_080e3b80:
	movs	r3, #3
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #7
	movs	r3, #7
	movs	r0, #46
	bl	sub_080ed408
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r0, #47
	movs	r1, #7
	movs	r2, #7
	movs	r3, #7
	bl	sub_080ed408
.L_080e3ba0:
	ldr	r3, [pc, #260]
	adds	r2, r3, #0
	adds	r2, #184
	ldr	r2, [r2, #0]
	str	r2, [sp, #48]
	adds	r3, #188
	ldr	r3, [r3, #0]
	mov	r0, sp
	adds	r0, #48
	str	r0, [sp, #16]
	str	r3, [r0, #4]
	movs	r0, #1
	bl	sub_080030f8
	ldr	r1, [sp, #28]
	cmp	r1, #4
	bne.n	.L_080e3bc6
	ldr	r0, [pc, #232]
	b.n	.L_080e3bf2
.L_080e3bc6:
	ldr	r2, [sp, #28]
	cmp	r2, #3
	bne.n	.L_080e3bda
	ldr	r0, [pc, #224]
	mov	r1, fp
	movs	r2, #0
	movs	r3, #0
	bl	sub_080e0524
	b.n	.L_080e3c0a
.L_080e3bda:
	ldr	r3, [sp, #28]
	cmp	r3, #2
	beq.n	.L_080e3bfe
	cmp	r3, #2
	bgt.n	.L_080e3bea
	cmp	r3, #0
	blt.n	.L_080e3c0a
	b.n	.L_080e3bf0
.L_080e3bea:
	ldr	r5, [sp, #28]
	cmp	r5, #5
	bne.n	.L_080e3c0a
.L_080e3bf0:
	ldr	r0, [pc, #192]
.L_080e3bf2:
	mov	r1, fp
	movs	r2, #1
	movs	r3, #1
	bl	sub_080e0524
	b.n	.L_080e3c0a
.L_080e3bfe:
	ldr	r0, [pc, #184]
	mov	r1, fp
	movs	r2, #1
	movs	r3, #1
	bl	sub_080e0524
.L_080e3c0a:
	ldr	r3, [pc, #176]
	add	r3, fp
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #8]
	cmp	r3, #7
	ble.n	.L_080e3c2c
	ldr	r0, [pc, #168]
	bl	sub_08002f40
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #160]
	lsls	r0, r0, #19
	movs	r2, #128
	bl	sub_080072f0
	b.n	.L_080e3c40
.L_080e3c2c:
	ldr	r0, [pc, #152]
	bl	sub_08002f40
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #140]
	lsls	r0, r0, #19
	movs	r2, #128
	bl	sub_080072f0
.L_080e3c40:
	movs	r0, #1
	bl	sub_080030f8
	ldr	r0, [pc, #132]
	ldr	r1, [sp, #36]
	movs	r2, #0
	movs	r3, #0
	bl	sub_080e0524
	ldr	r0, [pc, #124]
	ldr	r1, [pc, #124]
	movs	r2, #1
	movs	r3, #0
	bl	sub_080e0524
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, fp
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r2, [pc, #108]
	movs	r3, #50
	add	r2, fp
	movs	r1, #144
	str	r3, [r2, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #100]
	bl	sub_080041d8
	ldr	r5, [pc, #64]
	ldr	r2, [pc, #96]
	ldr	r3, [pc, #36]
	add	r5, fp
	strh	r3, [r2, #0]
	ldr	r3, [r5, #0]
	mov	r1, sp
	movs	r6, #36
	ldrsh	r0, [r3, r6]
	adds	r1, #68
	str	r1, [sp, #20]
	bl	sub_080e3958
	ldr	r3, [r5, #0]
	ldr	r3, [r3, #4]
	cmp	r3, #0
	bne.n	.L_080e3ce4
	ldr	r3, [sp, #20]
	ldr	r2, [r3, #0]
	movs	r3, #96
	b.n	.L_080e3cea
	.4byte 0x00001f81
	.4byte 0x03001e50
	.4byte 0x0000006b
	.4byte 0x000000c5
	.4byte 0x000000b5
	.4byte 0x000000b6
	.4byte 0x00007828
	.4byte 0x0000008e
	.4byte 0x03001388
	.4byte 0x0000004a
	.4byte 0x00000076
	.4byte 0x00000099
	.4byte 0x02010000
	.4byte 0x00007784
	.4byte 0x080cd261
	.2byte 0x000a
	.2byte 0x0400
.L_080e3ce4:
	ldr	r5, [sp, #20]
	ldr	r2, [r5, #0]
	movs	r3, #32
.L_080e3cea:
	subs	r3, r3, r2
	str	r3, [sp, #40]
	ldr	r6, [sp, #40]
	cmp	r6, #0
	ble.n	.L_080e3cf8
	movs	r0, #0
	str	r0, [sp, #40]
.L_080e3cf8:
	movs	r3, #128
	ldr	r1, [sp, #40]
	negs	r3, r3
	cmp	r1, r3
	bge.n	.L_080e3d04
	str	r3, [sp, #40]
.L_080e3d04:
	ldr	r2, [sp, #20]
	ldr	r5, [sp, #40]
	ldr	r3, [r2, #0]
	add	r6, sp, #40
	adds	r3, r3, r5
	str	r3, [r2, #0]
	ldrh	r6, [r6, #0]
	ldr	r2, [pc, #868]
	ldr	r5, [pc, #868]
	movs	r3, #80
	strh	r6, [r2, #4]
	strh	r3, [r2, #6]
	add	r5, fp
	movs	r0, #1
	bl	sub_080030f8
	ldr	r3, [r5, #0]
	movs	r1, #36
	ldrsh	r0, [r3, r1]
	bl	sub_080b5098
	ldr	r3, [r5, #0]
	ldr	r6, [r0, #0]
	movs	r2, #36
	ldrsh	r0, [r3, r2]
	bl	sub_080b5070
	lsrs	r3, r0, #31
	adds	r0, r0, r3
	movs	r5, #225
	asrs	r0, r0, #1
	movs	r3, #0
	lsls	r5, r5, #7
	mov	r8, r0
	mov	sl, r3
	movs	r7, #255
	add	r5, fp
.L_080e3d4e:
	ldr	r3, [r6, #8]
	str	r3, [r5, #0]
	ldr	r3, [r6, #12]
	add	r3, r8
	str	r3, [r5, #4]
	ldr	r3, [r6, #16]
	str	r3, [r5, #8]
	bl	sub_08004458
	ands	r0, r7
	lsls	r0, r0, #10
	str	r0, [r5, #12]
	bl	sub_08004458
	ands	r0, r7
	subs	r0, #32
	lsls	r0, r0, #10
	str	r0, [r5, #16]
	bl	sub_08004458
	ands	r0, r7
	subs	r0, #127
	ldr	r3, [r5, #0]
	lsls	r0, r0, #10
	str	r0, [r5, #20]
	cmp	r3, #0
	ble.n	.L_080e3d8a
	ldr	r3, [r5, #12]
	negs	r3, r3
	str	r3, [r5, #12]
.L_080e3d8a:
	ldr	r3, [r5, #12]
	movs	r0, #1
	negs	r3, r3
	str	r3, [r5, #12]
	mov	r3, sl
	add	sl, r0
	adds	r3, #16
	mov	r1, sl
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r1, #64
	bne.n	.L_080e3d4e
	ldr	r5, [sp, #32]
	mov	r3, sp
	adds	r3, #80
	adds	r5, #12
	movs	r2, #0
	str	r3, [sp, #12]
	str	r5, [sp, #8]
	mov	r9, r2
.L_080e3db2:
	mov	r6, r9
	cmp	r6, #5
	bne.n	.L_080e3dd8
	ldr	r0, [sp, #24]
	movs	r1, #148
	lsls	r1, r1, #1
	adds	r3, r0, r1
	ldrb	r0, [r3, #0]
	bl	sub_080b5058
	cmp	r0, #0
	beq.n	.L_080e3dd2
	movs	r0, #134
	bl	sub_080b50e8
	b.n	.L_080e3dd8
.L_080e3dd2:
	movs	r0, #133
	bl	sub_080b50e8
.L_080e3dd8:
	mov	r2, r9
	cmp	r2, #4
	bne.n	.L_080e3dee
	ldr	r3, [pc, #668]
	add	r3, fp
	ldr	r3, [r3, #0]
	movs	r1, #0
	movs	r5, #36
	ldrsh	r0, [r3, r5]
	bl	sub_080b5088
.L_080e3dee:
	ldr	r6, [pc, #652]
	add	r6, fp
	ldr	r3, [r6, #0]
	ldr	r1, [sp, #12]
	ldr	r0, [r3, #8]
	bl	sub_080e396c
	ldr	r1, [sp, #12]
	ldr	r0, [r1, #4]
	adds	r7, r0, #0
	adds	r7, #16
	str	r7, [r1, #4]
	ldr	r2, [sp, #28]
	cmp	r2, #4
	bne.n	.L_080e3e7a
	mov	r3, r9
	cmp	r3, #11
	ble.n	.L_080e3e14
	b.n	.L_080e3f6e
.L_080e3e14:
	ldr	r3, [r6, #0]
	ldr	r3, [r3, #4]
	cmp	r3, #0
	bne.n	.L_080e3e4c
	mov	r5, r9
	lsrs	r2, r5, #31
	add	r2, r9
	asrs	r2, r2, #1
	movs	r3, #5
	subs	r3, r3, r2
	ldr	r6, [sp, #12]
	lsls	r1, r3, #1
	ldr	r2, [r6, #0]
	adds	r1, r1, r3
	ldr	r3, [sp, #40]
	movs	r5, #48
	adds	r2, r2, r3
	adds	r3, r0, #0
	movs	r0, #16
	str	r0, [sp, #4]
	str	r5, [sp, #0]
	ldr	r6, [sp, #16]
	lsls	r1, r1, #8
	add	r1, fp
	subs	r2, #48
	adds	r3, #8
	ldr	r4, [r6, #4]
	b.n	.L_080e3f32
.L_080e3e4c:
	mov	r1, r9
	lsrs	r2, r1, #31
	add	r2, r9
	asrs	r2, r2, #1
	movs	r3, #5
	subs	r3, r3, r2
	lsls	r1, r3, #1
	adds	r1, r1, r3
	ldr	r3, [sp, #12]
	movs	r6, #48
	ldr	r2, [r3, #0]
	adds	r3, r0, #0
	movs	r0, #16
	str	r0, [sp, #4]
	ldr	r5, [sp, #40]
	ldr	r0, [sp, #16]
	str	r6, [sp, #0]
	lsls	r1, r1, #8
	ldr	r4, [r0, #4]
	add	r1, fp
	adds	r2, r2, r5
	adds	r3, #8
	b.n	.L_080e3f32
.L_080e3e7a:
	ldr	r1, [sp, #28]
	cmp	r1, #2
	bls.n	.L_080e3e84
	cmp	r1, #5
	bne.n	.L_080e3eee
.L_080e3e84:
	mov	r2, r9
	cmp	r2, #11
	bgt.n	.L_080e3f6e
	ldr	r3, [r6, #0]
	ldr	r3, [r3, #4]
	cmp	r3, #0
	bne.n	.L_080e3ec0
	lsrs	r3, r2, #31
	add	r3, r9
	asrs	r3, r3, #1
	lsls	r1, r3, #3
	subs	r1, r1, r3
	lsls	r1, r1, #2
	subs	r1, r1, r3
	ldr	r3, [sp, #12]
	ldr	r5, [sp, #40]
	ldr	r2, [r3, #0]
	movs	r6, #48
	adds	r3, r0, #0
	movs	r0, #72
	str	r0, [sp, #4]
	str	r6, [sp, #0]
	ldr	r0, [sp, #16]
	lsls	r1, r1, #7
	adds	r2, r2, r5
	ldr	r4, [r0, #4]
	add	r1, fp
	subs	r2, #48
	subs	r3, #24
	b.n	.L_080e3f32
.L_080e3ec0:
	mov	r1, r9
	lsrs	r3, r1, #31
	add	r3, r9
	asrs	r3, r3, #1
	lsls	r1, r3, #3
	subs	r1, r1, r3
	lsls	r1, r1, #2
	subs	r1, r1, r3
	ldr	r3, [sp, #12]
	movs	r6, #48
	ldr	r2, [r3, #0]
	adds	r3, r0, #0
	movs	r0, #72
	str	r0, [sp, #4]
	ldr	r5, [sp, #40]
	ldr	r0, [sp, #16]
	str	r6, [sp, #0]
	lsls	r1, r1, #7
	ldr	r4, [r0, #4]
	add	r1, fp
	adds	r2, r2, r5
	subs	r3, #24
	b.n	.L_080e3f32
.L_080e3eee:
	mov	r1, r9
	cmp	r1, #17
	bgt.n	.L_080e3f6e
	mov	r0, r9
	movs	r1, #3
	bl	sub_080022ec
	ldr	r3, [r6, #0]
	ldr	r3, [r3, #4]
	adds	r5, r0, #0
	cmp	r3, #0
	bne.n	.L_080e3f3a
	ldr	r2, [pc, #376]
	lsls	r3, r5, #1
	ldrh	r1, [r2, r3]
	ldr	r6, [sp, #12]
	ldr	r3, [pc, #372]
	ldr	r2, [r6, #0]
	ldrb	r3, [r3, r5]
	ldr	r0, [sp, #40]
	adds	r2, r2, r3
	adds	r2, r2, r0
	ldr	r3, [pc, #364]
	ldr	r0, [pc, #364]
	ldrb	r4, [r3, r5]
	ldrb	r0, [r0, r5]
	str	r4, [sp, #4]
	str	r0, [sp, #0]
	ldr	r5, [sp, #16]
	lsrs	r3, r4, #1
	add	r1, fp
	subs	r2, #58
	subs	r3, r7, r3
	ldr	r4, [r5, #4]
.L_080e3f32:
	ldr	r0, [sp, #44]
	bl	sub_080072f4
	b.n	.L_080e3f6e
.L_080e3f3a:
	ldr	r2, [pc, #324]
	lsls	r3, r5, #1
	ldrh	r1, [r2, r3]
	ldr	r6, [sp, #12]
	ldr	r3, [pc, #320]
	ldr	r2, [r6, #0]
	ldrb	r3, [r3, r5]
	subs	r2, r2, r3
	ldr	r3, [pc, #320]
	ldr	r0, [sp, #40]
	ldrb	r4, [r3, r5]
	ldr	r3, [pc, #308]
	adds	r2, r2, r0
	ldrb	r0, [r3, r5]
	str	r4, [sp, #0]
	str	r0, [sp, #4]
	ldr	r5, [sp, #16]
	subs	r2, r2, r4
	lsrs	r3, r0, #1
	add	r1, fp
	adds	r2, #58
	subs	r3, r7, r3
	ldr	r4, [r5, #4]
	ldr	r0, [sp, #44]
	bl	sub_080072f4
.L_080e3f6e:
	mov	r5, r9
	subs	r5, #4
	cmp	r5, #11
	bhi.n	.L_080e3fa0
	lsrs	r3, r5, #31
	adds	r3, r5, r3
	asrs	r3, r3, #1
	ldr	r0, [sp, #20]
	lsls	r1, r3, #4
	subs	r1, r1, r3
	ldr	r6, [pc, #268]
	ldr	r2, [r0, #0]
	ldr	r3, [r0, #4]
	lsls	r1, r1, #7
	movs	r0, #40
	adds	r1, r1, r6
	movs	r6, #48
	str	r0, [sp, #0]
	subs	r2, #16
	subs	r3, #24
	str	r6, [sp, #4]
	ldr	r4, [sp, #48]
	ldr	r0, [sp, #44]
	bl	sub_080072f4
.L_080e3fa0:
	bl	sub_080049ac
	ldr	r0, [sp, #32]
	ldr	r1, [sp, #8]
	bl	sub_080051d8
	cmp	r5, #27
	bhi.n	.L_080e402e
	movs	r1, #56
	movs	r0, #0
	add	r1, sp
	mov	sl, r0
	mov	r8, r1
.L_080e3fba:
	mov	r2, sl
	lsrs	r3, r2, #31
	add	r3, sl
	asrs	r6, r3, #1
	lsls	r3, r6, #3
	subs	r3, r3, r6
	lsls	r3, r3, #2
	movs	r5, #225
	lsls	r5, r5, #7
	add	r3, fp
	adds	r7, r3, r5
	ldr	r5, [r7, #24]
	cmp	r5, #0
	ble.n	.L_080e4024
	mov	r1, r8
	adds	r0, r7, #0
	bl	sub_08005268
	mov	r0, r8
	ldr	r2, [r0, #0]
	ldr	r1, [sp, #40]
	asrs	r5, r5, #3
	adds	r5, #2
	lsls	r4, r5, #1
	adds	r2, r2, r1
	ldr	r1, [pc, #164]
	str	r2, [r0, #0]
	subs	r3, r4, #2
	ldrh	r1, [r1, r3]
	movs	r0, #1
	ldr	r3, [sp, #36]
	ands	r0, r6
	mov	r6, r8
	adds	r1, r3, r1
	ldr	r3, [r6, #4]
	subs	r2, r2, r5
	subs	r3, r3, r5
	str	r4, [sp, #0]
	str	r4, [sp, #4]
	ldr	r5, [sp, #16]
	lsls	r0, r0, #2
	ldr	r4, [r0, r5]
	ldr	r0, [sp, #44]
	bl	sub_080072f4
	adds	r0, r7, #0
	movs	r1, #60
	ldr	r2, [pc, #124]
	bl	sub_080e38b8
	ldr	r3, [r7, #24]
	subs	r3, #1
	str	r3, [r7, #24]
.L_080e4024:
	movs	r6, #1
	add	sl, r6
	mov	r0, sl
	cmp	r0, #64
	bne.n	.L_080e3fba
.L_080e402e:
	ldr	r2, [pc, #108]
	movs	r3, #1
	add	r2, fp
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_080030f8
	movs	r1, #1
	add	r9, r1
	mov	r2, r9
	cmp	r2, #32
	beq.n	.L_080e4048
	b.n	.L_080e3db2
.L_080e4048:
	ldr	r0, [pc, #84]
	bl	sub_08004278
	movs	r0, #47
	bl	sub_08002dd8
	movs	r0, #46
	bl	sub_08002dd8
	ldr	r3, [pc, #28]
	mov	r5, r9
	strh	r5, [r3, #6]
	bl	sub_080cdd14
	add	sp, #92
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001ad0
	.4byte 0x00007828
	.4byte 0x080eedbe
	.4byte 0x080eedca
	.4byte 0x080eedb8
	.4byte 0x080eedb2
	.4byte 0x02010000
	.4byte 0x080ede5c
	.4byte 0xfffffc00
	.4byte 0x00007824
	.4byte 0x080cd261
