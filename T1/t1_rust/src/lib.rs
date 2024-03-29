use wasm_bindgen::prelude::*;

#[wasm_bindgen]
pub fn bocchi_shut_up(flag: i32, seq: &[i32], size: i32) -> i32 {
    let mut freq = [0; 6]; 
    let mut max_freq = 0;  
    let mut cnt = 0;
    let mut value = -1;

    if flag==1{
        for &num in seq {
            if (11..=16).contains(&num) {
                freq[(num-11) as usize] += 1;
                max_freq = max_freq.max(freq[(num-11) as usize]);
            }
        }
    }else{
        for &num in seq {
            if (21..=26).contains(&num) {
                freq[(num-21) as usize] += 1;
                max_freq = max_freq.max(freq[(num-21) as usize]);
            }
        }
    }

    for (i, &f) in freq.iter().enumerate() {
        if f == max_freq {
            cnt += 1;
            if flag == 1 {
                value = i as i32 + 11;
            }else{
                value = i as i32 + 21;
            }
            
        }
    }

    if cnt == 1 {
        value
    } else {
        10
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn bocchi_shut_up_test_when_10() {
        assert_eq!(bocchi_shut_up(1, &[11,12,13], 3), 10);
    }

    #[test]
    fn bocchi_shut_up_test_when_not_10() {
        assert_eq!(bocchi_shut_up(1, &[11,12,12], 3), 12);
    }
}