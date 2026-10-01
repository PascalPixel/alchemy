.syntax unified
	.thumb
	.global Encounter_SelectEnemyGroup
	.thumb_func
Encounter_SelectEnemyGroup:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #8
	str r0, [sp, #4]
	str r1, [sp, #0]
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r0, #96
	adds r0, #255
	mov r11, r3
	bl GameFlag_Test
	cmp r0, #0
	bne .L_080ca0ea
	movs r0, #176
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	bne .L_080ca072
	movs r0, #98
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_080ca072
	ldr r0, [sp, #4]
	cmp r0, #0
	beq .L_080ca072
	ldr r5, .L_080ca154
	movs r2, #183
	lsls r2, r2, #1
	adds r2, #255
	adds r3, r5, r2
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	movs r0, #0
	cmp r3, #0
	beq .L_080ca036
	b .L_080ca146
.L_080ca036:
	ldr r0, [sp, #4]
	ldr r2, .L_080ca158
	lsls r3, r0, #3
	subs r3, r3, r0
	lsls r3, r3, #2
	adds r3, r3, r2
	ldrh r7, [r3]
	mov r9, r3
	movs r0, #0
	cmp r7, #0
	beq .L_080ca146
	bl Func_080ad290
	mov r2, r9
	ldrh r3, [r2, #2]
	subs r0, r0, r3
	cmp r0, #0
	bge .L_080ca05c
	movs r0, #0
.L_080ca05c:
	cmp r0, #5
	ble .L_080ca062
	movs r0, #5
.L_080ca062:
	cmp r0, #0
	ble .L_080ca076
	movs r2, #153
	lsls r2, r2, #2
	adds r3, r5, r2
	ldr r3, [r3]
	cmp r3, #0
	beq .L_080ca076
.L_080ca072:
	movs r0, #0
	b .L_080ca146
.L_080ca076:
	lsls r3, r0, #2
	adds r3, r3, r0
	adds r7, r7, r3
	movs r3, #202
	lsls r3, r3, #1
	add r3, r11
	ldr r5, [r3]
	mov r10, r3
	cmp r5, #0
	bne .L_080ca0b2
	bl Random16
	adds r5, r0, #0
	bl Random16
	mov r8, r0
	bl Random16
	adds r6, r0, #0
	bl Random16
	mov r2, r8
	subs r5, r5, r2
	adds r5, r5, r6
	subs r5, r5, r0
	lsrs r3, r5, #31
	adds r5, r5, r3
	asrs r5, r5, #1
	mov r3, r10
	str r5, [r3]
.L_080ca0b2:
	lsls r3, r7, #4
	subs r3, #16
	muls r3, r5
	lsls r0, r7, #20
	movs r1, #128
	adds r0, r0, r3
	lsls r1, r1, #13
	ldr r3, .L_080ca15c
	mov lr, r3
	.2byte 0xf800
	ldr r3, .L_080ca160
	ldr r1, [sp, #0]
	mov lr, r3
	.2byte 0xf800
	ldr r3, .L_080ca154
	movs r2, #150
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r2, [r3]
	adds r2, r2, r0
	str r2, [r3]
	movs r3, #204
	lsls r3, r3, #1
	add r3, r11
	ldr r3, [r3]
	movs r0, #0
	cmp r2, r3
	blt .L_080ca146
.L_080ca0ea:
	movs r2, #202
	lsls r2, r2, #1
	add r2, r11
	movs r3, #0
	str r3, [r2]
	mov r2, r9
	movs r5, #0
	adds r2, #20
	movs r1, #7
.L_080ca0fc:
	ldrb r3, [r2]
	subs r1, #1
	adds r2, #1
	adds r5, r5, r3
	cmp r1, #0
	bge .L_080ca0fc
	movs r0, #0
	cmp r5, #0
	beq .L_080ca146
	bl Random16
	adds r3, r5, #0
	muls r3, r0
	mov r0, r9
	lsrs r2, r3, #16
	ldrb r3, [r0, #20]
	movs r1, #0
	subs r2, r2, r3
	cmp r2, #0
	blt .L_080ca136
	adds r0, #20
.L_080ca126:
	adds r1, #1
	cmp r1, #7
	bgt .L_080ca136
	adds r0, #1
	ldrb r3, [r0]
	subs r2, r2, r3
	cmp r2, #0
	bge .L_080ca126
.L_080ca136:
	lsls r3, r1, #1
	adds r3, #4
	mov r2, r9
	ldrh r5, [r2, r3]
	ldr r0, [sp, #4]
	bl Func_080ca5d8
	adds r0, r5, #0
.L_080ca146:
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_080ca154:
	.4byte gPartyState
.L_080ca158:
	.4byte Encounter_EnemyGroupTable
.L_080ca15c:
	.4byte IwramRatioMulQ14
.L_080ca160:
	.4byte IwramMulQ16
