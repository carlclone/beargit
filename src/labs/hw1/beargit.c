#include <stdio.h>
#include <string.h>

#include <unistd.h>
#include <sys/stat.h>

#include "beargit.h"
#include "util.h"

/* Implementation Notes:
 *
 * - Functions return 0 if successful, 1 if there is an error.
 * - All error conditions in the function description need to be implemented
 *   and written to stderr. We catch some additional errors for you in main.c.
 * - Output to stdout needs to be exactly as specified in the function description.
 * - Only edit this file (beargit.c)
 * - You are given the following helper functions:
 *   * fs_mkdir(dirname): create directory <dirname>
 *   * fs_rm(filename): delete file <filename>
 *   * fs_mv(src,dst): move file <src> to <dst>, overwriting <dst> if it exists
 *   * fs_cp(src,dst): copy file <src> to <dst>, overwriting <dst> if it exists
 *   * write_string_to_file(filename,str): write <str> to filename (overwriting contents)
 *   * read_string_from_file(filename,str,size): read a string of at most <size> (incl.
 *     NULL character) from file <filename> and store it into <str>. Note that <str>
 *     needs to be large enough to hold that string.
 *  - You NEED to test your code. The autograder we provide does not contain the
 *    full set of tests that we will run on your code. See "Step 5" in the homework spec.
 */

/* beargit init
 *
 * - Create .beargit directory
 * - Create empty .beargit/.index file
 * - Create .beargit/.prev file containing 0..0 commit id
 *
 * Output (to stdout):
 * - None if successful
 */

int beargit_init(void) {
  fs_mkdir(".beargit");

  FILE* findex = fopen(".beargit/.index", "w");
  fclose(findex);
  
  write_string_to_file(".beargit/.prev", "0000000000000000000000000000000000000000");

  return 0;
}


/* beargit add <filename>
 * 
 * - Append filename to list in .beargit/.index if it isn't in there yet
 *
 * Possible errors (to stderr):
 * >> ERROR: File <filename> already added
 *
 * Output (to stdout):
 * - None if successful
 */

int beargit_add(const char* filename) {
  FILE* findex = fopen(".beargit/.index", "r");
  FILE *fnewindex = fopen(".beargit/.newindex", "w");

  char line[FILENAME_SIZE];
  while(fgets(line, sizeof(line), findex)) {
    strtok(line, "\n");
    if (strcmp(line, filename) == 0) {
      fprintf(stderr, "ERROR: File %s already added\n", filename);
      fclose(findex);
      fclose(fnewindex);
      fs_rm(".beargit/.newindex");
      return 3;
    }

    fprintf(fnewindex, "%s\n", line);
  }

  fprintf(fnewindex, "%s\n", filename);
  fclose(findex);
  fclose(fnewindex);

  fs_mv(".beargit/.newindex", ".beargit/.index");

  return 0;
}


/* beargit rm <filename>
 * 
 * See "Step 2" in the homework 1 spec.
 *
 */

int beargit_rm(const char* filename) {
  /* COMPLETE THE REST */
  //遍历文件并写入新文件,写完后删除原文件并重命名新文件 (fs_mv() 会直接替换)
    FILE *oldFd = fopen(".beargit/.index","r");
    FILE *newFd = fopen(".beargit/.newindex","r");

    char fileName[FILENAME_SIZE];
    int deleted = 0;

    while (fgets(fileName, sizeof(fileName), oldFd)) {
        strtok(fileName, "\n");
        if (strcmp(fileName, filename) == 0) {
            deleted=1;
            continue;
        }
        fprintf(newFd, "%s\n", fileName);
    }
    fclose(oldFd);
    fclose(newFd);
    fs_mv(".beargit/.newindex",".beargit/.index");
    if (deleted) {
        fprintf(stdout,"ERROR: File %s not tracked",filename);
        return 1;
    }
    return 0;

  return 0;
}

/* beargit commit -m <msg>
 *
 * See "Step 3" in the homework 1 spec.
 *
 */

const char* go_bears = "GO BEARS!";

int is_commit_msg_ok(const char* msg) {
  /* COMPLETE THE REST */
  //包含"GO BEARS!"子串 , 两个字符串指针 p1,p2 移动
  char gb[] = "GO BEARS!";
  int p1=0;
  int matchP1 = strlen(gb);
  while (*msg!="\0") {
    if (*msg == gb[p1]) {
      if (p1==matchP1) {
        return 1;
      }
      p1++;

    } else if (p1!=0)
    {
      p1=0;
    }
  
    msg++;
  }
  return 0;
}

const char* digits="61c";

void next_commit_id(char* commit_id) {
  /* COMPLETE THE REST */
  
  char currentBranch[BRANCHNAME_SIZE];
  read_string_from_file(".beargit/.current_branch",currentBranch,BRANCHNAME_SIZE);

  //6 1 c 的三进制转换
  //0 1 2 映射
  //只填了前10个 , branch相关
  int n = get_branch_number(currentBranch);
  for (int i=0;i<COMMIT_ID_BRANCH_BYTES;i++) {
    // n%3 , 得到第i位的值
    commit_id[i] = digits[n%3];
    // n/=3 , 左移,去掉第i位
    n/=3;
  }

  //填剩余的30个
  next_commit_id_part1(commit_id+COMMIT_ID_BRANCH_BYTES);

  

}

