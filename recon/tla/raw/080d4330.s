.syntax unified
	.thumb
	.global Func_080d4330
	.thumb_func
Func_080d4330:
	push {r5, lr}
	movs r3, #192
	movs r1, #128
	lsls r3, r3, #18
	lsls r1, r1, #24
	ldr r5, [r3, #60]
	cmp r0, r1
	bne .L_080d4354
	movs r2, #152
	lsls r2, r2, #5
	movs r1, #152
	adds r2, #132
	lsls r1, r1, #5
	adds r3, r5, r2
	adds r1, #134
	movs r2, #0
	strh r2, [r3]
	b .L_080d4376
.L_080d4354:
	bl ObjectTable_ReadActiveValue
	bl Func_080d1eac
	ldr r3, .L_080d437c
	movs r1, #139
	lsls r1, r1, #2
	adds r3, r3, r1
	ldrb r3, [r3]
	ldr r2, .L_080d4380
	movs r1, #152
	lsls r1, r1, #5
	ldrb r2, [r2, r3]
	adds r1, #132
	adds r3, r5, r1
	adds r1, #2
	strh r0, [r3]
.L_080d4376:
	adds r3, r5, r1
	strh r2, [r3]
	pop {r5, pc}
.L_080d437c:
	.4byte gPartyState
.L_080d4380:
	.4byte Data_080f330c
