#include "ENTRY_SETUP.H"

void SceneState_PassZeroAndMinusOneRecord(void)
{
    struct Args_02003fe4 args;

    args.first = 0;
    args.last = -1;
    VinasuHeya_StepActorWithDust(&args);
}

void SceneState_PassRange0To1(void)
{
    struct Args_02003ffc args;

    args.first = 0;
    args.last = 1;
    VinasuHeya_StepActorWithDust(&args);
}

void SceneState_PassRangeNeg1To0(void)
{
    struct Args_02004014 args;

    args.first = -1;
    args.last = 0;
    VinasuHeya_StepActorWithDust(&args);
}

void SceneState_CallHandlerWithFlagPair(void)
{
    struct Args_02004030 args;

    args.first = 1;
    args.last = 0;
    VinasuHeya_StepActorWithDust(&args);
}
