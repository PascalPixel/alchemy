.syntax unified
	.thumb
	.global Func_0802dac0
	.thumb_func
Func_0802dac0:
	push {r5, lr}
	ldr r4, [r0, #8]
	ldr r3, [r0]
	asrs r2, r4, #17
	asrs r1, r3, #17
	adds r3, r2, #0
	cmp r2, #0
	bge .L_0802dad2
	adds r3, r2, #7
.L_0802dad2:
	asrs r3, r3, #3
	movs r5, #63
	ands r3, r5
	lsls r0, r3, #6
	adds r3, r1, #0
	cmp r1, #0
	bge .L_0802dae2
	adds r3, r1, #7
.L_0802dae2:
	asrs r3, r3, #3
	ands r3, r5
	adds r5, r0, r3
	lsrs r3, r4, #31
	adds r3, r2, r3
	asrs r3, r3, #1
	movs r2, #3
	ands r3, r2
	lsls r0, r3, #1
	adds r3, r1, #0
	cmp r1, #0
	bge .L_0802dafc
	adds r3, r1, #3
.L_0802dafc:
	movs r2, #1
	asrs r3, r3, #2
	ands r3, r2
	ldr r2, .L_0802db54
	adds r4, r0, r3
	adds r3, r5, r2
	ldrb r3, [r3]
	ldr r2, .L_0802db58
	lsls r3, r3, #3
	adds r3, r3, r4
	adds r3, r3, r2
	ldrb r2, [r3]
	cmp r2, #0
	beq .L_0802db2a
	movs r3, #2
	ands r3, r1
	lsrs r0, r2, #4
	cmp r3, #0
	bne .L_0802db26
	movs r0, #15
	ands r0, r2
.L_0802db26:
	cmp r0, #0
	bne .L_0802db52
.L_0802db2a:
	ldr r2, .L_0802db5c
	adds r3, r5, r2
	ldrb r3, [r3]
	ldr r2, .L_0802db60
	lsls r3, r3, #3
	adds r3, r3, r4
	adds r3, r3, r2
	ldrb r2, [r3]
	cmp r2, #0
	beq .L_0802db50
	movs r3, #2
	ands r3, r1
	lsrs r0, r2, #4
	cmp r3, #0
	bne .L_0802db4c
	movs r0, #15
	ands r0, r2
.L_0802db4c:
	cmp r0, #0
	bne .L_0802db52
.L_0802db50:
	movs r0, #7
.L_0802db52:
	pop {r5, pc}
.L_0802db54:
	.4byte 0x06005000
.L_0802db58:
	.4byte Data_0202c800
.L_0802db5c:
	.4byte 0x06004000
.L_0802db60:
	.4byte Data_0202c000
