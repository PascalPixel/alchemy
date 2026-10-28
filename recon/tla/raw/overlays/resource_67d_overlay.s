.syntax unified
	.thumb
	.section .text.x0200a00c,"ax",%progbits
	.global Func_0200200c
	.thumb_func
Func_0200200c:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #85
	str r2, [r3]
	ldr r3, .L_0200a18c
	subs r2, #31
	adds r5, r3, r2
	movs r2, #0
	ldrsh r3, [r5, r2]
	cmp r3, #7
	beq .L_0200a034
	movs r0, #212
	lsls r0, r0, #2
	bl GameFlag_ClearBit
.L_0200a034:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #39
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a09e
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #40
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a09e
	movs r2, #0
	ldrsh r3, [r5, r2]
	cmp r3, #7
	bne .L_0200a09e
	movs r0, #7
	bl Func_0200231c
	movs r1, #144
	movs r2, #142
	movs r0, #22
	lsls r1, r1, #16
	lsls r2, r2, #18
	bl Func_02002214
	movs r0, #21
	movs r1, #0
	movs r2, #0
	bl Func_02002214
	movs r0, #16
	movs r1, #0
	movs r2, #0
	bl Func_02002214
	movs r0, #17
	movs r1, #0
	movs r2, #0
	bl Func_02002214
	movs r0, #18
	movs r1, #0
	movs r2, #0
	bl Func_02002214
	movs r0, #19
	movs r1, #0
	movs r2, #0
	bl Func_02002214
.L_0200a09e:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #39
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a104
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #40
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a104
	ldr r3, .L_0200a18c
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #7
	bne .L_0200a104
	movs r0, #212
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a104
	movs r0, #7
	bl Func_0200231c
	movs r0, #16
	movs r1, #0
	movs r2, #0
	bl Func_02002214
	movs r0, #17
	movs r1, #0
	movs r2, #0
	bl Func_02002214
	movs r0, #18
	movs r1, #0
	movs r2, #0
	bl Func_02002214
	movs r0, #19
	movs r1, #0
	movs r2, #0
	bl Func_02002214
.L_0200a104:
	ldr r3, .L_0200a18c
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #5
	bne .L_0200a134
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #39
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a134
	movs r0, #17
	movs r1, #5
	bl Actor_SetModeById
	movs r1, #2
	movs r0, #17
	adds r1, #255
	bl Func_02002274
.L_0200a134:
	ldr r3, .L_0200a18c
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #7
	bne .L_0200a186
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #39
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a186
	movs r0, #9
	movs r1, #5
	bl Actor_SetModeById
	movs r0, #10
	movs r1, #5
	bl Actor_SetModeById
	movs r0, #11
	movs r1, #5
	bl Actor_SetModeById
	movs r1, #5
	movs r0, #12
	bl Actor_SetModeById
	movs r0, #11
	bl Actor_GetById
	adds r5, r0, #0
	movs r0, #11
	bl Actor_GetById
	ldr r3, [r0, #24]
	negs r3, r3
	str r3, [r5, #24]
.L_0200a186:
	movs r0, #0
	pop {r5, pc}
	.2byte 0x0000
.L_0200a18c:
	.4byte gAdventure
