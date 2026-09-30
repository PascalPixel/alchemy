.syntax unified
	.thumb
	.global Func_0811b180
	.thumb_func
Func_0811b180:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #4
	mov r11, r2
	mov r9, r1
	str r3, [sp, #0]
	movs r2, #30
	cmp r1, #4
	ble .L_0811b19e
	movs r2, #27
.L_0811b19e:
	mov r3, r9
	subs r3, #1
	muls r3, r2
	lsrs r2, r3, #31
	adds r3, r3, r2
	movs r1, #0
	mov r2, r9
	asrs r7, r3, #1
	mov r10, r1
	cmp r2, #0
	beq .L_0811b286
	adds r6, r0, #0
	mov r8, r1
.L_0811b1b8:
	movs r3, #80
	negs r3, r3
	mov r0, r8
	mov r1, r11
	str r3, [r0, r1]
	mov r3, r10
	movs r2, #0
	cmp r3, #0
	beq .L_0811b23a
	ldrh r3, [r6]
	movs r0, #255
	lsls r0, r0, #8
	adds r0, #2
	adds r3, r3, r0
	movs r1, #128
	lsls r3, r3, #16
	lsls r1, r1, #9
	movs r2, #25
	cmp r3, r1
	bls .L_0811b23a
	ldrh r0, [r6]
	bl Owner_GetState
	movs r2, #165
	lsls r2, r2, #1
	adds r5, r0, #0
	adds r3, r5, r2
	ldrh r0, [r3]
	bl Summon_IsEntryFlagged
	movs r2, #27
	cmp r0, #0
	bne .L_0811b1fc
	movs r2, #38
.L_0811b1fc:
	movs r0, #165
	lsls r0, r0, #1
	adds r3, r5, r0
	ldrh r0, [r3]
	movs r1, #102
	adds r1, #255
	cmp r0, r1
	beq .L_0811b230
	movs r3, #174
	lsls r3, r3, #1
	cmp r0, r3
	beq .L_0811b230
	cmp r0, #91
	beq .L_0811b230
	cmp r0, #92
	beq .L_0811b230
	cmp r0, #93
	beq .L_0811b230
	cmp r0, #94
	beq .L_0811b230
	cmp r0, #95
	beq .L_0811b230
	cmp r0, #96
	beq .L_0811b230
	cmp r0, #97
	bne .L_0811b23a
.L_0811b230:
	movs r3, #50
	negs r3, r3
	mov r0, r8
	mov r1, r11
	str r3, [r0, r1]
.L_0811b23a:
	lsrs r3, r2, #1
	subs r7, r7, r3
	ldr r3, [sp, #0]
	mov r2, r8
	str r7, [r2, r3]
	movs r0, #255
	ldrh r3, [r6]
	lsls r0, r0, #8
	adds r0, #2
	adds r3, r3, r0
	movs r1, #128
	lsls r3, r3, #16
	lsls r1, r1, #9
	movs r2, #25
	cmp r3, r1
	bls .L_0811b274
	ldrh r0, [r6]
	bl Owner_GetState
	movs r2, #165
	lsls r2, r2, #1
	adds r3, r0, r2
	ldrh r0, [r3]
	bl Summon_IsEntryFlagged
	movs r2, #27
	cmp r0, #0
	bne .L_0811b274
	movs r2, #38
.L_0811b274:
	lsrs r3, r2, #1
	movs r0, #1
	subs r7, r7, r3
	add r10, r0
	movs r3, #4
	adds r6, #2
	add r8, r3
	cmp r10, r9
	bne .L_0811b1b8
.L_0811b286:
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
