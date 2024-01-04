#![feature(isqrt)]
#![allow(dead_code)]

mod hash_table;
mod prime;

#[cfg(test)]
mod tests {
    use crate::prime::PrimeResult;

    use super::hash_table::HashTable;
    use super::prime::{is_prime, next_prime};

    #[test]
    fn prime_generation() {
        for _ in 0..100 {
            let r: u32 = rand::random();
            let prime = next_prime(r as usize);
            assert!(prime >= r as usize);
            assert!(is_prime(prime) == PrimeResult::Prime);
        }
    }

    #[test]
    fn initializing() {
        let _ht = HashTable::<&str, &str>::new();
    }

    #[test]
    fn insert() {
        let mut ht = HashTable::<&str, &str>::new();
        ht.insert(&"Steven", &"Skiena");
    }

    #[test]
    fn insert_multiple() {
        let mut ht = HashTable::<String, String>::new();
        for c in u8::MIN..=u8::MAX {
            assert_eq!(None, ht.insert(char::from(c).into(), char::from(c).into()));
        }
    }

    #[test]
    fn overwrite_value() {
        let mut ht = HashTable::<&str, &str>::new();
        ht.insert(&"Steven", &"Skiena");
        assert_eq!(Some("Skiena"), ht.insert(&"Steven", &"Jobs"));
    }

    #[test]
    fn remove_item() {
        let mut ht = HashTable::<&str, &str>::new();
        ht.insert(&"Steven", &"Skiena");
        assert_eq!(Some("Skiena"), ht.remove(&"Steven"));
    }

    #[test]
    fn get_item() {
        let mut ht = HashTable::<&str, &str>::new();
        ht.insert(&"Steven", &"Skiena");
        assert_eq!(Some(&"Skiena"), ht.get(&"Steven"));
    }

    #[test]
    fn get_mutable() {
        let mut ht = HashTable::<&str, &str>::new();
        ht.insert(&"Steven", &"Skiena");

        let mutable = ht.get_mut(&"Steven").unwrap();
        assert_eq!(&"Skiena", mutable);

        *mutable = "Spilberg";
        assert_eq!(Some(&"Spilberg"), ht.get(&"Steven"))
    }

    #[test]
    fn get_not_present_item() {
        let mut ht = HashTable::<&str, &str>::new();
        ht.insert(&"Steven", &"Skiena");
        assert_eq!(None, ht.get(&"Stephen"));
    }

    #[test]
    fn index_item() {
        let mut ht = HashTable::<&str, &str>::new();
        ht.insert(&"Steven", &"Skiena");
        assert_eq!("Skiena", ht[&"Steven"]);
    }

    #[test]
    #[should_panic]
    fn index_not_present_item() {
        let mut ht = HashTable::<&str, &str>::new();
        ht.insert(&"Steven", &"Skiena");
        ht["Stephen"];
    }

    #[test]
    fn clearing() {
        let mut ht = HashTable::<&str, &str>::new();
        ht.insert(&"Steven", &"Skiena");
        ht.clear();

        assert_eq!(None, ht.get(&"Steven"));
        ht.insert(&"Stephen", &"Hawking");
        assert_eq!(Some(&"Hawking"), ht.get(&"Stephen"));
    }
}
