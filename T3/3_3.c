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
    struct node * father;
    struct node * child[7];
    int value;//净胜棋数
    int flag;//上一步的先手
    int id;//11,12,13,14,15,16,21,22,23,24,25,26
    int hole_current[3][8];
    int depth;
};
// struct element{
//     struct node * value;
//     struct element * next;
// };
// struct element *queue=NULL;
// struct element * head=NULL,*tail=NULL;
// struct node * pop(){
//     if(head==tail)return NULL;
//     struct element *q=head;
//     head=head->next;
//     struct node * value=q->value;
//     free(q);
//     return value;
// }
// void push(struct node * value){
//     struct element *q=(struct element *)malloc(sizeof(struct element));
//     q->value=value;
//     q->next=NULL;
//     tail->next=q;
//     tail=q;
// }
int max_value=-1;
struct node * max_node=NULL;
struct QueueNode {
    struct node* data;
    struct QueueNode* next;
};

// 定义队列结构
struct Queue {
    struct QueueNode* front;
    struct QueueNode* rear;
};

// 创建一个新的队列节点
struct QueueNode* createQueueNode(struct node* data) {
    struct QueueNode* newNode = (struct QueueNode*)malloc(sizeof(struct QueueNode));
    if (newNode == NULL) {
        printf("Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

// 初始化队列
struct Queue* initializeQueue() {
    struct Queue* queue = (struct Queue*)malloc(sizeof(struct Queue));
    if (queue == NULL) {
        printf("Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    queue->front = NULL;
    queue->rear = NULL;
    return queue;
}

// 入队
void enqueue(struct Queue* queue, struct node* data) {
    struct QueueNode* newNode = createQueueNode(data);
    if (queue->rear == NULL) {
        queue->front = newNode;
        queue->rear = newNode;
    } else {
        queue->rear->next = newNode;
        queue->rear = newNode;
    }
}

// 出队
struct node* dequeue(struct Queue* queue) {
    struct QueueNode* temp = queue->front;
    struct node* data = temp->data;
    queue->front = queue->front->next;
    if (queue->front == NULL) {
        queue->rear = NULL;
    }
    free(temp);
    return data;
}

void bfs(struct node* root) {
    struct Queue* queue = initializeQueue();
    
    // 将根节点入队
    enqueue(queue, root);
    
    while (queue->front != NULL) {
        // 出队一个节点
        struct node* current = dequeue(queue);
        for (int m = 1; m <3; m++) {
                for (int n = 1; n <=7; n++) {
                    hole[m][n]=current->hole_current[m][n];
                }
            }
        // 如果游戏结束或者达到最大步数，返回当前节点的值
        if (isGameOver() ||current->depth == 9) {
            if (isGameOver()) fetch();
            current->value = hole[me][7] - hole[me % 2 + 1][7];
            if(current->value>max_value){
                max_value=current->value;
                max_node=current;
            }
            continue;
        }
        int flag = current->flag;
        
        if (current->id > 0) {
            if (!scatter(current->id / 10, current->id % 10)) {
                flag = flag % 2 + 1;
            }
        }
        if(hole[me][7] - hole[me % 2 + 1][7]<-10){
            free(current);
            continue;
        }
        current->value = hole[me][7] - hole[me % 2 + 1][7];
        // for(int i=6;i>=1;i--)printf("%d ",hole[2][i]);printf("\n");
        // printf("  ");
        // for(int i=1;i<7;i++)printf("%d ",hole[1][i]);printf("\n");
        //printf("%d\n",hole[me][7] - hole[me % 2 + 1][7]);
        if(current->value>max_value){
            max_value=current->value;
            max_node=current;
        }
        // 计算当前节点的子节点，并入队
        for (int i = 1; i < 7; i++) {
            if (hole[flag][i] == 0) {
                current->child[i] = NULL;
                continue;
            }
            current->child[i] = (struct node*)malloc(sizeof(struct node));
            current->child[i]->father = current;
            current->child[i]->id = flag * 10 + i;
            current->child[i]->flag = flag;
            current->child[i]->depth=current->depth+1;
            for (int m = 1; m <3; m++) {
                for (int n = 1; n <=7; n++) {
                    current->child[i]->hole_current[m][n] = hole[m][n];
                }
            }
            // 将子节点入队
            enqueue(queue, current->child[i]);
        }
    }
    
}

void dfs_print(struct node * a){
    if(a==NULL){
        return ;
    }
    printf("%d\n",a->value);
    for(int i=1;i<7;i++){
        dfs_print(a->child[i]);
    }
}
int mancalaOperator(int flag,int status[]){
    struct node root;
    int cnt=0;
    me=flag;
    for (int j=1;j<3;j++){
        for(int i=1;i<=7;i++){
            root.hole_current[j][i]=status[cnt++];
        }
    }
    root.flag=flag;
    root.id=-1;
    root.depth=0;
    root.father=NULL;
    bfs(&root);
    //dfs_print(&root);
    while(max_node->father!=&root){
        max_node=max_node->father;
    }
    printf("%d\n",max_value);
    for(int i=1;i<7;i++){
        if(max_node==root.child[i])return i;
    }
    return -1;
}
int main(){
    int a[14]={4,4,4,4,4,4,0,4,4,4,4,4,4,0};
    
    clock_t start, end;
    double cpu_time_used;

    start = clock();
    printf("%d\n",mancalaOperator(1,a));
    end = clock();

    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("time:%f second\n", cpu_time_used);
    return 0;
}
