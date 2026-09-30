.syntax unified
	.thumb
	.global Func_0811f330
	.thumb_func
Func_0811f330:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r7, r0, #0
	sub sp, #4
	bl Owner_GetState
	adds r0, r7, #0
	bl GetBattleObjectSlot
	movs r1, #5
	ldr r0, [r0]
	bl Object_SetMode
	movs r3, #1
	mov r6, sp
	mov r8, r3
.L_0811f352:
	movs r3, #255
	adds r0, r6, #0
	strh r3, [r6, #2]
	strh r7, [r6]
	bl Func_080382a0
	adds r0, r7, #0
	bl GetBattleObjectSlot
	movs r1, #7
	ldr r0, [r0]
	bl Func_0811f030
	movs r0, #2
	bl WaitFrames
	adds r0, r6, #0
	strh r7, [r6]
	bl Func_080382a0
	adds r0, r7, #0
	bl GetBattleObjectSlot
	adds r5, r0, #0
	adds r0, r7, #0
	bl Func_0811a484
	adds r1, r0, #0
	ldr r0, [r5]
	bl Func_0811f030
	movs r0, #2
	bl WaitFrames
	movs r3, #1
	negs r3, r3
	add r8, r3
	mov r3, r8
	cmp r3, #0
	bge .L_0811f352
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #36]
	adds r3, #65
	ldrb r0, [r3]
	bl UiWindow_DrawPartyStatusContentsFar
	add sp, #4
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
