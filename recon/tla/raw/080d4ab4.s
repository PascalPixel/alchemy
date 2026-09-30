.syntax unified
	.thumb
	.global Func_080d4ab4
	.thumb_func
Func_080d4ab4:
	push {r5, lr}
	ldr r3, .L_080d4b08
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	adds r5, r0, #0
	ldr r0, [r3]
	bl ObjectTable_Get
	ldr r3, [r0, #80]
	ldrb r3, [r3, #9]
	lsls r3, r3, #28
	lsrs r4, r3, #30
	adds r3, r5, #0
	adds r3, #84
	ldrb r2, [r3]
	movs r3, #15
	ands r3, r2
	cmp r3, #1
	bne .L_080d4b04
	ldr r1, [r5, #80]
	movs r2, #13
	ldrb r0, [r1, #9]
	negs r2, r2
	adds r3, r2, #0
	lsls r4, r4, #2
	ands r3, r0
	orrs r3, r4
	strb r3, [r1, #9]
	adds r1, #37
	ldrb r3, [r1]
	ands r2, r3
	orrs r2, r4
	strb r2, [r1]
	adds r1, r5, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #254
	ands r3, r2
	strb r3, [r1]
.L_080d4b04:
	movs r0, #0
	pop {r5, pc}
.L_080d4b08:
	.4byte gPartyState
