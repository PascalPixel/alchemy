.syntax unified
	.thumb
	.global Func_0811d914
	.thumb_func
Func_0811d914:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r6, r0, #0
	ldrb r3, [r6]
	sub sp, #20
	cmp r3, #7
	bhi .L_0811d93a
	mov r7, sp
	adds r0, r7, #0
	bl BattleParty_PrepareActiveOwners
	adds r5, r0, #0
	lsls r0, r5, #1
	adds r0, r7, r0
	bl BattleParty_PrepareReserveOwners
	adds r5, r5, r0
	b .L_0811d946
.L_0811d93a:
	mov r7, sp
	movs r0, #2
	adds r1, r7, #0
	bl BattleParty_ListActorIds
	adds r5, r0, #0
.L_0811d946:
	cmp r5, #0
	ble .L_0811d988
	movs r3, #0
	mov r8, r3
	movs r3, #1
	mov lr, r3
	movs r3, #31
	adds r3, r3, r6
	adds r2, r6, #0
	mov r12, r3
	adds r2, #17
	adds r1, r6, #3
	adds r0, r7, #0
	adds r4, r5, #0
.L_0811d962:
	ldrh r3, [r0]
	mov r7, r8
	strb r3, [r1]
	strb r7, [r2]
	mov r3, lr
	mov r7, r12
	strb r3, [r7]
	mov r3, r8
	strb r3, [r2, #28]
	mov r3, lr
	strb r3, [r7, #28]
	subs r4, #1
	movs r7, #1
	adds r0, #2
	adds r1, #1
	add r12, r7
	adds r2, #1
	cmp r4, #0
	bne .L_0811d962
.L_0811d988:
	movs r2, #149
	lsls r2, r2, #1
	str r2, [r6, #76]
	movs r2, #144
	lsls r2, r2, #9
	movs r3, #0
	adds r2, #182
	strb r5, [r6, #1]
	str r2, [r6, #88]
	str r3, [r6, #96]
	add sp, #20
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
