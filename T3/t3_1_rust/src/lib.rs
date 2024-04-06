use wasm_bindgen::prelude::*;
use js_sys::Int32Array;

const ROWS: usize = 3;
const COLS: usize = 8;

static mut HOLE: [[i32; COLS]; ROWS] = [[0; COLS]; ROWS];
static mut PLAYER_NOW: i32 = 0;

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
fn mancala_result(flag: usize, seq: &[usize], size: usize) -> i32 {
    unsafe { PLAYER_NOW =flag as i32;}
    let mut player = flag;
    for (index, &item) in seq.iter().enumerate(){
        if index == size{ break;}
        if player != item.div_euclid(10) || is_game_over() || unsafe { HOLE[player][item % 10] } == 0 {
            return 30000 + index as i32;
        }
        if !scatter(player, item % 10) {
            player = player % 2 + 1;
        }
    }
    unsafe { PLAYER_NOW = player as i32;}
    if is_game_over() {
        fetch();
        return 15000 + unsafe { HOLE[flag][7] } - unsafe { HOLE[flag % 2 + 1][7] };
    } else {
        return 20000 + unsafe { HOLE[flag][7] };
    }
}
#[wasm_bindgen]
pub fn mancala_board(flag: i32, seq: &[usize], size: usize) -> js_sys::Int32Array {
    let array = js_sys::Int32Array::new_with_length(15);
    
    unsafe {
        for i in 1..3 {
            for j in 1..7 {
                HOLE[i][j] = 4;
            }
        }
        HOLE[1][7] = 0;
        HOLE[2][7] = 0;
    }
    
    unsafe {
        mancala_result((seq[0].div_euclid(10)) as usize, seq, size - 1);
    }
    
    if unsafe { PLAYER_NOW } == flag
        && !is_game_over()
        && unsafe { HOLE[seq[seq.len() - 1].div_euclid(10)][seq[seq.len() - 1] % 10] } != 0
    {
        unsafe {
            mancala_result(seq[seq.len() - 1].div_euclid(10), &[seq[seq.len() - 1]], 1);
        }
        for m in 1..3 {
            for n in 1..=7 {
                array.set_index(((m - 1) * 7 + (n - 1))as u32, unsafe {HOLE[m][n]});
            }
        }
        if is_game_over() {
            array.set_index(14, 200 + (unsafe { HOLE[1][7] } - unsafe { HOLE[2][7] }));
        } else {
            array.set_index(14, unsafe { PLAYER_NOW });
        }
    } else {
        for m in 1..3 {
            for n in 1..=7 {
                array.set_index(((m - 1) * 7 + (n - 1))as u32, unsafe {HOLE[m][n]});
            }
        }
        if flag == 1 {
            unsafe {
                HOLE[2][7] = 48 - HOLE[1][7];
                for i in 1..7 {
                    HOLE[1][i] = 0;
                    HOLE[2][i] = 0;
                }
                array.set_index(14, 200 + HOLE[1][7] - HOLE[2][7]);
            }
        } else {
            unsafe {
                HOLE[1][7] = 48 - HOLE[2][7];
                for i in 1..7 {
                    HOLE[1][i] = 0;
                    HOLE[2][i] = 0;
                }
                array.set_index(14, 200 + HOLE[1][7] - HOLE[2][7]);
            }
        }
    }
    array
}



#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn my_test1() {
        assert_eq!(mancala_board(2, &[13,16,26,12,16,11,22,25,13,16,15,21,16,14,25,22,16,15,24,16,14,23,15,21,16,14,24,12,26,13,16,15,16,14,16,15,25,16,11,26], 40), 
    [0,0,0,0,0,0,36,0,0,0,0,0,0,12,224]);
    }

    #[test]
    fn my_test2() {
        assert_eq!(mancala_board(1, &[13,16,26,12,16,11,22,25,13,16,15,21,16,14,25,22,16,15,24,16,14,23,15,21,16,14,24,12,26,13,16,15,16,14,16,15,25,16,11,11], 40), 
    [0,2,1,1,1,1,30,0,0,0,0,0,1,11,212]);
    }
    #[test]
    fn my_test3() {
        assert_eq!(mancala_board(2, &[13,14,22], 3),[0,2,1,1,1,1,30,0,0,0,0,0,1,11,212]);

    }

}