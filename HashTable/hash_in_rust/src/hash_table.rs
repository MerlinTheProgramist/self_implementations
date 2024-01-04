use crate::prime::next_prime;
use std::default::Default;

pub trait HashKey {
    fn hash(&self, a: u64, m: u64) -> u64;
}

impl HashKey for &str {
    fn hash(&self, a: u64, m: u64) -> u64 {
        self.char_indices().fold(0u64, |acc, (i, c)| {
            (acc + a.wrapping_pow((self.len() - (i + 1)) as u32 * c as u32)) % m
        })
    }
}
impl HashKey for String {
    fn hash(&self, a: u64, m: u64) -> u64 {
        self.as_str().hash(a, m)
    }
}

#[derive(Clone, Default)]
struct Item<K, V> {
    key: K,
    value: V,
}

#[derive(Clone)]
enum Entry<K, V> {
    Occupied(Item<K, V>),
    Deleted,
}

impl<K, V> Entry<K, V> {
    pub fn is_occupied(&self) -> bool {
        match self {
            Self::Occupied(_) => true,
            _ => false,
        }
    }
}

// impl<K, V> Copy for Item<K, V> {}

// impl<K: Key, V: Copy> Clone for Item<K, V> {
//     fn clone(&self) -> Self {

//     }
// }
// impl<K: Key, V: Copy> Clone for Entry<K, V> {
//     fn clone(&self) -> Self {
//         *self
//     }
// }

const INITIAL_BASE_SIZE: usize = 53;
const HT_PRIME_1: u64 = 50331653;
const HT_PRIME_2: u64 = 201326611;

pub struct HashTable<K: Clone + HashKey + PartialEq, V: Clone> {
    base_size: usize,
    size: usize,
    count: usize,
    items: Vec<Option<Entry<K, V>>>,
}

impl<K: Clone + HashKey + PartialEq, V: Clone> HashTable<K, V> {
    pub fn new() -> Self {
        Self::with_size(INITIAL_BASE_SIZE)
    }

    pub fn with_size(base_size: usize) -> Self {
        let size = next_prime(base_size);
        Self {
            base_size,
            size,
            count: 0,
            items: vec![None; size],
        }
    }
    /// inserts a new value,
    /// or updates if there is already with the same key, and returns the original
    pub fn insert<'a>(&mut self, key: K, value: V) -> Option<V> {
        let load = self.count * 100 / self.size;
        if load > 70 {
            self.resize_up();
        }

        let mut i = 0;
        let mut index = self.get_hash(&key, self.size, i);
        // let mut curr_item = self.items[index];
        while let Some(item) = &mut self.items[index] {
            if let Entry::Occupied(item) = item {
                if item.key == key {
                    let old: V = item.value.clone();
                    item.value = value;
                    return Some(old);
                }
                index = self.get_hash(&key, self.size, i);
                i += 1;
            }
        }
        self.items[index] = Some(Entry::Occupied(Item { key, value }));
        self.count += 1;
        None
    }

    pub fn get(&self, key: &K) -> Option<&V> {
        if self.is_empty() {
            return None;
        }

        let mut i = 0;
        let mut index = self.get_hash(key, self.size, i);
        while let Some(item) = &self.items[index] {
            if let Entry::Occupied(item) = item {
                if item.key == *key {
                    return Some(&item.value);
                }
            }
            i += 1;
            index = self.get_hash(key, self.size, i);
        }
        return None;
    }
    pub fn get_mut<'a>(&'a mut self, key: &K) -> Option<&'a mut V> {
        if self.is_empty() {
            return None;
        }

        let mut final_id = None;
        let mut i = 0;
        let mut index = self.get_hash(key, self.size, i);
        while let Some(item) = &self.items[index] {
            if let Entry::Occupied(item) = item {
                if item.key == *key {
                    final_id = Some(index);
                    break;
                }
            }
            i += 1;
            index = self.get_hash(key, self.size, i);
        }
        match final_id {
            Some(index) => match unsafe { self.items[index].as_mut().unwrap_unchecked() } {
                Entry::Occupied(item) => Some(&mut item.value),
                Entry::Deleted => None,
            },
            None => None,
        }
    }

    pub fn clear(&mut self) {
        *self = Self::new();
    }

    pub fn is_empty(&self) -> bool {
        self.count == 0
    }
    pub fn len(&self) -> usize {
        self.count
    }

    pub fn remove(&mut self, key: &K) -> Option<V> {
        if self.is_empty() {
            return None;
        }

        let load = self.count * 100 / self.size;
        if load < 10 {
            self.resize_down();
        }

        let mut i = 0;
        let mut index = self.get_hash(key, self.size, i);
        while let Some(entry) = &self.items[index] {
            if let Entry::Occupied(item) = entry {
                if item.key == *key {
                    let value = item.value.clone();
                    self.items[index] = Some(Entry::Deleted);
                    self.count -= 1;
                    return Some(value);
                }
            }
            i += 1;
            index = self.get_hash(key, self.size, i);
        }
        return None;
    }

    // private

    fn resize(&mut self, base_size: usize) {
        if base_size < INITIAL_BASE_SIZE {
            return;
        }

        let items_collected: Vec<_> = self
            .items
            .iter()
            .filter_map(|field| {
                // filter only present items,
                if let Some(Entry::Occupied(item)) = field {
                    Some(item.clone())
                } else {
                    None
                }
            })
            .collect();

        // create new hashTable
        *self = Self::with_size(base_size);
        // insert collected items
        for item in items_collected {
            self.insert(item.key, item.value);
        }
    }

    fn resize_up(&mut self) {
        self.resize(self.base_size * 2);
    }
    fn resize_down(&mut self) {
        self.resize(self.base_size / 2);
    }

    fn get_hash(&self, key: &K, num_buckets: usize, attempt: usize) -> usize {
        let hash_a = HashKey::hash(key, HT_PRIME_1, num_buckets as u64);
        let hash_b = HashKey::hash(key, HT_PRIME_2, num_buckets as u64);
        ((hash_a + (attempt as u64 * hash_b + 1)) % num_buckets as u64) as usize
    }
}

/// panics if no element with key was present
impl<K: Clone + HashKey + PartialEq, V: Clone> std::ops::Index<K> for HashTable<K, V> {
    type Output = V;
    fn index(&self, key: K) -> &Self::Output {
        self.get(&key).unwrap()
    }
}
