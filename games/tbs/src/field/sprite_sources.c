/// @file
/// The packed resource that holds each animation number's pictures
/// (struct SpritePictureCacheSource), which the sprite picture cache decodes.

#include "types.h"
#include "field_internal.h"

/// Each row: {resource, animation number}; number 0 ends the table. A
/// resource's number cannot be a symbol in a halfword, so each row names its
/// resource in a comment.
// clang-format off
struct SpritePictureCacheSource gSpritePictureCache_Sources[] = {
    {242, 400}, // ResourceId_AxeGoblinSprites
    {243, 401}, // ResourceId_GhoulSprites
    {244, 402}, // ResourceId_HarpySprites
    {245, 403}, // ResourceId_TrollSprites
    {246, 404}, // ResourceId_WitchSprites
    {247, 408}, // ResourceId_GriffinSprites
    {248, 409}, // ResourceId_ShieldKnightSprites
    {249, 410}, // ResourceId_OrcSprites
    {250, 411}, // ResourceId_ChimeraSprites
    {251, 415}, // ResourceId_LivingArmorSprites
    {252, 417}, // ResourceId_ManticoreSprites
    {253, 421}, // ResourceId_OrcLordSprites
    {254, 424}, // ResourceId_GargoyleSprites
    {255, 429}, // ResourceId_LizardManSprites
    {256, 430}, // ResourceId_OgreSprites
    {257, 433}, // ResourceId_TailedImpSprites
    {258, 434}, // ResourceId_WispGhostSprites
    {259, 435}, // ResourceId_SalamanderSprites
    {260, 436}, // ResourceId_AquaBeastSprites
    {261, 448}, // ResourceId_HoundSprites
    {262, 450}, // ResourceId_BearSprites
    {263, 451}, // ResourceId_SirenSprites
    {264, 453}, // ResourceId_BlueWispSprites
    {265, 454}, // ResourceId_MushroomSprites
    {266, 455}, // ResourceId_RedWormSprites
    {267, 456}, // ResourceId_FairySprites
    {268, 457}, // ResourceId_BatSprites
    {269, 458}, // ResourceId_SmallLizardSprites
    {270, 459}, // ResourceId_WalkingPlantSprites
    {271, 460}, // ResourceId_FlySprites
    {272, 461}, // ResourceId_CrabSprites
    {273, 462}, // ResourceId_SmallBirdSprites
    {274, 463}, // ResourceId_MothSprites
    {275, 464}, // ResourceId_FrogSprites
    {276, 465}, // ResourceId_ManedLionSprites
    {277, 466}, // ResourceId_SerpentSprites
    {278, 467}, // ResourceId_WingedDragonSprites
    {279, 468}, // ResourceId_BlueMageSprites
    {280, 469}, // ResourceId_SkeletonSprites
    {281, 470}, // ResourceId_FlameSkullSprites
    {282, 471}, // ResourceId_FireDragonSprites
    {283, 472}, // ResourceId_CrawlerSprites
    {284, 473}, // ResourceId_MimicSprites
    {285, 474}, // ResourceId_SeaBeastSprites
    {286, 475}, // ResourceId_SpiritHorseSprites
    {287, 476}, // ResourceId_KrakenSprites
    {288, 477}, // ResourceId_SlimeSprites
    {289, 478}, // ResourceId_SaturosSprites
    {290, 479}, // ResourceId_MenardiSprites
    {291, 481}, // ResourceId_GhostHeadSprites
    {292, 483}, // ResourceId_FusionDragonSprites
    {293, 484}, // ResourceId_PurpleBeastSprites
    {294, 485}, // ResourceId_RedWarriorSprites
    {295, 486}, // ResourceId_SwordGirlSprites
    {0, 0},
};
