.syntax unified
	.thumb
	.global Func_0802dc48
	.thumb_func
Func_0802dc48:
	push {r5, r6, r7, lr}
	adds r5, r0, #0
	adds r6, r1, #0
	bl Func_0802dac0
	ldr r3, [r5]
	adds r7, r0, #0
	movs r4, #0
	cmp r3, #0
	bge .L_0802dc60
	ldr r2, .L_0802dcb0
	adds r3, r3, r2
.L_0802dc60:
	ldr r0, [r5, #8]
	asrs r2, r3, #21
	movs r1, #31
	ands r2, r1
	cmp r0, #0
	bge .L_0802dc70
	ldr r3, .L_0802dcb0
	adds r0, r0, r3
.L_0802dc70:
	asrs r3, r0, #21
	ands r3, r1
	lsls r3, r3, #5
	adds r3, r2, r3
	ldr r2, .L_0802dcb4
	lsls r3, r3, #2
	adds r0, r3, r2
	ldrb r2, [r0, #3]
	movs r3, #128
	ands r3, r2
	cmp r3, #0
	beq .L_0802dc8a
	movs r4, #16
.L_0802dc8a:
	ldr r3, [r0]
	lsls r3, r3, #2
	lsrs r0, r3, #26
	str r0, [r6]
	cmp r0, #40
	bls .L_0802dc98
	movs r4, #32
.L_0802dc98:
	cmp r0, #36
	bne .L_0802dc9e
	movs r4, #64
.L_0802dc9e:
	cmp r0, #12
	beq .L_0802dca6
	cmp r0, #15
	bne .L_0802dca8
.L_0802dca6:
	movs r4, #48
.L_0802dca8:
	ldr r3, .L_0802dcb8
	adds r2, r4, r7
	ldrb r0, [r3, r2]
	pop {r5, r6, r7, pc}
.L_0802dcb0:
	.4byte 0x001fffff
.L_0802dcb4:
	.4byte gMapBlocks
.L_0802dcb8:
	.4byte Data_0802f004
