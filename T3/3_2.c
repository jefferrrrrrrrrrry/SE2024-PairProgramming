#include <stdio.h>
#include <stdlib.h>
#include <time.h>

 
int hole[3][8];
int player_now;
int me;
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
struct node{
    struct node * child[7];
    int value;//净胜棋数
    int flag;//上一步的先手
    int id;//11,12,13,14,15,16,21,22,23,24,25,26
};

int dfs(struct node *root,int step){//返回当前节点的最大净胜棋子数
    if(isGameOver()||step==9){
        if(isGameOver())fetch();
        root->value=hole[me][7]-hole[me % 2 + 1][7];
        return hole[me][7]-hole[me % 2 + 1][7];
    }
    int max_value=-48;
    int flag = root->flag;
    if(root->id>0){
        if(!scatter(root->id/10,root->id%10)){
            flag = flag % 2 + 1;
        }
        
    }
    root->child[0]=NULL;
    for(int i=1;i<7;i++){
        if(hole[flag][i]==0){
            root->child[i]=NULL;
            continue;
        }
        root->child[i] = (struct node *)malloc(sizeof(struct node));
        for(int j=0;j<7;j++){
            root->child[i]->child[j]=NULL;
        }
        root->child[i]->id=flag*10+i;
        root->child[i]->flag=flag;
        //printf("%d\n",root->child[i]->id);
        int tmp[3][8]={0};
        for(int m=1;m<3;m++){
            for(int n=1;n<8;n++){
                tmp[m][n]=hole[m][n];
            }
        }
        int tmps=dfs(root->child[i],step+1);
        if(max_value<tmps)max_value=tmps;
        for(int m=1;m<3;m++){
            for(int n=1;n<8;n++){
                hole[m][n]=tmp[m][n];
            }
        }
    }
    
    root->value=max_value;
    return root->value;
}
// void dfs_print(struct node * a){
//     if(a==NULL){
//         return ;
//     }
//     printf("%d\n",a->value);
//     for(int i=1;i<7;i++){
//         dfs_print(a->child[i]);
//     }
// }
int mancalaOperator(int flag,int status[]){
    struct node root;
    int cnt=0;
    me=flag;
    for (int j=1;j<3;j++){
        for(int i=1;i<=7;i++){
            hole[j][i]=status[cnt++];
        }
    }
    root.flag=flag;
    root.id=-1;
    int max_value=-1;
    max_value=dfs(&root,0);
    for(int i=1;i<7;i++){
        if(max_value==root.child[i]->value)return i;
    }
    return -1;
}
int main(){
    int a[14]={4,4,4,4,0,4,0,4,4,4,4,0,4,8};
    
    clock_t start, end;
    double cpu_time_used;

    start = clock();
    printf("%d\n",mancalaOperator(1,a));
    end = clock();

    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("time:%f second\n", cpu_time_used);
    return 0;
}
