
# guide reading

> 实际上是一个snapshot工具 (全量快照版本的 git)

- `{.beargit}` 根目录
   - `.index` 存放当前repo所有已跟踪的文件
   - `.prev` 存放最后一个commit的id , 如果没有就是0...0000
   - `.branches` 存放本repo拥有的所有分支
   - `{ID}` 对应commit_id的目录
    - `.index` 本次commit 跟踪的文件
    - `.msg` 存放当前提交的commit msg
    - 存放所有文件的一份copy , 不跟踪diff(太难)
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

- "GO BEARS!" 判断是否可 commit
- 从`.beargit/.prev 读取父 commit_id (形成链表用于回溯)
- 生成next_id,也就是本次的 commit_id (一大堆逻辑)
- 新建`.beargit/<newid>`目录作为本次 commit 的目录
- 复制`.beargit/.index`和`.beargit/.prev`到目录里,感觉就是打快照版本的 git ,实现起来简单许多
- 写入`.beargit/.msg`
- copy 所有 tracked files 一份
- 更新`.beartgit/.prev`


```
Generate the next ID (newid) in such a way that:

1. ID Length is COMMIT_ID_BYTES (not including NULL terminator)

2. All characters of the id are either 6, 1 or c

3. Generating 100 IDs in a row will generate 100 IDs that are all unique (Hint: you can do this in such a way that you go through all possible IDs before you repeat yourself. Some of the ideas from number representation may help you!)

4. Calling next_commit_id(char* commit_id) results in commit_id being updated to a ID.

5. The ID string consists of a branch-id (of size COMMIT_ID_BRANCH_BYTES) followed by a tag-id to ﬁll the rest of the size of the ID.

(Note: the tag-id used here has nothing to do with a git tag, git tags aren't involved in this project!)

6. We have implemented the branch-id step for you in next_commit_id(char* commit_id). Don't worry too much about where the branch-id is coming from yet (more on that in part 5), but pay close attention to what indices in the commit_id string are being updated and how the pointer is being passed to next_commit_id_part1(). To ﬁnish the next ID generation you will need to complete next_commit_id_part1().
```

# 实现 log

难度**

- 读取.prev
- 打印 commit_id 和 msg
- 读取上一个.prev
- 打印 commit_id 和 msg
- 限制数量 -n

```
$ beargit log [BLANK LINE] commit <ID1>

<msg1> [BLANK LINE] commit <ID2>

<msg2> [...] commit <IDN>

<msgN> [BLANK LINE]
```

# 实现 branch

难度*****

- 打印出所有分支名
- 在当前分支名前加星号
- 根目录有个.branches 文件,和.index 很像
- 根目录有个.current_branch 文件
- 从老到新打印
- checkout commit 的时候不用分支名加星号 , 因为 detach 了

```
beargit branch 
<branch1> 
<branch2> 
[...] 
* <current_branch> 
[...] 
<branchN>
```

# 实现 checkout

难度*****

- 支持 checkout <commit_id>
- 支持 checkout <branch>
- 支持 checkout -b <newbranch>
- commit_id,40字符,都是 6 or 1 or c
- checkout commit
    - 替换所有 tracked files
- checkout branch
    - 是否存在该分支
    - 要保存当前分支的最新提交id(用于切换回来)
    - 修改 .current_branch
    - 从 .branch_targetbranch读取分支的 head commit_id 
    - 然后和 checkout commit 一样

- checkout -b
    - 和 checkout branch 类似
    - 是从.prev 读取 head commit id
    - 要加到.branches 列表里
- 有一个找 bug 作业,用 cgdb 和 printf 找, 一行一个..
- 


```
$ docker-compose run --rm shark
# can also run with Docker
$ docker run --rm -v "`pwd`"/labs:/usr/src/app/ -it lx70716/cmu-15213

```