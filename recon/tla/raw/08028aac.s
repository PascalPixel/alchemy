.syntax unified
	.thumb
	.global Func_08028aac
	.thumb_func
Func_08028aac:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	adds r7, r0, #0
	movs r1, #8
	adds r1, r1, r7
	mov r8, r1
	sub sp, #96
	movs r0, #0
	str r0, [sp, #4]
	mov r0, r8
	bl GetWorldMapCollision
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #108]
	subs r3, r0, #1
	cmp r3, #2
	bhi .L_08028ae8
	movs r2, #208
	add r4, sp, #4
	lsls r2, r2, #4
	ldrb r4, [r4]
	adds r2, #58
	adds r3, r1, r2
	strb r4, [r3]
.L_08028ae8:
	subs r3, r0, #5
	cmp r3, #1
	bhi .L_08028afa
	movs r3, #208
	lsls r3, r3, #4
	adds r3, #58
	adds r2, r1, r3
	movs r3, #1
	strb r3, [r2]
.L_08028afa:
	cmp r0, #4
	bne .L_08028b0a
	movs r4, #208
	lsls r4, r4, #4
	adds r4, #58
	adds r2, r1, r4
	movs r3, #2
	strb r3, [r2]
.L_08028b0a:
	subs r3, r0, #7
	cmp r3, #5
	bhi .L_08028b1c
	movs r0, #208
	lsls r0, r0, #4
	adds r0, #58
	adds r2, r1, r0
	movs r3, #3
	strb r3, [r2]
.L_08028b1c:
	add r1, sp, #4
	ldrb r1, [r1]
	adds r3, r7, #0
	adds r3, #85
	movs r2, #0
	strb r1, [r3]
	str r2, [sp, #12]
	ldr r0, [r7, #12]
	adds r3, r0, #0
	cmp r0, #0
	bge .L_08028b34
	adds r3, #15
.L_08028b34:
	movs r4, #160
	lsls r4, r4, #9
	asrs r3, r3, #4
	adds r3, r3, r4
	str r3, [r7, #48]
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r7, #52]
	ldr r5, .L_08028e90
	ldr r1, .L_08028e94
	ldr r3, [r5]
	movs r2, #15
	lsrs r3, r3, #4
	ands r3, r2
	lsls r3, r3, #1
	ldrsh r2, [r1, r3]
	movs r1, #255
	str r2, [sp, #8]
	lsls r1, r1, #8
	lsls r2, r2, #16
	lsrs r6, r2, #16
	adds r1, #255
	mov r11, r2
	cmp r6, r1
	bne .L_08028b74
	movs r2, #4
	str r2, [sp, #12]
	b .L_08028dd4
.L_08028b6c:
	mov r3, r11
	asrs r3, r3, #16
	str r3, [sp, #8]
	b .L_08028d88
.L_08028b74:
	mov r4, sp
	adds r4, #84
	str r4, [sp, #0]
	mov r1, r8
	ldr r3, [r1]
	str r0, [r4, #4]
	str r3, [r4]
	ldr r3, [r7, #16]
	movs r2, #128
	lsls r2, r2, #12
	str r3, [r4, #8]
	mov r10, r2
	mov r0, r10
	adds r1, r6, #0
	ldr r2, [sp, #0]
	bl Vector_AddPolarOffset
	ldr r3, .L_08028e98
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_08028bac
	ldr r3, [r5]
	movs r2, #128
	lsls r2, r2, #2
	ands r3, r2
	cmp r3, #0
	beq .L_08028bac
	b .L_08028dd4
.L_08028bac:
	adds r0, r7, #0
	ldr r1, [sp, #0]
	bl Func_0802dbd0
	cmp r0, #0
	bne .L_08028c58
	mov r4, r8
	ldr r3, [r4]
	add r5, sp, #72
	str r3, [r5]
	ldr r3, [r7, #12]
	movs r0, #128
	str r3, [r5, #4]
	ldr r3, [r7, #16]
	lsls r0, r0, #5
	adds r1, r6, r0
	str r3, [r5, #8]
	mov r0, r10
	adds r2, r5, #0
	bl Vector_AddPolarOffset
	adds r0, r7, #0
	adds r1, r5, #0
	bl Func_0802dbd0
	cmp r0, #0
	bne .L_08028c58
	mov r1, r8
	ldr r3, [r1]
	ldr r2, .L_08028e9c
	str r3, [r5]
	ldr r3, [r7, #12]
	adds r1, r6, r2
	str r3, [r5, #4]
	ldr r3, [r7, #16]
	mov r0, r10
	str r3, [r5, #8]
	adds r2, r5, #0
	bl Vector_AddPolarOffset
	adds r0, r7, #0
	adds r1, r5, #0
	bl Func_0802dbd0
	cmp r0, #0
	bne .L_08028c58
	mov r4, r8
	ldr r3, [r4]
	movs r0, #128
	str r3, [r5]
	ldr r3, [r7, #12]
	lsls r0, r0, #6
	str r3, [r5, #4]
	ldr r3, [r7, #16]
	adds r1, r6, r0
	str r3, [r5, #8]
	mov r0, r10
	adds r2, r5, #0
	bl Vector_AddPolarOffset
	adds r0, r7, #0
	adds r1, r5, #0
	bl Func_0802dbd0
	cmp r0, #0
	bne .L_08028c58
	mov r1, r8
	ldr r3, [r1]
	ldr r2, .L_08028ea0
	str r3, [r5]
	ldr r3, [r7, #12]
	adds r1, r6, r2
	str r3, [r5, #4]
	ldr r3, [r7, #16]
	mov r0, r10
	str r3, [r5, #8]
	adds r2, r5, #0
	bl Vector_AddPolarOffset
	adds r0, r7, #0
	adds r1, r5, #0
	bl Func_0802dbd0
	cmp r0, #0
	bne .L_08028c58
	b .L_08028d88
.L_08028c58:
	add r3, sp, #16
	mov r4, r11
	mov r9, r3
	movs r0, #128
	lsrs r3, r4, #16
	ldr r4, .L_08028e9c
	lsls r0, r0, #5
	adds r2, r3, r0
	mov r1, r9
	strh r2, [r1]
	mov r0, r9
	adds r2, r3, r4
	strh r2, [r0, #2]
	movs r1, #128
	ldr r0, .L_08028ea0
	lsls r1, r1, #6
	adds r2, r3, r1
	mov r4, r9
	strh r2, [r4, #4]
	mov r1, r9
	adds r2, r3, r0
	strh r2, [r1, #6]
	movs r4, #192
	ldr r1, .L_08028ea4
	lsls r4, r4, #6
	adds r2, r3, r4
	mov r0, r9
	strh r2, [r0, #8]
	adds r3, r3, r1
	mov r2, r9
	strh r3, [r2, #10]
	ldr r0, [sp, #0]
	movs r3, #6
	str r3, [sp, #4]
	movs r4, #0
	mov r10, r4
	mov r8, r0
.L_08028ca2:
	mov r1, r10
	lsls r3, r1, #1
	mov r4, r9
	ldrsh r2, [r4, r3]
	ldr r3, [r7, #8]
	mov r1, r8
	str r3, [r1]
	ldr r3, [r7, #12]
	lsls r2, r2, #16
	str r3, [r1, #4]
	ldr r3, [r7, #16]
	lsrs r6, r2, #16
	movs r0, #128
	str r3, [r1, #8]
	lsls r0, r0, #12
	adds r1, r6, #0
	mov r11, r2
	mov r2, r8
	bl Vector_AddPolarOffset
	adds r0, r7, #0
	mov r1, r8
	bl Func_0802dbd0
	cmp r0, #0
	bne .L_08028d76
	ldr r3, [r7, #8]
	add r5, sp, #72
	str r3, [r5]
	ldr r3, [r7, #12]
	movs r2, #128
	str r3, [r5, #4]
	ldr r3, [r7, #16]
	lsls r2, r2, #5
	movs r0, #128
	adds r1, r6, r2
	lsls r0, r0, #12
	str r3, [r5, #8]
	adds r2, r5, #0
	bl Vector_AddPolarOffset
	adds r0, r7, #0
	adds r1, r5, #0
	bl Func_0802dbd0
	cmp r0, #0
	bne .L_08028d76
	ldr r3, [r7, #8]
	movs r0, #128
	str r3, [r5]
	ldr r3, [r7, #12]
	lsls r0, r0, #12
	str r3, [r5, #4]
	ldr r3, [r7, #16]
	adds r2, r5, #0
	str r3, [r5, #8]
	ldr r3, .L_08028e9c
	adds r1, r6, r3
	bl Vector_AddPolarOffset
	adds r0, r7, #0
	adds r1, r5, #0
	bl Func_0802dbd0
	cmp r0, #0
	bne .L_08028d76
	ldr r3, [r7, #8]
	movs r4, #128
	str r3, [r5]
	ldr r3, [r7, #12]
	lsls r4, r4, #6
	str r3, [r5, #4]
	ldr r3, [r7, #16]
	movs r0, #128
	adds r1, r6, r4
	lsls r0, r0, #12
	str r3, [r5, #8]
	adds r2, r5, #0
	bl Vector_AddPolarOffset
	adds r0, r7, #0
	adds r1, r5, #0
	bl Func_0802dbd0
	cmp r0, #0
	bne .L_08028d76
	ldr r3, [r7, #8]
	ldr r0, .L_08028ea0
	str r3, [r5]
	ldr r3, [r7, #12]
	adds r1, r6, r0
	str r3, [r5, #4]
	ldr r3, [r7, #16]
	movs r0, #128
	lsls r0, r0, #12
	str r3, [r5, #8]
	adds r2, r5, #0
	bl Vector_AddPolarOffset
	adds r0, r7, #0
	adds r1, r5, #0
	bl Func_0802dbd0
	cmp r0, #0
	bne .L_08028d76
	b .L_08028b6c
.L_08028d76:
	ldr r2, [sp, #4]
	movs r1, #1
	add r10, r1
	cmp r10, r2
	blt .L_08028ca2
	ldr r4, [sp, #12]
	movs r3, #1
	orrs r4, r3
	str r4, [sp, #12]
.L_08028d88:
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #20]
	movs r0, #63
	adds r6, r5, #0
	mov r10, r0
	adds r6, #89
.L_08028d96:
	ldr r3, [r5]
	cmp r3, #0
	beq .L_08028dc4
	ldrb r2, [r6]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	beq .L_08028dc4
	cmp r5, r7
	beq .L_08028dc4
	adds r0, r5, #0
	adds r0, #8
	ldrh r1, [r5, #32]
	ldrh r3, [r7, #32]
	ldr r2, [sp, #0]
	bl Func_08026f80
	cmp r0, #0
	blt .L_08028dc4
	ldr r1, [sp, #12]
	movs r3, #2
	orrs r1, r3
	str r1, [sp, #12]
.L_08028dc4:
	movs r2, #1
	negs r2, r2
	add r10, r2
	mov r3, r10
	adds r6, #128
	adds r5, #128
	cmp r3, #0
	bge .L_08028d96
.L_08028dd4:
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #108]
	cmp r1, #0
	beq .L_08028e0a
	ldr r4, [sp, #12]
	movs r2, #3
	ands r2, r4
	cmp r2, #0
	beq .L_08028df6
	movs r0, #194
	lsls r0, r0, #1
	adds r2, r1, r0
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_08028dfe
.L_08028df6:
	movs r4, #194
	lsls r4, r4, #1
	adds r3, r1, r4
	strh r2, [r3]
.L_08028dfe:
	ldr r3, .L_08028e90
	movs r0, #195
	ldr r3, [r3]
	lsls r0, r0, #1
	adds r2, r1, r0
	strh r3, [r2]
.L_08028e0a:
	ldr r3, [r7, #12]
	movs r1, #128
	lsls r1, r1, #11
	cmp r3, r1
	blt .L_08028e1e
	adds r0, r7, #0
	movs r1, #6
	bl ObjectDispatch_ApplyArgumentToChildren
	b .L_08028e36
.L_08028e1e:
	ldr r2, [sp, #12]
	cmp r2, #0
	beq .L_08028e2e
	adds r0, r7, #0
	movs r1, #1
	bl ObjectDispatch_ApplyArgumentToChildren
	b .L_08028e36
.L_08028e2e:
	adds r0, r7, #0
	movs r1, #2
	bl ObjectDispatch_ApplyArgumentToChildren
.L_08028e36:
	ldr r3, [sp, #12]
	cmp r3, #0
	beq .L_08028ea8
	movs r3, #128
	lsls r3, r3, #24
	str r3, [r7, #56]
	str r3, [r7, #60]
	str r3, [r7, #64]
	movs r3, #0
	str r3, [r7, #36]
	str r3, [r7, #44]
	ldr r4, [sp, #12]
	movs r3, #3
	ands r3, r4
	cmp r3, #0
	beq .L_08028e7a
	ldr r0, [sp, #8]
	ldrh r2, [r7, #6]
	lsls r3, r0, #16
	lsrs r3, r3, #16
	subs r3, r3, r2
	lsls r3, r3, #16
	asrs r1, r3, #16
	movs r3, #128
	lsls r3, r3, #5
	cmp r1, r3
	ble .L_08028e6e
	adds r1, r3, #0
.L_08028e6e:
	ldr r3, .L_08028e9c
	cmp r1, r3
	bge .L_08028e76
	adds r1, r3, #0
.L_08028e76:
	adds r3, r2, r1
	strh r3, [r7, #6]
.L_08028e7a:
	movs r1, #100
	adds r1, r1, r7
	mov r10, r1
	movs r3, #0
	mov r2, r10
	strh r3, [r2]
	adds r2, r7, #0
	adds r2, #102
	movs r3, #2
	strh r3, [r2]
	b .L_08028f2a
.L_08028e90:
	.4byte gInput
.L_08028e94:
	.4byte Data_0802ec5c
.L_08028e98:
	.4byte Data_03001238
.L_08028e9c:
	.4byte 0xfffff000
.L_08028ea0:
	.4byte 0xffffe000
.L_08028ea4:
	.4byte 0xffffd000
.L_08028ea8:
	add r3, sp, #84
	ldr r2, [r3, #4]
	ldr r1, [r3]
	adds r0, r7, #0
	ldr r3, [r3, #8]
	bl Object_SetMoveTarget
	ldr r1, [r7, #36]
	ldr r6, .L_08028fbc
	adds r0, r1, #0
	mov lr, r6
	.2byte 0xf800
	ldr r1, [r7, #44]
	adds r5, r0, #0
	adds r0, r1, #0
	mov lr, r6
	.2byte 0xf800
	adds r5, r5, r0
	adds r0, r5, #0
	bl Func_080149e0
	ldr r3, [sp, #12]
	adds r2, r7, #0
	str r3, [r7, #36]
	str r3, [r7, #44]
	ldr r4, [sp, #8]
	adds r2, #36
	lsls r3, r4, #16
	lsrs r5, r3, #16
	adds r1, r5, #0
	bl Vector_AddPolarOffset
	movs r0, #100
	adds r0, r0, r7
	movs r1, #0
	ldrsh r3, [r0, r1]
	mov r10, r0
	ldrh r2, [r0]
	cmp r3, #0
	beq .L_08028efe
	subs r3, r2, #1
	mov r2, r10
	strh r3, [r2]
.L_08028efe:
	adds r1, r7, #0
	adds r1, #90
	ldrb r3, [r1]
	movs r2, #254
	ands r2, r3
	strb r2, [r1]
	ldrh r2, [r7, #6]
	subs r3, r5, r2
	lsls r3, r3, #16
	asrs r1, r3, #16
	movs r3, #128
	lsls r3, r3, #5
	cmp r1, r3
	ble .L_08028f1e
	movs r1, #128
	lsls r1, r1, #3
.L_08028f1e:
	ldr r4, .L_08028fc0
	cmp r1, r4
	bge .L_08028f26
	ldr r1, .L_08028fc4
.L_08028f26:
	adds r3, r2, r1
	strh r3, [r7, #6]
.L_08028f2a:
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #32]
	ldr r3, .L_08028fc8
	ldr r1, .L_08028fcc
	ldr r3, [r3]
	movs r2, #15
	lsrs r3, r3, #4
	ands r3, r2
	lsls r3, r3, #2
	ldr r0, [r1, r3]
	ldr r1, [r7, #12]
	cmp r1, #0
	bge .L_08028f48
	adds r1, #3
.L_08028f48:
	movs r2, #128
	lsls r2, r2, #9
	asrs r1, r1, #2
	adds r1, r1, r2
	ldr r3, .L_08028fbc
	mov lr, r3
	.2byte 0xf800
	movs r3, #143
	lsls r3, r3, #1
	adds r1, r5, r3
	ldrh r4, [r1]
	subs r3, r0, r4
	lsls r3, r3, #16
	asrs r3, r3, #16
	cmp r3, #0
	bge .L_08028f6a
	adds r3, #7
.L_08028f6a:
	asrs r2, r3, #3
	cmp r2, #32
	ble .L_08028f72
	movs r2, #32
.L_08028f72:
	movs r3, #32
	negs r3, r3
	cmp r2, r3
	bge .L_08028f7c
	adds r2, r3, #0
.L_08028f7c:
	adds r3, r2, #1
	cmp r3, #2
	bhi .L_08028f86
	ldrh r3, [r1]
	subs r2, r0, r3
.L_08028f86:
	adds r3, r4, r2
	strh r3, [r1]
	adds r3, r7, #0
	adds r3, #84
	ldr r4, .L_08028fb8
	ldrb r3, [r3]
	mov r9, r4
	mov r8, r3
	cmp r3, #1
	bne .L_0802903a
	ldr r3, [r7, #12]
	movs r0, #128
	lsls r0, r0, #11
	cmp r3, r0
	bge .L_0802902c
	mov r2, r10
	movs r1, #0
	ldrsh r3, [r2, r1]
	cmp r3, #0
	bne .L_0802903a
	ldr r3, [sp, #12]
	cmp r3, #0
	bne .L_0802903a
	b .L_08028fd0
	.2byte 0x0000
.L_08028fb8:
	.4byte 0x00000000
.L_08028fbc:
	.4byte IwramMulQ16
.L_08028fc0:
	.4byte 0xfffff000
.L_08028fc4:
	.4byte 0xfffffc00
.L_08028fc8:
	.4byte gInput
.L_08028fcc:
	.4byte Data_0802eca0
.L_08028fd0:
	movs r0, #14
	adds r0, #255
	ldr r1, [r7, #8]
	ldr r3, [r7, #16]
	movs r2, #0
	bl Func_08023220
	adds r5, r0, #0
	cmp r5, #0
	beq .L_0802903a
	ldr r1, .L_080290e0
	ldr r6, [r5, #80]
	bl ObjectDispatch_Initialize
	adds r3, r5, #0
	adds r3, #85
	mov r4, r9
	strb r4, [r3]
	mov r0, r8
	subs r3, #51
	strb r0, [r3]
	cmp r6, #0
	beq .L_08029024
	movs r1, #1
	adds r0, r6, #0
	bl Animation_ApplyChildArgument
	mov r1, r9
	strb r1, [r6, #26]
	movs r2, #13
	ldrb r1, [r6, #5]
	negs r2, r2
	adds r3, r2, #0
	ands r3, r1
	movs r1, #4
	orrs r3, r1
	strb r3, [r6, #5]
	ldrb r3, [r6, #9]
	ands r2, r3
	movs r3, #8
	orrs r2, r3
	strb r2, [r6, #9]
.L_08029024:
	movs r3, #10
	mov r2, r10
	strh r3, [r2]
	b .L_0802903a
.L_0802902c:
	ldr r3, .L_080290e4
	adds r0, r7, #0
	ldr r1, [r3]
	mov r3, r8
	ands r1, r3
	bl ObjectDispatch_SetSingleChildField1a
.L_0802903a:
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #108]
	movs r1, #128
	ldr r3, .L_080290e8
	movs r4, #140
	lsls r1, r1, #2
	adds r1, #50
	ldr r0, .L_080290ec
	lsls r4, r4, #2
	adds r2, r3, r4
	adds r3, r3, r1
	ldrh r1, [r2]
	ldrh r2, [r3]
	ldr r3, [r0]
	orrs r2, r1
	ands r3, r2
	cmp r3, #0
	beq .L_08029076
	movs r3, #208
	lsls r3, r3, #4
	adds r3, #59
	adds r2, r5, r3
	movs r3, #1
	strb r3, [r2]
	ldr r3, [r7, #12]
	movs r4, #128
	lsls r4, r4, #6
	adds r3, r3, r4
	b .L_0802907c
.L_08029076:
	ldr r3, [r7, #12]
	ldr r0, .L_080290f0
	adds r3, r3, r0
.L_0802907c:
	str r3, [r7, #12]
	bl Func_080ad318
	cmp r0, #0
	bne .L_0802908e
	ldr r3, [r7, #12]
	ldr r1, .L_080290f0
	adds r3, r3, r1
	str r3, [r7, #12]
.L_0802908e:
	ldr r2, [r7, #12]
	cmp r2, #0
	bgt .L_080290b2
	movs r4, #208
	lsls r4, r4, #4
	adds r4, #59
	adds r3, r5, r4
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	beq .L_080290b2
	movs r0, #181
	lsls r0, r0, #1
	adds r2, r5, r0
	movs r3, #254
	strh r3, [r2]
	ldr r2, [r7, #12]
.L_080290b2:
	movs r1, #192
	lsls r1, r1, #12
	cmp r2, r1
	blt .L_080290c2
	movs r3, #192
	lsls r3, r3, #12
	str r3, [r7, #12]
	adds r2, r3, #0
.L_080290c2:
	cmp r2, #0
	bgt .L_080290ca
	movs r3, #0
	str r3, [r7, #12]
.L_080290ca:
	ldrh r3, [r7, #4]
	movs r0, #1
	adds r3, #1
	strh r3, [r7, #4]
	add sp, #96
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_080290e0:
	.4byte Data_0802ec94
.L_080290e4:
	.4byte Data_0300122c
.L_080290e8:
	.4byte gPartyState
.L_080290ec:
	.4byte gInput
.L_080290f0:
	.4byte 0xffffc000
