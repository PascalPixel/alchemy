.syntax unified
	.thumb
	.global Menu_CenterResourceEntries
	.thumb_func
Menu_CenterResourceEntries:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #232
	ldr r7, [r3]
	movs r3, #144
	adds r3, r3, r7
	mov r8, r3
	adds r3, r7, #0
	mov r10, r0
	adds r1, #2
	mov r5, r8
	adds r3, #146
	strh r1, [r5]
	mov r6, r10
	strh r2, [r3]
	adds r3, #2
	strh r6, [r3]
	movs r1, #142
	adds r1, r1, r7
	movs r2, #0
	ldrsh r6, [r1, r2]
	mov r9, r1
	mov r1, r8
	movs r3, #0
	ldrsh r0, [r1, r3]
	movs r1, #3
	lsls r0, r0, #1
	sub sp, #4
	bl __divsi3
	lsls r5, r6, #1
	adds r5, r5, r6
	adds r5, r5, r0
	lsrs r3, r5, #31
	adds r5, r5, r3
	asrs r5, r5, #1
	movs r3, #15
	movs r1, #0
	subs r0, r3, r5
	cmp r1, r6
	bge .L_0804d462
	mov r2, r10
	lsls r4, r2, #3
	mov r12, r9
	adds r2, r7, #0
.L_0804d44c:
	lsls r3, r0, #3
	strh r3, [r2, #12]
	strh r4, [r2, #14]
	mov r6, r12
	movs r5, #0
	ldrsh r3, [r6, r5]
	adds r1, #1
	adds r0, #3
	adds r2, #20
	cmp r1, r3
	blt .L_0804d44c
.L_0804d462:
	mov r3, r8
	movs r1, #0
	ldrsh r2, [r3, r1]
	movs r3, #2
	str r3, [sp, #0]
	mov r1, r10
	movs r3, #3
	bl UiWindow_Create
	str r0, [r7, #120]
	add sp, #4
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
