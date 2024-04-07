use wasm_bindgen::prelude::*;

const ROWS: usize = 3;
const COLS: usize = 8;

static mut HOLE: [[i32; COLS]; ROWS] = [[0; COLS]; ROWS];

fn scatter(player: usize, start: usize) -> bool {
    let mut tmp_player = player;
    let mut cnt = unsafe { HOLE[player][start] };
    unsafe { HOLE[player][start] = 0; }
    let mut loc = start + 1;
    while cnt > 0 {
        cnt -= 1;
        if loc >= 8 {
            tmp_player = tmp_player % 2 + 1;
            loc -= 7;
        }
        if loc == 7 && player != tmp_player {
            // do nothing
            cnt += 1;
        } else {
            if loc != 7 && cnt == 0 &&
                tmp_player == player &&
                unsafe { HOLE[tmp_player][loc] } == 0 &&
                unsafe { HOLE[tmp_player % 2 + 1][7 - loc] } != 0 {
                unsafe {
                    HOLE[player][7] += unsafe { HOLE[tmp_player % 2 + 1][7 - loc] } + 1;
                    HOLE[tmp_player % 2 + 1][7 - loc] = 0;
                }
            } else {
                unsafe {
                    HOLE[tmp_player][loc] += 1;
                }
            }
        }
        loc += 1;
        
    }
    if loc == 8 && player == tmp_player {
        return true;
    }
    false
}

fn is_game_over() -> bool {
    for i in 1..3 {
        let mut cnt = 0;
        for j in 1..7 {
            cnt += unsafe { HOLE[i][j] };
        }
        if cnt == 0 {
            return true;
        }
    }
    false
}

fn fetch() {
    let mut cnt1 = 0;
    let mut cnt2 = 0;
    for j in 1..COLS - 1 {
        cnt1 += unsafe { HOLE[1][j] };
        cnt2 += unsafe { HOLE[2][j] };
    }

    if cnt1 == 0 {
        unsafe {
            HOLE[2][7] += cnt2;
            for i in 0..=6 {
                HOLE[2][i] = 0;
            }
        }
    }
    if cnt2 == 0 {
        unsafe {
            HOLE[1][7] += cnt1;
            for i in 0..=6 {
                HOLE[1][i] = 0;
            }
        }
    }
}
#[wasm_bindgen]
pub fn mancala_result(flag: usize, seq: &[usize], size: usize) -> i32 {
    unsafe {
        for i in 1..3 {
            for j in 1..7 {
                HOLE[i][j] = 4;
            }
        }
        HOLE[1][7] = 0;
        HOLE[2][7] = 0;
    }
    let mut player = flag;
    for (index, &item) in seq.iter().enumerate(){
        if player != item.div_euclid(10) || is_game_over() || unsafe { HOLE[player][item % 10] } == 0 {
            return 30000 + index as i32;
        }
        if !scatter(player, item % 10) {
            player = player % 2 + 1;
        }
    }
    if is_game_over() {
        fetch();
        return 15000 + unsafe { HOLE[flag][7] } - unsafe { HOLE[flag % 2 + 1][7] };
    } else {
        return 20000 + unsafe { HOLE[flag][7] };
    }
}
#[cfg(test)]
mod tests {
    use super::*;
    #[test]//取子测试
    fn my_test_quzi() {
        assert_eq!(mancala_result(1, &[16,23,11], 3), 20007);
    }
    #[test]//再次行动测试
    fn my_test_again() {
        assert_eq!(mancala_result(1, &[13], 1), 20001);
    }
    #[test]//游戏结束测试
    fn my_test_all() {
        assert_eq!(mancala_result(1,&[13,11, 23, 21, 12, 14, 22, 11, 26,16, 26, 23, 26, 21, 12, 22, 
            11, 23,13, 15, 23, 14, 26, 21, 16, 21, 11,22, 12], 29), 15028);
    }
    
    #[test]//混合测试+游戏结束
    fn my_test_all() {
        assert_eq!(mancala_result(1, &[13,16,26,12,16,11,22,25,13,16,15,21,16,14,25,22,16,15,24,
            16,14,23,15,21,16,14,24,12,26,13,16,15,16,14,16,15,25,16,11,26], 40), 15024);
    } 

}