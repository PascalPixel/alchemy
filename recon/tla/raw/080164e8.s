.syntax unified
	.thumb
	.global Func_080164e8
	.thumb_func
Func_080164e8:
	push {r5, r6, r7, lr}
	ldr r3, .L_0801653c
	ldr r5, .L_08016540
	ldr r7, [r3]
	ldrb r3, [r5, #1]
	adds r6, r0, #0
	adds r0, r1, #0
	cmp r3, #1
	bne .L_0801650a
	bl Func_080165b4
	adds r0, r6, #0
	bl Func_08016544
	ldrb r3, [r5, #11]
	adds r3, #1
	strb r3, [r5, #11]
.L_0801650a:
	ldrb r3, [r5, #2]
	ldrb r2, [r5, #3]
	lsls r3, r3, #8
	orrs r2, r3
	ldrb r3, [r5]
	cmp r3, #8
	bne .L_0801651c
	movs r3, #128
	orrs r2, r3
.L_0801651c:
	ldrb r3, [r5, #9]
	adds r0, r2, #0
	cmp r3, #0
	beq .L_0801652a
	movs r3, #128
	lsls r3, r3, #5
	orrs r0, r3
.L_0801652a:
	lsls r3, r7, #26
	lsrs r3, r3, #30
	cmp r3, #1
	bls .L_08016538
	movs r3, #128
	lsls r3, r3, #6
	orrs r0, r3
.L_08016538:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0801653c:
	.4byte 0x04000128
.L_08016540:
	.4byte gSerialRuntime
