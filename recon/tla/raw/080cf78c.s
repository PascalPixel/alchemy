.syntax unified
	.thumb
	.global Func_080cf78c
	.thumb_func
Func_080cf78c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r6, [r3, #32]
	ldr r3, [r3, #124]
	sub sp, #24
	movs r0, #160
	str r3, [sp, #20]
	lsls r0, r0, #3
	adds r0, #60
	adds r4, r3, r0
	movs r2, #0
	ldrsb r2, [r4, r2]
	cmp r2, #0
	bne .L_080cf7b8
	b .L_080cf8b8
.L_080cf7b8:
	movs r5, #160
	lsls r5, r5, #3
	adds r5, #61
	adds r1, r3, r5
	movs r3, #0
	ldrsb r3, [r1, r3]
	ldrb r0, [r1]
	cmp r3, r2
	blt .L_080cf874
	movs r3, #0
	strb r3, [r4]
	ldr r7, [sp, #20]
	movs r0, #160
	lsls r0, r0, #3
	adds r0, #62
	adds r3, r7, r0
	movs r2, #0
	ldrsb r2, [r3, r2]
	cmp r2, #0
	bne .L_080cf868
	movs r1, #160
	lsls r1, r1, #3
	adds r1, #59
	adds r3, r7, r1
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #64
	bne .L_080cf7fc
	movs r1, #128
	lsls r1, r1, #19
	ldrh r2, [r1]
	movs r3, #129
	b .L_080cf804
.L_080cf7fc:
	movs r1, #128
	lsls r1, r1, #19
	ldrh r2, [r1]
	movs r3, #159
.L_080cf804:
	lsls r3, r3, #8
	adds r3, #255
	ands r3, r2
	strh r3, [r1]
	movs r3, #128
	lsls r3, r3, #19
	ldrh r2, [r3]
	movs r3, #128
	lsls r3, r3, #8
	ands r3, r2
	cmp r3, #0
	beq .L_080cf82a
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #74
	ldrh r2, [r3]
	ldr r1, .L_080cf85c
	orrs r2, r1
	strh r2, [r3]
.L_080cf82a:
	ldr r0, .L_080cf860
	bl Scheduler_RemoveCallback
	ldr r0, .L_080cf864
	bl Scheduler_RemoveCallback
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #176
	ldrh r1, [r2, #10]
	movs r3, #197
	lsls r3, r3, #8
	adds r3, #255
	ands r3, r1
	strh r3, [r2, #10]
	movs r3, #254
	ldrh r1, [r2, #10]
	lsls r3, r3, #7
	adds r3, #255
	ands r3, r1
	strh r3, [r2, #10]
	ldrh r3, [r2, #10]
	bl .L_080d00e2
	.2byte 0x0000
.L_080cf85c:
	.4byte 0x0000001f
.L_080cf860:
	.4byte Func_080cf6fc
.L_080cf864:
	.4byte Func_080cf78c
.L_080cf868:
	ldr r5, [sp, #20]
	movs r7, #165
	lsls r7, r7, #3
	adds r3, r5, r7
	strh r2, [r3]
	b .L_080cf8b8
.L_080cf874:
	ldr r2, [sp, #20]
	movs r5, #160
	lsls r5, r5, #3
	adds r5, #59
	adds r3, r2, r5
	movs r2, #0
	ldrsb r2, [r3, r2]
	ldr r7, [sp, #20]
	movs r3, #160
	lsls r3, r3, #3
	adds r3, #58
	adds r5, r7, r3
	movs r3, #0
	ldrsb r3, [r5, r3]
	subs r2, r2, r3
	adds r3, r0, #1
	strb r3, [r1]
	lsls r3, r3, #24
	asrs r3, r3, #24
	adds r0, r3, #0
	muls r0, r2
	movs r1, #0
	ldrsb r1, [r4, r1]
	ldr r3, .L_080cfa64
	mov lr, r3
	.2byte 0xf800
	movs r3, #0
	ldrsb r3, [r5, r3]
	movs r5, #160
	lsls r5, r5, #3
	adds r5, #42
	adds r3, r3, r0
	adds r2, r7, r5
	strh r3, [r2]
.L_080cf8b8:
	ldr r7, [sp, #20]
	movs r0, #160
	lsls r0, r0, #3
	adds r0, #57
	adds r3, r7, r0
	ldrb r3, [r3]
	movs r2, #1
	eors r2, r3
	lsls r3, r2, #2
	adds r3, r3, r2
	lsls r3, r3, #5
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r4, r7, r3
	adds r0, r4, #4
	str r4, [sp, #0]
	bl Func_08038258
	movs r1, #165
	lsls r1, r1, #3
	adds r3, r7, r1
	ldrh r3, [r3]
	ldr r4, [sp, #0]
	cmp r3, #77
	bls .L_080cf8ee
	bl .L_080d00d0
.L_080cf8ee:
	ldr r2, .L_080cfa68
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
	.2byte 0x0000
.L_080cf8f8:
	.4byte .L_080cfa30
	.4byte .L_080cfaa0
	.4byte .L_080cfb72
	.4byte .L_080cfbe4
	.4byte .L_080cfc76
	.4byte .L_080d00d0
	.4byte .L_080d00d0
	.4byte .L_080cfcde
	.4byte .L_080cfdd2
	.4byte .L_080cfef4
	.4byte .L_080d00d0
	.4byte .L_080d00d0
	.4byte .L_080d00d0
	.4byte .L_080d00d0
	.4byte .L_080d00d0
	.4byte .L_080d00d0
	.4byte .L_080d00d0
	.4byte .L_080d00d0
	.4byte .L_080d00d0
	.4byte .L_080d00d0
	.4byte .L_080d00d0
	.4byte .L_080d00d0
	.4byte .L_080d00d0
	.4byte .L_080d00d0
	.4byte .L_080d00d0
	.4byte .L_080d00d0
	.4byte .L_080d00d0
	.4byte .L_080d00d0
	.4byte .L_080d00d0
	.4byte .L_080d00d0
	.4byte .L_080d00d0
	.4byte .L_080d00d0
	.4byte .L_080d00d0
	.4byte .L_080d00d0
	.4byte .L_080d00d0
	.4byte .L_080d00d0
	.4byte .L_080d00d0
	.4byte .L_080d00d0
	.4byte .L_080d00d0
	.4byte .L_080d00d0
	.4byte .L_080d00d0
	.4byte .L_080d00d0
	.4byte .L_080d00d0
	.4byte .L_080d00d0
	.4byte .L_080d00d0
	.4byte .L_080d00d0
	.4byte .L_080d00d0
	.4byte .L_080d00d0
	.4byte .L_080d00d0
	.4byte .L_080d00d0
	.4byte .L_080d00d0
	.4byte .L_080d00d0
	.4byte .L_080d00d0
	.4byte .L_080d00d0
	.4byte .L_080d00d0
	.4byte .L_080d00d0
	.4byte .L_080d00d0
	.4byte .L_080d00d0
	.4byte .L_080d00d0
	.4byte .L_080d00d0
	.4byte .L_080d00d0
	.4byte .L_080d00d0
	.4byte .L_080d00d0
	.4byte .L_080d00d0
	.4byte .L_080d00d0
	.4byte .L_080d00d0
	.4byte .L_080d00d0
	.4byte .L_080d00d0
	.4byte .L_080d00d0
	.4byte .L_080d00d0
	.4byte .L_080d00d0
	.4byte .L_080d00d0
	.4byte .L_080d00d0
	.4byte .L_080d00d0
	.4byte .L_080d00d0
	.4byte .L_080d00d0
	.4byte .L_080d00d0
	.4byte .L_080cffec
.L_080cfa30:
	ldr r3, .L_080cfa5c
	movs r5, #160
	strh r3, [r4]
	ldr r3, .L_080cfa60
	adds r4, #2
	strh r3, [r4]
	ldr r2, [sp, #20]
	lsls r5, r5, #3
	adds r5, #42
	adds r3, r2, r5
	ldrh r5, [r3]
	movs r2, #32
	adds r3, r5, #0
	ands r3, r2
	adds r4, #2
	cmp r3, #0
	beq .L_080cfa6c
	movs r3, #31
	ands r3, r5
	subs r5, r2, r3
	b .L_080cfa70
	.2byte 0x0000
.L_080cfa5c:
	.4byte 0x00007f7f
.L_080cfa60:
	.4byte 0x00000001
.L_080cfa64:
	.4byte IwramSignedDivide
.L_080cfa68:
	.4byte .L_080cf8f8
.L_080cfa6c:
	movs r3, #31
	ands r5, r3
.L_080cfa70:
	ldr r3, .L_080cfb98
	movs r7, #0
	ldrb r5, [r3, r5]
	movs r3, #241
	mov r8, r7
	subs r6, r3, r5
.L_080cfa7c:
	str r4, [sp, #0]
	bl Random16
	adds r3, r6, #0
	muls r3, r0
	ldr r4, [sp, #0]
	lsrs r3, r3, #16
	movs r0, #1
	lsls r2, r3, #8
	add r8, r0
	adds r3, r3, r5
	orrs r2, r3
	mov r1, r8
	strh r2, [r4]
	adds r4, #4
	cmp r1, #159
	bls .L_080cfa7c
	b .L_080d00d0
.L_080cfaa0:
	ldr r2, [sp, #20]
	movs r5, #160
	lsls r5, r5, #3
	adds r5, #42
	adds r3, r2, r5
	ldrh r5, [r3]
	movs r3, #31
	ands r3, r5
	lsls r2, r3, #3
	subs r2, r2, r3
	ldr r3, .L_080cfb9c
	lsls r2, r2, #2
	adds r2, r2, r3
	movs r3, #32
	ands r3, r5
	cmp r3, #0
	beq .L_080cfacc
	ldrh r3, [r2]
	strh r3, [r4]
	adds r4, #2
	ldrh r3, [r2, #2]
	b .L_080cfad4
.L_080cfacc:
	ldrh r3, [r2, #2]
	strh r3, [r4]
	adds r4, #2
	ldrh r3, [r2]
.L_080cfad4:
	strh r3, [r4]
	adds r4, #2
	movs r7, #0
	adds r2, #4
	mov r8, r7
	mov r10, r2
.L_080cfae0:
	mov r2, r10
	ldrh r7, [r2]
	ldrh r0, [r2, #2]
	cmp r7, #0
	beq .L_080cfb62
	cmp r0, #0
	beq .L_080cfb04
	movs r1, #0
	mov r9, r1
	cmp r9, r7
	bge .L_080cfb62
.L_080cfaf6:
	movs r2, #1
	add r9, r2
	strh r0, [r4]
	adds r4, #4
	cmp r9, r7
	blt .L_080cfaf6
	b .L_080cfb62
.L_080cfb04:
	ldrb r3, [r2, #4]
	ldrb r6, [r2, #6]
	mov r11, r3
	ldrb r0, [r2, #7]
	ldrb r3, [r2, #5]
	cmp r7, #0
	beq .L_080cfb62
	mov r5, r11
	subs r5, r3, r5
	subs r0, r0, r6
	str r5, [sp, #16]
	str r0, [sp, #12]
	movs r2, #0
	movs r3, #0
	mov r9, r7
.L_080cfb22:
	adds r0, r3, #0
	adds r1, r7, #0
	str r2, [sp, #8]
	str r3, [sp, #4]
	str r4, [sp, #0]
	bl Math_Div
	ldr r2, [sp, #8]
	adds r5, r0, #0
	adds r1, r7, #0
	adds r0, r2, #0
	bl Math_Div
	add r5, r11
	ldr r4, [sp, #0]
	adds r0, r6, r0
	lsls r5, r5, #8
	adds r5, r5, r0
	strh r5, [r4]
	ldr r0, [sp, #12]
	ldr r2, [sp, #8]
	movs r5, #1
	ldr r3, [sp, #4]
	ldr r1, [sp, #16]
	negs r5, r5
	add r9, r5
	adds r2, r2, r0
	mov r0, r9
	adds r4, #4
	adds r3, r3, r1
	cmp r0, #0
	bne .L_080cfb22
.L_080cfb62:
	movs r2, #1
	add r8, r2
	movs r1, #8
	mov r3, r8
	add r10, r1
	cmp r3, #2
	bls .L_080cfae0
	b .L_080d00d0
.L_080cfb72:
	ldr r5, [sp, #20]
	movs r7, #160
	lsls r7, r7, #3
	adds r7, #42
	adds r3, r5, r7
	ldrh r3, [r3]
	subs r5, r3, #1
	movs r3, #32
	ands r3, r5
	cmp r3, #0
	beq .L_080cfba0
	ldr r3, .L_080cfb90
	strh r3, [r4]
	ldr r3, .L_080cfb94
	b .L_080cfba6
.L_080cfb90:
	.4byte 0x00000001
.L_080cfb94:
	.4byte 0x00007f7f
.L_080cfb98:
	.4byte Data_080f01c4
.L_080cfb9c:
	.4byte Data_080f2e9c
.L_080cfba0:
	ldr r3, .L_080cfbcc
	strh r3, [r4]
	ldr r3, .L_080cfbd0
.L_080cfba6:
	adds r4, #2
	strh r3, [r4]
	adds r4, #2
	movs r3, #31
	ands r5, r3
	movs r0, #0
	mov r8, r0
	lsls r5, r5, #4
.L_080cfbb6:
	str r4, [sp, #0]
	bl Random16
	lsls r0, r0, #4
	lsrs r0, r0, #16
	adds r0, r5, r0
	ldr r4, [sp, #0]
	cmp r0, #255
	bls .L_080cfbd4
	movs r0, #255
	b .L_080cfbd4
.L_080cfbcc:
	.4byte 0x00007f7f
.L_080cfbd0:
	.4byte 0x00000001
.L_080cfbd4:
	movs r1, #1
	add r8, r1
	mov r2, r8
	strh r0, [r4]
	adds r4, #4
	cmp r2, #159
	bls .L_080cfbb6
	b .L_080d00d0
.L_080cfbe4:
	ldr r5, [sp, #20]
	movs r7, #160
	lsls r7, r7, #3
	adds r7, #42
	adds r3, r5, r7
	ldr r2, .L_080cfc20
	ldrh r5, [r3]
	ldr r3, .L_080cfc24
	strh r2, [r4]
	adds r4, #2
	strh r3, [r4]
	adds r4, #2
	cmp r5, #32
	bls .L_080cfc0c
	strh r3, [r4]
	adds r4, #2
	movs r3, #64
	strh r2, [r4]
	subs r5, r3, r5
	adds r4, #2
.L_080cfc0c:
	lsls r3, r5, #2
	adds r5, r3, r5
	adds r3, r5, #0
	muls r3, r5
	ldr r7, .L_080cfc28
	lsls r3, r3, #16
	movs r0, #0
	mov r11, r3
	mov r8, r0
	b .L_080cfc2c
.L_080cfc20:
	.4byte 0x00007f7f
.L_080cfc24:
	.4byte 0x00000001
.L_080cfc28:
	.4byte IwramFillWords + 0x74
.L_080cfc2c:
	mov r5, r8
	subs r5, #80
	adds r0, r5, #0
	muls r0, r5
	mov r1, r11
	lsls r0, r0, #16
	str r4, [sp, #0]
	subs r0, r1, r0
	mov lr, r7
	.2byte 0xf800
	movs r3, #120
	asrs r0, r0, #8
	subs r6, r3, r0
	ldr r4, [sp, #0]
	adds r0, #120
	cmp r6, #0
	bge .L_080cfc50
	movs r6, #0
.L_080cfc50:
	cmp r0, #0
	bge .L_080cfc56
	movs r0, #0
.L_080cfc56:
	cmp r6, #240
	ble .L_080cfc5c
	movs r6, #240
.L_080cfc5c:
	cmp r0, #240
	ble .L_080cfc62
	movs r0, #240
.L_080cfc62:
	lsls r3, r6, #8
	movs r2, #1
	adds r3, r3, r0
	add r8, r2
	strh r3, [r4]
	mov r3, r8
	adds r4, #4
	cmp r3, #159
	bls .L_080cfc2c
	b .L_080d00d0
.L_080cfc76:
	ldr r5, [sp, #20]
	movs r7, #160
	lsls r7, r7, #3
	adds r7, #42
	adds r3, r5, r7
	ldrh r5, [r3]
	movs r3, #32
	ands r3, r5
	cmp r3, #0
	beq .L_080cfc9c
	ldr r3, .L_080cfc94
	strh r3, [r4]
	ldr r3, .L_080cfc98
	b .L_080cfca2
	.2byte 0x0000
.L_080cfc94:
	.4byte 0x00000001
.L_080cfc98:
	.4byte 0x00007f7f
.L_080cfc9c:
	ldr r3, .L_080cfcc4
	strh r3, [r4]
	ldr r3, .L_080cfcc8
.L_080cfca2:
	adds r4, #2
	strh r3, [r4]
	adds r4, #2
	movs r3, #31
	ands r3, r5
	lsls r2, r3, #4
	subs r2, r2, r3
	lsls r2, r2, #4
	lsrs r5, r2, #5
	movs r3, #240
	subs r3, r3, r5
	movs r0, #0
	lsls r3, r3, #8
	mov r8, r0
	adds r3, #240
	b .L_080cfccc
	.2byte 0x0000
.L_080cfcc4:
	.4byte 0x00007f7f
.L_080cfcc8:
	.4byte 0x00000001
.L_080cfccc:
	movs r1, #2
	add r8, r1
	mov r2, r8
	strh r5, [r4]
	strh r3, [r4, #4]
	adds r4, #8
	cmp r2, #159
	bls .L_080cfccc
	b .L_080d00d0
.L_080cfcde:
	adds r2, r6, #0
	adds r2, #228
	ldr r3, .L_080cffdc
	ldr r5, [r2]
	ldr r6, [r2, #4]
	ands r5, r3
	ands r6, r3
	str r4, [sp, #0]
	bl Func_080cdf5c
	bl ObjectTable_Get
	ldr r3, [r0, #8]
	ldr r4, [sp, #0]
	subs r3, r3, r5
	cmp r3, #0
	bge .L_080cfd08
	movs r5, #255
	lsls r5, r5, #8
	adds r5, #255
	adds r3, r3, r5
.L_080cfd08:
	ldr r2, [r0, #12]
	asrs r7, r3, #16
	ldr r3, [r0, #16]
	subs r3, r3, r2
	subs r0, r3, r6
	cmp r0, #0
	bge .L_080cfd1e
	movs r1, #255
	lsls r1, r1, #8
	adds r1, #255
	adds r0, r0, r1
.L_080cfd1e:
	ldr r2, [sp, #20]
	movs r5, #160
	asrs r3, r0, #16
	lsls r5, r5, #3
	subs r3, #16
	adds r5, #54
	mov r10, r3
	adds r3, r2, r5
	ldrh r3, [r3]
	movs r0, #160
	strh r3, [r4]
	lsls r0, r0, #3
	adds r0, #52
	adds r3, r2, r0
	ldrh r3, [r3]
	adds r4, #2
	movs r1, #160
	strh r3, [r4]
	lsls r1, r1, #3
	adds r1, #42
	adds r3, r2, r1
	ldrh r5, [r3]
	movs r2, #32
	adds r3, r5, #0
	ands r3, r2
	adds r4, #2
	cmp r3, #0
	bne .L_080cfd5e
	movs r3, #31
	ands r3, r5
	subs r5, r2, r3
	b .L_080cfd62
.L_080cfd5e:
	movs r3, #31
	ands r5, r3
.L_080cfd62:
	ldr r3, .L_080cffe0
	movs r2, #1
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	beq .L_080cfd70
	movs r5, #0
.L_080cfd70:
	lsls r3, r5, #2
	adds r5, r3, r5
	adds r3, r5, #0
	muls r3, r5
	lsls r3, r3, #16
	mov r11, r3
	ldr r3, .L_080cffe4
	movs r2, #0
	mov r8, r2
	mov r9, r3
.L_080cfd84:
	mov r0, r8
	mov r1, r10
	subs r5, r0, r1
	adds r3, r5, #0
	muls r3, r5
	lsls r0, r3, #1
	adds r0, r0, r3
	mov r2, r11
	lsls r0, r0, #15
	str r4, [sp, #0]
	subs r0, r2, r0
	mov lr, r9
	.2byte 0xf800
	asrs r0, r0, #8
	subs r6, r7, r0
	ldr r4, [sp, #0]
	adds r0, r7, r0
	cmp r6, #0
	bge .L_080cfdac
	movs r6, #0
.L_080cfdac:
	cmp r0, #0
	bge .L_080cfdb2
	movs r0, #0
.L_080cfdb2:
	cmp r6, #240
	ble .L_080cfdb8
	movs r6, #240
.L_080cfdb8:
	cmp r0, #240
	ble .L_080cfdbe
	movs r0, #240
.L_080cfdbe:
	lsls r3, r6, #8
	adds r3, r3, r0
	strh r3, [r4]
	movs r3, #1
	add r8, r3
	mov r5, r8
	adds r4, #4
	cmp r5, #159
	bls .L_080cfd84
	b .L_080d00d0
.L_080cfdd2:
	adds r1, r6, #0
	adds r1, #228
	ldr r3, .L_080cffdc
	ldr r2, [r1]
	ldr r7, [sp, #20]
	ldr r1, [r1, #4]
	movs r0, #160
	lsls r0, r0, #3
	adds r0, #44
	ands r2, r3
	ands r1, r3
	adds r3, r7, r0
	ldr r3, [r3]
	subs r3, r3, r2
	cmp r3, #0
	bge .L_080cfdfa
	movs r2, #255
	lsls r2, r2, #8
	adds r2, #255
	adds r3, r3, r2
.L_080cfdfa:
	ldr r5, [sp, #20]
	movs r0, #166
	lsls r0, r0, #3
	asrs r7, r3, #16
	adds r3, r5, r0
	ldr r3, [r3]
	subs r1, r3, r1
	cmp r1, #0
	bge .L_080cfe14
	movs r2, #255
	lsls r2, r2, #8
	adds r2, #255
	adds r1, r1, r2
.L_080cfe14:
	asrs r3, r1, #16
	subs r3, #16
	lsls r2, r3, #1
	mov r10, r3
	ldr r3, .L_080cffe0
	ldr r5, [sp, #20]
	ldr r3, [r3]
	movs r0, #160
	lsls r0, r0, #3
	subs r3, r3, r2
	adds r0, #52
	mov r9, r3
	adds r3, r5, r0
	ldrh r3, [r3]
	movs r1, #160
	strh r3, [r4]
	lsls r1, r1, #3
	adds r1, #54
	adds r3, r5, r1
	ldrh r3, [r3]
	adds r4, #2
	movs r2, #160
	strh r3, [r4]
	lsls r2, r2, #3
	adds r2, #42
	adds r3, r5, r2
	ldrh r5, [r3]
	movs r2, #32
	adds r3, r5, #0
	ands r3, r2
	adds r4, #2
	cmp r3, #0
	beq .L_080cfe5e
	movs r3, #31
	ands r3, r5
	subs r5, r2, r3
	b .L_080cfe62
.L_080cfe5e:
	movs r3, #31
	ands r5, r3
.L_080cfe62:
	lsls r3, r5, #2
	adds r5, r3, r5
	adds r3, r5, #0
	muls r3, r5
	lsls r3, r3, #16
	mov r11, r3
	movs r3, #0
	mov r8, r3
.L_080cfe72:
	mov r5, r8
	mov r0, r10
	movs r1, #120
	subs r3, r5, r0
	negs r1, r1
	cmp r3, r1
	bge .L_080cfe84
	movs r3, #120
	negs r3, r3
.L_080cfe84:
	cmp r3, #120
	ble .L_080cfe8a
	movs r3, #120
.L_080cfe8a:
	adds r2, r3, #0
	muls r2, r3
	adds r3, r2, #0
	lsls r0, r3, #1
	adds r0, r0, r3
	lsls r0, r0, #15
	mov r3, r11
	subs r0, r3, r0
	str r4, [sp, #0]
	ldr r3, .L_080cffe4
	mov lr, r3
	.2byte 0xf800
	asrs r0, r0, #8
	subs r6, r7, r0
	adds r0, r7, r0
	ldr r4, [sp, #0]
	cmp r6, r0
	bge .L_080cfec4
	ldr r2, .L_080cffe8
	movs r3, #31
	mov r5, r9
	ands r3, r5
	ldrsb r3, [r2, r3]
	subs r6, r6, r3
	adds r0, r0, r3
	cmp r6, r0
	blt .L_080cfec4
	movs r6, #240
	movs r0, #240
.L_080cfec4:
	cmp r6, #0
	bge .L_080cfeca
	movs r6, #0
.L_080cfeca:
	cmp r0, #0
	bge .L_080cfed0
	movs r0, #0
.L_080cfed0:
	cmp r6, #240
	ble .L_080cfed6
	movs r6, #240
.L_080cfed6:
	cmp r0, #240
	ble .L_080cfedc
	movs r0, #240
.L_080cfedc:
	movs r1, #1
	lsls r3, r6, #8
	add r8, r1
	adds r3, r3, r0
	mov r2, r8
	movs r0, #2
	strh r3, [r4]
	add r9, r0
	adds r4, #4
	cmp r2, #159
	bls .L_080cfe72
	b .L_080d00d0
.L_080cfef4:
	adds r2, r6, #0
	adds r2, #228
	ldr r3, .L_080cffdc
	ldr r5, [r2]
	ldr r6, [r2, #4]
	ands r5, r3
	ands r6, r3
	str r4, [sp, #0]
	bl Func_080cdf5c
	bl ObjectTable_Get
	ldr r3, [r0, #8]
	ldr r4, [sp, #0]
	subs r3, r3, r5
	cmp r3, #0
	bge .L_080cff1e
	movs r5, #255
	lsls r5, r5, #8
	adds r5, #255
	adds r3, r3, r5
.L_080cff1e:
	ldr r2, [r0, #12]
	asrs r7, r3, #16
	ldr r3, [r0, #16]
	subs r3, r3, r2
	subs r0, r3, r6
	cmp r0, #0
	bge .L_080cff34
	movs r1, #255
	lsls r1, r1, #8
	adds r1, #255
	adds r0, r0, r1
.L_080cff34:
	ldr r2, [sp, #20]
	movs r5, #160
	asrs r3, r0, #16
	lsls r5, r5, #3
	subs r3, #16
	adds r5, #52
	mov r10, r3
	adds r3, r2, r5
	ldrh r3, [r3]
	movs r0, #160
	strh r3, [r4]
	lsls r0, r0, #3
	adds r0, #54
	adds r3, r2, r0
	ldrh r3, [r3]
	adds r4, #2
	movs r1, #160
	strh r3, [r4]
	lsls r1, r1, #3
	adds r1, #42
	adds r3, r2, r1
	ldrh r5, [r3]
	movs r2, #32
	adds r3, r5, #0
	ands r3, r2
	adds r4, #2
	cmp r3, #0
	beq .L_080cff74
	movs r3, #31
	ands r3, r5
	subs r5, r2, r3
	b .L_080cff78
.L_080cff74:
	movs r3, #31
	ands r5, r3
.L_080cff78:
	lsls r3, r5, #2
	adds r5, r3, r5
	adds r3, r5, #0
	muls r3, r5
	lsls r3, r3, #16
	mov r11, r3
	ldr r3, .L_080cffe4
	movs r2, #0
	mov r8, r2
	mov r9, r3
.L_080cff8c:
	mov r0, r8
	mov r1, r10
	subs r5, r0, r1
	adds r3, r5, #0
	muls r3, r5
	lsls r0, r3, #1
	adds r0, r0, r3
	mov r2, r11
	lsls r0, r0, #15
	str r4, [sp, #0]
	subs r0, r2, r0
	mov lr, r9
	.2byte 0xf800
	asrs r0, r0, #8
	subs r6, r7, r0
	ldr r4, [sp, #0]
	adds r0, r7, r0
	cmp r6, #0
	bge .L_080cffb4
	movs r6, #0
.L_080cffb4:
	cmp r0, #0
	bge .L_080cffba
	movs r0, #0
.L_080cffba:
	cmp r6, #240
	ble .L_080cffc0
	movs r6, #240
.L_080cffc0:
	cmp r0, #240
	ble .L_080cffc6
	movs r0, #240
.L_080cffc6:
	lsls r3, r6, #8
	adds r3, r3, r0
	strh r3, [r4]
	movs r3, #1
	add r8, r3
	mov r5, r8
	adds r4, #4
	cmp r5, #159
	bls .L_080cff8c
	b .L_080d00d0
	.2byte 0x0000
.L_080cffdc:
	.4byte 0xffff0000
.L_080cffe0:
	.4byte Data_0300122c
.L_080cffe4:
	.4byte IwramFillWords + 0x74
.L_080cffe8:
	.4byte Data_080f01e6
.L_080cffec:
	adds r2, r6, #0
	adds r2, #228
	ldr r3, .L_080d00f0
	ldr r5, [r2]
	ldr r6, [r2, #4]
	ands r5, r3
	ands r6, r3
	str r4, [sp, #0]
	bl Func_080cdf5c
	bl ObjectTable_Get
	ldr r3, [r0, #8]
	ldr r4, [sp, #0]
	subs r3, r3, r5
	cmp r3, #0
	bge .L_080d0016
	movs r7, #255
	lsls r7, r7, #8
	adds r7, #255
	adds r3, r3, r7
.L_080d0016:
	ldr r2, [r0, #12]
	asrs r7, r3, #16
	ldr r3, [r0, #16]
	subs r3, r3, r2
	subs r0, r3, r6
	cmp r0, #0
	bge .L_080d002c
	movs r1, #255
	lsls r1, r1, #8
	adds r1, #255
	adds r0, r0, r1
.L_080d002c:
	ldr r2, [sp, #20]
	movs r5, #160
	asrs r3, r0, #16
	lsls r5, r5, #3
	subs r3, #8
	adds r5, #52
	mov r10, r3
	adds r3, r2, r5
	ldrh r3, [r3]
	movs r0, #160
	strh r3, [r4]
	lsls r0, r0, #3
	adds r0, #54
	adds r3, r2, r0
	ldrh r3, [r3]
	adds r4, #2
	movs r1, #160
	strh r3, [r4]
	lsls r1, r1, #3
	adds r1, #42
	adds r3, r2, r1
	ldrh r5, [r3]
	movs r2, #32
	adds r3, r5, #0
	ands r3, r2
	adds r4, #2
	cmp r3, #0
	beq .L_080d006c
	movs r3, #31
	ands r3, r5
	subs r5, r2, r3
	b .L_080d0070
.L_080d006c:
	movs r3, #31
	ands r5, r3
.L_080d0070:
	lsls r3, r5, #2
	adds r5, r3, r5
	adds r3, r5, #0
	muls r3, r5
	lsls r3, r3, #16
	mov r11, r3
	ldr r3, .L_080d00f4
	movs r2, #0
	mov r8, r2
	mov r9, r3
.L_080d0084:
	mov r0, r8
	mov r1, r10
	subs r5, r0, r1
	adds r3, r5, #0
	muls r3, r5
	lsls r0, r3, #1
	adds r0, r0, r3
	mov r2, r11
	lsls r0, r0, #14
	str r4, [sp, #0]
	subs r0, r2, r0
	mov lr, r9
	.2byte 0xf800
	asrs r0, r0, #8
	subs r6, r7, r0
	ldr r4, [sp, #0]
	adds r0, r7, r0
	cmp r6, #0
	bge .L_080d00ac
	movs r6, #0
.L_080d00ac:
	cmp r0, #0
	bge .L_080d00b2
	movs r0, #0
.L_080d00b2:
	cmp r6, #240
	ble .L_080d00b8
	movs r6, #240
.L_080d00b8:
	cmp r0, #240
	ble .L_080d00be
	movs r0, #240
.L_080d00be:
	lsls r3, r6, #8
	adds r3, r3, r0
	strh r3, [r4]
	movs r3, #1
	add r8, r3
	mov r5, r8
	adds r4, #4
	cmp r5, #159
	bls .L_080d0084
.L_080d00d0:
	ldr r7, [sp, #20]
	movs r0, #160
	lsls r0, r0, #3
	adds r0, #57
	adds r3, r7, r0
	ldrb r2, [r3]
	movs r1, #1
	eors r2, r1
	strb r2, [r3]
.L_080d00e2:
	add sp, #24
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_080d00f0:
	.4byte 0xffff0000
.L_080d00f4:
	.4byte IwramFillWords + 0x74
