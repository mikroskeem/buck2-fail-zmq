#[global_allocator]
static GLOBAL: mimalloc::MiMalloc = mimalloc::MiMalloc;

fn main() {
    let _ctx = zmq::Context::new();
    println!("Hello, world!");
}
