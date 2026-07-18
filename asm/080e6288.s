@ 呼出しグラフから到達した領域の再構築サム逆アセンブル。
@ （コードとデータが混在）。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.set sub_080022fc, 0x080022fc
	.set sub_08002dd8, 0x08002dd8
	.set sub_080030f8, 0x080030f8
	.set sub_08004278, 0x08004278
	.set sub_080072f4, 0x080072f4
	.set sub_080072f8, 0x080072f8
	.set sub_080b5088, 0x080b5088
	.set sub_080b50e8, 0x080b50e8
	.set sub_080cd52c, 0x080cd52c
	.set sub_080cdbc0, 0x080cdbc0
	.set sub_080d6888, 0x080d6888
	.set sub_080d9ac4, 0x080d9ac4
	.set sub_080e155c, 0x080e155c
	.set sub_080e38b8, 0x080e38b8
	.set sub_080e3944, 0x080e3944
	.set sub_080e52f8, 0x080e52f8
	.set sub_080f9010, 0x080f9010
	.global Region_080e6288
Region_080e6288:
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
	subs r1, r1, r3
	ldr r3, [r0, #4]
	lsls r2, r2, #2
	subs r3, r3, r2
	adds r3, #24
	adds r2, r1, #0
	movs r1, #32
	str r1, [sp, #0]
	movs r1, #64
	str r1, [sp, #4]
	ldr r1, [sp, #52]
	subs r2, #16
	ldr r4, [r1, #4]
	subs r3, #32
	ldr r0, [sp, #88]
	ldr r1, [pc, #8]
	bl sub_080072f4
	b .L_080e640e
	.2byte 0x0000
	.4byte 0x02010000
	ldr r2, [sp, #96]
	cmp r2, #12
	bne .L_080e6374
	ldr r3, [sp, #84]
	cmp r3, #47
	ble .L_080e62e6
	ldr r5, [sp, #84]
	ldr r2, [pc, #28]
	ldr r1, [pc, #28]
	ldr r3, [pc, #32]
	subs r2, r2, r5
	orrs r2, r1
	strh r2, [r3, #0]
.L_080e62e6:
	movs r0, #0
	movs r1, #3
	movs r2, #1
	ldr r7, [pc, #20]
	mov r10, r0
	mov r9, r1
	add r6, sp, #112
	mov r8, r2
	b .L_080e6308
	.4byte 0x00000040
	.4byte 0x00001000
	.4byte 0x04000052
	.4byte 0x02014000
.L_080e6308:
	mov r0, r10
	movs r1, #3
	bl sub_080022fc
	adds r1, r6, #0
	adds r5, r0, #0
	adds r0, r7, #0
	bl sub_080e3944
	ldr r2, [r6, #0]
	lsrs r3, r2, #31
	adds r2, r2, r3
	asrs r2, r2, #1
	str r2, [r6, #0]
	movs r0, #24
	mov r4, r10
	mov r3, r8
	lsls r1, r5, #3
	ands r4, r3
	adds r1, r1, r5
	ldr r3, [r6, #4]
	ldr r5, [pc, #728]
	str r0, [sp, #0]
	str r0, [sp, #4]
	ldr r0, [sp, #52]
	lsls r4, r4, #2
	lsls r1, r1, #6
	ldr r4, [r4, r0]
	adds r1, r1, r5
	subs r2, #12
	subs r3, #12
	ldr r0, [sp, #88]
	bl sub_080072f4
	mov r3, r10
	mov r1, r9
	ands r3, r1
	adds r3, #11
	mov r2, r8
	lsls r2, r3
	adds r0, r7, #0
	movs r1, #60
	bl sub_080e38b8
	ldr r3, [r7, #24]
	movs r2, #1
	adds r3, #1
	add r10, r2
	str r3, [r7, #24]
	mov r3, r10
	adds r7, #28
	cmp r3, #16
	bne .L_080e6308
	b .L_080e640e
.L_080e6374:
	ldr r5, [sp, #96]
	cmp r5, #100
	beq .L_080e640e
	movs r1, #112
	movs r0, #0
	add r1, sp
	ldr r7, [pc, #652]
	mov r10, r0
	mov r8, r1
.L_080e6386:
	mov r3, r10
	ldr r2, [sp, #84]
	adds r3, #4
	cmp r2, r3
	blt .L_080e6402
	ldr r5, [r7, #24]
	cmp r5, #23
	bgt .L_080e6402
	cmp r5, #0
	bge .L_080e639c
	adds r5, #3
.L_080e639c:
	mov r6, r8
	adds r1, r6, #0
	adds r0, r7, #0
	bl sub_080e3944
	ldr r2, [r6, #0]
	lsrs r3, r2, #31
	adds r2, r2, r3
	asrs r5, r5, #2
	asrs r2, r2, #1
	mov r3, r10
	movs r0, #24
	movs r4, #1
	str r2, [r6, #0]
	ands r4, r3
	lsls r1, r5, #3
	ldr r3, [r6, #4]
	str r0, [sp, #0]
	movs r0, #48
	str r0, [sp, #4]
	adds r1, r1, r5
	ldr r0, [sp, #52]
	ldr r5, [pc, #576]
	lsls r4, r4, #2
	lsls r1, r1, #7
	adds r1, r1, r5
	ldr r4, [r4, r0]
	subs r2, #12
	subs r3, #24
	ldr r0, [sp, #88]
	bl sub_080072f4
	ldr r1, [sp, #96]
	cmp r1, #25
	bne .L_080e63f0
	movs r2, #128
	adds r0, r7, #0
	movs r1, #60
	lsls r2, r2, #3
	bl sub_080e38b8
	b .L_080e63fc
.L_080e63f0:
	movs r2, #128
	adds r0, r7, #0
	movs r1, #60
	lsls r2, r2, #5
	bl sub_080e38b8
.L_080e63fc:
	ldr r3, [r7, #24]
	adds r3, #1
	str r3, [r7, #24]
.L_080e6402:
	movs r2, #1
	add r10, r2
	mov r3, r10
	adds r7, #28
	cmp r3, #16
	bne .L_080e6386
.L_080e640e:
	ldr r5, [sp, #96]
	cmp r5, #7
	bgt .L_080e6464
	ldr r0, [sp, #84]
	cmp r0, #5
	bgt .L_080e6464
	add r5, sp, #112
	adds r1, r5, #0
	ldr r0, [sp, #8]
	bl sub_080e3944
	ldr r2, [r5, #0]
	lsrs r3, r2, #31
	adds r2, r2, r3
	asrs r2, r2, #1
	movs r0, #20
	str r2, [r5, #0]
	ldr r3, [r5, #4]
	str r0, [sp, #0]
	movs r0, #40
	str r0, [sp, #4]
	ldr r5, [sp, #52]
	ldr r1, [pc, #472]
	subs r2, #10
	subs r3, #4
	ldr r4, [r5, #4]
	ldr r0, [sp, #88]
	bl sub_080072f4
	ldr r0, [sp, #8]
	ldr r1, [sp, #24]
	ldr r3, [r0, #0]
	ldr r2, [r1, #0]
	adds r3, r3, r2
	str r3, [r0, #0]
	ldr r3, [r0, #4]
	ldr r2, [r1, #4]
	adds r3, r3, r2
	str r3, [r0, #4]
	ldr r3, [r0, #8]
	ldr r2, [r1, #8]
	adds r3, r3, r2
	str r3, [r0, #8]
.L_080e6464:
	ldr r2, [sp, #84]
	cmp r2, #3
	bne .L_080e6472
	movs r0, #1
	negs r0, r0
	bl sub_080b50e8
.L_080e6472:
	ldr r3, [sp, #84]
	cmp r3, #4
	bne .L_080e647e
	movs r0, #134
	bl sub_080f9010
.L_080e647e:
	ldr r5, [sp, #84]
	cmp r5, #6
	bne .L_080e6530
	ldr r0, [sp, #16]
	cmp r0, #1
	bls .L_080e64a8
	ldr r1, [sp, #96]
	cmp r1, #7
	beq .L_080e64a8
	cmp r1, #13
	beq .L_080e64a8
	cmp r1, #18
	beq .L_080e64a8
	cmp r1, #19
	beq .L_080e64a8
	cmp r1, #23
	beq .L_080e64a8
	cmp r1, #34
	beq .L_080e64a8
	cmp r1, #100
	bne .L_080e64c2
.L_080e64a8:
	ldr r2, [sp, #92]
	ldr r5, [pc, #364]
	adds r3, r2, r5
	ldr r3, [r3, #0]
	movs r1, #36
	ldrsh r0, [r3, r1]
	movs r1, #4
	bl sub_080b5088
	ldr r3, [sp, #92]
	subs r5, #128
	adds r2, r3, r5
	b .L_080e650c
.L_080e64c2:
	ldr r0, [sp, #96]
	cmp r0, #20
	beq .L_080e64d0
	cmp r0, #14
	beq .L_080e64d0
	cmp r0, #33
	bne .L_080e64ec
.L_080e64d0:
	ldr r1, [sp, #92]
	ldr r2, [pc, #324]
	adds r3, r1, r2
	ldr r3, [r3, #0]
	movs r1, #1
	movs r5, #36
	ldrsh r0, [r3, r5]
	bl sub_080b5088
	ldr r1, [pc, #312]
	ldr r0, [sp, #92]
	movs r3, #2
	adds r2, r0, r1
	b .L_080e650e
.L_080e64ec:
	ldr r2, [sp, #96]
	cmp r2, #30
	beq .L_080e64f6
	cmp r2, #8
	bne .L_080e6510
.L_080e64f6:
	ldr r5, [sp, #92]
	ldr r0, [pc, #284]
	adds r3, r5, r0
	ldr r3, [r3, #0]
	movs r1, #36
	ldrsh r0, [r3, r1]
	movs r1, #3
	bl sub_080b5088
	ldr r3, [pc, #272]
	adds r2, r5, r3
.L_080e650c:
	movs r3, #8
.L_080e650e:
	str r3, [r2, #0]
.L_080e6510:
	ldr r5, [sp, #84]
	cmp r5, #6
	bne .L_080e6530
	ldr r0, [sp, #92]
	ldr r1, [pc, #252]
	adds r3, r0, r1
	ldr r3, [r3, #0]
	movs r2, #36
	ldrsh r0, [r3, r2]
	movs r3, #4
	str r3, [sp, #0]
	movs r1, #7
	movs r2, #5
	movs r3, #0
	bl sub_080d6888
.L_080e6530:
	ldr r3, [sp, #84]
	cmp r3, #14
	bne .L_080e6550
	ldr r0, [pc, #224]
	ldr r5, [sp, #92]
	adds r3, r5, r0
	ldr r3, [r3, #0]
	movs r1, #36
	ldrsh r0, [r3, r1]
	movs r3, #4
	str r3, [sp, #0]
	movs r1, #7
	movs r2, #5
	movs r3, #0
	bl sub_080d6888
.L_080e6550:
	movs r1, #8
	movs r0, #8
	bl sub_080e155c
	bl sub_080cd52c
	ldr r5, [pc, #192]
	ldr r3, [sp, #92]
	adds r2, r3, r5
	movs r3, #1
	movs r0, #1
	str r3, [r2, #0]
	bl sub_080030f8
	ldr r0, [sp, #84]
	ldr r1, [sp, #64]
	adds r0, #1
	str r0, [sp, #84]
	cmp r0, r1
	beq .L_080e657c
	bl sub_080e52f8
.L_080e657c:
	ldr r2, [sp, #96]
	cmp r2, #21
	bne .L_080e65c4
	movs r1, #128
	ldr r5, [pc, #156]
	lsls r1, r1, #7
	ldr r0, [pc, #156]
	bl sub_080072f8
	movs r1, #128
	ldr r0, [sp, #88]
	lsls r1, r1, #7
	bl sub_080072f8
	ldr r0, [pc, #124]
	ldr r5, [sp, #92]
	adds r3, r5, r0
	ldr r2, [r3, #0]
	movs r3, #0
	str r3, [r2, #28]
	ldr r0, [pc, #132]
	bl sub_08004278
	ldr r0, [pc, #132]
	bl sub_08004278
	movs r0, #47
	bl sub_08002dd8
	movs r0, #46
	bl sub_08002dd8
	ldr r0, [sp, #100]
	bl sub_080d9ac4
	b .L_080e65f8
.L_080e65c4:
	ldr r1, [sp, #20]
	cmp r1, #1
	bls .L_080e65dc
	ldr r2, [sp, #96]
	cmp r2, #12
	beq .L_080e65dc
	cmp r2, #22
	beq .L_080e65dc
	cmp r2, #28
	beq .L_080e65dc
	cmp r2, #29
	bne .L_080e65e2
.L_080e65dc:
	ldr r0, [pc, #84]
	bl sub_08004278
.L_080e65e2:
	ldr r0, [pc, #76]
	bl sub_08004278
	movs r0, #47
	bl sub_08002dd8
	movs r0, #46
	bl sub_08002dd8
	bl sub_080cdbc0
.L_080e65f8:
	add sp, #184
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
