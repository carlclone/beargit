int beargit_init(void);
int beargit_add(const char* filename);
int beargit_rm(const char* filename);
int beargit_commit(const char* message);
int beargit_status();
int beargit_log();

void next_commit_id_part1(char *commit_id);
int checkout_commit(const char *commit_id);
int get_branch_number(const char* branch_name);
int is_it_a_commit_id(const char* commit_id);
bool fs_check_dir_exists(char* dir);

// Number of bytes in a commit id
#define COMMIT_ID_BYTES 40

// Preprocessor macros capturing the maximum size of different  structures
#define FILENAME_SIZE 512
#define COMMIT_ID_SIZE (COMMIT_ID_BYTES+1)
#define MSG_SIZE 512
#define BRANCHNAME_SIZE 128
#define COMMIT_ID_BRANCH_BYTES 10