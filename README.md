
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