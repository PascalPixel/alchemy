.syntax unified
	.thumb
	.global Func_0803ea54
	.thumb_func
Func_0803ea54:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r1, #192
	adds r5, r0, #0
	lsls r1, r1, #2
	movs r0, #231
	adds r1, #158
	lsls r0, r0, #2
	adds r7, r5, r0
	adds r6, r5, r1
	ldrh r3, [r7]
	ldrh r1, [r6]
	movs r2, #229
	adds r3, r3, r1
	lsls r2, r2, #2
	adds r3, #1
	adds r2, r2, r5
	mov r10, r3
	ldrh r3, [r2]
	mov r8, r2
	cmp r10, r3
	beq .L_0803eb22
	adds r0, r5, #0
	bl Menu_ReloadNodeResource
	movs r3, #192
	lsls r3, r3, #2
	adds r3, #162
	adds r2, r5, r3
	movs r3, #33
	strh r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldrh r1, [r6]
	movs r0, #128
	adds r3, r1, #1
	strh r3, [r6]
	lsls r0, r0, #11
	lsls r3, r3, #16
	cmp r3, r0
	bne .L_0803eaea
	mov r0, r8
	ldrh r3, [r0]
	mov r2, r10
	adds r2, #1
	cmp r2, r3
	bcs .L_0803eaea
	movs r2, #128
	lsls r2, r2, #9
	adds r3, r1, r2
	strh r3, [r6]
	movs r3, #8
	strh r3, [r5, #60]
	ldrh r3, [r7]
	adds r0, r5, #0
	adds r3, #1
	strh r3, [r7]
	movs r1, #1
	bl Menu_ScrollSelectionList
	ldrh r2, [r6]
	ldrh r3, [r7]
	mov r0, r8
	adds r3, r3, r2
	ldrh r2, [r0]
	adds r3, #2
	cmp r3, r2
	bne .L_0803eae6
	movs r3, #0
	strh r3, [r5, #62]
.L_0803eae6:
	movs r3, #1
	strh r3, [r5, #10]
.L_0803eaea:
	movs r1, #192
	lsls r1, r1, #2
	adds r1, #162
	adds r2, r5, r1
	movs r3, #1
	strh r3, [r2]
	movs r2, #192
	lsls r2, r2, #2
	adds r2, #158
	adds r3, r5, r2
	ldrh r1, [r3]
	adds r0, r5, #0
	bl Menu_LoadSelectionNodeResource
	movs r0, #1
	bl WaitFrames
	movs r0, #210
	lsls r0, r0, #2
	adds r3, r5, r0
	ldr r3, [r3]
	movs r1, #0
	ldrh r0, [r3, #10]
	bl Menu_OpenSelectionWindow
	movs r0, #1
	bl WaitFrames
.L_0803eb22:
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