void next_commit_id_part1(char* commit_id) {
  /* COMPLETE THE REST */
  //todo;这是在干嘛 , 就不能也用进制的写法
  char* new_id = commit_id;
  //当指针没有到尾部
  while (*new_id != '\0') {
    //如果是0 , 都填上6 , 然后下一次循环 00000000 -> 6666666666
    if (*new_id == '0') {
      *new_id = '6';
      new_id++;
      continue;
      // 6 1 c
      // 0 1 2
      //如果是6了,加一,变成1
    } else if (*new_id == '6') {
      *new_id = '1';
      break;
      //如果是1了,加一,变成c
    } else if (*new_id == '1') {
      *new_id = 'c';
      break;
      //如果是c了,进位,原位变为6
    } else if (*new_id == 'c') {
      *new_id = '6';
      new_id++;
      //似乎和第一个if一样
    } else {
      *new_id = '6';
      new_id++;
    }
  }
}



int beargit_commit(const char* msg) {
  if (!is_commit_msg_ok(msg)) {
    fprintf(stderr, "ERROR: Message must contain \"%s\"\n", go_bears);
    return 1;
  }

  char commit_id[COMMIT_ID_SIZE];
  read_string_from_file(".beargit/.prev", commit_id, COMMIT_ID_SIZE);
  next_commit_id(commit_id);

  /* COMPLETE THE REST */
  //创建本次commit的目录
  char *filename=malloc(strlen(".beargit/") + strlen(commit_id)+1);
  sprintf(filename,"%s/%s",".beargit",commit_id);
  fs_mkdir(filename);
  //创建本次的.index文件
  char *index=malloc(strelen(filename)+strlen("/.index")+1);
  sprintf(index,"%s/%s",filename,"/.index");
  //拷贝.prev
  fs_cp(".beargit/.prev",prev);
  
  //拷贝所有本次.index里的一份文件
  FILE *filelist=fopen(".beargit/.index","r");
  char file[FILENAME_SIZE];
  while (fgets(file,sizeof(file),filelist)) {
    strtok(file,"\n");
    char *new_file=malloc(strlen(filename)+strlen(file)+1);
    sprintf(new_file,"%s/%s",filename,file);
    fs_cp(file,new_file);
  }
  

  //创建并写入.msg
char *commit_msg = malloc(strlen(filename)+strlen("/.msg")+1);
sprintf(commit_msg,"%s%s","/.msg");
write_string_to_file(commit_msg,msg);
write_string_to_file(".beargit/.prev",commit_id);
fclose(filelist);

  //结束
  return 0;
}

/* beargit status
 *
 * See "Step 1" in the homework 1 spec.
 *
 */

int beargit_status() {
  /* COMPLETE THE REST */

  //输出到标准输出流 :"Tracked files:"
  fprintf(stdout,"%s\n\n","Tracked files:");


  //读取.beargit 目录的.index , 一行一行输出文件名 , 并保持文件计数
  int fileNums=0;
  //打开文件,得到文件descriptor(在进程的文件描述符表创建一条文件名->inode(文件位置?)的 kv 映射)
  FILE* fd=fopen(".beargit/.index","r");
  //循环读取文件直到结束
  char fileName[FILENAME_SIZE];
    //fgets 每次读取数据流中的一行,
    // 当读取 (n-1) 个字符时，或者读取到换行符时，或者到达文件末尾时，它会停止
    //https://baike.baidu.com/item/fgets
    //如果文件中的该行，不足n-1个字符，则读完该行就结束。如若该行（包括最后一个换行符）的字符数超过n-1，则fgets只返回一个不完整的行，但是，缓冲区总是以NULL字符结尾，对fgets的下一次调用会继续读该行。函数成功将返回stream，失败或读到文件结尾返回NULL。因此不能直接通过fgets的返回值来判断函数是否是出错而终止的，应该借助feof函数或者ferror函数来判断。
    //返回值
    //如果成功，该函数返回相同的 str 参数。如果到达文件末尾或者没有读取到任何字符，str 的内容保持不变，并返回一个空指针。如果发生错误，返回一个空指针。
    //在读字符时遇到end-of-file，则eof指示器被设置，如果还没读入任何字符就遇到这种情况，则stream保持原来的内容，返回NULL；
    //如果发生读入错误，error指示器被设置，返回NULL，stream的值可能被改变。
    //char *fgets(char *str, int n, FILE *stream);
  while (fgets(fileName,sizeof(fileName),fd)) {
      strtok(fileName,"\n"); //去除读取到的"\n"
      fprintf(stdout,"%s",fileName);
      fileNums++;
  }


  //输出"<N> files total"
  fprintf(stdout,"%d %s\n\n",fileNums,"files total");

  //忘记关闭文件了..百密总有一疏 , 墨菲定律..
  // 肉眼检查,伪代码都不可靠,测试也可能测不出来...
  // 想齐全 case 多自测吧 ,
  // 做 lab 的时候都是靠的 test 套件齐全,做算法题也是
  // 想起那个数学 类比拼图的说法 ,
  // 没人会从中间开始拼,都是从外到里一点一点积累,积累的足够多了就能接触到更里层的拼块了
  fclose(fd);
  return 0;
}

/* beargit log
 *
 * See "Step 4" in the homework 1 spec.
 *
 */

int beargit_log() {
  /* COMPLETE THE REST */

  return 0;
}
