.syntax unified
	.thumb
	.global Func_0811d9a4
	.thumb_func
Func_0811d9a4:
	push {r5, r6, lr}
	sub sp, #28
	mov r5, sp
	movs r0, #3
	adds r1, r5, #0
	bl BattleParty_ListActorIds
	cmp r0, #0
	ble .L_0811d9c8
	adds r6, r5, #0
	adds r5, r0, #0
.L_0811d9ba:
	ldrh r0, [r6]
	subs r5, #1
	adds r6, #2
	bl Actor_ResetMotionAtAnchor
	cmp r5, #0
	bne .L_0811d9ba
.L_0811d9c8:
	add sp, #28
	pop {r5, r6, pc}
