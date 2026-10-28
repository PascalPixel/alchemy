#include "objcmd.h"
#include "types.h"

//--------------------------------------------------------------------------
// Field object scripts: the ones the field starts by itself, the command
// table and the work value handlers SC_SET, SC_ADD and SC_CMP call.
//--------------------------------------------------------------------------

/// A command handler takes the object and returns nonzero to go on with
/// the script this frame, zero to stop. The handlers take different views
/// of the object, so the type leaves the parameters unspecified.
typedef s32 (*ScriptCommandFn)();

struct ScriptOperands;
typedef void (*OperandFunc)(struct ScriptOperands *state, s32 operation,
                            s32 value);

s32 Script_Wait();
s32 Script_WaitForArrival();
s32 Func_08024fe0();
s32 Func_08025038();
s32 Func_08025078();
s32 Script_ApplyRelativePosition();
s32 Script_StoreAngleToWatchedObject();
s32 Script_ApplyWatchedObjectPosition();
s32 Script_ApplyLocalOffsetPosition();
s32 Func_08025698();
s32 Func_08025830();
s32 Object_ApproachWatchedObject();
s32 Func_08025160();
s32 Script_RepeatOrJump();
s32 Script_JumpToLabel();
u32 Script_JumpIfTrue();
u32 Script_JumpIfFalse();
s32 Script_End();
s32 Script_TestFlag();
s32 Script_SetFlagAndTest();
s32 Script_ClearFlagAndTest();
s32 Script_ToggleFlag();
s32 Script_ApplyOperandSet();
s32 Script_ApplyOperandAdd();
s32 Script_ApplyOperandCompare();
s32 Func_08027064();
s32 Func_080279b0();
s32 Object_MoveByKeys();
s32 Func_080292fc();
s32 Func_08029570();
s32 Func_080298c0();
s32 Func_08027e20();
s32 Func_08028534();
s32 Func_08028aac();
s32 Func_0802a124();
s32 Func_0802632c();
s32 Func_080267bc();
s32 Func_08027e0c();
s32 ObjectDispatch_ReleaseAndClear();
s32 Script_SetAnimation();
s32 Script_Nop();
s32 Script_PlaySound();
s32 Script_CallWithOperand();
s32 Script_JumpToNextOperandPointer();
s32 Script_ClearByte54();
s32 Script_SetByte54();
s32 Script_InvokeCallback();
s32 Func_08025428();
s32 Func_0802509c();

void Script_SetOrCompareAddress(struct ScriptOperands *state, s32 operation,
                                s32 value);
void Script_SetOrCompareCursor(struct ScriptOperands *state, s32 operation,
                               s32 value);
void Script_SetOrCompareUnsignedHalfword(struct ScriptOperands *state,
                                         s32 operation, s32 value);
void Script_SetOrCompareX(struct ScriptOperands *state, s32 operation,
                          s32 value);
void Script_SetOrCompareY(struct ScriptOperands *state, s32 operation,
                          s32 value);
void Script_SetOrCompareZ(struct ScriptOperands *state, s32 operation,
                          s32 value);
void Script_SetOrCompareRadius(struct ScriptOperands *state, s32 operation,
                               s32 value);
void Script_SetOrCompareScaleX(struct ScriptOperands *state, s32 operation,
                               s32 value);
void Script_SetOrCompareScaleY(struct ScriptOperands *state, s32 operation,
                               s32 value);
void Script_SetOrCompareVelocityX(struct ScriptOperands *state, s32 operation,
                                  s32 value);
void Script_SetOrCompareVelocityY(struct ScriptOperands *state, s32 operation,
                                  s32 value);
void Script_SetOrCompareVelocityZ(struct ScriptOperands *state, s32 operation,
                                  s32 value);
void Script_SetOrCompareSpeedLimit(struct ScriptOperands *state, s32 operation,
                                   s32 value);
void Script_SetOrCompareAcceleration(struct ScriptOperands *state,
                                     s32 operation, s32 value);
