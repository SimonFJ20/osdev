pub const ALPHABET: [u8; NUM_CHARS] = [
    b'a', b'b', b'c', b'd', b'e', b'f', b'g', b'h', b'i', b'j', b'k', b'l', b'm', b'n', b'o', b'p',
    b'q', b'r', b's', b't', b'u', b'v', b'w', b'x', b'y', b'z', b'1', b'2', b'3', b'4', b'5', b'6',
    b'7', b'8', b'9', b'0', b'.', b'\n', b' ',
];

pub const NUM_CHARS: usize = 39;
pub const BACKSPACE_BIT: usize = 39;

pub struct Keyboard {
    keys_down: u64,
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

            Action::One(bool) => set_bit(bool, 26),
            Action::Two(bool) => set_bit(bool, 27),
            Action::Three(bool) => set_bit(bool, 28),
            Action::Four(bool) => set_bit(bool, 29),
            Action::Five(bool) => set_bit(bool, 30),
            Action::Six(bool) => set_bit(bool, 31),
            Action::Seven(bool) => set_bit(bool, 32),
            Action::Eight(bool) => set_bit(bool, 33),
            Action::Nine(bool) => set_bit(bool, 34),
            Action::Zero(bool) => set_bit(bool, 35),
            Action::Point(bool) => set_bit(bool, 36),

            Action::Enter(bool) => set_bit(bool, 37),
            Action::Space(bool) => set_bit(bool, 38),

            Action::Backspace(bool) => set_bit(bool, 39),
        }
    }

    pub fn state(&self) -> u64 {
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

    One(bool),
    Two(bool),
    Three(bool),
    Four(bool),
    Five(bool),
    Six(bool),
    Seven(bool),
    Eight(bool),
    Nine(bool),
    Zero(bool),

    Point(bool),

    Backspace(bool),
    Space(bool),
    Enter(bool),
}

impl Action {
    pub fn from_qwerty_u8(byte: u8) -> Self {
        let is_pressed = byte & 1 << 7 != 0;
        let byte = byte & !(1 << 7);
        match byte {
            0x10 => Action::Q(is_pressed),
            0x11 => Action::W(is_pressed),
            0x12 => Action::E(is_pressed),
            0x13 => Action::R(is_pressed),
            0x14 => Action::T(is_pressed),
            0x15 => Action::Y(is_pressed),
            0x16 => Action::U(is_pressed),
            0x17 => Action::I(is_pressed),
            0x18 => Action::O(is_pressed),
            0x19 => Action::P(is_pressed),
            0x1E => Action::A(is_pressed),
            0x1F => Action::S(is_pressed),
            0x20 => Action::D(is_pressed),
            0x21 => Action::F(is_pressed),
            0x22 => Action::G(is_pressed),
            0x23 => Action::H(is_pressed),
            0x24 => Action::J(is_pressed),
            0x25 => Action::K(is_pressed),
            0x26 => Action::L(is_pressed),
            0x2C => Action::Z(is_pressed),
            0x2D => Action::X(is_pressed),
            0x2E => Action::C(is_pressed),
            0x2F => Action::V(is_pressed),
            0x30 => Action::B(is_pressed),
            0x31 => Action::N(is_pressed),
            0x32 => Action::M(is_pressed),

            0x2 => Action::One(is_pressed),

            0x3 => Action::Two(is_pressed),

            0x4 => Action::Three(is_pressed),

            0x5 => Action::Four(is_pressed),

            0x6 => Action::Five(is_pressed),

            0x7 => Action::Six(is_pressed),

            0x8 => Action::Seven(is_pressed),

            0x9 => Action::Eight(is_pressed),

            0xA => Action::Nine(is_pressed),

            0xB => Action::Zero(is_pressed),

            0x34 => Action::Point(is_pressed),

            0xE => Action::Backspace(is_pressed),
            0x39 => Action::Space(is_pressed),
            0x1C => Action::Enter(is_pressed),

            _ => Action::Unrecognized,
        }
    }
}
