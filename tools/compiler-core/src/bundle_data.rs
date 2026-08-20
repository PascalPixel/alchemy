pub type ExecutableDigests = (&'static str, &'static [&'static str]);
pub type TargetExecutables = (&'static str, &'static [ExecutableDigests]);
pub type HostTargets = (&'static str, &'static [TargetExecutables]);
pub type HostDigests = (&'static str, &'static [&'static str]);

const GS1: &[ExecutableDigests] = &[
    (
        "xgcc",
        &["1c0b96c0193a3f547bf10c70078d87df10a62d34207da545653165b65c80d4c2"],
    ),
    (
        "cpp",
        &["c69d8c0423517be6a4962bd88e343c193a0842a164dd600550ceb36f8155d736"],
    ),
    (
        "tradcpp",
        &["f1e4340c3550029675b49e22976dd7d10588e365816ae0f2decd19729371ee00"],
    ),
    (
        "cc1",
        &["f7d1aeb098815f9b98c5df25bb47bab89aa14b1ab184b63c1472986f515fa3dd"],
    ),
];
const GS2: &[ExecutableDigests] = &[
    (
        "xgcc",
        &["128520f13ff01aee64a984b1279a6e3a682a3679de44c99296064f46fb1e8ec2"],
    ),
    (
        "cpp0",
        &["b4ac7f5ff7fd74f4eca40385832fd0360d13cb5d4f0b6c8b3ead4a67d2f3d5b0"],
    ),
    (
        "tradcpp0",
        &["7698319dfea3647dace68ffb5c3dbc0fd459f3a859699acb47c669d3eb8956a3"],
    ),
    (
        "cc1",
        &["91b2a67275a100e8b6695d85ef2d82d1fd144853cbcb361ddf1d8be31858230f"],
    ),
];
const EMPTY_GS1: &[ExecutableDigests] =
    &[("xgcc", &[]), ("cpp", &[]), ("tradcpp", &[]), ("cc1", &[])];
const EMPTY_GS2: &[ExecutableDigests] = &[
    ("xgcc", &[]),
    ("cpp0", &[]),
    ("tradcpp0", &[]),
    ("cc1", &[]),
];
const EMPTY: &[TargetExecutables] = &[("gs1", EMPTY_GS1), ("gs2", EMPTY_GS2)];

pub static EXPECTED: &[HostTargets] = &[
    ("darwin-arm64", &[("gs1", GS1), ("gs2", GS2)]),
    ("darwin-x64", EMPTY),
    ("linux-x64", EMPTY),
    ("linux-arm64", EMPTY),
];
pub static AGBCC_EXPECTED: &[HostDigests] = &[
    (
        "darwin-arm64",
        &["dfbbb5a8a0a9e14e7ed84a0a723eb6e68974836159a2c3e925b904058382e15a"],
    ),
    ("darwin-x64", &[]),
    ("linux-x64", &[]),
    ("linux-arm64", &[]),
];
pub static GCC3_EXPECTED: &[HostDigests] = &[
    ("darwin-arm64", &[]),
    ("darwin-x64", &[]),
    ("linux-x64", &[]),
    ("linux-arm64", &[]),
];
