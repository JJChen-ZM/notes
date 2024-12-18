## n 皇后问题 研究的是如何将 n 个皇后放置在 n × n 的棋盘上，并且使皇后彼此之间不能相互攻击。给你一个整数 n ，返回 n 皇后问题 不同的解决方案的数量。

要解决 \( n \)-皇后问题，并返回不同的解决方案的数量，可以使用回溯算法。这种方法通过逐行放置皇后，并检查每次放置是否满足条件（即皇后之间互不攻击）。

---

### 思路

1. **棋盘规则**：
   - 皇后不能放在同一行、同一列或对角线（主对角线与副对角线）上。
   - 用三个辅助数组记录已被占用的列、主对角线和副对角线。
     - 列数组：`cols`，记录每列是否有皇后。
     - 主对角线数组：`main_diag`，主对角线索引为 \( \text{row} - \text{col} + (n - 1) \)。
     - 副对角线数组：`anti_diag`，副对角线索引为 \( \text{row} + \text{col} \)。

2. **回溯算法**：
   - 从第 0 行开始尝试放置皇后。
   - 对于当前行的每一列，检查是否安全放置皇后。
   - 如果安全，则更新辅助数组并递归尝试放置下一行。
   - 如果无法放置，回溯并尝试其他位置。

3. **终止条件**：
   - 当成功放置 \( n \) 个皇后时，计数器加 1。

---

### 实现代码

```cpp
#include <iostream>
#include <vector>

class NQueens {
public:
    int totalNQueens(int n) {
        // 初始化计数器和辅助数组
        int count = 0;
        std::vector<bool> cols(n, false);           // 列是否被占用
        std::vector<bool> main_diag(2 * n - 1, false); // 主对角线是否被占用
        std::vector<bool> anti_diag(2 * n - 1, false); // 副对角线是否被占用

        // 开始回溯
        backtrack(0, n, cols, main_diag, anti_diag, count);

        return count;
    }

private:
    void backtrack(int row, int n, std::vector<bool>& cols,
                   std::vector<bool>& main_diag, std::vector<bool>& anti_diag,
                   int& count) {
        if (row == n) {
            // 成功放置 n 个皇后，找到一个解
            count++;
            return;
        }

        for (int col = 0; col < n; ++col) {
            // 计算当前格子的主对角线和副对角线索引
            int main_diag_idx = row - col + (n - 1);
            int anti_diag_idx = row + col;

            // 检查当前格子是否可以放置皇后
            if (cols[col] || main_diag[main_diag_idx] || anti_diag[anti_diag_idx]) {
                continue;
            }

            // 做选择：放置皇后
            cols[col] = main_diag[main_diag_idx] = anti_diag[anti_diag_idx] = true;

            // 递归到下一行
            backtrack(row + 1, n, cols, main_diag, anti_diag, count);

            // 撤销选择：移除皇后
            cols[col] = main_diag[main_diag_idx] = anti_diag[anti_diag_idx] = false;
        }
    }
};

int main() {
    NQueens solution;
    int n = 8; // 示例输入
    int result = solution.totalNQueens(n);
    std::cout << "Total solutions for " << n << "-Queens: " << result << std::endl;
    return 0;
}
```

---

### 示例运行

输入：
```cpp
n = 8
```

输出：
```
Total solutions for 8-Queens: 92
```

---

### 代码解析

1. **辅助数组**：
   - `cols`：记录每列是否被占用。
   - `main_diag`：记录主对角线是否被占用，索引为 \( \text{row} - \text{col} + (n - 1) \)。
   - `anti_diag`：记录副对角线是否被占用，索引为 \( \text{row} + \text{col} \)。

2. **回溯过程**：
   - 遍历每行的所有列，尝试放置皇后。
   - 如果当前位置不可用，则跳过。
   - 如果可用，则放置皇后并递归尝试下一行。

3. **终止条件**：
   - 当所有 \( n \) 行都放置成功时，计数器加 1。

---

### 时间复杂度
- **时间复杂度**: \( O(n!) \)（最坏情况下，逐行尝试所有可能）。
- **空间复杂度**: \( O(n) \)，用于辅助数组的存储。

---

### 优化点
1. **位运算优化**：可以用位运算替代辅助数组，提高效率。
2. **提前剪枝**：在放置皇后时，可以进一步优化冲突检测逻辑。