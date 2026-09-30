.syntax unified
	.thumb
	.global Func_080fae8c
	.thumb_func
Func_080fae8c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	adds r6, r3, #0
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	mov r9, r1
	movs r1, #0
	sub sp, #4
	mov r11, r2
	ldr r7, [r3]
	mov r10, r1
	adds r5, r0, #0
	bl Owner_GetState
	mov r2, r9
	str r0, [sp, #0]
	lsls r3, r2, #1
	adds r3, #216
	ldrh r3, [r0, r3]
	movs r0, #128
	lsls r0, r0, #1
	adds r0, #255
	ands r0, r3
	mov r8, r3
	bl Item_Get
	mov r3, r11
	cmp r3, #1
	bne .L_080faed8
	movs r1, #128
	lsls r1, r1, #1
	mov r10, r1
.L_080faed8:
	ldrb r0, [r0, #2]
	cmp r0, #11
	bls .L_080faee0
	b .L_080fb08a
.L_080faee0:
	ldr r2, .L_080fb098
	lsls r3, r0, #2
	ldr r3, [r3, r2]
	mov pc, r3
.L_080faee8:
	.4byte .L_080fb004
	.4byte .L_080faf70
	.4byte .L_080faf70
	.4byte .L_080faf70
	.4byte .L_080faf70
	.4byte .L_080faf70
	.4byte .L_080faff8
	.4byte .L_080faf70
	.4byte .L_080faf70
	.4byte .L_080faf70
	.4byte .L_080faf18
	.4byte .L_080fb004
.L_080faf18:
	cmp r5, r6
	bne .L_080faf20
	movs r3, #9
	b .L_080faffe
.L_080faf20:
	adds r0, r6, #0
	bl Owner_GetState
	str r0, [sp, #0]
	movs r0, #166
	lsls r0, r0, #1
	bl Runtime_BumpAllocate
	movs r2, #166
	lsls r2, r2, #1
	ldr r3, .L_080fb09c
	ldr r1, [sp, #0]
	mov r11, r0
	mov lr, r3
	.2byte 0xf800
	adds r0, r6, #0
	bl Inventory_RemoveFirstUnflagged
	adds r2, r0, #0
	cmp r2, #0
	beq .L_080fafd6
	ldr r3, .L_080fb0a0
	mov r1, r8
	ands r1, r3
	adds r0, r6, #0
	mov r8, r1
	bl Inventory_AddItemFar
	movs r3, #1
	adds r2, r0, #0
	negs r3, r3
	cmp r2, r3
	beq .L_080fafc8
	movs r3, #9
	mov r1, r10
	orrs r1, r3
	mov r10, r1
	ldr r0, [r7, #40]
	adds r1, r6, #0
	b .L_080fafce
.L_080faf70:
	cmp r5, r6
	bne .L_080faf78
	movs r3, #2
	b .L_080faffe
.L_080faf78:
	adds r0, r6, #0
	bl Owner_GetState
	str r0, [sp, #0]
	movs r0, #166
	lsls r0, r0, #1
	bl Runtime_BumpAllocate
	movs r2, #166
	lsls r2, r2, #1
	ldr r3, .L_080fb09c
	ldr r1, [sp, #0]
	mov r11, r0
	mov lr, r3
	.2byte 0xf800
	adds r0, r6, #0
	bl Inventory_RemoveFirstUnflagged
	adds r2, r0, #0
	cmp r2, #0
	beq .L_080fafd6
	ldr r3, .L_080fb0a0
	mov r1, r8
	ands r1, r3
	adds r0, r6, #0
	mov r8, r1
	bl Inventory_AddItemFar
	movs r3, #1
	adds r2, r0, #0
	negs r3, r3
	cmp r2, r3
	beq .L_080fafc8
	movs r3, #2
	mov r1, r10
	orrs r1, r3
	mov r10, r1
	ldr r0, [r7, #40]
	adds r1, r6, #0
	b .L_080fafce
.L_080fafc8:
	ldr r0, [r7, #40]
	adds r1, r6, #0
	mov r2, r9
.L_080fafce:
	mov r3, r10
	bl Func_080f8170
	b .L_080fafe2
.L_080fafd6:
	ldr r0, [r7, #40]
	adds r1, r6, #0
	mov r2, r9
	mov r3, r10
	bl Func_080f8170
.L_080fafe2:
	movs r2, #166
	ldr r3, .L_080fb09c
	ldr r0, [sp, #0]
	mov r1, r11
	lsls r2, r2, #1
	mov lr, r3
	.2byte 0xf800
	mov r0, r11
	bl Sys_Free
	b .L_080fb08a
.L_080faff8:
	cmp r6, r5
	bne .L_080fb012
	movs r3, #4
.L_080faffe:
	mov r2, r10
	orrs r2, r3
	mov r10, r2
.L_080fb004:
	ldr r0, [r7, #40]
	adds r1, r6, #0
	mov r2, r9
	mov r3, r10
	bl Func_080f8170
	b .L_080fb08a
.L_080fb012:
	adds r0, r6, #0
	bl Owner_GetState
	str r0, [sp, #0]
	movs r0, #166
	lsls r0, r0, #1
	bl Runtime_BumpAllocate
	movs r2, #166
	lsls r2, r2, #1
	ldr r3, .L_080fb09c
	ldr r1, [sp, #0]
	mov r11, r0
	mov lr, r3
	.2byte 0xf800
	adds r0, r6, #0
	bl Inventory_RemoveFirstUnflagged
	adds r2, r0, #0
	cmp r2, #0
	beq .L_080fb06a
	adds r0, r6, #0
	mov r1, r8
	bl Inventory_AddItemFar
	movs r3, #1
	adds r2, r0, #0
	negs r3, r3
	cmp r2, r3
	beq .L_080fb05c
	movs r3, #4
	mov r1, r10
	orrs r1, r3
	mov r10, r1
	ldr r0, [r7, #40]
	adds r1, r6, #0
	b .L_080fb062
.L_080fb05c:
	ldr r0, [r7, #40]
	adds r1, r6, #0
	mov r2, r9
.L_080fb062:
	mov r3, r10
	bl Func_080f8170
	b .L_080fb076
.L_080fb06a:
	ldr r0, [r7, #40]
	adds r1, r6, #0
	mov r2, r9
	mov r3, r10
	bl Func_080f8170
.L_080fb076:
	movs r2, #166
	ldr r3, .L_080fb09c
	ldr r0, [sp, #0]
	mov r1, r11
	lsls r2, r2, #1
	mov lr, r3
	.2byte 0xf800
	mov r0, r11
	bl Sys_Free
.L_080fb08a:
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_080fb098:
	.4byte .L_080faee8
.L_080fb09c:
	.4byte IwramCopyWords
.L_080fb0a0:
	.4byte 0xfffffdff
