pub const ALPHABET: [u8; 28] = [
    b'a', b'b', b'c', b'd', b'e', b'f', b'g', b'h', b'i', b'j', b'k', b'l', b'm', b'n', b'o', b'p',
    b'q', b'r', b's', b't', b'u', b'v', b'w', b'x', b'y', b'z', b'\n', b' ',
];

pub struct Keyboard {
    keys_down: u32,
}

impl Keyboard {
    pub fn new() -> Self {
        Self { keys_down: 0 }
    }

    pub fn update(&mut self) {
        let mut x: u8 = 0;
        unsafe {
            asm!(
                "in al, 0x60",
                "mov {}, al",
                inout(reg_byte) x,
            );
        }
        let action = Action::from_qwerty_u8(x);
        let mut set_bit = |enable: bool, n: usize| {
            if enable {
                self.keys_down |= 1 << n;
            } else {
                self.keys_down &= !(1 << n);
            }
        };

        match action {
            Action::Unrecognized => {}

            Action::A(bool) => set_bit(bool, 0),
            Action::B(bool) => set_bit(bool, 1),
            Action::C(bool) => set_bit(bool, 2),
            Action::D(bool) => set_bit(bool, 3),
            Action::E(bool) => set_bit(bool, 4),
            Action::F(bool) => set_bit(bool, 5),
            Action::G(bool) => set_bit(bool, 6),
            Action::H(bool) => set_bit(bool, 7),
            Action::I(bool) => set_bit(bool, 8),
            Action::J(bool) => set_bit(bool, 9),
            Action::K(bool) => set_bit(bool, 10),
            Action::L(bool) => set_bit(bool, 11),
            Action::M(bool) => set_bit(bool, 12),
            Action::N(bool) => set_bit(bool, 13),
            Action::O(bool) => set_bit(bool, 14),
            Action::P(bool) => set_bit(bool, 15),
            Action::Q(bool) => set_bit(bool, 16),
            Action::R(bool) => set_bit(bool, 17),
            Action::S(bool) => set_bit(bool, 18),
            Action::T(bool) => set_bit(bool, 19),
            Action::U(bool) => set_bit(bool, 20),
            Action::V(bool) => set_bit(bool, 21),
            Action::W(bool) => set_bit(bool, 22),
            Action::X(bool) => set_bit(bool, 23),
            Action::Y(bool) => set_bit(bool, 24),
            Action::Z(bool) => set_bit(bool, 25),
            Action::Backspace(bool) => {}
            Action::Enter(bool) => set_bit(bool, 26),
            Action::Space(bool) => set_bit(bool, 27),

            Action::Zero(bool) => {}
            Action::One(bool) => {}
            Action::Nine(bool) => {}

            Action::Point(bool) => {}
        }
    }

    pub fn state(&self) -> u32 {
        self.keys_down
    }
}

enum Action {
    Unrecognized,

    Q(bool),
    W(bool),
    E(bool),
    R(bool),
    T(bool),
    Y(bool),
    U(bool),
    I(bool),
    O(bool),
    P(bool),
    A(bool),
    S(bool),
    D(bool),
    F(bool),
    G(bool),
    H(bool),
    J(bool),
    K(bool),
    L(bool),
    Z(bool),
    X(bool),
    C(bool),
    V(bool),
    B(bool),
    N(bool),
    M(bool),

    Zero(bool),
    One(bool),
    Nine(bool),

    Point(bool),

    Backspace(bool),
    Space(bool),
    Enter(bool),
}

impl Action {
    pub fn from_qwerty_u8(byte: u8) -> Self {
        match byte {
            0x10 => Action::Q(true),
            0x90 => Action::Q(false),
            0x11 => Action::W(true),
            0x91 => Action::W(false),
            0x12 => Action::E(true),
            0x92 => Action::E(false),
            0x13 => Action::R(true),
            0x93 => Action::R(false),
            0x14 => Action::T(true),
            0x94 => Action::T(false),
            0x15 => Action::Y(true),
            0x95 => Action::Y(false),
            0x16 => Action::U(true),
            0x96 => Action::U(false),
            0x17 => Action::I(true),
            0x97 => Action::I(false),
            0x18 => Action::O(true),
            0x98 => Action::O(false),
            0x19 => Action::P(true),
            0x99 => Action::P(false),
            0x1E => Action::A(true),
            0x9E => Action::A(false),
            0x1F => Action::S(true),
            0x9F => Action::S(false),
            0x20 => Action::D(true),
            0xA0 => Action::D(false),
            0x21 => Action::F(true),
            0xA1 => Action::F(false),
            0x22 => Action::G(true),
            0xA2 => Action::G(false),
            0x23 => Action::H(true),
            0xA3 => Action::H(false),
            0x24 => Action::J(true),
            0xA4 => Action::J(false),
            0x25 => Action::K(true),
            0xA5 => Action::K(false),
            0x26 => Action::L(true),
            0xA6 => Action::L(false),
            0x2C => Action::Z(true),
            0xAC => Action::Z(false),
            0x2D => Action::X(true),
            0xAD => Action::X(false),
            0x2E => Action::C(true),
            0xAE => Action::C(false),
            0x2F => Action::V(true),
            0xAF => Action::V(false),
            0x30 => Action::B(true),
            0xB0 => Action::B(false),
            0x31 => Action::N(true),
            0xB1 => Action::N(false),
            0x32 => Action::M(true),
            0xB2 => Action::M(false),

            0x29 => Action::Zero(true),
            0x2 => Action::One(true),
            0xA => Action::Nine(true),

            0x34 => Action::Point(true),
            0xB4 => Action::Point(false),

            0xE => Action::Backspace(true),
            0x8E => Action::Backspace(false),
            0x39 => Action::Space(true),
            0xB9 => Action::Space(false),
            0x1C => Action::Enter(true),
            0x9C => Action::Enter(false),

            _ => Action::Unrecognized,
        }
    }
}
