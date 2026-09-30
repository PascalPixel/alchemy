.syntax unified
	.thumb
	.global Func_0811c314
	.thumb_func
Func_0811c314:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r7, r1, #0
	mov r8, r0
	bl GetBattleObjectSlot
	ldr r5, [r0]
	movs r1, #0
	adds r0, r5, #0
	bl GetMotionRecord
	adds r5, #8
	adds r6, r0, #0
	bl Func_0811bd10
	adds r0, r5, #0
	adds r1, r7, #0
	bl Func_08015778
	ldr r1, [r6, #12]
	ldr r6, .L_0811c378
	mov lr, r6
	.2byte 0xf800
	adds r5, r0, #0
	mov r0, r8
	bl Owner_GetState
	movs r2, #165
	lsls r2, r2, #1
	adds r3, r0, r2
	ldrh r0, [r3]
	bl Func_081280fc
	cmp r0, #0
	beq .L_0811c362
	adds r0, r5, #0
	movs r1, #24
	b .L_0811c366
.L_0811c362:
	adds r0, r5, #0
	movs r1, #48
.L_0811c366:
	mov lr, r6
	.2byte 0xf800
	ldr r3, [r7, #4]
	subs r3, r3, r0
	str r3, [r7, #4]
	movs r0, #0
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_0811c378:
	.4byte IwramMulQ16
