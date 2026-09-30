.syntax unified
	.thumb
	.global Func_0814cc4c
	.thumb_func
Func_0814cc4c:
	push {r5, r6, r7, lr}
	adds r5, r0, #0
	ldr r3, [r5, #20]
	sub sp, #40
	movs r4, #0
	movs r2, #0
	cmp r3, #0
	beq .L_0814cc90
	add r1, sp, #12
	movs r6, #36
	adds r7, r1, #0
.L_0814cc62:
	ldrsh r0, [r6, r5]
	str r1, [sp, #8]
	str r2, [sp, #4]
	str r4, [sp, #0]
	bl Owner_GetState
	movs r3, #56
	ldrsh r0, [r0, r3]
	ldr r1, [sp, #8]
	ldr r2, [sp, #4]
	ldr r4, [sp, #0]
	cmp r0, #0
	ble .L_0814cc84
	ldrh r3, [r6, r5]
	adds r4, #1
	strh r3, [r7]
	adds r7, #2
.L_0814cc84:
	ldr r3, [r5, #20]
	adds r2, #1
	adds r6, #2
	cmp r2, r3
	bne .L_0814cc62
	b .L_0814cc92
.L_0814cc90:
	add r1, sp, #12
.L_0814cc92:
	ldr r3, .L_0814cca4
	lsls r2, r4, #1
	strh r3, [r1, r2]
	adds r0, r1, #0
	movs r1, #0
	bl BattleActor_SpawnObjectsForListFar
	add sp, #40
	pop {r5, r6, r7, pc}
.L_0814cca4:
	.4byte 0x000000ff
