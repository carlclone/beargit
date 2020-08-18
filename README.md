
# guide reading

- `{.beargit}` 根目录
   - `.index` 存放当前repo所有已跟踪的文件
   - `.prev` 存放最后一个commit的id , 如果没有就是0...0000
   - `.branches` 存放本repo拥有的所有分支
   - `{ID}` 对应commit_id的目录
    - `.index` 本次commit 跟踪的文件
    - `.msg` 存放当前提交的commit msg
    - 存放本次修改的所有文件的一份copy , 不跟踪diff(有点难)
    - `.prev`存放上一个commit的id , 用于形成链表
    

- commit_id只由6,1,c三个字符组成,长度是固定的40 , 3^40个 , 排列的算法,还要保证不重复

- commit msg 必须包含"GO BEARS!"

- 不要求track 子目录和`.`开头的目录和文件

- rm不删除文件,只从index里移除 (只需要移根目录的.index和本次的吗)

- 要求自己想case 写测试 (Testing your code in CS61C)

- strcat()连接两个字符串, sprintf()也可以, strtok(str,"\n") 移除从文件读出来的\n,替换为\0


# 实现 status

难度*

输出.index的所有tracked files

```
$ beargit status
Tracked files:

  <file1>
  [...]
  <fileN>

<N> files total
```

# 实现 rm

难度*

把某个文件名从.index移除

```
$ beargit rm FILE_THAT_IS_NOT_TRACKED.txt
ERROR: File <filename> not tracked
```

# 实现commit

难度*****

本lab的核心就是commit和checkout , 慢慢分析...