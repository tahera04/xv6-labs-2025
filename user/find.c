#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

// regex bits from grep.c
int mh(char*, char*);
int ms(int, char*, char*);

int mm(char *r, char *t) {
  if(r[0] == '^')
    return mh(r+1, t);
  do {
    if(mh(r, t))
      return 1;
  } while(*t++ != '\0');
  return 0;
}

int mh(char *r, char *t) {
  if(r[0] == '\0')
    return 1;
  if(r[1] == '*')
    return ms(r[0], r+2, t);
  if(r[0] == '$' && r[1] == '\0')
    return *t == '\0';
  if(*t!='\0' && (r[0]=='.' || r[0]==*t))
    return mh(r+1, t+1);
  return 0;
}

int ms(int c, char *r, char *t) {
  do {
    if(mh(r, t))
      return 1;
  } while(*t!='\0' && (*t++==c || c=='.'));
  return 0;
}

void look(char *pth, char *pat) {
  char b[512], *pp;
  int f;
  struct dirent d;
  struct stat s;
  
  if ((f = open(pth, 0)) < 0) {
    return;
  }
  
  if (fstat(f, &s) < 0) {
    close(f);
    return;
  }
  
  if (s.type == T_FILE) {
    char *nm = pth + strlen(pth);
    while (nm > pth && nm[-1] != '/')
      nm--;
    if (mm(pat, nm))
      printf("%s\n", pth);
    close(f);
    return;
  }
  
  if (s.type != T_DIR) {
    close(f);
    return;
  }
  
  if (strlen(pth) + 1 + DIRSIZ + 1 > sizeof b) {
    close(f);
    return;
  }
  
  strcpy(b, pth);
  pp = b + strlen(b);
  *pp++ = '/';
  
  while (read(f, &d, sizeof(d)) == sizeof(d)) {
    if (d.inum == 0)
      continue;
    
    memmove(pp, d.name, DIRSIZ);
    pp[DIRSIZ] = 0;
    char *nm = pp;
    
    if (strcmp(nm, ".") == 0 || strcmp(nm, "..") == 0)
      continue;
    
    if (stat(b, &s) < 0)
      continue;
    
    if (mm(pat, nm))
      printf("%s\n", b);
    
    if (s.type == T_DIR)
      look(b, pat);
  }
  close(f);
}

int main(int ac, char *av[]) {
  if (ac != 3) {
    printf("Usage: find path regex\n");
    exit(1);
  }
  look(av[1], av[2]);
  exit(0);
}
