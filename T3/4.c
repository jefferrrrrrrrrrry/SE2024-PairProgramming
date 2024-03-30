#include <stdio.h>
#include <stdlib.h>

int hole[3][8];
int player;
int path[6];


int isGameOver(int tmp[][8]){
    for(int i=1;i<3;i++){
        int cnt=0;
        for(int j=1;j<7;j++){
            cnt+=tmp[i][j];
        }
        if(cnt==0)return 1;
    }
    return 0;
}

int a_b_search(int tmp[][8], int depth, int alpha, int beta, int flag) {
    if (depth == 0 || isGameOver(tmp)) {
        return tmp[flag][7] - tmp[flag % 2 + 1][7];
    }

    if (flag == player) { //更好
        
    } else { //更差

    }
}


int mancalaOperator(int flag, int status[]) {
    player = flag;
    int best_value = -48;
    int alpha = -48;
    int beta = 48;
    int depth = 6;
    int cnt=0;
    for (int j=1;j<3;j++){
        for(int i=1;i<=7;i++){
            hole[j][i]=status[cnt++];
        }
    }

    for (int i = 1;i < 7;i++) {
        int tmp[3][7];
        int value = a_b_search(tmp, depth - 1, alpha, beta, flag);
        if (value > best_value) {
            best_value = value;
        }
    }

}