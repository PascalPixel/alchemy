.syntax unified
	.thumb
	.global Func_0804a85c
	.thumb_func
Func_0804a85c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #324
	str r2, [sp, #84]
	str r3, [sp, #80]
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #36]
	mov r10, r0
	mov r8, r1
	movs r0, #0
	movs r1, #255
	str r0, [sp, #64]
	lsls r1, r1, #8
	movs r0, #128
	adds r1, #255
	lsls r0, r0, #1
	str r3, [sp, #76]
	str r1, [sp, #56]
	bl Resource_LoadIntoFreeSlot
	ldr r3, [sp, #84]
	movs r2, #0
	str r0, [sp, #52]
	mov r9, r2
	cmp r3, #0
	bne .L_0804a8a0
	movs r4, #1
	str r4, [sp, #84]
.L_0804a8a0:
	mov r5, r8
	cmp r5, #2
	beq .L_0804a8aa
	cmp r5, #4
	bne .L_0804a8b8
.L_0804a8aa:
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #228
	ldr r2, [r3]
	movs r3, #2
	negs r3, r3
	b .L_0804a8c2
.L_0804a8b8:
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #228
	ldr r2, [r3]
	movs r3, #16
.L_0804a8c2:
	str r3, [r2, #40]
	mov r6, sp
	adds r6, #212
	str r6, [sp, #28]
	mov r3, sp
	movs r2, #0
	movs r7, #5
	adds r3, #234
.L_0804a8d2:
	subs r7, #1
	strb r2, [r3]
	subs r3, #4
	cmp r7, #0
	bge .L_0804a8d2
	movs r7, #1
	negs r7, r7
	mov r0, r8
	str r7, [sp, #68]
	cmp r0, #2
	bne .L_0804a926
	ldr r1, [sp, #76]
	movs r3, #88
	ldrsh r3, [r1, r3]
	movs r7, #0
	cmp r3, #255
	beq .L_0804a97c
	movs r3, #154
	lsls r3, r3, #1
	add r3, sp
	str r3, [sp, #20]
	ldr r4, [sp, #64]
	ldr r5, [sp, #20]
	adds r2, r1, #0
	lsls r3, r4, #1
	adds r2, #88
	adds r1, r3, r5
.L_0804a908:
	ldrh r3, [r2]
	adds r7, #1
	strh r3, [r1]
	ldr r6, [sp, #64]
	adds r1, #2
	adds r6, #1
	str r6, [sp, #64]
	adds r2, #2
	cmp r7, #5
	bgt .L_0804a996
	movs r0, #0
	ldrsh r3, [r2, r0]
	cmp r3, #255
	bne .L_0804a908
	b .L_0804a996
.L_0804a926:
	mov r1, r8
	cmp r1, #4
	bne .L_0804a940
	movs r2, #154
	lsls r2, r2, #1
	add r2, sp
	mov r3, r10
	adds r4, r2, #0
	movs r5, #1
	str r2, [sp, #20]
	strh r3, [r4]
	str r5, [sp, #64]
	b .L_0804a996
.L_0804a940:
	ldr r1, [sp, #76]
	movs r3, #100
	adds r1, #2
	ldrsh r3, [r1, r3]
	movs r7, #0
	cmp r3, #255
	beq .L_0804a98e
	movs r0, #154
	lsls r0, r0, #1
	add r0, sp
	str r0, [sp, #20]
	ldr r2, [sp, #64]
	ldr r4, [sp, #20]
	lsls r3, r2, #1
	movs r0, #100
	adds r2, r3, r4
.L_0804a960:
	ldrh r3, [r1, r0]
	adds r7, #1
	strh r3, [r2]
	ldr r5, [sp, #64]
	adds r2, #2
	adds r5, #1
	str r5, [sp, #64]
	adds r0, #2
	cmp r7, #5
	bgt .L_0804a996
	ldrsh r3, [r1, r0]
	cmp r3, #255
	bne .L_0804a960
	b .L_0804a996
.L_0804a97c:
	movs r7, #154
	lsls r7, r7, #1
	add r7, sp
	str r7, [sp, #20]
	b .L_0804a996
.L_0804a986:
	ldr r0, [sp, #20]
	ldrh r5, [r0, r5]
	mov r10, r5
	b .L_0804aaca
.L_0804a98e:
	movs r1, #154
	lsls r1, r1, #1
	add r1, sp
	str r1, [sp, #20]
.L_0804a996:
	ldr r3, [sp, #64]
	ldr r4, [sp, #20]
	lsls r2, r3, #1
	ldr r3, .L_0804a9c8
	mov r6, r8
	strh r3, [r4, r2]
	ldr r5, [sp, #64]
	str r5, [sp, #60]
	cmp r6, #2
	beq .L_0804a9ac
	b .L_0804aaf4
.L_0804a9ac:
	ldr r7, [sp, #84]
	cmp r7, #255
	bne .L_0804a9b4
	b .L_0804aaca
.L_0804a9b4:
	ldr r0, [sp, #80]
	cmp r0, #0
	bne .L_0804a9bc
	b .L_0804aaca
.L_0804a9bc:
	movs r6, #0
	movs r7, #0
	cmp r6, r5
	blt .L_0804a9c6
	b .L_0804aaca
.L_0804a9c6:
	b .L_0804a9cc
.L_0804a9c8:
	.4byte 0x000000ff
.L_0804a9cc:
	ldr r1, [sp, #20]
	lsls r5, r6, #1
	ldrh r3, [r1, r5]
	cmp r3, #254
	beq .L_0804aac0
	adds r0, r3, #0
	bl Owner_GetState
	ldr r3, [sp, #80]
	adds r1, r0, #0
	subs r3, #3
	cmp r3, #5
	bhi .L_0804aaba
	ldr r2, .L_0804ab94
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
	.2byte 0x0000
.L_0804a9f0:
	.4byte .L_0804aa1a
	.4byte .L_0804aa2c
	.4byte .L_0804aa08
	.4byte .L_0804aa5a
	.4byte .L_0804aaba
	.4byte .L_0804aaa0
.L_0804aa08:
	movs r2, #56
	ldrsh r3, [r1, r2]
	cmp r3, #0
	bne .L_0804aaba
	ldr r3, [sp, #20]
	movs r7, #1
	ldrh r3, [r3, r5]
	mov r10, r3
	b .L_0804aaba
.L_0804aa1a:
	movs r4, #50
	adds r4, #255
	adds r3, r1, r4
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	beq .L_0804aaba
	b .L_0804aab8
.L_0804aa2c:
	movs r0, #156
	lsls r0, r0, #1
	adds r3, r1, r0
	ldr r3, [r3]
	movs r2, #255
	lsls r2, r2, #24
	adds r2, #255
	ands r3, r2
	cmp r3, #0
	bne .L_0804aab8
	movs r2, #158
	lsls r2, r2, #1
	adds r3, r1, r2
	ldrh r3, [r3]
	cmp r3, #0
	bne .L_0804aab8
	movs r4, #66
	adds r4, #255
	adds r3, r1, r4
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_0804aaba
	b .L_0804aab8
.L_0804aa5a:
	movs r0, #156
	lsls r0, r0, #1
	adds r3, r1, r0
	ldr r3, [r3]
	movs r2, #255
	lsls r2, r2, #24
	adds r2, #255
	ands r3, r2
	cmp r3, #0
	bne .L_0804aab8
	movs r2, #158
	lsls r2, r2, #1
	adds r3, r1, r2
	ldrh r3, [r3]
	cmp r3, #0
	bne .L_0804aab8
	movs r4, #66
	adds r4, #255
	adds r3, r1, r4
	ldrb r3, [r3]
	cmp r3, #0
	bne .L_0804aab8
	subs r0, #7
	adds r3, r1, r0
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	bne .L_0804aab8
	adds r2, #4
	adds r3, r1, r2
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_0804aaba
	b .L_0804aab8
.L_0804aaa0:
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #228
	ldr r3, [r3]
	movs r4, #134
	ldr r0, [sp, #20]
	lsls r4, r4, #1
	adds r3, r3, r4
	ldrh r2, [r0, r5]
	ldr r3, [r3]
	cmp r3, r2
	bne .L_0804aaba
.L_0804aab8:
	movs r7, #1
.L_0804aaba:
	cmp r7, #0
	beq .L_0804aac0
	b .L_0804a986
.L_0804aac0:
	ldr r1, [sp, #64]
	adds r6, #1
	cmp r6, r1
	bge .L_0804aaca
	b .L_0804a9cc
.L_0804aaca:
	ldr r2, [sp, #64]
	movs r6, #0
	cmp r6, r2
	bge .L_0804aaec
	ldr r4, [sp, #20]
	ldrh r3, [r4]
	cmp r3, r10
	beq .L_0804aaec
	adds r2, r4, #0
.L_0804aadc:
	ldr r5, [sp, #64]
	adds r6, #1
	cmp r6, r5
	bge .L_0804aaec
	adds r2, #2
	ldrh r3, [r2]
	cmp r3, r10
	bne .L_0804aadc
.L_0804aaec:
	ldr r7, [sp, #64]
	cmp r6, r7
	beq .L_0804aaf4
	str r6, [sp, #68]
.L_0804aaf4:
	ldr r0, [sp, #68]
	cmp r0, #0
	bge .L_0804ab1c
	ldr r3, [sp, #64]
	subs r3, #1
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	str r3, [sp, #68]
	b .L_0804ab1c
.L_0804ab08:
	ldr r1, [sp, #68]
	ldr r2, [sp, #64]
	adds r3, r1, r2
	subs r3, #1
	adds r0, r3, #0
	adds r1, r2, #0
	str r3, [sp, #68]
	bl Math_Mod
	str r0, [sp, #68]
.L_0804ab1c:
	ldr r3, [sp, #68]
	ldr r4, [sp, #20]
	lsls r3, r3, #1
	str r3, [sp, #16]
	ldrh r3, [r4, r3]
	cmp r3, #254
	beq .L_0804ab08
	movs r0, #182
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0804ab4e
	mov r5, r8
	cmp r5, #1
	bne .L_0804ab4e
	ldr r6, [sp, #20]
	ldr r7, [sp, #16]
	ldrh r0, [r6, r7]
	bl Owner_GetState
	movs r1, #56
	ldrsh r3, [r0, r1]
	cmp r3, #0
	beq .L_0804ab08
.L_0804ab4e:
	mov r2, r8
	cmp r2, #2
	beq .L_0804ab6c
	add r5, sp, #200
	mov r0, r10
	adds r1, r5, #0
	bl Func_08118088 + 0x30
	ldr r4, [sp, #28]
	movs r3, #8
	strb r3, [r4, #2]
	ldr r3, [r5]
	strb r3, [r4]
	movs r3, #128
	strb r3, [r4, #1]
.L_0804ab6c:
	movs r3, #74
	str r3, [sp, #0]
	movs r0, #0
	movs r1, #12
	movs r2, #30
	movs r3, #4
	bl UiWindow_Create
	movs r6, #152
	mov r5, sp
	mov r7, sp
	adds r5, #236
	add r6, sp
	adds r7, #88
	str r0, [sp, #72]
	str r5, [sp, #24]
	mov r11, r6
	str r7, [sp, #36]
	b .L_0804ab9e
	.2byte 0x0000
.L_0804ab94:
	.4byte .L_0804a9f0
.L_0804ab98:
	ldr r0, [sp, #68]
	lsls r0, r0, #1
	str r0, [sp, #16]
.L_0804ab9e:
	movs r1, #0
	str r1, [sp, #48]
	ldr r3, [sp, #16]
	ldr r2, [sp, #20]
	mov r1, r11
	ldrh r0, [r2, r3]
	bl Func_08118088 + 0x30
	ldr r3, .L_0804abfc
	ldr r4, [sp, #24]
	str r3, [r4, #4]
	ldr r5, [sp, #48]
	str r5, [r4, #8]
	ldr r5, .L_0804ac00
	movs r3, #31
	ldr r1, [r5]
	ldr r0, [sp, #52]
	lsrs r1, r1, #2
	ands r1, r3
	ldr r3, .L_0804ac04
	lsls r1, r1, #8
	adds r1, r1, r3
	bl Resource_GetBuffer
	ldr r3, .L_0804abf8
	ldr r6, [sp, #24]
	ands r0, r3
	ldrh r2, [r6, #8]
	ldr r3, .L_0804ac08
	adds r7, r6, #0
	ands r3, r2
	orrs r3, r0
	ldr r0, [r5]
	strh r3, [r7, #8]
	lsls r0, r0, #12
	bl Trig_Sin
	cmp r0, #0
	bge .L_0804ac0c
	movs r1, #254
	lsls r1, r1, #7
	adds r1, #255
	adds r0, r0, r1
	b .L_0804ac0c
	.2byte 0x0000
.L_0804abf8:
	.4byte 0x000003ff
.L_0804abfc:
	.4byte 0x40002000
.L_0804ac00:
	.4byte Data_0300122c
.L_0804ac04:
	.4byte Data_0805c9c4
.L_0804ac08:
	.4byte 0xfffffc00
.L_0804ac0c:
	mov r4, r11
	ldr r3, [r4, #4]
	asrs r2, r0, #15
	adds r0, r3, r2
	str r0, [r4, #4]
	ldr r5, [sp, #28]
	movs r1, #1
	ldrb r2, [r5, #2]
	adds r3, r1, #0
	ands r3, r2
	cmp r3, #0
	beq .L_0804ac5e
	ldr r4, [r4]
	ldrb r3, [r5]
	adds r3, r4, r3
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r1, r3, #1
	ldrb r3, [r5, #1]
	adds r3, r0, r3
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r0, r3, #1
	subs r3, r4, r1
	cmp r3, #0
	blt .L_0804ac46
	cmp r3, #7
	ble .L_0804ac4c
	b .L_0804ac50
.L_0804ac46:
	subs r3, r1, r4
	cmp r3, #7
	bgt .L_0804ac50
.L_0804ac4c:
	movs r6, #1
	str r6, [sp, #48]
.L_0804ac50:
	mov r7, r11
	str r1, [r7]
	ldr r2, [sp, #28]
	str r0, [r7, #4]
	strb r1, [r2]
	strb r0, [r2, #1]
	b .L_0804ac96
.L_0804ac5e:
	movs r4, #192
	lsls r3, r2, #24
	lsls r4, r4, #18
	cmp r3, r4
	bhi .L_0804ac78
	mov r5, r11
	ldr r6, [sp, #28]
	ldr r3, [r5]
	str r0, [r5, #4]
	strb r3, [r6]
	strb r0, [r6, #1]
	strb r1, [r6, #2]
	b .L_0804ac96
.L_0804ac78:
	ldr r7, [sp, #28]
	mov r0, r11
	ldrb r3, [r7]
	str r3, [r0]
	ldrb r3, [r7, #1]
	str r3, [r0, #4]
	adds r3, r2, #0
	adds r3, #252
	movs r2, #192
	strb r3, [r7, #2]
	lsls r2, r2, #18
	lsls r3, r3, #24
	cmp r3, r2
	bhi .L_0804ac96
	strb r1, [r7, #2]
.L_0804ac96:
	mov r3, r11
	ldr r2, [r3]
	ldr r4, [sp, #24]
	ldr r3, .L_0804acd4
	subs r2, #8
	ldrh r1, [r4, #6]
	ands r2, r3
	ldr r3, .L_0804acd8
	adds r5, r4, #0
	ands r3, r1
	orrs r3, r2
	mov r6, r11
	strh r3, [r5, #6]
	ldr r3, [r6, #4]
	movs r1, #240
	subs r3, #16
	strb r3, [r5, #4]
	ldr r0, [sp, #24]
	bl Func_08014128
	ldr r7, [sp, #84]
	cmp r7, #255
	bne .L_0804acec
	ldr r2, .L_0804acdc
	ldr r3, [sp, #88]
	ands r3, r2
	movs r2, #128
	lsls r2, r2, #1
	orrs r3, r2
	movs r2, #255
	b .L_0804ace0
.L_0804acd4:
	.4byte 0x000001ff
.L_0804acd8:
	.4byte 0xfffffe00
.L_0804acdc:
	.4byte 0xffff0000
.L_0804ace0:
	lsls r2, r2, #8
	adds r2, #255
	ands r3, r2
	movs r2, #128
	lsls r2, r2, #17
	b .L_0804ad02
.L_0804acec:
	ldr r2, .L_0804ae04
	ldr r3, [sp, #88]
	ands r3, r2
	movs r2, #176
	orrs r3, r2
	movs r2, #255
	lsls r2, r2, #8
	adds r2, #255
	ands r3, r2
	movs r2, #176
	lsls r2, r2, #16
.L_0804ad02:
	orrs r3, r2
	str r3, [sp, #88]
	ldr r0, [sp, #36]
	ldr r3, .L_0804ae04
	ldr r2, [r0, #4]
	ands r2, r3
	str r2, [r0, #4]
	ldr r0, [sp, #36]
	bl Func_0801401c
	ldr r1, [sp, #56]
	movs r3, #1
	ands r3, r1
	str r0, [sp, #44]
	cmp r3, #0
	bne .L_0804ad24
	b .L_0804b3b6
.L_0804ad24:
	movs r2, #0
	str r2, [sp, #64]
	ldr r1, [sp, #28]
	movs r0, #253
	movs r7, #5
.L_0804ad2e:
	ldrb r2, [r1, #2]
	adds r3, r0, #0
	ands r3, r2
	subs r7, #1
	strb r3, [r1, #2]
	adds r1, #4
	cmp r7, #0
	bge .L_0804ad2e
	ldr r3, [sp, #84]
	movs r7, #0
	cmp r7, r3
	bcs .L_0804ae08
	ldr r6, [sp, #64]
	add r4, sp, #172
	ldr r1, [sp, #64]
	mov r10, r4
	ldr r5, [sp, #20]
	add r0, sp, #324
	ldr r4, [sp, #28]
	adds r3, r6, r0
	adds r6, r3, #0
	mov r2, r10
	lsls r3, r1, #1
	mov lr, r5
	adds r0, r4, #0
	adds r5, r3, r2
	movs r3, #254
	subs r6, #160
	mov r8, r3
	adds r0, #24
.L_0804ad6a:
	ldr r1, [sp, #68]
	ldr r2, [sp, #60]
	adds r3, r1, r7
	cmp r3, r2
	bge .L_0804adaa
	lsls r3, r3, #1
	mov r1, lr
	ldrh r2, [r1, r3]
	adds r3, r2, #0
	cmp r3, #254
	beq .L_0804adaa
	ldrb r3, [r4, #2]
	strh r2, [r5]
	movs r2, #2
	orrs r2, r3
	movs r3, #0
	orrs r2, r3
	movs r3, #3
	ldrsb r3, [r4, r3]
	strb r2, [r4, #2]
	cmp r3, r7
	beq .L_0804ad9e
	mov r1, r8
	ands r2, r1
	strb r2, [r4, #2]
	strb r7, [r4, #3]
.L_0804ad9e:
	strb r7, [r6]
	ldr r2, [sp, #64]
	adds r6, #1
	adds r2, #1
	str r2, [sp, #64]
	adds r5, #2
.L_0804adaa:
	cmp r7, #0
	beq .L_0804adf6
	ldr r1, [sp, #68]
	subs r3, r1, r7
	cmp r3, #0
	blt .L_0804adf6
	lsls r3, r3, #1
	mov r1, lr
	ldrh r2, [r1, r3]
	adds r3, r2, #0
	cmp r3, #254
	beq .L_0804adf6
	ldrb r3, [r0, #2]
	strh r2, [r5]
	movs r2, #6
	subs r2, r2, r7
	mov r12, r2
	movs r2, #2
	orrs r2, r3
	movs r3, #0
	orrs r2, r3
	movs r3, #3
	ldrsb r3, [r0, r3]
	negs r1, r7
	strb r2, [r0, #2]
	cmp r3, r1
	beq .L_0804ade8
	mov r3, r8
	ands r2, r3
	strb r2, [r0, #2]
	strb r1, [r0, #3]
.L_0804ade8:
	mov r1, r12
	strb r1, [r6]
	ldr r2, [sp, #64]
	adds r6, #1
	adds r2, #1
	str r2, [sp, #64]
	adds r5, #2
.L_0804adf6:
	ldr r3, [sp, #84]
	adds r7, #1
	adds r4, #4
	subs r0, #4
	cmp r7, r3
	bcc .L_0804ad6a
	b .L_0804ae0c
.L_0804ae04:
	.4byte 0xffff0000
.L_0804ae08:
	add r4, sp, #172
	mov r10, r4
.L_0804ae0c:
	ldr r1, [sp, #28]
	movs r4, #2
	movs r0, #6
	movs r7, #5
.L_0804ae14:
	ldrb r2, [r1, #2]
	adds r3, r4, #0
	ands r3, r2
	cmp r3, #0
	bne .L_0804ae20
	strb r0, [r1, #3]
.L_0804ae20:
	subs r7, #1
	adds r1, #4
	cmp r7, #0
	bge .L_0804ae14
	ldr r5, [sp, #64]
	ldr r3, .L_0804ae60
	lsls r2, r5, #1
	mov r6, r10
	strh r3, [r6, r2]
	mov r0, r10
	movs r1, #1
	bl Func_08118088 + 0x58
	movs r3, #192
	ldr r1, [sp, #20]
	ldr r2, [sp, #16]
	lsls r3, r3, #18
	adds r3, #228
	ldr r3, [r3]
	movs r7, #134
	ldrh r0, [r1, r2]
	lsls r7, r7, #1
	adds r3, r3, r7
	str r0, [r3]
	cmp r0, #7
	bls .L_0804ae56
	b .L_0804b2b0
.L_0804ae56:
	ldr r3, [sp, #84]
	cmp r3, #255
	bne .L_0804ae5e
	b .L_0804b3ac
.L_0804ae5e:
	b .L_0804ae64
.L_0804ae60:
	.4byte 0x000000ff
.L_0804ae64:
	ldr r4, [sp, #80]
	cmp r4, #0
	bne .L_0804ae6c
	b .L_0804b3ac
.L_0804ae6c:
	bl Owner_GetState
	ldr r5, [sp, #20]
	ldr r7, [sp, #16]
	adds r6, r0, #0
	mov r1, r11
	ldrh r0, [r5, r7]
	bl Func_08118088 + 0x30
	mov r0, r9
	cmp r0, #0
	beq .L_0804ae8a
	movs r1, #1
	bl UiWork_Finalize
.L_0804ae8a:
	ldr r3, [sp, #80]
	subs r3, #1
	cmp r3, #6
	bls .L_0804ae94
	b .L_0804b3ac
.L_0804ae94:
	ldr r2, .L_0804b108
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
.L_0804ae9c:
	.4byte .L_0804aee8
	.4byte .L_0804af42
	.4byte .L_0804afd8
	.4byte .L_0804b01e
	.4byte .L_0804afa4
	.4byte .L_0804b138
	.4byte .L_0804aeb8
.L_0804aeb8:
	mov r1, r11
	ldr r3, [r1]
	cmp r3, #0
	bge .L_0804aec2
	adds r3, #7
.L_0804aec2:
	asrs r3, r3, #3
	subs r0, r3, #4
	adds r3, #4
	cmp r3, #29
	ble .L_0804aece
	movs r0, #22
.L_0804aece:
	movs r3, #6
	str r3, [sp, #0]
	movs r1, #8
	movs r2, #9
	movs r3, #3
	bl UiWindow_Create
	mov r9, r0
	movs r0, #2
	bl Func_08041f70
	ldr r0, .L_0804b10c
	b .L_0804b29e
.L_0804aee8:
	mov r2, r11
	ldr r3, [r2]
	cmp r3, #0
	bge .L_0804aef2
	adds r3, #7
.L_0804aef2:
	asrs r3, r3, #3
	subs r0, r3, #7
	adds r3, #6
	cmp r3, #29
	ble .L_0804aefe
	movs r0, #17
.L_0804aefe:
	movs r3, #6
	str r3, [sp, #0]
	movs r1, #8
	movs r2, #13
	movs r3, #3
	bl UiWindow_Create
	mov r9, r0
	movs r3, #0
	ldr r0, .L_0804b110
	mov r1, r9
	movs r2, #0
	bl UiText_DrawStringAtOffset
	movs r5, #0
	movs r3, #56
	ldrsh r0, [r6, r3]
	movs r1, #4
	mov r2, r9
	movs r3, #16
	str r5, [sp, #0]
	bl UiText_DrawNumberInWindow
	ldr r0, .L_0804b114
	mov r1, r9
	movs r2, #48
	movs r3, #0
	bl UiText_DrawStringAtOffset
	movs r4, #52
	ldrsh r0, [r6, r4]
	movs r1, #4
	mov r2, r9
	b .L_0804af9a
.L_0804af42:
	mov r5, r11
	ldr r3, [r5]
	cmp r3, #0
	bge .L_0804af4c
	adds r3, #7
.L_0804af4c:
	asrs r3, r3, #3
	subs r0, r3, #7
	adds r3, #6
	cmp r3, #29
	ble .L_0804af58
	movs r0, #17
.L_0804af58:
	movs r3, #6
	str r3, [sp, #0]
	movs r1, #8
	movs r2, #13
	movs r3, #3
	bl UiWindow_Create
	mov r9, r0
	mov r1, r9
	ldr r0, .L_0804b118
	movs r2, #0
	movs r3, #0
	bl UiText_DrawStringAtOffset
	movs r5, #0
	movs r7, #58
	ldrsh r0, [r6, r7]
	movs r1, #4
	mov r2, r9
	movs r3, #16
	str r5, [sp, #0]
	bl UiText_DrawNumberInWindow
	mov r1, r9
	ldr r0, .L_0804b114
	movs r2, #48
	movs r3, #0
	bl UiText_DrawStringAtOffset
	movs r1, #54
	ldrsh r0, [r6, r1]
	mov r2, r9
	movs r1, #4
.L_0804af9a:
	movs r3, #56
	str r5, [sp, #0]
	bl UiText_DrawNumberInWindow
	b .L_0804b3ac
.L_0804afa4:
	mov r2, r11
	ldr r3, [r2]
	cmp r3, #0
	bge .L_0804afae
	adds r3, #7
.L_0804afae:
	asrs r3, r3, #3
	subs r0, r3, #7
	adds r3, #5
	cmp r3, #29
	ble .L_0804afba
	movs r0, #18
.L_0804afba:
	movs r3, #6
	str r3, [sp, #0]
	movs r1, #8
	movs r3, #3
	movs r2, #12
	bl UiWindow_Create
	movs r4, #56
	ldrsh r3, [r6, r4]
	mov r9, r0
	cmp r3, #0
	beq .L_0804afd4
	b .L_0804b296
.L_0804afd4:
	ldr r0, .L_0804b11c
	b .L_0804b012
.L_0804afd8:
	mov r5, r11
	ldr r3, [r5]
	cmp r3, #0
	bge .L_0804afe2
	adds r3, #7
.L_0804afe2:
	asrs r3, r3, #3
	subs r0, r3, #7
	adds r3, #5
	cmp r3, #29
	ble .L_0804afee
	movs r0, #18
.L_0804afee:
	movs r3, #6
	str r3, [sp, #0]
	movs r1, #8
	movs r3, #3
	movs r2, #12
	bl UiWindow_Create
	movs r7, #50
	adds r7, #255
	adds r3, r6, r7
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	mov r9, r0
	cmp r3, #0
	bne .L_0804b010
	b .L_0804b296
.L_0804b010:
	ldr r0, .L_0804b120
.L_0804b012:
	mov r1, r9
	movs r2, #0
	movs r3, #0
	bl UiText_DrawCharacterAtOffset
	b .L_0804b3ac
.L_0804b01e:
	movs r0, #156
	lsls r0, r0, #1
	adds r7, r6, r0
	ldrb r3, [r7]
	movs r5, #0
	cmp r3, #0
	beq .L_0804b02e
	movs r5, #1
.L_0804b02e:
	movs r1, #60
	adds r1, #255
	adds r1, r1, r6
	ldrb r3, [r1]
	mov r8, r1
	cmp r3, #0
	beq .L_0804b03e
	adds r5, #1
.L_0804b03e:
	movs r2, #158
	lsls r2, r2, #1
	adds r2, r2, r6
	ldrb r3, [r2]
	mov r10, r2
	cmp r3, #0
	beq .L_0804b04e
	adds r5, #1
.L_0804b04e:
	movs r3, #62
	adds r3, #255
	adds r3, r6, r3
	str r3, [sp, #40]
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_0804b05e
	adds r5, #1
.L_0804b05e:
	movs r4, #66
	adds r4, #255
	adds r6, r6, r4
	ldrb r3, [r6]
	cmp r3, #0
	beq .L_0804b06c
	adds r5, #1
.L_0804b06c:
	cmp r5, #0
	bne .L_0804b072
	movs r5, #1
.L_0804b072:
	movs r3, #9
	subs r1, r3, r5
	cmp r1, #3
	bgt .L_0804b07c
	movs r1, #4
.L_0804b07c:
	mov r0, r11
	ldr r3, [r0]
	cmp r3, #0
	bge .L_0804b086
	adds r3, #7
.L_0804b086:
	asrs r3, r3, #3
	subs r0, r3, #7
	adds r3, #9
	cmp r3, #29
	ble .L_0804b092
	movs r0, #14
.L_0804b092:
	movs r2, #6
	adds r3, r5, #2
	str r2, [sp, #0]
	movs r2, #16
	bl UiWindow_Create
	ldrb r3, [r7]
	mov r9, r0
	movs r5, #0
	cmp r3, #0
	beq .L_0804b0b6
	ldr r0, .L_0804b124
	mov r1, r9
	movs r2, #0
	movs r3, #0
	bl UiText_DrawCharacterAtOffset
	movs r5, #1
.L_0804b0b6:
	mov r1, r8
	ldrb r3, [r1]
	cmp r3, #0
	beq .L_0804b0cc
	lsls r3, r5, #3
	ldr r0, .L_0804b128
	mov r1, r9
	movs r2, #0
	bl UiText_DrawCharacterAtOffset
	adds r5, #1
.L_0804b0cc:
	mov r2, r10
	ldrb r3, [r2]
	cmp r3, #0
	beq .L_0804b0e2
	lsls r3, r5, #3
	ldr r0, .L_0804b12c
	mov r1, r9
	movs r2, #0
	bl UiText_DrawCharacterAtOffset
	adds r5, #1
.L_0804b0e2:
	ldr r4, [sp, #40]
	ldrb r3, [r4]
	cmp r3, #0
	beq .L_0804b0f8
	lsls r3, r5, #3
	ldr r0, .L_0804b130
	mov r1, r9
	movs r2, #0
	bl UiText_DrawCharacterAtOffset
	adds r5, #1
.L_0804b0f8:
	ldrb r3, [r6]
	cmp r3, #0
	bne .L_0804b100
	b .L_0804b290
.L_0804b100:
	lsls r3, r5, #3
	ldr r0, .L_0804b134
	b .L_0804b286
	.2byte 0x0000
.L_0804b108:
	.4byte .L_0804ae9c
.L_0804b10c:
	.4byte 0x00000d0c
.L_0804b110:
	.4byte Data_0805f88c
.L_0804b114:
	.4byte Data_0805f890
.L_0804b118:
	.4byte Data_0805f894
.L_0804b11c:
	.4byte 0x00000d0b
.L_0804b120:
	.4byte 0x00000d04
.L_0804b124:
	.4byte 0x00000d05
.L_0804b128:
	.4byte 0x00000d06
.L_0804b12c:
	.4byte 0x00000d07
.L_0804b130:
	.4byte 0x00000d08
.L_0804b134:
	.4byte 0x00000d09
.L_0804b138:
	movs r7, #50
	adds r7, #255
	adds r3, r6, r7
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	movs r5, #0
	cmp r3, #0
	beq .L_0804b14c
	movs r5, #1
.L_0804b14c:
	movs r0, #156
	lsls r0, r0, #1
	adds r3, r6, r0
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_0804b15a
	adds r5, #1
.L_0804b15a:
	movs r1, #60
	adds r1, #255
	adds r3, r6, r1
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_0804b168
	adds r5, #1
.L_0804b168:
	movs r2, #158
	lsls r2, r2, #1
	adds r3, r6, r2
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_0804b176
	adds r5, #1
.L_0804b176:
	movs r4, #62
	adds r4, #255
	adds r3, r6, r4
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_0804b184
	adds r5, #1
.L_0804b184:
	movs r7, #66
	adds r7, #255
	adds r3, r6, r7
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_0804b192
	adds r5, #1
.L_0804b192:
	movs r0, #160
	lsls r0, r0, #1
	adds r3, r6, r0
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_0804b1a0
	adds r5, #1
.L_0804b1a0:
	cmp r5, #0
	bne .L_0804b1a6
	movs r5, #1
.L_0804b1a6:
	movs r3, #9
	subs r1, r3, r5
	cmp r1, #3
	bgt .L_0804b1b0
	movs r1, #4
.L_0804b1b0:
	mov r2, r11
	ldr r3, [r2]
	cmp r3, #0
	bge .L_0804b1ba
	adds r3, #7
.L_0804b1ba:
	asrs r3, r3, #3
	subs r0, r3, #7
	adds r3, #9
	cmp r3, #29
	ble .L_0804b1c6
	movs r0, #14
.L_0804b1c6:
	movs r2, #6
	adds r3, r5, #2
	str r2, [sp, #0]
	movs r2, #16
	bl UiWindow_Create
	movs r4, #50
	adds r4, #255
	adds r3, r6, r4
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	mov r9, r0
	movs r5, #0
	cmp r3, #0
	beq .L_0804b1f4
	ldr r0, .L_0804b33c
	mov r1, r9
	movs r2, #0
	movs r3, #0
	bl UiText_DrawCharacterAtOffset
	movs r5, #1
.L_0804b1f4:
	movs r7, #156
	lsls r7, r7, #1
	adds r3, r6, r7
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_0804b20e
	lsls r3, r5, #3
	ldr r0, .L_0804b340
	mov r1, r9
	movs r2, #0
	bl UiText_DrawCharacterAtOffset
	adds r5, #1
.L_0804b20e:
	movs r0, #60
	adds r0, #255
	adds r3, r6, r0
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_0804b228
	lsls r3, r5, #3
	ldr r0, .L_0804b344
	mov r1, r9
	movs r2, #0
	bl UiText_DrawCharacterAtOffset
	adds r5, #1
.L_0804b228:
	movs r1, #158
	lsls r1, r1, #1
	adds r3, r6, r1
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_0804b242
	lsls r3, r5, #3
	ldr r0, .L_0804b348
	mov r1, r9
	movs r2, #0
	bl UiText_DrawCharacterAtOffset
	adds r5, #1
.L_0804b242:
	movs r2, #62
	adds r2, #255
	adds r3, r6, r2
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_0804b25c
	lsls r3, r5, #3
	ldr r0, .L_0804b34c
	mov r1, r9
	movs r2, #0
	bl UiText_DrawCharacterAtOffset
	adds r5, #1
.L_0804b25c:
	movs r4, #66
	adds r4, #255
	adds r3, r6, r4
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_0804b276
	lsls r3, r5, #3
	ldr r0, .L_0804b350
	mov r1, r9
	movs r2, #0
	bl UiText_DrawCharacterAtOffset
	adds r5, #1
.L_0804b276:
	movs r7, #160
	lsls r7, r7, #1
	adds r3, r6, r7
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_0804b290
	lsls r3, r5, #3
	ldr r0, .L_0804b354
.L_0804b286:
	mov r1, r9
	movs r2, #0
	bl UiText_DrawCharacterAtOffset
	adds r5, #1
.L_0804b290:
	cmp r5, #0
	beq .L_0804b296
	b .L_0804b3ac
.L_0804b296:
	movs r0, #2
	bl Func_08041f70
	ldr r0, .L_0804b358
.L_0804b29e:
	mov r1, r9
	movs r2, #0
	movs r3, #0
	bl UiText_DrawCharacterAtOffset
	movs r0, #15
	bl Func_08041f70
	b .L_0804b3ac
.L_0804b2b0:
	ldr r1, [sp, #84]
	cmp r1, #255
	bne .L_0804b2b8
	b .L_0804b3ac
.L_0804b2b8:
	bl Owner_GetState
	ldr r3, [sp, #16]
	ldr r2, [sp, #20]
	add r5, sp, #108
	mov r8, r0
	adds r1, r5, #0
	ldrh r0, [r2, r3]
	bl Func_08118088 + 0x30
	ldr r3, .L_0804b35c
	ldr r0, [r3]
	lsls r0, r0, #12
	bl Trig_Sin
	cmp r0, #0
	bge .L_0804b2e2
	movs r4, #254
	lsls r4, r4, #7
	adds r4, #255
	adds r0, r0, r4
.L_0804b2e2:
	ldr r3, [r5, #4]
	asrs r2, r0, #15
	adds r3, r3, r2
	str r3, [r5, #4]
	movs r3, #165
	lsls r3, r3, #1
	add r3, r8
	ldrh r3, [r3]
	movs r6, #100
	adds r6, #255
	cmp r3, r6
	beq .L_0804b308
	movs r7, #176
	lsls r7, r7, #1
	cmp r3, r7
	beq .L_0804b308
	movs r7, #0
	add r6, sp, #120
	b .L_0804b328
.L_0804b308:
	movs r3, #165
	lsls r3, r3, #1
	add r3, r8
	ldrh r3, [r3]
	movs r1, #100
	adds r1, #255
	ldr r0, .L_0804b360
	cmp r3, r1
	bne .L_0804b31c
	adds r0, #1
.L_0804b31c:
	add r6, sp, #120
	adds r1, r6, #0
	movs r2, #14
	bl UiText_CopyMessageString
	b .L_0804b36a
.L_0804b328:
	cmp r7, #13
	bgt .L_0804b364
	mov r4, r8
	ldrb r3, [r4, r7]
	lsls r2, r7, #1
	strh r3, [r6, r2]
	adds r7, #1
	cmp r3, #0
	bne .L_0804b328
	b .L_0804b366
.L_0804b33c:
	.4byte 0x00000d04
.L_0804b340:
	.4byte 0x00000d05
.L_0804b344:
	.4byte 0x00000d06
.L_0804b348:
	.4byte 0x00000d07
.L_0804b34c:
	.4byte 0x00000d08
.L_0804b350:
	.4byte 0x00000d09
.L_0804b354:
	.4byte 0x00000d0a
.L_0804b358:
	.4byte 0x00000d03
.L_0804b35c:
	.4byte Data_0300122c
.L_0804b360:
	.4byte 0x00000c5a
.L_0804b364:
	lsls r2, r7, #1
.L_0804b366:
	ldr r3, .L_0804b398
	strh r3, [r6, r2]
.L_0804b36a:
	adds r0, r6, #0
	bl UiText_GetWideStringWidth
	ldr r3, [r5]
	lsrs r2, r0, #31
	adds r2, r0, r2
	asrs r2, r2, #1
	subs r3, r3, r2
	subs r3, #8
	str r3, [r5]
	adds r3, r3, r0
	cmp r3, #224
	ble .L_0804b38a
	movs r3, #224
	subs r3, r3, r0
	str r3, [r5]
.L_0804b38a:
	ldr r3, [r5]
	cmp r3, #0
	bge .L_0804b39c
	movs r3, #0
	str r3, [r5]
	b .L_0804b39c
	.2byte 0x0000
.L_0804b398:
	.4byte 0x00000000
.L_0804b39c:
	bl Func_080396a0
	ldr r2, [r5]
	adds r0, r6, #0
	ldr r1, [sp, #72]
	movs r3, #4
	bl Func_0803aae4
.L_0804b3ac:
	ldr r5, [sp, #56]
	movs r3, #2
	negs r3, r3
	ands r5, r3
	str r5, [sp, #56]
.L_0804b3b6:
	ldr r6, [sp, #48]
	cmp r6, #0
	bne .L_0804b3be
	b .L_0804b500
.L_0804b3be:
	ldr r0, [sp, #64]
	movs r7, #1
	cmp r7, r0
	blt .L_0804b3c8
	b .L_0804b500
.L_0804b3c8:
	mov r2, sp
	adds r2, #164
	movs r3, #96
	ldr r5, [sp, #24]
	movs r6, #2
	movs r1, #172
	add r3, sp
	str r2, [sp, #32]
	str r6, [sp, #12]
	add r1, sp
	mov r8, r3
	mov r10, r1
	mov r4, r8
	adds r5, #12
.L_0804b3e4:
	ldr r0, [sp, #32]
	ldr r1, [sp, #28]
	ldrb r3, [r0, r7]
	ldr r2, [sp, #12]
	lsls r3, r3, #2
	adds r3, r1, r3
	str r3, [sp, #4]
	mov r3, r10
	ldrh r0, [r2, r3]
	adds r1, r4, #0
	str r4, [sp, #8]
	bl Func_08118088 + 0x30
	ldr r3, .L_0804b4bc
	ldr r0, [r3]
	lsls r0, r0, #12
	bl Trig_Sin
	ldr r4, [sp, #8]
	cmp r0, #0
	bge .L_0804b416
	movs r6, #254
	lsls r6, r6, #7
	adds r6, #255
	adds r0, r0, r6
.L_0804b416:
	ldr r3, [r4, #4]
	asrs r2, r0, #15
	adds r3, r3, r2
	str r3, [r4, #4]
	ldr r0, [sp, #24]
	mov lr, r5
	mov r12, r0
	mov r1, lr
	mov r2, r12
	ldmia r2!, {r0, r3, r6}
	stmia r1!, {r0, r3, r6}
	ldr r1, [sp, #4]
	ldrb r2, [r1, #2]
	movs r1, #1
	adds r3, r1, #0
	ands r3, r2
	cmp r3, #0
	beq .L_0804b460
	ldr r2, [sp, #4]
	ldr r1, [r4]
	ldrb r3, [r2]
	ldrb r2, [r2, #1]
	adds r1, r1, r3
	lsrs r3, r1, #31
	adds r1, r1, r3
	ldr r3, [r4, #4]
	asrs r1, r1, #1
	adds r3, r3, r2
	str r1, [r4]
	ldr r6, [sp, #4]
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	strb r1, [r6]
	str r3, [r4, #4]
	strb r3, [r6, #1]
	b .L_0804b476
.L_0804b460:
	ldrh r3, [r5, #6]
	ldrb r2, [r5, #4]
	ldr r0, [sp, #4]
	lsls r3, r3, #23
	lsrs r3, r3, #23
	adds r2, #8
	strb r1, [r0, #2]
	str r3, [r4]
	strb r3, [r0]
	str r2, [r4, #4]
	strb r2, [r0, #1]
.L_0804b476:
	ldrb r2, [r5, #5]
	movs r1, #13
	negs r1, r1
	adds r3, r1, #0
	adds r0, r2, #0
	mov r2, r8
	ands r0, r3
	ldr r1, [r2]
	movs r3, #4
	orrs r0, r3
	ldr r3, .L_0804b4b4
	subs r1, #8
	ands r1, r3
	ldr r2, .L_0804b4b8
	ldrh r3, [r5, #6]
	mov r6, r8
	ands r3, r2
	orrs r3, r1
	strh r3, [r5, #6]
	ldr r3, [r6, #4]
	strb r0, [r5, #5]
	subs r3, #12
	strb r3, [r5, #4]
	ldr r1, [sp, #84]
	cmp r1, #255
	bne .L_0804b4c0
	movs r2, #4
	negs r2, r2
	ands r0, r2
	b .L_0804b4ca
	.2byte 0x0000
.L_0804b4b4:
	.4byte 0x000001ff
.L_0804b4b8:
	.4byte 0xfffffe00
.L_0804b4bc:
	.4byte Data_0300122c
.L_0804b4c0:
	movs r3, #4
	negs r3, r3
	ands r0, r3
	movs r3, #1
	orrs r0, r3
.L_0804b4ca:
	strb r0, [r5, #5]
	ldr r2, [sp, #44]
	movs r3, #31
	ands r2, r3
	movs r6, #63
	ldrb r3, [r5, #7]
	negs r6, r6
	adds r1, r6, #0
	lsls r2, r2, #1
	ands r3, r1
	orrs r3, r2
	strb r3, [r5, #7]
	adds r0, r5, #0
	movs r1, #240
	str r4, [sp, #8]
	bl Func_08014128
	ldr r0, [sp, #12]
	ldr r1, [sp, #64]
	adds r0, #2
	adds r7, #1
	adds r5, #12
	str r0, [sp, #12]
	ldr r4, [sp, #8]
	cmp r7, r1
	bge .L_0804b500
	b .L_0804b3e4
.L_0804b500:
	ldr r3, .L_0804b69c
	ldr r6, [r3, #4]
	ldr r5, [r3, #12]
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #228
	ldr r2, [r3]
	adds r3, r2, #0
	adds r3, #216
	ldr r3, [r3]
	cmp r3, #0
	beq .L_0804b532
	adds r2, #220
	ldr r3, [r2]
	movs r5, #0
	movs r6, #0
	cmp r3, #0
	bne .L_0804b52e
	movs r3, #60
	str r3, [r2]
	movs r5, #1
	movs r6, #1
	b .L_0804b532
.L_0804b52e:
	subs r3, #1
	str r3, [r2]
.L_0804b532:
	movs r3, #1
	ands r3, r6
	cmp r3, #0
	beq .L_0804b5b4
	ldr r2, [sp, #20]
	ldr r3, [sp, #16]
	movs r5, #0
	ldrh r4, [r2, r3]
	ldr r7, [sp, #76]
	str r5, [sp, #56]
	movs r3, #88
	ldrsh r3, [r7, r3]
	movs r0, #1
	negs r0, r0
	movs r1, #0
	cmp r3, #255
	beq .L_0804b57a
	cmp r3, r4
	bne .L_0804b55e
	movs r0, #128
	lsls r0, r0, #1
	b .L_0804b57a
.L_0804b55e:
	adds r1, #1
	cmp r1, #5
	bgt .L_0804b57a
	ldr r5, [sp, #76]
	lsls r3, r1, #1
	adds r3, #88
	ldrsh r3, [r5, r3]
	cmp r3, #255
	beq .L_0804b57a
	cmp r3, r4
	bne .L_0804b55e
	movs r0, #128
	lsls r0, r0, #1
	orrs r0, r1
.L_0804b57a:
	cmp r0, #0
	bge .L_0804b5b0
	ldr r2, [sp, #76]
	movs r5, #192
	adds r2, #102
	movs r7, #0
	ldrsh r3, [r2, r7]
	movs r1, #0
	lsls r5, r5, #1
	cmp r3, #255
	beq .L_0804b5b0
	cmp r3, r4
	bne .L_0804b598
	adds r0, r5, #0
	b .L_0804b5b0
.L_0804b598:
	adds r1, #1
	adds r2, #2
	cmp r1, #5
	bgt .L_0804b5b0
	movs r7, #0
	ldrsh r3, [r2, r7]
	cmp r3, #255
	beq .L_0804b5b0
	cmp r3, r4
	bne .L_0804b598
	adds r0, r5, #0
	orrs r0, r1
.L_0804b5b0:
	str r0, [sp, #68]
	b .L_0804b61c
.L_0804b5b4:
	ldr r0, [sp, #84]
	cmp r0, #255
	beq .L_0804b61c
	movs r3, #144
	ands r3, r5
	cmp r3, #0
	beq .L_0804b5ea
	movs r0, #111
	bl Audio_PlayCue
.L_0804b5c8:
	ldr r1, [sp, #68]
	adds r1, #1
	str r1, [sp, #68]
	adds r0, r1, #0
	ldr r1, [sp, #60]
	bl Math_Mod
	str r0, [sp, #68]
	ldr r4, [sp, #20]
	lsls r2, r0, #1
	ldrh r3, [r4, r2]
	cmp r3, #254
	beq .L_0804b5c8
	ldr r7, [sp, #56]
	movs r3, #1
	orrs r7, r3
	str r7, [sp, #56]
.L_0804b5ea:
	movs r3, #96
	ands r3, r5
	cmp r3, #0
	beq .L_0804b61c
	movs r0, #111
	bl Audio_PlayCue
.L_0804b5f8:
	ldr r0, [sp, #68]
	ldr r1, [sp, #60]
	adds r3, r0, r1
	subs r3, #1
	adds r0, r3, #0
	str r3, [sp, #68]
	bl Math_Mod
	str r0, [sp, #68]
	ldr r2, [sp, #20]
	lsls r3, r0, #1
	ldrh r3, [r2, r3]
	cmp r3, #254
	beq .L_0804b5f8
	ldr r4, [sp, #56]
	movs r3, #1
	orrs r4, r3
	str r4, [sp, #56]
.L_0804b61c:
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #228
	ldr r3, [r3]
	ldr r3, [r3, #76]
	cmp r3, #0
	beq .L_0804b632
	movs r3, #2
	ands r3, r6
	cmp r3, #0
	beq .L_0804b640
.L_0804b632:
	movs r0, #113
	bl Audio_PlayCue
	movs r5, #1
	negs r5, r5
	str r5, [sp, #68]
	b .L_0804b650
.L_0804b640:
	movs r0, #1
	bl WaitFrames
	ldr r6, [sp, #56]
	cmp r6, #0
	beq .L_0804b650
	bl .L_0804ab98
.L_0804b650:
	movs r0, #1
	bl WaitFrames
	mov r7, r9
	ldr r0, [sp, #52]
	bl Func_08014274
	cmp r7, #0
	beq .L_0804b66a
	mov r0, r9
	movs r1, #1
	bl UiWork_Finalize
.L_0804b66a:
	ldr r0, [sp, #72]
	movs r1, #1
	bl UiWork_Finalize
	movs r1, #0
	ldr r0, [sp, #20]
	bl Func_08118088 + 0x58
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #228
	ldr r2, [r3]
	movs r3, #0
	str r3, [r2, #40]
	movs r0, #1
	bl WaitFrames
	ldr r0, [sp, #68]
	add sp, #324
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0804b69c:
	.4byte gInput
