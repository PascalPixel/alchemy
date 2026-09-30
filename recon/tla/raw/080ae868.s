.syntax unified
	.thumb
	.global Func_080ae868
	.thumb_func
Func_080ae868:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r0, #128
	lsls r0, r0, #4
	sub sp, #12
	bl GameFlag_Test
	cmp r0, #0
	beq .L_080ae886
	b .L_080aea18
.L_080ae886:
	bl Party_CountActiveOwners
	mov r11, r0
	cmp r0, #0
	ble .L_080ae8aa
	ldr r3, .L_080aea28
	movs r0, #134
	lsls r0, r0, #2
	add r2, sp, #4
	adds r1, r3, r0
	mov r5, r11
.L_080ae89c:
	ldrb r3, [r1]
	subs r5, #1
	strb r3, [r2]
	adds r1, #1
	adds r2, #1
	cmp r5, #0
	bne .L_080ae89c
.L_080ae8aa:
	mov r1, r11
	cmp r1, #0
	ble .L_080ae8c6
	add r6, sp, #4
	mov r5, r11
.L_080ae8b4:
	ldrb r0, [r6]
	subs r5, #1
	lsls r0, r0, #24
	asrs r0, r0, #24
	adds r6, #1
	bl Func_080afe1c
	cmp r5, #0
	bne .L_080ae8b4
.L_080ae8c6:
	movs r5, #0
.L_080ae8c8:
	adds r0, r5, #0
	adds r5, #1
	bl Func_080afdd8
	cmp r5, #3
	ble .L_080ae8c8
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl Djinn_AddFoundToOwner
	movs r2, #0
	movs r0, #3
	movs r1, #1
	bl Djinn_AddFoundToOwner
	movs r3, #1
	mov r6, sp
	movs r2, #0
	strb r3, [r6]
	strb r3, [r6, #1]
	strb r2, [r6, #2]
	strb r2, [r6, #3]
	movs r2, #16
	mov r9, r2
	mov r10, r6
.L_080ae8fc:
	bl Random16
	lsls r0, r0, #2
	lsrs r5, r0, #16
	bl Random16
	lsls r3, r0, #3
	subs r3, r3, r0
	lsls r0, r5, #2
	lsrs r3, r3, #16
	adds r0, r0, r5
	mov r8, r3
	lsls r0, r0, #2
	add r0, r8
	adds r0, #48
	bl GameFlag_Test
	cmp r0, #0
	bne .L_080ae99e
	mov r0, r10
	ldrb r3, [r0, r5]
	movs r1, #144
	lsls r1, r1, #20
	adds r3, #1
	strb r3, [r0, r5]
	mov r12, r1
	movs r7, #0
	adds r1, r6, #0
	adds r0, r6, #0
	adds r2, r6, #3
.L_080ae938:
	ldrb r3, [r0]
	adds r0, #1
	lsls r3, r3, #24
	asrs r4, r3, #24
	cmp r12, r3
	ble .L_080ae94a
	ldrb r3, [r1]
	lsls r3, r3, #24
	mov r12, r3
.L_080ae94a:
	asrs r3, r7, #24
	cmp r3, r4
	bge .L_080ae954
	ldrb r3, [r1]
	lsls r7, r3, #24
.L_080ae954:
	adds r1, #1
	cmp r1, r2
	ble .L_080ae938
	mov r2, r10
	ldrb r3, [r2, r5]
	mov r0, r12
	subs r3, #1
	strb r3, [r2, r5]
	asrs r3, r7, #24
	asrs r2, r0, #24
	subs r3, r3, r2
	cmp r3, #1
	bgt .L_080ae99e
	bl Random16
	movs r3, #100
	adds r2, r0, #0
	muls r2, r3
	lsls r3, r5, #3
	ldr r1, .L_080aea2c
	subs r3, r3, r5
	add r3, r8
	lsls r3, r3, #1
	ldrsh r3, [r1, r3]
	lsrs r2, r2, #16
	cmp r2, r3
	bcs .L_080ae99e
	mov r1, r8
	adds r0, r5, #0
	bl Djinn_AddToLeastLoadedOwner
	ldrb r3, [r6, r5]
	movs r1, #1
	adds r3, #1
	strb r3, [r6, r5]
	negs r1, r1
	add r9, r1
.L_080ae99e:
	mov r2, r9
	cmp r2, #0
	bne .L_080ae8fc
	bl Func_080b1004
	movs r5, #0
	mov r10, r5
.L_080ae9ac:
	movs r3, #0
	mov r8, r3
	lsls r3, r5, #2
	adds r3, r3, r5
	lsls r3, r3, #2
	movs r7, #128
	adds r6, r3, #0
	lsls r7, r7, #4
	adds r6, #48
	add r7, r10
.L_080ae9c0:
	adds r0, r6, #0
	bl GameFlag_Test
	cmp r0, #0
	beq .L_080ae9d0
	adds r0, r7, #0
	bl GameFlag_SetBit
.L_080ae9d0:
	adds r0, r6, #0
	bl GameFlag_ClearBit
	movs r0, #1
	add r8, r0
	mov r1, r8
	adds r6, #1
	adds r7, #1
	cmp r1, #6
	ble .L_080ae9c0
	movs r2, #7
	adds r5, #1
	add r10, r2
	cmp r5, #3
	ble .L_080ae9ac
	movs r5, #0
.L_080ae9f0:
	adds r0, r5, #0
	adds r5, #1
	bl Func_080afe1c
	cmp r5, #3
	ble .L_080ae9f0
	mov r3, r11
	cmp r3, #0
	ble .L_080aea18
	add r6, sp, #4
	mov r5, r11
.L_080aea06:
	ldrb r0, [r6]
	subs r5, #1
	lsls r0, r0, #24
	asrs r0, r0, #24
	adds r6, #1
	bl Func_080afdd8
	cmp r5, #0
	bne .L_080aea06
.L_080aea18:
	add sp, #12
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080aea28:
	.4byte gPartyState
.L_080aea2c:
	.4byte Data_080b1290
