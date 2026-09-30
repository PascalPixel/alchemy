.syntax unified
	.thumb
	.global Func_0803e918
	.thumb_func
Func_0803e918:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #72]
	adds r6, r0, #0
	movs r1, #0
	adds r0, r5, #0
	bl Menu_LoadSelectionNodeResource
.L_0803e92a:
	movs r0, #1
	bl WaitFrames
	movs r2, #232
	lsls r2, r2, #2
	adds r3, r5, r2
	ldrh r3, [r3]
	cmp r3, #0
	bne .L_0803e92a
	movs r3, #186
	lsls r3, r3, #2
	adds r3, #255
	cmp r6, r3
	beq .L_0803e97e
	ldr r1, .L_0803e994
	movs r2, #16
	ldr r3, [r1, #12]
	ands r3, r2
	cmp r3, #0
	beq .L_0803e95a
	adds r0, r5, #0
	bl Menu_StepRight
	b .L_0803e97e
.L_0803e95a:
	ldr r3, [r1, #12]
	movs r2, #32
	ands r3, r2
	cmp r3, #0
	beq .L_0803e96c
	adds r0, r5, #0
	bl Menu_StepLeft
	b .L_0803e97e
.L_0803e96c:
	ldr r3, [r1, #4]
	movs r2, #1
	ands r3, r2
	cmp r3, #0
	beq .L_0803e97e
	adds r0, r5, #0
	bl Menu_ConfirmSelection
	b .L_0803e992
.L_0803e97e:
	cmp r6, #0
	beq .L_0803e92a
	ldr r3, .L_0803e994
	movs r2, #2
	ldr r3, [r3, #4]
	ands r3, r2
	cmp r3, #0
	beq .L_0803e92a
	movs r0, #1
	negs r0, r0
.L_0803e992:
	pop {r5, r6, pc}
.L_0803e994:
	.4byte gInput
