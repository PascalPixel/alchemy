.syntax unified
	.thumb
	.global UiText_OpenMessageAtObject
	.type UiText_OpenMessageAtObject, %function
	.thumb_func
UiText_OpenMessageAtObject:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .Lpool0
	ldr r1, [r3]
	sub sp, #64
	str r1, [sp, #32]
	ldr r3, [r3, #48]
	movs r2, #0
	adds r6, r0, #0
	str r3, [sp, #28]
	mov r10, r2
	mov r9, r2
	bl ObjectTable_ReadActiveValue
	movs r2, #240
	lsls r2, r2, #8
	movs r3, #0
	mov r11, r2
	movs r1, #4
	str r3, [sp, #20]
	str r3, [sp, #16]
	str r1, [sp, #12]
	mov r3, r11
	ldr r1, [sp, #28]
	movs r2, #236
	lsls r2, r2, #1
	str r0, [sp, #24]
	ands r3, r6
	mov r11, r3
	adds r3, r1, r2
	movs r2, #0
	ldrsh r1, [r3, r2]
	ldr r3, .Lpool1
	ands r6, r3
	movs r4, #0
	adds r0, r6, #0
	mov r8, r1
	str r4, [sp, #4]
	bl ObjectTable_Get
	movs r2, #250
	ldr r1, [sp, #28]
	lsls r2, r2, #1
	adds r3, r1, r2
	str r6, [r3]
	subs r2, #40
	adds r3, r1, r2
	ldr r3, [r3]
	movs r5, #0
	movs r7, #0
	ldr r4, [sp, #4]
	cmp r3, #0
	beq .L0
	b .L1
.L0:
	cmp r0, #0
	beq .L2
	subs r2, #46
	adds r3, r1, r2
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #3
	bne .L3
	add r5, sp, #52
	adds r1, r5, #0
	adds r0, #8
	bl Render_ProjectPoint
	ldr r3, [r5]
	asrs r4, r3, #3
	ldr r3, [r5, #4]
	asrs r5, r3, #3
	movs r7, #1
	subs r5, #2
	b .L4
.L3:
	add r5, sp, #52
	adds r1, r5, #0
	adds r0, r6, #0
	b .L5
.L2:
	cmp r6, #7
	bgt .L4
	ldr r3, .Lpool2
	movs r2, #250
	lsls r2, r2, #1
	str r6, [sp, #24]
	adds r5, r3, r2
	ldr r0, [r5]
	bl ObjectTable_Get
	movs r2, #207
	ldr r1, [sp, #28]
	lsls r2, r2, #1
	adds r3, r1, r2
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #3
	bne .L6
	add r5, sp, #52
	adds r1, r5, #0
	adds r0, #8
	bl Render_ProjectPoint
	ldr r3, [r5]
	asrs r4, r3, #3
	ldr r3, [r5, #4]
	movs r7, #1
	b .L7
.L6:
	ldr r0, [r5]
	add r5, sp, #52
	adds r1, r5, #0
.L5:
	bl Object_GetScreenPosition
	mvns r0, r0
	negs r3, r0
	orrs r3, r0
	lsrs r7, r3, #31
	ldr r3, [r5]
	asrs r4, r3, #3
	ldr r3, [r5, #4]
.L7:
	asrs r5, r3, #3
.L4:
	cmp r7, #0
	bne .L8
	movs r3, #15
	str r3, [sp, #48]
	movs r3, #10
	b .L9
.L8:
	movs r3, #0
	add r0, sp, #36
	str r3, [sp, #48]
	str r3, [sp, #44]
	add r2, sp, #44
	add r3, sp, #40
	str r0, [sp, #0]
	add r1, sp, #48
	mov r0, r8
	str r4, [sp, #4]
	bl UiText_GetResourceDimensionsAltFar
	ldr r3, [sp, #40]
	lsrs r2, r3, #31
	adds r3, r3, r2
	ldr r4, [sp, #4]
	asrs r3, r3, #1
	subs r3, r4, r3
	str r3, [sp, #48]
	movs r3, #128
	lsls r3, r3, #7
	mov r2, r11
	ands r3, r2
	cmp r3, #0
	beq .L10
	ldr r3, [sp, #36]
	subs r3, r5, r3
	subs r3, #1
	b .L9
.L10:
	mov r1, r11
	lsrs r3, r1, #15
	cmp r3, #0
	bne .L11
	cmp r5, #8
	bgt .L11
	ldr r3, [sp, #36]
	subs r3, r5, r3
	subs r3, #1
	b .L9
.L11:
	adds r3, r5, #4
.L9:
	str r3, [sp, #44]
	ldr r2, [sp, #32]
	ldr r1, .Lpool3
	adds r3, r2, r1
	ldrb r3, [r3]
	cmp r3, #0
	beq .L12
	movs r2, #5
	str r2, [sp, #12]
.L12:
	movs r3, #128
	lsls r3, r3, #5
	mov r1, r11
	ands r3, r1
	adds r6, r4, #0
	cmp r3, #0
	beq .L13
	ldr r2, [sp, #12]
	subs r3, r6, r2
	subs r6, r3, #2
	cmp r6, #0
	bge .L14
	movs r6, #0
	b .L14
.L13:
	movs r3, #128
	lsls r3, r3, #6
	mov r1, r11
	ands r3, r1
	cmp r3, #0
	beq .L15
	ldr r2, [sp, #12]
	adds r6, #2
	adds r3, r6, r2
	cmp r3, #29
	ble .L14
	movs r3, #29
	subs r6, r3, r2
	b .L14
.L15:
	cmp r6, #15
	bgt .L16
	ldr r1, [sp, #12]
	subs r3, r6, r1
	subs r6, r3, #2
	cmp r6, #0
	bge .L14
	adds r6, r4, #2
	b .L14
.L16:
	ldr r2, [sp, #12]
	adds r6, #2
	adds r3, r6, r2
	cmp r3, #29
	ble .L14
	subs r3, r4, r2
	subs r6, r3, #2
.L14:
	ldr r0, [sp, #24]
	bl Localization_LookupEntryIdFar
	movs r3, #1
	negs r3, r3
	adds r7, r0, #0
	mov r10, r3
	cmp r7, r10
	beq .L17
	mov r3, sp
	movs r1, #48
	movs r2, #44
	add r1, sp
	add r2, sp
	adds r3, #40
	add r7, sp, #36
	mov r0, r8
	mov r9, r1
	mov r11, r2
	str r3, [sp, #8]
	str r7, [sp, #0]
	bl UiText_GetResourceDimensionsAltFar
	ldr r2, [sp, #44]
	subs r1, r2, #5
	mov r8, r10
	str r1, [sp, #16]
	cmp r2, r5
	bgt .L18
	ldr r3, [sp, #36]
	adds r3, r2, r3
	str r3, [sp, #16]
.L18:
	ldr r3, [sp, #16]
	cmp r3, #0
	bge .L19
	ldr r3, [sp, #36]
	adds r3, r2, r3
	str r3, [sp, #16]
	b .L20
.L19:
	ldr r3, [sp, #16]
	adds r3, #5
	cmp r3, #19
	ble .L20
	subs r1, r2, #5
	str r1, [sp, #16]
.L20:
	ldr r3, [sp, #16]
	cmp r2, r3
	bge .L21
	movs r0, #1
	mov r1, r9
	ldr r3, [sp, #8]
	negs r0, r0
	mov r2, r11
	ldr r5, [sp, #36]
	str r7, [sp, #0]
	bl UiText_GetResourceDimensionsFar
	ldr r3, [sp, #36]
	movs r1, #1
	subs r5, r5, r3
	negs r1, r1
	adds r5, #1
	mov r8, r1
	str r5, [sp, #20]
	b .L21
.L17:
	ldr r3, [sp, #44]
	cmp r3, r5
	bge .L21
	add r0, sp, #36
	add r3, sp, #40
	str r0, [sp, #0]
	add r1, sp, #48
	mov r0, r8
	add r2, sp, #44
	ldr r5, [sp, #36]
	bl UiText_GetResourceDimensionsFar
	ldr r3, [sp, #36]
	subs r5, r5, r3
	adds r5, #1
	str r5, [sp, #20]
	mov r8, r7
.L21:
	cmp r6, #0
	bge .L22
	movs r6, #0
	b .L23
.L22:
	ldr r2, [sp, #12]
	adds r3, r6, r2
	cmp r3, #29
	ble .L23
	movs r3, #29
	subs r6, r3, r2
.L23:
	ldr r1, [sp, #32]
	ldr r2, .Lpool3
	adds r3, r1, r2
	ldrb r3, [r3]
	cmp r3, #0
	beq .L24
	movs r0, #8
	bl WaitFrames
	ldr r3, [sp, #20]
	cmp r3, #0
	beq .L25
	ldr r2, [sp, #44]
	adds r2, r2, r3
	ldr r1, [sp, #48]
	subs r2, #1
	mov r0, r8
	movs r3, #18
	b .L26
.L25:
	ldr r1, [sp, #48]
	ldr r2, [sp, #44]
	mov r0, r8
	movs r3, #2
	b .L26
.L24:
	ldr r0, [sp, #24]
	bl BattleFx_GetResourceId
	ldr r1, [sp, #20]
	cmp r1, #0
	beq .L27
	ldr r3, [sp, #20]
	ldr r2, [sp, #44]
	adds r2, r2, r3
	lsls r3, r0, #16
	movs r0, #17
	orrs r3, r0
	ldr r1, [sp, #48]
	subs r2, #1
	b .L28
.L27:
	lsls r3, r0, #16
	movs r0, #1
	orrs r3, r0
	ldr r1, [sp, #48]
	ldr r2, [sp, #44]
.L28:
	mov r0, r8
.L26:
	bl UiText_OpenMessageWindowFar
	mov r10, r0
	ldr r1, [sp, #32]
	ldr r2, .Lpool3
	adds r3, r1, r2
	ldrb r3, [r3]
	ldr r0, [sp, #24]
	movs r1, #0
	adds r2, r6, #0
	ldr r3, [sp, #16]
	bl UiWindow_CreateWithSideObjectFar
	mov r9, r0
	b .L29
.L30:
	movs r0, #1
	bl WaitFrames
.L29:
	bl UiWork_IsCompleteFar
	cmp r0, #0
	beq .L30
.L1:
	ldr r1, [sp, #28]
	movs r2, #252
	lsls r2, r2, #1
	adds r3, r1, r2
	mov r1, r10
	str r1, [r3]
	ldr r2, [sp, #28]
	movs r1, #254
	lsls r1, r1, #1
	adds r3, r2, r1
	mov r2, r9
	str r2, [r3]
	ldr r3, [sp, #28]
	subs r1, #36
	adds r2, r3, r1
	ldrh r3, [r2]
	adds r3, #1
	mov r0, r10
	strh r3, [r2]
	sub sp, #-64
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r1}
	bx r1
.Lpool0:
	.4byte gWindowWork
.Lpool1:
	.4byte 0x00000fff
.Lpool2:
	.4byte gGameState
.Lpool3:
	.4byte 0x00000ea4
	.size UiText_OpenMessageAtObject, .-UiText_OpenMessageAtObject
