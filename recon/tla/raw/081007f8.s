.syntax unified
	.thumb
	.global Func_081007f8
	.thumb_func
Func_081007f8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #20
	str r1, [sp, #12]
	str r3, [sp, #8]
	str r0, [sp, #16]
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r3, [r3]
	mov r10, r2
	str r3, [sp, #4]
	bl BattleAction_Get
	movs r1, #0
	mov r11, r0
	mov r2, r10
	movs r0, #0
	mov r9, r0
	movs r7, #0
	mov r8, r1
	cmp r2, #9
	bne .L_0810083c
	ldr r3, .L_08100b8c
	movs r4, #133
	lsls r4, r4, #2
	adds r3, r3, r4
	ldr r0, [r3]
	b .L_0810083e
.L_0810083c:
	mov r0, r10
.L_0810083e:
	bl Owner_GetState
	adds r5, r0, #0
	mov r0, r11
	ldrb r2, [r0, #8]
	movs r1, #0
	ldr r4, [sp, #4]
	movs r0, #139
	str r1, [sp, #0]
	lsls r0, r0, #1
	adds r0, #255
	adds r3, r4, r0
	ldrb r3, [r3]
	cmp r1, r3
	bcc .L_0810085e
	b .L_08100cd0
.L_0810085e:
	adds r3, r2, #0
	cmp r3, #255
	bne .L_0810087c
	ldr r1, [sp, #0]
	ldr r4, [sp, #4]
	movs r2, #129
	lsls r3, r1, #1
	lsls r2, r2, #2
	adds r3, r3, r2
	ldrh r3, [r4, r3]
	mov r10, r3
	mov r0, r10
	bl Owner_GetState
	adds r5, r0, #0
.L_0810087c:
	mov r0, r11
	ldrb r2, [r0, #1]
	movs r3, #15
	ands r3, r2
	subs r3, #1
	ldrh r6, [r0, #10]
	cmp r3, #10
	bls .L_0810088e
	b .L_08100a74
.L_0810088e:
	ldr r2, .L_08100b90
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
	.2byte 0x0000
.L_08100898:
	.4byte .L_081008c4
	.4byte .L_08100a74
	.4byte .L_08100a74
	.4byte .L_08100a74
	.4byte .L_08100a74
	.4byte .L_08100a74
	.4byte .L_08100a74
	.4byte .L_08100a74
	.4byte .L_0810095e
	.4byte .L_08100a74
	.4byte .L_08100a1e
.L_081008c4:
	ldr r1, [sp, #8]
	cmp r1, #0
	bne .L_081008f2
	mov r2, r11
	ldrb r3, [r2, #2]
	cmp r3, #4
	beq .L_081008e4
	ldr r0, [sp, #12]
	bl Owner_GetState
	mov r4, r11
	ldrb r3, [r4, #2]
	lsls r3, r3, #2
	adds r3, #72
	ldrsh r1, [r0, r3]
	b .L_081008e6
.L_081008e4:
	movs r1, #100
.L_081008e6:
	movs r2, #128
	adds r0, r6, #0
	lsls r2, r2, #1
	bl Battle_CalcRestore
	adds r6, r0, #0
.L_081008f2:
	movs r4, #56
	ldrsh r1, [r5, r4]
	ldrh r3, [r5, #56]
	cmp r1, #0
	bgt .L_08100908
	mov r0, r8
	cmp r0, #0
	beq .L_08100904
	b .L_08100a74
.L_08100904:
	movs r7, #2
	b .L_08100a74
.L_08100908:
	movs r4, #52
	ldrsh r2, [r5, r4]
	ldrh r0, [r5, #52]
	cmp r1, r2
	bne .L_0810091e
	mov r0, r8
	cmp r0, #0
	beq .L_0810091a
	b .L_08100a74
.L_0810091a:
	movs r7, #4
	b .L_08100a74
.L_0810091e:
	adds r3, r3, r6
	strh r3, [r5, #56]
	lsls r3, r3, #16
	asrs r3, r3, #16
	cmp r3, r2
	ble .L_0810093a
	subs r3, r3, r2
	mov r1, r8
	subs r6, r6, r3
	strh r0, [r5, #56]
	cmp r1, #0
	bne .L_08100942
	movs r7, #0
	b .L_08100942
.L_0810093a:
	mov r2, r8
	cmp r2, #0
	bne .L_08100942
	movs r7, #1
.L_08100942:
	mov r0, r10
	bl Owner_RecalculateRatiosFar
	movs r3, #1
	mov r4, r11
	mov r9, r3
	ldrb r3, [r4, #8]
	cmp r3, #255
	beq .L_08100956
	b .L_08100a74
.L_08100956:
	movs r0, #1
	mov r8, r0
	movs r7, #3
	b .L_08100a74
.L_0810095e:
	bl Random16
	lsls r0, r0, #2
	lsrs r0, r0, #16
	cmp r0, #0
	bne .L_08100970
	movs r0, #1
	negs r0, r0
	b .L_0810097e
.L_08100970:
	movs r1, #1
	adds r2, r1, #0
	eors r2, r0
	negs r3, r2
	orrs r3, r2
	lsrs r0, r3, #31
	subs r0, r1, r0
.L_0810097e:
	movs r3, #252
	ldr r1, [sp, #16]
	lsls r3, r3, #6
	ldr r2, .L_08100b94
	adds r3, #255
	ands r3, r1
	adds r3, r3, r2
	cmp r3, #5
	bhi .L_08100a74
	ldr r2, .L_08100b98
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
.L_08100998:
	.4byte .L_081009b0
	.4byte .L_081009c0
	.4byte .L_081009ee
	.4byte .L_08100a06
	.4byte .L_081009d0
	.4byte .L_081009e0
.L_081009b0:
	ldrh r3, [r5, #16]
	adds r2, r6, r0
	adds r3, r3, r2
	strh r3, [r5, #16]
	movs r3, #1
	movs r7, #16
	mov r9, r3
	b .L_08100a74
.L_081009c0:
	ldrh r3, [r5, #18]
	adds r2, r6, r0
	adds r3, r3, r2
	movs r4, #1
	strh r3, [r5, #18]
	movs r7, #17
	mov r9, r4
	b .L_08100a74
.L_081009d0:
	ldrh r3, [r5, #28]
	adds r2, r6, r0
	adds r3, r3, r2
	movs r0, #1
	strh r3, [r5, #28]
	movs r7, #18
	mov r9, r0
	b .L_08100a74
.L_081009e0:
	ldrb r3, [r5, #30]
	movs r1, #1
	adds r3, r3, r6
	strb r3, [r5, #30]
	movs r7, #19
	mov r9, r1
	b .L_08100a74
.L_081009ee:
	ldrh r3, [r5, #24]
	adds r2, r6, r0
	adds r3, r3, r2
	strh r3, [r5, #24]
	movs r0, #3
	movs r1, #5
	bl UiText_DrawQuantity
	movs r2, #1
	movs r7, #20
	mov r9, r2
	b .L_08100a74
.L_08100a06:
	ldrh r3, [r5, #26]
	adds r2, r6, r0
	adds r3, r3, r2
	strh r3, [r5, #26]
	movs r0, #4
	movs r1, #5
	bl UiText_DrawQuantity
	movs r3, #1
	movs r7, #21
	mov r9, r3
	b .L_08100a74
.L_08100a1e:
	movs r4, #58
	ldrsh r3, [r5, r4]
	movs r4, #54
	ldrsh r2, [r5, r4]
	ldrh r1, [r5, #58]
	ldrh r0, [r5, #54]
	cmp r3, r2
	bne .L_08100a38
	mov r0, r8
	cmp r0, #0
	bne .L_08100a74
	movs r7, #7
	b .L_08100a74
.L_08100a38:
	adds r3, r1, r6
	strh r3, [r5, #58]
	lsls r3, r3, #16
	asrs r3, r3, #16
	cmp r3, r2
	ble .L_08100a54
	subs r3, r3, r2
	mov r1, r8
	subs r6, r6, r3
	strh r0, [r5, #58]
	cmp r1, #0
	bne .L_08100a5c
	movs r7, #5
	b .L_08100a5c
.L_08100a54:
	mov r2, r8
	cmp r2, #0
	bne .L_08100a5c
	movs r7, #6
.L_08100a5c:
	mov r0, r10
	bl Owner_RecalculateRatiosFar
	movs r3, #1
	mov r4, r11
	mov r9, r3
	ldrb r3, [r4, #8]
	cmp r3, #255
	bne .L_08100a74
	movs r0, #1
	mov r8, r0
	movs r7, #8
.L_08100a74:
	mov r1, r11
	ldrb r3, [r1, #3]
	subs r3, #1
	cmp r3, #56
	bls .L_08100a80
	b .L_08100ca6
.L_08100a80:
	ldr r2, .L_08100b9c
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
.L_08100a88:
	.4byte .L_08100b6c
	.4byte .L_08100bc2
	.4byte .L_08100c7e
	.4byte .L_08100ca6
	.4byte .L_08100c08
	.4byte .L_08100ca6
	.4byte .L_08100ca6
	.4byte .L_08100ca6
	.4byte .L_08100ca6
	.4byte .L_08100ca6
	.4byte .L_08100ca6
	.4byte .L_08100ca6
	.4byte .L_08100ca6
	.4byte .L_08100ca6
	.4byte .L_08100ca6
	.4byte .L_08100ca6
	.4byte .L_08100ca6
	.4byte .L_08100ca6
	.4byte .L_08100ca6
	.4byte .L_08100ca6
	.4byte .L_08100ca6
	.4byte .L_08100ca6
	.4byte .L_08100ca6
	.4byte .L_08100ca6
	.4byte .L_08100ca6
	.4byte .L_08100ca6
	.4byte .L_08100ca6
	.4byte .L_08100ca6
	.4byte .L_08100ca6
	.4byte .L_08100ca6
	.4byte .L_08100ca6
	.4byte .L_08100ca6
	.4byte .L_08100ca6
	.4byte .L_08100ca6
	.4byte .L_08100ca6
	.4byte .L_08100ca6
	.4byte .L_08100ca6
	.4byte .L_08100ca6
	.4byte .L_08100ca6
	.4byte .L_08100ca6
	.4byte .L_08100ca6
	.4byte .L_08100ca6
	.4byte .L_08100ca6
	.4byte .L_08100ca6
	.4byte .L_08100ca6
	.4byte .L_08100ca6
	.4byte .L_08100ca6
	.4byte .L_08100ca6
	.4byte .L_08100ca6
	.4byte .L_08100ca6
	.4byte .L_08100ca6
	.4byte .L_08100ca6
	.4byte .L_08100ca6
	.4byte .L_08100ca6
	.4byte .L_08100ca6
	.4byte .L_08100c32
	.4byte .L_08100c4c
.L_08100b6c:
	movs r4, #56
	ldrsh r2, [r5, r4]
	ldrh r3, [r5, #56]
	cmp r2, #0
	ble .L_08100b80
	movs r4, #52
	ldrsh r1, [r5, r4]
	ldrh r0, [r5, #52]
	cmp r2, r1
	bne .L_08100ba0
.L_08100b80:
	mov r0, r8
	cmp r0, #0
	beq .L_08100b88
	b .L_08100ca6
.L_08100b88:
	movs r7, #2
	b .L_08100ca6
.L_08100b8c:
	.4byte gPartyState
.L_08100b90:
	.4byte .L_08100898
.L_08100b94:
	.4byte 0xfffffefc
.L_08100b98:
	.4byte .L_08100998
.L_08100b9c:
	.4byte .L_08100a88
.L_08100ba0:
	adds r3, r3, r6
	strh r3, [r5, #56]
	lsls r3, r3, #16
	asrs r3, r3, #16
	cmp r3, r1
	ble .L_08100bb8
	mov r1, r8
	strh r0, [r5, #56]
	cmp r1, #0
	bne .L_08100bfc
	movs r7, #0
	b .L_08100bfc
.L_08100bb8:
	mov r2, r8
	cmp r2, #0
	bne .L_08100bfc
	movs r7, #1
	b .L_08100bfc
.L_08100bc2:
	movs r4, #58
	ldrsh r3, [r5, r4]
	movs r4, #54
	ldrsh r2, [r5, r4]
	ldrh r1, [r5, #58]
	ldrh r0, [r5, #54]
	cmp r3, r2
	bne .L_08100bdc
	mov r0, r8
	cmp r0, #0
	bne .L_08100ca6
	movs r7, #7
	b .L_08100ca6
.L_08100bdc:
	adds r3, r1, r6
	strh r3, [r5, #58]
	lsls r3, r3, #16
	asrs r3, r3, #16
	cmp r3, r2
	ble .L_08100bf4
	mov r1, r8
	strh r0, [r5, #58]
	cmp r1, #0
	bne .L_08100bfc
	movs r7, #5
	b .L_08100bfc
.L_08100bf4:
	mov r2, r8
	cmp r2, #0
	bne .L_08100bfc
	movs r7, #6
.L_08100bfc:
	mov r0, r10
	bl Owner_RecalculateRatiosFar
	movs r3, #1
	mov r9, r3
	b .L_08100ca6
.L_08100c08:
	movs r4, #56
	ldrsh r3, [r5, r4]
	cmp r3, #0
	bne .L_08100c28
	ldrh r3, [r5, #52]
	mov r0, r10
	strh r3, [r5, #56]
	bl Owner_RecalculateRatiosFar
	movs r0, #1
	mov r1, r8
	mov r9, r0
	cmp r1, #0
	bne .L_08100ca6
	movs r7, #12
	b .L_08100ca6
.L_08100c28:
	mov r2, r8
	cmp r2, #0
	bne .L_08100ca6
	movs r7, #13
	b .L_08100ca6
.L_08100c32:
	movs r4, #56
	ldrsh r3, [r5, r4]
	cmp r3, #0
	bne .L_08100c74
	ldrh r3, [r5, #52]
	mov r0, r10
	lsls r3, r3, #16
	asrs r2, r3, #16
	lsrs r3, r3, #31
	adds r2, r2, r3
	asrs r2, r2, #1
	strh r2, [r5, #56]
	b .L_08100c66
.L_08100c4c:
	movs r2, #56
	ldrsh r3, [r5, r2]
	cmp r3, #0
	bne .L_08100c74
	movs r4, #52
	ldrsh r3, [r5, r4]
	movs r1, #10
	lsls r0, r3, #3
	subs r0, r0, r3
	bl __divsi3
	strh r0, [r5, #56]
	mov r0, r10
.L_08100c66:
	bl Owner_RecalculateRatiosFar
	mov r0, r8
	cmp r0, #0
	bne .L_08100ca6
	movs r7, #12
	b .L_08100ca6
.L_08100c74:
	mov r1, r8
	cmp r1, #0
	bne .L_08100ca6
	movs r7, #13
	b .L_08100ca6
.L_08100c7e:
	movs r3, #50
	adds r3, #255
	adds r2, r5, r3
	movs r3, #0
	ldrsb r3, [r2, r3]
	cmp r3, #0
	beq .L_08100c9e
	movs r3, #0
	movs r4, #1
	mov r0, r8
	strb r3, [r2]
	mov r9, r4
	cmp r0, #0
	bne .L_08100ca6
	movs r7, #10
	b .L_08100ca6
.L_08100c9e:
	mov r1, r8
	cmp r1, #0
	bne .L_08100ca6
	movs r7, #11
.L_08100ca6:
	mov r2, r11
	ldrb r3, [r2, #8]
	adds r2, r3, #0
	adds r3, r2, #0
	cmp r3, #255
	bne .L_08100cd0
	ldr r3, [sp, #0]
	ldr r4, [sp, #4]
	adds r3, #1
	lsls r3, r3, #24
	lsrs r3, r3, #24
	movs r0, #139
	str r3, [sp, #0]
	lsls r0, r0, #1
	adds r0, #255
	adds r3, r4, r0
	ldrb r3, [r3]
	ldr r1, [sp, #0]
	cmp r1, r3
	bcs .L_08100cd0
	b .L_0810085e
.L_08100cd0:
	mov r2, r9
	cmp r2, #0
	bne .L_08100ce8
	ldr r4, [sp, #4]
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #94
	adds r3, r4, r0
	movs r0, #1
	strh r7, [r3]
	negs r0, r0
	b .L_08100d30
.L_08100ce8:
	movs r1, #0
	ldr r2, [sp, #4]
	movs r4, #139
	str r1, [sp, #0]
	lsls r4, r4, #1
	adds r4, #255
	adds r3, r2, r4
	ldrb r3, [r3]
	cmp r1, r3
	bcs .L_08100d22
	adds r5, r2, r4
.L_08100cfe:
	ldr r0, [sp, #0]
	ldr r2, [sp, #4]
	movs r1, #129
	lsls r3, r0, #1
	lsls r1, r1, #2
	adds r3, r3, r1
	ldrh r0, [r2, r3]
	bl BattleUnit_Recalculate
	ldr r3, [sp, #0]
	adds r3, #1
	lsls r3, r3, #24
	lsrs r3, r3, #24
	str r3, [sp, #0]
	ldr r4, [sp, #0]
	ldrb r3, [r5]
	cmp r4, r3
	bcc .L_08100cfe
.L_08100d22:
	ldr r0, [sp, #4]
	movs r1, #128
	lsls r1, r1, #2
	adds r1, #94
	adds r3, r0, r1
	strh r7, [r3]
	movs r0, #0
.L_08100d30:
	add sp, #20
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
