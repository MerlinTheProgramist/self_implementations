#[derive(PartialEq)]
pub enum PrimeResult {
    Prime,
    NotPrime,
    Undefined,
}

pub fn is_prime(x: usize) -> PrimeResult {
    use PrimeResult::*;
    if x < 2 {
        Undefined
    } else if x < 4 {
        Prime
    } else if x % 2 == 0 {
        NotPrime
    } else {
        for i in (3..=x.isqrt()).step_by(2) {
            if x % i == 0 {
                return NotPrime;
            }
        }
        Prime
    }
}

pub fn next_prime(mut x: usize) -> usize {
    while is_prime(x) != PrimeResult::Prime {
        x += 1;
    }
    x
}
