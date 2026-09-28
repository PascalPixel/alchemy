//! Named C source repairs. Callers name a plan from their own evidence; the
//! label tells the reader which source change to try, and the change still
//! has to be written and verified byte for byte.
use regex::Regex;
#[derive(Clone, Debug, Eq, PartialEq)]
pub enum Repair {
    SwapDeclarations {
        left: String,
        right: String,
    },
    SplitLifetime {
        name: String,
    },
    MergeZeroCarrier,
    ReciprocalRoleSwap {
        name: String,
    },
    MergeCarrierPhases {
        earlier: String,
        later: String,
    },
    /// Structural repair: the reference's comparison branches are the
    /// mirror of the candidate's, so some relational guard is spelled with
    /// its operands in the other order.
    MirrorRelationalGuards,
}

#[derive(Clone, Debug, Eq, PartialEq)]
pub struct RepairPlan {
    repairs: Vec<Repair>,
}

impl From<Repair> for RepairPlan {
    fn from(repair: Repair) -> Self {
        Self::one(repair)
    }
}

impl RepairPlan {
    pub fn one(repair: Repair) -> Self {
        Self {
            repairs: vec![repair],
        }
    }

    pub fn two(first: Repair, second: Repair) -> Self {
        Self {
            repairs: vec![first, second],
        }
    }

    pub fn label(&self) -> String {
        if self.repairs.len() == 1 {
            self.repairs[0].label()
        } else {
            format!(
                "compose({})",
                self.repairs
                    .iter()
                    .map(Repair::label)
                    .collect::<Vec<_>>()
                    .join(",")
            )
        }
    }
}

impl Repair {
    pub fn label(&self) -> String {
        match self {
            Self::SwapDeclarations { left, right } => format!("swap_declarations({left},{right})"),
            Self::SplitLifetime { name } => format!("split_lifetime({name})"),
            Self::MergeZeroCarrier => "merge_lifetime(zero_carrier)".into(),
            Self::ReciprocalRoleSwap { name } => format!("reciprocal_register_role_swap({name})"),
            Self::MergeCarrierPhases { earlier, later } => {
                format!("merge_carrier_phases({earlier},{later})")
            }
            Self::MirrorRelationalGuards => "mirror_relational_guards".into(),
        }
    }
}

pub fn split_pointer_uses(code: &str, name: &str) -> bool {
    let name = regex::escape(name);
    let uses = format!(r"\b{name}\b");
    Regex::new(&uses).unwrap().find_iter(code).count() == 4
        && !Regex::new(&format!(
            r"(?:\+\+|--)\s*\b{0}\b|\b{0}\b\s*(?:\+\+|--|[-+*/%&|^]=|<<=|>>=)",
            name
        ))
        .unwrap()
        .is_match(code)
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn plans_label_one_repair_or_a_composition() {
        assert_eq!(
            RepairPlan::one(Repair::MirrorRelationalGuards).label(),
            "mirror_relational_guards"
        );
        let plan = RepairPlan::two(
            Repair::MergeZeroCarrier,
            Repair::SplitLifetime { name: "i".into() },
        );
        assert_eq!(
            plan.label(),
            "compose(merge_lifetime(zero_carrier),split_lifetime(i))"
        );
    }

    #[test]
    fn pointer_split_rejects_induction() {
        let source = "u8 *id; id = base; use(*id); use(*id);";
        assert!(split_pointer_uses(source, "id"));
        for update in ["id++", "++id", "id--", "--id", "id += 1", "id <<= 1"] {
            assert!(!split_pointer_uses(
                &source.replace("use(*id);", update),
                "id"
            ));
        }
    }
}
