.syntax unified
	.thumb
	.global Func_08119734
	.thumb_func
Func_08119734:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	movs r5, #118
	ldr r6, [r3, #36]
	adds r5, #255
	cmp r0, #78
	beq .L_08119748
	movs r5, #188
	lsls r5, r5, #1
.L_08119748:
	movs r0, #128
	bl ActivateBattleObjectSlot
	bl Func_0811bc98
	movs r1, #1
	adds r0, r5, #0
	bl Summon_TakeCharge
	adds r1, r5, #0
	movs r2, #255
	movs r0, #128
	bl BattleUnit_AssignFar
	adds r2, r6, #0
	adds r2, #102
	movs r3, #128
	strh r3, [r2]
	adds r2, #2
	movs r3, #255
	strh r3, [r2]
	bl Summon_Refresh
	movs r0, #128
	bl GetBattleObjectSlot
	ldr r2, [r0, #12]
	cmp r2, #0
	bge .L_0811978a
	movs r1, #255
	lsls r1, r1, #8
	adds r1, #255
	adds r2, r2, r1
.L_0811978a:
	ldr r3, [r0, #16]
	asrs r2, r2, #16
	cmp r3, #0
	bge .L_0811979a
	movs r1, #255
	lsls r1, r1, #8
	adds r1, #255
	adds r3, r3, r1
.L_0811979a:
	asrs r3, r3, #16
	movs r1, #128
	bl BattlePresentation_SpawnActorObject
	bl BattleActor_CommitPlacement
	pop {r5, r6, pc}
