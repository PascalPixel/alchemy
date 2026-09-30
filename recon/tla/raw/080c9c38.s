.syntax unified
	.thumb
	.global Func_080c9c38
	.thumb_func
Func_080c9c38:
	push {r5, r6, r7, lr}
	ldr r6, .L_080c9dc0
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r6, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	ldr r2, .L_080c9dc4
	lsls r3, r3, #3
	adds r3, r3, r2
	movs r7, #2
	ldrsb r7, [r3, r7]
	cmp r1, #0
	beq .L_080c9c56
	b .L_080c9d78
.L_080c9c56:
	movs r5, #128
	lsls r5, r5, #2
.L_080c9c5a:
	adds r0, r5, #0
	bl GameFlag_ClearBit
	movs r3, #128
	lsls r3, r3, #2
	adds r5, #1
	adds r3, #255
	cmp r5, r3
	ble .L_080c9c5a
	ldr r3, .L_080c9dc0
	movs r1, #246
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r7, r3
	beq .L_080c9cea
	movs r5, #192
	lsls r5, r5, #2
.L_080c9c80:
	adds r0, r5, #0
	bl GameFlag_ClearBit
	movs r3, #192
	lsls r3, r3, #2
	adds r5, #1
	adds r3, #255
	cmp r5, r3
	ble .L_080c9c80
	movs r0, #48
	adds r0, #255
	bl GameFlag_SetBit
	ldr r5, .L_080c9dc0
	movs r1, #150
	lsls r1, r1, #2
	movs r2, #0
	adds r3, r5, r1
	subs r1, #6
	str r2, [r3]
	movs r0, #136
	adds r3, r5, r1
	strh r2, [r3]
	lsls r0, r0, #1
	bl GameFlag_ClearBit
	movs r0, #18
	adds r0, #255
	bl GameFlag_ClearBit
	movs r0, #137
	lsls r0, r0, #1
	bl GameFlag_ClearBit
	movs r0, #20
	adds r0, #255
	bl GameFlag_ClearBit
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r5, r2
	ldrh r2, [r3]
	movs r1, #152
	lsls r1, r1, #2
	adds r3, r5, r1
	strh r2, [r3]
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r5, r2
	ldrh r2, [r3]
	adds r1, #2
	adds r3, r5, r1
	strh r2, [r3]
.L_080c9cea:
	movs r5, #128
.L_080c9cec:
	adds r0, r5, #0
	adds r5, #1
	bl GameFlag_ClearBit
	cmp r5, #223
	ble .L_080c9cec
	movs r0, #182
	lsls r0, r0, #1
	bl GameFlag_ClearBit
	movs r0, #162
	lsls r0, r0, #1
	bl GameFlag_ClearBit
	movs r0, #98
	adds r0, #255
	bl GameFlag_ClearBit
	movs r0, #36
	adds r0, #255
	bl GameFlag_ClearBit
	movs r0, #142
	lsls r0, r0, #1
	bl GameFlag_ClearBit
	movs r0, #163
	lsls r0, r0, #1
	bl GameFlag_ClearBit
	movs r0, #190
	lsls r0, r0, #1
	bl GameFlag_ClearBit
	movs r0, #126
	adds r0, #255
	bl GameFlag_ClearBit
	movs r0, #191
	lsls r0, r0, #1
	bl GameFlag_ClearBit
	ldr r3, .L_080c9dc0
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #118
	adds r3, r3, r2
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	beq .L_080c9d5c
	movs r0, #187
	lsls r0, r0, #1
	bl GameFlag_SetBit
	b .L_080c9d64
.L_080c9d5c:
	movs r0, #187
	lsls r0, r0, #1
	bl GameFlag_ClearBit
.L_080c9d64:
	ldr r1, .L_080c9dc0
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #62
	adds r2, r1, r3
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	strh r3, [r2]
	adds r6, r1, #0
.L_080c9d78:
	movs r1, #246
	lsls r1, r1, #1
	adds r3, r6, r1
	strh r7, [r3]
	movs r2, #240
	lsls r2, r2, #1
	adds r5, r6, r2
	subs r1, #10
	movs r3, #0
	ldrsh r0, [r5, r3]
	adds r3, r6, r1
	movs r2, #0
	ldrsh r1, [r3, r2]
	bl Func_080c9de0
	movs r1, #0
	ldrsh r3, [r5, r1]
	ldr r2, .L_080c9dc4
	lsls r3, r3, #3
	adds r3, r3, r2
	movs r1, #128
	movs r2, #3
	ldrsb r2, [r3, r2]
	lsls r1, r1, #2
	adds r1, #94
	adds r3, r6, r1
	strh r2, [r3]
	cmp r2, #2
	bne .L_080c9dba
	movs r0, #36
	adds r0, #255
	bl GameFlag_SetBit
.L_080c9dba:
	bl Func_080ad2b8
	pop {r5, r6, r7, pc}
.L_080c9dc0:
	.4byte gPartyState
.L_080c9dc4:
	.4byte Data_080f17a8
