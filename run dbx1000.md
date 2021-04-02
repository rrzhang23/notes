### 机器
10.11.6.120、10.11.6.121  
账号：zhangrongrong 密码 zhangrr

所有文件文件夹放在 /home/zhangrongrong/sunjiabo 下

### git 仓库
```
cd ~/sunjiabo
git clone git@github.com:rongzia/DBx1000.git
cd DBx1000
# 从 5a225d9 版本新建一个分支用于实验，5a225d9 是目前能跑没有冲突情况的版本
git branch new-branch 5a225d9
git checkout new-branch
```

### 编译
需要调整的参数都放在 config.h 下：
```
// 没有使用相对路径，必须修改的参数：
// YCSB 负载对应的表描述：
#define g_schame_path "/home/zhangrongrong/CLionProjects/DBx1000/common/workload/YCSB_schema.txt"
// 数据库持久化位置：
#define DB_PREFIX "/home/zhangrongrong/CLionProjects/DBx1000/db/"
```

### 编译
```
mkdir build && cd build
cmake .. && make -j32
```

### 实验时需要调整的参数：
也在 config.h 里
```
// 所有机器的总进程（或者总实例数）数和线程数（每个实例的线程数量）
#define THREAD_CNT					8
#define PROCESS_CNT					2
```

### run
```
1. 先生成数据库文件
cd build
./gen_db
// 中途可能会让输密码：zhangrr，把本地数据库文件传到另一台机器上。这个过程可以把 test/gen_db.cpp 第183 行注释掉就行。

2. 在 10.11.6.120 上起 disk_service 进程
nohup ./disk_service &

2. 在 10.11.6.120 上起 lock_server 进程
./main_lock_service

3. 起 instance 进程
../run_test.sh 0 2
// 这是一个脚本，第一个参数 0 表示在第 0 个机器上，2 表示在这个机器上起多少个进程


```


run_test.sh 参数和 THREAD_CNT、PROCESS_CNT 关系 ：  
当 (THREAD_CNT * PROCESS_CNT < 32) 时，可以直接在一个机器上跑，比如：  
THREAD_CNT = 4  
PROCESS_CNT = 4  
可以 ../run_test.sh 0 4

当 (THREAD_CNT * PROCESS_CNT >= 32) 时，要分在不同的机器上跑，比如：  
THREAD_CNT = 4  
PROCESS_CNT = 12  
10.11.6.120可以 ../run_test.sh 0 6  
10.11.6.121可以 ../run_test.sh 1 6
这样 10.11.6.120 跑的实例对应(0,1,...5), 10.11.6.121 跑的实例对应(6,7...11)

### config.josn
这个文件定义 server、instance节点对应的 rpc IP+port，跑之前需要修改

