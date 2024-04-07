# 结对项目：博客问题清单

请将本文件**在代码仓库外复制一份**，一边阅读和完成结对项目、一边填写入**代码仓库外的版本**，或采取简记、语音备忘等方式记载较复杂问题的要点之后再补充。**请不要将本文档内的作答提交到代码仓库。**

## Chapter.1 缠结的字节码（Bytecode Complex）

#### 引入

###### **→ 📖 Q1.1(P) 请记录下目前的时间。**

2024.3.29 19：00

###### **→ 📖 Q1.2(P) 根据之前学习的专业课和各类开发经验，回答：为什么原生应用相较 Web app 更有快的印象？这样的“速度”又是如何达成的？**

- 我们认为，原生应用是面向需求开发的，对于Web App更加“定制化”，可以充分利用设备的硬件资源，更改底层实现代码，比如使用静态编译或即时编译、本地存储等来实现运行速度的提升。
- 经调研，原生应用通过充分利用设备硬件资源、静态编译、本地存储以及定制化的 UI/UX 设计等技术手段，实现了更高效的性能和更快速的用户体验。

###### **→ 📖 Q1.3(P) “要让 Web 生态、或者说一种雄心勃勃的软件运行环境的未来，受益于既有的‘原生’软件开发模式、技术栈和工具链，我们还缺少什么？”——你们的答案是？**

实现一种编码，使得既有的‘原生’软件开发模式、技术栈和工具链生产的代码，通过统一的编码，运行在Web 生态之上，并且保持运行在原生应用之上的性能。

#### 结对过程

###### **→ 📖 Q1.4(I) 作为本项目的调查：请如实标注在开始项目之前对 Wasm 的熟悉程度分级，可以的话请细化具体的情况。**

> I. 没有听说过；

> II. 仅限于听说过相关名词；

> III. 听说过，且有一定了解；

> IV. 听说过，且使用 Wasm 实际进行过开发（即便是玩具项目的开发）。

I. 没有听说过；

###### **→ 📖 Q1.5(P) 请在完成任务的同时记录，并在完成任务后整理完善：**

> 完成编程任务期间，你和你的搭档依次做了什么，比如：
>
> 1. 查阅了什么资料；
>2. 如何进行了开发；
> 3. 遇到了什么问题，又通过什么方式解决。

