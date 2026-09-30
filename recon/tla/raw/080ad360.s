.syntax unified
	.thumb
	.global Func_080ad360
	.thumb_func
Func_080ad360:
	push {r5, r6, r7, lr}
	sub sp, #4
	bl Party_CountActiveOwners
	adds r7, r0, #0
	movs r6, #0
	movs r0, #0
	cmp r7, #0
	beq .L_080ad3a0
	cmp r6, r7
	bge .L_080ad396
	ldr r3, .L_080ad3a4
	movs r1, #134
	lsls r1, r1, #2
	adds r2, r3, r1
	adds r5, r7, #0
.L_080ad380:
	ldrb r0, [r2]
	adds r2, #1
	str r2, [sp, #0]
	bl Owner_GetState
	ldrb r3, [r0, #15]
	subs r5, #1
	adds r6, r6, r3
	ldr r2, [sp, #0]
	cmp r5, #0
	bne .L_080ad380
.L_080ad396:
	adds r0, r6, #0
	adds r1, r7, #0
	bl __divsi3
	adds r6, r0, #0
.L_080ad3a0:
	add sp, #4
	pop {r5, r6, r7, pc}
.L_080ad3a4:
	.4byte gPartyState
