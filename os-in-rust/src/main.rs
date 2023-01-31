#![no_std]
#![no_main]
// see https://docs.rust-embedded.org/embedonomicon/smallest-no-std.html
#![feature(lang_items)]
#![feature(global_asm)]
#![feature(asm)]

// see https://docs.rust-embedded.org/embedonomicon/smallest-no-std.html
#[lang = "eh_personality"]
extern "C" fn eh_personality() {}

use core::panic::PanicInfo;

mod keyboard;
mod vga;

global_asm!(include_str!("boot.s"));

#[no_mangle]
fn kernel_main() {
    use keyboard::*;
    use vga::*;
    let mut vga = VGA::new();
    vga.disable_cursor();
    let mut keyboard = Keyboard::new();
    let mut last = [false; KEY_AMOUNT];
    loop {
        keyboard.update();
        let state = keyboard.state();
        state
            .iter()
            .zip(last.iter())
            .enumerate()
            .filter(|(_, (&current, &last))| current != last && current)
            .map(|(i, _)| ALPHABET[i])
            .for_each(|c| vga.put_char(c));
        last = state;
    }
}

#[inline(never)]
#[panic_handler]
fn panic(_info: &PanicInfo) -> ! {
    loop {
        use vga::*;
        let mut vga = VGA::new();
        vga.set_cursor_position(0, VGA_HEIGHT - 1);
        vga.put_string(b"error");
    }
}
