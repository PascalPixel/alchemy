.syntax unified
	.thumb
	.global Func_080e1024
	.thumb_func
Func_080e1024:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #224
	ldr r3, [r3]
	sub sp, #12
	ldr r1, [r3, #16]
	movs r2, #4
	str r1, [sp, #0]
	mov r10, r3
	add r2, sp
	movs r3, #63
	adds r6, r0, #0
	movs r7, #0
	mov r9, r2
	mov r11, r3
.L_080e1050:
	ldr r2, [r6, #12]
	movs r3, #128
	lsls r3, r3, #15
	movs r0, #16
	ldr r1, [r6, #8]
	adds r2, r2, r3
	adds r0, #255
	ldr r3, [r6, #16]
	bl Func_080200c0
	lsls r3, r7, #2
	mov r1, r9
	str r0, [r3, r1]
	cmp r0, #0
	beq .L_080e1108
	ldr r3, [r6, #20]
	movs r2, #0
	str r3, [r0, #20]
	adds r3, r0, #0
	adds r3, #85
	ldr r5, [r0, #80]
	strb r2, [r3]
	adds r3, #15
	strh r2, [r3]
	ldr r1, .L_080e1098
	movs r3, #204
	lsls r3, r3, #7
	adds r3, #102
	mov r8, r1
	str r6, [r0, #104]
	str r3, [r0, #28]
	str r3, [r0, #24]
	cmp r5, #0
	beq .L_080e1108
	b .L_080e109c
	.2byte 0x0000
.L_080e1098:
	.4byte 0x00000000
.L_080e109c:
	movs r1, #0
	adds r0, r5, #0
	bl Animation_ApplyChildArgumentFar
	mov r2, r8
	strb r2, [r5, #26]
	ldrb r0, [r5, #16]
	bl Resource_ResetEntry
	movs r3, #226
	lsls r3, r3, #3
	add r3, r10
	ldrh r3, [r3]
	movs r2, #1
	strb r3, [r5, #16]
	ldrb r3, [r5, #17]
	orrs r3, r2
	strb r3, [r5, #17]
	ldrb r3, [r5, #16]
	ldr r2, .L_080e1104
	lsls r3, r3, #2
	adds r3, r3, r2
	ldrh r1, [r3, #2]
	ldr r2, .L_080e1100
	ldrh r3, [r5, #8]
	lsls r1, r1, #17
	lsrs r1, r1, #22
	ands r3, r2
	orrs r3, r1
	strh r3, [r5, #8]
	movs r1, #33
	ldrb r3, [r5, #5]
	negs r1, r1
	adds r2, r1, #0
	ands r3, r2
	mov r2, r11
	ands r3, r2
	movs r2, #64
	orrs r3, r2
	ldrb r2, [r5, #7]
	strb r3, [r5, #5]
	mov r3, r11
	ands r3, r2
	movs r2, #128
	orrs r3, r2
	strb r3, [r5, #7]
	ldr r3, [r5, #40]
	mov r1, r8
	strb r1, [r3, #22]
	b .L_080e1108
.L_080e1100:
	.4byte 0xfffffc00
.L_080e1104:
	.4byte ResourceTableEntries
.L_080e1108:
	adds r7, #1
	cmp r7, #1
	ble .L_080e1050
	ldr r2, [sp, #4]
	ldr r3, .L_080e114c
	ldr r0, [r2, #80]
	str r3, [r2, #108]
	ldrb r1, [r0, #9]
	movs r2, #13
	negs r2, r2
	adds r3, r2, #0
	ands r3, r1
	strb r3, [r0, #9]
	mov r3, r9
	ldr r1, [r3, #4]
	ldr r3, .L_080e1150
	ldr r0, [r1, #80]
	str r3, [r1, #108]
	ldr r1, [sp, #0]
	add sp, #12
	ldr r3, [r1, #80]
	ldrb r1, [r3, #9]
	movs r3, #12
	ands r3, r1
	ldrb r1, [r0, #9]
	ands r2, r1
	orrs r2, r3
	strb r2, [r0, #9]
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_080e114c:
	.4byte BattleFx_UpdateDescendingParticleNegativeArc
.L_080e1150:
	.4byte BattleFx_UpdateDescendingParticlePositiveArc
