.syntax unified
	.thumb
	.global BattleMotion_SetRecordChildValues
	.thumb_func
BattleMotion_SetRecordChildValues:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r3, #255
	mov r8, r0
	adds r7, r1, #0
	movs r6, #0
	mov r10, r3
	b .L_0811f072
.L_0811f044:
	ldr r2, [r0, #40]
	adds r1, r0, #0
	ldrb r3, [r2, #22]
	ldrb r0, [r0, #27]
	mov r4, r10
	orrs r3, r4
	adds r1, #44
	strb r7, [r2, #5]
	strb r3, [r2, #22]
	cmp r0, #1
	ble .L_0811f070
	movs r5, #0
	movs r4, #255
	subs r0, #1
.L_0811f060:
	ldmia r1!, {r2}
	subs r0, #1
	ldrb r3, [r2, #22]
	strb r5, [r2, #5]
	orrs r3, r4
	strb r3, [r2, #22]
	cmp r0, #0
	bne .L_0811f060
.L_0811f070:
	adds r6, #1
.L_0811f072:
	mov r0, r8
	adds r1, r6, #0
	bl GetMotionRecord
	cmp r0, #0
	bne .L_0811f044
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
