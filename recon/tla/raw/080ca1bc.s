.syntax unified
	.thumb
	.global Func_080ca1bc
	.thumb_func
Func_080ca1bc:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r4, .L_080ca278
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r4, r2
	movs r1, #79
	mov r10, r1
	movs r2, #0
	ldrsh r1, [r3, r2]
	ldr r5, .L_080ca27c
	mov r11, r1
	movs r1, #241
	lsls r1, r1, #1
	adds r3, r4, r1
	adds r1, #10
	movs r2, #0
	ldrsh r7, [r3, r2]
	ldrh r0, [r5]
	adds r3, r4, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	adds r1, r0, #0
	mov r9, r2
	lsls r3, r1, #16
	movs r2, #1
	asrs r3, r3, #16
	negs r2, r2
	cmp r3, r2
	beq .L_080ca262
	movs r6, #254
	lsls r6, r6, #7
	adds r6, #255
	mov r8, r2
.L_080ca20a:
	ldrb r2, [r5, #3]
	movs r3, #128
	ands r3, r2
	cmp r3, #0
	beq .L_080ca21e
	lsls r3, r1, #16
	asrs r3, r3, #16
	cmp r3, r11
	bne .L_080ca252
	b .L_080ca226
.L_080ca21e:
	lsls r3, r0, #16
	asrs r3, r3, #16
	cmp r3, r9
	bne .L_080ca252
.L_080ca226:
	ldrh r2, [r5, #2]
	adds r3, r6, #0
	ands r3, r2
	cmp r3, r6
	beq .L_080ca238
	lsls r3, r2, #17
	asrs r3, r3, #17
	cmp r3, r7
	bne .L_080ca252
.L_080ca238:
	movs r2, #4
	ldrsh r0, [r5, r2]
	cmp r0, r8
	beq .L_080ca248
	bl GameFlag_Test
	cmp r0, #0
	beq .L_080ca252
.L_080ca248:
	movs r1, #6
	ldrsh r3, [r5, r1]
	ldr r4, .L_080ca278
	mov r10, r3
	b .L_080ca262
.L_080ca252:
	adds r5, #8
	ldrh r1, [r5]
	lsls r3, r1, #16
	asrs r3, r3, #16
	adds r0, r1, #0
	cmp r3, r8
	bne .L_080ca20a
	ldr r4, .L_080ca278
.L_080ca262:
	movs r2, #132
	lsls r2, r2, #2
	adds r3, r4, r2
	mov r1, r10
	strh r1, [r3]
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_080ca278:
	.4byte gPartyState
.L_080ca27c:
	.4byte Data_080ef094
