.syntax unified
	.thumb
	.global Summon_ResetCharge
	.thumb_func
Summon_ResetCharge:
	push {r5, r6, r7, lr}
	adds r6, r0, #0
	movs r5, #0
	movs r7, #49
.L_08127c34:
	adds r0, r5, #0
	adds r0, #128
	bl Owner_GetState
	adds r2, r0, #0
	movs r0, #149
	lsls r0, r0, #1
	adds r3, r2, r0
	ldrb r1, [r3]
	cmp r1, #1
	bne .L_08127c76
	adds r0, #32
	adds r3, r2, r0
	ldrh r3, [r3]
	cmp r3, r6
	bne .L_08127c76
	ldrb r3, [r2]
	movs r0, #0
	cmp r3, #0
	bne .L_08127c62
	strb r7, [r2]
	strb r0, [r2, r1]
	b .L_08127c7c
.L_08127c62:
	adds r0, #1
	cmp r0, #13
	bgt .L_08127c7c
	ldrb r1, [r2, r0]
	cmp r1, #0
	bne .L_08127c62
	adds r3, r0, #1
	strb r7, [r2, r0]
	strb r1, [r2, r3]
	b .L_08127c7c
.L_08127c76:
	adds r5, #1
	cmp r5, #5
	ble .L_08127c34
.L_08127c7c:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
