.syntax unified
	.thumb
	.global OwnerAction_Add
	.thumb_func
OwnerAction_Add:
	push {r5, r6, r7, lr}
	adds r6, r1, #0
	adds r7, r0, #0
	bl Owner_GetState
	movs r3, #252
	lsls r3, r3, #6
	adds r3, #255
	adds r5, r0, #0
	movs r0, #88
	mov r12, r3
	ands r6, r3
	ldrh r3, [r0, r5]
	mov r2, r12
	movs r4, #1
	ands r2, r3
	negs r4, r4
	movs r1, #0
	cmp r2, r6
	bne .L_080af722
	strh r2, [r0, r5]
	movs r4, #0
	b .L_080af738
.L_080af722:
	adds r1, #1
	adds r0, #4
	cmp r1, #30
	bgt .L_080af738
	ldrh r3, [r0, r5]
	mov r2, r12
	ands r2, r3
	cmp r2, r6
	bne .L_080af722
	strh r2, [r0, r5]
	adds r4, r1, #0
.L_080af738:
	cmp r4, #0
	bge .L_080af76c
	movs r2, #88
	ldrh r3, [r5, r2]
	movs r1, #0
	cmp r3, #0
	bne .L_080af74c
	strh r6, [r5, r2]
	movs r4, #0
	b .L_080af762
.L_080af74c:
	adds r1, #1
	cmp r1, #30
	bgt .L_080af762
	lsls r3, r1, #2
	adds r2, r3, #0
	adds r2, #88
	ldrh r3, [r5, r2]
	cmp r3, #0
	bne .L_080af74c
	strh r6, [r5, r2]
	adds r4, r1, #0
.L_080af762:
	cmp r4, #0
	bge .L_080af76c
	movs r0, #1
	negs r0, r0
	b .L_080af790
.L_080af76c:
	adds r0, r7, #0
	bl Owner_RefreshClassActions
	movs r3, #88
	ldrh r3, [r5, r3]
	movs r1, #0
	cmp r3, r6
	beq .L_080af78e
	adds r0, r5, #0
	adds r0, #88
.L_080af780:
	adds r1, #1
	cmp r1, #31
	bgt .L_080af78e
	adds r0, #4
	ldrh r3, [r0]
	cmp r3, r6
	bne .L_080af780
.L_080af78e:
	adds r0, r1, #0
.L_080af790:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
