@ 呼出しグラフから到達した領域の再構築サム逆アセンブル。
@ （コードとデータが混在）。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.set sub_08004458, 0x08004458
	.set sub_080049ac, 0x080049ac
	.set sub_08004bd4, 0x08004bd4
	.set sub_08004c1c, 0x08004c1c
	.set sub_080072f0, 0x080072f0
	.set sub_080072f4, 0x080072f4
	.set sub_080d6888, 0x080d6888
	.set sub_080e3944, 0x080e3944
	.set sub_080e6288, 0x080e6288
	.set sub_080e62a2, 0x080e62a2
	.set sub_080e62cc, 0x080e62cc
	.set sub_080e640e, 0x080e640e
	.set sub_080f9010, 0x080f9010
	.global Region_080e5e28
Region_080e5e28:
	ldr r1, [sp, #96]
	cmp r1, #32
	beq .L_080e5e30
	b .L_080e600a
.L_080e5e30:
	ldr r2, [sp, #72]
	ldr r3, [sp, #68]
	ldr r5, [sp, #84]
	adds r2, r2, r3
	str r2, [sp, #72]
	cmp r5, #6
	ble .L_080e5e50
	ldr r0, [sp, #68]
	lsls r3, r3, #1
	adds r3, r3, r0
	lsls r3, r3, #4
	cmp r3, #0
	bge .L_080e5e4c
	adds r3, #63
.L_080e5e4c:
	asrs r3, r3, #6
	str r3, [sp, #68]
.L_080e5e50:
	ldr r1, [sp, #72]
	ldr r2, [pc, #76]
	asrs r3, r1, #16
	lsls r3, r3, #8
	str r3, [r2, #0]
	ldr r1, [sp, #84]
	subs r1, #16
	cmp r1, #15
	bhi .L_080e5e6e
	ldr r2, [pc, #52]
	subs r2, r2, r1
	ldr r1, [pc, #52]
	ldr r3, [pc, #56]
	orrs r2, r1
	strh r2, [r3, #0]
.L_080e5e6e:
	ldr r3, [sp, #84]
	subs r3, #4
	cmp r3, #1
	bhi .L_080e5e84
	movs r1, #128
	ldr r3, [pc, #44]
	ldr r0, [sp, #88]
	lsls r1, r1, #7
	ldr r2, [pc, #44]
	bl sub_080072f0
.L_080e5e84:
	ldr r2, [sp, #84]
	cmp r2, #3
	bgt .L_080e5ee2
	ldr r5, [sp, #92]
	ldr r0, [pc, #32]
	adds r3, r5, r0
	ldr r3, [r3, #0]
	ldr r3, [r3, #4]
	b .L_080e5eb4
	.2byte 0x0000
	.4byte 0x00000010
	.4byte 0x00001000
	.4byte 0x04000028
	.4byte 0x04000052
	.4byte 0x03000168
	.4byte 0x3f3f3f3f
	.4byte 0x00007828
.L_080e5eb4:
	cmp r3, #1
	bne .L_080e5eca
	movs r3, #80
	str r3, [sp, #0]
	movs r3, #104
	str r3, [sp, #4]
	ldr r4, [sp, #104]
	ldr r0, [sp, #88]
	adds r1, r5, #0
	movs r2, #0
	b .L_080e5eda
.L_080e5eca:
	movs r3, #80
	str r3, [sp, #0]
	movs r3, #104
	str r3, [sp, #4]
	ldr r4, [sp, #104]
	ldr r0, [sp, #88]
	ldr r1, [sp, #92]
	movs r2, #48
.L_080e5eda:
	movs r3, #24
	bl sub_080072f4
	b sub_080e640e
.L_080e5ee2:
	ldr r1, [sp, #84]
	cmp r1, #7
	bgt .L_080e5f24
	ldr r2, [sp, #92]
	ldr r5, [pc, #872]
	adds r3, r2, r5
	ldr r3, [r3, #0]
	ldr r3, [r3, #4]
	cmp r3, #1
	bne .L_080e5f0e
	movs r3, #80
	str r3, [sp, #0]
	movs r3, #104
	str r3, [sp, #4]
	adds r1, r2, #0
	ldr r4, [sp, #104]
	ldr r0, [sp, #88]
	movs r2, #0
	movs r3, #24
	bl sub_080072f4
	b .L_080e5f24
.L_080e5f0e:
	movs r3, #80
	str r3, [sp, #0]
	movs r3, #104
	str r3, [sp, #4]
	ldr r4, [sp, #104]
	ldr r0, [sp, #88]
	ldr r1, [sp, #92]
	movs r2, #48
	movs r3, #24
	bl sub_080072f4
.L_080e5f24:
	ldr r0, [sp, #92]
	ldr r1, [pc, #812]
	adds r3, r0, r1
	ldr r3, [r3, #0]
	ldr r3, [r3, #4]
	cmp r3, #1
	bne .L_080e5f4e
	movs r3, #80
	movs r2, #240
	lsls r2, r2, #5
	str r3, [sp, #0]
	movs r3, #104
	adds r1, r0, r2
	str r3, [sp, #4]
	ldr r4, [sp, #104]
	ldr r0, [sp, #88]
	movs r2, #16
	movs r3, #16
	bl sub_080072f4
	b .L_080e5f6a
.L_080e5f4e:
	ldr r3, [sp, #92]
	movs r5, #240
	lsls r5, r5, #5
	adds r1, r3, r5
	movs r3, #80
	str r3, [sp, #0]
	movs r3, #104
	str r3, [sp, #4]
	ldr r4, [sp, #104]
	ldr r0, [sp, #88]
	movs r2, #32
	movs r3, #16
	bl sub_080072f4
.L_080e5f6a:
	ldr r3, [sp, #84]
	subs r3, #6
	cmp r3, #1
	bhi .L_080e5f8e
	ldr r0, [sp, #92]
	movs r3, #128
	movs r2, #250
	lsls r2, r2, #6
	str r3, [sp, #0]
	movs r3, #91
	adds r1, r0, r2
	str r3, [sp, #4]
	ldr r4, [sp, #104]
	ldr r0, [sp, #88]
	movs r2, #0
	movs r3, #16
	bl sub_080072f4
.L_080e5f8e:
	ldr r3, [sp, #84]
	subs r3, #8
	cmp r3, #1
	bhi .L_080e5fac
	movs r3, #128
	str r3, [sp, #0]
	movs r3, #91
	str r3, [sp, #4]
	ldr r4, [sp, #104]
	ldr r0, [sp, #88]
	ldr r1, [pc, #692]
	movs r2, #0
	movs r3, #16
	bl sub_080072f4
.L_080e5fac:
	ldr r3, [sp, #84]
	subs r3, #10
	cmp r3, #1
	bhi .L_080e5fca
	movs r3, #128
	str r3, [sp, #0]
	movs r3, #59
	str r3, [sp, #4]
	ldr r1, [pc, #668]
	ldr r4, [sp, #104]
	ldr r0, [sp, #88]
	movs r2, #0
	movs r3, #16
	bl sub_080072f4
.L_080e5fca:
	ldr r3, [sp, #84]
	subs r3, #12
	cmp r3, #1
	bhi .L_080e5fe8
	movs r3, #128
	str r3, [sp, #0]
	movs r3, #29
	str r3, [sp, #4]
	ldr r1, [pc, #644]
	ldr r4, [sp, #104]
	ldr r0, [sp, #88]
	movs r2, #0
	movs r3, #16
	bl sub_080072f4
.L_080e5fe8:
	ldr r3, [sp, #84]
	subs r3, #14
	cmp r3, #1
	bls .L_080e5ff2
	b sub_080e640e
.L_080e5ff2:
	movs r3, #128
	str r3, [sp, #0]
	movs r3, #26
	str r3, [sp, #4]
	ldr r1, [pc, #616]
	ldr r4, [sp, #104]
	ldr r0, [sp, #88]
	movs r2, #0
	movs r3, #16
	bl sub_080072f4
	b sub_080e640e
.L_080e600a:
	ldr r3, [sp, #96]
	cmp r3, #20
	bne .L_080e60ba
	movs r5, #0
	mov r10, r5
.L_080e6014:
	mov r3, r10
	ldr r0, [sp, #84]
	mov r5, r10
	adds r3, #6
	adds r5, #1
	cmp r0, r3
	blt .L_080e60b2
	adds r3, #12
	cmp r0, r3
	bge .L_080e60ae
	mov r1, r10
	subs r3, r0, r1
	subs r3, #6
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r4, r3, #1
	ldr r3, [sp, #60]
	ldr r2, [r3, #0]
	ldr r7, [pc, #556]
	lsrs r3, r2, #31
	adds r2, r2, r3
	ldrb r3, [r7, r4]
	asrs r2, r2, #1
	lsrs r3, r3, #1
	subs r6, r2, r3
	movs r3, #1
	ands r3, r1
	cmp r3, #0
	beq .L_080e605c
	lsrs r3, r5, #31
	adds r3, r5, r3
	asrs r3, r3, #1
	lsls r2, r3, #1
	adds r2, r2, r3
	adds r6, r6, r2
	b .L_080e606c
.L_080e605c:
	mov r5, r10
	adds r5, #1
	lsrs r3, r5, #31
	adds r3, r5, r3
	asrs r3, r3, #1
	lsls r2, r3, #1
	adds r2, r2, r3
	subs r6, r6, r2
.L_080e606c:
	mov r1, r10
	movs r0, #1
	cmp r1, #0
	beq .L_080e6084
	mov r3, r10
	subs r3, #1
	movs r2, #3
	ands r3, r2
	movs r0, #0
	cmp r3, #1
	ble .L_080e6084
	movs r0, #1
.L_080e6084:
	ldr r2, [pc, #484]
	lsls r3, r4, #1
	ldrh r1, [r2, r3]
	ldr r2, [pc, #460]
	ldr r3, [pc, #480]
	adds r1, r1, r2
	ldrb r2, [r7, r4]
	ldrb r3, [r3, r4]
	str r2, [sp, #0]
	ldr r2, [pc, #476]
	ldrb r2, [r2, r4]
	str r2, [sp, #4]
	ldr r2, [sp, #52]
	lsls r0, r0, #2
	ldr r4, [r0, r2]
	adds r3, #48
	ldr r0, [sp, #88]
	adds r2, r6, #0
	bl sub_080072f4
	b .L_080e60b2
.L_080e60ae:
	mov r5, r10
	adds r5, #1
.L_080e60b2:
	mov r10, r5
	cmp r5, #12
	bne .L_080e6014
	b sub_080e640e
.L_080e60ba:
	ldr r3, [sp, #96]
	cmp r3, #16
	beq .L_080e60c2
	b .L_080e61d4
.L_080e60c2:
	ldr r5, [sp, #84]
	cmp r5, #0
	bne .L_080e610c
	movs r0, #0
	ldr r6, [pc, #428]
	ldr r5, [pc, #428]
	mov r10, r0
	movs r7, #0
.L_080e60d2:
	bl sub_08004458
	movs r3, #127
	ands r3, r0
	adds r3, #32
	str r3, [r5, #0]
	str r7, [r5, #4]
	str r7, [r5, #8]
	bl sub_08004458
	ands r0, r6
	str r0, [r5, #12]
	bl sub_08004458
	ands r0, r6
	str r0, [r5, #16]
	bl sub_08004458
	movs r1, #1
	add r10, r1
	ands r0, r6
	mov r2, r10
	str r0, [r5, #20]
	adds r5, #28
	cmp r2, #64
	bne .L_080e60d2
	ldr r2, [pc, #376]
	movs r3, #159
	str r3, [r2, #4]
.L_080e610c:
	movs r5, #112
	ldr r0, [sp, #60]
	movs r3, #0
	add r5, sp
	ldr r7, [pc, #356]
	mov r10, r3
	mov r9, r5
	mov r8, r0
.L_080e611c:
	ldr r3, [r7, #0]
	cmp r3, #0
	blt .L_080e61c6
	mov r1, r10
	lsrs r3, r1, #31
	add r3, r10
	ldr r2, [sp, #84]
	asrs r3, r3, #1
	cmp r2, r3
	blt .L_080e61c6
	movs r5, #3
	ands r5, r1
	bl sub_080049ac
	ldr r0, [r7, #12]
	bl sub_08004bd4
	mov r6, r9
	ldr r0, [r7, #16]
	bl sub_08004c1c
	adds r1, r6, #0
	adds r0, r7, #0
	bl sub_080e3944
	ldr r2, [r6, #0]
	mov r0, r8
	lsrs r3, r2, #31
	adds r2, r2, r3
	ldr r3, [r0, #0]
	lsrs r1, r3, #31
	adds r3, r3, r1
	asrs r3, r3, #1
	asrs r2, r2, #1
	adds r2, r2, r3
	str r2, [r6, #0]
	ldr r3, [r6, #4]
	ldr r1, [r0, #4]
	adds r3, r3, r1
	adds r1, r3, #0
	adds r1, #32
	str r1, [r6, #4]
	ldr r1, [pc, #272]
	movs r0, #8
	lsls r5, r5, #1
	ldrh r1, [r1, r5]
	str r0, [sp, #0]
	str r0, [sp, #4]
	ldr r5, [pc, #216]
	ldr r0, [sp, #52]
	adds r3, #28
	ldr r4, [r0, #4]
	adds r1, r1, r5
	subs r2, #4
	ldr r0, [sp, #88]
	bl sub_080072f4
	ldr r3, [r7, #0]
	subs r3, #6
	str r3, [r7, #0]
	cmp r3, #0
	bge .L_080e61c6
	movs r3, #7
	mov r1, r10
	ands r3, r1
	cmp r3, #0
	beq .L_080e61a6
	cmp r1, #63
	bne .L_080e61c6
.L_080e61a6:
	movs r0, #133
	bl sub_080f9010
	ldr r5, [pc, #164]
	ldr r2, [sp, #92]
	adds r3, r2, r5
	ldr r3, [r3, #0]
	movs r1, #36
	ldrsh r0, [r3, r1]
	movs r3, #4
	str r3, [sp, #0]
	movs r1, #7
	movs r2, #5
	movs r3, #0
	bl sub_080d6888
.L_080e61c6:
	movs r2, #1
	add r10, r2
	mov r3, r10
	adds r7, #28
	cmp r3, #64
	bne .L_080e611c
	b sub_080e640e
.L_080e61d4:
	ldr r5, [sp, #96]
	cmp r5, #8
	bne .L_080e6218
	ldr r3, [sp, #84]
	subs r3, #5
	cmp r3, #44
	bls .L_080e61e4
	b sub_080e640e
.L_080e61e4:
	ldr r0, [sp, #84]
	cmp r0, #25
	ble .L_080e61f2
	lsls r2, r0, #2
	movs r3, #196
	subs r1, r3, r2
	b .L_080e61fa
.L_080e61f2:
	ldr r1, [sp, #84]
	lsls r3, r1, #4
	adds r1, r3, #0
	subs r1, #64
.L_080e61fa:
	cmp r1, #96
	ble .L_080e6200
	movs r1, #96
.L_080e6200:
	movs r2, #32
	movs r3, #104
	subs r3, r3, r1
	str r2, [sp, #0]
	str r1, [sp, #4]
	ldr r4, [sp, #104]
	ldr r0, [sp, #88]
	ldr r1, [pc, #72]
	movs r2, #48
	bl sub_080072f4
	b sub_080e640e
.L_080e6218:
	ldr r3, [sp, #96]
	subs r3, #33
	cmp r3, #1
	bhi sub_080e62cc
	ldr r2, [sp, #84]
	cmp r2, #5
	ble .L_080e6228
	b sub_080e640e
.L_080e6228:
	ldr r5, [sp, #92]
	ldr r0, [pc, #40]
	adds r3, r5, r0
	ldr r3, [r3, #0]
	ldr r3, [r3, #4]
	cmp r3, #0
	bne sub_080e6288
	ldr r2, [sp, #60]
	ldr r1, [r2, #0]
	ldr r5, [sp, #84]
	lsrs r3, r1, #31
	adds r1, r1, r3
	movs r3, #6
	subs r3, r3, r5
	lsls r2, r3, #1
	adds r2, r2, r3
	ldr r0, [sp, #60]
	lsls r3, r2, #1
	asrs r1, r1, #1
	adds r1, r1, r3
	b sub_080e62a2
