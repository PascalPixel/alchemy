.syntax unified
	.thumb
	.global BattleActor_CommitPlacement
	.thumb_func
BattleActor_CommitPlacement:
	push {r5, lr}
	sub sp, #28
	mov r5, sp
	adds r1, r5, #0
	movs r0, #3
	bl BattleParty_ListActorIds
	adds r0, r5, #0
	movs r1, #1
	bl BattleActor_SpawnObjectsForList
	add sp, #28
	pop {r5, pc}
	.2byte 0x0000
