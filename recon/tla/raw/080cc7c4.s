.syntax unified
	.thumb
	.global Func_080cc7c4
	.thumb_func
Func_080cc7c4:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	sub sp, #8
	ldr r3, [r3, #32]
	ldr r6, .L_080cc978
	movs r2, #4
	ldr r5, .L_080cc97c
	add r2, sp
	movs r0, #241
	lsls r0, r0, #1
	mov r10, r2
	mov r11, r3
	adds r3, r6, r0
	mov r0, r10
	str r5, [sp, #4]
	movs r1, #0
	ldrsh r7, [r3, r1]
	bl Func_080cc994
	lsls r0, r0, #16
	lsrs r0, r0, #16
	adds r0, r0, r5
	str r0, [sp, #4]
.L_080cc800:
	ldr r3, [sp, #4]
	movs r5, #1
	movs r2, #0
	ldrsb r2, [r3, r2]
	negs r5, r5
	cmp r2, r5
	bne .L_080cc820
	ldr r0, .L_080cc980
	movs r3, #128
	ldr r1, .L_080cc984
	lsls r3, r3, #7
	str r3, [r0]
	movs r3, #0
	strh r3, [r1]
	movs r0, #0
	b .L_080cc968
.L_080cc820:
	cmp r2, r7
	beq .L_080cc826
	b .L_080cc962
.L_080cc826:
	adds r3, #1
	mov r0, r10
	str r3, [sp, #4]
	bl Func_080cc9ac
	lsls r0, r0, #16
	asrs r0, r0, #16
	mov r8, r0
	mov r0, r10
	bl Func_080cc9ac
	lsls r0, r0, #16
	asrs r0, r0, #16
	mov r2, r8
	lsls r6, r2, #16
	lsls r7, r0, #16
	mov r9, r0
	adds r1, r6, #0
	movs r0, #0
	adds r2, r7, #0
	bl Map_GetTerrainHeightFar
	asrs r0, r0, #16
	str r0, [sp, #0]
	ldr r3, [sp, #4]
	movs r0, #10
	adds r0, #255
	ldrb r5, [r3]
	bl GameFlag_Test
	cmp r0, #0
	bne .L_080cc898
	ldr r2, .L_080cc978
	movs r1, #254
	lsls r1, r1, #1
	adds r3, r2, r1
	adds r1, #8
	str r6, [r3]
	adds r3, r2, r1
	str r7, [r3]
	ldr r1, .L_080cc980
	movs r3, #15
	ands r3, r5
	lsls r3, r3, #12
	str r3, [r1]
	ldr r3, .L_080cc984
	strh r0, [r3]
	movs r3, #16
	ands r3, r5
	cmp r3, #0
	beq .L_080cc898
	movs r0, #128
	lsls r0, r0, #2
	movs r3, #128
	adds r2, r2, r0
	lsls r3, r3, #12
	str r3, [r2]
.L_080cc898:
	movs r3, #32
	ands r3, r5
	movs r0, #1
	cmp r3, #0
	bne .L_080cc968
	ldr r3, .L_080cc988
	mov r0, r10
	str r3, [sp, #4]
	bl Func_080cc994
	ldr r1, .L_080cc97c
	lsls r0, r0, #16
	lsrs r0, r0, #16
	adds r0, r0, r1
	str r0, [sp, #4]
	movs r2, #1
	movs r3, #0
	ldrsb r3, [r0, r3]
	negs r2, r2
	cmp r3, r2
	beq .L_080cc95e
	movs r3, #244
	add r3, r11
	mov lr, r3
	mov r10, r2
.L_080cc8ca:
	adds r3, r0, #1
	movs r5, #0
	ldrsb r5, [r0, r5]
	str r3, [sp, #4]
	mov r12, r5
	movs r7, #0
	ldrsb r7, [r3, r7]
	adds r3, #1
	str r3, [sp, #4]
	lsls r2, r5, #4
	movs r6, #0
	ldrsb r6, [r3, r6]
	adds r3, #1
	str r3, [sp, #4]
	lsls r4, r7, #4
	movs r5, #0
	ldrsb r5, [r3, r5]
	adds r3, #1
	lsls r0, r6, #4
	lsls r1, r5, #4
	str r3, [sp, #4]
	cmp r8, r2
	blt .L_080cc954
	cmp r8, r0
	bgt .L_080cc954
	ldr r2, [sp, #0]
	mov r0, r9
	subs r3, r0, r2
	cmp r3, r4
	blt .L_080cc954
	cmp r3, r1
	bgt .L_080cc954
	mov r0, r12
	mov r1, r11
	lsls r3, r0, #20
	adds r1, #236
	mov r0, r11
	str r3, [r1]
	adds r0, #240
	lsls r3, r7, #20
	str r3, [r0]
	mov r2, lr
	lsls r3, r6, #20
	mov r4, r11
	str r3, [r2]
	adds r4, #248
	lsls r3, r5, #20
	str r3, [r4]
	movs r5, #240
	ldr r3, [r1]
	ldr r2, [r2]
	lsls r5, r5, #16
	adds r3, r3, r5
	cmp r3, r2
	ble .L_080cc93e
	ldr r5, .L_080cc98c
	adds r3, r2, r5
	str r3, [r1]
.L_080cc93e:
	ldr r3, [r0]
	movs r1, #160
	ldr r2, [r4]
	lsls r1, r1, #16
	adds r3, r3, r1
	cmp r3, r2
	ble .L_080cc95e
	ldr r5, .L_080cc990
	adds r3, r2, r5
	str r3, [r0]
	b .L_080cc95e
.L_080cc954:
	ldr r0, [sp, #4]
	movs r3, #0
	ldrsb r3, [r0, r3]
	cmp r3, r10
	bne .L_080cc8ca
.L_080cc95e:
	movs r0, #1
	b .L_080cc968
.L_080cc962:
	adds r3, #6
	str r3, [sp, #4]
	b .L_080cc800
.L_080cc968:
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080cc978:
	.4byte gPartyState
.L_080cc97c:
	.4byte Data_0202e000
.L_080cc980:
	.4byte Data_02000448
.L_080cc984:
	.4byte Data_0200044c
.L_080cc988:
	.4byte Data_0202e004
.L_080cc98c:
	.4byte 0xff100000
.L_080cc990:
	.4byte 0xff600000
