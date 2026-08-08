// Approved bundle digests, mirrored from `tools/lib/alchemy_gcc.ts`.
//
// NOT generated: no tool writes this file, and the "@generated -- do not edit by
// hand" banner it used to carry was a false claim.
//
// These tables previously lived in `tools-rs/alchemy-routing/src/bundle_data.rs`
// carrying an honest note that they were hand-synced with NO gate, because
// `EXPECTED` is a three-level nested literal whose TypeScript keys are bare
// identifiers rather than strings, so the routing crate's flat string-sequence
// comparison did not apply to them. They now live in the crate that actually
// reads them, and the gate exists: `crate::drift` parses the object literals
// back out of `tools/lib/alchemy_gcc.ts` with a real object-literal parser and
// compares every host, target, executable name and digest element-wise and in
// order.
//
// A digest is only admissible from a green `bun run verify` on the bundle that
// produced it; see the Japanese note above `EXPECTED` in the TypeScript. The
// values are NOT path-reproducible -- the same gcc sources built at a different
// prefix give a byte-different `cc1` with identical codegen -- so a digest that
// does not match a locally rebuilt binary is not evidence of drift.

/// Approved digests for one executable in a bundle.
pub type ExecutableDigests = (&'static str, &'static [&'static str]);
/// One compiler target's executables.
pub type TargetExecutables = (&'static str, &'static [ExecutableDigests]);
/// One host's targets.
pub type HostTargets = (&'static str, &'static [TargetExecutables]);

pub static EXPECTED: &[HostTargets] = &[
    ("darwin-arm64", &[
        ("gs1", &[
            ("xgcc", &["87e09e3f1e2fd711e952d6831c73099b14a059a6ca594b16c11b9a83394483ed", "df0413f0051c07c654a753764235f39891d6f08a95d603a50f3cca9c645fc4e3", "f92badaf03dbcfc4a79f0c4da9ee5159186f48dd8328403cbf05734a1aa21f42"]),
            ("cpp", &["f72b13ad2368419f2cc8c24966e030a57638bfce3f97868043196dac41e13575", "c6e5093aa3cda856c10b8fdff5a7f645a6ca63c92d2aea46688f8da4f5357915", "96ef7e4d9e3932817c023712850e3a15f0eb5b33904215c63c4eda4c17b43b1a"]),
            ("tradcpp", &["822c5cf4b38ea231f6eeeadcdf3a457518a25202c8a0a04aadf0942154e5436b", "553a34add496b8a063707e32376824ba11cf0153b4b6283309c9a2518a866281", "ebc87e2f3bf595bd2014ee9f8a67d07a27cb83b4ba50e3b2ca62b1f91999e5d4"]),
            ("cc1", &["4a19cd86373f0c95f8d5e2f1d6b85867e4afcb2a166e39e1fecc9f7777533906", "7e66357ce5b69114713705da032c339a6aed02a1f7f90dd8ee330ae112313bed", "40ede14c48e7383c7b284450cd3c1357eb2d5dc014bf4949b69a2a8bc682b241", "c69829903ed05d68d46bc134de6cc554729fd59e1abca11715828db2a9a38365", "45d291c1ee530c2dc6ca5928e3186e4fc55234805a8b4b79c4b7d7977f7188cb", "6cee4484a765a41b8731e4983e7ae0751d3a21e516d63849f37c82313045f290", "802e08a756f582f6c10d467e747143486daa46d13f53c879c9cbd572a693d897", "31441a4d40d157050c7917c53a2567f1d3f9c93d6b2358d7168ba5554fe6ebaf", "41b5d62baf165b5119d3bfa569be831d0ebeb1ed1187857c51ced44394549e75", "47fdf35d6c41ed4b1879ab9ce4ce019d395350b411953bc619e316abad225955", "c74a9073698099d112e341db60e4e2ca85c0c189048c4fc5d1277d8d8f58923d", "610bedba4d9b133d0ff37fbd37c43e7ad1c0b066e6325a4677d9fd80d75f965e", "5f95a10d93349d67bad586c52362d0f6ede3e63c02c7bee87cc07203b228613d", "93419a9f5dca85c386efc04dcb26368b058191b73cc3ba808ef497fd458036dc", "fd1bd7c1c8cdbdb224569221c12744386bb0a75c86e1b22ff1e42aa392a9de07", "adc81071ec570ed11eae60e138bac9db3ff11ab63bef7f1a725bd198a4c70575", "3c8e99bb06fe15eaae8f04c83eec291a3b362d56649d79dec57a4884b064d7cf", "fa87afe4493a462fc2dbb11d0967b189ed040eeeb2b860422d3da5e38ab5d69c", "22a827bbcd8295b84148aa2e12270ead8d92fdd6af4582e2341afa21825014b7", "5a62f4a9686ac3492956d1d9e2e8da3039ff1d7d74961991d96ac366d7ccfba7", "0b7d6f6bd1490d49017a2438eb1bdbdd6c977b179c79f5f8a41d66299ce7ba53", "63bab14236d935a3e74910921f576928afe6a2a8f20cbe1c239a637d3cf4e1a6", "42189f1a4abeacc3eb5d07d4fbbd646052dfa38c3079a5b053ae1bc1efcc1142", "63292824103cbfd4405f2d8f1a9fe7780dec4b078e792cd37a7677d4a2c13ad4", "2e8cc5b36323f7c1e1f3b9e8e975c30de825e7173425b156e6aba6dd0ff40130", "3f6dc8780dff73c710237741b2f3fc90e20ce000238a72e99833418ae3109ee7", "afe85b0001a5c6abced6f76adf9a02991ae5da0957ad4360755ca2db082f92cf", "e1bc6d7d905057d59dff47512e52503cf4215422a1e7f283754a2d446f41afa1", "d7f9e8909c2a1fe73b6aa9a2ef06e6689e25e63e6ba72c8fe73996746c02bd75", "a8e8a5d7f7684661d897e98bcccfd163ae91220c38a32d90fc104113c8951879", "78884ab5d9682fdf2b9f0dc5b4d0060976a1a042c186fbcae848fcd2d0657dbe", "efb56c6c7c16ac99165388f121154f83d90ba8705a76379e686dd3fb1188d055", "42b0383dee924cc75538975717471faed6551d1c4908897845b0dc7d898a17b7", "ebcd3e838d972bbf3fdddfd06dd0ff3b7c17a34179ae131e01738dfb4dad7105", "ebcd3e838d972bbf3fdddfd06dd0ff3b7c17a34179ae131e01738dfb4dad7105", "bd5a3ad4c90eb537d970d5dae5b2df2e79255df65ee2e6e8850197c942bde35f", "8fb7a9c28981701f0210085cfa9d91a6c1e77768741f1dff4842b7e82ec5334c", "804608eb96163abf8fdd6bcf2365dfa6fbfbc160b8a60c41bd25c053a3c72139", "a818520e2026b0a532241ce411c3580af797bc4f6492c718463735292e72c2f9", "e8466545e362128ab94f384c1ab2b7eb6c7ac5475d2ff6f67fe4d044c7ed1e0c", "8463b1c436cd30705500715949c2734bd63165f8877245268cac4ff44c4716f4", "25d6216d27234fa58e19a10952422c12659133e3977c7efe5e7c896ca73d3e33", "529d0e9ed7793955d948ce20e73edd612f23eb90e48aaf55a4ee1d88ad6cced4", "70a49ce7075dc060f63e1984177620ad914613b1c08cedc9c2e9d1d98e24fcac", "c3447c0cc251c7d2c0185352d108f992d980890a02638ddbbfeba753a39a5a0b", "f6f8e9c1a2bd5bb0bfeb6d75a2847617e6f35bf132de9f070d731cff21d39945", "799c1cfb3aa700a8cc75572cb576d612d7d7ed700420ff48cf09c8ba536662e4", "1dcc9902c957c8504e3bbf2b43d067b8f87a7b25fe16f34ad3bfeb69357b05fa", "b9d32c281a4a74b092aa78568e6c6c6700a2a4aa7012670f62d1fdc8f48a1c54", "ea45be7c7bbcf917946bad0f8f7130b77b89e0d0828292039288c3cccdc85d24", "39618b85563aae1ee776522c14d6de42eb3dceadbf9c7c76cbc501b3533457a1", "fe41ef5881fd46a4ec84ceb4224c5ea00abb8b6cb431b726a174ac99580085c6", "ae5cc8a44b848b4dcdab7c3a1b5aab61eefd13af1428107828fc82294423004b", "cf040aad9108e4595a2e0eea69d4dfc134ee93127213819efd48eb37d2e51859", "3db8ea91c88c50a170ac918c4b3475716f70f0d554bf91a5c7e982da7e6d807a", "fdf336d0b046fbae55fb670ddd0852730b65cd7ebcd173827cd0e0cd9039b768", "df015cd830e04f26ce2ae1d3cc83205182f98cea1e41a29d586a79fb72d193a4", "792d4cd9b47acafaf93f6873f58b8701918db5a39af62852e3796037473387c4", "cce7c26cfda8ee1844256ac9226d0420d74c476fb24823c46bcce26db89a4983", "e654b8f55bef2f2a06efec89f171f46a76f0a55f671eb75e8b82ddc994f85b27", "0767fccd6046d0b4dcaae1150a82e505a29e59ca9f4f979e2535e7970f3de449", "d12bf2c7b96d2b1b6cec4c09b76f986249285070b1ca09d1ba1baf31b859cc18", "9ebef7d0fac03bbd44ce3016b8e06534cde5ef514b29042be9dcbf9414f248ff", "99b10b574bebe822798dd1c24eae495f08e08ec0af052a2fc8fa545ddfe67033", "c8ab73932d0de44ea5cf337ddcb4b50cdf2b17696bbfd925224c5026dc6a7e8d", "52a086c84a620d4cc8d9acb7d53ecd5826892065edd7f289447ff44f77162d74", "e68ef21ee84393f9ca196f05731cd1688e811dec61015e164d9b72fcdab62ca7", "45d3b62f28b91b005df4063493381bf77a28e03850463a68457e5409fd486bc2", "d2ac7c989cce4c289950cc76cbf269f6057f4eb42b2a999dd9c4e756f866858d", "cee7a5014ceb6ff7f702dc0b12f5378a57f92a100f6e5da772f54930604f0284", "0a5442b5dcc96c3acb88597bb5074cedf6af996869708a0c840feb143f9d93a8", "943a15a0679086634f873ef7403ca4fde941da0845bf5be3bd2d3f2744228855", "943a15a0679086634f873ef7403ca4fde941da0845bf5be3bd2d3f2744228855", "541728170855e1f3002918fde83f91824e70f9e2d19cd50e93029529dae5b547", "735821ddefdabb338994007671c41b5ffd3a02653411fd1613e9fc8a5e7e722b", "f3f9b5276f4aab31ef2d3ebb85eb5a65e3cc4050900d403ef2622ed1d60c7b2b", "e51f4b67d08661edf2ca533df45025ddb9dd31503f761f07b17b1a9b28289ac0", "f4509bfbe10781093b5f16e84854ef7d91729e5972f2136e078b1c77959ab1c4", "1dc83047ac1e444c2c399ca54c91538ab562c1ff7ced02ec946812148f8bf43c", "3bae0b4cc3685eaa14c674c4b859f67652bd15751613c48c43dd1e688ea7d584", "43ecbc402bd3e6abff7ea434414c0ef9f3001836cc689a6f1bc06bc896b44881", "1d6043ba5cfc56bc6bdbf2396637708e45c5896e671841178e60ba12a8336e0b", "610bedba4d9b133d0ff37fbd37c43e7ad1c0b066e6325a4677d9fd80d75f965e", "f76bdc9dccde93acc1a2f382d760ec79292591c5b39cc0221368630755068ce8", "dfe6fd74ceeae8d33695b6ac06285cedab253e06866a3dc569ba3657583ebdf5", "39dd5e4674ae60996d03a4187518eccccd40f1c7317452b3e0db22a20b71fecc", "4649d7267990a9f91503ade3b2259e15f94b702cddb0f41a6a566c14035d20d3"]),
        ]),
        ("gs2", &[
            ("xgcc", &["128520f13ff01aee64a984b1279a6e3a682a3679de44c99296064f46fb1e8ec2"]),
            ("cpp0", &["b4ac7f5ff7fd74f4eca40385832fd0360d13cb5d4f0b6c8b3ead4a67d2f3d5b0"]),
            ("tradcpp0", &["7698319dfea3647dace68ffb5c3dbc0fd459f3a859699acb47c669d3eb8956a3"]),
            ("cc1", &["91b2a67275a100e8b6695d85ef2d82d1fd144853cbcb361ddf1d8be31858230f"]),
        ]),
    ]),
    ("darwin-x64", &[
        ("gs1", &[
            ("xgcc", &[]),
            ("cpp", &[]),
            ("tradcpp", &[]),
            ("cc1", &[]),
        ]),
        ("gs2", &[
            ("xgcc", &[]),
            ("cpp0", &[]),
            ("tradcpp0", &[]),
            ("cc1", &[]),
        ]),
    ]),
    // PORT NOTE: `linux-arm64` precedes `linux-x64` here and follows it in
    // every other table below. That asymmetry is in the TypeScript, which is
    // keyed by host so the order is not observable for this table; it is
    // mirrored rather than tidied so the parity gate compares like for like.
    ("linux-arm64", &[
        ("gs1", &[
            ("xgcc", &[]),
            ("cpp", &[]),
            ("tradcpp", &[]),
            ("cc1", &[]),
        ]),
        ("gs2", &[
            ("xgcc", &[]),
            ("cpp0", &[]),
            ("tradcpp0", &[]),
            ("cc1", &[]),
        ]),
    ]),
    ("linux-x64", &[
        ("gs1", &[
            ("xgcc", &["845b828e15efedfeacc1956ac2694101e2b520824643d5b9f7608f9c389aee03", "2ed03493228a7873f020b16a63b89b3aadf4835be2d1a3a217cabca0fa244444", "1e7c30e3ba311981dcbf93bcc1b9e38e0581a993a22f93a7bc3193c21d254130"]),
            ("cpp", &["60d0b6637deb0f98cbf952a89694b02a0557fc87ca968121759be139372e90cc", "06096beb427848574f610626bb53408b1a76f69b178ee2d7f0a05f6c2f6d3778", "31bb32028d63e328877111df74a4d1390b6d5bfa8d077b739ae85bf7504d1a91"]),
            ("tradcpp", &["87f89bebf41cd12ac7706604dd24624061b2276f95cc1e9998c22de1accfee2a", "f9b951486d4e1769e06b892a59980c91a45435559505d73039130b63d1156803", "7a030f2838a1fa6c6e92091136dcdf8cf81d2cf49a2e11a6ee633d839c33ccac"]),
            ("cc1", &["c1cc6d2864567297451662d36fba7abbce7a916d138f7115832a265de6868a06", "640964de34d6202f6dc5943b0c22b0afd1a8f4f1307ba6d3cf30af4110f5f5e2", "c1c5be8f10668a7a66bfa2d6de2ca89f6ac17ffb57ffe62aedf7935da172f21a", "13287dcb29210f197d7fe484532458197d21a7dc2bdf7ca3fca08af447631d3b"]),
        ]),
        ("gs2", &[
            ("xgcc", &["7b1a6a96fc4bd5e9de4d83fb2a4ba2ca2a82397cdcd102c4a4d76ef91dc17f58", "d0b10d67bc7f9965d586eba766b77e6ca54cc791b5eb297b55a6b9b6d6d0ef3d"]),
            ("cpp0", &["89791031fa8d4dd686355efb0efdb7c019a4080b770f187b15671dc3c1e71ecc", "9d93c7762f60d13474764d2ca9e721b235ed4935ba7b69012aba054cace60d0d"]),
            ("tradcpp0", &["a1013c94647eefbe0caad3c2e244b66c1acf2961197bdc31012e4259616c3198", "010da8763b9ebf39cb52aab0412ca350e038ccf4c3aa5647440c2abc91dcad6c"]),
            ("cc1", &["3e5f1cbcae107b0f6c038a8b91880e377a3612a965014165002b4c530feab56b", "1b1c039eda51c0c2ee67d076f33e3284dd369789378cdf34671b66ba76cd6c75"]),
        ]),
    ]),
];

pub static AGBCC_EXPECTED: &[(&str, &[&str])] = &[
    ("darwin-arm64", &["4f7664872d10a737184fb2e0502c407c9d74505f0cff7313ba4e9083736c2207"]),
    ("darwin-x64", &[]),
    ("linux-x64", &["30a2a042c4be2acdd215ffc26c7d27498098ac38607ec8af43cc6598dcecdf55", "0c2d5ec04129f7b9d1ecf738f096167af152661bc2506f8fdb2749305fa3eb37", "21eca5a4e4d1138a1fdebccc03f6a140cbb74c072d3c10c299d64fa2cf13aef9"]),
    ("linux-arm64", &[]),
];

/// One host's approved digests for a single-executable bundle.
pub type HostDigests = (&'static str, &'static [&'static str]);

pub static PRET_EARLY_THUMB_EXPECTED: &[HostDigests] = &[
    ("darwin-arm64", &["8a1e0e9e18801efb595a3e0d571137db5ba8f97e413c323e99f18b0521a31636"]),
    ("darwin-x64", &[]),
    ("linux-x64", &["c988f677e3ebd7252a6ad1ad2fef301f85b05be0612ee3192b37ec47d22f8082"]),
    ("linux-arm64", &[]),
];

pub static GCC2951_EXPECTED: &[HostDigests] = &[
    ("darwin-arm64", &["cb41bba7e0e600721d906c46349119efb4c6fd35c711d7e0f244cb783de383a6"]),
    ("darwin-x64", &[]),
    ("linux-x64", &[
        "c8f80fffa2aa0aa2809d93ad86d11ea0e8ebf08e9bba6cc5b8d391aef05c3fe4",
        "edbee4fec1a1b59d0fd77273559aebbaf2c92b344bbeeb3539a10b689e71716d",
        "79859ae26c9c29d6b874fe27d4f4cdce72b80839ff05cc017906ef7e179c582a",
    ]),
    ("linux-arm64", &[]),
];

// Stock gcc-3.0 comparison probe (build.sh gcc3 / stage.sh gcc3), used only to
// test whether Golden Sun sources match unmodified GCC 3.0 codegen instead of
// the gcc-2.96/Camelot-ABI fork. Locally-built linux-x64 digest, 2026-08-04.
pub static GCC3_EXPECTED: &[HostDigests] = &[
    ("darwin-arm64", &[]),
    ("darwin-x64", &[]),
    ("linux-x64", &["78840db683cc441be9741153418e0fd991ea6f628a0c4f08cb4bdc1cf5ebdb9b"]),
    ("linux-arm64", &[]),
];
