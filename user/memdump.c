#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"

// memdump
void memdump(char *fmt, char *data);

int
main(int argc, char *argv[])
{
  if(argc == 1){
    printf("Example 1:\n");
    int arr[2] = { 61810, 2025 };
    memdump("ii", (char*) arr);
    
    printf("Example 2:\n");
    memdump("S", "a string");
    
    printf("Example 3:\n");
    char *st = "another";
    memdump("s", (char *) &st);

    struct thing {
      char *p;
      int n1;
      short n2;
      char ch;
      char words[8];
    } ex;
    
    ex.p = "hello";
    ex.n1 = 1819438967;
    ex.n2 = 100;
    ex.ch = 'z';
    strcpy(ex.words, "xyzzy");
    
    printf("Example 4:\n");
    memdump("pihcS", (char*) &ex);
    
    printf("Example 5:\n");
    memdump("sccccc", (char*) &ex);
  } else if(argc == 2){
    char buf[512];
    int k = 0;
    memset(buf, '\0', sizeof(buf));
    while(k < sizeof(buf)){
      int more = read(0, buf + k, sizeof(buf) - k);
      if(more <= 0)
        break;
      k += more;
    }
    memdump(argv[1], buf);
  } else {
    printf("Usage: memdump [format]\n");
    exit(1);
  }
  exit(0);
}

void
memdump(char *fmt, char *data)
{
  char *q = data;  // move step by step
  for (int j = 0; fmt[j] != '\0'; j++) {
    char f = fmt[j];
    if(f == 'i'){   // int 4 byte
      int *x = (int *)q;
      printf("%d\n", *x);
      q += 4;
    }
    else if(f == 'p'){  // pointer 8 byte
      long long *y = (long long *)q;
      printf("%llx\n", *y);
      q += 8;
    }
    else if(f == 'h'){  // short 2 byte
      short *z = (short *)q;
      printf("%d\n", *z);
      q += 2;
    }
    else if(f == 'c'){  // char 1 byte
      char *ch = (char *)q;
      printf("%c\n", *ch);
      q += 1;
    }
    else if(f == 's'){  // pointer to string
      char **pp = (char **)q;
      printf("%s\n", *pp);
      q += 8;
    }
    else if(f == 'S'){  // string in data
      char *str = (char *)q;
      printf("%s\n", str);
      q += strlen(str) + 1; // skip after \0
    }
    else{
      printf("bad fmt: %c\n", f);
    }
  }
}

