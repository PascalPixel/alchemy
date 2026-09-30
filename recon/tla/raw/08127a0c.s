.syntax unified
	.thumb
	.global Func_08127a0c
	.thumb_func
Func_08127a0c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	mov r10, r0
	movs r0, #36
	bl Runtime_BumpAllocateAlternatePool
	mov r8, r0
	mov r0, r10
	bl Owner_GetState
	adds r7, r0, #0
	adds r6, r7, #0
	adds r6, #16
	adds r1, r6, #0
	ldr r3, .L_08127ad8
	movs r2, #36
	mov r0, r8
	mov lr, r3
	.2byte 0xf800
	movs r1, #0
	ldrsh r2, [r6, r1]
	mov r1, r8
	lsls r3, r2, #1
	adds r3, r3, r2
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r5, r3, #1
	movs r2, #0
	ldrsh r3, [r1, r2]
	movs r1, #10
	lsls r0, r3, #3
	subs r0, r0, r3
	bl __divsi3
	cmp r5, r0
	bge .L_08127a5a
	adds r5, r0, #0
.L_08127a5a:
	movs r3, #252
	lsls r3, r3, #6
	adds r3, #255
	cmp r5, r3
	ble .L_08127a66
	adds r5, r3, #0
.L_08127a66:
	strh r5, [r6]
	ldrh r2, [r7, #24]
	lsls r3, r2, #2
	adds r3, r3, r2
	cmp r3, #0
	bge .L_08127a74
	adds r3, #3
.L_08127a74:
	mov r2, r8
	asrs r5, r3, #2
	ldrh r3, [r2, #8]
	movs r1, #10
	lsls r0, r3, #3
	subs r0, r0, r3
	bl __divsi3
	cmp r5, r0
	bge .L_08127a8a
	adds r5, r0, #0
.L_08127a8a:
	movs r6, #186
	lsls r6, r6, #2
	adds r6, #255
	cmp r5, r6
	ble .L_08127a96
	adds r5, r6, #0
.L_08127a96:
	ldrh r2, [r7, #26]
	strh r5, [r7, #24]
	lsls r3, r2, #2
	adds r3, r3, r2
	cmp r3, #0
	bge .L_08127aa4
	adds r3, #3
.L_08127aa4:
	mov r1, r8
	asrs r5, r3, #2
	ldrh r3, [r1, #10]
	movs r1, #10
	lsls r0, r3, #3
	subs r0, r0, r3
	bl __divsi3
	cmp r5, r0
	bge .L_08127aba
	adds r5, r0, #0
.L_08127aba:
	cmp r5, r6
	ble .L_08127ac0
	adds r5, r6, #0
.L_08127ac0:
	mov r0, r10
	strh r5, [r7, #26]
	bl BattleUnit_Recalculate
	mov r0, r8
	bl Sys_Free
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_08127ad8:
	.4byte IwramCopyWords
