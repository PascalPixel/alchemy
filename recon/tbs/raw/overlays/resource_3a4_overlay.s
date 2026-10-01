.syntax unified
	.thumb
	.section .text.x02008d2c,"ax",%progbits
	.global Func_02000d2c
	.thumb_func
Func_02000d2c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r0, #10
	sub sp, #4
	bl Object_GetById
	ldr r3, [r0, #12]
	ldr r2, [r0, #8]
	ldr r6, [r0, #80]
	mov r9, r2
	str r3, [sp, #0]
	mov r10, r0
	bl Engine_EventBegin
	movs r0, #141
	bl Engine_AudioPlayCue
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #9
	lsls r2, r2, #9
	lsls r0, r0, #10
	bl Engine_WorkSetValuesIfNonNegative
	movs r0, #10
	bl Engine_EventWait
	ldr r0, .L_02008eb0
	bl Engine_AudioPlayCue
	movs r0, #1
	movs r1, #1
	ldr r2, .L_02008eb4
	negs r0, r0
	negs r1, r1
	bl Engine_WorkSetValuesIfNonNegative
	movs r0, #20
	bl Engine_EventWait
	ldr r2, .L_02008eb8
	movs r7, #0
	mov r8, r2
.L_02008d8e:
	movs r3, #128
	lsls r3, r3, #12
	ldrh r2, [r6, #30]
	adds r7, r7, r3
	lsrs r3, r7, #16
	adds r3, r3, r2
	strh r3, [r6, #30]
	movs r2, #128
	ldrh r0, [r6, #30]
	lsls r2, r2, #7
	adds r0, r0, r2
	bl Engine_MathCos
	adds r5, r0, #0
	lsls r3, r5, #4
	add r3, r9
	mov r2, r10
	str r3, [r2, #8]
	ldrh r1, [r6, #30]
	cmp r1, r8
	bhi .L_02008dc0
	movs r0, #1
	bl Engine_TaskWait
	b .L_02008d8e
.L_02008dc0:
	movs r3, #224
	lsls r3, r3, #7
	movs r7, #0
	mov r8, r3
.L_02008dc8:
	movs r2, #128
	lsls r2, r2, #12
	adds r7, r7, r2
	lsrs r3, r7, #16
	subs r3, r1, r3
	strh r3, [r6, #30]
	movs r3, #128
	ldrh r0, [r6, #30]
	lsls r3, r3, #7
	adds r0, r0, r3
	bl Engine_MathCos
	adds r5, r0, #0
	lsls r3, r5, #4
	add r3, r9
	mov r2, r10
	str r3, [r2, #8]
	ldrh r1, [r6, #30]
	cmp r1, r8
	bls .L_02008dfa
	movs r0, #1
	bl Engine_TaskWait
	ldrh r1, [r6, #30]
	b .L_02008dc8
.L_02008dfa:
	movs r3, #128
	movs r7, #128
	lsls r3, r3, #8
	lsls r7, r7, #12
	mov r11, r3
.L_02008e04:
	lsrs r2, r7, #19
	lsrs r3, r7, #16
	adds r3, r3, r2
	lsls r3, r3, #16
	adds r7, r3, #0
	lsrs r2, r7, #16
	adds r3, r2, r1
	strh r3, [r6, #30]
	movs r3, #128
	ldrh r0, [r6, #30]
	lsls r3, r3, #7
	adds r0, r0, r3
	mov r8, r2
	bl Engine_MathCos
	adds r5, r0, #0
	ldrh r0, [r6, #30]
	add r0, r11
	bl Engine_MathSin
	lsls r3, r5, #4
	add r3, r9
	mov r2, r10
	str r3, [r2, #8]
	ldrh r3, [r6, #30]
	cmp r3, r11
	bls .L_02008e44
	ldr r2, [sp, #0]
	lsls r3, r0, #3
	subs r3, r2, r3
	mov r2, r10
	str r3, [r2, #12]
.L_02008e44:
	ldrh r3, [r6, #30]
	ldr r2, .L_02008ebc
	add r3, r8
	cmp r3, r2
	bgt .L_02008e58
	movs r0, #1
	bl Engine_TaskWait
	ldrh r1, [r6, #30]
	b .L_02008e04
.L_02008e58:
	movs r0, #1
	bl Engine_TaskWait
	movs r3, #192
	lsls r3, r3, #8
	strh r3, [r6, #30]
	movs r0, #183
	bl Engine_AudioPlayCue
	movs r0, #192
	movs r1, #192
	movs r2, #128
	lsls r1, r1, #10
	lsls r2, r2, #9
	lsls r0, r0, #10
	bl Engine_WorkSetValuesIfNonNegative
	movs r0, #20
	bl Engine_EventWait
	ldr r0, .L_02008eb0
	bl Engine_AudioPlayCue
	movs r0, #1
	movs r1, #1
	negs r1, r1
	ldr r2, .L_02008eb4
	negs r0, r0
	bl Engine_WorkSetValuesIfNonNegative
	movs r0, #5
	bl FieldScene_RunSharedSetPiece
	bl Engine_EventEnd
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
.L_02008eb0:
	.4byte 0x00000121
.L_02008eb4:
	.4byte 0x0000e666
.L_02008eb8:
	.4byte 0x00008fff
.L_02008ebc:
	.4byte 0x0000bfff
	.section .rodata.x0200bd28,"a",%progbits
	.global ArutinYama_PaletteScript
ArutinYama_PaletteScript:
	.4byte 0x20021003
	.4byte 0x20024001
	.4byte 0x000000ff
	.global ArutinYama_ActorScript
ArutinYama_ActorScript:
	.4byte 0x00000022
	.4byte OverlayObject_UpdateFacingTowardTarget
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.global ArutinYama_EarlyActorScript
ArutinYama_EarlyActorScript:
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00000222
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00000222
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x0000006c
	.4byte 0x00000000
	.4byte 0x00000010
	.global ArutinYama_LogRideScript
ArutinYama_LogRideScript:
	.4byte 0x00000003
	.4byte 0x03120000
	.4byte 0x00000000
	.4byte 0x00dc0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x03120000
	.4byte 0x00000000
	.4byte 0x01460000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x03120000
	.4byte 0x00000000
	.4byte 0x01460000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x03120000
	.4byte 0x00000000
	.4byte 0x01e00000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x03120000
	.4byte 0x00000000
	.4byte 0x02080000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000023
	.4byte 0x00000001
	.4byte 0x00000010
	.global ArutinYama_LeaderRideScript
ArutinYama_LeaderRideScript:
	.4byte 0x0000001c
	.4byte 0x00000005
	.4byte 0x00000003
	.4byte 0x03240000
	.4byte 0x00000000
	.4byte 0x008c0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x03240000
	.4byte 0x00000000
	.4byte 0x00a60000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x02fc0000
	.4byte 0x00000000
	.4byte 0x00a60000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x02fc0000
	.4byte 0x00000000
	.4byte 0x00c60000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x03120000
	.4byte 0x00000000
	.4byte 0x00c60000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x03120000
	.4byte 0x00000000
	.4byte 0x00f60000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x02fc0000
	.4byte 0x00000000
	.4byte 0x00f60000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x02fc0000
	.4byte 0x00000000
	.4byte 0x011a0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x030c0000
	.4byte 0x00000000
	.4byte 0x012a0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x03240000
	.4byte 0x00000000
	.4byte 0x012a0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x03240000
	.4byte 0x00000000
	.4byte 0x01460000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x03080000
	.4byte 0x00000000
	.4byte 0x01460000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x03080000
	.4byte 0x00000000
	.4byte 0x01660000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x03240000
	.4byte 0x00000000
	.4byte 0x01660000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x03240000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x03120000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x02fa0000
	.4byte 0x00000000
	.4byte 0x01a00000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x02fa0000
	.4byte 0x00000000
	.4byte 0x01b60000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x03240000
	.4byte 0x00000000
	.4byte 0x01b60000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x03240000
	.4byte 0x00000000
	.4byte 0x01d80000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x031a0000
	.4byte 0x00000000
	.4byte 0x01d80000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x03180000
	.4byte 0x00000000
	.4byte 0x01f80000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x03280000
	.4byte 0x00000000
	.4byte 0x01f80000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x03280000
	.4byte 0x00000000
	.4byte 0x02280000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000023
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x03560000
	.4byte 0x00000000
	.4byte 0x02280000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x80010000
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000400
	.4byte 0x0000000c
	.4byte 0x00000018
	.4byte 0xc0010000
	.4byte 0x00000010
	.global ArutinYama_SparkScript
ArutinYama_SparkScript:
	.4byte 0x00000022
	.4byte OverlayObject_IntegrateAndDamp
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x0000003c
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000001b
	.global ArutinYama_GeraldScript
ArutinYama_GeraldScript:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x013a0000
	.4byte 0x00000000
	.4byte 0x007e0000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000010
	.global ArutinYama_IvanScript
ArutinYama_IvanScript:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x015a0000
	.4byte 0x00000000
	.4byte 0x007e0000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000010
	.global ArutinYama_MiaScript
ArutinYama_MiaScript:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x008a0000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000010
	.global ArutinYama_CelebrateScript
ArutinYama_CelebrateScript:
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00000080
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffffc00
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x0000003c
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000010
	.global ArutinYama_PartyScript
ArutinYama_PartyScript:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00760000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000010
	.global gArutinYamaEntrancesOther
gArutinYamaEntrancesOther:
	.4byte 0xffff0000
	.4byte 0x000000a0
	.4byte 0x400000a0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gArutinYamaEntrances1
gArutinYamaEntrances1:
	.4byte 0xffff0000
	.4byte 0x000000a0
	.4byte 0x400000a0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x000001c8
	.4byte 0xc0000208
	.4byte 0x00000000
	.4byte 0x01fa0038
	.4byte 0x00000212
	.4byte 0xffff0062
	.4byte 0x000001c8
	.4byte 0xc00001e8
	.4byte 0x00000000
	.4byte 0x01fa0038
	.4byte 0x00000212
	.4byte 0xffff0063
	.4byte 0x00000068
	.4byte 0x400000e8
	.4byte 0x00000000
	.4byte 0x01fa0038
	.4byte 0x00000212
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gArutinYamaEntrances2
gArutinYamaEntrances2:
	.4byte 0xffff0000
	.4byte 0x000000a0
	.4byte 0x400000a0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x000000f8
	.4byte 0xc00001d0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0002
	.4byte 0x00000038
	.4byte 0x40000048
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gArutinYamaEntrances3
gArutinYamaEntrances3:
	.4byte 0xffff0000
	.4byte 0x000000a0
	.4byte 0x400000a0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x000001e8
	.4byte 0xc0000230
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0002
	.4byte 0x00000298
	.4byte 0x4000017c
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0003
	.4byte 0x00000388
	.4byte 0xc00000d6
	.4byte 0x034a0000
	.4byte 0x03f80028
	.4byte 0x000000f8
	.4byte 0xffff0062
	.4byte 0x000001e8
	.4byte 0xc0000230
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0063
	.4byte 0x00000140
	.4byte 0x400001d8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gArutinYamaEntrances4
gArutinYamaEntrances4:
	.4byte 0xffff0000
	.4byte 0x000000a0
	.4byte 0x400000a0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x000000f8
	.4byte 0xc00001f2
	.4byte 0x00180000
	.4byte 0x02880000
	.4byte 0x00000230
	.4byte 0xffff0002
	.4byte 0x00000077
	.4byte 0x4000006c
	.4byte 0x00180000
	.4byte 0x02880000
	.4byte 0x00000230
	.4byte 0xffff0003
	.4byte 0x00000237
	.4byte 0x40000069
	.4byte 0x00180000
	.4byte 0x02880000
	.4byte 0x00000230
	.4byte 0xffff0004
	.4byte 0x00000377
	.4byte 0xc000031e
	.4byte 0x02c80000
	.4byte 0x03c00238
	.4byte 0x0000035c
	.4byte 0xffff0005
	.4byte 0x00000317
	.4byte 0x400002a7
	.4byte 0x02c80000
	.4byte 0x03c00230
	.4byte 0x0000035c
	.4byte 0xffff0006
	.4byte 0x00000058
	.4byte 0xc0000348
	.4byte 0x00200000
	.4byte 0x011002c0
	.4byte 0x00000360
	.4byte 0xffff0007
	.4byte 0x000000d8
	.4byte 0xc0000348
	.4byte 0x00200000
	.4byte 0x011002c0
	.4byte 0x00000360
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gArutinYamaEntrances5
gArutinYamaEntrances5:
	.4byte 0xffff0000
	.4byte 0x000000a0
	.4byte 0x400000a0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000249
	.4byte 0xc00001f5
	.4byte 0x00000000
	.4byte 0x031e0000
	.4byte 0x00000212
	.4byte 0xffff0062
	.4byte 0x00000249
	.4byte 0xc00001f5
	.4byte 0x00000000
	.4byte 0x031e0000
	.4byte 0x00000212
	.4byte 0xffff0063
	.4byte 0x000002eb
	.4byte 0x40000102
	.4byte 0x00000000
	.4byte 0x031e0000
	.4byte 0x00000212
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gArutinYamaEntrances6
gArutinYamaEntrances6:
	.4byte 0xffff0000
	.4byte 0x000000a0
	.4byte 0x400000a0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000238
	.4byte 0xc000015c
	.4byte 0x00000000
	.4byte 0x031e0000
	.4byte 0x00000212
	.4byte 0xffff0002
	.4byte 0x00000127
	.4byte 0x4000005f
	.4byte 0x00000000
	.4byte 0x031e0000
	.4byte 0x00000212
	.4byte 0xffff0003
	.4byte 0x000001ba
	.4byte 0xc0000306
	.4byte 0x00000000
	.4byte 0x02000212
	.4byte 0x000003b4
	.4byte 0xffff0004
	.4byte 0x00000048
	.4byte 0xc0000366
	.4byte 0x00000000
	.4byte 0x02000212
	.4byte 0x000003b4
	.4byte 0xffff0005
	.4byte 0x00000396
	.4byte 0x400002c8
	.4byte 0x02080000
	.4byte 0x03e0023a
	.4byte 0x00000312
	.4byte 0xffff0006
	.4byte 0x00000267
	.4byte 0x400002ac
	.4byte 0x02080000
	.4byte 0x03e0023a
	.4byte 0x00000312
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gArutinYamaEntrances7
gArutinYamaEntrances7:
	.4byte 0xffff0000
	.4byte 0x000000a0
	.4byte 0x400000a0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000048
	.4byte 0xc00001f6
	.4byte 0x00000000
	.4byte 0x03340000
	.4byte 0x00000238
	.4byte 0xffff0002
	.4byte 0x00000098
	.4byte 0x400000ae
	.4byte 0x00000000
	.4byte 0x03340000
	.4byte 0x00000238
	.4byte 0xffff0003
	.4byte 0x00000108
	.4byte 0xc000037a
	.4byte 0x00900000
	.4byte 0x018002d0
	.4byte 0x0000038c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gArutinYamaEntrances8
gArutinYamaEntrances8:
	.4byte 0xffff0000
	.4byte 0x000000a0
	.4byte 0x400000a0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000188
	.4byte 0x40000074
	.4byte 0x00000000
	.4byte 0x033e0000
	.4byte 0x00000234
	.4byte 0xffff0002
	.4byte 0x00000028
	.4byte 0xc0000074
	.4byte 0x00000000
	.4byte 0x033e0000
	.4byte 0x00000234
	.4byte 0xffff0003
	.4byte 0x00000208
	.4byte 0x40000268
	.4byte 0x00f40000
	.4byte 0x04000234
	.4byte 0x00000400
	.4byte 0xffff0004
	.4byte 0x00000198
	.4byte 0xc00003ca
	.4byte 0x00f40000
	.4byte 0x04000234
	.4byte 0x00000400
	.4byte 0xffff0005
	.4byte 0x000003b7
	.4byte 0x40000317
	.4byte 0x00f40000
	.4byte 0x04000234
	.4byte 0x00000400
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gArutinYamaEntrances9
gArutinYamaEntrances9:
	.4byte 0xffff0000
	.4byte 0x000000a0
	.4byte 0x400000a0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000169
	.4byte 0xc0000135
	.4byte 0x00000000
	.4byte 0x020c0000
	.4byte 0x00000330
	.4byte 0xffff0002
	.4byte 0x00000059
	.4byte 0xc00002ed
	.4byte 0x00000000
	.4byte 0x020c0000
	.4byte 0x00000330
	.4byte 0xffff0003
	.4byte 0x000002d8
	.4byte 0xc00002cd
	.4byte 0x026a0000
	.4byte 0x03de0000
	.4byte 0x00000356
	.4byte 0xffff0004
	.4byte 0x000003a7
	.4byte 0x400001ed
	.4byte 0x026a0000
	.4byte 0x03de0000
	.4byte 0x00000356
	.4byte 0xffff0005
	.4byte 0x00000317
	.4byte 0x40000236
	.4byte 0x026a0000
	.4byte 0x03de0000
	.4byte 0x00000356
	.4byte 0xffff0006
	.4byte 0x00100318
	.4byte 0x40000241
	.4byte 0x026a0000
	.4byte 0x03e60000
	.4byte 0x00000356
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gArutinYamaEntrances10
gArutinYamaEntrances10:
	.4byte 0xffff0000
	.4byte 0x000000a0
	.4byte 0x400000a0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00040058
	.4byte 0x400001a8
	.4byte 0x00000000
	.4byte 0x01c00000
	.4byte 0x0000020c
	.4byte 0xffff0062
	.4byte 0x00040058
	.4byte 0x400001a8
	.4byte 0x00000000
	.4byte 0x01c00000
	.4byte 0x0000020c
	.4byte 0xffff0063
	.4byte 0x00000148
	.4byte 0xc0000076
	.4byte 0x00000000
	.4byte 0x01c00000
	.4byte 0x0000020c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gArutinYamaEntrances11
gArutinYamaEntrances11:
	.4byte 0xffff0000
	.4byte 0x000000a0
	.4byte 0x400000a0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000198
	.4byte 0xc0000205
	.4byte 0x00000000
	.4byte 0x021c0000
	.4byte 0x0000024e
	.4byte 0xffff0002
	.4byte 0x000000c6
	.4byte 0x40000150
	.4byte 0x00000000
	.4byte 0x021c0000
	.4byte 0x0000024e
	.4byte 0xffff0003
	.4byte 0x00000238
	.4byte 0xc0000088
	.4byte 0x01e00000
	.4byte 0x02ac000f
	.4byte 0x000000b4
	.4byte 0xffff0004
	.4byte 0x00000049
	.4byte 0xc00000be
	.4byte 0x00000000
	.4byte 0x021c0000
	.4byte 0x0000024e
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gArutinYamaRegions9
gArutinYamaRegions9:
	/* Europe combines the two ninth-room boxes and widens the tenth. */
	.ifdef TBS_EDITION_ES
	.4byte 0xff8c0310
	.4byte 0x0320023c
	.4byte 0x024cff9c
	.4byte 0x0006ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gArutinYamaRegions10
gArutinYamaRegions10:
	.4byte 0x00220050
	.4byte 0x006001a0
	.4byte 0x01b00032
	.4byte 0x0001ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.else
	.ifdef TBS_EDITION_FR
	.4byte 0xff8c0310
	.4byte 0x0320023c
	.4byte 0x024cff9c
	.4byte 0x0006ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gArutinYamaRegions10
gArutinYamaRegions10:
	.4byte 0x00220050
	.4byte 0x006001a0
	.4byte 0x01b00032
	.4byte 0x0001ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.else
	.ifdef TBS_EDITION_IT
	.4byte 0xff8c0310
	.4byte 0x0320023c
	.4byte 0x024cff9c
	.4byte 0x0006ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gArutinYamaRegions10
gArutinYamaRegions10:
	.4byte 0x00220050
	.4byte 0x006001a0
	.4byte 0x01b00032
	.4byte 0x0001ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.else
	.4byte 0xff960316
	.4byte 0x031a023f
	.4byte 0x0243ff9a
	.4byte 0x0006ffff
	.4byte 0xff940314
	.4byte 0x031c0243
	.4byte 0x024bff9c
	.4byte 0x0006ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gArutinYamaRegions10
gArutinYamaRegions10:
	.4byte 0x00260054
	.4byte 0x005c01a4
	.4byte 0x01ac002e
	.4byte 0x0001ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.endif
	.endif
	.endif
	.global ArutinYama_StatueTable
ArutinYama_StatueTable:
	.4byte 0x0000004d
	.4byte 0x0010a04b
	.4byte 0x0000004e
	.4byte 0x00103050
	.4byte 0x0020104f
	.4byte 0x0000004f
	.4byte 0x0010204e
	.4byte 0x0020304f
	.4byte 0x0030204f
	.4byte 0x00000050
	.4byte 0x0010b04b
	.4byte 0x00204050
	.4byte 0x0030104e
	.4byte 0x00402050
	.4byte 0x00501052
	.4byte 0x0060404a
	.4byte 0x0070504a
	.4byte 0x00000051
	.4byte 0x00106052
	.4byte 0x00000052
	.4byte 0x00105050
	.4byte 0x00203052
	.4byte 0x00302052
	.4byte 0x00405052
	.4byte 0x00504052
	.4byte 0x00601051
	.4byte 0x00000053
	.4byte 0x00101054
	.4byte 0x00201055
	.4byte 0x0030d04b
	.4byte 0x00000054
	.4byte 0x00101053
	.4byte 0x00205055
	.4byte 0x00301057
	.4byte 0x0040c04b
	.4byte 0x00503055
	.4byte 0x00000055
	.4byte 0x00102053
	.4byte 0x00204055
	.4byte 0x00305054
	.4byte 0x00402055
	.4byte 0x00502054
	.4byte 0x00601056
	.4byte 0x00000056
	.4byte 0x00106055
	.4byte 0x00000057
	.4byte 0x00103054
	.4byte 0x00203057
	.4byte 0x00302057
	.4byte 0x00434002
	.4byte 0x000001ff
	.global gArutinYamaPlacementsOther
gArutinYamaPlacementsOther:
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gArutinYamaPlacements1
gArutinYamaPlacements1:
	.4byte 0xffff00e3
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00024000
	.4byte 0xffff00e3
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00024000
	.4byte 0xffff00be
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x01500000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gArutinYamaPlacements3
gArutinYamaPlacements3:
	.4byte 0xffff00e7
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00020000
	.4byte 0xffff00be
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x01d00000
	.4byte 0x00028000
	.4byte 0xffff0127
	.4byte 0x00000001
	.4byte 0x03780000
	.4byte 0x00000000
	.4byte 0x00ac0000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gArutinYamaPlacements5
gArutinYamaPlacements5:
	.4byte 0xffff00e7
	.4byte 0x00000001
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00020000
	.4byte 0xffff00e3
	.4byte 0x00000001
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00024000
	.4byte 0xffff00be
	.4byte 0x00000001
	.4byte 0x02f70000
	.4byte 0x00000000
	.4byte 0x011d0000
	.4byte 0x00028000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gArutinYamaPlacements6
gArutinYamaPlacements6:
	.4byte 0xffff00e7
	.4byte 0x00000001
	.4byte 0x02c80000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00020000
	.4byte 0xffff00e3
	.4byte 0x00000001
	.4byte 0x02b80000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gArutinYamaPlacements7
gArutinYamaPlacements7:
	.4byte 0xffff00e7
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00020000
	.4byte 0xffff00e3
	.4byte 0x00000001
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00024000
	.4byte 0xffff00d3
	.4byte 0x00000001
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00024000
	.4byte 0xffff00e3
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00024000
	.4byte 0xffff00e3
	.4byte 0x00000001
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00024000
	.4byte 0xffff0127
	.4byte 0x00000001
	.4byte 0x02080000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x00024000
	.4byte 0x0047005b
	.4byte 0x00000001
	.4byte 0x00380000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gArutinYamaPlacements8
gArutinYamaPlacements8:
	.4byte 0xffff0127
	.4byte 0x00000001
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x027c0000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gArutinYamaPlacements9
gArutinYamaPlacements9:
	.4byte 0xffff00c8
	.4byte 0x00000001
	.4byte 0x03100000
	.4byte 0x00000000
	.4byte 0x00500000
	.4byte 0x00024000
	.4byte 0xffff0120
	.4byte 0x00000007
	.4byte 0x03100000
	.4byte 0x00000000
	.4byte 0x00500000
	.4byte 0x00024000
	.4byte 0xffff00cd
	.4byte 0x00000007
	.4byte 0x03060000
	.4byte 0x00000000
	.4byte 0x004c0000
	.4byte 0x00024000
	.4byte 0xffff0016
	.4byte 0x00000007
	.4byte 0x03060000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00024000
	.4byte 0xffff0112
	.4byte 0x00000007
	.4byte 0x03180000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00024000
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gArutinYamaPlacements10
gArutinYamaPlacements10:
	.4byte 0xffff00c7
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00540000
	.4byte 0x00024000
	.4byte 0xffff0120
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00024000
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff0002
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff0003
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gArutinYamaPlacements11
gArutinYamaPlacements11:
	.4byte 0xffff0127
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00024000
	.4byte 0xffff00d3
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gArutinYamaEventsOther
gArutinYamaEventsOther:
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global gArutinYamaEvents1
gArutinYamaEvents1:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x0904000a
	.4byte FieldScene_BuildMultiPhasePresentation
	.4byte 0x00000002
	.4byte 0x0905000b
	.4byte ArutinYama_RunLeapSequence
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte SceneState_SetWorkByte22bTo3
	.4byte 0x00008d15
	.4byte 0xffff040a
	.4byte SceneState_SetWorkByte22bTo3
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global ArutinYama_OpenedAreaScript
ArutinYama_OpenedAreaScript:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00001815
	.4byte 0x02000008
	.4byte ActorPresentation_SetCellAndLowerActorEight
	.4byte 0x00001815
	.4byte 0x02010009
	.4byte SceneState_ApplyRectAndSetActor9Byte55
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global gArutinYamaEvents2
gArutinYamaEvents2:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global gArutinYamaEvents3
gArutinYamaEvents3:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte ArutinYama_BeginRollingRide
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte SceneState_SetByte22bTo3
	.4byte 0x00008d15
	.4byte 0xffff0409
	.4byte SceneState_SetByte22bTo3
	.4byte 0x00008f15
	.4byte 0xffff000a
	.4byte 0x00000000
	.4byte 0x00000003
	.4byte 0xffff0053
	.4byte FieldScene_RunValue1528Scene
	.4byte 0x00000013
	.4byte 0x0f760064
	.4byte 0x001000c0
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000013
	.4byte 0x0f050064
	.4byte 0x001000b4
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global gArutinYamaEvents4
gArutinYamaEvents4:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global gArutinYamaEvents5
gArutinYamaEvents5:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00001815
	.4byte 0x02000009
	.4byte SceneActor_RaiseSlot9StepB
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte SceneState_SetByte22bTo3AndSend51
	.4byte 0x00008d15
	.4byte 0xffff040a
	.4byte SceneState_SetByte22bTo3AndSend51
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte ArutinYama_BeginRollingRide
	.4byte 0x00000003
	.4byte 0xffff0032
	.4byte FieldScene_RunLine1528Sequence
	.4byte 0x00002413
	.4byte 0x0f770064
	.4byte 0x0010007b
	.4byte 0x00000413
	.4byte 0x0f770064
	.4byte 0x0010007b
	.4byte 0x0000e413
	.4byte 0x0f770064
	.4byte 0x0010007b
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global gArutinYamaEvents6
gArutinYamaEvents6:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00001815
	.4byte 0x02000009
	.4byte SceneState_ApplyRectAndLowerActor9
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte ArutinYama_BeginRollingRide
	.4byte 0x00000003
	.4byte 0xffff0032
	.4byte FieldScene_RunScene3a4SequenceC
	.4byte 0x00000013
	.4byte 0x0ef20064
	.4byte 0x00500002
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global gArutinYamaEvents7
gArutinYamaEvents7:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000013
	.4byte 0x0f070065
	.4byte 0x001000ba
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte SceneState_SetValue14Mode23
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte ArutinYama_BeginRollingRide
	.4byte 0x00000003
	.4byte 0xffff0032
	.4byte FieldScene_RunScene3a4SequenceD
	.4byte 0x00000002
	.4byte 0xffff000a
	.4byte SceneActor_SetActor10Byte23To3
	.4byte 0x00000002
	.4byte 0xffff000b
	.4byte SceneActor_SetActor10Byte23To1
	.4byte 0x00008c15
	.4byte 0xffff000a
	.4byte SceneActor_UpdateSlot10ByTileX
	.4byte 0x00000602
	.4byte 0xffff000c
	.4byte FieldScene_RunScene3a4_02000c9c
	.4byte 0x00001815
	.4byte 0x02000009
	.4byte SceneActor_RaiseSlot9StepA
	.4byte 0x00001815
	.4byte 0x0201000b
	.4byte SceneActor_RaiseSlot11AndSetFlag201
	.4byte 0x00001815
	.4byte 0x0204000c
	.4byte SceneActor_AdjustSlot12AndSetFlag204
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global gArutinYamaEvents8
gArutinYamaEvents8:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000013
	.4byte 0x0f780064
	.4byte 0x001000e5
	.4byte 0x00008f15
	.4byte 0xffff0008
	.4byte 0x00000000
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global gArutinYamaEvents9
gArutinYamaEvents9:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000003
	.4byte 0xffff000c
	.4byte FieldScene_RunFallingRocksWarning
	.4byte 0x00008e15
	.4byte 0x0908000b
	.4byte Func_02000d2c
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global gArutinYamaEvents10
gArutinYamaEvents10:
	.4byte 0x00000002
	.4byte 0x0909000a
	.4byte FieldScene_RunLateAuxiliarySequence
	.4byte 0x00000013
	.4byte 0x0f080064
	.4byte 0x001000cb
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global gArutinYamaEvents11
gArutinYamaEvents11:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00008f15
	.4byte 0xffff0008
	.4byte 0x00000000
	.4byte 0x00008c15
	.4byte 0xffff0009
	.4byte 0x00000000
	.4byte 0x00000013
	.4byte 0x0f790064
	.4byte 0x001000b6
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global ArutinYama_CueTicks
ArutinYama_CueTicks:
	.4byte 0x00000000
	.global ArutinYama_RollRadii
ArutinYama_RollRadii:
	.4byte 0x00010000
	.4byte 0x00010b55
	.4byte 0x0001172b
	.4byte 0x00012387
	.4byte 0x0001306f
	.4byte 0x00013dea
	.4byte 0x00014bfd
	.4byte 0x00015ab0
	.4byte 0x00016a09
	.4byte 0x00017a11
	.4byte 0x00018ace
	.4byte 0x00019c49
	.4byte 0x0001ae89
	.4byte 0x0001c199
	.4byte 0x0001d581
	.4byte 0x0001ea4b
	.4byte 0x00020000
	.4byte 0x000216ab
	.4byte 0x00022e56
	.4byte 0x0002470f
	.4byte 0x000260df
	.4byte 0x00027bd4
	.4byte 0x000297fb
	.4byte 0x0002b560
	.4byte 0x0002d413
	.4byte 0x0002f422
	.4byte 0x0003159c
	.4byte 0x00033892
	.4byte 0x00035d13
	.4byte 0x00038333
	.4byte 0x0003ab03
	.4byte 0x0003d495
	.4byte 0x00040000
	.section .bss,"aw",%nobits
	.global ArutinYama_RiseTimer
ArutinYama_RiseTimer:
	.space 4
	.global ArutinYama_LeafMode
ArutinYama_LeafMode:
	.space 4
	.global ArutinYama_LeafOrigin
ArutinYama_LeafOrigin:
	.space 12
	.space 16
	.global ArutinYama_PaletteHold
ArutinYama_PaletteHold:
	.space 4
	.global ArutinYama_PaletteStep
ArutinYama_PaletteStep:
	.space 4
