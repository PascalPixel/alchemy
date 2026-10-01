.syntax unified
	.thumb
	.global ScriptObject_WanderNearHome
	.thumb_func
ScriptObject_WanderNearHome:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	adds r6, r0, #0
	movs r0, #4
	ldrsh r2, [r6, r0]
	ldr r3, [r6]
	lsls r2, r2, #2
	adds r3, r3, r2
	adds r3, #4
	ldmia r3!, {r1}
	sub sp, #52
	str r1, [sp, #24]
	ldmia r3!, {r2}
	str r2, [sp, #20]
	ldr r3, [r3]
	cmp r3, #0
	bge .L_0800df34
	ldr r4, .L_0800e210
	adds r3, r3, r4
.L_0800df34:
	asrs r3, r3, #16
	adds r1, r3, #0
	muls r1, r3
	movs r2, #0
	movs r0, #6
	ldrsh r5, [r6, r0]
	mov r10, r2
	ldr r2, [r6, #8]
	str r3, [sp, #16]
	str r5, [sp, #12]
	str r1, [sp, #16]
	cmp r2, #0
	bge .L_0800df52
	ldr r3, .L_0800e210
	adds r2, r2, r3
.L_0800df52:
	adds r4, r6, #0
	adds r4, #100
	str r4, [sp, #8]
	movs r5, #0
	ldrsh r3, [r4, r5]
	asrs r2, r2, #16
	subs r2, r2, r3
	mov r11, r2
	ldr r2, [r6, #16]
	cmp r2, #0
	bge .L_0800df6c
	ldr r0, .L_0800e210
	adds r2, r2, r0
.L_0800df6c:
	adds r1, r6, #0
	adds r1, #102
	str r1, [sp, #4]
	movs r4, #0
	ldrsh r3, [r1, r4]
	asrs r2, r2, #16
	subs r2, r2, r3
	mov r9, r2
	mov r5, r11
	mov r0, r9
	mov r3, r11
	muls r3, r5
	mov r2, r9
	muls r2, r0
	ldr r1, [sp, #16]
	adds r3, r3, r2
	cmp r3, r1
	ble .L_0800df92
	b .L_0800e146
.L_0800df92:
	movs r2, #1
	add r10, r2
	mov r3, r10
	cmp r3, #7
	ble .L_0800df9e
	b .L_0800e146
.L_0800df9e:
	bl Random16
	ldr r3, .L_0800e214
	ldr r1, [sp, #20]
	movs r0, r0
	mov r12, pc
	bx r3
	ldr r4, [sp, #24]
	adds r4, r4, r0
	mov r8, r4
	bl Random16
	adds r5, r0, #0
	bl Random16
	ldr r3, [r6, #8]
	add r7, sp, #40
	ldr r1, [sp, #12]
	str r3, [r7]
	lsls r2, r1, #16
	ldr r3, [r6, #12]
	lsrs r5, r5, #2
	lsrs r2, r2, #16
	lsrs r0, r0, #2
	str r3, [r7, #4]
	adds r2, r2, r5
	subs r2, r2, r0
	ldr r3, [r6, #16]
	lsls r2, r2, #16
	lsrs r4, r2, #16
	movs r0, #128
	adds r1, r4, #0
	str r3, [r7, #8]
	lsls r0, r0, #12
	adds r2, r7, #0
	str r4, [sp, #0]
	bl Vector_AddPolarOffset
	adds r0, r6, #0
	adds r1, r7, #0
	bl ScriptObject_CheckOverlap
	ldr r4, [sp, #0]
	cmp r0, #0
	bne .L_0800df92
	ldr r3, [r6, #8]
	str r3, [r7]
	ldr r3, [r6, #12]
	str r3, [r7, #4]
	ldr r3, [r6, #16]
	adds r1, r4, #0
	str r3, [r7, #8]
	mov r0, r8
	adds r2, r7, #0
	bl Vector_AddPolarOffset
	adds r0, r6, #0
	adds r1, r7, #0
	bl Func_080120dc
	ldr r4, [sp, #0]
	cmp r0, #0
	bne .L_0800df92
	ldr r3, [r6, #8]
	add r5, sp, #28
	str r3, [r5]
	ldr r3, [r6, #12]
	str r3, [r5, #4]
	movs r2, #128
	ldr r3, [r6, #16]
	lsls r2, r2, #12
	add r8, r2
	adds r1, r4, #0
	str r3, [r5, #8]
	mov r0, r8
	adds r2, r5, #0
	bl Vector_AddPolarOffset
	adds r0, r6, #0
	adds r1, r5, #0
	bl Func_080120dc
	ldr r4, [sp, #0]
	cmp r0, #0
	bne .L_0800df92
	ldr r3, [r6, #8]
	str r3, [r5]
	ldr r3, [r6, #12]
	str r3, [r5, #4]
	ldr r3, [r6, #16]
	str r3, [r5, #8]
	movs r3, #128
	lsls r3, r3, #6
	adds r1, r4, r3
	mov r0, r8
	adds r2, r5, #0
	bl Vector_AddPolarOffset
	adds r0, r6, #0
	adds r1, r5, #0
	bl Func_080120dc
	ldr r4, [sp, #0]
	cmp r0, #0
	bne .L_0800df92
	ldr r3, [r6, #8]
	str r3, [r5]
	ldr r3, [r6, #12]
	str r3, [r5, #4]
	ldr r0, .L_0800e218
	ldr r3, [r6, #16]
	adds r1, r4, r0
	str r3, [r5, #8]
	mov r0, r8
	adds r2, r5, #0
	bl Vector_AddPolarOffset
	adds r0, r6, #0
	adds r1, r5, #0
	bl Func_080120dc
	ldr r4, [sp, #0]
	cmp r0, #0
	beq .L_0800e098
	b .L_0800df92
.L_0800e098:
	ldr r3, [r6, #8]
	str r3, [r5]
	ldr r3, [r6, #12]
	str r3, [r5, #4]
	movs r2, #128
	ldr r3, [r6, #16]
	lsls r2, r2, #7
	adds r1, r4, r2
	str r3, [r5, #8]
	mov r0, r8
	adds r2, r5, #0
	bl Vector_AddPolarOffset
	adds r0, r6, #0
	adds r1, r5, #0
	bl Func_080120dc
	ldr r4, [sp, #0]
	cmp r0, #0
	beq .L_0800e0c2
	b .L_0800df92
.L_0800e0c2:
	ldr r3, [r6, #8]
	str r3, [r5]
	ldr r3, [r6, #12]
	str r3, [r5, #4]
	ldr r3, [r6, #16]
	str r3, [r5, #8]
	ldr r3, .L_0800e21c
	mov r0, r8
	adds r1, r4, r3
	adds r2, r5, #0
	bl Vector_AddPolarOffset
	adds r0, r6, #0
	adds r1, r5, #0
	bl Func_080120dc
	cmp r0, #0
	beq .L_0800e0e8
	b .L_0800df92
.L_0800e0e8:
	ldr r1, [r7]
	adds r2, r1, #0
	cmp r1, #0
	bge .L_0800e0f4
	ldr r4, .L_0800e210
	adds r2, r1, r4
.L_0800e0f4:
	ldr r0, [sp, #8]
	movs r5, #0
	ldrsh r3, [r0, r5]
	asrs r2, r2, #16
	ldr r4, [r7, #8]
	subs r2, r2, r3
	mov r11, r2
	adds r2, r4, #0
	cmp r4, #0
	bge .L_0800e10c
	ldr r3, .L_0800e210
	adds r2, r4, r3
.L_0800e10c:
	ldr r0, [sp, #4]
	movs r5, #0
	ldrsh r3, [r0, r5]
	asrs r2, r2, #16
	subs r2, r2, r3
	mov r9, r2
	mov r5, r9
	mov r2, r11
	mov r3, r11
	muls r3, r2
	mov r2, r9
	muls r2, r5
	ldr r0, [sp, #16]
	adds r3, r3, r2
	cmp r3, r0
	ble .L_0800e12e
	b .L_0800df92
.L_0800e12e:
	adds r0, r6, #0
	adds r0, #89
	ldrb r3, [r0]
	movs r2, #2
	orrs r2, r3
	strb r2, [r0]
	ldr r2, [r7, #4]
	adds r0, r6, #0
	adds r3, r4, #0
	bl Object_SetMoveTarget
	b .L_0800e1f6
.L_0800e146:
	movs r1, #0
	mov r10, r1
	mov r0, r9
	mov r1, r11
	bl ArcTan2
	movs r2, #128
	lsls r2, r2, #8
	adds r0, r0, r2
	lsls r0, r0, #16
	asrs r0, r0, #16
	str r0, [sp, #12]
.L_0800e15e:
	movs r3, #1
	add r10, r3
	mov r4, r10
	cmp r4, #7
	bgt .L_0800e1f6
	bl Random16
	ldr r3, .L_0800e214
	ldr r1, [sp, #20]
	mov r12, pc
	bx r3
	ldr r5, [sp, #24]
	adds r5, r5, r0
	bl Random16
	mov r8, r5
	adds r5, r0, #0
	bl Random16
	ldr r1, [sp, #12]
	lsls r2, r1, #16
	ldr r3, [r6, #8]
	lsrs r5, r5, #2
	lsrs r2, r2, #16
	adds r2, r2, r5
	add r5, sp, #40
	str r3, [r5]
	ldr r3, [r6, #12]
	lsrs r0, r0, #2
	str r3, [r5, #4]
	subs r2, r2, r0
	ldr r3, [r6, #16]
	lsls r2, r2, #16
	lsrs r7, r2, #16
	movs r0, #128
	lsls r0, r0, #12
	adds r1, r7, #0
	str r3, [r5, #8]
	adds r2, r5, #0
	bl Vector_AddPolarOffset
	adds r0, r6, #0
	adds r1, r5, #0
	bl ScriptObject_CheckOverlap
	cmp r0, #0
	bne .L_0800e15e
	ldr r3, [r6, #8]
	str r3, [r5]
	ldr r3, [r6, #12]
	str r3, [r5, #4]
	ldr r3, [r6, #16]
	mov r0, r8
	adds r1, r7, #0
	str r3, [r5, #8]
	adds r2, r5, #0
	bl Vector_AddPolarOffset
	adds r0, r6, #0
	adds r1, r5, #0
	bl Func_080120dc
	cmp r0, #0
	bne .L_0800e15e
	adds r1, r6, #0
	adds r1, #89
	ldrb r2, [r1]
	movs r3, #253
	ands r3, r2
	strb r3, [r1]
	adds r0, r6, #0
	ldr r1, [r5]
	ldr r2, [r5, #4]
	ldr r3, [r5, #8]
	bl Object_SetMoveTarget
.L_0800e1f6:
	ldrh r3, [r6, #4]
	adds r3, #4
	movs r0, #1
	strh r3, [r6, #4]
	add sp, #52
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r1}
	bx r1
.L_0800e210:
	.4byte 0x0000ffff
.L_0800e214:
	.4byte IwramMulQ16ReturnIp
.L_0800e218:
	.4byte 0xffffe000
.L_0800e21c:
	.4byte 0xffffc000
