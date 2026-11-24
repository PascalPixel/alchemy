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
    {515, 526}, // ResourceId_BlueDevilSprites
    {516, 527}, // ResourceId_PinkBruteSprites
    {517, 528}, // ResourceId_ClubTrollSprites
    {518, 529}, // ResourceId_JesterImpSprites
    {519, 530}, // ResourceId_GoldDragonSprites
    {520, 531}, // ResourceId_ApeSprites
    {521, 532}, // ResourceId_SwordImpSprites
    {522, 533}, // ResourceId_RedBeastSprites
    {523, 534}, // ResourceId_RedBeetleSprites
    {524, 535}, // ResourceId_PinkWormSprites
    {525, 536}, // ResourceId_BanditSprites
    {526, 537}, // ResourceId_SeaDragonSprites
    {527, 539}, // ResourceId_SkullMageSprites
    {528, 540}, // ResourceId_ManEaterSprites
    {529, 541}, // ResourceId_BlueCrabSprites
    {530, 542}, // ResourceId_OstrichSprites
    {531, 543}, // ResourceId_BlueWyvernSprites
    {532, 544}, // ResourceId_DarkWizardSprites
    {533, 545}, // ResourceId_WhiteKnightSprites
    {534, 546}, // ResourceId_GoldenBirdSprites
    {535, 547}, // ResourceId_DarkKnightSprites
    {536, 548}, // ResourceId_TreantSprites
    {537, 549}, // ResourceId_GreenOgreSprites
    {538, 550}, // ResourceId_PinkOgreSprites
    {539, 551}, // ResourceId_WerewolfSprites
    {540, 552}, // ResourceId_MummySprites
    {541, 553}, // ResourceId_RaptorSprites
    {542, 554}, // ResourceId_GreenKnightSprites
    {543, 555}, // ResourceId_WyvernSprites
    {544, 556}, // ResourceId_HydraSprites
    {545, 557}, // ResourceId_LarvaSprites
    {546, 558}, // ResourceId_JackalSprites
    {547, 559}, // ResourceId_UrchinSprites
    {548, 560}, // ResourceId_DemonWarriorSprites
    {549, 561}, // ResourceId_DragonLeftSprites
    {550, 562}, // ResourceId_DragonRightSprites
    {551, 563}, // ResourceId_SeaSerpentSprites
    {552, 564}, // ResourceId_SwordsmanSprites
    {553, 565}, // ResourceId_StorkSprites
    {554, 566}, // ResourceId_VultureSprites
    {555, 567}, // ResourceId_ToadSprites
    {556, 568}, // ResourceId_SwordMaidenSprites
    {557, 569}, // ResourceId_BeastLordSprites
    {558, 570}, // ResourceId_BrawlerSprites
    {559, 571}, // ResourceId_MiniKnightSprites
    {560, 572}, // ResourceId_ScorpionSprites
    {561, 573}, // ResourceId_MermanSprites
    {562, 574}, // ResourceId_YoungSwordsmanSprites
    {563, 575}, // ResourceId_GreenMageSprites
    {564, 576}, // ResourceId_BlueOrbSprites
    {565, 577}, // ResourceId_AxeWarriorSprites
    {566, 578}, // ResourceId_SpearWarriorSprites
    {567, 587}, // ResourceId_AxeGoblinSprites
    {568, 588}, // ResourceId_GhoulSprites
    {569, 589}, // ResourceId_HarpySprites
    {570, 590}, // ResourceId_TrollSprites
    {571, 591}, // ResourceId_WitchSprites
    {572, 592}, // ResourceId_GriffinSprites
    {573, 593}, // ResourceId_ShieldKnightSprites
    {574, 594}, // ResourceId_OrcSprites
    {575, 595}, // ResourceId_ChimeraSprites
    {576, 596}, // ResourceId_ManticoreSprites
    {577, 597}, // ResourceId_OrcLordSprites
    {578, 598}, // ResourceId_GargoyleSprites
    {579, 599}, // ResourceId_OgreSprites
    {580, 600}, // ResourceId_TailedImpSprites
    {581, 601}, // ResourceId_WispGhostSprites
    {582, 602}, // ResourceId_SalamanderSprites
    {583, 603}, // ResourceId_AquaBeastSprites
    {584, 604}, // ResourceId_HoundSprites
    {585, 605}, // ResourceId_BearSprites
    {586, 606}, // ResourceId_SirenSprites
    {587, 607}, // ResourceId_BlueWispSprites
    {588, 608}, // ResourceId_MushroomSprites
    {589, 609}, // ResourceId_RedWormSprites
    {590, 610}, // ResourceId_BatSprites
    {591, 611}, // ResourceId_JellySprites
    {592, 612}, // ResourceId_SmallLizardSprites
    {593, 613}, // ResourceId_WalkingPlantSprites
    {594, 614}, // ResourceId_FlySprites
    {595, 615}, // ResourceId_CrabSprites
    {596, 616}, // ResourceId_SmallBirdSprites
    {597, 617}, // ResourceId_MothSprites
    {598, 618}, // ResourceId_FrogSprites
    {599, 619}, // ResourceId_ManedLionSprites
    {600, 620}, // ResourceId_SerpentSprites
    {601, 621}, // ResourceId_WingedDragonSprites
    {602, 622}, // ResourceId_BlueMageSprites
    {603, 623}, // ResourceId_SkeletonSprites
    {604, 624}, // ResourceId_FlameSkullSprites
    {605, 625}, // ResourceId_FireDragonSprites
    {606, 626}, // ResourceId_CrawlerSprites
    {607, 627}, // ResourceId_MimicSprites
    {608, 628}, // ResourceId_SeaBeastSprites
    {609, 629}, // ResourceId_SpiritHorseSprites
    {610, 630}, // ResourceId_LizardManSprites
    {611, 631}, // ResourceId_KrakenSprites
    {612, 632}, // ResourceId_SlimeSprites
    {613, 633}, // ResourceId_SaturosSprites
    {614, 634}, // ResourceId_MenardiSprites
    {615, 636}, // ResourceId_GhostHeadSprites
    {616, 638}, // ResourceId_FusionDragonSprites
    {617, 639}, // ResourceId_PurpleBeastSprites
    {618, 640}, // ResourceId_RedWarriorSprites
    {619, 641}, // ResourceId_SwordGirlSprites
    {0, 0},
};
