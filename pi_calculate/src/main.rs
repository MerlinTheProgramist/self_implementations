use std::{
    borrow::BorrowMut,
    f64::consts::PI,
    ops::{Add, AddAssign},
    sync::Mutex,
    thread,
};
// use std::

// trait NegWhen<T>{
//     fn neg_when(fn()) ->
// }

fn threaded_sum<'a, Tin: Sync, Tout: Send + Default + Clone + AddAssign>(
    params: &Vec<Tin>,
    func: fn(&Tin) -> Tout,
    max_threads: i32,
) -> Tout {
    let result = Mutex::new(Tout::default());
    let jobs = Mutex::new(params.into_iter());

    thread::scope(|scope| {
        for _ in 0..max_threads {
            scope.spawn(|| {
                // Closure used to prevent mutex beeing held during loop body
                let next = || jobs.lock().unwrap().next();

                while let Some(i) = next() {
                    let out = func(i);
                    *result.lock().unwrap() += out;
                }
            });
        }
    });
    result.into_inner().unwrap()
}

fn nilcatha_series(n: u128) -> f64 {
    let inputs = (2..n).step_by(2).collect::<Vec<_>>();
    3f64 + threaded_sum(
        &inputs,
        |x| if x % 4 == 0 { -4f64 } else { 4f64 } / (x * (x + 1) * (x + 2)) as f64,
        10,
    )
}

fn main() {
    println!(
        "PI =     {}",
        nilcatha_series(
            std::env::args()
                .last()
                .unwrap()
                .parse()
                .expect("unsigned integer expected")
        )
    );
    println!("REAL PI: {}", PI);
}
