.syntax unified
	.thumb
	.global Func_0811cfd0
	.thumb_func
Func_0811cfd0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #48
	movs r1, #0
	str r0, [sp, #16]
	str r1, [sp, #12]
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #36]
	movs r0, #0
	adds r3, #69
	str r3, [sp, #8]
	ldrb r3, [r3]
	cmp r3, #1
	bne .L_0811cffa
	b .L_0811d152
.L_0811cffa:
	mov r2, sp
	adds r2, #20
	movs r0, #2
	adds r1, r2, #0
	str r2, [sp, #4]
	bl BattleParty_ListLivingUnits
	movs r3, #31
	mov r10, r0
	ldr r6, [sp, #4]
	mov r8, r3
	cmp r0, #0
	bne .L_0811d018
	movs r0, #0
	b .L_0811d152
.L_0811d018:
	bl Random16
	mov r5, r10
	muls r5, r0
	bl Random16
	mov r2, r10
	muls r2, r0
	lsrs r5, r5, #16
	lsrs r2, r2, #16
	lsls r2, r2, #1
	lsls r5, r5, #1
	ldrh r1, [r6, r5]
	ldrh r3, [r6, r2]
	strh r3, [r6, r5]
	strh r1, [r6, r2]
	movs r1, #1
	negs r1, r1
	add r8, r1
	mov r2, r8
	cmp r2, #0
	bge .L_0811d018
	ldr r1, [sp, #8]
	ldrb r3, [r1]
	cmp r3, #2
	bne .L_0811d064
	bl Random16
	lsls r3, r0, #2
	adds r3, r3, r0
	lsrs r3, r3, #16
	adds r3, #1
	cmp r3, #1
	bgt .L_0811d05e
	movs r3, #2
.L_0811d05e:
	cmp r3, r10
	bge .L_0811d064
	mov r10, r3
.L_0811d064:
	movs r2, #0
	mov r8, r2
	cmp r8, r10
	bge .L_0811d150
.L_0811d06c:
	ldr r2, [sp, #4]
	mov r1, r8
	lsls r3, r1, #1
	ldrh r3, [r2, r3]
	movs r6, #0
	mov r9, r3
	mov r0, r9
	bl Owner_GetState
	adds r7, r0, #0
	adds r2, r7, #0
	adds r2, #67
	str r2, [sp, #0]
	ldrb r3, [r2]
	cmp r6, r3
	bge .L_0811d148
	ldr r1, [sp, #12]
	movs r3, #64
	adds r3, r3, r7
	mov r11, r3
	lsls r3, r1, #4
	ldr r1, [sp, #16]
	adds r5, r3, r1
.L_0811d09a:
	mov r1, r11
	ldrh r0, [r1]
	ldrb r1, [r2]
	mov r3, r9
	strh r3, [r5]
	strh r0, [r5, #4]
	strh r6, [r5, #14]
	cmp r1, #4
	bne .L_0811d0d0
	cmp r6, #0
	beq .L_0811d0fe
	cmp r6, #1
	bne .L_0811d0be
	lsls r3, r0, #16
	asrs r3, r3, #16
	lsls r0, r3, #2
	adds r0, r0, r3
	b .L_0811d0c6
.L_0811d0be:
	cmp r6, #2
	bne .L_0811d0f2
	lsls r0, r0, #16
	asrs r0, r0, #14
.L_0811d0c6:
	movs r1, #6
	bl __divsi3
	strh r0, [r5, #4]
	b .L_0811d0fe
.L_0811d0d0:
	cmp r1, #3
	bne .L_0811d0ee
	cmp r6, #0
	beq .L_0811d0fe
	cmp r6, #1
	bne .L_0811d0f2
	lsls r3, r0, #16
	asrs r3, r3, #16
	lsls r2, r3, #1
	adds r0, r2, r3
	cmp r0, #0
	bge .L_0811d0ea
	adds r0, #3
.L_0811d0ea:
	asrs r3, r0, #2
	b .L_0811d0fc
.L_0811d0ee:
	cmp r6, #0
	beq .L_0811d0fe
.L_0811d0f2:
	lsls r2, r0, #16
	asrs r3, r2, #16
	lsrs r2, r2, #31
	adds r3, r3, r2
	asrs r3, r3, #1
.L_0811d0fc:
	strh r3, [r5, #4]
.L_0811d0fe:
	movs r2, #158
	lsls r2, r2, #1
	adds r3, r7, r2
	ldrb r3, [r3]
	cmp r3, #0
	bne .L_0811d116
	movs r1, #60
	adds r1, #255
	adds r3, r7, r1
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_0811d126
.L_0811d116:
	movs r3, #0
	strh r3, [r5, #8]
	movs r3, #128
	movs r2, #8
	lsls r3, r3, #1
	strh r2, [r5, #6]
	strh r3, [r5, #10]
	b .L_0811d12e
.L_0811d126:
	adds r0, r5, #0
	movs r1, #0
	bl Func_08122514
.L_0811d12e:
	ldr r2, [sp, #12]
	ldr r1, [sp, #8]
	adds r2, #1
	str r2, [sp, #12]
	adds r5, #16
	ldrb r3, [r1]
	cmp r3, #2
	beq .L_0811d148
	ldr r2, [sp, #0]
	adds r6, #1
	ldrb r3, [r2]
	cmp r6, r3
	blt .L_0811d09a
.L_0811d148:
	movs r2, #1
	add r8, r2
	cmp r8, r10
	blt .L_0811d06c
.L_0811d150:
	ldr r0, [sp, #12]
.L_0811d152:
	add sp, #48
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
