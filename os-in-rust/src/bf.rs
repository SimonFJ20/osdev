#[derive(Copy, Clone)]
pub struct BracketPair {
    pub start: usize,
    pub end: usize,
}

#[derive(Debug)]
enum ErrorType {
    InvalidBracketPairError,
}

pub struct Context {
    bracket_index: usize,
    bracket_pairs: [BracketPair; 15],
    stack: [u8; 30000],
    stack_index: usize,
}

impl Context {
    pub fn new() -> Self {
        Self {
            bracket_index: 0,
            bracket_pairs: [BracketPair { start: 0, end: 0 }; 15],
            stack: [0; 30000],
            stack_index: 0,
        }
    }
    fn create_bracket_pairs(&mut self, code: &[u8], start: usize) -> Result<(), ErrorType> {
        let mut nested = 0;
        for (pos, c) in code.iter().enumerate().skip(1 + start) {
            if *c == b'[' {
                nested += 1;
            } else if *c == b']' {
                if nested == 0 {
                    self.bracket_pairs[self.bracket_index] = BracketPair {
                        start: start,
                        end: pos,
                    };
                    self.bracket_index += 1;
                    return Ok(());
                } else {
                    nested -= 1;
                }
            }
        }
        Err(ErrorType::InvalidBracketPairError)
    }
    pub fn generate_bracket_pairs(&mut self, code: &[u8]) {
        for (i, c) in code.iter().enumerate() {
            if *c == b'[' {
                self.create_bracket_pairs(code, i)
                    .expect("unclosed bracket start");
            } else if *c == b']' {
                self.get_bracket_pair(i).expect("unclosed bracket end");
            }
        }
    }
    pub fn get_bracket_pair(&self, pos: usize) -> Option<BracketPair> {
        for i in 0..self.bracket_pairs.len() {
            if self.bracket_pairs[i].start == pos || self.bracket_pairs[i].end == pos {
                return Some(self.bracket_pairs[i]);
            }
        }
        None
    }

    fn incr(&mut self) {
        if self.stack[self.stack_index] == u8::MAX {
            self.stack[self.stack_index] = u8::MIN;
        } else {
            self.stack[self.stack_index] += 1;
        }
    }

    fn decr(&mut self) {
        if self.stack[self.stack_index] == u8::MIN {
            self.stack[self.stack_index] = u8::MAX;
        } else {
            self.stack[self.stack_index] -= 1;
        }
    }

    fn output(&self, vga: &mut crate::vga::VGA) {
        vga.put_char(self.stack[self.stack_index]);
    }

    fn incr_ptr(&mut self) {
        self.stack_index += 1;
    }

    fn decr_ptr(&mut self) {
        if self.stack_index == 0 {
            panic!("pointer went below 0");
        } else {
            self.stack_index -= 1;
        }
    }

    fn handle_loop_start(&mut self, cursor: &mut usize) {
        if self.stack[self.stack_index] == 0 {
            let pair = self
                .get_bracket_pair(*cursor)
                .expect("could not find bracket pair end");
            *cursor = pair.end;
        }
    }

    fn handle_loop_end(&mut self, cursor: &mut usize) {
        if self.stack[self.stack_index] != 0 {
            let pair = self
                .get_bracket_pair(*cursor)
                .expect("could not find bracket pair start");
            *cursor = pair.start;
        }
    }

    pub fn run(&mut self, vga: &mut crate::vga::VGA, code: &[u8]) {
        let mut cursor: usize = 0;
        while cursor < code.len() {
            let c = code
                .iter()
                .nth(cursor)
                .expect("could not get current character");
            match c {
                b'+' => self.incr(),
                b'-' => self.decr(),
                b'.' => self.output(vga),
                //b',' => self.input(),
                b'>' => self.incr_ptr(),
                b'<' => self.decr_ptr(),
                b'[' => self.handle_loop_start(&mut cursor),
                b']' => self.handle_loop_end(&mut cursor),
                _ => {}
            }
            cursor += 1;
        }
    }
}
