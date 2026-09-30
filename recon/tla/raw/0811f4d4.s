.syntax unified
	.thumb
	.global Func_0811f4d4
	.thumb_func
Func_0811f4d4:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r3, #192
	movs r1, #0
	lsls r3, r3, #18
	sub sp, #12
	mov r8, r1
	ldr r2, [r3, #36]
	cmp r0, #0
	beq .L_0811f530
	movs r3, #88
	ldrsh r3, [r2, r3]
	movs r7, #0
	cmp r3, #255
	beq .L_0811f580
	adds r5, r2, #0
	adds r5, #88
	mov r6, sp
.L_0811f4fc:
	movs r2, #0
	ldrsh r0, [r5, r2]
	cmp r0, #254
	beq .L_0811f51e
	bl Owner_GetState
	movs r1, #56
	ldrsh r3, [r0, r1]
	cmp r3, #0
	beq .L_0811f51e
	ldr r2, .L_0811f52c
	adds r3, r7, #0
	orrs r3, r2
	movs r2, #1
	strh r3, [r6]
	add r8, r2
	adds r6, #2
.L_0811f51e:
	adds r5, #2
	movs r1, #0
	ldrsh r3, [r5, r1]
	adds r7, #1
	cmp r3, #255
	bne .L_0811f4fc
	b .L_0811f580
.L_0811f52c:
	.4byte 0x00000100
.L_0811f530:
	adds r2, #2
	movs r3, #100
	ldrsh r3, [r2, r3]
	movs r7, #0
	mov r10, r2
	cmp r3, #255
	beq .L_0811f580
	mov r1, r8
	lsls r3, r1, #1
	add r1, sp, #12
	adds r3, r3, r1
	adds r5, r3, #0
	movs r6, #100
	subs r5, #12
.L_0811f54c:
	ldrsh r0, [r2, r6]
	cmp r0, #254
	beq .L_0811f56c
	bl Owner_GetState
	movs r1, #56
	ldrsh r3, [r0, r1]
	cmp r3, #0
	beq .L_0811f56c
	ldr r2, .L_0811f57c
	adds r3, r7, #0
	orrs r3, r2
	movs r2, #1
	strh r3, [r5]
	add r8, r2
	adds r5, #2
.L_0811f56c:
	adds r6, #2
	mov r2, r10
	ldrsh r3, [r2, r6]
	adds r7, #1
	cmp r3, #255
	bne .L_0811f54c
	b .L_0811f580
	.2byte 0x0000
.L_0811f57c:
	.4byte 0x00000180
.L_0811f580:
	mov r2, r8
	movs r0, #0
	cmp r2, #0
	beq .L_0811f598
	mov r5, sp
	bl BattleRandom16Far
	mov r3, r8
	muls r3, r0
	lsrs r3, r3, #16
	lsls r3, r3, #1
	ldrh r0, [r5, r3]
.L_0811f598:
	add sp, #12
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
