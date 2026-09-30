.syntax unified
	.thumb
	.global Func_080432a4
	.thumb_func
Func_080432a4:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	bl Func_0801596c
	movs r1, #9
	negs r1, r1
	movs r6, #0
	mov r8, r1
	cmp r0, #0
	bne .L_0804333a
	bl Func_08016054
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #204
	ldr r2, .L_0804330c
	ldr r7, [r3]
	ldr r1, .L_08043310
	ldr r3, .L_08043314
	strh r6, [r2]
	mov r10, r3
	strh r6, [r1]
	strh r6, [r3]
	movs r3, #192
	mov lr, r2
	lsls r3, r3, #6
	ldr r2, .L_08043308
	adds r3, #108
	mov r8, r0
	movs r5, #0
	mov r12, r1
	adds r0, r7, r3
.L_080432e8:
	movs r3, #1
	ldrsb r3, [r0, r3]
	lsls r4, r5, #6
	cmp r3, #0
	beq .L_080432f8
	mov r1, lr
	strh r2, [r1]
	adds r6, #1
.L_080432f8:
	movs r3, #2
	ldrsb r3, [r0, r3]
	cmp r3, #0
	beq .L_08043318
	mov r3, r12
	strh r2, [r3]
	b .L_08043318
	.2byte 0x0000
.L_08043308:
	.4byte 0x00000001
.L_0804330c:
	.4byte Data_02003860
.L_08043310:
	.4byte Data_020036d8
.L_08043314:
	.4byte Data_02005350
.L_08043318:
	movs r1, #192
	lsls r1, r1, #6
	adds r1, #88
	adds r3, r4, r1
	ldrb r3, [r7, r3]
	cmp r3, #0
	beq .L_08043332
	movs r3, #3
	ldrsb r3, [r0, r3]
	cmp r3, #0
	bne .L_08043332
	mov r3, r10
	strh r2, [r3]
.L_08043332:
	adds r5, #1
	adds r0, #64
	cmp r5, #2
	ble .L_080432e8
.L_0804333a:
	bl Func_0801613c
	mov r1, r8
	cmp r1, #0
	beq .L_0804334e
	cmp r6, r8
	bne .L_0804334e
	mov r0, r8
	adds r0, #100
	b .L_08043350
.L_0804334e:
	mov r0, r8
.L_08043350:
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
