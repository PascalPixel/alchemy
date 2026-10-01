.syntax unified
	.thumb
	.global BattleEscape_CheckSuccess
	.thumb_func
BattleEscape_CheckSuccess:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r2, #0
	sub sp, #36
	str r2, [sp, #4]
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #36]
	adds r3, r2, #0
	adds r3, #69
	ldrb r3, [r3]
	cmp r3, #1
	bne .L_0811cebe
	movs r3, #1
	str r3, [sp, #4]
	b .L_0811cf7a
.L_0811cebe:
	adds r2, #70
	str r2, [sp, #0]
	movs r0, #1
	ldrb r2, [r2]
	movs r6, #0
	lsls r3, r2, #5
	subs r3, r3, r2
	lsls r3, r3, #2
	adds r3, r3, r2
	movs r2, #152
	lsls r2, r2, #5
	lsls r3, r3, #4
	adds r2, #136
	adds r2, r2, r3
	add r3, sp, #8
	mov r10, r3
	mov r1, r10
	mov r9, r2
	bl BattleParty_ListLivingUnits
	ldr r2, [sp, #4]
	mov r8, r0
	cmp r2, r8
	bge .L_0811cf08
	mov r11, r10
	movs r7, #0
	mov r5, r8
.L_0811cef4:
	mov r3, r11
	ldrsh r0, [r7, r3]
	bl Owner_GetState
	ldrb r3, [r0, #15]
	subs r5, #1
	adds r6, r6, r3
	adds r7, #2
	cmp r5, #0
	bne .L_0811cef4
.L_0811cf08:
	lsls r0, r6, #5
	subs r0, r0, r6
	lsls r0, r0, #2
	adds r0, r0, r6
	mov r1, r8
	lsls r0, r0, #2
	bl __divsi3
	mov r1, r10
	add r9, r0
	movs r0, #2
	bl BattleParty_ListLivingUnits
	movs r6, #0
	mov r8, r0
	cmp r6, r8
	bge .L_0811cf42
	movs r7, #0
	mov r5, r8
.L_0811cf2e:
	mov r3, r10
	ldrsh r0, [r7, r3]
	bl Owner_GetState
	ldrb r3, [r0, #15]
	subs r5, #1
	adds r6, r6, r3
	adds r7, #2
	cmp r5, #0
	bne .L_0811cf2e
.L_0811cf42:
	lsls r0, r6, #5
	subs r0, r0, r6
	lsls r0, r0, #2
	adds r0, r0, r6
	lsls r0, r0, #2
	mov r1, r8
	bl __divsi3
	mov r3, r9
	subs r3, r3, r0
	mov r9, r3
	cmp r3, #0
	ble .L_0811cf72
	bl Random16
	movs r3, #156
	lsls r3, r3, #6
	adds r3, #16
	muls r3, r0
	lsrs r3, r3, #16
	cmp r3, r9
	bcs .L_0811cf72
	movs r2, #1
	str r2, [sp, #4]
.L_0811cf72:
	ldr r2, [sp, #0]
	ldrb r3, [r2]
	adds r3, #1
	strb r3, [r2]
.L_0811cf7a:
	ldr r3, .L_0811cfa0
	movs r2, #166
	lsls r2, r2, #1
	adds r2, #255
	adds r3, r3, r2
	ldrb r3, [r3]
	cmp r3, #2
	bne .L_0811cf8e
	movs r3, #0
	str r3, [sp, #4]
.L_0811cf8e:
	ldr r0, [sp, #4]
	add sp, #36
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0811cfa0:
	.4byte gPartyState
