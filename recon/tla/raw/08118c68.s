.syntax unified
	.thumb
	.global Func_08118c68
	.thumb_func
Func_08118c68:
	push {r5, r6, r7, lr}
	sub sp, #28
	mov r7, sp
	movs r3, #192
	lsls r3, r3, #18
	adds r0, r7, #0
	ldr r5, [r3, #36]
	bl Func_0811a038
	adds r6, r0, #0
	cmp r6, #0
	ble .L_08118c98
	adds r2, r5, #0
	adds r0, r7, #0
	adds r2, #88
	movs r4, #0
	adds r1, r6, #0
.L_08118c8a:
	ldrh r3, [r4, r0]
	subs r1, #1
	strh r3, [r2]
	adds r4, #2
	adds r2, #2
	cmp r1, #0
	bne .L_08118c8a
.L_08118c98:
	ldr r2, .L_08118ccc
	lsls r3, r6, #1
	adds r3, #88
	strh r2, [r5, r3]
	adds r0, r7, #0
	bl BattleParty_ListPresentEnemies
	adds r3, r5, #0
	adds r3, #66
	ldrb r3, [r3]
	adds r6, r0, #0
	cmp r3, #0
	blt .L_08118ce0
	cmp r3, #1
	bgt .L_08118ce0
	movs r1, #0
	cmp r1, r6
	bge .L_08118d12
	adds r3, r5, #2
	adds r2, r5, #0
	mov r12, r3
	adds r0, r7, #0
	adds r2, #102
	movs r4, #0
	b .L_08118cd0
	.2byte 0x0000
.L_08118ccc:
	.4byte 0x000000ff
.L_08118cd0:
	ldrh r3, [r4, r0]
	adds r1, #1
	strh r3, [r2]
	adds r4, #2
	adds r2, #2
	cmp r1, r6
	blt .L_08118cd0
	b .L_08118d16
.L_08118ce0:
	cmp r6, #0
	ble .L_08118d12
	lsrs r3, r6, #31
	adds r3, r6, r3
	ldr r4, .L_08118d28
	adds r5, #2
	asrs r3, r3, #1
	mov r12, r5
	mov lr, r3
	adds r0, r7, #0
	adds r1, r6, #0
.L_08118cf6:
	ldrb r3, [r4]
	ldrh r2, [r0]
	lsls r3, r3, #24
	asrs r3, r3, #24
	add r3, lr
	lsls r3, r3, #1
	adds r3, #100
	subs r1, #1
	adds r4, #1
	adds r0, #2
	strh r2, [r5, r3]
	cmp r1, #0
	bne .L_08118cf6
	b .L_08118d16
.L_08118d12:
	adds r5, #2
	mov r12, r5
.L_08118d16:
	ldr r2, .L_08118d24
	lsls r3, r6, #1
	adds r3, #100
	mov r1, r12
	strh r2, [r1, r3]
	add sp, #28
	pop {r5, r6, r7, pc}
.L_08118d24:
	.4byte 0x000000ff
.L_08118d28:
	.4byte Data_081287c8
