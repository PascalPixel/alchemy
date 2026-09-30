.syntax unified
	.thumb
	.global Func_080ca2fc
	.thumb_func
Func_080ca2fc:
	push {r5, r6, r7, lr}
	ldr r0, .L_080ca360
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r0, r1
	movs r2, #0
	ldrsh r6, [r3, r2]
	movs r4, #241
	ldr r2, .L_080ca364
	lsls r4, r4, #1
	adds r3, r0, r4
	movs r7, #0
	ldrsh r5, [r3, r7]
	movs r4, #0
	ldrsh r3, [r2, r4]
	movs r4, #1
	negs r4, r4
	ldrh r1, [r2]
	cmp r3, r4
	beq .L_080ca35c
	movs r7, #242
	movs r3, #243
	lsls r7, r7, #1
	lsls r3, r3, #1
	mov r12, r4
	adds r4, r0, r7
	adds r0, r0, r3
.L_080ca332:
	lsls r3, r1, #16
	asrs r3, r3, #16
	cmp r3, r6
	bne .L_080ca350
	movs r7, #2
	ldrsh r3, [r2, r7]
	cmp r3, r12
	beq .L_080ca346
	cmp r3, r5
	bne .L_080ca350
.L_080ca346:
	ldrh r3, [r2, #4]
	strh r3, [r4]
	ldrh r3, [r2, #6]
	strh r3, [r0]
	b .L_080ca35c
.L_080ca350:
	adds r2, #16
	movs r7, #0
	ldrsh r3, [r2, r7]
	ldrh r1, [r2]
	cmp r3, r12
	bne .L_080ca332
.L_080ca35c:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080ca360:
	.4byte gPartyState
.L_080ca364:
	.4byte Data_080ef824
