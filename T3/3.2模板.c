#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#define max(a,b) ((a)<(b)?(b):(a))
#define min(a,b) ((a)>(b)?(b):(a))
// 定义状态结构体
typedef struct {
    int value;  // 状态的值
} State;

// 判断是否到达叶子节点或者游戏结束
int is_terminal(State state) {
    // 在这个示例中，我们假设游戏结束条件是状态值大于等于 100 或者小于等于 -100
    return (state.value >= 100 || state.value <= -100);
}

// 评估给定状态的价值
int evaluate(State state) {
    return state.value;  // 在这个示例中，我们简单地返回状态的值作为评估值
}

// Alpha-Beta 剪枝搜索函数
int alpha_beta_search(State state, int depth, int alpha, int beta, int maximizing_player) {
    if (depth == 0 || is_terminal(state)) { // 如果达到搜索深度或游戏结束，则返回状态的评估值
        return evaluate(state);
    }

    if (maximizing_player) { // 如果是最大化玩家的回合
        int value = INT_MIN; // 初始化当前状态的值为负无穷
        for (int i = 1; i <= 3; i++) {  // 假设每个状态有 3 个可能的子状态

        
            State child_state = {state.value + i};  // 生成子状态
            value = max(value, alpha_beta_search(child_state, depth - 1, alpha, beta, 0)); // 递归搜索子状态的值
            alpha = max(alpha, value); // 更新 alpha 值
            if (beta <= alpha) { // 如果 beta 小于等于 alpha，则进行 Beta 剪枝
                break;
            }
        }
        return value; // 返回当前状态的值
    } else { // 如果是最小化玩家的回合
        int value = INT_MAX; // 初始化当前状态的值为正无穷
        for (int i = 1; i <= 3; i++) {  // 假设每个状态有 3 个可能的子状态
            State child_state = {state.value - i};  // 生成子状态
            value = min(value, alpha_beta_search(child_state, depth - 1, alpha, beta, 1)); // 递归搜索子状态的值
            beta = min(beta, value); // 更新 beta 值
            if (beta <= alpha) { // 如果 beta 小于等于 alpha，则进行 Alpha 剪枝
                break;
            }
        }
        return value; // 返回当前状态的值
    }
}

// 测试主函数
int main() {
    State initial_state = {0};  // 初始状态的值为 0
    int depth = 3;  // 设置搜索深度为 3
    int alpha = INT_MIN; // 初始化 alpha 值为负无穷
    int beta = INT_MAX; // 初始化 beta 值为正无穷
    int maximizing_player = 1; // 最大化玩家

    // 调用 Alpha-Beta 剪枝搜索函数
    int result = alpha_beta_search(initial_state, depth, alpha, beta, maximizing_player); // 获取最佳状态值
    printf("Best value found: %d\n", result); // 输出最佳状态值

    return 0;
}
