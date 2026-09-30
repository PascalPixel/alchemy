.syntax unified
	.thumb
	.global Func_080da968
	.thumb_func
Func_080da968:
	push {lr}
	ldr r2, [r0, #80]
	movs r3, #253
	strb r3, [r2, #22]
	movs r3, #12
	strb r3, [r2, #23]
	ldr r3, .L_080da9a0
	movs r2, #128
	ldr r3, [r3, #12]
	lsls r2, r2, #2
	ands r3, r2
	cmp r3, #0
	beq .L_080da988
	ldr r2, .L_080da9a4
	movs r3, #1
	str r3, [r2]
.L_080da988:
	ldr r3, .L_080da9a4
	ldr r3, [r3]
	cmp r3, #0
	beq .L_080da99c
	movs r0, #0
	movs r1, #12
	movs r2, #13
	movs r3, #0
	bl Func_080da060
.L_080da99c:
	pop {pc}
	.2byte 0x0000
.L_080da9a0:
	.4byte gInput
.L_080da9a4:
	.4byte Data_080f3950
