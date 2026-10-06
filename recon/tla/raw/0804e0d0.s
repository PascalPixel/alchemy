.syntax unified
	.thumb
	.global Func_0804e0d0
	.thumb_func
Func_0804e0d0:
	push {r5, r6, lr}
	movs r5, #0
.L_0804e0d4:
	adds r0, r5, #0
	bl Func_08040798
	movs r6, #1
	adds r5, r0, #0
	negs r6, r6
	adds r0, r6, #0
	cmp r5, r6
	beq .L_0804e13a
	cmp r5, #0
	bne .L_0804e11a
	movs r0, #190
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0804e0fa
	ldr r0, .L_0804e13c
	b .L_0804e108
.L_0804e0fa:
	movs r0, #126
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0804e110
	ldr r0, .L_0804e140
.L_0804e108:
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	b .L_0804e0d4
.L_0804e110:
	bl Func_08043690
	cmp r0, r6
	bne .L_0804e138
	b .L_0804e0d4
.L_0804e11a:
	cmp r5, #1
	bne .L_0804e12c
	ldr r0, .L_0804e144
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	ldr r3, .L_0804e148
	strb r5, [r3]
	b .L_0804e138
.L_0804e12c:
	cmp r5, #2
	bne .L_0804e138
	bl Func_080400e8
	cmp r0, r6
	beq .L_0804e0d4
.L_0804e138:
	movs r0, #0
.L_0804e13a:
	pop {r5, r6, pc}
.L_0804e13c:
	.4byte 0x00000022
.L_0804e140:
	.4byte 0x00000023
.L_0804e144:
	.4byte 0x00001160
.L_0804e148:
	.4byte gSleepRequested
