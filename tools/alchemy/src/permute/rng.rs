//! Integer-seeded MT19937 with deterministic sampling and replay.

#[derive(Clone, Debug, PartialEq, Eq)]
pub struct Rng {
    words: [u32; 624],
    index: usize,
}

impl Rng {
    pub fn new(seed: u64) -> Self {
        let key = [seed as u32, (seed >> 32) as u32];
        Self::from_words(&key[..if key[1] == 0 { 1 } else { 2 }])
    }

    /// Seed from the integer's absolute value, in little-endian
    /// 32-bit limbs. Decimal parsing avoids imposing a machine integer limit.
    pub fn from_decimal(seed: &str) -> Result<Self, String> {
        let digits = seed.trim().strip_prefix(['-', '+']).unwrap_or(seed.trim());
        if digits.is_empty() || !digits.bytes().all(|b| b.is_ascii_digit()) {
            return Err("expected a signed decimal integer".into());
        }
        let mut key = vec![0u32];
        for digit in digits.bytes() {
            let mut carry = u64::from(digit - b'0');
            for word in &mut key {
                carry += u64::from(*word) * 10;
                *word = carry as u32;
                carry >>= 32;
            }
            if carry != 0 { key.push(carry as u32); }
        }
        Ok(Self::from_words(&key))
    }

    fn from_words(key: &[u32]) -> Self {
        let mut rng = Self { words: [0; 624], index: 624 };
        rng.words[0] = 19_650_218;
        for i in 1..624 {
            rng.words[i] = 1_812_433_253u32
                .wrapping_mul(rng.words[i - 1] ^ (rng.words[i - 1] >> 30))
                .wrapping_add(i as u32);
        }
        let (mut i, mut j) = (1, 0);
        for _ in 0..624.max(key.len()) {
            rng.words[i] = (rng.words[i] ^ (rng.words[i - 1] ^ (rng.words[i - 1] >> 30)).wrapping_mul(1_664_525))
                .wrapping_add(key[j])
                .wrapping_add(j as u32);
            i += 1;
            j += 1;
            if i == 624 {
                rng.words[0] = rng.words[623];
                i = 1;
            }
            if j == key.len() {
                j = 0;
            }
        }
        for _ in 0..623 {
            rng.words[i] = (rng.words[i] ^ (rng.words[i - 1] ^ (rng.words[i - 1] >> 30)).wrapping_mul(1_566_083_941)).wrapping_sub(i as u32);
            i += 1;
            if i == 624 {
                rng.words[0] = rng.words[623];
                i = 1;
            }
        }
        rng.words[0] = 0x8000_0000;
        rng
    }

    fn word(&mut self) -> u32 {
        if self.index == 624 {
            for i in 0..624 {
                let y = (self.words[i] & 0x8000_0000) | (self.words[(i + 1) % 624] & 0x7fff_ffff);
                self.words[i] = self.words[(i + 397) % 624] ^ (y >> 1) ^ if y & 1 != 0 { 0x9908_b0df } else { 0 };
            }
            self.index = 0;
        }
        let mut y = self.words[self.index];
        self.index += 1;
        y ^= y >> 11;
        y ^= (y << 7) & 0x9d2c_5680;
        y ^= (y << 15) & 0xefc6_0000;
        y ^ (y >> 18)
    }

    fn bits(&mut self, bits: u32) -> u64 {
        if bits == 0 {
            return 0;
        }
        if bits <= 32 {
            return u64::from(self.word() >> (32 - bits));
        }
        let low = u64::from(self.word());
        low | (u64::from(self.word() >> (64 - bits)) << 32)
    }

    pub fn next(&mut self) -> u64 {
        self.bits(64)
    }

    pub fn candidate_seed(&mut self) -> u128 {
        const WIDTH: u128 = 100_000_000_000_000_000_000 - 1;
        loop {
            let value = u128::from(self.bits(64)) | (u128::from(self.bits(3)) << 64);
            if value < WIDTH { return value + 1; }
        }
    }

    pub fn random(&mut self) -> f64 {
        let a = self.word() >> 5;
        let b = self.word() >> 6;
        (f64::from(a) * 67_108_864.0 + f64::from(b)) / 9_007_199_254_740_992.0
    }

    pub fn below(&mut self, bound: usize) -> usize {
        assert!(bound > 0, "empty random choice");
        let bits = usize::BITS - bound.leading_zeros();
        loop {
            let value = self.bits(bits) as usize;
            if value < bound {
                return value;
            }
        }
    }

    pub fn chance(&mut self, numerator: u64, denominator: u64) -> bool {
        self.random() < numerator as f64 / denominator as f64
    }

