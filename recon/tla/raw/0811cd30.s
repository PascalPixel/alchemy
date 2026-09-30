.syntax unified
	.thumb
	.global Func_0811cd30
	.thumb_func
Func_0811cd30:
	push {r5, r6, lr}
	movs r2, #10
	ldrsh r5, [r0, r2]
	sub sp, #28
	adds r0, r5, #0
	bl Owner_GetState
	movs r2, #56
	ldrsh r3, [r0, r2]
	adds r0, r5, #0
	cmp r3, #0
	bne .L_0811cd76
	cmp r5, #127
	ble .L_0811cd52
	mov r6, sp
	movs r0, #2
	b .L_0811cd56
.L_0811cd52:
	mov r6, sp
	movs r0, #1
.L_0811cd56:
	adds r1, r6, #0
	bl BattleParty_ListLivingUnits
	adds r5, r0, #0
	cmp r5, #0
	bne .L_0811cd68
	movs r0, #128
	lsls r0, r0, #1
	b .L_0811cd76
.L_0811cd68:
	bl Random16
	adds r3, r5, #0
	muls r3, r0
	lsrs r3, r3, #16
	lsls r3, r3, #1
	ldrsh r0, [r6, r3]
.L_0811cd76:
	add sp, #28
	pop {r5, r6, pc}
	.2byte 0x0000