void Script_SetOrCompareTargetX(struct ScriptOperands *state, s32 operation,
                                s32 value);
void Script_SetOrCompareTargetY(struct ScriptOperands *state, s32 operation,
                                s32 value);
void Script_SetOrCompareTargetZ(struct ScriptOperands *state, s32 operation,
                                s32 value);
void Script_SetOrCompareVerticalMotion(struct ScriptOperands *state,
                                       s32 operation, s32 value);
void Script_SetOrCompareGravity(struct ScriptOperands *state, s32 operation,
                                s32 value);
void Script_SetOrCompareTerrainHeight(struct ScriptOperands *state,
                                      s32 operation, s32 value);
void Script_SetOrCompareWord4c(struct ScriptOperands *state, s32 operation,
                               s32 value);
void Script_SetOrCompareAddress50(struct ScriptOperands *state, s32 operation,
                                  s32 value);
void Script_SetOrCompareByte54(struct ScriptOperands *state, s32 operation,
                               s32 value);
void Script_SetOrCompareMotionFlags(struct ScriptOperands *state, s32 operation,
                                    s32 value);
void Script_SetOrCompareArrivalAxis(struct ScriptOperands *state, s32 operation,
                                    s32 value);
void Script_SetOrCompareComparisonResult(struct ScriptOperands *state,
                                         s32 operation, s32 value);
void Script_SetOrCompareSnapToTarget(struct ScriptOperands *state,
                                     s32 operation, s32 value);
void Script_SetOrCompareCollisionFlags(struct ScriptOperands *state,
                                       s32 operation, s32 value);
void Script_SetOrCompareSteeringFlags(struct ScriptOperands *state,
                                      s32 operation, s32 value);
void Script_SetOrComparePaused(struct ScriptOperands *state, s32 operation,
                               s32 value);
void Script_SetOrCompareRepeatCount(struct ScriptOperands *state, s32 operation,
                                    s32 value);
void Script_SetOrCompareHalfword5e(struct ScriptOperands *state, s32 operation,
                                   s32 value);
void Script_SetOrCompareSignedHalfword64(struct ScriptOperands *state,
                                         s32 operation, s32 value);
void Script_SetOrCompareSignedHalfword66(struct ScriptOperands *state,
                                         s32 operation, s32 value);
void Script_SetOrCompareWord68(struct ScriptOperands *state, s32 operation,
                               s32 value);
void Script_SetOrCompareUpdateAddress(struct ScriptOperands *state,
                                      s32 operation, s32 value);
void Script_SetOrCompareByte62(struct ScriptOperands *state, s32 operation,
                               s32 value);
void Script_SetOrCompareByte63(struct ScriptOperands *state, s32 operation,
                               s32 value);

