pub struct Trie {
    root: Box<Node>,
}

type Link = Option<Box<Node>>;

#[derive(Clone)]
struct Node {
    data: u8,
    children: Vec<Link>,
    is_leaf: bool,
}

impl Node {
    fn new(size: usize, data: u8) -> Box<Node> {
        Box::new(Self {
            data,
            children: vec![None; size],
            is_leaf: false,
        })
    }
    fn insert(&mut self, word: &[u8]) {
        let size = self.children.len();
        match word.len() {
            0 => {
                self.is_leaf = true;
            }
            _ => {
                self.children[word[0] as usize]
                    .get_or_insert(Node::new(size, word[0]))
                    .insert(&word[1..]);
            }
        }
    }
}

impl Trie {
    pub fn new(size: usize) -> Self {
        Self {
            root: Node::new(size, 0),
        }
    }
    pub fn insert(&mut self, word: &str) {
        self.root.insert(word.as_bytes())
    }
    pub fn search(&self, word: &str) -> bool {
        let mut node = &self.root;
        for c in word.as_bytes().iter().map(|c| c - b'a') {
            match &node.children[c as usize] {
                None => return false,
                Some(next) => node = &next,
            }
        }
        node.is_leaf
    }
    pub fn starts_with(&self, prefix: &str) -> bool {
        let mut node = &self.root;
        for c in prefix.as_bytes().iter().map(|c| c - b'a') {
            match &node.children[c as usize] {
                None => return false,
                Some(next) => node = &next,
            }
        }

        return true;
    }
}
