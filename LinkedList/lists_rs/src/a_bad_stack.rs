#[derive(Debug)]
pub struct List<T> {
    head: Link<T>,
}

#[derive(Debug)]
struct Node<T> {
    elem: T,
    next: Link<T>,
}

#[derive(Debug)]
enum Link<T> {
    Empty,
    More(Box<Node<T>>),
}

// impl<T> Link<T> {
//     fn new(val: T) -> Self {
//         Self::Occupied(Box::new(Node {}))
//     }
// }
impl<T> Link<T> {
    fn pop_node(&mut self) -> Link<T> {
        std::mem::replace(self, Link::Empty)
    }
}

impl<T> List<T> {
    pub fn new() -> Self {
        Self { head: Link::Empty }
    }
    pub fn push(&mut self, val: T) {
        let node = Box::new(Node {
            elem: val,
            next: std::mem::replace(&mut self.head, Link::Empty),
        });
        self.head = Link::More(node);
    }

    pub fn pop(&mut self) -> Option<T> {
        // head -> next
        // head = next
        // drop head;
        match self.head.pop_node() {
            Link::Empty => None,
            Link::More(node) => {
                self.head = node.next;
                Some(node.elem)
            }
        }
    }
}

// implement custom drop function to make it tail recursive instread of default stack saving recursion
impl<T> Drop for List<T> {
    fn drop(&mut self) {
        let mut cur_link = self.head.pop_node();

        while let Link::More(mut boxed_node) = cur_link {
            cur_link = boxed_node.next.pop_node();
            // boxed_node gets out of the scope and dropped here
            // but its next has been set to Empty
            // so no unbounded recursion occurs
        }
    }
}

#[cfg(test)]
mod tests {
    use super::List;

    #[test]
    fn first() {
        let mut list = List::new();
        const N: usize = 20;
        for i in 0..N {
            list.push(i);
        }

        for i in (0..N).rev() {
            assert!(list.pop().is_some_and(|x| x == i));
        }

        assert_eq!(list.pop(), None);
    }
}
