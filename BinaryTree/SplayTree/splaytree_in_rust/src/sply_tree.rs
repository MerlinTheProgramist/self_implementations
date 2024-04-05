use std::{default, fmt::Debug};

use crate::tree::{Node, Tree};

pub struct SplayTree<K, V> {
    root: Node<K, V>,
}

impl<K, V> SplayTree<K, V>
where
    K: Default + Ord,
    V: Default + Ord,
{
    pub fn new() -> Self {
        Self {
            root: Node {
                key: Default::default(),
                value: Default::default(),
                left: None,
                right: None,
            },
        }
    }
}

impl<K, V> Tree<K, V> for SplayTree<K, V>
where
    K: Default + Ord,
    V: Default,
{
    fn get(key: &K) -> &V {}
    fn insert(key: K, value: V) {
        root.insert(key, value);
    }
    fn remove(key: &K) {}
}
