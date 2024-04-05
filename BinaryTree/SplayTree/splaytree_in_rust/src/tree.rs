pub struct Node<K, V> {
    key: K,
    value: V,
    left: Option<Box<Node<K, V>>>,
    right: Option<Box<Node<K, V>>>,
}

impl<K: PartialOrd, V> Node<K, V> {
    fn insert(&self, key: K, value: V) {
        if key < self.key {
            if let Some(node) = self.left {
                node.insert(key, value);
            } else {
                self.left = Some(Box::new(Node {
                    key,
                    value,
                    left: None,
                    right: None,
                }));
            }
        } else {
            if let Some(node) = self.right {
                node.insert(key, value);
            } else {
                self.right = Some(Box::new(Node {
                    key,
                    value,
                    left: None,
                    right: None,
                }));
            }
        }
    }
}

pub trait Tree<K, V> {
    fn get(key: &K) -> &V;
    fn insert(key: K, value: V);
    fn remove(key: &K);
}
