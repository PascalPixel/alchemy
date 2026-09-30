.syntax unified
	.thumb
	.global Func_0803d450
	.thumb_func
Func_0803d450:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r6, [r3, #60]
	movs r2, #161
	lsls r2, r2, #3
	adds r5, r6, r2
	bl Localization_LookupEntryId
	movs r3, #1
	negs r3, r3
	cmp r0, r3
	beq .L_0803d4b8
	movs r2, #152
	lsls r2, r2, #5
	adds r2, #126
	adds r3, r6, r2
	ldrh r3, [r3]
	cmp r3, r0
	bne .L_0803d486
	movs r3, #1
	b .L_0803d496
.L_0803d47c:
	adds r0, r5, #0
	movs r1, #2
	bl UiWork_Finalize
	b .L_0803d4b8
.L_0803d486:
	movs r2, #152
	lsls r2, r2, #5
	adds r2, #124
	adds r3, r6, r2
	ldrh r3, [r3]
	cmp r3, r0
	bne .L_0803d4b8
	movs r3, #0
.L_0803d496:
	movs r2, #156
	lsls r3, r3, #1
	lsls r2, r2, #5
	adds r3, r3, r2
	ldrh r0, [r6, r3]
	movs r1, #0
.L_0803d4a2:
	ldr r2, [r5]
	ldrb r3, [r2, #4]
	cmp r3, #2
	bne .L_0803d4b0
	ldrb r3, [r2, #14]
	cmp r3, r0
	beq .L_0803d47c
.L_0803d4b0:
	adds r1, #1
	adds r5, #36
	cmp r1, #12
	bne .L_0803d4a2
.L_0803d4b8:
	pop {r5, r6, pc}
	.2byte 0x0000
