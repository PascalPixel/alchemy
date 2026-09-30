.syntax unified
	.thumb
	.global Func_081053a8
	.thumb_func
Func_081053a8:
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
	ldr r3, [r3]
	ldr r2, .L_08105420
	movs r6, #176
	movs r7, #192
	lsls r6, r6, #4
	lsls r7, r7, #4
	mov r10, r3
	adds r6, #204
	mov r11, r2
	adds r7, #8
	movs r3, #0
	movs r2, #4
	add r6, r10
	add r7, r10
	mov r9, r3
	mov r8, r2
.L_081053dc:
	bl Resource_FindFreeEntry
	adds r5, r0, #0
	cmp r5, #95
	bgt .L_08105428
	mov r3, r9
	mov r2, r11
	ldrh r1, [r3, r2]
	movs r0, #8
	adds r2, r5, #0
	movs r3, #0
	bl UiWindow_SetTilemapEntryFar + 0x18
	movs r3, #128
	adds r2, r6, #4
	lsls r3, r3, #23
	stmia r2!, {r3}
	movs r3, #0
	str r3, [r2]
	ldr r2, .L_08105424
	lsls r3, r5, #2
	adds r3, r3, r2
	ldrh r1, [r3, #2]
	ldr r2, .L_0810541c
	ldrh r3, [r6, #8]
	lsls r1, r1, #17
	lsrs r1, r1, #22
	ands r3, r2
	orrs r3, r1
	strh r3, [r6, #8]
	b .L_08105428
	.2byte 0x0000
.L_0810541c:
	.4byte 0xfffffc00
.L_08105420:
	.4byte Data_08105a50
.L_08105424:
	.4byte ResourceTableEntries
.L_08105428:
	movs r2, #1
	negs r2, r2
	movs r3, #2
	add r8, r2
	add r9, r3
	mov r3, r8
	strh r5, [r7]
	adds r6, #12
	adds r7, #2
	cmp r3, #0
	bge .L_081053dc
	movs r2, #192
	lsls r2, r2, #4
	adds r2, #18
	movs r1, #128
	add r2, r10
	movs r3, #1
	lsls r1, r1, #3
	strb r3, [r2]
	adds r1, #138
	ldr r0, .L_08105464
	bl Func_080145a8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_08105464:
	.4byte Func_08105370
