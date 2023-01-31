pub const VGA_WIDTH: usize = 80;
pub const VGA_HEIGHT: usize = 25;

#[allow(dead_code)]
#[derive(Clone)]
#[repr(u8)]
pub enum Color {
    Black = 0,
    Blue = 1,
    Green = 2,
    Cyan = 3,
    Red = 4,
    Magenta = 5,
    Brown = 6,
    LightGrey = 7,
    DarkGrey = 8,
    LightBlue = 9,
    LightGreen = 10,
    LightCyan = 11,
    LightRed = 12,
    LightMagenta = 13,
    LightBrown = 14,
    White = 15,
}

pub struct VGA {
    row: usize,
    column: usize,
    color: u8,
    buffer: *mut u8,
}

#[allow(dead_code)]
impl VGA {
    pub fn new() -> Self {
        let vga_buffer = 0xb8000 as *mut u8;
        Self {
            row: 0,
            column: 0,
            buffer: vga_buffer,
            color: Color::White as u8,
        }
    }
    pub fn set_cursor_position(&mut self, row: usize, column: usize) {
        self.row = row;
        self.column = column;
    }
    pub fn set_background_color(&mut self, input: Color) {
        self.color &= 0b0000_1111;
        self.color |= (input as u8) << 4;
    }
    pub fn set_foreground_color(&mut self, input: Color) {
        self.color &= 0b1111_0000;
        self.color |= input as u8;
    }
    pub fn put_char(&mut self, input: u8) {
        if input == b'\n' {
            self.row = VGA_WIDTH;
            self.step();
            return;
        }
        let index = self.row + self.column * VGA_WIDTH;
        unsafe {
            *self.buffer.offset(index as isize * 2) = input;
            *self.buffer.offset(index as isize * 2 + 1) = self.color as u8;
        }
        self.step();
    }
    pub fn disable_cursor(&mut self) {
        unsafe {
            asm!(
                "mov dx, 0x3D4",
                "mov al, 0xA",
                "out dx, al",
                "inc dx",
                "mov al, 0x20",
                "out dx, al",
            );
        }
    }
    pub fn put_string(&mut self, input: &[u8]) {
        input.iter().for_each(|c| self.put_char(*c));
    }
    fn step(&mut self) {
        self.row += 1;
        if self.row >= VGA_WIDTH {
            self.row = 0;
            self.column += 1;
        }
        if self.column >= VGA_HEIGHT {
            self.column = VGA_HEIGHT - 1;
            self.scroll();
        }
    }
    fn scroll(&mut self) {
        (1..VGA_HEIGHT).for_each(|h| {
            (0..VGA_WIDTH * 2).for_each(|w| {
                let current_index = w + h * VGA_WIDTH * 2;
                let target_index = w + (h - 1) * VGA_WIDTH * 2;
                unsafe {
                    *self.buffer.add(target_index) = *self.buffer.add(current_index);
                }
            });
        });
        (0..VGA_WIDTH * 2).for_each(|width| {
            let height = VGA_HEIGHT - 1;
            let current_index = width + height * VGA_WIDTH * 2;
            unsafe {
                *self.buffer.add(current_index) = 0;
            }
        });
    }
}
