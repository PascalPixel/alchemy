.syntax unified
	.thumb
	.global Func_080fa50c
	.thumb_func
Func_080fa50c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r7, [r3]
	mov r10, r0
	mov r6, r10
	movs r1, #0
	adds r6, #28
	ldr r0, [r7, #48]
	mov r8, r1
	ldrsb r5, [r7, r6]
	bl RenderOutput_ClearListFar
	movs r1, #139
	lsls r1, r1, #1
	adds r1, #255
	adds r3, r7, r1
	ldrb r3, [r3]
	adds r2, r7, #2
	strb r3, [r2, r6]
	movs r2, #1
	negs r2, r2
	cmp r5, r2
	bne .L_080fa54c
	mov r3, r8
	strb r3, [r7, r6]
	movs r6, #0
	b .L_080fa55a
.L_080fa54c:
	lsls r6, r5, #1
	adds r0, r6, r5
	lsls r0, r0, #3
	subs r0, #10
	movs r1, #16
	bl Func_080f8ab4
.L_080fa55a:
	movs r5, #129
	lsls r5, r5, #2
	adds r3, r6, r5
	ldrh r0, [r7, r3]
	bl Owner_GetState
	movs r1, #226
	lsls r1, r1, #1
	adds r6, r7, r1
	adds r1, r6, #0
	movs r2, #0
	bl Func_080fad88
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r7, r2
	adds r5, r7, r5
	strb r0, [r3]
	adds r1, r6, #0
	adds r0, r5, #0
	bl Func_080fa5a4
	mov r1, r10
	lsls r3, r1, #2
	adds r3, #20
	mov r8, r0
	ldr r0, [r7, r3]
	bl UiIcon_PrepareObject
	movs r0, #1
	bl WaitFrames
	mov r0, r8
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
