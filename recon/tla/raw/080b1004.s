.syntax unified
	.thumb
	.global Func_080b1004
	.thumb_func
Func_080b1004:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	sub sp, #32
	mov r5, sp
	adds r0, r5, #0
	bl Party_ListActiveOwners
	mov r10, r0
	bl BattlePlacement_UpdateTimedEntriesTwentyTimesFar
	movs r3, #0
	mov r8, r3
	cmp r8, r10
	bge .L_080b1062
	adds r7, r5, #0
.L_080b1026:
	movs r6, #0
.L_080b1028:
	movs r5, #0
.L_080b102a:
	ldrh r0, [r7]
	adds r1, r6, #0
	adds r2, r5, #0
	bl Trade_CanOfferDjinn
	cmp r0, #0
	beq .L_080b104c
	adds r1, r6, #0
	adds r2, r5, #0
	ldrh r0, [r7]
	bl Djinn_Activate
	ldrh r0, [r7]
	adds r1, r6, #0
	adds r2, r5, #0
	bl Trade_RemoveOffer
.L_080b104c:
	adds r5, #1
	cmp r5, #19
	ble .L_080b102a
	adds r6, #1
	cmp r6, #3
	ble .L_080b1028
	movs r3, #1
	add r8, r3
	adds r7, #2
	cmp r8, r10
	blt .L_080b1026
.L_080b1062:
	add sp, #32
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
