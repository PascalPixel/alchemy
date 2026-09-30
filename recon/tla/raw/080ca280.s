.syntax unified
	.thumb
	.global Func_080ca280
	.thumb_func
Func_080ca280:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r3, #0
	adds r6, r1, #0
	ldr r5, .L_080ca2f8
	mov r8, r3
	adds r7, r0, #0
	bl BattleFx_GetResourceGroup
	ldrh r4, [r5]
	movs r2, #1
	adds r1, r4, #0
	lsls r3, r1, #16
	asrs r3, r3, #16
	negs r2, r2
	mov r12, r0
	cmp r3, r2
	beq .L_080ca2f0
	movs r0, #254
	lsls r0, r0, #7
	adds r0, #255
	mov lr, r2
.L_080ca2ae:
	ldrb r2, [r5, #3]
	movs r3, #128
	ands r3, r2
	cmp r3, #0
	beq .L_080ca2c2
	lsls r3, r1, #16
	asrs r3, r3, #16
	cmp r3, r7
	bne .L_080ca2e2
	b .L_080ca2ca
.L_080ca2c2:
	lsls r3, r4, #16
	asrs r3, r3, #16
	cmp r3, r12
	bne .L_080ca2e2
.L_080ca2ca:
	ldrh r2, [r5, #2]
	adds r3, r0, #0
	ands r3, r2
	cmp r3, r0
	beq .L_080ca2dc
	lsls r3, r2, #17
	asrs r3, r3, #17
	cmp r3, r6
	bne .L_080ca2e2
.L_080ca2dc:
	ldr r5, [r5, #4]
	mov r8, r5
	b .L_080ca2f0
.L_080ca2e2:
	adds r5, #8
	ldrh r1, [r5]
	lsls r3, r1, #16
	asrs r3, r3, #16
	adds r4, r1, #0
	cmp r3, lr
	bne .L_080ca2ae
.L_080ca2f0:
	mov r0, r8
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_080ca2f8:
	.4byte Data_080ef4a4
