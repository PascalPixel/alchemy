.syntax unified
	.thumb
	.global InventoryMenu_SortByListOrder
	.thumb_func
InventoryMenu_SortByListOrder:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #112
	str r0, [sp, #12]
	add r5, sp, #16
	movs r0, #0
	str r0, [sp, #8]
	mov r11, r0
	adds r0, r1, #0
	adds r1, r5, #0
	bl Menu_CopyListOrder
	add r1, sp, #48
	mov r9, r1
	movs r2, #0
	movs r6, #14
.L_080f8e32:
	ldr r0, [sp, #12]
	subs r6, #1
	ldrh r3, [r2, r0]
	strh r3, [r2, r1]
	adds r2, #2
	cmp r6, #0
	bge .L_080f8e32
	movs r1, #0
	mov r8, r1
	mov r2, r9
	movs r6, #14
.L_080f8e48:
	ldrh r3, [r2]
	adds r2, #2
	cmp r3, #0
	beq .L_080f8e54
	movs r3, #1
	add r8, r3
.L_080f8e54:
	subs r6, #1
	cmp r6, #0
	bge .L_080f8e48
	mov r0, r8
	cmp r0, #14
	bgt .L_080f8e7c
	add r3, sp, #80
	lsls r2, r0, #1
	ldr r1, .L_080f8e78
	adds r2, r2, r3
	movs r3, #15
	subs r6, r3, r0
.L_080f8e6c:
	subs r6, #1
	strh r1, [r2]
	adds r2, #2
	cmp r6, #0
	bne .L_080f8e6c
	b .L_080f8e7c
.L_080f8e78:
	.4byte 0x00000000
.L_080f8e7c:
	ldrb r3, [r5]
	cmp r3, #255
	beq .L_080f8f14
	mov r1, sp
	adds r1, #80
	str r1, [sp, #4]
	mov r10, r9
	adds r7, r5, #0
.L_080f8e8c:
	movs r6, #0
	movs r4, #0
	cmp r6, r8
	bge .L_080f8ee2
	mov r5, r9
.L_080f8e96:
	ldrh r3, [r5]
	cmp r3, #0
	beq .L_080f8eda
	adds r0, r3, #0
	str r4, [sp, #0]
	bl Item_Get
	ldrb r1, [r7]
	ldrb r3, [r0, #2]
	movs r2, #127
	ands r2, r1
	ldr r4, [sp, #0]
	cmp r2, r3
	bne .L_080f8eda
	movs r3, #128
	ands r3, r1
	cmp r3, #0
	beq .L_080f8ecc
	ldrh r2, [r5]
	ldr r3, .L_080f8ec8
	ands r3, r2
	cmp r3, #0
	beq .L_080f8eda
	b .L_080f8ece
	.2byte 0x0000
.L_080f8ec8:
	.4byte 0x00000200
.L_080f8ecc:
	ldrh r2, [r5]
.L_080f8ece:
	ldr r3, .L_080f8efc
	ands r3, r2
	cmp r4, r3
	bge .L_080f8eda
	str r6, [sp, #8]
	adds r4, r3, #0
.L_080f8eda:
	adds r6, #1
	adds r5, #2
	cmp r6, r8
	blt .L_080f8e96
.L_080f8ee2:
	cmp r4, #0
	beq .L_080f8f0c
	ldr r0, [sp, #8]
	mov r3, r11
	lsls r2, r0, #1
	mov r0, r10
	lsls r1, r3, #1
	ldrh r3, [r0, r2]
	ldr r0, [sp, #4]
	strh r3, [r0, r1]
	ldr r3, .L_080f8f00
	mov r1, r10
	b .L_080f8f04
.L_080f8efc:
	.4byte 0x000001ff
.L_080f8f00:
	.4byte 0x00000000
.L_080f8f04:
	strh r3, [r1, r2]
	movs r3, #1
	add r11, r3
	b .L_080f8e8c
.L_080f8f0c:
	adds r7, #1
	ldrb r3, [r7]
	cmp r3, #255
	bne .L_080f8e8c
.L_080f8f14:
	mov r0, r8
	cmp r0, #0
	ble .L_080f8f2e
	add r1, sp, #80
	movs r2, #0
	mov r6, r8
.L_080f8f20:
	ldrh r3, [r2, r1]
	ldr r0, [sp, #12]
	subs r6, #1
	strh r3, [r2, r0]
	adds r2, #2
	cmp r6, #0
	bne .L_080f8f20
.L_080f8f2e:
	movs r0, #1
	add sp, #112
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
