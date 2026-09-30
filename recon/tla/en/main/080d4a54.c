/* Facing, scene tables and the villagers' first lines. */
#include "CALL.H"

extern u8 MsgHaidiaADifficultTimeThreeYears[];
extern u8 MsgHaidiaCheckedThePsynergyStone[];
extern u8 MsgHaidiaDidTheTravelersMeetThe[];
extern u8 MsgHaidiaMeditateOnMtAlephDaily[];
extern u8 MsgHaidiaPartyPpRestored[];

extern u8 MsgHaidiaArentWorriedCrossing[];
extern u8 MsgHaidiaBeholdPowerPsynergy[];
extern u8 MsgHaidiaShownNewAbility[];

extern u8 MsgHaidiaAnythingInterestingOnYourTrip[];
extern u8 MsgHaidiaCanIUsePsynergy[];
extern u8 MsgHaidiaIHaveSomePsynergyLeft[];
extern u8 MsgHaidiaTheStoneFellOnThe[];
extern u8 MsgHaidiaYouSawTheWiseOne[];

extern u8 MsgHaidiaSureHelp[];
extern u8 MsgHaidiaWentOffWay[];

extern u8 MsgHaidiaGoAidElders[];
extern u8 MsgHaidiaOnlyTwoSurvived[];
extern u8 MsgHaidiaThePsynergyStoneIsGone[];
extern u8 MsgHaidiaThisIsVale[];
extern u8 MsgHaidiaYouCameBackHome[];

/* The four actors' closing actions, where the overlay's data lies. */
extern u8 HaidiaIe_Actor23Actions[];
extern u8 HaidiaIe_Actor24Actions[];
extern u8 HaidiaIe_Actor25Actions[];
extern u8 HaidiaIe_LeaderActions[];

s32 Object_UpdateFacingTowardTarget(struct FacingObject *object)
{
    s32 facing_delta;
    u16 old_facing;
    s32 target_facing;
    struct FacingObject *target;

    target = object->facing_target;
    if (target != NULL) {
        object->facing_flags = (u8)(0xFE & object->facing_flags);
        target_facing = (u16)ArcTan2(target->position_z - object->position_z, target->position_x - object->position_x);
        old_facing = object->facing;
        facing_delta = (s16)(target_facing - old_facing);
        if (facing_delta != 0) {
            if (facing_delta > 0x1000) {
                facing_delta = 0x1000;
            }
            /* The loader relocates the stored pool word to -0x1000. */
            if (facing_delta < -0x1000) {
                facing_delta = -0x1000;
            }
            object->facing = (u16)(old_facing + facing_delta);
        }
    }
    return 1;
}
