.syntax unified
	.thumb
	.global Func_08040f14
	.thumb_func
Func_08040f14:
	push {lr}
	movs r0, #184
	adds r0, #255
	bl PartyInventory_AddFar
	movs r0, #220
	lsls r0, r0, #1
	bl PartyInventory_AddFar
	movs r0, #186
	adds r0, #255
	bl PartyInventory_AddFar
	movs r0, #136
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_SetBit
	ldr r3, .L_08040f50
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #66
	adds r3, r3, r2
	movs r2, #141
	strh r2, [r3]
	ldr r0, .L_08040f54
	movs r1, #9
	bl Event_SetPairWork1c0Far
	pop {pc}
.L_08040f50:
	.4byte gPartyState
.L_08040f54:
	.4byte 0x000000b6
