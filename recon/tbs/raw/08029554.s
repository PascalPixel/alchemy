.syntax unified
	.thumb
	.global DebugMenu_BrowseIcons
	.thumb_func
DebugMenu_BrowseIcons:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #16
	movs r2, #1
	movs r1, #0
	str r2, [sp, #4]
	ldr r3, .L_08029760
	mov r10, r1
	mov r11, r1
	mov r9, r1
	add r1, sp, #4
	ldr r3, [r3]
	ldrh r1, [r1]
	movs r0, #1
	strh r1, [r3, #4]
	bl WaitFrames
.L_08029580:
	ldr r2, .L_08029764
	ldr r3, [r2]
	movs r2, #32
	ands r3, r2
	cmp r3, #0
	beq .L_08029596
	movs r3, #1
	movs r1, #1
	negs r3, r3
	str r1, [sp, #4]
	add r11, r3
.L_08029596:
	ldr r2, .L_08029764
	ldr r3, [r2]
	movs r2, #16
	ands r3, r2
	cmp r3, #0
	beq .L_080295a8
	movs r3, #1
	str r3, [sp, #4]
	add r11, r3
.L_080295a8:
	ldr r1, .L_08029764
	movs r2, #128
	ldr r3, [r1]
	lsls r2, r2, #2
	ands r3, r2
	cmp r3, #0
	beq .L_080295c0
	movs r2, #1
	movs r3, #1
	negs r2, r2
	str r3, [sp, #4]
	add r9, r2
.L_080295c0:
	ldr r1, .L_08029764
	movs r2, #128
	ldr r3, [r1]
	lsls r2, r2, #1
	ands r3, r2
	cmp r3, #0
	beq .L_080295d4
	movs r2, #1
	str r2, [sp, #4]
	add r9, r2
.L_080295d4:
	ldr r1, .L_08029764
	ldr r3, [r1]
	movs r2, #1
	ands r3, r2
	cmp r3, #0
	beq .L_080295e2
	b .L_0802973c
.L_080295e2:
	ldr r3, [r1]
	movs r5, #2
	ands r3, r5
	cmp r3, #0
	beq .L_080295ee
	b .L_0802973c
.L_080295ee:
	ldr r2, [sp, #4]
	cmp r2, #0
	bne .L_080295f6
	b .L_08029734
.L_080295f6:
	mov r2, r11
	movs r3, #0
	adds r2, #8
	str r3, [sp, #4]
	adds r3, r2, #0
	cmp r2, #0
	bge .L_08029608
	mov r3, r11
	adds r3, #15
.L_08029608:
	asrs r3, r3, #3
	lsls r3, r3, #3
	mov r0, r9
	subs r2, r2, r3
	movs r1, #3
	adds r0, #3
	mov r11, r2
	bl __modsi3
	movs r1, #2
	mov r9, r0
	mov r0, r10
	bl UiWork_Finalize
	movs r1, #0
	movs r0, #10
	movs r2, #18
	movs r3, #12
	str r5, [sp, #0]
	bl UiWindow_Create
	mov r1, r9
	mov r10, r0
	cmp r1, #0
	bne .L_0802963e
	ldr r0, .L_08029768
	b .L_08029646
.L_0802963e:
	mov r2, r9
	cmp r2, #1
	bne .L_08029652
	ldr r0, .L_0802976c
.L_08029646:
	mov r1, r10
	movs r2, #0
	movs r3, #0
	bl UiText_DrawStringInWindow
	b .L_0802965e
.L_08029652:
	ldr r0, .L_08029770
	mov r1, r10
	movs r2, #0
	movs r3, #0
	bl UiText_DrawStringInWindow
.L_0802965e:
	ldr r0, .L_08029774
	mov r1, r10
	movs r2, #0
	movs r3, #8
	bl UiText_DrawStringInWindow
	movs r3, #8
	str r3, [sp, #0]
	mov r0, r11
	movs r1, #0
	mov r2, r10
	movs r3, #40
	bl UiText_DrawNumberInWindow
	mov r1, r11
	lsls r1, r1, #5
	mov r8, r1
	movs r2, #8
	str r2, [sp, #0]
	mov r0, r8
	movs r1, #3
	mov r2, r10
	movs r3, #64
	bl UiText_DrawNumberInWindow
	ldr r0, .L_08029778
	mov r1, r10
	movs r2, #88
	movs r3, #8
	bl UiText_DrawStringInWindow
	movs r3, #8
	mov r0, r8
	str r3, [sp, #0]
	adds r0, #31
	movs r1, #3
	mov r2, r10
	movs r3, #96
	bl UiText_DrawNumberInWindow
	movs r5, #0
.L_080296b0:
	movs r3, #1
	negs r3, r3
	str r3, [sp, #12]
	adds r2, r5, #0
	cmp r5, #0
	bge .L_080296be
	adds r2, r5, #7
.L_080296be:
	asrs r2, r2, #3
	lsls r3, r2, #3
	lsls r2, r2, #4
	subs r3, r5, r3
	adds r6, r2, #0
	mov r1, r9
	lsls r7, r3, #4
	adds r6, #16
	cmp r1, #0
	bne .L_080296e4
	mov r2, r8
	adds r0, r2, r5
	str r1, [sp, #0]
	add r2, sp, #12
	movs r1, #1
	add r3, sp, #8
	bl UiIcon_BuildItemIconTiles
	b .L_080296fc
.L_080296e4:
	mov r3, r9
	cmp r3, #1
	bne .L_0802970e
	mov r1, r8
	movs r3, #0
	adds r0, r1, r5
	str r3, [sp, #0]
	movs r1, #1
	add r2, sp, #12
	add r3, sp, #8
	bl UiIcon_BuildAbilityIconTiles
.L_080296fc:
	movs r1, #128
	ldr r0, [sp, #12]
	lsls r1, r1, #23
	mov r2, r10
	adds r3, r7, #0
	str r6, [sp, #0]
	bl RenderOutput_Create
	b .L_0802972e
.L_0802970e:
	bl Resource_FindFreeEntry
	movs r1, #0
	adds r2, r0, #0
	adds r0, r5, #0
	str r2, [sp, #12]
	bl Ui_BuildPatternToSlot
	movs r1, #128
	ldr r0, [sp, #12]
	lsls r1, r1, #23
	mov r2, r10
	adds r3, r7, #0
	str r6, [sp, #0]
	bl RenderOutput_Create
.L_0802972e:
	adds r5, #1
	cmp r5, #31
	ble .L_080296b0
.L_08029734:
	movs r0, #1
	bl WaitFrames
	b .L_08029580
.L_0802973c:
	mov r0, r10
	movs r1, #2
	bl UiWork_Finalize
	ldr r3, .L_08029760
	ldr r2, [r3]
	movs r3, #0
	movs r0, #0
	strh r3, [r2, #4]
	add sp, #16
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r1}
	bx r1
.L_08029760:
	.4byte gMenuCtrlWork
.L_08029764:
	.4byte gKeysRepeat
.L_08029768:
	.4byte Data_08037440
.L_0802976c:
	.4byte Data_08037448
.L_08029770:
	.4byte Data_08037450
.L_08029774:
	.4byte Data_08037458
.L_08029778:
	.4byte Data_08037460
