.syntax unified
	.thumb
	.global BattleParty_ListActorIds
	.thumb_func
BattleParty_ListActorIds:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r6, r0, #0
	ldr r0, [r3, #36]
	movs r3, #1
	ands r3, r6
	movs r5, #0
	cmp r3, #0
	beq .L_0811a35a
	movs r3, #88
	ldrsh r3, [r0, r3]
	cmp r3, #255
	beq .L_0811a35a
	adds r2, r0, #0
	adds r2, #88
.L_0811a33c:
	movs r7, #0
	ldrsh r3, [r2, r7]
	ldrh r4, [r2]
	cmp r3, #254
	beq .L_0811a350
	cmp r1, #0
	beq .L_0811a34e
	strh r4, [r1]
	adds r1, #2
.L_0811a34e:
	adds r5, #1
.L_0811a350:
	adds r2, #2
	movs r4, #0
	ldrsh r3, [r2, r4]
	cmp r3, #255
	bne .L_0811a33c
.L_0811a35a:
	movs r3, #2
	ands r3, r6
	cmp r3, #0
	beq .L_0811a38c
	adds r2, r0, #2
	movs r3, #100
	ldrsh r3, [r2, r3]
	mov r12, r2
	cmp r3, #255
	beq .L_0811a38c
	movs r0, #100
.L_0811a370:
	ldrsh r3, [r2, r0]
	ldrh r4, [r2, r0]
	cmp r3, #254
	beq .L_0811a382
	cmp r1, #0
	beq .L_0811a380
	strh r4, [r1]
	adds r1, #2
.L_0811a380:
	adds r5, #1
.L_0811a382:
	adds r0, #2
	mov r2, r12
	ldrsh r3, [r2, r0]
	cmp r3, #255
	bne .L_0811a370
.L_0811a38c:
	cmp r1, #0
	beq .L_0811a394
	ldr r3, .L_0811a398
	strh r3, [r1]
.L_0811a394:
	adds r0, r5, #0
	pop {r5, r6, r7, pc}
.L_0811a398:
	.4byte 0x000000ff
