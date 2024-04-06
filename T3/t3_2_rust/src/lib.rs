use wasm_bindgen::prelude::*;

const SIZE: usize = 3;
const LENGTH: usize = 8;

static mut HOLE: [[i32; LENGTH]; SIZE] = [[0; LENGTH]; SIZE];
static mut PLAYER_NOW: i32 = 0;
static mut ME: i32 = 0;

fn scatter(player: i32, start: usize) -> i32 {
    let mut tmp_player = player;
    unsafe {
        let mut cnt = HOLE[player as usize][start]; // Number of chess pieces
        HOLE[player as usize][start] = 0;
        let mut loc = start + 1;
        while cnt > 0 {
            if loc >= LENGTH {
                tmp_player = tmp_player % 2 + 1;
                loc -= 7;
            }
            if loc == 7 && player != tmp_player {
                // do nothing
                cnt += 1;
            } else {
                if loc != 7 && cnt == 0 && tmp_player == player && HOLE[tmp_player as usize][loc] == 0 && HOLE[(tmp_player % 2 + 1) as usize][7 - loc] != 0 {
                    HOLE[player as usize][7] += HOLE[(tmp_player % 2 + 1) as usize][7 - loc] + 1;
                    HOLE[(tmp_player % 2 + 1) as usize][7 - loc] = 0;
                } else {
                    HOLE[tmp_player as usize][loc] += 1;
                }
            }
            loc += 1;
            cnt -= 1;
        }
        if loc == LENGTH && player == tmp_player {
            return 1;
        }
        return 0;
    }
}

fn is_game_over() -> i32 {
    unsafe {
        for i in 1..SIZE {
            let mut cnt = 0;
            for j in 1..LENGTH - 1 {
                cnt += HOLE[i][j];
            }
            if cnt == 0 {
                return 1;
            }
        }
        return 0;
    }
}

fn fetch() {
    unsafe {
        let mut cnt1 = 0;
        let mut cnt2 = 0;
        for j in 1..LENGTH - 1 {
            cnt1 += HOLE[1][j];
            cnt2 += HOLE[2][j];
        }

        if cnt1 == 0 {
            HOLE[2][7] += cnt2;
            for i in 0..=6 {
                HOLE[2][i] = 0;
            }
        }
        if cnt2 == 0 {
            HOLE[1][7] += cnt1;
            for i in 0..=6 {
                HOLE[1][i] = 0;
            }
        }
    }
}

struct Node {
    child: Vec<Option<Box<Node>>>,
    value: i32,
    flag: i32,
    id: i32,
}

fn dfs(root: &mut Node, step: i32) -> i32 {
    if is_game_over() != 0 || step == 8 {
        unsafe {
            if is_game_over() != 0 {
                fetch();
            }
            root.value = HOLE[ME as usize][7] - HOLE[ME as usize % 2 + 1][7];
            return HOLE[ME as usize][7] - HOLE[ME as usize % 2 + 1][7];
        }
    }
    let mut max_value = -48;
    let mut flag = root.flag;
    unsafe {
        if root.id > 0 {
            if scatter(root.id / 10, (root.id % 10) as usize) == 0 {
                flag = flag % 2 + 1;
            }
        }
        root.child[0] = None;
        for i in 1..LENGTH - 1 {
            if HOLE[flag as usize][i] == 0 {
                root.child[i] = None;
                continue;
            }
            root.child[i] = Some(Box::new(Node {
                child: Vec::with_capacity(7),
                value: 0,
                flag: flag,
                id: flag * 10 + i as i32,
            }));
            if let Some(child_node) = &mut root.child[i] {
                for _ in 0..7 {
                    child_node.child.push(None);
                }
            }
            let mut tmp = [[0; LENGTH]; SIZE];
            for m in 1..SIZE {
                for n in 1..LENGTH {
                    tmp[m][n] = HOLE[m][n];
                }
            }
            let tmps = dfs(root.child[i].as_mut().unwrap(), step + 1);
            if max_value < tmps {
                max_value = tmps;
            }
            for m in 1..SIZE {
                for n in 1..LENGTH {
                    HOLE[m][n] = tmp[m][n];
                }
            }
        }
        root.value = max_value;
        return root.value;
    }
}

#[wasm_bindgen]
pub fn mancala_operator(flag: i32, status: &[i32]) -> i32 {
    let mut root = Node {
        child: Vec::with_capacity(7),
        value: 0,
        flag: flag,
        id: -1,
    };
    for _ in 0..7 {
        root.child.push(None);
    }
    let mut cnt = 0;
    unsafe {
        ME = flag;
        for j in 1..3 {
            for i in 1..=7 {
                HOLE[j][i] = status[cnt];
                cnt += 1;
            }
        }
    }
    let mut max_value = -1;
    max_value = dfs(&mut root, 0);
    for i in 1..7 {
        if let Some(child) = &root.child[i] {
            // 使用 if let 检查 Option 是否为 Some，并获取指向 Child 结构体的引用
            if max_value == child.value {
                return flag * 10 + i as i32;
            }
        }
    }
    
    return -1;
}



#[cfg(test)]
mod tests {
    use super::*;

    // #[test]
    // fn my_test1() {
    //     assert_eq!(mancala_operator(1, &[4,4,4,4,0,4,0,4,4,4,4,0,4,8]),3);
    // }
    #[test]
    fn my_test2() {
        assert_eq!(mancala_operator(1, &[4, 4, 0, 5, 5, 5,1, 4, 4, 4, 4, 4,4, 0]),3);
    }

}