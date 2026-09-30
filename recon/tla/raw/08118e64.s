.syntax unified
	.thumb
	.global Func_08118e64
	.thumb_func
Func_08118e64:
	push {r5, r6, r7, lr}
	sub sp, #32
	mov r6, sp
	adds r0, r6, #0
	bl Func_0811a038
	adds r5, r0, #0
	lsls r0, r5, #1
	adds r0, r6, r0
	bl Func_0811a0b0
	adds r5, r5, r0
	lsls r1, r5, #1
	adds r1, r6, r1
	movs r0, #2
	bl BattleParty_ListActorIds
	adds r5, r5, r0
	cmp r5, #0
	ble .L_08118eac
	movs r7, #0
.L_08118e8e:
	ldrh r0, [r6]
	bl Owner_GetState
	movs r1, #44
	adds r2, r0, #0
	adds r1, #255
	adds r3, r2, r1
	adds r1, #26
	strb r7, [r3]
	subs r5, #1
	adds r3, r2, r1
	adds r6, #2
	strb r7, [r3]
	cmp r5, #0
	bne .L_08118e8e
.L_08118eac:
	add sp, #32
	pop {r5, r6, r7, pc}
