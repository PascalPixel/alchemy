.syntax unified
	.thumb
	.global Func_08100738
	.thumb_func
Func_08100738:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	adds r7, r1, #0
	mov r11, r0
	adds r0, r7, #0
	sub sp, #4
	mov r9, r2
	bl Owner_GetState
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r3, [r3]
	mov r1, r11
	str r3, [sp, #0]
	movs r2, #128
	lsls r3, r1, #1
	adds r5, r0, #0
	adds r3, #216
	lsls r2, r2, #1
	adds r2, #255
	mov r8, r3
	ldrh r3, [r5, r3]
	mov r10, r2
	mov r1, r10
	ands r1, r3
	mov r10, r1
	mov r0, r10
	bl Item_Get
	adds r6, r0, #0
	ldrh r3, [r6, #40]
	movs r0, #252
	lsls r0, r0, #6
	adds r0, #255
	ands r0, r3
	mov r2, r9
	adds r1, r7, #0
	movs r3, #1
	bl BattleEffect_ApplyToTargets
	movs r2, #1
	negs r2, r2
	cmp r0, r2
	beq .L_081007ea
	mov r3, r8
	ldrh r0, [r5, r3]
	bl Item_Get
	adds r6, r0, #0
	ldrb r2, [r6, #12]
	adds r3, r2, #0
	cmp r3, #1
	bne .L_081007d2
	mov r1, r11
	adds r0, r7, #0
	bl Inventory_RemoveFar
	ldr r2, [sp, #0]
	movs r3, #226
	lsls r3, r3, #1
	adds r1, r2, r3
	adds r0, r5, #0
	movs r2, #0
	bl ItemMenu_Collect
	ldr r1, [sp, #0]
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r1, r2
	strb r0, [r3]
	ldrb r2, [r6, #12]
.L_081007d2:
	adds r3, r2, #0
	cmp r3, #4
	bne .L_081007e8
	mov r3, r10
	cmp r3, #184
	bne .L_081007e2
	movs r1, #185
	mov r10, r1
.L_081007e2:
	mov r3, r10
	mov r2, r8
	strh r3, [r5, r2]
.L_081007e8:
	movs r0, #0
.L_081007ea:
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
