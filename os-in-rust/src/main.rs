#![no_std]
#![no_main]
// see https://docs.rust-embedded.org/embedonomicon/smallest-no-std.html
#![feature(lang_items)]
#![feature(global_asm)]
#![feature(asm)]
#![feature(bench_black_box)]

// see https://docs.rust-embedded.org/embedonomicon/smallest-no-std.html
#[lang = "eh_personality"]
extern "C" fn eh_personality() {}

use core::panic::PanicInfo;

//mod bf;
mod keyboard;
mod vga;

global_asm!(include_str!("boot.s"));

#[no_mangle]
fn kernel_main() {
    //use bf::*;
    use keyboard::*;
    use vga::*;
    let mut vga = VGA::new();
    vga.disable_cursor();
    let mut keyboard = Keyboard::new();
    let mut last = 0;
    loop {
        keyboard.update();
        let state = keyboard.state();
        if state != last {
            for n in 0..NUM_CHARS {
                let is_set = state & !last & 1 << n != 0;
                if is_set {
                    vga.put_char(ALPHABET[n]);
                }
            }

            let is_backspace_set = state & !last & 1 << BACKSPACE_BIT != 0;
            if is_backspace_set {
                vga.delete_char();
            }

            /*
            let is_run_set = state & !last & 1 << 35 != 0;
            if is_run_set {
                let mut context = Context::new();
                let buffer = unsafe {
                    core::slice::from_raw_parts(vga.buffer(), VGA_WIDTH * VGA_HEIGHT * 2)
                };
                context.generate_bracket_pairs(buffer);
                context.run(&mut vga, buffer);
            }
            */

            core::hint::black_box(last);
            last = state;
        }
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
