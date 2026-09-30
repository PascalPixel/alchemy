.syntax unified
	.thumb
	.global Inventory_Remove
	.thumb_func
Inventory_Remove:
	push {r5, r6, r7, lr}
	adds r5, r1, #0
	adds r7, r0, #0
	bl Owner_GetState
	lsls r5, r5, #1
	adds r5, #216
	ldrh r3, [r0, r5]
	movs r6, #1
	negs r6, r6
	cmp r3, #0
	beq .L_080af1be
	movs r2, #248
	lsls r2, r2, #8
	ands r2, r3
	cmp r2, #0
	beq .L_080af178
	ldr r2, .L_080af174
	movs r6, #1
	adds r3, r3, r2
	strh r3, [r0, r5]
	b .L_080af1be
.L_080af174:
	.4byte 0xfffff800
.L_080af178:
	adds r6, r0, #0
	adds r6, #216
	strh r2, [r0, r5]
	adds r4, r6, #0
	movs r5, #0
	adds r1, r6, #0
	movs r0, #14
.L_080af186:
	ldrh r2, [r4]
	adds r4, #2
	lsls r3, r2, #16
	cmp r3, #0
	beq .L_080af196
	strh r2, [r1]
	adds r5, #1
	adds r1, #2
.L_080af196:
	subs r0, #1
	cmp r0, #0
	bge .L_080af186
	cmp r5, #14
	bgt .L_080af1bc
	lsls r3, r5, #1
	ldr r2, .L_080af1b8
	adds r0, r3, r6
	movs r3, #15
	subs r5, r3, r5
.L_080af1aa:
	subs r5, #1
	strh r2, [r0]
	adds r0, #2
	cmp r5, #0
	bne .L_080af1aa
	b .L_080af1bc
	.2byte 0x0000
.L_080af1b8:
	.4byte 0x00000000
.L_080af1bc:
	movs r6, #2
.L_080af1be:
	adds r0, r7, #0
	bl Owner_RecalculateStats
	adds r0, r6, #0
	pop {r5, r6, r7, pc}
