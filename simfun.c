#include <stdio.h>
#include <stdlib.h>
int x0, x1, x2, x3, pc;

//i'm only having 5 memory locations, because I only use mem[1], mem[2]
int mem[] = {0,5,0,0,0};

void branch(char line);
void bz(char line);
void ldi(char line);
void add(char line);
void st(char line);
void ld(char line);
void neg(char line);
void listReg();

int main() {
    x0 = 0;
    x1 = 0;
    x2 = 0;
    x3 = 0;
    pc = 0;
    int x0i = x0;
    int x1i = x1;
    int x2i = x2;
    int x3i = x3;
    int pci = pc;
    FILE *ptr;
    char *lines;
    long fileLength;


    ptr = fopen("square.bin", "rb");

    // for(int j = 0; j < 5; j++){
    //   printf("mem[%d]: %d\n", j, mem[j]);
    // }

    // file length
    fseek(ptr, 0, SEEK_END);
    fileLength = ftell(ptr);
    fseek(ptr, 0, SEEK_SET);

    // Allocate memory for the buffer
    lines = (char *)malloc(fileLength);
    if (lines == NULL) {
        printf("Memory allocation error.\n");
        return 1;
    }

    fread(lines, 1, fileLength, ptr);

    fclose(ptr);

    for (long i = 0; i < fileLength; i++) {
        printf("Byte %ld: %02X\n", i, lines[i] & 0xFF); // Printing bytes in hexadecimal format
    }

    int i = 1;


    while (i==1){
      char line = lines[pc];
      //printf("%d: ", pc);

//BRANCH
      if((unsigned char)line >> 5 == 0){
        branch(line);
        pc++;
//BZ
      }else if(((unsigned char)line >> 5 == 1)){
        bz(line);
        pc++;
//LD
      }else if(((unsigned char)line >> 5 == 2)){
        ld(line);
        pc++;
//LDI
      }else if(((unsigned char)line >> 5 == 3)){
        ldi(line);
        pc++;
//ST
      }else if(((unsigned char)line >> 5 == 4 )){
        st(line);
        pc++;
//ADD
      }else if(((unsigned char)line >> 5 == 5)){
        add(line);
        pc++;
//NEG
      }else if(((unsigned char)line >> 5 == 6)){
        neg(line);
        pc++;
//HALT
      }else if(((unsigned char)line >> 5 == 7)){
        i--;
        //printf("\n");
      }
    }

    for(int j = 0; j < 5; j++){
      printf("mem[%d]: %d\n", j, mem[j]);
    }


    // Free allocated memory
    free(lines);
    printf("x0: %d --> %d\n", x0i, x0);
    printf("x1: %d --> %d\n", x1i, x1);
    printf("x2: %d --> %d\n", x2i, x2);
    printf("x3: %d --> %d\n", x3i, x3);
    printf("pc: %d --> %d\n", pci, pc);

    //printf("mem[2]: %d\n", mem[2]);


    return 0;
}

//returns pointer to a register
int * intToReg(char num){
  int *reg = &x0;
  if(num == 1){
    reg = &x1;
  }else if(num == 2){
    reg = &x2;
  }else if(num == 3){
    reg = &x3;
  }else if(num > 3 || num < 0){
    return 0;
  }
  return reg;
}

int charToSignedInt(char imm){
  int signedInt = imm;
  //only for 5 bit binary
  if((imm >> 4) == 1){
    imm = ((~imm)& 0x1f) + 1;
    signedInt = -imm;
  }
  return signedInt;
}

void listReg(){
  printf("x0: %d\n", x0);
  printf("x1: %d\n", x1);
  printf("x2: %d\n", x2);
  printf("x3: %d\n", x3);
  printf("pc: %d\n", pc);
}


void branch(char line){
  //printf("moves cp by %d\n", charToSignedInt (line & 0x1f));
  pc += charToSignedInt (line & 0x1f);

}
void bz(char line){
  if(x1 == 0){
    branch(line);
  }
}
void ld(char line){
  char s = (line>>2) & 0x03;
  char r = line & 0x03;
  *intToReg(r) = mem[*intToReg(s)];
  //printf("set reg %d to %d \n", );
}
void ldi(char line){
  x0 = charToSignedInt(line & 0x1f);

}
void st(char line){
  char s = (line>>2) & 0x03;
  char r = line & 0x03;
  mem[*intToReg(s)] = *intToReg(r);
}
void add(char line){
  char s = (line>>2) & 0x03;
  char r = line & 0x03;
  //printf("adds registers %02X and %02X which hold values %d and %d\n", r & 0xFF, s & 0xFF, *intToReg(r),*intToReg(s));
  *intToReg(r) = *intToReg(r) + *intToReg(s);
  //printf("register %02X is now %d\n", r & 0xFF, *intToReg(r));

}

void neg(char line){
  char r = line & 0x03;
  //printf("negates register %02X \n",  r & 0xFF);
  *intToReg(r) = -*intToReg(r);
  //printf("register %02X is now %d\n", r & 0xFF, *intToReg(r));


}
