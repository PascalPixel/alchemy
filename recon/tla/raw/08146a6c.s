.syntax unified
	.thumb
	.global Func_08146a6c
	.thumb_func
Func_08146a6c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #68
	str r0, [sp, #52]
	movs r5, #192
	lsls r5, r5, #18
	ldr r0, [r5, #92]
	str r0, [sp, #48]
	movs r0, #1
	ldr r1, [r5, #96]
	str r1, [sp, #44]
	ldr r2, [r5, #100]
	str r2, [sp, #24]
	ldr r3, [r5, #48]
	str r3, [sp, #20]
	bl Func_081435e0
	ldr r0, .L_08146de0
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_08146de4
	movs r2, #128
	lsls r0, r0, #19
	mov lr, r3
	.2byte 0xf800
	movs r2, #0
	ldr r1, [sp, #24]
	movs r3, #0
	ldr r0, .L_08146de8
	bl Func_08157cf4
	movs r1, #19
	movs r0, #104
	bl Func_081963ec
	ldr r5, [r5, #104]
	movs r0, #239
	str r5, [sp, #32]
	ldr r5, [sp, #48]
	movs r1, #238
	lsls r0, r0, #7
	lsls r1, r1, #7
	adds r2, r5, r0
	movs r3, #2
	adds r1, #132
	str r3, [r2]
	adds r2, r5, r1
	movs r3, #50
	movs r1, #200
	str r3, [r2]
	lsls r1, r1, #4
	ldr r0, .L_08146dec
	bl Func_080145a8
	ldr r2, [sp, #52]
	ldr r0, [r2, #8]
	bl Func_08118088 + 0x10
	ldr r0, [r0]
	ldr r5, [sp, #52]
	mov r8, r0
	movs r3, #36
	ldrsh r0, [r5, r3]
	bl Func_08118088 + 0x10
	ldr r7, [r0]
	movs r0, #0
	str r0, [sp, #28]
	ldr r6, [sp, #48]
.L_08146b04:
	mov r2, r8
	ldr r3, [r2, #8]
	ldr r1, [sp, #28]
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	str r3, [r6]
	mov r0, r8
	ldr r3, [r0, #12]
	lsls r5, r1, #3
	movs r1, #240
	lsls r1, r1, #15
	adds r3, r3, r1
	str r3, [r6, #4]
	ldr r3, [r0, #16]
	str r3, [r6, #8]
	bl Random16
	movs r3, #127
	ands r3, r0
	ldr r0, [r7, #8]
	subs r3, #64
	lsls r3, r3, #16
	adds r0, r0, r3
	ldr r3, [r6]
	movs r1, #12
	subs r0, r0, r3
	bl __divsi3
	str r0, [r6, #12]
	ldr r3, [r6, #4]
	ldr r0, [r7, #12]
	movs r2, #160
	lsls r2, r2, #13
	subs r0, r0, r3
	adds r0, r0, r2
	movs r1, #12
	bl __divsi3
	str r0, [r6, #16]
	ldr r3, [r6, #8]
	ldr r0, [r7, #16]
	movs r1, #12
	subs r0, r0, r3
	bl __divsi3
	str r0, [r6, #20]
	bl Random16
	movs r3, #15
	ands r3, r0
	adds r3, r3, r5
	str r3, [r6, #24]
	ldr r3, [sp, #28]
	adds r6, #28
	adds r3, #1
	str r3, [sp, #28]
	cmp r3, #8
	bne .L_08146b04
	movs r5, #0
	str r5, [sp, #40]
.L_08146b7e:
	movs r1, #170
	movs r2, #170
	lsls r1, r1, #8
	lsls r2, r2, #7
	ldr r0, [sp, #40]
	adds r1, #171
	adds r2, #85
	movs r3, #0
	bl Func_081496c8
	ldr r0, [sp, #40]
	cmp r0, #96
	bne .L_08146b9e
	movs r0, #134
	bl Func_08118088 + 0x60
.L_08146b9e:
	movs r1, #0
	str r1, [sp, #28]
	str r1, [sp, #16]
	ldr r7, [sp, #48]
.L_08146ba6:
	ldr r3, [r7, #24]
	ldr r2, [sp, #40]
	cmp r2, r3
	bge .L_08146bb0
	b .L_08146d7a
.L_08146bb0:
	bl Func_08014de4
	ldr r0, [sp, #20]
	adds r1, r0, #0
	adds r1, #12
	bl Func_080156e8
	add r5, sp, #56
	adds r0, r7, #0
	adds r1, r5, #0
	bl Func_0815e1ec
	ldr r3, [r5]
	asrs r3, r3, #1
	str r3, [r5]
	adds r3, #8
	cmp r3, #135
	bls .L_08146bd6
	b .L_08146cfe
.L_08146bd6:
	ldr r3, [r5, #4]
	cmp r3, #127
	ble .L_08146bde
	b .L_08146cfe
.L_08146bde:
	movs r0, #8
	negs r0, r0
	cmp r3, r0
	bge .L_08146be8
	b .L_08146cfe
.L_08146be8:
	ldr r1, [sp, #28]
	ldr r3, .L_08146df0
	lsls r2, r1, #2
	adds r2, r2, r1
	mov r10, r5
	mov r8, r3
	ldr r5, .L_08146df4
	lsls r3, r2, #3
	subs r3, r3, r2
	lsls r3, r3, #3
	movs r4, #0
	adds r1, r3, r5
.L_08146c00:
	movs r3, #200
	ldr r2, [sp, #40]
	ldr r0, [r7, #24]
	lsls r3, r3, #5
	adds r3, #154
	adds r6, r4, #0
	muls r6, r3
	subs r0, r2, r0
	lsls r0, r0, #11
	subs r0, r6, r0
	str r1, [sp, #12]
	str r4, [sp, #8]
	bl Trig_Sin
	ldr r4, [sp, #8]
	movs r5, #1
	ands r5, r4
	mov r2, r8
	ldrb r3, [r2, r5]
	ldr r1, [sp, #12]
	adds r2, r3, #0
	muls r2, r0
	mov r0, r10
	lsrs r3, r2, #31
	adds r2, r2, r3
	ldr r3, [r0]
	asrs r2, r2, #17
	adds r3, r3, r2
	str r3, [r1, #12]
	ldr r2, [sp, #40]
	ldr r3, [r7, #24]
	subs r3, r2, r3
	lsls r3, r3, #11
	subs r6, r6, r3
	adds r0, r6, #0
	bl Trig_Cos
	mov r2, r8
	ldrb r3, [r2, r5]
	mov r5, r10
	adds r2, r3, #0
	muls r2, r0
	ldr r4, [sp, #8]
	ldr r3, [r5, #4]
	ldr r1, [sp, #12]
	asrs r2, r2, #16
	subs r3, r3, r2
	adds r4, #1
	str r3, [r1, #16]
	adds r1, #28
	cmp r4, #10
	bne .L_08146c00
	ldr r0, [sp, #16]
	movs r4, #0
	mov r11, r0
.L_08146c6e:
	mov r1, r11
	adds r2, r4, r1
	lsls r3, r2, #3
	subs r3, r3, r2
	ldr r2, .L_08146df4
	adds r4, #1
	lsls r3, r3, #2
	mov r9, r4
	adds r2, r2, r3
	mov r0, r9
	movs r1, #10
	mov r10, r2
	bl Math_Mod
	add r0, r11
	lsls r3, r0, #3
	ldr r5, .L_08146df4
	subs r3, r3, r0
	lsls r3, r3, #2
	adds r5, r5, r3
	mov r8, r5
	movs r4, #0
.L_08146c9a:
	mov r0, r8
	mov r1, r10
	ldr r6, [r1, #12]
	ldr r3, [r0, #12]
	movs r1, #12
	subs r3, r3, r6
	adds r0, r4, #0
	muls r0, r3
	str r4, [sp, #8]
	bl __divsi3
	mov r2, r8
	adds r6, r6, r0
	mov r0, r10
	ldr r3, [r2, #16]
	ldr r5, [r0, #16]
	ldr r4, [sp, #8]
	subs r3, r3, r5
	adds r0, r4, #0
	muls r0, r3
	movs r1, #12
	bl __divsi3
	ldr r2, .L_08146df8
	movs r3, #4
	subs r3, #2
	ldrh r1, [r2, r3]
	ldr r3, [sp, #24]
	adds r5, r5, r0
	movs r0, #1
	adds r1, r3, r1
	subs r6, r6, r0
	subs r5, #2
	movs r2, #2
	movs r3, #4
	str r2, [sp, #0]
	str r3, [sp, #4]
	ldr r0, [sp, #44]
	adds r3, r5, #0
	adds r2, r6, #0
	ldr r5, [sp, #32]
	mov lr, r5
	.2byte 0xf800
	ldr r4, [sp, #8]
	adds r4, #1
	cmp r4, #12
	bne .L_08146c9a
	mov r4, r9
	cmp r4, #10
	bne .L_08146c6e
.L_08146cfe:
	ldr r3, [r7, #4]
	ldr r0, .L_08146dfc
	cmp r3, r0
	bgt .L_08146d62
	ldr r3, [r7, #16]
	movs r0, #134
	negs r3, r3
	str r3, [r7, #16]
	ldr r3, [r7, #12]
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	str r3, [r7, #12]
	ldr r3, [r7, #20]
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	str r3, [r7, #20]
	ldr r1, [sp, #48]
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #168
	adds r2, r1, r3
	movs r3, #4
	str r3, [r2]
	bl Audio_PlayCue
	ldr r5, [sp, #52]
	movs r4, #0
	ldr r3, [r5, #20]
	cmp r3, #0
	beq .L_08146d62
	movs r6, #8
	movs r5, #36
.L_08146d42:
	ldr r1, [sp, #52]
	adds r3, r4, #0
	ldrsh r0, [r5, r1]
	movs r1, #7
	movs r2, #5
	str r4, [sp, #8]
	str r6, [sp, #0]
	bl Func_0814cd48
	ldr r0, [sp, #52]
	ldr r4, [sp, #8]
	ldr r3, [r0, #20]
	adds r4, #1
	adds r5, #2
	cmp r4, r3
	bne .L_08146d42
.L_08146d62:
	ldr r3, [r7]
	ldr r2, [r7, #12]
	adds r3, r3, r2
	str r3, [r7]
	ldr r2, [r7, #16]
	ldr r3, [r7, #4]
	adds r3, r3, r2
	str r3, [r7, #4]
	ldr r2, [r7, #20]
	ldr r3, [r7, #8]
	adds r3, r3, r2
	str r3, [r7, #8]
.L_08146d7a:
	ldr r1, [sp, #16]
	ldr r2, [sp, #28]
	adds r1, #10
	adds r2, #1
	str r1, [sp, #16]
	adds r7, #28
	str r2, [sp, #28]
	cmp r2, #8
	beq .L_08146d8e
	b .L_08146ba6
.L_08146d8e:
	movs r0, #4
	movs r1, #4
	bl Func_08158ce0
	bl Func_081434f8
	movs r5, #240
	ldr r3, [sp, #48]
	lsls r5, r5, #7
	adds r5, #232
	adds r2, r3, r5
	movs r3, #1
	movs r0, #1
	str r3, [r2]
	bl WaitFrames
	ldr r0, [sp, #40]
	adds r0, #1
	str r0, [sp, #40]
	cmp r0, #128
	beq .L_08146dba
	b .L_08146b7e
.L_08146dba:
	ldr r0, .L_08146dec
	bl Func_08014644
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	bl Func_08143bb8
	add sp, #68
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_08146de0:
	.4byte 0x0000013a
.L_08146de4:
	.4byte IwramCopyWords
.L_08146de8:
	.4byte 0x00000134
.L_08146dec:
	.4byte Func_08143000
.L_08146df0:
	.4byte Data_0819793c
.L_08146df4:
	.4byte gMapCellBuffer
.L_08146df8:
	.4byte Data_08197410
.L_08146dfc:
	.4byte 0x001dffff
