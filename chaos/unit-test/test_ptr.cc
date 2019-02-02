#include<stdio.h>
#include<string.h>
int main()
{
  char buffer[1024] = {0,1,2,3,4,5,6,7};
  int iTest = 0x12345678;//16进制1位表示4个bit，itest表示4个byte
  int *p = (int *)(buffer + 7);//指针+数字表示表示截取数组，buffer第7位开始以后的数组
   printf("intlen: %d itestlen:%d itest:%d\n",sizeof(int),sizeof(iTest),iTest);
  memcpy(p, &iTest, sizeof(iTest));//iTest
  printf("%x\n", buffer[6]);
  printf("%x\n", buffer[9]);
  return 0;
}
