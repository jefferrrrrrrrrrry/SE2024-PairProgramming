#include <stdio.h>
int hole[3][8];
int player_now;
//7 6 5 4 3 2 1 7 
//7 1 2 3 4 5 6 7
int scatter(int player,int start){
    int tmp_player=player;
    int cnt=hole[player][start];//棋子数
    hole[player][start]=0;
    int loc=start+1;
    while(cnt--){
        if(loc >= 8){
            tmp_player = tmp_player % 2 + 1;
            loc-= 7;
        }
        if(loc==7&&player!=tmp_player){
            //do nothing
            cnt++;
        }else{
            if(loc !=7  && cnt==0   &&
               tmp_player==player   && 
               hole[tmp_player][loc]==0&&
               hole[tmp_player%2+1][7-loc]!=0){
                hole[player][7]+=(hole[tmp_player%2+1][7-loc]+1);
                hole[tmp_player%2+1][7-loc]=0;
                                                                                                            
            }else{
                hole[tmp_player][loc]+=1;
                
            }
        }
        loc++;
    }
    if(loc==8&&player==tmp_player){
        return 1;
    }
    return 0;
}
int isGameOver(){
    for(int i=1;i<3;i++){
        int cnt=0;
        for(int j=1;j<7;j++){
            cnt+=hole[i][j];
        }
        if(cnt==0)return 1;
    }
    return 0;
}
void fetch(){
    int cnt1=0,cnt2=0;
    for(int j=1;j<7;j++){
        cnt1+=hole[1][j];
        cnt2+=hole[2][j];
    }

    if(cnt1==0){
        hole[2][7]+=cnt2;
        for(int i=0;i<=6;i++){
            hole[2][i]=0;
        }
    }
    if(cnt2==0){
        hole[1][7]+=cnt1;
        for(int i=0;i<=6;i++){
            hole[1][i]=0;
        }
    }
}
int mancalaResult(int flag,int seq[],int size){
    player_now=flag;
    for(int i=0;i<size;i++){
        int item=seq[i];
        if(player_now!=(item/10)||isGameOver()||hole[player_now][item%10]==0){
            //printf("error:\nplayer:%d\ngameover:%d,null:%d\n",player,isGameOver(),hole[player][item%10]);
            return 30000+i;
        }
        if(!scatter(player_now,item%10)){
            player_now = player_now % 2 + 1;
        }
    }
    if(isGameOver()){
        fetch();
        return 15000+hole[flag][7]-hole[flag % 2 + 1][7];
    }else{
        return 20000+hole[flag][7];
    }

}
int * mancalaBoard(int flag,int seq[],int size){
    int a[3]={13,14,22};
    for(int i=1;i<3;i++){
        for(int j=1;j<7;j++){
            hole[i][j]=4;
        }
    }
    hole[1][7]=0;
    hole[2][7]=0;
    static int ans[15]={0};
    mancalaResult(seq[0]/10,seq,size-1);//前n-1步
    if(player_now==flag&&flag==seq[size-1]/10&&!isGameOver()&&hole[seq[size-1]/10][seq[size-1]%10]!=0){
        int last[1]={seq[size-1]};
        mancalaResult(last[0]/10,last,1);
        int i=0;
        for(int m=1;m<3;m++){
            for(int n=1;n<=7;n++){
                ans[i++]=hole[m][n];
            }
        }
        if(isGameOver()){
            ans[14]=200+(hole[1][7]-hole[2][7]);
        }else{
            ans[14]=player_now;
        }
    }else{
        int i=0;
        for(int m=1;m<3;m++){
            for(int n=1;n<=7;n++){
                ans[i++]=hole[m][n];
            }
        }

        if(flag==1){
            hole[2][7]=48-hole[1][7];
            for(int i=1;i<7;i++){
                hole[1][i]=0;
                hole[2][i]=0;
            }
            ans[14]=200+hole[1][7]-hole[2][7];
        }else{
            hole[1][7]=48-hole[2][7];
            for(int i=1;i<7;i++){
                hole[1][i]=0;
                hole[2][i]=0;
            }
            ans[14]=200+hole[1][7]-hole[2][7];
        }
    }
    return ans;
}
int main(){
    
    
    int *b=mancalaBoard(2,a,3);
    for(int i=0;i<15;i++){
        printf("%d,",b[i]);
    }
    return 0;
}