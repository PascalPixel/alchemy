.syntax unified
	.thumb
	.global Func_0811ce50
	.thumb_func
Func_0811ce50:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #28
	mov r5, sp
	movs r0, #3
	adds r1, r5, #0
	bl BattleParty_ListActorIds
	cmp r0, #0
	ble .L_0811ce8c
	movs r6, #0
	adds r7, r5, #0
	mov r8, r6
	adds r5, r0, #0
.L_0811ce6e:
	ldrh r0, [r6, r7]
	bl Owner_GetState
	movs r2, #44
	adds r2, #255
	adds r3, r0, r2
	mov r2, r8
	ldrh r0, [r6, r7]
	strb r2, [r3]
	subs r5, #1
	bl Owner_RecalculateStatsFar
	adds r6, #2
	cmp r5, #0
	bne .L_0811ce6e
.L_0811ce8c:
	add sp, #28
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
