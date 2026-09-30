.syntax unified
	.thumb
	.global Func_0811c984
	.thumb_func
Func_0811c984:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #176
	ldr r2, [r3]
	movs r1, #128
	ldr r3, [r2]
	lsls r1, r1, #6
	sub sp, #88
	adds r6, r0, #0
	cmp r3, r1
	bne .L_0811c9a6
	str r1, [r2]
	movs r0, #10
	bl WaitFrames
	b .L_0811c9ae
.L_0811c9a6:
	str r1, [r2]
	movs r0, #30
	bl WaitFrames
.L_0811c9ae:
	movs r3, #0
	ldrsh r0, [r6, r3]
	mov r5, sp
	str r0, [r5, #8]
	bl Func_0811c650
	cmp r0, #0
	blt .L_0811c9ce
	ldrh r0, [r6, #10]
	strh r0, [r5, #36]
	lsls r0, r0, #16
	asrs r0, r0, #16
	bl Func_0811c650
	cmp r0, #0
	bge .L_0811c9d4
.L_0811c9ce:
	movs r0, #1
	negs r0, r0
	b .L_0811ca4a
.L_0811c9d4:
	ldr r0, [r5, #8]
	bl Owner_GetState
	movs r3, #36
	ldrsh r0, [r5, r3]
	bl Owner_GetState
	bl Random16
	movs r1, #1
	ldr r0, [r5, #8]
	bl UiText_DrawQuantity
	ldr r0, .L_0811ca50
	bl Func_080381c0 + 0x8
	movs r2, #13
	movs r3, #36
	ldrsh r1, [r5, r3]
	ldr r0, [r5, #8]
	movs r3, #0
	bl Func_0811c120
	ldr r0, [r5, #8]
	bl GetBattleObjectSlot
	movs r1, #16
	ldr r0, [r0]
	bl ObjectDispatch_ApplyValueToChildrenFar
	movs r3, #36
	ldrsh r0, [r5, r3]
	bl GetBattleObjectSlot
	ldrh r3, [r5, #36]
	movs r2, #1
	str r2, [r5, #20]
	cmp r3, #7
	bhi .L_0811ca26
	str r2, [r5, #4]
	b .L_0811ca2a
.L_0811ca26:
	movs r3, #0
	str r3, [r5, #4]
.L_0811ca2a:
	movs r3, #0
	movs r0, #4
	str r3, [r5, #28]
	bl WaitFrames
	adds r0, r5, #0
	bl Resource_FarCall00C + 0x8
	movs r3, #36
	ldrsh r0, [r5, r3]
	bl Actor_ResetMotionAtAnchor
	ldr r0, [r5, #8]
	bl Actor_ResetMotionAtAnchor
	movs r0, #0
.L_0811ca4a:
	add sp, #88
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0811ca50:
	.4byte 0x00000c60
