.syntax unified
	.thumb
	.global Func_08014128
	.thumb_func
Func_08014128:
	push {r5, r6, lr}
	adds r5, r0, #0
	cmp r1, #255
	ble .L_08014132
	movs r1, #255
.L_08014132:
	cmp r1, #0
	bge .L_08014138
	movs r1, #0
.L_08014138:
	ldr r3, .L_0801416c
	lsrs r2, r1, #3
	adds r6, r2, r3
	ldrb r4, [r6]
	ldr r3, .L_08014170
	movs r0, #7
	lsls r2, r1, #2
	ands r0, r1
	adds r1, r4, #0
	adds r2, r2, r3
	lsrs r1, r0
	movs r3, #1
	ands r1, r3
	cmp r1, #0
	beq .L_0801415e
	ldr r3, [r2]
	str r5, [r2]
	str r3, [r5]
	b .L_08014168
.L_0801415e:
	lsls r3, r0
	orrs r4, r3
	strb r4, [r6]
	str r5, [r2]
	str r1, [r5]
.L_08014168:
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0801416c:
	.4byte gOamBucketMasks
.L_08014170:
	.4byte gOamBuckets
