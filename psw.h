#define N 100
#define MAXNAME 32
#define MAXPSW  32

struct psw {
	char username[MAXNAME];
	uint uid;
	char password[MAXNAME];
	uint gid;
	char groupname[MAXNAME];
};
