.syntax unified
	.thumb
	.global FieldObject_UpdatePlayerControl
	.thumb_func
FieldObject_UpdatePlayerControl:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #104
	adds r7, r0, #0
	movs r0, #0
	str r0, [sp, #20]
	str r0, [sp, #16]
	ldr r5, .L_0800edf0
	ldrb r3, [r5]
	cmp r3, #0
	beq .L_0800ec40
	movs r0, #175
	lsls r0, r0, #1
	bl Func_080770c0
	cmp r0, #0
	beq .L_0800ec40
	movs r2, #128
	ldr r1, .L_0800edf4
	movs r0, #0
	lsls r2, r2, #2
.L_0800ec20:
	ldrb r3, [r1]
	adds r1, #1
	cmp r3, #255
	bne .L_0800ec2a
	adds r0, #1
.L_0800ec2a:
	subs r2, #1
	cmp r2, #0
	bne .L_0800ec20
	adds r3, r0, #0
	subs r3, #136
	cmp r3, #0
	bge .L_0800ec3e
	movs r0, #135
	bl Func_080f9010
.L_0800ec3e:
	ldr r5, .L_0800edf0
.L_0800ec40:
	ldrb r3, [r5]
	cmp r3, #0
	beq .L_0800ec76
	ldr r5, .L_0800edf8
	movs r2, #128
	ldr r3, [r5]
	lsls r2, r2, #2
	ands r3, r2
	cmp r3, #0
	beq .L_0800ec78
	adds r3, r2, #0
.L_0800ec56:
	subs r3, #1
	cmp r3, #0
	bne .L_0800ec56
	movs r3, #95
.L_0800ec5e:
	subs r3, #1
	cmp r3, #0
	bge .L_0800ec5e
	movs r3, #63
.L_0800ec66:
	subs r3, #1
	cmp r3, #0
	bge .L_0800ec66
	movs r3, #63
.L_0800ec6e:
	subs r3, #1
	cmp r3, #0
	bge .L_0800ec6e
	b .L_0800ec78
.L_0800ec76:
	ldr r5, .L_0800edf8
.L_0800ec78:
	ldr r3, .L_0800edfc
	movs r1, #135
	lsls r1, r1, #2
	adds r3, r3, r1
	ldrh r2, [r3]
	ldr r3, [r5]
	ands r3, r2
	cmp r3, #0
	beq .L_0800ec9c
	movs r3, #192
	lsls r3, r3, #9
	str r3, [r7, #48]
	movs r3, #128
	lsls r3, r3, #7
	movs r2, #5
	str r3, [r7, #52]
	str r2, [sp, #8]
	b .L_0800ecac
.L_0800ec9c:
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r7, #48]
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r7, #52]
	movs r3, #2
	str r3, [sp, #8]
.L_0800ecac:
	ldr r0, .L_0800ee00
	bl Func_080770c0
	cmp r0, #0
	beq .L_0800ecd4
	ldr r5, .L_0800edf8
	ldr r3, [r5]
	movs r2, #2
	ands r3, r2
	cmp r3, #0
	beq .L_0800ecd6
	movs r3, #128
	lsls r3, r3, #11
	str r3, [r7, #48]
	movs r3, #128
	lsls r3, r3, #9
	movs r0, #5
	str r3, [r7, #52]
	str r0, [sp, #8]
	b .L_0800ecd6
.L_0800ecd4:
	ldr r5, .L_0800edf8
.L_0800ecd6:
	ldr r3, [r5]
	movs r2, #15
	lsrs r3, r3, #4
	ldr r1, .L_0800ee04
	ands r3, r2
	lsls r3, r3, #1
	ldrsh r3, [r1, r3]
	lsls r3, r3, #16
	str r3, [sp, #4]
	lsrs r1, r3, #16
	ldr r3, .L_0800ee08
	cmp r1, r3
	bne .L_0800ecfa
	ldr r0, [sp, #20]
	movs r3, #4
	orrs r0, r3
	str r0, [sp, #20]
	b .L_0800f0c6
.L_0800ecfa:
	movs r2, #0
	movs r3, #92
	str r2, [sp, #20]
	add r3, sp
	mov r11, r3
	ldr r3, [r7, #8]
	mov r0, r11
	str r3, [r0]
	ldr r3, [r7, #12]
	str r3, [r0, #4]
	ldr r3, [r7, #16]
	str r3, [r0, #8]
	movs r0, #128
	lsls r0, r0, #12
	mov r2, r11
	bl Vector_AddPolarOffset
	ldr r3, .L_0800edf0
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_0800ed38
	ldr r3, [r5]
	movs r2, #128
	ldr r1, [sp, #4]
	lsls r2, r2, #2
	asrs r1, r1, #16
	ands r3, r2
	str r1, [sp, #12]
	cmp r3, #0
	beq .L_0800ed38
	b .L_0800f0c6
.L_0800ed38:
	adds r0, r7, #0
	mov r1, r11
	bl Func_080120dc
	cmp r0, #0
	bne .L_0800ee14
	ldr r3, [r7, #8]
	add r5, sp, #80
	str r3, [r5]
	ldr r3, [r7, #12]
	str r3, [r5, #4]
	ldr r3, [r7, #16]
	str r3, [r5, #8]
	ldr r3, [sp, #4]
	movs r2, #128
	lsls r2, r2, #12
	movs r0, #128
	mov r8, r2
	lsrs r6, r3, #16
	lsls r0, r0, #5
	adds r1, r6, r0
	adds r2, r5, #0
	mov r0, r8
	bl Vector_AddPolarOffset
	adds r0, r7, #0
	adds r1, r5, #0
	bl Func_080120dc
	cmp r0, #0
	bne .L_0800ee14
	ldr r3, [r7, #8]
	str r3, [r5]
	ldr r3, [r7, #12]
	str r3, [r5, #4]
	ldr r3, [r7, #16]
	ldr r2, .L_0800ee0c
	mov r0, r8
	adds r1, r6, r2
	str r3, [r5, #8]
	adds r2, r5, #0
	bl Vector_AddPolarOffset
	adds r0, r7, #0
	adds r1, r5, #0
	bl Func_080120dc
	cmp r0, #0
	bne .L_0800ee14
	ldr r3, [r7, #8]
	str r3, [r5]
	ldr r3, [r7, #12]
	str r3, [r5, #4]
	ldr r3, [r7, #16]
	str r3, [r5, #8]
	movs r3, #128
	lsls r3, r3, #6
	adds r1, r6, r3
	mov r0, r8
	adds r2, r5, #0
	bl Vector_AddPolarOffset
	adds r0, r7, #0
	adds r1, r5, #0
	bl Func_080120dc
	cmp r0, #0
	bne .L_0800ee14
	ldr r3, [r7, #8]
	str r3, [r5]
	ldr r3, [r7, #12]
	str r3, [r5, #4]
	ldr r3, [r7, #16]
	ldr r0, .L_0800ee10
	str r3, [r5, #8]
	adds r1, r6, r0
	adds r2, r5, #0
	mov r0, r8
	bl Vector_AddPolarOffset
	adds r0, r7, #0
	adds r1, r5, #0
	bl Func_080120dc
	cmp r0, #0
	bne .L_0800ee14
	ldr r1, [sp, #4]
	asrs r1, r1, #16
	lsls r2, r1, #16
	str r1, [sp, #12]
	str r2, [sp, #0]
	b .L_0800ef44
.L_0800edf0:
	.4byte gDebugMode
.L_0800edf4:
	.4byte ResourceBlockOwners
.L_0800edf8:
	.4byte Data_03001ae8
.L_0800edfc:
	.4byte gCell
.L_0800ee00:
	.4byte 0x0000017f
.L_0800ee04:
	.4byte Data_08013254
.L_0800ee08:
	.4byte 0x0000ffff
.L_0800ee0c:
	.4byte 0xfffff000
.L_0800ee10:
	.4byte 0xffffe000
.L_0800ee14:
	ldr r0, [sp, #4]
	add r3, sp, #24
	movs r1, #128
	mov r10, r3
	lsls r1, r1, #5
	lsrs r3, r0, #16
	adds r2, r3, r1
	ldr r1, .L_0800f188
	mov r0, r10
	strh r2, [r0]
	adds r2, r3, r1
	movs r1, #128
	lsls r1, r1, #6
	strh r2, [r0, #2]
	adds r2, r3, r1
	ldr r1, .L_0800f18c
	strh r2, [r0, #4]
	adds r2, r3, r1
	movs r1, #192
	lsls r1, r1, #6
	strh r2, [r0, #6]
	adds r2, r3, r1
	ldr r1, .L_0800f190
	strh r2, [r0, #8]
	adds r3, r3, r1
	mov r2, r10
	strh r3, [r2, #10]
	movs r3, #0
	mov r9, r3
	mov r8, r11
.L_0800ee50:
	mov r0, r9
	lsls r3, r0, #1
	mov r1, r10
	ldrsh r1, [r1, r3]
	str r1, [sp, #12]
	ldr r3, [r7, #8]
	mov r0, r8
	str r3, [r0]
	ldr r3, [r7, #12]
	str r3, [r0, #4]
	ldr r3, [r7, #16]
	lsls r1, r1, #16
	str r3, [r0, #8]
	lsrs r6, r1, #16
	movs r0, #128
	str r1, [sp, #0]
	lsls r0, r0, #12
	adds r1, r6, #0
	mov r2, r8
	bl Vector_AddPolarOffset
	adds r0, r7, #0
	mov r1, r8
	bl Func_080120dc
	cmp r0, #0
	bne .L_0800ef24
	ldr r3, [r7, #8]
	add r5, sp, #80
	str r3, [r5]
	ldr r3, [r7, #12]
	str r3, [r5, #4]
	ldr r3, [r7, #16]
	movs r2, #128
	lsls r2, r2, #5
	movs r0, #128
	adds r1, r6, r2
	lsls r0, r0, #12
	str r3, [r5, #8]
	adds r2, r5, #0
	bl Vector_AddPolarOffset
	adds r0, r7, #0
	adds r1, r5, #0
	bl Func_080120dc
	cmp r0, #0
	bne .L_0800ef24
	ldr r3, [r7, #8]
	str r3, [r5]
	ldr r3, [r7, #12]
	str r3, [r5, #4]
	ldr r3, [r7, #16]
	str r3, [r5, #8]
	ldr r3, .L_0800f188
	movs r0, #128
	adds r1, r6, r3
	lsls r0, r0, #12
	adds r2, r5, #0
	bl Vector_AddPolarOffset
	adds r0, r7, #0
	adds r1, r5, #0
	bl Func_080120dc
	cmp r0, #0
	bne .L_0800ef24
	ldr r3, [r7, #8]
	str r3, [r5]
	ldr r3, [r7, #12]
	movs r0, #128
	str r3, [r5, #4]
	ldr r3, [r7, #16]
	lsls r0, r0, #6
	adds r1, r6, r0
	movs r0, #128
	lsls r0, r0, #12
	str r3, [r5, #8]
	adds r2, r5, #0
	bl Vector_AddPolarOffset
	adds r0, r7, #0
	adds r1, r5, #0
	bl Func_080120dc
	cmp r0, #0
	bne .L_0800ef24
	ldr r3, [r7, #8]
	str r3, [r5]
	ldr r3, [r7, #12]
	str r3, [r5, #4]
	ldr r3, [r7, #16]
	ldr r2, .L_0800f18c
	movs r0, #128
	adds r1, r6, r2
	lsls r0, r0, #12
	str r3, [r5, #8]
	adds r2, r5, #0
	bl Vector_AddPolarOffset
	adds r0, r7, #0
	adds r1, r5, #0
	bl Func_080120dc
	cmp r0, #0
	beq .L_0800ef44
.L_0800ef24:
	movs r3, #1
	add r9, r3
	mov r0, r9
	cmp r0, #6
	blt .L_0800ee50
	ldr r3, [r7, #8]
	mov r1, r11
	str r3, [r1]
	ldr r3, [r7, #12]
	str r3, [r1, #4]
	ldr r3, [r7, #16]
	str r3, [r1, #8]
	ldr r2, [sp, #20]
	movs r3, #1
	orrs r2, r3
	str r2, [sp, #20]
.L_0800ef44:
	add r3, sp, #68
	mov r11, r3
	ldr r3, [r7, #8]
	mov r0, r11
	str r3, [r0]
	ldr r3, [r7, #12]
	str r3, [r0, #4]
	ldr r3, [r7, #16]
	str r3, [r0, #8]
	ldr r2, [sp, #0]
	movs r0, #128
	lsrs r1, r2, #16
	lsls r0, r0, #11
	mov r2, r11
	bl Vector_AddPolarOffset
	ldr r3, .L_0800f194
	ldr r3, [r3]
	mov r8, r3
	mov r6, r8
	movs r3, #63
	mov r9, r3
	adds r6, #8
.L_0800ef72:
	ldrh r3, [r7, #32]
	mov r0, r8
	subs r1, r3, #2
	ldr r3, [r0]
	cmp r3, #0
	bne .L_0800ef80
	b .L_0800f09a
.L_0800ef80:
	mov r3, r8
	adds r3, #89
	ldrb r2, [r3]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	bne .L_0800ef90
	b .L_0800f09a
.L_0800ef90:
	cmp r8, r7
	bne .L_0800ef96
	b .L_0800f09a
.L_0800ef96:
	ldrh r3, [r6, #24]
	adds r0, r6, #0
	subs r3, #2
	mov r2, r11
	bl Runtime_CheckRadiusOverlap
	cmp r0, #0
	blt .L_0800f09a
	ldr r3, [r6, #80]
	ldr r2, .L_0800f198
	movs r1, #128
	ands r3, r2
	lsls r1, r1, #2
	cmp r3, r1
	bne .L_0800f092
	ldr r3, [r7, #16]
	ldr r0, [r6, #8]
	ldr r1, [r6]
	subs r0, r0, r3
	ldr r3, [r7, #8]
	subs r1, r1, r3
	bl ArcTan2
	ldr r3, [r6]
	add r5, sp, #80
	str r3, [r5]
	ldr r3, [r6, #4]
	str r3, [r5, #4]
	lsls r0, r0, #16
	ldr r3, [r6, #8]
	asrs r2, r0, #16
	lsrs r0, r0, #16
	mov r10, r0
	movs r0, #128
	str r3, [r5, #8]
	lsls r0, r0, #7
	str r2, [sp, #12]
	mov r1, r10
	adds r2, r5, #0
	bl Vector_AddPolarOffset
	mov r0, r8
	adds r1, r5, #0
	bl ScriptObject_CheckOverlap
	cmp r0, #0
	bne .L_0800f092
	ldr r3, [r6]
	str r3, [r5]
	ldr r3, [r6, #4]
	str r3, [r5, #4]
	ldr r3, [r6, #8]
	movs r0, #160
	lsls r0, r0, #12
	mov r1, r10
	str r3, [r5, #8]
	adds r2, r5, #0
	bl Vector_AddPolarOffset
	mov r0, r8
	adds r1, r5, #0
	bl Func_080120dc
	cmp r0, #0
	bne .L_0800f092
	ldr r3, [r6]
	str r3, [r5]
	ldr r3, [r6, #4]
	str r3, [r5, #4]
	movs r1, #128
	ldr r3, [r6, #8]
	lsls r1, r1, #5
	movs r0, #160
	add r1, r10
	lsls r0, r0, #12
	str r3, [r5, #8]
	adds r2, r5, #0
	bl Vector_AddPolarOffset
	mov r0, r8
	adds r1, r5, #0
	bl Func_080120dc
	cmp r0, #0
	bne .L_0800f092
	mov r0, r8
	adds r1, r5, #0
	bl Func_080120dc
	cmp r0, #0
	bne .L_0800f092
	ldr r3, [r6]
	str r3, [r5]
	ldr r3, [r6, #4]
	str r3, [r5, #4]
	ldr r1, .L_0800f188
	ldr r3, [r6, #8]
	movs r0, #160
	add r1, r10
	lsls r0, r0, #12
	str r3, [r5, #8]
	adds r2, r5, #0
	bl Vector_AddPolarOffset
	mov r0, r8
	adds r1, r5, #0
	bl Func_080120dc
	cmp r0, #0
	bne .L_0800f092
	movs r0, #128
	lsls r0, r0, #7
	mov r1, r10
	adds r2, r6, #0
	bl Vector_AddPolarOffset
	movs r3, #128
	lsls r3, r3, #24
	str r3, [r6, #48]
	str r3, [r6, #52]
	str r3, [r6, #56]
	ldr r3, [sp, #16]
	movs r0, #1
	orrs r3, r0
	str r3, [sp, #16]
	b .L_0800f09a
.L_0800f092:
	ldr r1, [sp, #20]
	movs r3, #2
	orrs r1, r3
	str r1, [sp, #20]
.L_0800f09a:
	movs r2, #1
	negs r2, r2
	add r9, r2
	movs r3, #112
	mov r0, r9
	adds r6, #112
	add r8, r3
	cmp r0, #0
	blt .L_0800f0ae
	b .L_0800ef72
.L_0800f0ae:
	ldr r1, [sp, #20]
	cmp r1, #0
	bne .L_0800f0c6
	ldr r2, [sp, #16]
	cmp r2, #0
	beq .L_0800f0c6
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r7, #48]
	movs r3, #128
	lsls r3, r3, #6
	str r3, [r7, #52]
.L_0800f0c6:
	ldr r3, .L_0800f19c
	ldr r3, [r3]
	cmp r3, #0
	beq .L_0800f0ee
	ldr r0, [sp, #20]
	movs r2, #3
	ands r2, r0
	cmp r2, #0
	beq .L_0800f0e6
	movs r1, #206
	lsls r1, r1, #1
	adds r2, r3, r1
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_0800f0ee
.L_0800f0e6:
	movs r0, #206
	lsls r0, r0, #1
	adds r3, r3, r0
	strh r2, [r3]
.L_0800f0ee:
	ldr r1, [sp, #16]
	cmp r1, #0
	beq .L_0800f0fe
	adds r0, r7, #0
	movs r1, #8
	bl ObjectDispatch_ApplyArgumentToChildren
	b .L_0800f130
.L_0800f0fe:
	ldr r2, [sp, #20]
	cmp r2, #0
	beq .L_0800f128
	ldr r3, .L_0800f1a0
	movs r0, #250
	lsls r0, r0, #1
	adds r3, r3, r0
	ldr r0, [r3]
	bl Func_08077008
	movs r1, #56
	ldrsh r3, [r0, r1]
	movs r5, #9
	cmp r3, #0
	bne .L_0800f11e
	movs r5, #22
.L_0800f11e:
	adds r0, r7, #0
	adds r1, r5, #0
	bl ObjectDispatch_ApplyArgumentToChildren
	b .L_0800f130
.L_0800f128:
	adds r0, r7, #0
	ldr r1, [sp, #8]
	bl ObjectDispatch_ApplyArgumentToChildren
.L_0800f130:
	ldr r2, [sp, #20]
	cmp r2, #0
	beq .L_0800f1a4
	movs r3, #128
	lsls r3, r3, #24
	str r3, [r7, #56]
	str r3, [r7, #60]
	str r3, [r7, #64]
	movs r3, #0
	str r3, [r7, #36]
	str r3, [r7, #44]
	movs r3, #3
	ands r3, r2
	cmp r3, #0
	beq .L_0800f170
	ldr r0, [sp, #4]
	ldrh r1, [r7, #6]
	lsrs r3, r0, #16
	subs r3, r3, r1
	lsls r3, r3, #16
	movs r2, #128
	asrs r3, r3, #16
	lsls r2, r2, #5
	cmp r3, r2
	ble .L_0800f164
	adds r3, r2, #0
.L_0800f164:
	ldr r2, .L_0800f188
	cmp r3, r2
	bge .L_0800f16c
	adds r3, r2, #0
.L_0800f16c:
	adds r3, r1, r3
	strh r3, [r7, #6]
.L_0800f170:
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
	b .L_0800f1fa
	.2byte 0x0000
.L_0800f188:
	.4byte 0xfffff000
.L_0800f18c:
	.4byte 0xffffe000
.L_0800f190:
	.4byte 0xffffd000
.L_0800f194:
	.4byte gObjectSlots
.L_0800f198:
	.4byte 0xff000200
.L_0800f19c:
	.4byte Data_03001ebc
.L_0800f1a0:
	.4byte gCell
.L_0800f1a4:
	add r3, sp, #92
	ldr r1, [r3]
	ldr r2, [r3, #4]
	adds r0, r7, #0
	ldr r3, [r3, #8]
	bl Object_SetMoveTarget
	ldr r1, [r7, #36]
	ldr r4, .L_0800f2b4
	adds r0, r1, #0
	mov r12, pc
	bx r4
	ldr r1, [r7, #44]
	adds r3, r0, #0
	adds r0, r1, #0
	movs r0, r0
	mov r12, pc
	bx r4
	adds r3, r3, r0
	adds r0, r3, #0
	bl FixedSqrt
	ldr r3, [sp, #20]
	str r3, [r7, #36]
	str r3, [r7, #44]
	ldr r2, [sp, #12]
	lsls r1, r2, #16
	adds r2, r7, #0
	adds r2, #36
	lsrs r1, r1, #16
	bl Vector_AddPolarOffset
	movs r3, #100
	adds r3, r3, r7
	mov r10, r3
	ldrh r2, [r3]
	movs r0, #0
	ldrsh r3, [r3, r0]
	cmp r3, #0
	beq .L_0800f1fa
	subs r3, r2, #1
	mov r1, r10
	strh r3, [r1]
.L_0800f1fa:
	ldr r3, .L_0800f2b8
	ldr r3, [r3]
	ldr r2, [sp, #4]
	ldrb r3, [r3, #23]
	lsrs r2, r2, #16
	mov r8, r2
	cmp r3, #0
	beq .L_0800f2d8
	mov r1, r10
	movs r0, #0
	ldrsh r3, [r1, r0]
	cmp r3, #0
	bne .L_0800f2d8
	ldr r2, [sp, #20]
	cmp r2, #0
	bne .L_0800f2d8
	ldr r1, [r7, #8]
	ldr r2, [r7, #12]
	ldr r3, [r7, #16]
	movs r0, #25
	bl FieldObject_Create
	adds r6, r0, #0
	cmp r6, #0
	beq .L_0800f2d8
	ldr r3, [r7, #20]
	ldr r1, .L_0800f2bc
	str r3, [r6, #20]
	ldr r5, [r6, #80]
	bl ObjectDispatch_Initialize
	adds r2, r6, #0
	movs r3, #2
	adds r2, #35
	add r0, sp, #20
	strb r3, [r2]
	ldrb r0, [r0]
	adds r3, r6, #0
	adds r3, #85
	strb r0, [r3]
	cmp r5, #0
	beq .L_0800f270
	movs r1, #1
	adds r0, r5, #0
	bl AnimationObjects_SelectAnimation
	add r1, sp, #20
	adds r3, r5, #0
	ldrb r1, [r1]
	adds r3, #38
	strb r1, [r3]
	movs r3, #128
	lsls r3, r3, #7
	add r3, r8
	strh r3, [r5, #30]
	ldrb r3, [r5, #9]
	movs r2, #12
	orrs r3, r2
	strb r3, [r5, #9]
.L_0800f270:
	movs r2, #102
	adds r2, r2, r7
	mov r8, r2
	mov r1, r8
	movs r0, #0
	ldrsh r3, [r1, r0]
	ldrh r2, [r2]
	cmp r3, #2
	bne .L_0800f294
	adds r0, r5, #0
	movs r1, #2
	bl AnimationObjects_SelectAnimation
	add r2, sp, #20
	ldrh r2, [r2]
	mov r3, r8
	strh r2, [r3]
	ldr r2, .L_0800f2b0
.L_0800f294:
	lsls r3, r2, #16
	cmp r3, #0
	beq .L_0800f2a0
	movs r3, #128
	lsls r3, r3, #8
	strh r3, [r6, #6]
.L_0800f2a0:
	ldr r3, [sp, #8]
	cmp r3, #5
	bne .L_0800f2c0
	movs r3, #12
	mov r0, r10
	strh r3, [r0]
	b .L_0800f2c6
	.2byte 0x0000
.L_0800f2b0:
	.4byte 0x00000000
.L_0800f2b4:
	.4byte IwramMulQ16ReturnIp
.L_0800f2b8:
	.4byte gMapWork
.L_0800f2bc:
	.4byte Data_08013274
.L_0800f2c0:
	movs r3, #18
	mov r1, r10
	strh r3, [r1]
.L_0800f2c6:
	mov r2, r8
	ldrh r3, [r2]
	ldr r2, .L_0800f2d4
	mov r0, r8
	eors r3, r2
	strh r3, [r0]
	b .L_0800f2d8
.L_0800f2d4:
	.4byte 0x00000001
.L_0800f2d8:
	bl Field_CheckConfiguredKeys
	ldrh r3, [r7, #4]
	adds r3, #1
	movs r0, #1
	strh r3, [r7, #4]
	add sp, #104
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.2byte 0x0000