    /// rejection sampling. The order matters to variable aliasing/removal.
    pub fn sample_pair(&mut self, length: usize) -> (usize, usize) {
        assert!(length >= 2, "sample larger than population");
        let first = self.below(length);
        let second = if length <= 21 {
            let selected = self.below(length - 1);
            if selected == first { length - 1 } else { selected }
        } else {
            loop { let selected = self.below(length); if selected != first { break selected; } }
        };
        (first, second)
    }
}

/// Candidate-local randomization never consumes the scheduler's stream.
pub struct WalkRng {
    control: Rng,
    forced: Option<Rng>,
    pub mutation: Rng,
    started: bool,
}

impl WalkRng {
    pub fn new(control_seed: u64, forced: Option<Rng>) -> Self {
        Self { control: Rng::new(control_seed), forced, mutation: Rng::new(0), started: false }
    }

    pub fn keep(&mut self, random: bool, last_score: Option<(u64, usize)>, probability: f64) -> bool {
        // Randomized inputs consume the keep draw before checking the score,
        // including the initial evaluation.
        let keep = (random && self.control.random() < probability
            && last_score != Some((0, 0)) && last_score != Some((u64::MAX, usize::MAX))) || self.forced.is_some();
        let keep = self.started && keep;
        if !keep {
            self.mutation = self.forced.clone().unwrap_or_else(|| {
                Rng::from_decimal(&self.control.candidate_seed().to_string()).unwrap()
            });
            self.started = true;
        }
        keep
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn candidate_scheduling_and_independent_streams() {
        let mut walk = WalkRng::new(0, None);
        for row in include_str!("c/fixtures/rng-scheduling.tsv").lines() {
            let c: Vec<_> = row.split('\t').collect();
            let step: usize = c[3].parse().unwrap();
            if step == 0 {
                let forced = (!matches!(c[1], "none" | "0")).then(|| Rng::from_decimal(c[1]).unwrap());
                walk = WalkRng::new(c[0].parse().unwrap(), forced);
            }
            let score = match c[4] { "none" => None, "1000000000" => Some((u64::MAX, usize::MAX)), n => Some((n.parse().unwrap(), 0)) };
            assert_eq!(walk.keep(c[2] == "1", score, 0.6), c[5] == "1", "{row}");
            for _ in 0..step % 4 { walk.mutation.random(); }
            assert_eq!(walk.mutation.clone().random(), c[6].parse::<f64>().unwrap(), "{row}");
            assert_eq!(walk.control.clone().random(), c[7].parse::<f64>().unwrap(), "{row}");
        }
    }

    #[test]
    fn weighted_zero_with_instruction_differences_keeps_the_walk() {
        let mut walk = WalkRng::new(0, None);
        assert!(!walk.keep(true, None, 1.0));
        assert!(walk.keep(true, Some((0, 42)), 1.0));
        assert!(!walk.keep(true, Some((0, 0)), 1.0));
    }

    #[test]
    fn arbitrary_signed_integer_seeds_match_sampling_vectors() {
        for row in include_str!("c/fixtures/rng-integers.tsv").lines() {
            let mut columns = row.split('\t');
            let seed = columns.next().unwrap();
            let mut rng = Rng::from_decimal(seed).unwrap();
            for value in columns { assert_eq!(rng.random(), value.parse::<f64>().unwrap()); }
        }
        for invalid in ["", "-", "+", "1.0", "0x10", "--1", "1 2"] {
            assert!(Rng::from_decimal(invalid).is_err(), "{invalid}");
        }
        assert_eq!(Rng::from_decimal(" +0001 ").unwrap(), Rng::new(1));
        assert_eq!(Rng::from_decimal("-0").unwrap(), Rng::new(0));
    }

    #[test]
    fn ordered_pair_sampling_vectors() {
        for row in include_str!("c/fixtures/rng-sample.tsv").lines() {
            let c: Vec<_> = row.split('\t').collect();
            let mut rng = Rng::new(c[1].parse().unwrap());
            assert_eq!(rng.sample_pair(c[0].parse().unwrap()), (c[2].parse().unwrap(), c[3].parse().unwrap()), "size {} seed {}", c[0], c[1]);
            assert_eq!(rng.random(), c[4].parse::<f64>().unwrap());
        }
    }

    #[test]
    fn integer_seed_vectors() {
        let mut rng = Rng::new(0);
        assert_eq!((rng.word(), rng.word(), rng.word()), (3_626_764_237, 1_654_615_998, 3_255_389_356));
        let mut rng = Rng::new(1);
        assert_eq!(rng.random(), 0.13436424411240122);
        assert_eq!(rng.random(), 0.8474337369372327);
        assert_eq!(rng.random(), 0.763774618976614);
        let mut rng = Rng::new(0);
        assert_eq!((0..10).map(|_| rng.below(10)).collect::<Vec<_>>(), [6, 6, 0, 4, 8, 7, 6, 4, 7, 5]);
        let mut rng = Rng::new(1234);
        assert_eq!(rng.random(), 0.9664535356921388);
    }
}
