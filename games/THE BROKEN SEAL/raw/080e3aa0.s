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
	.set sub_08004bd4, 0x08004bd4
	.set sub_08004c1c, 0x08004c1c
	.set sub_08004c6c, 0x08004c6c
	.set sub_080051d8, 0x080051d8
	.set sub_08005268, 0x08005268
	.set sub_080072f0, 0x080072f0
	.set sub_080072f4, 0x080072f4
	.set sub_080072f8, 0x080072f8
	.set sub_08009088, 0x08009088
	.set sub_08077008, 0x08077008
	.set sub_080b5030, 0x080b5030
	.set sub_080b5048, 0x080b5048
	.set sub_080b5058, 0x080b5058
	.set sub_080b5070, 0x080b5070
	.set sub_080b5088, 0x080b5088
	.set sub_080b5098, 0x080b5098
	.set sub_080b50e8, 0x080b50e8
	.set sub_080c9048, 0x080c9048
	.set sub_080cd594, 0x080cd594
	.set sub_080cdb24, 0x080cdb24
	.set sub_080cdd14, 0x080cdd14
	.set sub_080cdd58, 0x080cdd58
	.set sub_080cef64, 0x080cef64
	.set sub_080df9d0, 0x080df9d0
	.set sub_080e0524, 0x080e0524
	.set sub_080e38b8, 0x080e38b8
	.set sub_080e3944, 0x080e3944
	.set sub_080e3958, 0x080e3958
	.set sub_080e396c, 0x080e396c
	.set sub_080e4e0c, 0x080e4e0c
	.set sub_080e4e54, 0x080e4e54
	.set sub_080e4e58, 0x080e4e58
	.set sub_080ed408, 0x080ed408
	.set sub_080f9010, 0x080f9010
	.global Overlay_080e3aa0
