.syntax unified
	.thumb
	.global Func_08126ba0
	.thumb_func
Func_08126ba0:
	push {r5, r6, r7, lr}
	sub sp, #28
	mov r5, sp
	movs r0, #3
	adds r1, r5, #0
	bl BattleParty_ListActorIds
	cmp r0, #0
	ble .L_08126bc8
	adds r7, r5, #0
	movs r6, #0
	adds r5, r0, #0
.L_08126bb8:
	ldrsh r0, [r6, r7]
	movs r1, #0
	subs r5, #1
	bl BattlePres_SetActorRecordMode
	adds r6, #2
	cmp r5, #0
	bne .L_08126bb8
.L_08126bc8:
	add sp, #28
	pop {r5, r6, r7, pc}
