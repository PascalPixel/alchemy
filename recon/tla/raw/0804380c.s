.syntax unified
	.thumb
	.global Func_0804380c
	.thumb_func
Func_0804380c:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r7, #0
	bl Func_0801596c
	cmp r0, #0
	beq .L_08043828
	ldr r0, .L_080438b4
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	subs r7, #9
	b .L_080438a6
.L_08043828:
	bl Func_08016054
	movs r0, #0
	movs r1, #2
	bl Func_08043cd8
	movs r3, #1
	adds r6, r0, #0
	negs r3, r3
	cmp r6, r3
	bne .L_08043842
	adds r7, r6, #0
	b .L_080438a6
.L_08043842:
	ldr r3, .L_080438b8
	adds r0, r6, #0
	mov r8, r3
	mov r1, r8
	bl Func_08015e44
	adds r6, r0, #0
	cmp r6, #0
	beq .L_0804385c
	ldr r0, .L_080438bc
	movs r1, #1
	movs r7, #2
	b .L_08043896
.L_0804385c:
	bl Func_08043230
	movs r3, #186
	lsls r3, r3, #2
	adds r6, r0, #0
	adds r3, #255
	cmp r6, r3
	bne .L_08043874
	ldr r0, .L_080438c0
	movs r1, #1
	movs r7, #5
	b .L_08043896
.L_08043874:
	movs r0, #2
	bl Func_08042e28
	mov r1, r8
	adds r5, r0, #0
	adds r0, r6, #0
	bl Func_08042f10
	adds r6, r0, #0
	adds r0, r5, #0
	bl Func_08042eec
	cmp r6, #0
	beq .L_0804389e
	ldr r0, .L_080438c0
	movs r1, #1
	movs r7, #3
.L_08043896:
	bl UiText_ShowPositionedMessageAndWait
	negs r7, r7
	b .L_080438a6
.L_0804389e:
	ldr r0, .L_080438c4
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
.L_080438a6:
	bl Func_0801613c
	adds r0, r7, #0
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080438b4:
	.4byte 0x0000000b
.L_080438b8:
	.4byte Data_02000000
.L_080438bc:
	.4byte 0x0000000d
.L_080438c0:
	.4byte 0x0000000e
.L_080438c4:
	.4byte 0x0000001b