1. - 查阅了如何将C/C++模块编译为WebAssembly的官方文档https://developer.mozilla.org/en-US/docs
   - 查阅了[知乎](https://zhuanlan.zhihu.com/p/655814356)，[stackoverflow](https://stackoverflow.com/questions/68266094/calling-and-wrapping-c-function-in-nodejs-emscripten)中关于如何编译的实例
   - 如何配置rust语言环境，[rust官方文档](https://www.rust-lang.org/)
2. - 两人分析题目
   - 一人负责编码，另一人负责梳理思路和质疑
   - 从C语言开始编码，在编译失败后尝试将C语言翻译为rust
3. - rust内有多个测试点的情况会并发执行，有概率出现结果不一致的情况	如何解决：先查阅资料发现了这一事实，然后每次只执行一个测试点
   - C语言转rust时有很大区别，比如全局变量静态数组，我们查阅了相关资料，使用unsafe块包裹所有对全局变量的修改

#### 编程语言选择

##### Rust

###### **→ 📖 Q1.6.R.1(P) 浏览 `Cargo.toml`，请问第 9 行的设置项中： `crate-type="cdylib"` 的作用是？可以从提供的参考文档当中寻找答案。**

这个设置使得编译器在构建 Rust 项目时，会生成一个动态链接库文件，而不是一个可执行文件或者静态库，允许代码在运行时动态加载。

###### **→ 📖 Q1.6.R.2(P) 浏览 `lib.rs`，请问第 3 行的属性（Attribute）注解`#[wasm_bindgen]` 的作用是？请尝试删除掉这一注解重新运行上面这条 `wasm-pack` 的编译和打包指令，检查删除前后 `/pkg` 内生成的文件发生的变化；并请参考提供的参考文档完善答案。**

`#[wasm_bindgen]` 是一个属性（attribute），用于标记 Rust 函数和类型，指示编译器生成与 JavaScript 可以相互调用的 WebAssembly 绑定。它来自于 `wasm-bindgen` crate，是 Rust 与 WebAssembly 交互的关键部分。

具体而言，使用 `#[wasm_bindgen]` 标记的 Rust 函数和类型将被 `wasm-bindgen` 工具处理，该工具将它们转换为 JavaScript 可以调用的函数和对象。这使得 Rust 代码可以直接在 WebAssembly 中运行，并与 JavaScript 代码进行交互。

删之后：

```javascript
//t1_rust.js:
let imports = {};
let wasm;

const path = require('path').join(__dirname, 't1_rust_bg.wasm');
const bytes = require('fs').readFileSync(path);

const wasmModule = new WebAssembly.Module(bytes);
const wasmInstance = new WebAssembly.Instance(wasmModule, imports);
wasm = wasmInstance.exports;
module.exports.__wasm = wasm;
//t1_rust.d.ts:
/* tslint:disable */
/* eslint-disable */

```

删之前：

```js
//t1_rust.js:
let imports = {};
let wasm;

let cachedUint32Memory0 = null;

function getUint32Memory0() {
    if (cachedUint32Memory0 === null || cachedUint32Memory0.byteLength === 0) {
        cachedUint32Memory0 = new Uint32Array(wasm.memory.buffer);
    }
    return cachedUint32Memory0;
}

let WASM_VECTOR_LEN = 0;

function passArray32ToWasm0(arg, malloc) {
    const ptr = malloc(arg.length * 4, 4) >>> 0;
    getUint32Memory0().set(arg, ptr / 4);
    WASM_VECTOR_LEN = arg.length;
    return ptr;
}
/**
* @param {number} flag
* @param {Int32Array} seq
* @param {number} size
* @returns {number}
*/
module.exports.bocchi_shut_up = function(flag, seq, size) {
    const ptr0 = passArray32ToWasm0(seq, wasm.__wbindgen_malloc);
    const len0 = WASM_VECTOR_LEN;
    const ret = wasm.bocchi_shut_up(flag, ptr0, len0, size);
    return ret;
};

const path = require('path').join(__dirname, 't1_rust_bg.wasm');
const bytes = require('fs').readFileSync(path);

const wasmModule = new WebAssembly.Module(bytes);
const wasmInstance = new WebAssembly.Instance(wasmModule, imports);
wasm = wasmInstance.exports;
module.exports.__wasm = wasm;

//t1_rust.d.ts:
/* tslint:disable */
/* eslint-disable */
/**
* @param {number} flag
* @param {Int32Array} seq
* @param {number} size
* @returns {number}
*/
export function bocchi_shut_up(flag: number, seq: Int32Array, size: number): number;

```



#### 总结

###### **→ 📖 Q1.7(P) 请记录下目前的时间。**

2024.3.29 19：34

###### **→ 📖 Q1.8(I) 请写下本部分的心得体会。**



## Chapter.2 石子翻滚，晨光降临你身（Rock'n Roll, Morning Light Falls on You）

#### 结对过程

###### **→ 📖 Q2.1(P) 请记录下目前的时间。**

2024.3.20 9:30

###### **→ 📖 Q2.2(P) 请在完成任务的同时记录，并在完成任务后整理完善：**

> 1. 浏览任务要求，参照 **附录A：基于 PSP 2.1 修改的 PSP 表格**，估计任务预计耗时；
>2. 完成编程任务期间，依次做了什么（比如查阅了什么资料，随后如何进行了开发，遇到了什么问题，又通过什么方式解决）；

1.

| Personal Software Process Stages        | 个人软件开发流程                                             | 预估耗时（分钟） | 实际耗时（分钟） |
| :-------------------------------------- | :----------------------------------------------------------- | :--------------- | :--------------- |
| **PLANNING**                            | **计划**                                                     |                  |                  |
| - Estimate                              | - 估计这个任务需要多少时间                                   | 120              | 110              |
| **DEVELOPMENT**                         | **开发**                                                     |                  |                  |
| - Analysis & Design Spec                | - 需求分析 & 生成设计规格（确定要实现什么）                  | 20               | 15               |
| - Technical Background                  | - 了解技术背景（包括学习新技术）                             | 5                | 5                |
| - Coding Standard                       | - 代码规范                                                   | 5                | 5                |
| - Design                                | - 具体设计（确定怎么实现）                                   | 20               | 20               |
| - Coding                                | - 具体编码                                                   | 40               | 40               |
| - Code Review                           | - 代码复审                                                   | 10               | 10               |
| - Test Design                           | - 测试设计（确定怎么测，比如要测试哪些情景、设计哪些种类的测试用例） | 10               | 5                |
| - Test Implement                        | - 测试实现（设计/生成具体的测试用例、编码实现测试）          | 10               | 10               |
| **REPORTING**                           | **报告**                                                     |                  |                  |
| - Quality Report                        | - 质量报告（评估设计、实现、测试的有效性）                   | 15               | 15               |
| - Size Measurement                      | - 计算工作量                                                 | 5                | 2                |
| - Postmortem & Process Improvement Plan | - 事后总结和过程改进计划（总结过程中的问题和改进点）         | 15               | 15               |
| **TOTAL**                               | **合计**                                                     | 155              | 142              |

2.首先进行题意分析，然后设计封装播种动作、判定游戏结束、结束清算为单独的函数，随后进行了coding，我们先用c语言实现，然后逐句转化为rust。c的编码过程中没有发生问题，转为rust过程中，原本在c中使用的全局变量在rust中需要unsafe块包裹。测试环节，测试样例是并发执行的，我们使用了全局变量，导致多次测试结果不一致，之后查阅资料发现了这个问题，将测试样例逐个测试，就解决了。

#### 测试

###### **→ 📖 Q2.3(P) 请说明针对该任务，你们设计和实现测试的方法及过程，包括但不限于：出于对需求的哪些考虑设计了哪些测试用例、如何评估所设计测试的有效性 等等。**

我们分析了需求，决定对规则逐条编写测试样例。

![image-20240406093839956](C:\Users\GZC\AppData\Roaming\Typora\typora-user-images\image-20240406093839956.png)

我们使用了混合测试，检验其他简易测试点的有效性，混合测试来自于B站真实游戏模拟视频，游戏结束测试来自T3的模拟对弈序列。

###### **→ 📖 Q2.4(I) 请说明<u>单元测试</u>对软件开发的作用。**

1. **发现问题：** 单元测试可以帮助开发人员在代码编写阶段及时发现问题和错误。通过针对每个单元（一个或多个函数）编写测试用例，可以验证代码的正确性。提前发现 bug，从而减少在后续阶段（集成测试、系统测试等）发现和修复问题所需的时间和成本。
2. **提高代码质量：** 单元测试要求开发人员编写可测试性强、模块化的代码。通过编写测试用例，开发人员会更加注意编写可读、简洁、可维护的代码，从而提高代码的质量。

#### 总结

###### **→ 📖 Q2.5(P) 请记录下目前的时间，并根据实际情况填写 附录A：基于 PSP 2.1 修改的 PSP 表格 的“实际耗时”栏目。**

2024.3.30 11:30

###### **→ 📖 Q2.6(I) 请写下本部分的心得体会。**



## Chapter.3 十二进制的黄昏（Dusk of the Duodecimal System）

#### 结对过程

###### **→ 📖 Q3.1(P) 请记录下目前的时间。**

2024.3.30 14:00

###### **→ 📖 Q3.2(P) 请在完成任务的同时记录，并在完成任务后整理完善：**

> 1. 浏览任务要求，参照 **附录A：基于 PSP 2.1 修改的 PSP 表格**，估计任务预计耗时；
>2. 完成编程任务期间，依次做了什么（比如查阅了什么资料，随后如何进行了开发，遇到了什么问题，又通过什么方式解决）；

1.

| Personal Software Process Stages        | 个人软件开发流程                                             | 预估耗时（分钟） | 实际耗时（分钟） |
| :-------------------------------------- | :----------------------------------------------------------- | :--------------- | :--------------- |
| **PLANNING**                            | **计划**                                                     |                  |                  |
| - Estimate                              | - 估计这个任务需要多少时间                                   | 210              | 205              |
| **DEVELOPMENT**                         | **开发**                                                     |                  |                  |
| - Analysis & Design Spec                | - 需求分析 & 生成设计规格（确定要实现什么）                  | 30               | 30               |
| - Technical Background                  | - 了解技术背景（包括学习新技术）                             | 30               | 30               |
| - Coding Standard                       | - 代码规范                                                   | 10               | 5                |
| - Design                                | - 具体设计（确定怎么实现）                                   | 40               | 30               |
| - Coding                                | - 具体编码                                                   | 60               | 60               |
| - Code Review                           | - 代码复审                                                   | 10               | 20               |
| - Test Design                           | - 测试设计（确定怎么测，比如要测试哪些情景、设计哪些种类的测试用例） | 20               | 20               |
| - Test Implement                        | - 测试实现（设计/生成具体的测试用例、编码实现测试）          | 10               | 10               |
| **REPORTING**                           | **报告**                                                     |                  |                  |
| - Quality Report                        | - 质量报告（评估设计、实现、测试的有效性）                   | 20               | 30               |
| - Size Measurement                      | - 计算工作量                                                 | 5                | 5                |
| - Postmortem & Process Improvement Plan | - 事后总结和过程改进计划（总结过程中的问题和改进点）         | 20               | 20               |
| **TOTAL**                               | **合计**                                                     | 255              | 260              |

2.

查阅资料：包括Alpha-Beta剪枝算法（启发式搜索）等博弈树算法，深度优先遍历算法等。先根据题目要求编写C语言程序，然后逐句转换为rust。编写C语言时，遇到了初学数据结构常见的问题——没有初始化指针。C语言转换过程中，由于缺乏对rust的了解，我们尝试创建一个包含 Vec<Option<Box<Node>>>类型的向量替代C语言中的二维数组，但是 Box<Node> 类型并没有实现 Clone trait，而在这个 vec! 宏调用中需要实现 Clone trait，因此我们需要手动构造向量：

   ```rust
   let mut child = Vec::with_capacity(7);
   for _ in 0..7 {
       child.push(None);
   }
   ```

#### 代码可复用性与需求变更

###### **→ 📖 Q3.3(P) 请说明针对该任务，你们对 `🧑‍💻 T2` 中已实现的代码进行了哪些复用和修改。**

### T3_1

我们保留了所有T2中的函数，仅新增了**mancalaBoard**。播种过程调用T2完成的**mancalaResult**，仅考察最后一次播种的正确性，返回相应的错误码即可。

![image-20240406095005052](C:\Users\GZC\AppData\Roaming\Typora\typora-user-images\image-20240406095005052.png)

### T3_2

我们保留了所有T2中的函数，新增了**struct node、dfs()和mancalaOperator()**,没有进行修改。

![image-20240406100401876](C:\Users\GZC\AppData\Roaming\Typora\typora-user-images\image-20240406100401876.png)

###### **→ 📖 Q3.4(I) 请说明在编码实现时，可以采取哪些<u>设计思想</u>、考虑哪些<u>设计冗余</u>，来提高既存代码适应需求变更的能力。**

### 设计思想：

- **模块化设计：** 封装函数实现单一特定的功能，提高代码的可重用性和可维护性，并使变更更容易。
- **高内聚低耦合：** 模块之间减少依赖，通过定义清晰的接口降低耦合性。
- **可扩展性设计：** 框架设计的时候考虑将代码设计成可以轻松修改和加需求的形式，以满足未来的需求变更。

### 设计冗余：

- **错误处理和容错机制：** 实现良好的错误处理和容错机制，以应对异常情况和意外错误。例如T3_1数据样例保证了前n-1步是正确的，但是未来可能的需求变更，不保证前n-1步正确性时，可以利用错误处理机制来实现。
- **单元测试和集成测试：** 编写全面的单元测试和集成测试，覆盖代码的各个功能和边界情况，确保之后的需求变更导致的代码修改不会破坏现有功能。
- **利于找bug**:设计一些函数输出当前的某些变量值，有利于寻找bug和做测试。

#### 需求建模和算法设计

###### **→ 📖 Q3.5(P) 请说明你们如何<u>建模</u>这一需求。

1. **定义棋盘：** 首先，我们需要用全局二维数组表示棋盘状态。这个二维数组包含了当前棋盘状态、净胜棋数等信息。我们可以在算法中便捷操作游戏状态。
2. **生成可行动作：** 我们需要考虑所有先手方当前含有棋子的棋洞。这些动作是当前状态下可选的洞。
3. **评估选择最佳动作：** 对于每个生成的动作，我们需要计算当前净胜棋数和未来几步的净胜棋数，选择最大的那一步。

###### **→ 📖 Q3.6(P) 请说明针对该任务，你们采取了<u>哪些策略</u>来取得更大的净胜棋数，如何编程实现这些策略。**

理想策略：

1. **优先考虑使己方得分增加的动作**：在选择动作时，优先考虑能经过己方积分洞的动作。尽量保持己方优势地位，并增加净胜棋数。
2. **优先考虑使己方多走一次**：选择动作时，优先考虑使自己最后一颗棋子落入己方计分洞，获得再走一次的机会。
3. **利用取子规则获取额外得分**：考虑游戏规则中的特殊取子情况，如最后一颗棋子落入无棋子的棋洞且正对面有对方的棋子，可以获取额外的得分。
4. **避免让对方取子**：可以牺牲一些棋子，以避免对方利用特殊取子规则获取额外得分。

我们基于以上策略采用动态规划的算法，利用**深度优先搜索**模拟之后8步的所有情况，找到净胜棋数最大的变化。

#### 软件度量

###### **→ 📖 Q3.7(P) 请说明你们如何量度所实现的程序模块的有效性，例如：“如何说明我们的程序模块对弈能力很强？”，尝试提出一些可能的定量分析方式。**

1. **调整遍历深度**：通过调整dfs的深度，将两个不同深度的算法作为两个选手模拟大量对局，分别记录胜率，以及平均每局的净胜棋数。如果深度较深的那一个程序能够在胜率以及胜棋数领先，那么就能说明我们的算法对弈能力较强。
2. **与其他对手的对战结果**：与其他同学的程序进行多次对战，并记录胜率和净胜棋数。如果程序模块在与其他同学的对战中能够稳定获胜，并且净胜棋数较大，那么说明程序模块具有较强的对弈能力。
3. **搜索效率分析**：分析程序模块的搜索效率，包括搜索所需的时间以及内存占用情况等。如果程序模块能够在更短的时间内找到更优的解决方案，那么说明程序模块具有较强的搜索能力。

#### 总结

###### **→ 📖 Q3.8(P) 请记录下目前的时间，并根据实际情况填写 附录A：基于 PSP 2.1 修改的 PSP 表格 的“实际耗时”栏目。**

2024.3.30 17:30

###### **→ 📖 Q3.9(I) 请写下本部分的心得体会。**



## 结对项目总结

#### 结对过程回顾和反思

###### **→ 📖 Q4.1(P) 提供两人在讨论的结对图像资料。**

![9ae7f85cd2dc20b0439c46ecdb8d6b2](C:\Users\GZC\Documents\WeChat Files\wxid_51qij6jrb1w522\FileStorage\Temp\9ae7f85cd2dc20b0439c46ecdb8d6b2.jpg)

###### **→ 📖 Q4.2(P) 回顾结对的过程，反思有哪些可以提升和改进的地方。**

1. **程序的测试**： 可以进行更充分的测试，例如利用自动化测试工具等
2. **版本管理与控制意识缺乏**：编程过程中，没有及时提交与保存代码，导致部分代码丢失与重写。之后我们应该定期进行代码库的备份，以防止意外情况发生时造成重要代码的丢失。
3. **代码质量与规范**： 结对编程过程中，代码的命名不规范可读性差。之后要注重代码质量和规范，注重代码的可读性和可测试性。

###### **→ 📖 Q4.3(I) 锐评一下你的搭档！并请至少列出三个优点和一个缺点。**



#### 对结对编程的理解

###### **→ 📖 Q4.4(I) 说明结对编程的优缺点、你对结对编程的理解。**


#### 代码仓库

###### **→ 📖 Q4.5(P) 请提供你们完成代码实现的代码仓库链接。**

[jefferrrrrrrrrrry/SE2024-PairProgramming (github.com)](https://github.com/jefferrrrrrrrrrry/SE2024-PairProgramming)
