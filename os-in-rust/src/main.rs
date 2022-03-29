#![no_std]
#![no_main]

mod vga_term;

#[no_mangle]
pub extern "C" fn _start() -> ! {
    println!("hello world");

    loop {}
}

#[cfg(not(test))]
#[panic_handler]
fn panic(info: &core::panic::PanicInfo) -> ! {
    println!("{}", info);
    loop {}
}
