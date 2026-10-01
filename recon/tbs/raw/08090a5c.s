.syntax unified
	.thumb
	.global BattleFx_BuildBuffer
	.thumb_func
BattleFx_BuildBuffer:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	mov r10, r1
	mov r8, r2
	movs r1, #224
	adds r2, r3, #0
	movs r3, #128
	sub sp, #40
	lsls r1, r1, #1
	lsls r3, r3, #8
	str r1, [sp, #36]
	cmp r0, r3
	bne .L_08090a86
	movs r3, #160
	lsls r3, r3, #19
	ldrh r0, [r3]
.L_08090a86:
	cmp r2, #1
	bne .L_08090a90
	movs r1, #224
	str r1, [sp, #36]
	b .L_08090aa4
.L_08090a90:
	cmp r2, #2
	bne .L_08090aa4
	movs r3, #168
	lsls r3, r3, #3
	add r8, r3
	movs r1, #224
	movs r3, #224
	lsls r1, r1, #1
	str r3, [sp, #36]
	add r10, r1
.L_08090aa4:
	movs r1, #128
	lsls r1, r1, #8
	cmp r0, r1
	bcs .L_08090afe
	ldr r2, .L_08090ae8
	adds r3, r0, #0
	ands r3, r2
	mov r2, r8
	strh r3, [r2]
	movs r3, #2
	ldr r2, .L_08090aec
	add r8, r3
	adds r3, r0, #0
	ands r3, r2
	mov r1, r8
	lsls r3, r3, #5
	strh r3, [r1]
	ldr r3, .L_08090af0
	movs r2, #2
	add r8, r2
	ands r0, r3
	lsls r3, r0, #10
	mov r1, r8
	strh r3, [r1]
	ldr r3, [sp, #36]
	subs r3, #1
	add r8, r2
	lsls r2, r3, #1
	adds r2, r2, r3
	lsls r2, r2, #1
	movs r4, #128
	lsls r4, r4, #24
	b .L_08090af4
	.2byte 0x0000
.L_08090ae8:
	.4byte 0x00007c00
.L_08090aec:
	.4byte 0x000003e0
.L_08090af0:
	.4byte 0x0000001f
.L_08090af4:
	mov r0, r8
	lsrs r2, r2, #1
	ldr r3, .L_08090e48
	subs r0, #6
	b .L_08091156
.L_08090afe:
	movs r3, #128
	lsls r3, r3, #13
	cmp r0, r3
	bcc .L_08090b08
	b .L_08090f3e
.L_08090b08:
	ldr r1, .L_08090e4c
	adds r0, r0, r1
	cmp r0, #6
	bls .L_08090b12
	b .L_08090ef0
.L_08090b12:
	ldr r2, .L_08090e50
	lsls r3, r0, #2
	ldr r3, [r3, r2]
	mov pc, r3
	.2byte 0x0000
.L_08090b1c:
	.4byte .L_08090b38
	.4byte .L_08090b86
	.4byte .L_08090c22
	.4byte .L_08090cb4
	.4byte .L_08090d4e
	.4byte .L_08090dd0
	.4byte .L_08090e64
.L_08090b38:
	movs r2, #0
	ldr r3, [sp, #36]
	mov r9, r2
	cmp r9, r3
	bcc .L_08090b44
	b .L_0809115e
.L_08090b44:
	ldr r6, .L_08090e54
	mov r5, r8
.L_08090b48:
	mov r1, r10
	ldrh r4, [r1]
	movs r3, #248
	lsls r0, r4, #11
	lsls r3, r3, #8
	movs r2, #2
	ands r0, r3
	movs r3, #248
	lsls r3, r3, #9
	add r10, r2
	lsls r2, r4, #7
	ands r2, r3
	movs r3, #248
	lsls r3, r3, #7
	ands r3, r4
	adds r0, r0, r2
	adds r0, r0, r3
	movs r1, #7
	bl _call_via_r6
	adds r4, r0, #0
	strh r4, [r5]
	strh r4, [r5, #2]
	strh r4, [r5, #4]
	movs r3, #1
	ldr r1, [sp, #36]
	add r9, r3
	adds r5, #6
	cmp r9, r1
	bcc .L_08090b48
	b .L_0809115e
.L_08090b86:
	movs r2, #0
	ldr r3, [sp, #36]
	mov r9, r2
	cmp r9, r3
	bcc .L_08090b92
	b .L_0809115e
.L_08090b92:
	movs r1, #31
	ldr r2, .L_08090e58
	mov r11, r1
.L_08090b98:
	mov r3, r10
	ldrh r4, [r3]
	movs r1, #2
	adds r6, r4, #0
	mov r3, r11
	lsrs r0, r4, #5
	ands r6, r3
	ands r0, r3
	add r10, r1
	lsrs r3, r4, #10
	mov r1, r11
	ands r3, r1
	adds r0, r6, r0
	adds r0, r0, r3
	str r2, [sp, #0]
	ldr r3, .L_08090e54
	movs r1, #10
	bl _call_via_r3
	adds r4, r0, #0
	lsls r3, r4, #2
	adds r6, r3, #5
	lsls r3, r4, #1
	adds r3, r3, r4
	adds r5, r3, #5
	adds r7, r5, #0
	ldr r2, [sp, #0]
	cmp r6, #7
	bgt .L_08090bd4
	movs r6, #8
.L_08090bd4:
	cmp r5, #7
	bgt .L_08090be0
	movs r7, #8
	cmp r5, #7
	bgt .L_08090be0
	movs r5, #8
.L_08090be0:
	cmp r6, #28
	ble .L_08090be6
	movs r6, #28
.L_08090be6:
	cmp r7, #28
	ble .L_08090bec
	movs r7, #28
.L_08090bec:
	cmp r5, #28
	ble .L_08090bf2
	movs r5, #28
.L_08090bf2:
	lsls r3, r5, #1
	ldrh r3, [r2, r3]
	mov r1, r8
	strh r3, [r1]
	movs r3, #2
	add r8, r3
	lsls r3, r7, #1
	ldrh r3, [r2, r3]
	mov r1, r8
	strh r3, [r1]
	movs r3, #2
	add r8, r3
	lsls r3, r6, #1
	ldrh r3, [r2, r3]
	mov r1, r8
	strh r3, [r1]
	movs r3, #2
	add r8, r3
	movs r1, #1
	ldr r3, [sp, #36]
	add r9, r1
	cmp r9, r3
	bcc .L_08090b98
	b .L_0809115e
.L_08090c22:
	movs r1, #0
	ldr r2, [sp, #36]
	mov r9, r1
	cmp r9, r2
	bcc .L_08090c2e
	b .L_0809115e
.L_08090c2e:
	movs r3, #31
	mov r11, r3
.L_08090c32:
	mov r1, r10
	ldrh r4, [r1]
	mov r3, r11
	adds r6, r4, #0
	ands r6, r3
	lsrs r7, r4, #5
	lsrs r5, r4, #10
	ands r7, r3
	ands r5, r3
	adds r3, r6, #0
	orrs r3, r7
	movs r2, #2
	orrs r3, r5
	add r10, r2
	cmp r3, #0
	beq .L_08090c7e
	lsrs r3, r6, #1
	movs r1, #3
	adds r0, r7, #0
	subs r6, r6, r3
	bl __divsi3
	adds r6, #10
	subs r7, r7, r0
	adds r0, r6, #0
	bl BattleFx_ClampRgb555Channel
	adds r7, #8
	adds r6, r0, #0
	adds r0, r7, #0
	bl BattleFx_ClampRgb555Channel
	subs r5, #7
	adds r7, r0, #0
	adds r0, r5, #0
	bl BattleFx_ClampRgb555Channel
	adds r5, r0, #0
.L_08090c7e:
	ldr r2, .L_08090e5c
	lsls r3, r5, #1
	ldrh r3, [r2, r3]
	mov r1, r8
	movs r2, #2
	strh r3, [r1]
	add r8, r2
	ldr r2, .L_08090e58
	lsls r3, r7, #1
	ldrh r3, [r2, r3]
	mov r1, r8
	movs r2, #2
	strh r3, [r1]
	add r8, r2
	ldr r2, .L_08090e60
	lsls r3, r6, #1
	ldrh r3, [r2, r3]
	mov r1, r8
	strh r3, [r1]
	ldr r1, [sp, #36]
	movs r3, #1
	movs r2, #2
	add r9, r3
	add r8, r2
	cmp r9, r1
	bcc .L_08090c32
	b .L_0809115e
.L_08090cb4:
	movs r2, #0
	ldr r3, [sp, #36]
	mov r9, r2
	cmp r9, r3
	bcc .L_08090cc0
	b .L_0809115e
.L_08090cc0:
	ldr r1, .L_08090e60
	mov r11, r1
.L_08090cc4:
	mov r2, r10
	ldrh r4, [r2]
	movs r1, #31
	adds r6, r4, #0
	movs r3, #2
	lsrs r7, r4, #5
	lsrs r5, r4, #10
	ands r6, r1
	add r10, r3
	ands r7, r1
	ands r5, r1
	cmp r6, #9
	bgt .L_08090ce0
	movs r6, #10
.L_08090ce0:
	cmp r7, #15
	bgt .L_08090ce6
	movs r7, #16
.L_08090ce6:
	cmp r5, #15
	bgt .L_08090cec
	movs r5, #16
.L_08090cec:
	cmp r6, #28
	ble .L_08090cf2
	movs r6, #28
.L_08090cf2:
	cmp r7, #24
	ble .L_08090cf8
	movs r7, #24
.L_08090cf8:
	cmp r5, #26
	ble .L_08090cfe
	movs r5, #26
.L_08090cfe:
	adds r0, r6, #0
	bl BattleFx_ClampRgb555Channel
	adds r7, #2
	adds r6, r0, #0
	adds r0, r7, #0
	bl BattleFx_ClampRgb555Channel
	adds r5, #2
	adds r7, r0, #0
	adds r0, r5, #0
	bl BattleFx_ClampRgb555Channel
	adds r5, r0, #0
	mov r2, r11
	lsls r3, r5, #1
	ldrh r3, [r2, r3]
	mov r1, r8
	strh r3, [r1]
	movs r2, #2
	mov r1, r11
	lsls r3, r7, #1
	ldrh r3, [r1, r3]
	add r8, r2
	mov r2, r8
	strh r3, [r2]
	movs r3, #2
	add r8, r3
	lsls r3, r6, #1
	ldrh r3, [r1, r3]
	mov r1, r8
	strh r3, [r1]
	ldr r1, [sp, #36]
	movs r3, #1
	movs r2, #2
	add r9, r3
	add r8, r2
	cmp r9, r1
	bcc .L_08090cc4
	b .L_0809115e
.L_08090d4e:
	movs r2, #0
	ldr r3, [sp, #36]
	mov r9, r2
	cmp r9, r3
	bcc .L_08090d5a
	b .L_0809115e
.L_08090d5a:
	ldr r1, .L_08090e5c
	mov r11, r1
.L_08090d5e:
	mov r2, r10
	ldrh r4, [r2]
	movs r1, #31
	adds r6, r4, #0
	lsrs r7, r4, #5
	lsrs r5, r4, #10
	ands r6, r1
	ands r7, r1
	ands r5, r1
	adds r0, r6, r7
	movs r1, #3
	movs r3, #2
	adds r0, r0, r5
	add r10, r3
	bl __divsi3
	bl BattleFx_ClampRgb555Channel
	asrs r3, r6, #1
	adds r6, r3, r0
	asrs r3, r7, #1
	adds r7, r3, r0
	asrs r3, r5, #1
	adds r5, r3, r0
	adds r0, r6, #0
	bl BattleFx_ClampRgb555Channel
	adds r6, r0, #0
	adds r0, r7, #0
	bl BattleFx_ClampRgb555Channel
	adds r7, r0, #0
	adds r0, r5, #0
	bl BattleFx_ClampRgb555Channel
	adds r5, r0, #0
	mov r2, r11
	lsls r3, r5, #1
	ldrh r3, [r2, r3]
	mov r1, r8
	strh r3, [r1]
	lsls r3, r7, #1
	ldrh r3, [r2, r3]
	mov r2, r8
	strh r3, [r2, #2]
	mov r1, r11
	lsls r3, r6, #1
	ldrh r3, [r1, r3]
	strh r3, [r2, #4]
	movs r1, #1
	ldr r2, [sp, #36]
	movs r3, #6
	add r9, r1
	add r8, r3
	cmp r9, r2
	bcc .L_08090d5e
	b .L_0809115e
.L_08090dd0:
	movs r3, #0
	ldr r1, [sp, #36]
	mov r9, r3
	cmp r9, r1
	bcc .L_08090ddc
	b .L_0809115e
.L_08090ddc:
	movs r2, #31
	mov r11, r2
.L_08090de0:
	mov r3, r10
	ldrh r4, [r3]
	mov r2, r11
	lsrs r7, r4, #5
	lsrs r5, r4, #10
	adds r6, r4, #0
	ands r7, r2
	ands r5, r2
	ands r6, r2
	asrs r3, r7, #3
	asrs r2, r5, #3
	adds r3, r3, r2
	adds r6, r6, r3
	movs r1, #2
	adds r0, r6, #0
	add r10, r1
	bl BattleFx_ClampRgb555Channel
	movs r1, #3
	adds r6, r0, #0
	adds r0, r7, #0
	bl __divsi3
	movs r1, #3
	subs r7, r7, r0
	adds r0, r5, #0
	bl __divsi3
	ldr r1, .L_08090e60
	subs r5, r5, r0
	lsls r3, r5, #1
	ldrh r3, [r1, r3]
	mov r2, r8
	strh r3, [r2]
	lsls r3, r7, #1
	ldrh r3, [r1, r3]
	mov r1, r8
	strh r3, [r1, #2]
	ldr r2, .L_08090e58
	lsls r3, r6, #1
	ldrh r3, [r2, r3]
	mov r2, r8
	strh r3, [r2, #4]
	movs r1, #1
	ldr r2, [sp, #36]
	movs r3, #6
	add r9, r1
	add r8, r3
	cmp r9, r2
	bcc .L_08090de0
	b .L_0809115e
	.2byte 0x0000
.L_08090e48:
	.4byte 0x040000d4
.L_08090e4c:
	.4byte 0xfffeffff
.L_08090e50:
	.4byte .L_08090b1c
.L_08090e54:
	.4byte IwramSignedDivide
.L_08090e58:
	.4byte Data_0809e96e
.L_08090e5c:
	.4byte Data_0809e9ae
.L_08090e60:
	.4byte Data_0809e92e
.L_08090e64:
	movs r3, #0
	ldr r1, [sp, #36]
	mov r9, r3
	cmp r9, r1
	bcc .L_08090e70
	b .L_0809115e
.L_08090e70:
	movs r2, #31
	mov r11, r2
.L_08090e74:
	mov r3, r10
	ldrh r4, [r3]
	mov r2, r11
	adds r6, r4, #0
	lsrs r7, r4, #5
	ands r6, r2
	ands r7, r2
	movs r1, #2
	lsrs r3, r6, #1
	lsrs r5, r4, #10
	adds r0, r7, #0
	add r10, r1
	movs r1, #3
	ands r5, r2
	subs r6, r6, r3
	bl __divsi3
	adds r6, #6
	subs r7, r7, r0
	adds r0, r6, #0
	bl BattleFx_ClampRgb555Channel
	adds r7, #4
	adds r6, r0, #0
	adds r0, r7, #0
	bl BattleFx_ClampRgb555Channel
	subs r5, #6
	adds r7, r0, #0
	adds r0, r5, #0
	bl BattleFx_ClampRgb555Channel
	ldr r2, .L_08090ee4
	adds r5, r0, #0
	lsls r3, r5, #1
	ldrh r3, [r2, r3]
	mov r1, r8
	strh r3, [r1]
	ldr r2, .L_08090ee8
	lsls r3, r7, #1
	ldrh r3, [r2, r3]
	mov r2, r8
	strh r3, [r2, #2]
	ldr r2, .L_08090eec
	lsls r3, r6, #1
	ldrh r3, [r2, r3]
	strh r3, [r1, #4]
	ldr r1, [sp, #36]
	movs r3, #1
	movs r2, #6
	add r9, r3
	add r8, r2
	cmp r9, r1
	bcc .L_08090e74
	b .L_0809115e
	.2byte 0x0000
.L_08090ee4:
	.4byte Data_0809e9ae
.L_08090ee8:
	.4byte Data_0809e96e
.L_08090eec:
	.4byte Data_0809e92e
.L_08090ef0:
	movs r2, #0
	ldr r3, [sp, #36]
	mov r9, r2
	cmp r9, r3
	bcc .L_08090efc
	b .L_0809115e
.L_08090efc:
	ldr r5, .L_08090f08
	ldr r0, .L_08090f0c
	ldr r2, .L_08090f10
	mov r1, r8
	b .L_08090f14
	.2byte 0x0000
.L_08090f08:
	.4byte 0x00007c00
.L_08090f0c:
	.4byte 0x000003e0
.L_08090f10:
	.4byte 0x0000001f
.L_08090f14:
	mov r3, r10
	ldrh r4, [r3]
	movs r3, #2
	add r10, r3
	adds r3, r4, #0
	ands r3, r5
	strh r3, [r1]
	adds r3, r4, #0
	ands r3, r0
	lsls r3, r3, #5
	ands r4, r2
	strh r3, [r1, #2]
	lsls r3, r4, #10
	strh r3, [r1, #4]
	movs r3, #1
	add r9, r3
	ldr r3, [sp, #36]
	adds r1, #6
	cmp r9, r3
	bcc .L_08090f14
	b .L_0809115e
.L_08090f3e:
	movs r3, #128
	lsls r3, r3, #14
	ands r3, r0
	cmp r3, #0
	beq .L_08090fde
	movs r3, #31
	str r0, [sp, #32]
	adds r1, r0, #0
	lsrs r2, r0, #5
	lsrs r0, r0, #10
	ands r1, r3
	mov r11, r0
	ands r2, r3
	str r1, [sp, #32]
	mov r1, r11
	ands r1, r3
	str r2, [sp, #28]
	ldr r3, [sp, #36]
	movs r2, #0
	mov r9, r2
	mov r11, r1
	cmp r9, r3
	bcc .L_08090f6e
	b .L_0809115e
.L_08090f6e:
	mov r1, r10
	ldrh r4, [r1]
	movs r3, #248
	lsls r0, r4, #11
	lsls r3, r3, #8
	movs r2, #2
	ands r0, r3
	movs r3, #248
	lsls r3, r3, #9
	add r10, r2
	lsls r2, r4, #7
	ands r2, r3
	movs r3, #248
	lsls r3, r3, #7
	ands r3, r4
	adds r0, r0, r2
	adds r0, r0, r3
	movs r1, #96
	ldr r3, .L_080910dc
	bl _call_via_r3
	ldr r1, [sp, #32]
	adds r4, r0, #0
	adds r7, r1, #0
	muls r7, r4
	ldr r2, [sp, #28]
	adds r0, r7, #0
	adds r6, r2, #0
	muls r6, r4
	mov r5, r11
	muls r5, r4
	bl BattleFx_ClampRgb555Component
	adds r7, r0, #0
	adds r0, r6, #0
	bl BattleFx_ClampRgb555Component
	adds r6, r0, #0
	adds r0, r5, #0
	bl BattleFx_ClampRgb555Component
	mov r3, r8
	mov r1, r8
	mov r2, r8
	adds r5, r0, #0
	strh r5, [r3]
	strh r6, [r1, #2]
	strh r7, [r2, #4]
	movs r1, #1
	ldr r2, [sp, #36]
	movs r3, #6
	add r9, r1
	add r8, r3
	cmp r9, r2
	bcc .L_08090f6e
	b .L_0809115e
.L_08090fde:
	movs r3, #128
	lsls r3, r3, #15
	ands r3, r0
	cmp r3, #0
	bne .L_08090fea
	b .L_080910e8
.L_08090fea:
	movs r3, #31
	str r0, [sp, #24]
	adds r1, r0, #0
	lsrs r2, r0, #5
	lsrs r0, r0, #10
	ands r1, r3
	mov r11, r0
	ands r2, r3
	str r1, [sp, #24]
	mov r1, r11
	ands r1, r3
	str r2, [sp, #20]
	ldr r3, [sp, #36]
	movs r2, #0
	mov r9, r2
	mov r11, r1
	cmp r9, r3
	bcc .L_08091010
	b .L_0809115e
.L_08091010:
	ldr r2, [sp, #20]
	ldr r1, [sp, #24]
	ldr r3, [sp, #24]
	adds r1, r1, r2
	str r1, [sp, #16]
	lsls r1, r2, #16
	mov r2, r11
	lsls r3, r3, #16
	lsls r2, r2, #16
	str r3, [sp, #12]
	str r1, [sp, #8]
	str r2, [sp, #4]
.L_08091028:
	mov r3, r10
	ldrh r4, [r3]
	movs r2, #31
	adds r6, r4, #0
	lsrs r0, r4, #5
	ands r6, r2
	ands r0, r2
	lsrs r3, r4, #10
	movs r1, #2
	ands r3, r2
	add r10, r1
	adds r0, r6, r0
	ldr r1, [sp, #16]
	adds r0, r0, r3
	add r1, r11
	ldr r3, .L_080910dc
	lsls r0, r0, #4
	bl _call_via_r3
	ldr r3, [sp, #24]
	adds r4, r0, #0
	adds r0, r3, #0
	muls r0, r4
	ldr r2, [sp, #12]
	lsrs r0, r0, #4
	lsls r0, r0, #16
	asrs r1, r2, #4
	ldr r3, .L_080910e0
	mov r12, pc
	bx r3
	ldr r1, [sp, #20]
	adds r7, r0, #0
	adds r0, r1, #0
	muls r0, r4
	ldr r2, [sp, #8]
	lsrs r0, r0, #4
	lsls r0, r0, #16
	asrs r1, r2, #4
	mov r12, pc
	bx r3
	adds r6, r0, #0
	mov r0, r11
	muls r0, r4
	ldr r3, [sp, #4]
	lsrs r0, r0, #4
	asrs r1, r3, #4
	lsls r0, r0, #16
	ldr r3, .L_080910e0
	mov r12, pc
	bx r3
	adds r5, r0, #0
	lsrs r0, r7, #16
	bl BattleFx_ClampRgb555Channel
	adds r7, r0, #0
	lsrs r0, r6, #16
	bl BattleFx_ClampRgb555Channel
	adds r6, r0, #0
	lsrs r0, r5, #16
	bl BattleFx_ClampRgb555Channel
	ldr r1, .L_080910e4
	adds r5, r0, #0
	lsls r3, r5, #1
	ldrh r3, [r1, r3]
	mov r2, r8
	strh r3, [r2]
	movs r3, #2
	add r8, r3
	lsls r3, r6, #1
	ldrh r3, [r1, r3]
	mov r1, r8
	strh r3, [r1]
	ldr r1, .L_080910e4
	movs r2, #2
	lsls r3, r7, #1
	ldrh r3, [r1, r3]
	add r8, r2
	mov r2, r8
	strh r3, [r2]
	movs r1, #1
	ldr r2, [sp, #36]
	movs r3, #2
	add r9, r1
	add r8, r3
	cmp r9, r2
	bcc .L_08091028
	b .L_0809115e
	.2byte 0x0000
.L_080910dc:
	.4byte IwramSignedDivide
.L_080910e0:
	.4byte IwramMulQ16ReturnIp
.L_080910e4:
	.4byte Data_0809e92e
.L_080910e8:
	movs r3, #128
	lsls r3, r3, #16
	ands r3, r0
	cmp r3, #0
	beq .L_0809113e
	movs r3, #0
	ldr r1, [sp, #36]
	mov r9, r3
	cmp r9, r1
	bcs .L_0809115e
	ldr r5, .L_08091108
	ldr r0, .L_0809110c
	ldr r2, .L_08091110
	mov r1, r8
	b .L_08091114
	.2byte 0x0000
.L_08091108:
	.4byte 0x00007c00
.L_0809110c:
	.4byte 0x000003e0
.L_08091110:
	.4byte 0x0000001f
.L_08091114:
	mov r3, r10
	ldrh r4, [r3]
	movs r3, #2
	add r10, r3
	adds r3, r4, #0
	ands r3, r5
	strh r3, [r1]
	adds r3, r4, #0
	ands r3, r0
	lsls r3, r3, #5
	ands r4, r2
	strh r3, [r1, #2]
	lsls r3, r4, #10
	strh r3, [r1, #4]
	movs r3, #1
	add r9, r3
	ldr r3, [sp, #36]
	adds r1, #6
	cmp r9, r3
	bcc .L_08091114
	b .L_0809115e
.L_0809113e:
	cmp r2, #2
	bne .L_08091148
	movs r1, #168
	lsls r1, r1, #3
	adds r0, r0, r1
.L_08091148:
	ldr r3, [sp, #36]
	lsls r2, r3, #1
	adds r2, r2, r3
	movs r4, #132
	lsls r4, r4, #24
	lsrs r2, r2, #1
	ldr r3, .L_08091170
.L_08091156:
	mov r1, r8
	orrs r2, r4
	stmia r3!, {r0, r1, r2}
	subs r3, #12
.L_0809115e:
	add sp, #40
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
.L_08091170:
	.4byte 0x040000d4
