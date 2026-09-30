.syntax unified
	.thumb
	.global Func_0810bebc
	.thumb_func
Func_0810bebc:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r7, [r3]
	movs r3, #192
	lsls r3, r3, #4
	ldr r6, .L_0810befc
	adds r3, #200
	sub sp, #4
	mov r9, r0
	mov r10, r1
	mov r11, r2
	movs r4, #14
	adds r5, r7, r3
.L_0810bee6:
	ldrh r3, [r5]
	cmp r3, #96
	beq .L_0810bf00
	adds r0, r3, #0
	str r4, [sp, #0]
	bl Resource_ResetEntry
	strh r6, [r5]
	ldr r4, [sp, #0]
	b .L_0810bf00
	.2byte 0x0000
.L_0810befc:
	.4byte 0x00000060
.L_0810bf00:
	subs r4, #1
	adds r5, #2
	cmp r4, #0
	bge .L_0810bee6
	movs r3, #192
	lsls r3, r3, #4
	adds r3, #20
	adds r6, r7, r3
	mov r3, r10
	movs r4, #0
	cmp r3, #0
	beq .L_0810bf88
	movs r3, #192
	lsls r3, r3, #4
	adds r3, #200
	adds r3, r3, r7
	mov r8, r3
	mov r7, r9
.L_0810bf24:
	ldrh r3, [r7]
	cmp r3, #0
	beq .L_0810bf6a
	str r4, [sp, #0]
	bl Resource_FindFreeEntry
	adds r5, r0, #0
	ldr r4, [sp, #0]
	cmp r5, #95
	bgt .L_0810bf66
	mov r1, r11
	adds r2, r5, #0
	ldrh r0, [r7]
	bl UiIcon_CopyResourceToSlotFar
	movs r3, #128
	adds r2, r6, #4
	lsls r3, r3, #23
	stmia r2!, {r3}
	movs r3, #0
	str r3, [r2]
	ldr r2, .L_0810bf84
	lsls r3, r5, #2
	adds r3, r3, r2
	ldrh r1, [r3, #2]
	ldr r2, .L_0810bf80
	ldrh r3, [r6, #8]
	lsls r1, r1, #17
	lsrs r1, r1, #22
	ands r3, r2
	orrs r3, r1
	strh r3, [r6, #8]
	ldr r4, [sp, #0]
.L_0810bf66:
	mov r3, r8
	strh r5, [r3]
.L_0810bf6a:
	movs r3, #2
	adds r4, #1
	add r8, r3
	adds r7, #2
	adds r6, #12
	cmp r4, #14
	bgt .L_0810bf88
	cmp r4, r10
	bne .L_0810bf24
	b .L_0810bf88
	.2byte 0x0000
.L_0810bf80:
	.4byte 0xfffffc00
.L_0810bf84:
	.4byte ResourceTableEntries
.L_0810bf88:
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
