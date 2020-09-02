
发现ucore的许多前置条件都不具备 , 用cs61c的hw补足c , c调试(gdb) , c开发环境 , c的ide

# todo
- [x] test method
- [ ] gdb/cgdb
- [x] status
- [x] rm
- [x] commit
    - [x] is_commit_msg_ok
    - [x] commit_id impl
- [x] log
- [x] branch
- [x] checkout

# 收获

- 实践了使用进制转换生成不重复 id 的思想, (cs61c integer representation 和玩转数据结构的哈希表一章)
- 了解到branch 的简洁设计
- 重点,c 语言的熟悉
- 大量的基础文件流和字符串(字节数组)操作经验
- 掌握 cgdb 调试使用

# 资料

[cs61c sp15](https://inst.eecs.berkeley.edu//~cs61c/sp15/)
[hw1 beargit basis](https://inst.eecs.berkeley.edu//~cs61c/sp15/hw/01/hw1.html)
[hw2 beargit branch/checkout](https://inst.eecs.berkeley.edu//~cs61c/sp15/hw/02/hw2.html)
[lab1 c and gdb](https://inst.eecs.berkeley.edu/~cs61c/sp15/labs/01/)

# guide reading 

`beargit-project spec.pdf`

实际上是一个snapshot工具 (全量快照版本的 git)

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

# dev 环境搭建

利用之前 15213 的 docker镜像,配合 clion 的 tool chain + makelist.txt 代码提示

# gdb / cgdb

- 材料 [ucore gdb](https://chyyuu.gitbooks.io/ucore_os_docs/content/lab0/lab0_2_3_3_gdb.html)
- 材料 [cs61c cgdb](https://inst.eecs.berkeley.edu/~cs61c/sp15/labs/01/)
- 材料 [cs61a]()
- 编译时需要gcc -g (-ggdb) 
- gdb progname
- refcard https://inst.eecs.berkeley.edu//~cs61c/resources/gdb5-refcard.pdf
- php源码阅读视频课第x节



# 测试方式

- 编译后自测
- 执行 init_test (提供了些许用例)
- 自己写 unit test

# 调试学习

cgdb , cs61c lab1




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




# commit_id 的设计

```
Commit IDs are not based on cryptographic hash functions,
but instead are a fixed sequence of 40-character strings that only contain '6', '1' and 'c' (why we chose those characters is left as an exercise to the reader).
Commit ID lengths are limited to 40 characters (not including the null terminator).
第一个c_id为0000..0000 , 40个0

Read the ID of the previous last commit from .beargit/.prev
Generate the next ID (newid) in such a way that:
All characters of the id are either 6, 1 or c
Generating 100 IDs in a row will generate 100 IDs that are all unique
**** 核心问题HashIds类库的实现 , 实际是进制转换,10进制转3进制(61c) , play with algorithm里
(Hint: you can do this in such a way that you go through all possible IDs before you repeat yourself. Some of the ideas from the number representation class may help you!)

BUT WAIT! We're not done yet: After following the above steps, you will need to make two changes to your newly formed beargit.c file:
you will need to add a _hw1 to the function names of your beargit_commit and next_commit_id functions
(so they become beargit_commit_hw1 and next_commit_id_hw1; note that you only should change those two lines, not the lines where they are called).
We will now explain why these changes are necessary.

Since we are now adding branching functionality to beargit, we have to change our next_commit_id function: in HW1,  (commit_id的实现因为branch功能要做出修改)
we generated the next commit ID solely based on its predecessor. Without branches, this would give us unique IDs,
but now that we can have multiple commits with the same predecessor, the commit ID cannot just depend on the last commit -- in that case,
all branches following a commit would have the same ID, which is not unique anymore.

To prevent this, we have adapted the next_commit_id function to write the branch number into the first 10 digits of the string, for a total of 3^10 possible branches.
(修改,前10个字符改成branch number的字符), branch number是?
If the remainder of the commit ID uses the same technique as in HW1 to generate unique IDs, we know that the IDs on each branch are unique,
and since the IDs between branches are different as well, we know that all IDs are unique.

To help you with this change, we have prepared a new next_commit_id function that uses your next_commit_id function to fill in the last 30 digits of the commit ID,
while our function is filling in the rest.

This means that you have to change your next_commit_id function to now only generate 30 digits -- note that we give you a pointer to digit 10 of the string,
so nothing in your functionality should change, except that you now operate on 30 digits instead of 40. We still require you to generate at least 100 unique IDs.(生成100个unique ids)

So beargit first needs to find out if you are giving it a commit or a branch name. For this, we have prepared a function is_it_a_commit_id, which you need to fill in. (提供了一个验证方法)
The function takes a string and returns true if and only if the string is 40 characters that are each 6, 1 or c.

if the argument is a commit ID (40 characters, each of which is '6', '1' or 'c')
```