/// Each runs one control command every frame: the command, SC_WAIT(1),
/// SC_LOOP(SC_FOREVER, SC_TOP).
const s32 gData_0802f098[] = {
    SC_CMD_25,
    SC_WAIT(1),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

const s32 gData_0802f0b0[] = {
    SC_CMD_1B,
    SC_WAIT(1),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

const s32 gData_0802f0c8[] = {
    SC_CMD_1C,
    SC_WAIT(1),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

const s32 gData_0802f0e0[] = {
    SC_CMD_1D,
    SC_WAIT(1),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

const s32 gData_0802f0f8[] = {
    SC_CMD_1E,
    SC_WAIT(1),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

const s32 gData_0802f110[] = {
    SC_CMD_23,
    SC_WAIT(1),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

const s32 gData_0802f128[] = {
    SC_CMD_24,
    SC_WAIT(1),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

const s32 gData_0802f140[] = {
    SC_PLAYER,
    SC_WAIT(1),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

const s32 gData_0802f158[] = {
    SC_CMD_1A,
    SC_WAIT(1),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

const s32 gData_0802f170[] = {
    SC_CMD_1F,
    SC_WAIT(1),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

const s32 gData_0802f188[] = {
    SC_CMD_20,
    SC_WAIT(1),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

const s32 gData_0802f1a0[] = {
    SC_CMD_21,
    SC_WAIT(1),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

const s32 gData_0802f1b8[] = {
    SC_CMD_22,
    SC_WAIT(1),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

const s32 gObjectDispatch_CameraScript[] = {
    SC_CAMERA,
    SC_WAIT(1),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

const s32 gData_0802f1e8[] = {
    SC_APPROACH_WATCHED,
    SC_WAIT(1),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

const s32 gObjectDispatch_EndScript[] = {
    SC_END,
};

/// The handler of each command. 0x31 to 0x35 repeat SC_END's.
const ScriptCommandFn gData_0802f204[54] = {
    Script_Wait,            // 0x00 SC_WAIT
    Script_WaitForArrival,                 // 0x01 SC_SYNC
    Func_08024fe0,                       // 0x02 SC_POSITION
    Func_08025038,                       // 0x03 SC_CMD_03
    Func_08025078,                       // 0x04 SC_MOVE
    Script_ApplyRelativePosition,        // 0x05 SC_MOVE_BY
    Script_StoreAngleToWatchedObject,     // 0x06 SC_FACE_WATCHED
    Script_ApplyWatchedObjectPosition,    // 0x07 SC_MOVE_TO_WATCHED
    Script_ApplyLocalOffsetPosition,     // 0x08 SC_MOVE_LOCAL
    Func_08025698,                       // 0x09 SC_WANDER
    Func_08025830,                       // 0x0a SC_WANDER_HOME
    Object_ApproachWatchedObject,         // 0x0b SC_APPROACH_WATCHED
    Func_08025160,                       // 0x0c SC_CAMERA
    Script_RepeatOrJump,                 // 0x0d SC_LOOP
    Script_JumpToLabel,                  // 0x0e SC_JUMP
    (ScriptCommandFn)Script_JumpIfTrue,  // 0x0f SC_IF
    (ScriptCommandFn)Script_JumpIfFalse, // 0x10 SC_IFNOT
    Script_End,               // 0x11 SC_END
    Script_TestFlag,                     // 0x12 SC_FLAG_CHECK
    Script_SetFlagAndTest,               // 0x13 SC_FLAG_ON
    Script_ClearFlagAndTest,             // 0x14 SC_FLAG_OFF
    Script_ToggleFlag,                   // 0x15 SC_FLAG_REV
    Script_ApplyOperandSet,              // 0x16 SC_SET
    Script_ApplyOperandAdd,              // 0x17 SC_ADD
    Script_ApplyOperandCompare,          // 0x18 SC_CMP
    Func_08027064,                       // 0x19 SC_PLAYER
    Func_080279b0,                       // 0x1a SC_CMD_1A
    Object_MoveByKeys,                   // 0x1b SC_KEYS
    Func_080292fc,                       // 0x1c SC_CMD_1C
    Func_08029570,                       // 0x1d SC_CMD_1D
    Func_080298c0,                       // 0x1e SC_CMD_1E
    Func_08027e20,                       // 0x1f SC_CMD_1F
    Func_08028534,                       // 0x20 SC_CMD_20
    Func_08028aac,                       // 0x21 SC_CMD_21
    Func_0802a124,                       // 0x22 SC_CMD_22
    Func_0802632c,                       // 0x23 SC_CMD_23
    Func_080267bc,                       // 0x24 SC_CMD_24
    Func_08027e0c,                       // 0x25 SC_CMD_25
    ObjectDispatch_ReleaseAndClear,      // 0x26 SC_FREE
    Script_SetAnimation,      // 0x27 SC_ARG
    Script_Nop,                  // 0x28 SC_NOP
    Script_PlaySound,                 // 0x29 SC_SE
    Script_CallWithOperand,              // 0x2a SC_CMD_2A
    Script_JumpToNextOperandPointer,     // 0x2b SC_GOTO
    Script_ClearByte54,                  // 0x2c SC_B54_OFF
    Script_SetByte54,                    // 0x2d SC_B54_ON
    Script_InvokeCallback,               // 0x2e SC_CALL
    Func_08025428,                       // 0x2f SC_CMD_2F
    Func_0802509c,                       // 0x30 SC_MOVE_XZ
    Script_End,               // 0x31
    Script_End,               // 0x32
    Script_End,               // 0x33
    Script_End,               // 0x34
    Script_End,               // 0x35
};

/// The handler of each work value (enum ScriptWork in objcmd.h).
const OperandFunc gScript_OperandHandlerTable[41] = {
    Script_SetOrCompareAddress,          // 0x00 SW_SCRIPT
    Script_SetOrCompareCursor,           // 0x01 SW_CURSOR
    Script_SetOrCompareUnsignedHalfword, // 0x02
    Script_SetOrCompareUnsignedHalfword, // 0x03 SW_FACING
    Script_SetOrCompareUnsignedHalfword, // 0x04
    Script_SetOrCompareX,                // 0x05 SW_X
    Script_SetOrCompareY,                // 0x06 SW_Y
    Script_SetOrCompareZ,                // 0x07 SW_Z
    Script_SetOrCompareRadius,           // 0x08 SW_RADIUS
    Script_SetOrCompareScaleX,           // 0x09 SW_SCALE_X
    Script_SetOrCompareScaleY,           // 0x0a SW_SCALE_Y
    Script_SetOrCompareScaleY,           // 0x0b
    Script_SetOrCompareVelocityX,        // 0x0c SW_VELOCITY_X
    Script_SetOrCompareVelocityY,        // 0x0d SW_VELOCITY_Y
    Script_SetOrCompareVelocityZ,        // 0x0e SW_VELOCITY_Z
    Script_SetOrCompareSpeedLimit,       // 0x0f SW_SPEED
    Script_SetOrCompareAcceleration,     // 0x10 SW_ACCEL
    Script_SetOrCompareTargetX,          // 0x11 SW_TARGET_X
    Script_SetOrCompareTargetY,          // 0x12 SW_TARGET_Y
    Script_SetOrCompareTargetZ,          // 0x13 SW_TARGET_Z
    Script_SetOrCompareVerticalMotion,   // 0x14 SW_BOUNCE
    Script_SetOrCompareGravity,          // 0x15 SW_GRAVITY
    Script_SetOrCompareTerrainHeight,    // 0x16 SW_GROUND
    Script_SetOrCompareWord4c,           // 0x17
    Script_SetOrCompareAddress50,        // 0x18
    Script_SetOrCompareByte54,           // 0x19
    Script_SetOrCompareMotionFlags,      // 0x1a SW_MOTION
    Script_SetOrCompareArrivalAxis,      // 0x1b SW_ARRIVAL
    Script_SetOrCompareComparisonResult, // 0x1c SW_RESULT
    Script_SetOrCompareSnapToTarget,     // 0x1d SW_SNAP
    Script_SetOrCompareCollisionFlags,   // 0x1e SW_COLLISION
    Script_SetOrCompareSteeringFlags,    // 0x1f SW_STEERING
    Script_SetOrComparePaused,           // 0x20 SW_PAUSED
    Script_SetOrCompareRepeatCount,      // 0x21 SW_REPEAT
    Script_SetOrCompareHalfword5e,       // 0x22 SW_WAIT
    Script_SetOrCompareSignedHalfword64, // 0x23
    Script_SetOrCompareSignedHalfword66, // 0x24
    Script_SetOrCompareWord68,           // 0x25
    Script_SetOrCompareUpdateAddress,    // 0x26 SW_HOOK
    Script_SetOrCompareByte62,           // 0x27
    Script_SetOrCompareByte63,           // 0x28 SW_BYTE_63
};
