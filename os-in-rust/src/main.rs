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
use core::sync::atomic;
use core::sync::atomic::Ordering;

mod vga;

global_asm!(include_str!("boot.s"));

#[no_mangle]
fn kernel_main() {
    use vga::*;
    let mut vga = VGA::new();
    vga.disable_cursor();
    vga.set_background_color(Color::Red);
    vga.put_string(b"abcd");
    vga.set_background_color(Color::Cyan);
    vga.put_string(b"abcd");
    vga.set_foreground_color(Color::Green);
    vga.put_string(b"abcd");
    vga.set_foreground_color(Color::Magenta);
    vga.put_string(b"abcd");
}

#[inline(never)]
#[panic_handler]
fn panic(_info: &PanicInfo) -> ! {
    loop {
        atomic::compiler_fence(Ordering::SeqCst);
    }
}