Overlay_080e3aa0:
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
	.2byte 0xd261
	.2byte 0x080c
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #112]
	adds	r2, r3, #0
	adds	r1, r0, #0
	ldmia	r2!, {r0}
	ldr	r2, [r2, #0]
	sub	sp, #112
	str	r2, [sp, #52]
	ldr	r2, [r3, #8]
	str	r2, [sp, #44]
	subs	r3, #108
	ldr	r3, [r3, #0]
	str	r3, [sp, #40]
	ldr	r3, [r1, #0]
	mov	fp, r0
	movs	r6, #1
	cmp	r3, #199
	bgt.n	.L_080e40d6
	movs	r6, #0
.L_080e40d6:
	ldr	r5, [pc, #80]
	add	r5, fp
	movs	r2, #130
	ldr	r0, [r1, #8]
	str	r1, [r5, #0]
	ldr	r1, [r1, #12]
	bl	sub_080b5030
	movs	r0, #1
	bl	sub_080030f8
	bl	sub_080cdd58
	ldr	r3, [pc, #44]
	ldr	r2, [pc, #56]
	strh	r3, [r2, #0]
	ldr	r3, [r5, #0]
	ldr	r3, [r3, #4]
	cmp	r3, #0
	bne.n	.L_080e4130
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
	b.n	.L_080e4150
	movs	r0, r0
	.4byte 0x00001f80
	.4byte 0x03001eec
	.4byte 0x00007828
	.2byte 0x000a
	.2byte 0x0400
.L_080e4130:
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
.L_080e4150:
	ldr	r3, [pc, #720]
	adds	r2, r3, #0
	adds	r2, #184
	ldr	r2, [r2, #0]
	str	r2, [sp, #56]
	adds	r3, #188
	mov	r5, sp
	ldr	r3, [r3, #0]
	adds	r5, #56
	str	r5, [sp, #16]
	str	r3, [r5, #4]
	ldr	r5, [pc, #704]
	add	r5, fp
	ldr	r3, [r5, #0]
	movs	r2, #130
	ldr	r0, [r3, #8]
	ldr	r1, [r3, #12]
	bl	sub_080b5030
	movs	r0, #1
	bl	sub_080030f8
	ldr	r0, [pc, #684]
	mov	r1, fp
	movs	r2, #1
	movs	r3, #0
	bl	sub_080e0524
	ldr	r3, [r5, #0]
	movs	r2, #130
	ldr	r0, [r3, #8]
	ldr	r1, [r3, #12]
	bl	sub_080b5030
	movs	r0, #1
	bl	sub_080030f8
	movs	r2, #1
	movs	r3, #1
	ldr	r0, [pc, #656]
	ldr	r1, [pc, #656]
	bl	sub_080e0524
	ldr	r2, [r5, #0]
	ldr	r3, [r2, #8]
	cmp	r3, #7
	ble.n	.L_080e41c4
	ldr	r0, [pc, #648]
	bl	sub_08002f40
	adds	r1, r0, #0
	movs	r0, #160
	movs	r2, #128
	ldr	r3, [pc, #640]
	lsls	r0, r0, #19
	bl	sub_080072f0
	ldr	r2, [r5, #0]
.L_080e41c4:
	ldr	r1, [r2, #12]
	ldr	r0, [r2, #8]
	movs	r2, #130
	bl	sub_080b5030
	movs	r0, #1
	bl	sub_080030f8
	ldr	r0, [pc, #616]
	ldr	r1, [sp, #44]
	movs	r2, #0
	movs	r3, #0
	bl	sub_080e0524
	ldr	r3, [r5, #0]
	movs	r2, #130
	ldr	r0, [r3, #8]
	ldr	r1, [r3, #12]
	bl	sub_080b5030
	movs	r0, #1
	bl	sub_080030f8
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, fp
	movs	r3, #1
	str	r3, [r2, #0]
	ldr	r2, [pc, #580]
	movs	r3, #0
	add	r2, fp
	movs	r1, #144
	str	r3, [r2, #0]
	ldr	r0, [pc, #576]
	lsls	r1, r1, #3
	bl	sub_080041d8
	cmp	r6, #1
	beq.n	.L_080e4214
	b.n	.L_080e43cc
.L_080e4214:
	ldr	r3, [r5, #0]
	ldr	r0, [r3, #8]
	bl	sub_080b5098
	ldr	r0, [r0, #0]
	movs	r5, #225
	mov	r8, r0
	lsls	r5, r5, #7
	movs	r0, #0
	ldr	r6, [pc, #548]
	mov	sl, r0
	movs	r7, #0
	add	r5, fp
.L_080e422e:
	bl	sub_08004458
	movs	r3, #63
	ands	r3, r0
	adds	r3, #16
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
	bne.n	.L_080e422e
	mov	r0, r8
	movs	r1, #0
	bl	sub_08009088
	mov	r3, r8
	ldr	r3, [r3, #36]
	str	r3, [sp, #36]
	mov	r5, r8
	ldr	r5, [r5, #40]
	str	r5, [sp, #32]
	mov	r0, r8
	ldr	r0, [r0, #44]
	str	r0, [sp, #28]
	mov	r1, r8
	ldr	r1, [r1, #72]
	str	r1, [sp, #20]
	mov	r2, r8
	ldr	r2, [r2, #52]
	movs	r5, #0
	mov	r3, r8
	str	r2, [sp, #24]
	str	r5, [r3, #36]
	str	r5, [r3, #40]
	str	r5, [r3, #44]
	str	r5, [r3, #52]
	str	r5, [r3, #72]
	ldr	r3, [pc, #400]
	add	r3, fp
	ldr	r3, [r3, #0]
	mov	r1, sp
	ldr	r0, [r3, #8]
	adds	r1, #100
	str	r1, [sp, #12]
	bl	sub_080e396c
	ldr	r2, [sp, #12]
	ldr	r3, [r2, #0]
	mov	r0, sl
	subs	r0, r0, r3
	str	r0, [sp, #48]
	add	r1, sp, #48
	ldr	r2, [pc, #408]
	ldrh	r1, [r1, #0]
	movs	r3, #80
	strh	r1, [r2, #4]
	strh	r3, [r2, #6]
	ldr	r2, [pc, #404]
	movs	r3, #24
	add	r2, fp
	str	r3, [r2, #0]
	ldr	r3, [pc, #400]
	movs	r1, #200
	add	r3, fp
	str	r5, [r3, #0]
	lsls	r1, r1, #4
	ldr	r0, [pc, #392]
	bl	sub_080041d8
	movs	r0, #212
	bl	sub_080f9010
	mov	r9, r5
.L_080e42de:
	ldr	r3, [pc, #328]
	add	r3, fp
	ldr	r3, [r3, #0]
	movs	r2, #130
	ldr	r0, [r3, #8]
	ldr	r1, [r3, #12]
	movs	r6, #225
	bl	sub_080b5030
	lsls	r6, r6, #7
	movs	r2, #0
	mov	sl, r2
	add	r6, fp
.L_080e42f8:
	ldr	r3, [r6, #0]
	cmp	r3, #0
	blt.n	.L_080e4384
	mov	r3, sl
	cmp	r3, #0
	bge.n	.L_080e4306
	adds	r3, #3
.L_080e4306:
	asrs	r3, r3, #2
	cmp	r9, r3
	blt.n	.L_080e4384
	mov	r5, sl
	movs	r3, #1
	ands	r3, r5
	adds	r7, r3, #5
	bl	sub_080049ac
	ldr	r0, [r6, #20]
	bl	sub_08004c6c
	ldr	r0, [r6, #12]
	bl	sub_08004bd4
	ldr	r0, [r6, #16]
	bl	sub_08004c1c
	add	r5, sp, #76
	adds	r0, r6, #0
	adds	r1, r5, #0
	bl	sub_080e3944
	ldr	r3, [r5, #0]
	adds	r3, #64
	str	r3, [r5, #0]
	ldr	r2, [sp, #104]
	ldr	r3, [r5, #4]
	adds	r3, r3, r2
	adds	r3, #24
	str	r3, [r5, #4]
	movs	r2, #60
	ldr	r3, [r5, #8]
	negs	r2, r2
	cmp	r3, r2
	bge.n	.L_080e4352
	str	r2, [r5, #8]
	adds	r3, r2, #0
.L_080e4352:
	cmp	r3, #60
	ble.n	.L_080e435a
	movs	r3, #60
	str	r3, [r5, #8]
.L_080e435a:
	lsls	r0, r7, #1
	ldr	r2, [pc, #256]
	adds	r3, #60
	str	r3, [r5, #8]
	subs	r3, r0, #2
	ldrh	r1, [r2, r3]
	ldr	r2, [sp, #44]
	ldr	r3, [r5, #4]
	adds	r1, r2, r1
	ldr	r2, [r5, #0]
	subs	r3, r3, r7
	str	r0, [sp, #0]
	str	r0, [sp, #4]
	subs	r2, r2, r7
	ldr	r4, [sp, #56]
	ldr	r0, [sp, #52]
	bl	sub_080072f4
	ldr	r3, [r6, #0]
	subs	r3, #4
	str	r3, [r6, #0]
.L_080e4384:
	movs	r3, #1
	add	sl, r3
	mov	r5, sl
	adds	r6, #28
	cmp	r5, #64
	bne.n	.L_080e42f8
	ldr	r2, [pc, #208]
	add	r2, fp
	movs	r0, #1
	str	r3, [r2, #0]
	bl	sub_080030f8
	movs	r0, #1
	add	r9, r0
	mov	r1, r9
	cmp	r1, #32
	bne.n	.L_080e42de
	ldr	r0, [pc, #180]
	bl	sub_08004278
	mov	r0, r8
	movs	r1, #16
	bl	sub_08009088
	ldr	r2, [sp, #36]
	mov	r3, r8
	str	r2, [r3, #36]
	ldr	r5, [sp, #32]
	str	r5, [r3, #40]
	ldr	r0, [sp, #28]
	str	r0, [r3, #44]
	ldr	r1, [sp, #24]
	str	r1, [r3, #52]
	ldr	r2, [sp, #20]
	str	r2, [r3, #72]
	b.n	.L_080e43d2
.L_080e43cc:
	mov	r3, sp
	adds	r3, #100
	str	r3, [sp, #12]
.L_080e43d2:
	movs	r1, #128
	ldr	r5, [pc, #144]
	ldr	r0, [sp, #52]
	lsls	r1, r1, #7
	bl	sub_080072f8
	movs	r1, #128
	lsls	r1, r1, #7
	ldr	r0, [pc, #136]
	bl	sub_080072f8
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, fp
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r2, [pc, #80]
	movs	r3, #75
	add	r2, fp
	ldr	r5, [pc, #44]
	str	r3, [r2, #0]
	ldr	r2, [pc, #112]
	ldr	r3, [pc, #32]
	add	r5, fp
	strh	r3, [r2, #0]
	ldr	r3, [r5, #0]
	add	r6, sp, #88
	movs	r1, #36
	ldrsh	r0, [r3, r1]
	adds	r1, r6, #0
	bl	sub_080e396c
	ldr	r3, [r5, #0]
	ldr	r3, [r3, #4]
	cmp	r3, #0
	bne.n	.L_080e4474
	ldr	r2, [r6, #0]
	movs	r3, #32
	b.n	.L_080e4478
	.4byte 0x00001f81
	.4byte 0x03001e50
	.4byte 0x00007828
	.4byte 0x00000049
	.4byte 0x0000004a
	.4byte 0x02010000
	.4byte 0x0000008e
	.4byte 0x03001388
	.4byte 0x00000076
	.4byte 0x00007784
	.4byte 0x080cd261
	.4byte 0x0000ffff
	.4byte 0x03001ad0
	.4byte 0x000077b4
	.4byte 0x000077b8
	.4byte 0x080cd4b5
	.4byte 0x080ede5c
	.4byte 0x00007824
	.4byte 0x03000164
	.4byte 0x06004000
	.2byte 0x000a
	.2byte 0x0400
.L_080e4474:
	ldr	r2, [r6, #0]
	movs	r3, #96
.L_080e4478:
	subs	r3, r3, r2
	str	r3, [sp, #48]
	ldr	r2, [sp, #48]
	cmp	r2, #0
	ble.n	.L_080e4486
	movs	r3, #0
	str	r3, [sp, #48]
.L_080e4486:
	movs	r3, #128
	ldr	r5, [sp, #48]
	negs	r3, r3
	cmp	r5, r3
	bge.n	.L_080e4492
	str	r3, [sp, #48]
.L_080e4492:
	ldr	r0, [sp, #48]
	ldr	r3, [r6, #0]
	ldr	r5, [pc, #564]
	ldr	r2, [pc, #564]
	adds	r3, r3, r0
	str	r3, [r6, #0]
	add	r5, fp
	add	r1, sp, #48
	movs	r3, #80
	strh	r3, [r2, #6]
	ldrh	r1, [r1, #0]
	ldr	r3, [r5, #0]
	strh	r1, [r2, #4]
	movs	r2, #36
	ldrsh	r0, [r3, r2]
	bl	sub_080b5098
	ldr	r3, [r5, #0]
	ldr	r6, [r0, #0]
	movs	r5, #36
	ldrsh	r0, [r3, r5]
	bl	sub_080b5070
	lsrs	r3, r0, #31
	adds	r0, r0, r3
	asrs	r0, r0, #1
	movs	r5, #225
	mov	r8, r0
	lsls	r5, r5, #7
	movs	r0, #0
	mov	sl, r0
	movs	r7, #255
	add	r5, fp
.L_080e44d4:
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
	lsls	r0, r0, #10
	str	r0, [r5, #16]
	bl	sub_08004458
	ands	r0, r7
	subs	r0, #127
	ldr	r3, [r5, #0]
	lsls	r0, r0, #10
	str	r0, [r5, #20]
	cmp	r3, #0
	ble.n	.L_080e450e
	ldr	r3, [r5, #12]
	negs	r3, r3
	str	r3, [r5, #12]
.L_080e450e:
	movs	r1, #1
	mov	r3, sl
	add	sl, r1
	adds	r3, #16
	mov	r2, sl
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r2, #64
	bne.n	.L_080e44d4
	ldr	r5, [sp, #40]
	adds	r5, #12
	movs	r3, #0
	str	r5, [sp, #8]
	mov	r9, r3
.L_080e452a:
	mov	r0, r9
	cmp	r0, #5
	bne.n	.L_080e4536
	movs	r0, #134
	bl	sub_080b50e8
.L_080e4536:
	mov	r1, r9
	cmp	r1, #4
	bne.n	.L_080e454c
	ldr	r3, [pc, #396]
	add	r3, fp
	ldr	r3, [r3, #0]
	movs	r1, #0
	movs	r2, #36
	ldrsh	r0, [r3, r2]
	bl	sub_080b5088
.L_080e454c:
	ldr	r3, [pc, #380]
	add	r3, fp
	ldr	r3, [r3, #0]
	ldr	r1, [sp, #12]
	ldr	r0, [r3, #8]
	bl	sub_080e396c
	ldr	r5, [sp, #12]
	ldr	r3, [r5, #4]
	mov	r0, r9
	adds	r3, #16
	str	r3, [r5, #4]
	cmp	r0, #1
	bgt.n	.L_080e4576
	movs	r1, #120
	str	r1, [sp, #0]
	str	r1, [sp, #4]
	ldr	r4, [sp, #56]
	ldr	r0, [sp, #52]
	mov	r1, fp
	b.n	.L_080e45a0
.L_080e4576:
	mov	r2, r9
	cmp	r2, #3
	bgt.n	.L_080e458e
	movs	r1, #225
	movs	r3, #120
	lsls	r1, r1, #6
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	ldr	r4, [sp, #56]
	ldr	r0, [sp, #52]
	add	r1, fp
	b.n	.L_080e45a0
.L_080e458e:
	mov	r5, r9
	cmp	r5, #5
	bgt.n	.L_080e45aa
	movs	r0, #120
	str	r0, [sp, #0]
	str	r0, [sp, #4]
	ldr	r4, [sp, #56]
	ldr	r0, [sp, #52]
	ldr	r1, [pc, #308]
.L_080e45a0:
	movs	r2, #0
	movs	r3, #0
	bl	sub_080072f4
	b.n	.L_080e45c4
.L_080e45aa:
	mov	r1, r9
	cmp	r1, #7
	bgt.n	.L_080e45c4
	movs	r2, #120
	str	r2, [sp, #0]
	str	r2, [sp, #4]
	ldr	r4, [sp, #56]
	ldr	r0, [sp, #52]
	ldr	r1, [pc, #284]
	movs	r2, #0
	movs	r3, #0
	bl	sub_080072f4
.L_080e45c4:
	bl	sub_080049ac
	ldr	r0, [sp, #40]
	ldr	r1, [sp, #8]
	bl	sub_080051d8
	mov	r3, r9
	subs	r3, #4
	cmp	r3, #27
	bhi.n	.L_080e465a
	movs	r5, #64
	movs	r3, #0
	add	r5, sp
	mov	sl, r3
	mov	r8, r5
.L_080e45e2:
	mov	r0, sl
	lsrs	r3, r0, #31
	add	r3, sl
	asrs	r5, r3, #1
	lsls	r3, r5, #3
	subs	r3, r3, r5
	lsls	r3, r3, #2
	movs	r1, #225
	add	r3, fp
	lsls	r1, r1, #7
	adds	r7, r3, r1
	ldr	r6, [r7, #24]
	cmp	r6, #0
	ble.n	.L_080e4650
	mov	r1, r8
	adds	r0, r7, #0
	bl	sub_080e3944
	mov	r3, r8
	ldr	r2, [r3, #0]
	ldr	r0, [sp, #48]
	adds	r2, r2, r0
	str	r2, [r3, #0]
	asrs	r6, r6, #3
	ldr	r3, [r3, #4]
	adds	r6, #2
	movs	r0, #1
	lsls	r4, r6, #1
	adds	r3, #16
	mov	r1, r8
	ands	r0, r5
	ldr	r5, [pc, #184]
	str	r3, [r1, #4]
	subs	r1, r4, #2
	ldrh	r1, [r5, r1]
	ldr	r5, [sp, #44]
	str	r4, [sp, #0]
	adds	r1, r5, r1
	str	r4, [sp, #4]
	ldr	r5, [sp, #16]
	lsls	r0, r0, #2
	subs	r3, r3, r6
	ldr	r4, [r0, r5]
	subs	r2, r2, r6
	ldr	r0, [sp, #52]
	bl	sub_080072f4
	adds	r0, r7, #0
	movs	r1, #60
	ldr	r2, [pc, #152]
	bl	sub_080e38b8
	ldr	r3, [r7, #24]
	subs	r3, #1
	str	r3, [r7, #24]
.L_080e4650:
	movs	r0, #1
	add	sl, r0
	mov	r1, sl
	cmp	r1, #64
	bne.n	.L_080e45e2
.L_080e465a:
	ldr	r2, [pc, #136]
	movs	r3, #1
	add	r2, fp
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_080030f8
	movs	r2, #1
	add	r9, r2
	mov	r3, r9
	cmp	r3, #32
	beq.n	.L_080e4674
	b.n	.L_080e452a
.L_080e4674:
	ldr	r0, [pc, #112]
	bl	sub_08004278
	movs	r0, #47
	bl	sub_08002dd8
	movs	r0, #46
	bl	sub_08002dd8
	ldr	r3, [pc, #72]
	mov	r5, r9
	strh	r5, [r3, #6]
	ldr	r3, [pc, #92]
	movs	r1, #201
	ldr	r3, [r3, #0]
	movs	r0, #0
	lsls	r1, r1, #3
	mov	r9, r0
	adds	r5, r3, r1
	movs	r6, #6
.L_080e469c:
	mov	r2, r9
	ldrh	r0, [r5, #0]
	subs	r1, r6, r2
	bl	sub_080b5048
	movs	r0, #1
	bl	sub_080030f8
	movs	r3, #1
	add	r9, r3
	mov	r0, r9
	cmp	r0, #7
	bne.n	.L_080e469c
	bl	sub_080cdd14
	add	sp, #112
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x00007828
	.4byte 0x03001ad0
	.4byte 0x02010000
	.4byte 0x02013840
	.4byte 0x080ede5c
	.4byte 0xfffffc00
	.4byte 0x00007824
	.4byte 0x080cd261
	.2byte 0x1e74
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r1, #160
	lsls	r1, r1, #19
	sub	sp, #128
	mov	r8, r1
	bl	sub_08002f40
	ldr	r3, [pc, #32]
	adds	r1, r0, #0
	movs	r2, #128
	mov	r0, sp
	bl	sub_080072f0
	movs	r3, #0
	mov	r2, sp
	strh	r3, [r2, #0]
	mov	ip, r3
	ldr	r7, [pc, #8]
	movs	r3, #31
	mov	sl, r3
	mov	lr, r2
	movs	r6, #0
	b.n	.L_080e472c
	.4byte 0x0000001f
	.2byte 0x1388
	.2byte 0x0300
.L_080e472c:
	mov	r1, r8
	ldrh	r3, [r1, #0]
	mov	r5, sl
	mov	r1, lr
	ands	r5, r3
	lsls	r3, r3, #16
	lsrs	r2, r3, #21
	lsrs	r0, r3, #26
	ldrh	r3, [r6, r1]
	mov	r1, sl
	ands	r1, r3
	lsls	r3, r3, #16
	lsrs	r4, r3, #21
	lsrs	r3, r3, #26
	ands	r2, r7
	ands	r0, r7
	ands	r4, r7
	ands	r3, r7
	cmp	r5, r1
	bge.n	.L_080e4758
	adds	r5, #1
	b.n	.L_080e475e
.L_080e4758:
	cmp	r5, r1
	ble.n	.L_080e475e
	subs	r5, #1
.L_080e475e:
	cmp	r2, r4
	bge.n	.L_080e4766
	adds	r2, #1
	b.n	.L_080e476c
.L_080e4766:
	cmp	r2, r4
	ble.n	.L_080e476c
	subs	r2, #1
.L_080e476c:
	cmp	r0, r3
	bge.n	.L_080e4774
	adds	r0, #1
	b.n	.L_080e477a
.L_080e4774:
	cmp	r0, r3
	ble.n	.L_080e477a
	subs	r0, #1
.L_080e477a:
	lsls	r2, r2, #5
	lsls	r3, r0, #10
	orrs	r3, r2
	orrs	r3, r5
	mov	r2, lr
	strh	r3, [r6, r2]
	movs	r3, #1
	add	ip, r3
	movs	r1, #2
	mov	r2, ip
	adds	r6, #2
	add	r8, r1
	cmp	r2, #64
	bne.n	.L_080e472c
	movs	r0, #160
	mov	r1, sp
	ldr	r3, [pc, #24]
	movs	r2, #128
	lsls	r0, r0, #19
	bl	sub_080072f0
	add	sp, #128
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.2byte 0x1388
	.2byte 0x0300
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
