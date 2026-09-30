.syntax unified
	.thumb
	.global Func_080afe1c
	.thumb_func
Func_080afe1c:
	push {r5, r6, lr}
	adds r5, r0, #0
	bl Party_CountActiveOwners
	adds r6, r0, #0
	adds r0, r5, #0
	bl GameFlag_ClearBit
	movs r1, #0
	cmp r1, r6
	bge .L_080afe4e
	ldr r0, .L_080afe74
	movs r2, #134
	lsls r2, r2, #2
	ldrb r3, [r0, r2]
	cmp r3, r5
	beq .L_080afe4e
	adds r2, r0, r2
.L_080afe40:
	adds r1, #1
	cmp r1, r6
	bge .L_080afe4e
	adds r2, #1
	ldrb r3, [r2]
	cmp r3, r5
	bne .L_080afe40
.L_080afe4e:
	subs r0, r6, #1
	cmp r1, r0
	bge .L_080afe6c
	ldr r3, .L_080afe74
	movs r4, #134
	adds r3, r1, r3
	lsls r4, r4, #2
	adds r2, r3, r4
	subs r1, r0, r1
.L_080afe60:
	ldrb r3, [r2, #1]
	subs r1, #1
	strb r3, [r2]
	adds r2, #1
	cmp r1, #0
	bne .L_080afe60
.L_080afe6c:
	bl Party_CountActiveOwners
	pop {r5, r6, pc}
	.2byte 0x0000
.L_080afe74:
	.4byte gPartyState
