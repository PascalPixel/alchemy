.syntax unified
	.thumb
	.global Func_080afdd8
	.thumb_func
Func_080afdd8:
	push {r5, r6, lr}
	adds r6, r0, #0
	bl Party_CountActiveOwners
	adds r5, r0, #0
	adds r0, r6, #0
	bl GameFlag_SetBit
	movs r2, #0
	cmp r2, r5
	bge .L_080afe0a
	ldr r0, .L_080afe18
	movs r3, #134
	lsls r3, r3, #2
	adds r1, r0, r3
.L_080afdf6:
	ldrb r3, [r1]
	adds r1, #1
	cmp r3, r6
	beq .L_080afe06
	adds r2, #1
	cmp r2, r5
	blt .L_080afdf6
	b .L_080afe0c
.L_080afe06:
	adds r0, r5, #0
	b .L_080afe16
.L_080afe0a:
	ldr r0, .L_080afe18
.L_080afe0c:
	movs r1, #134
	lsls r1, r1, #2
	adds r3, r2, r1
	strb r6, [r0, r3]
	adds r0, r5, #1
.L_080afe16:
	pop {r5, r6, pc}
.L_080afe18:
	.4byte gPartyState
