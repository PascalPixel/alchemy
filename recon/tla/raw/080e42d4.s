.syntax unified
	.thumb
	.global Func_080e42d4
	.thumb_func
Func_080e42d4:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	ldr r6, .L_080e4398
	movs r2, #133
	lsls r2, r2, #2
	mov r8, r3
	adds r3, r6, r2
	ldr r0, [r3]
	bl ObjectTable_Get
	movs r2, #226
	lsls r2, r2, #1
	adds r2, #255
	adds r3, r6, r2
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	adds r5, r0, #0
	movs r7, #0
	cmp r3, #0
	beq .L_080e4308
	movs r7, #1
.L_080e4308:
	adds r3, r5, #0
	adds r3, #34
	ldr r0, [r5, #8]
	ldr r1, [r5, #16]
	ldrb r2, [r3]
	bl Func_080dbcd8
	cmp r0, #0
	bne .L_080e431c
	movs r7, #2
.L_080e431c:
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #18
	adds r3, r6, r2
	ldrb r3, [r3]
	cmp r3, #5
	bne .L_080e4372
	adds r3, r5, #0
	adds r3, #102
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_080e4372
	adds r3, r5, #0
	adds r3, #100
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_080e4372
	ldr r0, [r5, #8]
	ldr r1, [r5, #16]
	movs r2, #2
	bl Func_080dbd48
	cmp r0, #0
	bne .L_080e4352
	movs r7, #3
.L_080e4352:
	ldr r3, .L_080e439c
	ldr r0, [r5, #8]
	ldr r1, [r5, #16]
	ands r0, r3
	ands r1, r3
	ldr r3, .L_080e43a0
	movs r2, #128
	lsls r2, r2, #12
	adds r0, r0, r2
	adds r1, r1, r3
	movs r2, #2
	bl Func_080dbd48
	cmp r0, #0
	bne .L_080e4372
	movs r7, #4
.L_080e4372:
	cmp r7, #0
	beq .L_080e4392
	adds r0, r5, #0
	bl Func_080e4244
	movs r3, #2
	ands r0, r3
	cmp r0, #0
	bne .L_080e4392
	movs r2, #179
	movs r3, #128
	lsls r2, r2, #1
	lsls r3, r3, #6
	add r2, r8
	adds r3, #139
	strh r3, [r2]
.L_080e4392:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_080e4398:
	.4byte gPartyState
.L_080e439c:
	.4byte 0xfff00000
.L_080e43a0:
	.4byte 0xfff80000
