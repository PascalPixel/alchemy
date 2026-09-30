.syntax unified
	.thumb
	.global UiText_DrawCharacterAtOffset
	.thumb_func
UiText_DrawCharacterAtOffset:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	adds r6, r3, #0
	movs r3, #192
	lsls r3, r3, #18
	ldr r7, [r3, #60]
	mov r10, r1
	movs r1, #152
	mov r9, r2
	lsls r1, r1, #5
	movs r2, #0
	mov r8, r2
	adds r1, #66
	adds r5, r7, r1
	mov r3, r8
	movs r1, #1
	strh r3, [r5]
	bl UiText_BuildRenderEntries
	ldrh r3, [r5]
	movs r4, #244
	lsls r4, r4, #4
	lsls r3, r3, #1
	adds r3, r3, r4
	mov r0, r8
	strh r0, [r7, r3]
	ldr r2, .L_08042084
	ldrh r3, [r5]
	lsrs r6, r6, #3
	adds r3, #1
	ands r3, r2
	strh r3, [r5]
	mov r2, r10
	movs r1, #14
	ldrsh r3, [r2, r1]
	movs r4, #12
	ldrsh r2, [r2, r4]
	adds r3, r3, r6
	mov r0, r9
	lsrs r1, r0, #3
	adds r3, #1
	adds r2, r2, r1
	lsls r3, r3, #5
	adds r3, r3, r2
	movs r2, #160
	adds r1, r3, #1
	lsls r2, r2, #2
	cmp r1, r2
	bcs .L_0804209e
	ldr r3, .L_08042088
	movs r4, #244
	lsls r1, r1, #1
	lsls r4, r4, #4
	b .L_0804208c
	.2byte 0x0000
.L_08042084:
	.4byte 0x000001ff
.L_08042088:
	.4byte 0x06002000
.L_0804208c:
	adds r2, r1, r3
	adds r0, r7, r4
	adds r1, r7, r1
	movs r3, #7
	mov r4, r9
	adds r1, #8
	ands r3, r4
	bl Func_080416cc
.L_0804209e:
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
