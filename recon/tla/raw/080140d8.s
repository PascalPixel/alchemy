.syntax unified
	.thumb
	.global Func_080140d8
	.thumb_func
Func_080140d8:
	push {r5, r6, r7, lr}
	adds r5, r0, #0
	cmp r1, #239
	bls .L_080140ea
	cmp r1, #0
	bge .L_080140e8
	movs r1, #0
	b .L_080140ea
.L_080140e8:
	movs r1, #239
.L_080140ea:
	ldr r3, .L_08014120
	lsrs r2, r1, #3
	adds r6, r2, r3
	ldrb r2, [r6]
	movs r0, #7
	ands r0, r1
	adds r4, r2, #0
	lsrs r4, r0
	movs r3, #1
	ands r4, r3
	ldr r7, .L_08014124
	cmp r4, #0
	beq .L_08014110
	lsls r3, r1, #2
	adds r3, r3, r7
	ldr r2, [r3]
	str r5, [r3]
	str r2, [r5]
	b .L_0801411c
.L_08014110:
	lsls r3, r0
	orrs r2, r3
	lsls r3, r1, #2
	strb r2, [r6]
	str r5, [r3, r7]
	str r4, [r5]
.L_0801411c:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_08014120:
	.4byte gOamBucketMasks
.L_08014124:
	.4byte gOamBuckets
