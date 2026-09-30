.syntax unified
	.thumb
	.global Func_080e2acc
	.thumb_func
Func_080e2acc:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r2, r3, #0
	adds r2, #224
	ldr r1, [r2]
	ldr r2, [r3, #108]
	ldr r1, [r1, #16]
	movs r0, #204
	lsls r0, r0, #4
	adds r2, r2, r0
	mov r9, r1
	ldr r5, [r3, #92]
	mov r0, r9
	movs r3, #0
	ldrsh r1, [r2, r3]
	ldr r3, [r0, #8]
	lsls r1, r1, #20
	subs r1, r1, r3
	movs r3, #2
	ldrsh r0, [r2, r3]
	mov r2, r9
	ldr r3, [r2, #16]
	movs r4, #128
	lsls r0, r0, #20
	lsls r4, r4, #12
	subs r0, r0, r3
	adds r1, r1, r4
	adds r0, r0, r4
	sub sp, #8
	bl ArcTan2
	lsls r0, r0, #16
	lsrs r0, r0, #16
	movs r3, #216
	str r0, [sp, #4]
	lsls r3, r3, #5
	movs r0, #216
	adds r3, #6
	lsls r0, r0, #5
	adds r2, r5, r3
	adds r0, #8
	movs r3, #0
	strh r3, [r2]
	adds r2, r5, r0
	movs r3, #1
	strh r3, [r2]
	ldr r0, .L_080e2c30
	bl Resource_GetTableEntry
	adds r1, r5, #0
	bl Func_0801587c
	bl Resource_FindFreeEntry
	movs r2, #192
	lsls r2, r2, #1
	mov r8, r2
	mov r1, r8
	adds r2, r5, #0
	adds r6, r0, #0
	bl VramBlock_LoadCached
	movs r2, #216
	mov r11, r0
	movs r0, #216
	lsls r0, r0, #5
	lsls r2, r2, #5
	adds r3, r5, r0
	adds r2, #2
	strh r6, [r3]
	mov r0, r11
	adds r3, r5, r2
	strh r0, [r3]
	movs r3, #132
	movs r2, #0
	lsls r3, r3, #5
	mov r0, r8
	mov r10, r2
	adds r6, r5, r3
	adds r7, r5, r0
.L_080e2b7a:
	mov r2, r11
	str r2, [sp, #0]
	adds r0, r7, #0
	movs r1, #4
	movs r2, #4
	movs r3, #0
	bl Func_080eaf98
	ldrb r1, [r7, #9]
	movs r0, #13
	movs r3, #250
	negs r0, r0
	strh r3, [r7, #30]
	adds r3, r0, #0
	ands r1, r3
	ldrb r3, [r7, #5]
	movs r2, #32
	orrs r3, r2
	strb r3, [r7, #5]
	movs r3, #15
	ands r1, r3
	strb r1, [r7, #9]
	mov r2, r9
	ldr r3, [r2, #8]
	str r3, [r6]
	ldr r3, [r2, #12]
	str r3, [r6, #4]
	ldr r3, [r2, #16]
	str r3, [r6, #8]
	bl Random16
	movs r3, #128
	adds r5, r0, #0
	lsls r3, r3, #12
	lsls r5, r5, #3
	adds r5, r5, r3
	bl Random16
	adds r2, r6, #0
	adds r1, r0, #0
	adds r0, r5, #0
	bl Vector_AddPolarOffset
	ldr r0, [sp, #4]
	str r0, [r6, #12]
	bl Random16
	movs r2, #128
	lsls r2, r2, #9
	adds r0, r0, r2
	str r0, [r6, #16]
	bl Random16
	movs r3, #1
	lsrs r0, r0, #6
	mov r2, r10
	add r0, r8
	ands r3, r2
	str r0, [r6, #20]
	cmp r3, #0
	beq .L_080e2bf8
	negs r3, r0
	str r3, [r6, #20]
.L_080e2bf8:
	mov r0, r10
	negs r3, r0
	lsrs r2, r3, #31
	adds r3, r3, r2
	movs r2, #1
	asrs r3, r3, #1
	add r10, r2
	str r3, [r6, #24]
	mov r3, r10
	adds r7, #40
	adds r6, #28
	cmp r3, #95
	ble .L_080e2b7a
	movs r0, #220
	bl Audio_PlayCue
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_080e2c34
	bl Scheduler_AddOrUpdateCallback
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_080e2c30:
	.4byte 0x000001e0
.L_080e2c34:
	.4byte Func_080e2c38
