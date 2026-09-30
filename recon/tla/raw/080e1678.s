.syntax unified
	.thumb
	.global Func_080e1678
	.thumb_func
Func_080e1678:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r1, #192
	lsls r1, r1, #18
	adds r3, r1, #0
	adds r3, #224
	ldr r3, [r3]
	sub sp, #72
	mov r11, r3
	adds r3, r1, #0
	adds r3, #240
	ldr r3, [r3]
	ldr r0, [r1, #92]
	str r3, [sp, #8]
	mov r8, r0
	ldr r1, [r1, #108]
	mov r9, r1
	add r1, sp, #60
	mov r10, r1
.L_080e16a8:
	movs r3, #128
	lsls r3, r3, #3
	adds r3, #36
	add r3, r8
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #5
	bls .L_080e16ba
	b .L_080e1b1a
.L_080e16ba:
	ldr r2, .L_080e19e0
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
	.2byte 0x0000
.L_080e16c4:
	.4byte .L_080e170e
	.4byte .L_080e1770
	.4byte .L_080e1838
	.4byte .L_080e1966
	.4byte .L_080e19e4
	.4byte .L_080e1af8
.L_080e16dc:
	movs r3, #128
	lsls r3, r3, #3
	adds r3, #50
	add r3, r8
	strb r6, [r3]
	movs r3, #134
	lsls r3, r3, #3
	add r3, r8
	movs r2, #128
	strh r4, [r3]
	lsls r2, r2, #3
	adds r2, #36
	add r2, r8
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	movs r2, #128
	lsls r2, r2, #3
	movs r3, #255
	adds r2, #38
	lsls r3, r3, #8
	add r2, r8
	adds r3, #255
	strh r3, [r2]
	b .L_080e16a8
.L_080e170e:
	movs r0, #128
	lsls r0, r0, #3
	adds r0, #38
	add r0, r8
	movs r3, #0
	ldrsh r1, [r0, r3]
	cmp r1, #0
	bne .L_080e1752
	movs r3, #128
	lsls r3, r3, #3
	adds r3, #46
	add r3, r8
	strb r1, [r3]
	movs r3, #204
	lsls r3, r3, #2
	adds r3, #255
	movs r2, #1
	add r3, r8
	strb r2, [r3]
	movs r3, #205
	lsls r3, r3, #2
	adds r3, #255
	add r3, r8
	strb r1, [r3]
	movs r3, #128
	lsls r3, r3, #3
	adds r3, #44
	add r3, r8
	strb r2, [r3]
	movs r2, #128
	lsls r2, r2, #3
	movs r3, #186
	adds r2, #42
	b .L_080e1b12
.L_080e1752:
	movs r3, #128
	lsls r3, r3, #3
	adds r3, #44
	add r3, r8
	movs r2, #0
	ldrsb r2, [r3, r2]
	cmp r2, #0
	beq .L_080e1764
	b .L_080e1b2c
.L_080e1764:
	movs r3, #128
	lsls r3, r3, #3
	adds r3, #42
	add r3, r8
	strh r2, [r3]
	b .L_080e1aa6
.L_080e1770:
	movs r6, #128
	lsls r6, r6, #3
	adds r6, #38
	add r6, r8
	movs r4, #0
	ldrsh r1, [r6, r4]
	ldrh r2, [r6]
	cmp r1, #0
	bne .L_080e17d0
	movs r5, #232
	lsls r5, r5, #2
	movs r3, #128
	mov r0, r10
	lsls r3, r3, #9
	add r5, r8
	str r3, [r5, #24]
	str r3, [r5, #20]
	str r1, [r0, #8]
	str r1, [r0, #4]
	str r1, [r0]
	mov r2, r11
	ldrh r1, [r2, #2]
	movs r0, #152
	movs r3, #128
	lsls r3, r3, #8
	lsls r0, r0, #6
	adds r1, r1, r3
	mov r2, r10
	adds r0, #102
	bl Func_0801489c
	mov r4, r10
	ldr r3, [r4]
	movs r7, #225
	lsls r7, r7, #2
	add r7, r8
	str r3, [r7, #12]
	movs r2, #205
	ldr r3, [r4, #4]
	lsls r2, r2, #2
	str r3, [r7, #16]
	adds r2, #255
	ldr r3, [r4, #8]
	add r2, r8
	str r3, [r7, #20]
	movs r3, #1
	strb r3, [r2]
	ldrh r2, [r6]
.L_080e17d0:
	movs r7, #160
	lsls r3, r2, #16
	lsls r7, r7, #13
	cmp r3, r7
	bne .L_080e17e8
	movs r2, #128
	lsls r2, r2, #3
	adds r2, #46
	add r2, r8
	movs r3, #1
	strb r3, [r2]
	ldrh r2, [r6]
.L_080e17e8:
	movs r0, #224
	lsls r3, r2, #16
	lsls r0, r0, #14
	cmp r3, r0
	beq .L_080e17f4
	b .L_080e1b1a
.L_080e17f4:
	movs r3, #0
	mov r1, r10
	str r3, [r1, #8]
	str r3, [r1, #4]
	str r3, [r1]
	mov r2, r11
	movs r0, #160
	ldrh r1, [r2, #2]
	lsls r0, r0, #11
	mov r2, r10
	bl Func_0801489c
	mov r4, r10
	ldr r3, [r4]
	movs r7, #225
	lsls r7, r7, #2
	add r7, r8
	str r3, [r7, #12]
	movs r2, #128
	ldr r3, [r4, #4]
	lsls r2, r2, #3
	str r3, [r7, #16]
	adds r2, #36
	ldr r3, [r4, #8]
	add r2, r8
	str r3, [r7, #20]
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	strh r3, [r6]
	b .L_080e1b1a
.L_080e1838:
	movs r5, #128
	lsls r5, r5, #3
	adds r5, #38
	add r5, r8
	movs r6, #0
	ldrsh r3, [r5, r6]
	movs r7, #225
	lsls r7, r7, #2
	add r7, r8
	cmp r3, #90
	bne .L_080e1866
	movs r3, #128
	lsls r3, r3, #3
	adds r3, #36
	add r3, r8
	ldrh r2, [r3]
	adds r2, #1
	strh r2, [r3]
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	strh r3, [r5]
	b .L_080e1b1a
.L_080e1866:
	movs r2, #128
	movs r3, #131
	lsls r2, r2, #3
	lsls r3, r3, #3
	adds r2, #12
	add r3, r8
	add r2, r8
	ldr r3, [r3]
	ldr r2, [r2]
	subs r1, r3, r2
	cmp r1, #0
	bge .L_080e1880
	subs r1, r2, r3
.L_080e1880:
	ldr r4, [r7]
	subs r3, r4, r2
	cmp r3, #0
	bge .L_080e188a
	subs r3, r2, r4
.L_080e188a:
	cmp r1, r3
	blt .L_080e18b6
	movs r2, #128
	movs r3, #132
	lsls r2, r2, #3
	lsls r3, r3, #3
	adds r2, #20
	add r3, r8
	add r2, r8
	ldr r3, [r3]
	ldr r2, [r2]
	subs r1, r3, r2
	cmp r1, #0
	bge .L_080e18a8
	subs r1, r2, r3
.L_080e18a8:
	ldr r0, [r7, #8]
	subs r3, r0, r2
	cmp r3, #0
	bge .L_080e18b2
	subs r3, r2, r0
.L_080e18b2:
	cmp r1, r3
	bge .L_080e18ce
.L_080e18b6:
	movs r3, #128
	lsls r3, r3, #3
	adds r3, #36
	add r3, r8
	ldrh r2, [r3]
	adds r2, #1
	strh r2, [r3]
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	strh r3, [r5]
	b .L_080e16a8
.L_080e18ce:
	mov r0, r10
	str r4, [r0]
	ldr r3, [r7, #4]
	mov r1, r11
	str r3, [r0, #4]
	ldr r3, [r7, #8]
	str r3, [r0, #8]
	ldr r0, [r1, #16]
	mov r1, r10
	bl Func_08020210
	cmp r0, #0
	ble .L_080e1918
	movs r2, #128
	lsls r2, r2, #3
	adds r2, #50
	add r2, r8
	movs r3, #1
	strb r3, [r2]
	movs r2, #134
	movs r3, #255
	lsls r2, r2, #3
	lsls r3, r3, #8
	add r2, r8
	adds r3, #255
	strh r3, [r2]
	movs r2, #128
	lsls r2, r2, #3
	adds r2, #36
	add r2, r8
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	movs r3, #1
	negs r3, r3
	strh r3, [r5]
	b .L_080e16a8
.L_080e1918:
	mov r5, r9
	movs r4, #0
	adds r5, #20
.L_080e191e:
	ldr r1, [r5]
	cmp r1, #0
	beq .L_080e195c
	ldr r3, [r1]
	cmp r3, #0
	beq .L_080e195c
	adds r3, r1, #0
	adds r3, #89
	ldrb r2, [r3]
	movs r6, #1
	adds r3, r6, #0
	ands r3, r2
	cmp r3, #0
	beq .L_080e195c
	mov r2, r11
	ldr r3, [r2, #16]
	cmp r1, r3
	beq .L_080e195c
	ldrh r3, [r1, #32]
	adds r2, r1, #0
	adds r2, #8
	subs r3, #2
	adds r0, r7, #0
	movs r1, #2
	str r4, [sp, #4]
	bl Func_080dbe80
	ldr r4, [sp, #4]
	cmp r0, #0
	blt .L_080e195c
	b .L_080e16dc
.L_080e195c:
	adds r4, #1
	adds r5, #4
	cmp r4, #79
	ble .L_080e191e
	b .L_080e1b1a
.L_080e1966:
	movs r3, #128
	lsls r3, r3, #3
	adds r3, #50
	add r3, r8
	movs r2, #0
	ldrsb r2, [r3, r2]
	cmp r2, #0
	beq .L_080e1982
	movs r2, #205
	lsls r2, r2, #2
	adds r2, #255
	add r2, r8
	movs r3, #0
	b .L_080e1996
.L_080e1982:
	movs r3, #204
	lsls r3, r3, #2
	adds r3, #255
	add r3, r8
	strb r2, [r3]
	movs r2, #128
	lsls r2, r2, #3
	adds r2, #45
	add r2, r8
	movs r3, #1
.L_080e1996:
	strb r3, [r2]
	movs r3, #0
	mov r4, r10
	str r3, [r4, #8]
	str r3, [r4, #4]
	str r3, [r4]
	movs r0, #128
	mov r6, r11
	lsls r0, r0, #10
	mov r2, r10
	ldrh r1, [r6, #2]
	bl Func_0801489c
	mov r0, r10
	ldr r3, [r0]
	movs r7, #225
	lsls r7, r7, #2
	add r7, r8
	str r3, [r7, #12]
	movs r2, #128
	ldr r3, [r0, #4]
	lsls r2, r2, #3
	str r3, [r7, #16]
	adds r2, #36
	ldr r3, [r0, #8]
	add r2, r8
	str r3, [r7, #20]
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	movs r2, #128
	lsls r2, r2, #3
	movs r3, #255
	adds r2, #38
	lsls r3, r3, #8
	b .L_080e1b14
	.2byte 0x0000
.L_080e19e0:
	.4byte .L_080e16c4
.L_080e19e4:
	movs r3, #128
	lsls r3, r3, #3
	adds r3, #50
	add r3, r8
	movs r2, #0
	ldrsb r2, [r3, r2]
	movs r5, #232
	lsls r5, r5, #2
	add r5, r8
	cmp r2, #0
	beq .L_080e1abe
	movs r0, #128
	lsls r0, r0, #3
	adds r0, #38
	add r0, r8
	movs r1, #0
	ldrsh r3, [r0, r1]
	cmp r3, #31
	bgt .L_080e1a60
	ldr r3, .L_080e1aec
	add r2, sp, #28
	mov r12, r2
	ldmia r3!, {r4, r6, r7}
	stmia r2!, {r4, r6, r7}
	ldmia r3!, {r1, r4, r6}
	stmia r2!, {r1, r4, r6}
	ldmia r3!, {r1, r7}
	stmia r2!, {r1, r7}
	ldr r3, .L_080e1af0
	add r4, sp, #12
	adds r2, r4, #0
	ldmia r3!, {r1, r6, r7}
	stmia r2!, {r1, r6, r7}
	ldr r3, [r3]
	mov r6, r12
	str r3, [r2]
	movs r3, #7
	ldrh r2, [r0]
	ands r3, r2
	lsls r2, r2, #16
	lsls r3, r3, #2
	asrs r2, r2, #16
	ldr r1, [r6, r3]
	cmp r2, #0
	bge .L_080e1a40
	adds r2, #7
.L_080e1a40:
	asrs r3, r2, #3
	lsls r3, r3, #2
	ldr r3, [r4, r3]
	adds r2, r3, #0
	muls r2, r1
	cmp r2, #0
	bge .L_080e1a56
	movs r7, #255
	lsls r7, r7, #8
	adds r7, #255
	adds r2, r2, r7
.L_080e1a56:
	mov r0, r11
	ldrh r3, [r0, #2]
	asrs r2, r2, #16
	adds r3, r3, r2
	strh r3, [r5, #28]
.L_080e1a60:
	movs r0, #128
	lsls r0, r0, #3
	adds r0, #38
	add r0, r8
	movs r1, #0
	ldrsh r3, [r0, r1]
	ldrh r2, [r0]
	cmp r3, #32
	bne .L_080e1a80
	movs r2, #128
	lsls r2, r2, #3
	adds r2, #45
	add r2, r8
	movs r3, #1
	strb r3, [r2]
	ldrh r2, [r0]
.L_080e1a80:
	lsls r3, r2, #16
	movs r2, #200
	lsls r2, r2, #14
	cmp r3, r2
	bne .L_080e1b1a
	movs r1, #205
	lsls r1, r1, #2
	adds r1, #255
	movs r2, #0
	movs r3, #1
	add r1, r8
	str r2, [r5, #24]
	str r2, [r5, #20]
	strb r3, [r1]
	movs r3, #204
	lsls r3, r3, #2
	adds r3, #255
	add r3, r8
	strb r2, [r3]
.L_080e1aa6:
	movs r2, #128
	lsls r2, r2, #3
	adds r2, #36
	add r2, r8
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	strh r3, [r0]
	b .L_080e1b1a
.L_080e1abe:
	ldr r3, [r5, #20]
	ldr r4, .L_080e1af4
	adds r3, r3, r4
	str r3, [r5, #20]
	str r3, [r5, #24]
	cmp r3, #0
	bge .L_080e1b1a
	str r2, [r5, #24]
	str r2, [r5, #20]
	movs r2, #128
	lsls r2, r2, #3
	adds r2, #36
	add r2, r8
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	movs r2, #128
	lsls r2, r2, #3
	movs r3, #255
	adds r2, #38
	lsls r3, r3, #8
	b .L_080e1b14
	.2byte 0x0000
.L_080e1aec:
	.4byte Data_080f0f34
.L_080e1af0:
	.4byte Data_080f0f54
.L_080e1af4:
	.4byte 0xffffd000
.L_080e1af8:
	movs r3, #128
	lsls r3, r3, #3
	adds r3, #45
	add r3, r8
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	bne .L_080e1b1a
	movs r2, #128
	lsls r2, r2, #3
	movs r3, #186
	adds r2, #36
.L_080e1b12:
	lsls r3, r3, #2
.L_080e1b14:
	add r2, r8
	adds r3, #255
	strh r3, [r2]
.L_080e1b1a:
	movs r3, #128
	lsls r3, r3, #3
	adds r3, #44
	add r3, r8
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	beq .L_080e1b6a
.L_080e1b2c:
	movs r5, #249
	lsls r5, r5, #2
	add r5, r8
	ldr r2, [r5, #20]
	ldrh r3, [r5, #28]
	movs r6, #128
	movs r7, #128
	lsls r6, r6, #3
	lsls r7, r7, #6
	movs r0, #128
	adds r2, r2, r6
	adds r3, r3, r7
	lsls r0, r0, #9
	str r2, [r5, #20]
	str r2, [r5, #24]
	strh r3, [r5, #28]
	cmp r2, r0
	blt .L_080e1b6a
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r5, #24]
	str r3, [r5, #20]
	mov r1, r11
	ldrh r3, [r1, #2]
	movs r2, #128
	strh r3, [r5, #28]
	lsls r2, r2, #3
	ldr r3, .L_080e1b98
	adds r2, #44
	add r2, r8
	strb r3, [r2]
.L_080e1b6a:
	movs r0, #128
	lsls r0, r0, #3
	adds r0, #45
	add r0, r8
	movs r3, #0
	ldrsb r3, [r0, r3]
	cmp r3, #0
	beq .L_080e1bb8
	movs r5, #249
	lsls r5, r5, #2
	add r5, r8
	ldr r2, [r5, #20]
	ldr r3, .L_080e1b9c
	movs r4, #128
	adds r2, r2, r3
	ldrh r3, [r5, #28]
	movs r6, #204
	lsls r4, r4, #6
	lsls r6, r6, #7
	adds r3, r3, r4
	adds r6, #101
	str r2, [r5, #20]
	b .L_080e1ba0
.L_080e1b98:
	.4byte 0x00000000
.L_080e1b9c:
	.4byte 0xfffffc00
.L_080e1ba0:
	str r2, [r5, #24]
	movs r1, #0
	strh r3, [r5, #28]
	cmp r2, r6
	bgt .L_080e1bb8
	ldr r3, .L_080e1bb4
	str r1, [r5, #24]
	str r1, [r5, #20]
	strb r3, [r0]
	b .L_080e1bb8
.L_080e1bb4:
	.4byte 0x00000000
.L_080e1bb8:
	movs r2, #128
	lsls r2, r2, #3
	adds r2, #42
	add r2, r8
	movs r0, #186
	movs r7, #0
	ldrsh r3, [r2, r7]
	lsls r0, r0, #2
	adds r0, #255
	ldrh r1, [r2]
	cmp r3, r0
	bne .L_080e1bd2
	b .L_080e1d7c
.L_080e1bd2:
	adds r3, r1, #1
	strh r3, [r2]
	lsls r3, r3, #16
	movs r5, #249
	asrs r3, r3, #16
	lsls r5, r5, #2
	subs r3, #10
	add r5, r8
	cmp r3, #62
	bls .L_080e1be8
	b .L_080e1d7c
.L_080e1be8:
	ldr r2, .L_080e1d1c
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
.L_080e1bf0:
	.4byte .L_080e1cec
	.4byte .L_080e1d7c
	.4byte .L_080e1d7c
	.4byte .L_080e1d7c
	.4byte .L_080e1d7c
	.4byte .L_080e1d7c
	.4byte .L_080e1d7c
	.4byte .L_080e1d7c
	.4byte .L_080e1d7c
	.4byte .L_080e1d7c
	.4byte .L_080e1cfa
	.4byte .L_080e1d7c
	.4byte .L_080e1d7c
	.4byte .L_080e1d7c
	.4byte .L_080e1d7c
	.4byte .L_080e1d7c
	.4byte .L_080e1d7c
	.4byte .L_080e1d7c
	.4byte .L_080e1d7c
	.4byte .L_080e1d7c
	.4byte .L_080e1d7c
	.4byte .L_080e1d7c
	.4byte .L_080e1d7c
	.4byte .L_080e1d7c
	.4byte .L_080e1d7c
	.4byte .L_080e1d7c
	.4byte .L_080e1d7c
	.4byte .L_080e1d7c
	.4byte .L_080e1d7c
	.4byte .L_080e1d7c
	.4byte .L_080e1d7c
	.4byte .L_080e1d7c
	.4byte .L_080e1d7c
	.4byte .L_080e1d7c
	.4byte .L_080e1d7c
	.4byte .L_080e1d7c
	.4byte .L_080e1d7c
	.4byte .L_080e1d7c
	.4byte .L_080e1d7c
	.4byte .L_080e1d7c
	.4byte .L_080e1d7c
	.4byte .L_080e1d7c
	.4byte .L_080e1d7c
	.4byte .L_080e1d7c
	.4byte .L_080e1d08
	.4byte .L_080e1d7c
	.4byte .L_080e1d7c
	.4byte .L_080e1d7c
	.4byte .L_080e1d50
	.4byte .L_080e1d7c
	.4byte .L_080e1d7c
	.4byte .L_080e1d7c
	.4byte .L_080e1d20
	.4byte .L_080e1d7c
	.4byte .L_080e1d7c
	.4byte .L_080e1d2e
	.4byte .L_080e1d7c
	.4byte .L_080e1d7c
	.4byte .L_080e1d3c
	.4byte .L_080e1d7c
	.4byte .L_080e1d7c
	.4byte .L_080e1d50
	.4byte .L_080e1d6c
.L_080e1cec:
	movs r3, #133
	lsls r3, r3, #3
	add r3, r8
	ldrh r2, [r3]
	ldr r3, .L_080e1d18
	adds r2, #32
	b .L_080e1d5e
.L_080e1cfa:
	movs r3, #133
	lsls r3, r3, #3
	add r3, r8
	ldrh r2, [r3]
	ldr r3, .L_080e1d18
	adds r2, #64
	b .L_080e1d5e
.L_080e1d08:
	movs r3, #133
	lsls r3, r3, #3
	add r3, r8
	ldrh r2, [r3]
	ldr r3, .L_080e1d18
	adds r2, #32
	b .L_080e1d5e
	.2byte 0x0000
.L_080e1d18:
	.4byte 0x000003ff
.L_080e1d1c:
	.4byte .L_080e1bf0
.L_080e1d20:
	movs r3, #133
	lsls r3, r3, #3
	add r3, r8
	ldrh r2, [r3]
	ldr r3, .L_080e1d4c
	adds r2, #96
	b .L_080e1d5e
.L_080e1d2e:
	movs r3, #133
	lsls r3, r3, #3
	add r3, r8
	ldrh r2, [r3]
	ldr r3, .L_080e1d4c
	adds r2, #128
	b .L_080e1d5e
.L_080e1d3c:
	movs r3, #133
	lsls r3, r3, #3
	add r3, r8
	ldrh r2, [r3]
	ldr r3, .L_080e1d4c
	adds r2, #160
	b .L_080e1d5e
	.2byte 0x0000
.L_080e1d4c:
	.4byte 0x000003ff
.L_080e1d50:
	movs r3, #133
	lsls r3, r3, #3
	add r3, r8
	ldrh r3, [r3]
	movs r2, #192
	lsls r2, r2, #2
	adds r2, #255
.L_080e1d5e:
	ands r2, r3
	ldrh r1, [r5, #8]
	ldr r3, .L_080e1f18
	ands r3, r1
	orrs r3, r2
	strh r3, [r5, #8]
	b .L_080e1d7c
.L_080e1d6c:
	movs r2, #128
	lsls r2, r2, #3
	movs r3, #186
	adds r2, #42
	lsls r3, r3, #2
	add r2, r8
	adds r3, #255
	strh r3, [r2]
.L_080e1d7c:
	movs r7, #242
	lsls r7, r7, #2
	add r7, r8
	ldr r3, [r7]
	mov r1, r10
	str r3, [r1]
	ldr r3, [r7, #4]
	mov r0, r10
	str r3, [r1, #4]
	ldr r3, [r7, #8]
	movs r5, #249
	str r3, [r1, #8]
	bl Func_080dc390
	mov r2, r10
	ldr r3, [r2]
	lsls r5, r5, #2
	add r5, r8
	str r3, [r5, #12]
	movs r7, #225
	ldr r3, [r2, #8]
	lsls r7, r7, #2
	str r3, [r5, #16]
	adds r0, r5, #0
	add r7, r8
	bl Func_080eb01c
	ldr r3, [r7]
	mov r4, r10
	str r3, [r4]
	ldr r3, [r7, #4]
	mov r0, r10
	str r3, [r4, #4]
	ldr r3, [r7, #8]
	mov r6, r10
	str r3, [r4, #8]
	bl Func_080dc390
	movs r5, #232
	ldr r3, [r6]
	lsls r5, r5, #2
	add r5, r8
	str r3, [r5, #12]
	adds r0, r5, #0
	ldr r3, [r6, #8]
	str r3, [r5, #16]
	bl Func_080eb01c
	movs r3, #205
	lsls r3, r3, #2
	adds r3, #255
	add r3, r8
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	beq .L_080e1df8
	adds r0, r7, #0
	movs r1, #64
	movs r2, #0
	bl BattleFx_IntegrateVector3
.L_080e1df8:
	movs r3, #128
	lsls r3, r3, #3
	adds r3, #46
	add r3, r8
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	beq .L_080e1eb8
	mov r7, r8
	movs r0, #9
	adds r7, #4
	mov r9, r0
.L_080e1e12:
	ldr r0, [r7, #24]
	cmp r0, #0
	bne .L_080e1e62
	bl Random16
	movs r6, #255
	movs r1, #128
	lsls r1, r1, #1
	ands r6, r0
	adds r6, r6, r1
	bl Random16
	movs r5, #248
	lsls r5, r5, #5
	movs r3, #240
	lsls r3, r3, #14
	adds r5, #255
	movs r2, #224
	lsls r2, r2, #7
	ands r5, r0
	str r3, [r7]
	movs r3, #224
	adds r5, r5, r2
	lsls r3, r3, #13
	str r3, [r7, #4]
	adds r0, r5, #0
	bl Trig_Cos
	adds r3, r6, #0
	muls r3, r0
	asrs r3, r3, #6
	str r3, [r7, #12]
	adds r0, r5, #0
	bl Trig_Sin
	adds r3, r6, #0
	muls r3, r0
	asrs r3, r3, #6
	str r3, [r7, #16]
	ldr r0, [r7, #24]
.L_080e1e62:
	cmp r0, #0
	ble .L_080e1e8c
	movs r3, #2
	ldrsh r1, [r7, r3]
	movs r3, #1
	ands r0, r3
	ldr r3, .L_080e1f1c
	lsls r0, r0, #6
	movs r4, #6
	ldrsh r2, [r7, r4]
	adds r0, r0, r3
	movs r3, #8
	str r3, [sp, #0]
	bl Func_080eb4a0
	adds r0, r7, #0
	movs r1, #60
	movs r2, #0
	bl BattleFx_IntegrateVector2
	ldr r0, [r7, #24]
.L_080e1e8c:
	adds r0, #1
	movs r3, #204
	str r0, [r7, #24]
	lsls r3, r3, #2
	adds r3, #255
	add r3, r8
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #1
	bne .L_080e1eaa
	cmp r0, #10
	bne .L_080e1eaa
	movs r3, #0
	str r3, [r7, #24]
.L_080e1eaa:
	movs r6, #1
	negs r6, r6
	add r9, r6
	mov r0, r9
	adds r7, #28
	cmp r0, #0
	bge .L_080e1e12
.L_080e1eb8:
	movs r7, #225
	lsls r7, r7, #2
	add r7, r8
	ldr r3, [r7]
	mov r1, r10
	str r3, [r1]
	ldr r3, [r7, #4]
	mov r2, r11
	str r3, [r1, #4]
	ldr r3, [r7, #8]
	ldr r0, .L_080e1f20
	str r3, [r1, #8]
	mov r6, r10
	ldrh r1, [r2, #2]
	mov r2, r10
	bl Func_0801489c
	mov r0, r10
	bl Func_080dc390
	ldr r2, [sp, #8]
	movs r4, #2
	ldrsh r3, [r6, r4]
	adds r2, #168
	str r3, [r2]
	ldr r2, [sp, #8]
	movs r7, #10
	ldrsh r3, [r6, r7]
	adds r2, #172
	str r3, [r2]
	ldr r2, [sp, #8]
	movs r3, #5
	adds r2, #193
	strb r3, [r2]
	movs r2, #128
	lsls r2, r2, #3
	adds r2, #38
	add r2, r8
	ldrh r3, [r2]
	add sp, #72
	adds r3, #1
	strh r3, [r2]
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_080e1f18:
	.4byte 0xfffffc00
.L_080e1f1c:
	.4byte Data_080eda0c
.L_080e1f20:
	.4byte 0xffdc0000
