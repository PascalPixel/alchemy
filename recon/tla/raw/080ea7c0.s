.syntax unified
	.thumb
	.global Func_080ea7c0
	.thumb_func
Func_080ea7c0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	mov r8, r0
	mov r9, r8
	mov r2, r9
	mov r10, r3
	ldrh r3, [r2]
	movs r2, #255
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	beq .L_080ea86a
	mov r11, r2
.L_080ea7ea:
	mov r3, r8
	ldrh r0, [r3]
	bl Object_GetById
	mov r3, r8
	adds r7, r0, #0
	movs r2, #2
	ldrsh r6, [r3, r2]
	bl Func_080eab70
	adds r3, r7, #0
	adds r3, #34
	ldrb r2, [r3]
	adds r1, r7, #0
	lsls r3, r2, #3
	subs r3, r3, r2
	movs r2, #156
	lsls r2, r2, #1
	lsls r3, r3, #3
	adds r3, r3, r2
	mov r2, r10
	ldr r5, [r2, r3]
	adds r2, r7, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	adds r1, #89
	ldrb r2, [r1]
	movs r3, #254
	ands r3, r2
	lsls r0, r0, #2
	strb r3, [r1]
	adds r5, r5, r0
	movs r1, #0
	adds r0, r7, #0
	lsls r6, r6, #16
	bl ObjectDispatch_SetSingleChildField26Far
	lsrs r6, r6, #16
	movs r3, #255
	strb r3, [r5, #2]
	adds r0, r6, #0
	bl GameFlag_Test
	cmp r0, #0
	beq .L_080ea856
	adds r0, r7, #0
	movs r1, #0
	bl Object_SetMode
	adds r0, r6, #0
	bl GameFlag_ClearBit
	b .L_080ea85e
.L_080ea856:
	adds r0, r7, #0
	movs r1, #1
	bl Object_SetMode
.L_080ea85e:
	movs r3, #4
	add r8, r3
	mov r2, r8
	ldrh r3, [r2]
	cmp r3, r11
	bne .L_080ea7ea
.L_080ea86a:
	mov r0, r9
	bl Func_080eaa14
	ldr r3, .L_080ea8a4
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	adds r5, r0, #0
	adds r3, r5, #0
	adds r3, #34
	ldrb r0, [r3]
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	bl Func_080201c0
	ldr r3, [r5, #12]
	cmp r3, r0
	bge .L_080ea898
	str r0, [r5, #20]
	str r0, [r5, #12]
.L_080ea898:
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_080ea8a4:
	.4byte gPartyState
