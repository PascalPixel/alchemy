.syntax unified
	.thumb
	.global DebugParty_LoadPreset
	.thumb_func
DebugParty_LoadPreset:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #40
	movs r1, #0
	str r0, [sp, #20]
	movs r0, #0
	str r1, [sp, #16]
	bl Party_RemoveActiveOwnerFar
	movs r0, #1
	bl Party_RemoveActiveOwnerFar
	movs r0, #2
	bl Party_RemoveActiveOwnerFar
	movs r0, #3
	bl Party_RemoveActiveOwnerFar
	movs r0, #5
	bl Party_RemoveActiveOwnerFar
	movs r0, #4
	bl Party_RemoveActiveOwnerFar
	movs r0, #6
	bl Party_RemoveActiveOwnerFar
	movs r0, #7
	bl Party_RemoveActiveOwnerFar
	movs r0, #0
	bl Resource_FarCall005
	movs r2, #148
	ldr r3, [sp, #16]
	lsls r2, r2, #1
	mov r1, sp
	adds r0, r0, r2
	adds r1, #24
	movs r5, #0
	str r3, [r0]
	str r5, [sp, #8]
	str r1, [sp, #4]
	movs r2, #0
	add r3, sp, #36
	mov r12, r1
.L_0811879e:
	str r2, [r3]
	subs r3, #4
	cmp r3, r12
	bge .L_0811879e
	movs r2, #0
	str r2, [sp, #12]
	b .L_081188f0
.L_081187ac:
	ldr r3, [sp, #8]
	adds r3, #1
	str r3, [sp, #8]
	b .L_081188ea
.L_081187b4:
	ldr r1, [sp, #8]
	ldr r2, [sp, #20]
	cmp r1, r2
	bge .L_081187be
	b .L_081188ea
.L_081187be:
	mov r0, r8
	bl Party_AddActiveOwnerFar
	ldr r1, .L_08118830
	mov r0, r8
	adds r3, r5, r1
	ldrb r1, [r3, #1]
	bl Party_AdvanceOwnerCountToTargetFar
	mov r0, r8
	bl Owner_GetState
	movs r5, #140
	adds r1, r0, #0
	adds r2, r1, #0
	lsls r5, r5, #1
	movs r4, #0
	movs r0, #0
	adds r2, #248
	adds r3, r1, r5
	movs r7, #3
.L_081187e8:
	subs r7, #1
	strb r4, [r3]
	strb r4, [r3, #4]
	str r0, [r2]
	str r0, [r2, #16]
	adds r3, #1
	adds r2, #4
	cmp r7, #0
	bge .L_081187e8
	ldr r3, .L_0811882c
	adds r0, r1, #0
	movs r7, #31
	adds r0, #212
.L_08118802:
	subs r7, #1
	strh r3, [r0]
	subs r0, #4
	cmp r7, #0
	bge .L_08118802
	ldr r3, [sp, #12]
	ldr r1, .L_08118830
	add r3, r11
	lsls r3, r3, #3
	adds r3, r1, r3
	adds r5, r3, #0
	adds r5, #18
	movs r7, #1
.L_0811881c:
	ldrh r3, [r5]
	cmp r3, #0
	beq .L_08118834
	adds r1, r3, #0
	mov r0, r8
	bl OwnerAction_AddFar
	b .L_08118834
.L_0811882c:
	.4byte 0x00000000
.L_08118830:
	.4byte Data_0812a174
.L_08118834:
	subs r7, #1
	adds r5, #2
	cmp r7, #0
	bge .L_0811881c
	ldr r3, [sp, #12]
	movs r2, #0
	add r3, r11
	lsls r3, r3, #3
	movs r7, #0
	mov r10, r3
	mov r9, r2
.L_0811884a:
	ldr r2, .L_0811894c
	mov r5, r10
	adds r2, #2
	ldrsb r3, [r2, r5]
	movs r4, #0
	cmp r4, r3
	bge .L_0811889a
	ldr r6, [sp, #4]
	mov r5, r9
.L_0811885c:
	ldr r0, [r6, r5]
	movs r1, #18
	adds r0, #7
	str r4, [sp, #0]
	bl __modsi3
	adds r1, r7, #0
	adds r2, r0, #0
	mov r0, r8
	bl Djinn_AddToOwnerFar
	ldr r0, [r6, r5]
	movs r1, #18
	adds r0, #7
	bl __modsi3
	adds r1, r7, #0
	adds r2, r0, #0
	mov r0, r8
	bl Djinn_ActivateFar
	ldr r3, [r6, r5]
	ldr r1, .L_08118950
	adds r3, #1
	ldr r4, [sp, #0]
	mov r2, r10
	str r3, [r6, r5]
	ldrsb r3, [r1, r2]
	adds r4, #1
	cmp r4, r3
	blt .L_0811885c
.L_0811889a:
	movs r3, #1
	movs r5, #4
	adds r7, #1
	add r10, r3
	add r9, r5
	cmp r7, #3
	ble .L_0811884a
	movs r7, #15
.L_081188aa:
	mov r0, r8
	movs r1, #0
	subs r7, #1
	bl Inventory_RemoveFar
	cmp r7, #0
	bge .L_081188aa
	ldr r3, [sp, #12]
	ldr r2, .L_0811894c
	add r3, r11
	lsls r3, r3, #3
	adds r3, r3, r2
	adds r5, r3, #6
	movs r7, #5
.L_081188c6:
	ldrh r3, [r5]
	cmp r3, #0
	beq .L_081188dc
	adds r1, r3, #0
	mov r0, r8
	bl Inventory_AddItemFar
	adds r1, r0, #0
	mov r0, r8
	bl Func_080ad048
.L_081188dc:
	subs r7, #1
	adds r5, #2
	cmp r7, #0
	bge .L_081188c6
	mov r0, r8
	bl Func_080ad298
.L_081188ea:
	ldr r1, [sp, #12]
	adds r1, #1
	str r1, [sp, #12]
.L_081188f0:
	ldr r2, [sp, #12]
	ldr r3, [sp, #12]
	lsls r2, r2, #1
	mov r11, r2
	add r3, r11
	lsls r5, r3, #3
	ldr r3, .L_0811894c
	ldr r1, [sp, #12]
	ldrsb r3, [r3, r5]
	movs r2, #220
	lsls r2, r2, #1
	mov r8, r3
	cmp r1, r2
	bls .L_08118912
	movs r3, #1
	str r3, [sp, #16]
	b .L_0811893a
.L_08118912:
	movs r1, #1
	negs r1, r1
	cmp r8, r1
	beq .L_0811891c
	b .L_081187b4
.L_0811891c:
	ldr r2, [sp, #8]
	cmp r2, #69
	bgt .L_0811892c
	ldr r3, [sp, #20]
	cmp r2, r3
	beq .L_0811892a
	b .L_081187ac
.L_0811892a:
	b .L_0811893a
.L_0811892c:
	ldr r5, [sp, #20]
	ldr r1, [sp, #8]
	movs r3, #1
	orrs r3, r5
	cmp r1, r3
	beq .L_0811893a
	b .L_081187ac
.L_0811893a:
	ldr r0, [sp, #16]
	add sp, #40
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0811894c:
	.4byte Data_0812a174
.L_08118950:
	.4byte Data_0812a176
