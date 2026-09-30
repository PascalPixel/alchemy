.syntax unified
	.thumb
	.global Inventory_PromptAndSetObjectMode
	.thumb_func
Inventory_PromptAndSetObjectMode:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r6, [r3, #108]
	mov r8, r0
	movs r0, #240
	lsls r0, r0, #1
	adds r3, r6, r0
	ldr r0, [r3]
	mov r10, r1
	bl Func_080cccb8
	movs r3, #0
	ldrsh r2, [r0, r3]
	movs r0, #242
	mov r9, r2
	lsls r0, r0, #1
	movs r2, #244
	adds r3, r6, r0
	lsls r2, r2, #1
	ldr r7, [r3]
	adds r3, r6, r2
	ldr r5, [r3]
	movs r3, #1
	mov r11, r3
	b .L_080d2886
.L_080d2880:
	movs r0, #1
	bl WaitFrames
.L_080d2886:
	bl UiWork_IsCompleteFar
	cmp r0, #0
	beq .L_080d2880
	ldr r3, .L_080d2958
	movs r0, #139
	lsls r0, r0, #2
	adds r3, r3, r0
	ldrb r2, [r3]
	movs r3, #2
	subs r3, r3, r2
	lsls r0, r3, #1
	adds r0, r0, r3
	bl WaitFrames
	mov r2, r10
	cmp r2, #0
	bne .L_080d28cc
	movs r3, #14
	ldrsh r2, [r7, r3]
	ldrh r3, [r7, #10]
	adds r1, r2, r3
	cmp r5, #0
	beq .L_080d28c4
	movs r0, #14
	ldrsh r2, [r5, r0]
	ldrh r3, [r5, #10]
	adds r2, r2, r3
	cmp r1, r2
	bge .L_080d28c4
	adds r1, r2, #0
.L_080d28c4:
	cmp r1, #15
	ble .L_080d28cc
	movs r2, #0
	mov r11, r2
.L_080d28cc:
	movs r0, #203
	lsls r0, r0, #4
	adds r3, r6, r0
	adds r0, #2
	movs r2, #0
	ldrsh r1, [r3, r2]
	adds r3, r6, r0
	movs r0, #0
	ldrsh r2, [r3, r0]
	movs r0, #192
	lsls r0, r0, #4
	adds r0, #180
	adds r3, r6, r0
	movs r0, #0
	ldrsh r3, [r3, r0]
	mov r0, r11
	bl Menu_RunConfirmSelectionFar
	adds r5, r0, #0
	cmp r5, #0
	beq .L_080d2920
	movs r1, #4
	mov r0, r8
	bl Object_SetModeById
	mov r0, r9
	bl UiWork_FinalizeEntityMatchingLocalizedIdFar
	bl Func_08038140
	ldr r3, .L_080d2958
	movs r2, #139
	lsls r2, r2, #2
	adds r3, r3, r2
	ldrb r3, [r3]
	cmp r3, #2
	beq .L_080d2948
	mov r0, r8
	movs r1, #4
	bl Func_080d2810
	b .L_080d2948
.L_080d2920:
	movs r1, #3
	mov r0, r8
	bl Object_SetModeById
	mov r0, r9
	bl UiWork_FinalizeEntityMatchingLocalizedIdFar
	bl Func_08038140
	ldr r3, .L_080d2958
	movs r0, #139
	lsls r0, r0, #2
	adds r3, r3, r0
	ldrb r3, [r3]
	cmp r3, #2
	beq .L_080d2948
	mov r0, r8
	movs r1, #3
	bl Func_080d2810
.L_080d2948:
	adds r0, r5, #0
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080d2958:
	.4byte gPartyState
