.syntax unified
	.thumb
	.global Func_080b1088
	.thumb_func
Func_080b1088:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r0, .L_080b115c
	movs r2, #206
	ldrh r3, [r0]
	lsls r2, r2, #7
	adds r2, #116
	sub sp, #4
	mov r8, r0
	cmp r3, r2
	bne .L_080b10aa
	b .L_080b11c0
.L_080b10aa:
	mov r1, r8
	ldr r3, .L_080b1160
	strh r2, [r1]
	movs r0, #144
	lsls r0, r0, #2
	movs r2, #2
	add r8, r2
	adds r2, r3, r0
	movs r0, #0
	ldrsh r1, [r2, r0]
	mov r11, r1
	movs r1, #128
	lsls r1, r1, #2
	adds r1, #66
	adds r3, r3, r1
	movs r0, #0
	ldrsh r2, [r3, r0]
	movs r1, #4
	str r2, [sp, #0]
	mov r10, r1
.L_080b10d2:
	mov r0, r10
	bl Owner_GetState
	adds r7, r0, #0
	adds r2, r7, #0
	adds r2, #216
	movs r6, #14
.L_080b10e0:
	ldrh r3, [r2]
	mov r0, r8
	movs r1, #2
	subs r6, #1
	adds r2, #2
	strh r3, [r0]
	add r8, r1
	cmp r6, #0
	bge .L_080b10e0
	movs r2, #128
	lsls r2, r2, #2
	adds r5, r7, #0
	mov r9, r2
	movs r6, #14
	adds r5, #216
.L_080b10fe:
	ldrh r0, [r5]
	bl Item_GetDirect
	ldrb r3, [r0, #2]
	movs r0, #192
	adds r3, #255
	lsls r3, r3, #24
	lsls r0, r0, #18
	cmp r3, r0
	bhi .L_080b1122
	ldrh r3, [r5]
	mov r1, r9
	ands r3, r1
	lsls r3, r3, #16
	lsrs r3, r3, #16
	cmp r3, #0
	bne .L_080b1122
	strh r3, [r5]
.L_080b1122:
	ldrh r0, [r5]
	bl Func_080b106c
	cmp r0, #0
	beq .L_080b1130
	ldr r3, .L_080b1158
	strh r3, [r5]
.L_080b1130:
	subs r6, #1
	adds r5, #2
	cmp r6, #0
	bge .L_080b10fe
	adds r0, r7, #0
	adds r0, #216
	movs r5, #0
	adds r4, r0, #0
	adds r1, r0, #0
	movs r6, #14
.L_080b1144:
	ldrh r2, [r4]
	adds r4, #2
	lsls r3, r2, #16
	cmp r3, #0
	beq .L_080b1164
	strh r2, [r1]
	adds r5, #1
	adds r1, #2
	b .L_080b1164
	.2byte 0x0000
.L_080b1158:
	.4byte 0x00000000
.L_080b115c:
	.4byte Data_020023c4
.L_080b1160:
	.4byte gPartyState
.L_080b1164:
	subs r6, #1
	cmp r6, #0
	bge .L_080b1144
	cmp r5, #14
	bgt .L_080b1188
	lsls r3, r5, #1
	ldr r2, .L_080b1184
	adds r0, r3, r0
	movs r3, #15
	subs r5, r3, r5
.L_080b1178:
	subs r5, #1
	strh r2, [r0]
	adds r0, #2
	cmp r5, #0
	bne .L_080b1178
	b .L_080b1188
.L_080b1184:
	.4byte 0x00000000
.L_080b1188:
	mov r0, r10
	bl Owner_RefreshDerivedData
	mov r0, r10
	bl Owner_RecalculateStats
	movs r2, #1
	add r10, r2
	mov r3, r10
	cmp r3, #7
	ble .L_080b10d2
	mov r3, sp
	movs r2, #2
	ldrh r3, [r3]
	mov r1, r8
	mov r0, r11
	add r8, r2
	strh r0, [r1]
	mov r0, r8
	strh r3, [r0]
	ldr r0, .L_080b11f4
	add r8, r2
	ldrh r3, [r0]
	mov r1, r8
	strh r3, [r1]
	mov r2, r8
	ldrh r3, [r0, #2]
	strh r3, [r2, #2]
.L_080b11c0:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_SetBit
	movs r0, #1
	bl Func_080ae16c
	movs r6, #229
	lsls r6, r6, #4
.L_080b11d4:
	adds r0, r6, #0
	bl GameFlag_ClearBit
	movs r3, #224
	lsls r3, r3, #4
	adds r6, #1
	adds r3, #89
	cmp r6, r3
	ble .L_080b11d4
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_080b11f4:
	.4byte Data_02000458
