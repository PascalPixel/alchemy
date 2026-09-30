.syntax unified
	.thumb
	.global Menu_HandleFlagGridInput
	.thumb_func
Menu_HandleFlagGridInput:
	push {r5, r6, lr}
	ldr r6, .L_0804e09c
	adds r5, r2, #0
	ldr r3, [r6, #12]
	movs r2, #1
	ands r3, r2
	adds r4, r5, #4
	cmp r3, #0
	beq .L_0804df9e
	ldr r3, [r1]
	ldr r2, [r4]
	lsls r3, r3, #4
	adds r3, r3, r2
	ldr r2, [r5]
	lsls r3, r3, #4
	adds r5, r3, r2
	adds r0, r5, #0
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0804df96
	adds r0, r5, #0
	bl GameFlag_ClearBit
	b .L_0804e092
.L_0804df96:
	adds r0, r5, #0
	bl GameFlag_SetBit
	b .L_0804e092
.L_0804df9e:
	ldr r3, [r6, #4]
	movs r2, #2
	ands r3, r2
	cmp r3, #0
	bne .L_0804dfb2
	ldr r3, [r6, #12]
	movs r2, #4
	ands r3, r2
	cmp r3, #0
	beq .L_0804dfb8
.L_0804dfb2:
	movs r0, #1
	negs r0, r0
	b .L_0804e098
.L_0804dfb8:
	ldr r0, [r6, #12]
	movs r3, #64
	ands r0, r3
	cmp r0, #0
	beq .L_0804dfd2
	ldr r3, [r4]
	subs r3, #1
	str r3, [r4]
	cmp r3, #0
	bge .L_0804e096
	movs r3, #15
	str r3, [r4]
	b .L_0804e096
.L_0804dfd2:
	ldr r3, [r6, #12]
	movs r2, #128
	ands r3, r2
	cmp r3, #0
	beq .L_0804dfea
	ldr r3, [r4]
	adds r3, #1
	str r3, [r4]
	cmp r3, #15
	ble .L_0804e096
	str r0, [r4]
	b .L_0804e096
.L_0804dfea:
	ldr r0, [r6, #12]
	movs r3, #32
	ands r0, r3
	cmp r0, #0
	beq .L_0804e004
	ldr r3, [r5]
	subs r3, #1
	str r3, [r5]
	cmp r3, #0
	bge .L_0804e096
	movs r3, #15
	str r3, [r5]
	b .L_0804e096
.L_0804e004:
	ldr r3, [r6, #12]
	movs r2, #16
	ands r3, r2
	cmp r3, #0
	beq .L_0804e01c
	ldr r3, [r5]
	adds r3, #1
	str r3, [r5]
	cmp r3, #15
	ble .L_0804e096
	str r0, [r5]
	b .L_0804e096
.L_0804e01c:
	ldr r3, [r6, #12]
	movs r2, #128
	lsls r2, r2, #2
	ands r3, r2
	cmp r3, #0
	beq .L_0804e038
	ldr r3, [r6, #12]
	movs r2, #8
	ands r3, r2
	cmp r3, #0
	beq .L_0804e038
	ldr r3, [r1]
	subs r3, #10
	b .L_0804e06e
.L_0804e038:
	ldr r3, [r6, #12]
	movs r2, #128
	lsls r2, r2, #1
	ands r3, r2
	cmp r3, #0
	beq .L_0804e05e
	ldr r3, [r6, #12]
	movs r2, #8
	ands r3, r2
	cmp r3, #0
	beq .L_0804e05e
	ldr r3, [r1]
	adds r3, #10
	str r3, [r1]
	cmp r3, #15
	ble .L_0804e092
	movs r3, #0
	str r3, [r1]
	b .L_0804e092
.L_0804e05e:
	ldr r0, [r6, #12]
	movs r3, #128
	lsls r3, r3, #2
	ands r0, r3
	cmp r0, #0
	beq .L_0804e07a
	ldr r3, [r1]
	subs r3, #1
.L_0804e06e:
	str r3, [r1]
	cmp r3, #0
	bge .L_0804e092
	movs r3, #15
	str r3, [r1]
	b .L_0804e092
.L_0804e07a:
	ldr r3, [r6, #12]
	movs r2, #128
	lsls r2, r2, #1
	ands r3, r2
	cmp r3, #0
	beq .L_0804e096
	ldr r3, [r1]
	adds r3, #1
	str r3, [r1]
	cmp r3, #15
	ble .L_0804e092
	str r0, [r1]
.L_0804e092:
	movs r0, #1
	b .L_0804e098
.L_0804e096:
	movs r0, #0
.L_0804e098:
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0804e09c:
	.4byte gInput
	.4byte 0x00004770
