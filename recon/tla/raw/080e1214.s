.syntax unified
	.thumb
	.global Func_080e1214
	.thumb_func
Func_080e1214:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #224
	ldr r3, [r3]
	sub sp, #8
	str r3, [sp, #4]
	movs r6, #0
	ldr r1, [r3, #16]
	ldr r7, [r1, #80]
	ldrh r3, [r1, #6]
	ldr r2, [r7, #40]
	str r3, [sp, #0]
	mov r8, r1
	ldrb r1, [r7, #26]
	mov r10, r2
	mov r11, r1
	bl Resource_FindFreeEntry
	ldr r2, [sp, #4]
	movs r1, #226
	lsls r1, r1, #3
	adds r3, r2, r1
	strh r0, [r3]
	movs r1, #128
	lsls r0, r0, #16
	lsls r1, r1, #1
	ldr r2, .L_080e136c
	asrs r0, r0, #16
	bl VramBlock_LoadCached
	ldr r5, .L_080e1370
	movs r3, #153
	lsls r3, r3, #2
	adds r2, r5, r3
	movs r3, #150
	lsls r3, r3, #20
	movs r0, #70
	str r3, [r2]
	adds r0, #255
	bl GameFlag_Test
	movs r1, #154
	lsls r1, r1, #2
	adds r3, r5, r1
	strb r0, [r3]
	movs r1, #0
	mov r0, r8
	bl Animation_ApplyChildValuesFar
	ldr r3, .L_080e1374
	mov r2, r8
	str r3, [r2, #108]
	mov r5, r8
	mov r3, r8
	adds r3, #102
	adds r5, #100
	strh r6, [r5]
	movs r0, #140
	strh r6, [r3]
	bl Audio_PlayCue
	movs r0, #15
	bl WaitFrames
	movs r3, #1
	strh r3, [r5]
	movs r0, #10
	bl WaitFrames
	movs r3, #7
	movs r6, #1
	movs r5, #19
	mov r9, r3
.L_080e12b4:
	mov r1, r9
	mov r2, r10
	strb r1, [r2, #5]
	movs r0, #2
	strb r6, [r7, #25]
	bl WaitFrames
	movs r3, #0
	mov r1, r10
	strb r3, [r1, #5]
	strb r6, [r7, #25]
	strb r6, [r7, #26]
	movs r0, #3
	subs r5, #1
	bl WaitFrames
	cmp r5, #0
	bge .L_080e12b4
	movs r3, #0
	mov r2, r8
	str r3, [r2, #108]
	mov r3, sp
	ldrh r3, [r3]
	mov r1, r8
	strh r3, [r1, #6]
	movs r5, #0
	movs r1, #1
	movs r3, #2
	mov r6, r11
	mov r8, r1
	orrs r6, r3
	mov r9, r5
.L_080e12f4:
	adds r0, r5, #0
	movs r1, #5
	bl __modsi3
	cmp r0, #0
	bne .L_080e1308
	mov r2, r8
	strb r2, [r7, #25]
	strb r6, [r7, #26]
	b .L_080e131a
.L_080e1308:
	cmp r0, #2
	bne .L_080e131a
	mov r3, r8
	strb r3, [r7, #25]
	mov r1, r11
	mov r2, r9
	mov r3, r10
	strb r1, [r7, #26]
	strb r2, [r3, #5]
.L_080e131a:
	cmp r5, #15
	bne .L_080e1324
	movs r0, #174
	bl Audio_PlayCue
.L_080e1324:
	movs r0, #1
	adds r5, #1
	bl WaitFrames
	cmp r5, #69
	ble .L_080e12f4
	movs r3, #1
	mov r1, r11
	movs r2, #0
	strb r3, [r7, #25]
	mov r3, r10
	strb r1, [r7, #26]
	strb r2, [r3, #5]
	movs r0, #1
	bl WaitFrames
	ldr r1, [sp, #4]
	movs r2, #226
	lsls r2, r2, #3
	adds r3, r1, r2
	movs r1, #0
	ldrsh r0, [r3, r1]
	bl Func_08014274
	ldr r0, .L_080e1378
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWaitFar
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080e136c:
	.4byte Data_080ed90c
.L_080e1370:
	.4byte gPartyState
.L_080e1374:
	.4byte Func_080e1154
.L_080e1378:
	.4byte 0x00000dbe
