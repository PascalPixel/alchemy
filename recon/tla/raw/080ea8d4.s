.syntax unified
	.thumb
	.global Func_080ea8d4
	.thumb_func
Func_080ea8d4:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	sub sp, #4
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	str r0, [sp, #0]
	mov r11, r3
	movs r2, #255
	ldrh r3, [r0]
	lsls r2, r2, #8
	adds r2, #255
	mov r10, r0
	cmp r3, r2
	beq .L_080ea9ce
	movs r3, #128
	lsls r3, r3, #13
	mov r9, r3
.L_080ea902:
	mov r2, r10
	ldrh r0, [r2]
	bl Object_GetById
	mov r2, r10
	adds r7, r0, #0
	movs r3, #2
	ldrsh r6, [r2, r3]
	bl Func_080eab70
	movs r3, #34
	adds r3, r3, r7
	ldrb r2, [r3]
	mov r8, r3
	lsls r3, r2, #3
	subs r3, r3, r2
	movs r2, #156
	lsls r2, r2, #1
	lsls r3, r3, #3
	adds r3, r3, r2
	mov r2, r11
	ldr r5, [r2, r3]
	adds r2, r7, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	adds r1, r7, #0
	adds r1, #89
	ldrb r2, [r1]
	movs r3, #254
	ands r3, r2
	lsls r0, r0, #2
	strb r3, [r1]
	adds r5, r5, r0
	movs r1, #0
	adds r0, r7, #0
	bl ObjectDispatch_SetSingleChildField26Far
	ldr r0, [r7, #8]
	movs r3, #255
	strb r3, [r5, #2]
	mov r3, r8
	ldrb r2, [r3]
	add r0, r9
	ldr r1, [r7, #16]
	ldr r3, [r7, #20]
	bl Func_080ea8a8
	ldr r0, [r7, #8]
	ldr r2, .L_080eaa10
	mov r3, r8
	adds r0, r0, r2
	ldr r1, [r7, #16]
	ldrb r2, [r3]
	ldr r3, [r7, #20]
	bl Func_080ea8a8
	ldr r1, [r7, #16]
	mov r3, r8
	ldrb r2, [r3]
	ldr r0, [r7, #8]
	add r1, r9
	ldr r3, [r7, #20]
	bl Func_080ea8a8
	ldr r1, [r7, #16]
	ldr r2, .L_080eaa10
	mov r3, r8
	lsls r6, r6, #16
	ldr r0, [r7, #8]
	adds r1, r1, r2
	lsrs r6, r6, #16
	ldrb r2, [r3]
	ldr r3, [r7, #20]
	bl Func_080ea8a8
	adds r0, r6, #0
	bl GameFlag_Test
	cmp r0, #0
	beq .L_080ea9b4
	adds r0, r7, #0
	movs r1, #0
	bl Object_SetMode
	adds r0, r6, #0
	bl GameFlag_ClearBit
	b .L_080ea9bc
.L_080ea9b4:
	adds r0, r7, #0
	movs r1, #1
	bl Object_SetMode
.L_080ea9bc:
	movs r2, #4
	add r10, r2
	mov r2, r10
	ldrh r3, [r2]
	movs r2, #255
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	bne .L_080ea902
.L_080ea9ce:
	ldr r0, [sp, #0]
	bl Func_080eaa14
	bl EventRuntime_GetControlledOwner
	bl Object_GetById
	adds r5, r0, #0
	adds r6, r5, #0
	adds r6, #34
	ldr r0, [r5, #8]
	ldr r1, [r5, #16]
	ldrb r2, [r6]
	ldr r3, [r5, #20]
	bl Func_08020310
	cmp r0, #0
	beq .L_080eaa00
	ldrb r0, [r6]
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	bl Map_GetTerrainHeightFar
	str r0, [r5, #12]
	str r0, [r5, #20]
.L_080eaa00:
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080eaa10:
	.4byte 0xfff00000
