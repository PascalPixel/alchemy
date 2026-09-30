.syntax unified
	.thumb
	.global Func_0810b6cc
	.thumb_func
Func_0810b6cc:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r3, [r3]
	mov r10, r1
	mov r9, r3
	adds r7, r0, #0
	bl Owner_GetState
	mov r2, r10
	lsls r3, r2, #1
	adds r3, #216
	ldrh r3, [r0, r3]
	movs r6, #128
	lsls r6, r6, #1
	adds r6, #255
	ands r6, r3
	adds r0, r6, #0
	bl Func_0810a748
	movs r3, #1
	adds r5, r0, #0
	negs r3, r3
	adds r0, r6, #0
	mov r8, r3
	bl Func_0810a748
	cmp r0, #0
	bne .L_0810b718
	ldr r0, .L_0810b780
.L_0810b712:
	bl Func_0810857c
	b .L_0810b774
.L_0810b718:
	adds r0, r6, #0
	movs r1, #2
	bl UiText_DrawQuantity
	cmp r5, #1
	bne .L_0810b728
	ldr r0, .L_0810b784
	b .L_0810b72a
.L_0810b728:
	ldr r0, .L_0810b788
.L_0810b72a:
	bl Func_0810857c
	movs r0, #0
	bl Func_08108630
	cmp r0, #0
	beq .L_0810b744
	cmp r5, #1
	bne .L_0810b740
	ldr r0, .L_0810b78c
	b .L_0810b712
.L_0810b740:
	ldr r0, .L_0810b790
	b .L_0810b712
.L_0810b744:
	cmp r5, #1
	bne .L_0810b74c
	ldr r0, .L_0810b794
	b .L_0810b74e
.L_0810b74c:
	ldr r0, .L_0810b798
.L_0810b74e:
	bl Func_0810857c
	mov r1, r10
	adds r0, r7, #0
	bl Inventory_RemoveFar
	mov r2, r9
	ldr r0, [r2, #36]
	adds r1, r7, #0
	bl Func_0810a004
	adds r0, r6, #0
	bl Func_0810a760
	adds r6, r0, #0
	bl Func_0810a834
	movs r3, #0
	mov r8, r3
.L_0810b774:
	mov r0, r8
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_0810b780:
	.4byte 0x00001300
.L_0810b784:
	.4byte 0x00001301
.L_0810b788:
	.4byte 0x00001302
.L_0810b78c:
	.4byte 0x00001303
.L_0810b790:
	.4byte 0x00001304
.L_0810b794:
	.4byte 0x00001305
.L_0810b798:
	.4byte 0x00001306
