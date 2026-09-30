.syntax unified
	.thumb
	.global UiText_OpenMessageAtObject
	.thumb_func
UiText_OpenMessageAtObject:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .L_08092f74
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
	ldr r3, .L_08092f78
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
	beq .L_08092cb6
	b .L_08092f3c
.L_08092cb6:
	cmp r0, #0
	beq .L_08092ce6
	subs r2, #46
	adds r3, r1, r2
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #3
	bne .L_08092cde
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
	b .L_08092d38
.L_08092cde:
	add r5, sp, #52
	adds r1, r5, #0
	adds r0, r6, #0
	b .L_08092d24
.L_08092ce6:
	cmp r6, #7
	bgt .L_08092d38
	ldr r3, .L_08092f7c
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
	bne .L_08092d1e
	add r5, sp, #52
	adds r1, r5, #0
	adds r0, #8
	bl Render_ProjectPoint
	ldr r3, [r5]
	asrs r4, r3, #3
	ldr r3, [r5, #4]
	movs r7, #1
	b .L_08092d36
.L_08092d1e:
	ldr r0, [r5]
	add r5, sp, #52
	adds r1, r5, #0
.L_08092d24:
	bl Object_GetScreenPosition
	mvns r0, r0
	negs r3, r0
	orrs r3, r0
	lsrs r7, r3, #31
	ldr r3, [r5]
	asrs r4, r3, #3
	ldr r3, [r5, #4]
.L_08092d36:
	asrs r5, r3, #3
.L_08092d38:
	cmp r7, #0
	bne .L_08092d44
	movs r3, #15
	str r3, [sp, #48]
	movs r3, #10
	b .L_08092d94
.L_08092d44:
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
	beq .L_08092d7e
	ldr r3, [sp, #36]
	subs r3, r5, r3
	subs r3, #1
	b .L_08092d94
.L_08092d7e:
	mov r1, r11
	lsrs r3, r1, #15
	cmp r3, #0
	bne .L_08092d92
	cmp r5, #8
	bgt .L_08092d92
	ldr r3, [sp, #36]
	subs r3, r5, r3
	subs r3, #1
	b .L_08092d94
.L_08092d92:
	adds r3, r5, #4
.L_08092d94:
	str r3, [sp, #44]
	ldr r2, [sp, #32]
	ldr r1, .L_08092f80
	adds r3, r2, r1
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_08092da6
	movs r2, #5
	str r2, [sp, #12]
.L_08092da6:
	movs r3, #128
	lsls r3, r3, #5
	mov r1, r11
	ands r3, r1
	adds r6, r4, #0
	cmp r3, #0
	beq .L_08092dc2
	ldr r2, [sp, #12]
	subs r3, r6, r2
	subs r6, r3, #2
	cmp r6, #0
	bge .L_08092dfe
	movs r6, #0
	b .L_08092dfe
.L_08092dc2:
	movs r3, #128
	lsls r3, r3, #6
	mov r1, r11
	ands r3, r1
	cmp r3, #0
	beq .L_08092dde
	ldr r2, [sp, #12]
	adds r6, #2
	adds r3, r6, r2
	cmp r3, #29
	ble .L_08092dfe
	movs r3, #29
	subs r6, r3, r2
	b .L_08092dfe
.L_08092dde:
	cmp r6, #15
	bgt .L_08092df0
	ldr r1, [sp, #12]
	subs r3, r6, r1
	subs r6, r3, #2
	cmp r6, #0
	bge .L_08092dfe
	adds r6, r4, #2
	b .L_08092dfe
.L_08092df0:
	ldr r2, [sp, #12]
	adds r6, #2
	adds r3, r6, r2
	cmp r3, #29
	ble .L_08092dfe
	subs r3, r4, r2
	subs r6, r3, #2
.L_08092dfe:
	ldr r0, [sp, #24]
	bl Localization_LookupEntryIdFar
	movs r3, #1
	negs r3, r3
	adds r7, r0, #0
	mov r10, r3
	cmp r7, r10
	beq .L_08092e80
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
	bgt .L_08092e3e
	ldr r3, [sp, #36]
	adds r3, r2, r3
	str r3, [sp, #16]
.L_08092e3e:
	ldr r3, [sp, #16]
	cmp r3, #0
	bge .L_08092e4c
	ldr r3, [sp, #36]
	adds r3, r2, r3
	str r3, [sp, #16]
	b .L_08092e58
.L_08092e4c:
	ldr r3, [sp, #16]
	adds r3, #5
	cmp r3, #19
	ble .L_08092e58
	subs r1, r2, #5
	str r1, [sp, #16]
.L_08092e58:
	ldr r3, [sp, #16]
	cmp r2, r3
	bge .L_08092ea2
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
	b .L_08092ea2
.L_08092e80:
	ldr r3, [sp, #44]
	cmp r3, r5
	bge .L_08092ea2
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
.L_08092ea2:
	cmp r6, #0
	bge .L_08092eaa
	movs r6, #0
	b .L_08092eb6
.L_08092eaa:
	ldr r2, [sp, #12]
	adds r3, r6, r2
	cmp r3, #29
	ble .L_08092eb6
	movs r3, #29
	subs r6, r3, r2
.L_08092eb6:
	ldr r1, [sp, #32]
	ldr r2, .L_08092f80
	adds r3, r1, r2
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_08092ee6
	movs r0, #8
	bl WaitFrames
	ldr r3, [sp, #20]
	cmp r3, #0
	beq .L_08092edc
	ldr r2, [sp, #44]
	adds r2, r2, r3
	ldr r1, [sp, #48]
	subs r2, #1
	mov r0, r8
	movs r3, #18
	b .L_08092f10
.L_08092edc:
	ldr r1, [sp, #48]
	ldr r2, [sp, #44]
	mov r0, r8
	movs r3, #2
	b .L_08092f10
.L_08092ee6:
	ldr r0, [sp, #24]
	bl BattleFx_GetResourceId
	ldr r1, [sp, #20]
	cmp r1, #0
	beq .L_08092f04
	ldr r3, [sp, #20]
	ldr r2, [sp, #44]
	adds r2, r2, r3
	lsls r3, r0, #16
	movs r0, #17
	orrs r3, r0
	ldr r1, [sp, #48]
	subs r2, #1
	b .L_08092f0e
.L_08092f04:
	lsls r3, r0, #16
	movs r0, #1
	orrs r3, r0
	ldr r1, [sp, #48]
	ldr r2, [sp, #44]
.L_08092f0e:
	mov r0, r8
.L_08092f10:
	bl UiText_OpenMessageWindowFar
	mov r10, r0
	ldr r1, [sp, #32]
	ldr r2, .L_08092f80
	adds r3, r1, r2
	ldrb r3, [r3]
	ldr r0, [sp, #24]
	movs r1, #0
	adds r2, r6, #0
	ldr r3, [sp, #16]
	bl Func_080150f8
	mov r9, r0
	b .L_08092f34
.L_08092f2e:
	movs r0, #1
	bl WaitFrames
.L_08092f34:
	bl UiWork_IsCompleteFar
	cmp r0, #0
	beq .L_08092f2e
.L_08092f3c:
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
	add sp, #64
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r1}
	bx r1
.L_08092f74:
	.4byte Data_03001e8c
.L_08092f78:
	.4byte 0x00000fff
.L_08092f7c:
	.4byte gCell
.L_08092f80:
	.4byte 0x00000ea4
