.syntax unified
	.thumb
	.global Func_0811f66c
	.thumb_func
Func_0811f66c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #92
	str r0, [sp, #28]
	movs r3, #192
	movs r0, #0
	lsls r3, r3, #18
	adds r7, r1, #0
	ldr r1, [r3, #36]
	str r0, [sp, #20]
	mov r8, r0
	ldrb r3, [r7]
	cmp r3, #0
	beq .L_0811f76e
	movs r2, #0
	cmp r3, #2
	beq .L_0811f69c
	cmp r3, #4
	bne .L_0811f69e
.L_0811f69c:
	movs r2, #1
.L_0811f69e:
	ldr r3, [sp, #28]
	cmp r3, #7
	bls .L_0811f6aa
	cmp r2, #0
	beq .L_0811f6ae
	b .L_0811f710
.L_0811f6aa:
	cmp r2, #0
	beq .L_0811f710
.L_0811f6ae:
	movs r4, #0
	str r4, [sp, #24]
	movs r3, #88
	ldrsh r3, [r1, r3]
	cmp r3, #255
	beq .L_0811f76e
	mov r0, r8
	lsls r3, r0, #1
	add r2, sp, #92
	adds r3, r3, r2
	mov r4, r8
	adds r0, r3, #0
	lsls r3, r4, #2
	adds r3, r3, r2
	ldr r5, .L_0811f6ec
	adds r4, r3, #0
	adds r1, #88
	subs r0, #12
	subs r4, #36
.L_0811f6d4:
	movs r6, #0
	ldrsh r2, [r1, r6]
	cmp r2, #254
	beq .L_0811f6fe
	ldrb r3, [r7]
	cmp r3, #4
	bne .L_0811f6f0
	ldr r3, [sp, #28]
	cmp r2, r3
	bne .L_0811f6fe
	b .L_0811f6f0
	.2byte 0x0000
.L_0811f6ec:
	.4byte 0x00000100
.L_0811f6f0:
	stmia r4!, {r2}
	ldr r3, [sp, #24]
	movs r6, #1
	orrs r3, r5
	strh r3, [r0]
	add r8, r6
	adds r0, #2
.L_0811f6fe:
	ldr r2, [sp, #24]
	adds r1, #2
	adds r2, #1
	str r2, [sp, #24]
	movs r6, #0
	ldrsh r3, [r1, r6]
	cmp r3, #255
	bne .L_0811f6d4
	b .L_0811f76e
.L_0811f710:
	movs r0, #0
	str r0, [sp, #24]
	adds r2, r1, #2
	movs r3, #100
	ldrsh r3, [r2, r3]
	mov r12, r2
	cmp r3, #255
	beq .L_0811f76e
	mov r4, r8
	lsls r3, r4, #1
	add r6, sp, #92
	adds r3, r3, r6
	adds r1, r3, #0
	lsls r3, r4, #2
	adds r3, r3, r6
	ldr r5, .L_0811f74c
	adds r4, r3, #0
	movs r0, #100
	subs r1, #12
	subs r4, #36
.L_0811f738:
	ldrsh r2, [r2, r0]
	cmp r2, #254
	beq .L_0811f75e
	ldrb r3, [r7]
	cmp r3, #4
	bne .L_0811f750
	ldr r6, [sp, #28]
	cmp r2, r6
	bne .L_0811f75e
	b .L_0811f750
.L_0811f74c:
	.4byte 0x00000180
.L_0811f750:
	stmia r4!, {r2}
	ldr r3, [sp, #24]
	movs r2, #1
	orrs r3, r5
	strh r3, [r1]
	add r8, r2
	adds r1, #2
.L_0811f75e:
	ldr r3, [sp, #24]
	adds r0, #2
	adds r3, #1
	str r3, [sp, #24]
	mov r2, r12
	ldrsh r3, [r2, r0]
	cmp r3, #255
	bne .L_0811f738
.L_0811f76e:
	mov r0, r8
	cmp r0, #0
	bne .L_0811f77a
	movs r0, #2
	negs r0, r0
	b .L_0811fe28
.L_0811f77a:
	movs r1, #0
	str r1, [sp, #24]
	cmp r1, r8
	blt .L_0811f784
	b .L_0811fc6c
.L_0811f784:
	add r2, sp, #56
	mov r10, r2
.L_0811f788:
	ldr r3, [sp, #24]
	mov r4, r10
	lsls r3, r3, #2
	ldr r0, [r4, r3]
	mov r9, r3
	bl Owner_GetState
	adds r5, r0, #0
	ldrb r0, [r7, #3]
	movs r6, #0
	cmp r0, #88
	bls .L_0811f7a2
	b .L_0811fb9c
.L_0811f7a2:
	ldr r2, .L_0811fac8
	lsls r3, r0, #2
	ldr r3, [r3, r2]
	mov pc, r3
	.2byte 0x0000
.L_0811f7ac:
	.4byte .L_0811fb9e
	.4byte .L_0811fb9e
	.4byte .L_0811fb9e
	.4byte .L_0811f9c6
	.4byte .L_0811f9da
	.4byte .L_0811fb82
	.4byte .L_0811f910
	.4byte .L_0811f910
	.4byte .L_0811f936
	.4byte .L_0811f936
	.4byte .L_0811f94e
	.4byte .L_0811f94e
	.4byte .L_0811f96a
	.4byte .L_0811f96a
	.4byte .L_0811f98a
	.4byte .L_0811f98a
	.4byte .L_0811f9a6
	.4byte .L_0811f9a6
	.4byte .L_0811fb44
	.4byte .L_0811fb52
	.4byte .L_0811fb9c
	.4byte .L_0811fb9c
	.4byte .L_0811fb9c
	.4byte .L_0811fb64
	.4byte .L_0811fb6c
	.4byte .L_0811fb9c
	.4byte .L_0811fb74
	.4byte .L_0811fb9c
	.4byte .L_0811fb3c
	.4byte .L_0811fb9c
	.4byte .L_0811fb9c
	.4byte .L_0811fb9c
	.4byte .L_0811fb9c
	.4byte .L_0811fa46
	.4byte .L_0811fb9c
	.4byte .L_0811fb9c
	.4byte .L_0811fb9c
	.4byte .L_0811fb9c
	.4byte .L_0811fb9c
	.4byte .L_0811fb9c
	.4byte .L_0811fb9c
	.4byte .L_0811fb9c
	.4byte .L_0811fb9c
	.4byte .L_0811fb9c
	.4byte .L_0811fb9c
	.4byte .L_0811fb9c
	.4byte .L_0811fb9c
	.4byte .L_0811fb90
	.4byte .L_0811fb9c
	.4byte .L_0811fb9c
	.4byte .L_0811fb9c
	.4byte .L_0811fb9c
	.4byte .L_0811fb9c
	.4byte .L_0811fb9c
	.4byte .L_0811fb9c
	.4byte .L_0811fb9c
	.4byte .L_0811fb82
	.4byte .L_0811fb82
	.4byte .L_0811fb9c
	.4byte .L_0811fb9c
	.4byte .L_0811fb9c
	.4byte .L_0811fa32
	.4byte .L_0811fa32
	.4byte .L_0811fb9c
	.4byte .L_0811facc
	.4byte .L_0811fb9c
	.4byte .L_0811fb9c
	.4byte .L_0811fb9c
	.4byte .L_0811fb9c
	.4byte .L_0811fb9c
	.4byte .L_0811fa32
	.4byte .L_0811fa32
	.4byte .L_0811fb9c
	.4byte .L_0811fb82
	.4byte .L_0811fb9c
	.4byte .L_0811fb9c
	.4byte .L_0811fb9c
	.4byte .L_0811fb9c
	.4byte .L_0811fb9c
	.4byte .L_0811fb9c
	.4byte .L_0811fb9c
	.4byte .L_0811fb9c
	.4byte .L_0811fb9c
	.4byte .L_0811fb9c
	.4byte .L_0811fb9c
	.4byte .L_0811fb64
	.4byte .L_0811fb9c
	.4byte .L_0811fb9c
	.4byte .L_0811fb90
.L_0811f910:
	movs r1, #52
	adds r1, #255
	adds r3, r5, r1
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	adds r3, #1
	cmp r3, #4
	bgt .L_0811f924
.L_0811f922:
	movs r6, #1
.L_0811f924:
	movs r2, #153
	lsls r2, r2, #1
	adds r3, r5, r2
.L_0811f92a:
	ldrb r3, [r3]
	cmp r3, #1
	beq .L_0811f932
	b .L_0811fb9e
.L_0811f932:
	adds r6, #1
	b .L_0811fb9e
.L_0811f936:
	movs r4, #52
	adds r4, #255
	adds r3, r5, r4
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	movs r1, #4
	subs r3, #1
	negs r1, r1
	cmp r3, r1
	blt .L_0811f924
	b .L_0811f922
.L_0811f94e:
	movs r4, #54
	adds r4, #255
	adds r3, r5, r4
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	adds r3, #1
	cmp r3, #4
	bgt .L_0811f962
	movs r6, #1
.L_0811f962:
	movs r1, #154
	lsls r1, r1, #1
	adds r3, r5, r1
	b .L_0811f92a
.L_0811f96a:
	movs r2, #54
	adds r2, #255
	adds r3, r5, r2
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	movs r4, #4
	subs r3, #1
	negs r4, r4
	cmp r3, r4
	blt .L_0811f982
	movs r6, #1
.L_0811f982:
	movs r1, #154
	lsls r1, r1, #1
	adds r3, r5, r1
	b .L_0811f92a
.L_0811f98a:
	movs r2, #56
	adds r2, #255
	adds r3, r5, r2
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	adds r3, #1
	cmp r3, #4
	bgt .L_0811f99e
	movs r6, #1
.L_0811f99e:
	movs r4, #155
	lsls r4, r4, #1
	adds r3, r5, r4
	b .L_0811f92a
.L_0811f9a6:
	movs r1, #56
	adds r1, #255
	adds r3, r5, r1
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	movs r2, #4
	subs r3, #1
	negs r2, r2
	cmp r3, r2
	blt .L_0811f9be
	movs r6, #1
.L_0811f9be:
	movs r4, #155
	lsls r4, r4, #1
	adds r3, r5, r4
	b .L_0811f92a
.L_0811f9c6:
	movs r1, #50
	adds r1, #255
	adds r3, r5, r1
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	bne .L_0811f9d8
	b .L_0811fb9e
.L_0811f9d8:
	b .L_0811fb9c
.L_0811f9da:
	movs r2, #156
	lsls r2, r2, #1
	adds r3, r5, r2
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_0811f9e8
	movs r6, #1
.L_0811f9e8:
	movs r4, #58
	adds r4, #255
	adds r3, r5, r4
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_0811f9f6
	adds r6, #1
.L_0811f9f6:
	movs r1, #157
	lsls r1, r1, #1
	adds r3, r5, r1
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_0811fa04
	adds r6, #1
.L_0811fa04:
	movs r2, #158
	lsls r2, r2, #1
	adds r3, r5, r2
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_0811fa12
	adds r6, #1
.L_0811fa12:
	movs r4, #62
	adds r4, #255
	adds r3, r5, r4
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_0811fa20
	adds r6, #1
.L_0811fa20:
	movs r1, #66
	adds r1, #255
	adds r3, r5, r1
	ldrb r3, [r3]
.L_0811fa28:
	cmp r3, #0
	bne .L_0811fa2e
	b .L_0811fb9e
.L_0811fa2e:
	adds r6, #1
	b .L_0811fb9e
.L_0811fa32:
	movs r3, #56
	ldrsh r2, [r5, r3]
	movs r4, #52
	ldrsh r3, [r5, r4]
	ldrh r1, [r5, #56]
	cmp r2, r3
	blt .L_0811fa42
	b .L_0811fba0
.L_0811fa42:
	movs r6, #1
	b .L_0811fba0
.L_0811fa46:
	movs r1, #52
	adds r1, #255
	adds r3, r5, r1
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	ble .L_0811fa58
	movs r6, #1
.L_0811fa58:
	movs r2, #54
	adds r2, #255
	adds r3, r5, r2
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	ble .L_0811fa6a
	adds r6, #1
.L_0811fa6a:
	movs r4, #56
	adds r4, #255
	adds r3, r5, r4
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	ble .L_0811fa7c
	adds r6, #1
.L_0811fa7c:
	movs r1, #150
	lsls r1, r1, #1
	adds r3, r5, r1
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	ble .L_0811fa8e
	adds r6, #1
.L_0811fa8e:
	movs r2, #46
	adds r2, #255
	adds r3, r5, r2
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	ble .L_0811faa0
	adds r6, #1
.L_0811faa0:
	movs r4, #151
	lsls r4, r4, #1
	adds r3, r5, r4
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	ble .L_0811fab2
	adds r6, #1
.L_0811fab2:
	movs r1, #48
	adds r1, #255
	adds r3, r5, r1
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	ble .L_0811fb9e
	adds r6, #1
	b .L_0811fb9e
	.2byte 0x0000
.L_0811fac8:
	.4byte .L_0811f7ac
.L_0811facc:
	movs r2, #156
	lsls r2, r2, #1
	adds r3, r5, r2
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_0811fada
	movs r6, #1
.L_0811fada:
	movs r4, #58
	adds r4, #255
	adds r3, r5, r4
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_0811fae8
	adds r6, #1
.L_0811fae8:
	movs r1, #157
	lsls r1, r1, #1
	adds r3, r5, r1
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_0811faf6
	adds r6, #1
.L_0811faf6:
	movs r2, #158
	lsls r2, r2, #1
	adds r3, r5, r2
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_0811fb04
	adds r6, #1
.L_0811fb04:
	movs r4, #62
	adds r4, #255
	adds r3, r5, r4
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_0811fb12
	adds r6, #1
.L_0811fb12:
	movs r1, #66
	adds r1, #255
	adds r3, r5, r1
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_0811fb20
	adds r6, #1
.L_0811fb20:
	movs r2, #160
	lsls r2, r2, #1
	adds r3, r5, r2
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_0811fb2e
	adds r6, #1
.L_0811fb2e:
	movs r4, #50
	adds r4, #255
	adds r3, r5, r4
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	b .L_0811fa28
.L_0811fb3c:
	movs r1, #66
	adds r1, #255
	adds r3, r5, r1
	b .L_0811fb7a
.L_0811fb44:
	movs r2, #50
	adds r2, #255
	adds r3, r5, r2
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	b .L_0811fb7c
.L_0811fb52:
	movs r4, #50
	adds r4, #255
	adds r3, r5, r4
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #1
	bgt .L_0811fb9e
	b .L_0811fb9c
.L_0811fb64:
	movs r1, #60
	adds r1, #255
	adds r3, r5, r1
	b .L_0811fb7a
.L_0811fb6c:
	movs r2, #158
	lsls r2, r2, #1
	adds r3, r5, r2
	b .L_0811fb7a
.L_0811fb74:
	movs r4, #160
	lsls r4, r4, #1
	adds r3, r5, r4
.L_0811fb7a:
	ldrb r3, [r3]
.L_0811fb7c:
	cmp r3, #0
	bne .L_0811fb9e
	b .L_0811fb9c
.L_0811fb82:
	movs r2, #56
	ldrsh r3, [r5, r2]
	ldrh r1, [r5, #56]
	cmp r3, #0
	bne .L_0811fbb0
	movs r6, #100
	b .L_0811fba0
.L_0811fb90:
	movs r4, #44
	adds r4, #255
	adds r3, r5, r4
	ldrb r3, [r3]
	cmp r3, #1
	bhi .L_0811fb9e
.L_0811fb9c:
	movs r6, #1
.L_0811fb9e:
	ldrh r1, [r5, #56]
.L_0811fba0:
	lsls r3, r1, #16
	cmp r3, #0
	bne .L_0811fbb0
	bl BattleFx_IsReviveFar
	cmp r0, #0
	bne .L_0811fbb0
	movs r6, #0
.L_0811fbb0:
	cmp r6, #0
	bne .L_0811fc40
	ldrb r2, [r7, #1]
	movs r3, #15
	ands r3, r2
	subs r3, #1
	cmp r3, #10
	bhi .L_0811fc3c
	ldr r2, .L_0811fe38
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
.L_0811fbc8:
	.4byte .L_0811fc0c
	.4byte .L_0811fc2c
	.4byte .L_0811fbf4
	.4byte .L_0811fbf4
	.4byte .L_0811fbfa
	.4byte .L_0811fbfa
	.4byte .L_0811fc3a
	.4byte .L_0811fbfa
	.4byte .L_0811fc2c
	.4byte .L_0811fc06
	.4byte .L_0811fc1e
.L_0811fbf4:
	movs r0, #56
	ldrsh r3, [r5, r0]
	b .L_0811fc36
.L_0811fbfa:
	ldrh r3, [r7, #10]
	cmp r3, #0
	beq .L_0811fc3c
	movs r1, #56
	ldrsh r3, [r5, r1]
	b .L_0811fc36
.L_0811fc06:
	movs r2, #58
	ldrsh r3, [r5, r2]
	b .L_0811fc36
.L_0811fc0c:
	movs r3, #56
	ldrsh r2, [r5, r3]
	cmp r2, #0
	beq .L_0811fc3c
	movs r4, #52
	ldrsh r3, [r5, r4]
	cmp r2, r3
	bge .L_0811fc3c
	b .L_0811fc3a
.L_0811fc1e:
	movs r0, #58
	ldrsh r2, [r5, r0]
	movs r1, #54
	ldrsh r3, [r5, r1]
	cmp r2, r3
	bge .L_0811fc3c
	b .L_0811fc3a
.L_0811fc2c:
	ldrh r3, [r7, #10]
	cmp r3, #0
	beq .L_0811fc3c
	movs r2, #56
	ldrsh r3, [r5, r2]
.L_0811fc36:
	cmp r3, #0
	beq .L_0811fc3c
.L_0811fc3a:
	adds r6, #1
.L_0811fc3c:
	cmp r6, #0
	beq .L_0811fc60
.L_0811fc40:
	ldr r3, [sp, #20]
	mov r4, r10
	mov r6, r9
	lsls r1, r3, #2
	ldr r3, [r4, r6]
	add r2, sp, #80
	str r3, [r4, r1]
	ldr r4, [sp, #24]
	mov r0, sp
	lsls r3, r4, #1
	ldrh r3, [r2, r3]
	adds r0, #32
	str r3, [r0, r1]
	ldr r6, [sp, #20]
	adds r6, #1
	str r6, [sp, #20]
.L_0811fc60:
	ldr r0, [sp, #24]
	adds r0, #1
	str r0, [sp, #24]
	cmp r0, r8
	bge .L_0811fc6c
	b .L_0811f788
.L_0811fc6c:
	ldr r1, [sp, #20]
	cmp r1, #0
	bne .L_0811fc78
	movs r0, #1
	negs r0, r0
	b .L_0811fe28
.L_0811fc78:
	ldrb r3, [r7]
	mov r2, sp
	adds r2, #32
	str r2, [sp, #12]
	cmp r3, #1
	beq .L_0811fc86
	b .L_0811fe16
.L_0811fc86:
	ldrb r3, [r7, #8]
	cmp r3, #1
	beq .L_0811fc8e
	b .L_0811fe00
.L_0811fc8e:
	ldr r0, [sp, #28]
	bl Owner_GetState
	movs r4, #165
	lsls r4, r4, #1
	adds r3, r0, r4
	ldrh r0, [r3]
	bl Owner_GetRecordFar
	adds r0, #43
	movs r3, #0
	ldrsb r3, [r0, r3]
	cmp r3, #2
	bne .L_0811fcac
	b .L_0811fe08
.L_0811fcac:
	ldrb r2, [r7, #1]
	movs r3, #15
	ands r3, r2
	subs r3, #3
	cmp r3, #2
	bls .L_0811fcba
	b .L_0811fe10
.L_0811fcba:
	ldr r0, [sp, #20]
	movs r1, #1
	movs r6, #0
	negs r1, r1
	str r6, [sp, #24]
	cmp r6, r0
	bge .L_0811fd88
	mov r2, sp
	adds r2, #32
	subs r0, #1
	str r2, [sp, #12]
	str r0, [sp, #8]
.L_0811fcd2:
	ldr r3, [sp, #24]
	ldr r4, [sp, #8]
	cmp r3, r4
	bge .L_0811fd7a
	mov r6, sp
	lsls r3, r3, #2
	adds r6, #56
	adds r0, r3, r6
	str r6, [sp, #16]
	ldr r6, [sp, #24]
	adds r2, r3, #4
	adds r5, r3, #0
	ldr r7, [sp, #16]
	ldr r3, [sp, #12]
	str r0, [sp, #4]
	mov r10, r2
	subs r4, r4, r6
	mov r9, r3
	mov r11, r4
	add r7, r10
.L_0811fcfa:
	ldr r2, [sp, #16]
	str r1, [sp, #0]
	ldr r0, [r5, r2]
	bl Owner_GetState
	mov r8, r0
	ldr r0, [r7]
	bl Owner_GetState
	adds r6, r0, #0
	ldr r0, [sp, #28]
	bl Owner_GetState
	movs r4, #165
	lsls r4, r4, #1
	adds r3, r0, r4
	ldrh r0, [r3]
	bl Owner_GetRecordFar
	adds r0, #43
	movs r3, #0
	ldrsb r3, [r0, r3]
	ldr r1, [sp, #0]
	cmp r3, #0
	bne .L_0811fd38
	mov r2, r8
	movs r0, #56
	ldrsh r3, [r2, r0]
	movs r4, #56
	ldrsh r0, [r6, r4]
	b .L_0811fd42
.L_0811fd38:
	mov r2, r8
	movs r0, #52
	ldrsh r3, [r2, r0]
	movs r4, #52
	ldrsh r0, [r6, r4]
.L_0811fd42:
	cmp r3, r0
	bge .L_0811fd60
	ldr r6, [sp, #16]
	ldr r3, [r7]
	ldr r2, [r5, r6]
	ldr r0, [sp, #4]
	mov r4, r10
	str r3, [r0]
	str r2, [r7]
	mov r3, r9
	ldr r2, [r3, r5]
	ldr r3, [r3, r4]
	mov r6, r9
	str r3, [r6, r5]
	str r2, [r6, r4]
.L_0811fd60:
	ldr r0, [sp, #4]
	movs r3, #1
	negs r3, r3
	add r11, r3
	adds r0, #4
	movs r2, #4
	mov r4, r11
	str r0, [sp, #4]
	adds r7, #4
	add r10, r2
	adds r5, #4
	cmp r4, #0
	bne .L_0811fcfa
.L_0811fd7a:
	ldr r6, [sp, #24]
	ldr r0, [sp, #20]
	adds r6, #1
	str r6, [sp, #24]
	cmp r6, r0
	blt .L_0811fcd2
	b .L_0811fd8e
.L_0811fd88:
	mov r2, sp
	adds r2, #32
	str r2, [sp, #12]
.L_0811fd8e:
	ldr r3, [sp, #20]
	cmp r3, #2
	beq .L_0811fdaa
	cmp r3, #2
	bgt .L_0811fd9e
	cmp r3, #1
	beq .L_0811fde2
	b .L_0811fdf4
.L_0811fd9e:
	ldr r4, [sp, #20]
	cmp r4, #3
	beq .L_0811fdbe
	cmp r4, #4
	beq .L_0811fdd2
	b .L_0811fdf4
.L_0811fdaa:
	bl Random16
	movs r3, #11
	muls r3, r0
	lsrs r3, r3, #16
	movs r1, #0
	cmp r3, #5
	bls .L_0811fdf4
.L_0811fdba:
	movs r1, #1
	b .L_0811fdf4
.L_0811fdbe:
	bl Random16
	lsls r3, r0, #4
	subs r3, r3, r0
	lsrs r1, r3, #16
	cmp r1, #5
	ble .L_0811fde2
	cmp r1, #10
	bgt .L_0811fdee
	b .L_0811fdba
.L_0811fdd2:
	bl Random16
	lsls r3, r0, #3
	adds r3, r3, r0
	lsls r3, r3, #1
	lsrs r1, r3, #16
	cmp r1, #5
	bgt .L_0811fde6
.L_0811fde2:
	movs r1, #0
	b .L_0811fdf4
.L_0811fde6:
	cmp r1, #10
	ble .L_0811fdba
	cmp r1, #14
	bgt .L_0811fdf2
.L_0811fdee:
	movs r1, #2
	b .L_0811fdf4
.L_0811fdf2:
	movs r1, #3
.L_0811fdf4:
	cmp r1, #0
	blt .L_0811fe16
	ldr r6, [sp, #12]
	lsls r3, r1, #2
	ldr r0, [r6, r3]
	b .L_0811fe28
.L_0811fe00:
	mov r0, sp
	adds r0, #32
	str r0, [sp, #12]
	b .L_0811fe16
.L_0811fe08:
	mov r1, sp
	adds r1, #32
	str r1, [sp, #12]
	b .L_0811fe16
.L_0811fe10:
	mov r2, sp
	adds r2, #32
	str r2, [sp, #12]
.L_0811fe16:
	bl Random16
	ldr r4, [sp, #20]
	ldr r6, [sp, #12]
	adds r3, r4, #0
	muls r3, r0
	lsrs r3, r3, #16
	lsls r3, r3, #2
	ldr r0, [r6, r3]
.L_0811fe28:
	add sp, #92
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0811fe38:
	.4byte .L_0811fbc8
