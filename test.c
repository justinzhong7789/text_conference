
#include "server/server.h"


int main(int argc, char ** argv){
    char u1[] = "aaaaaaaa";
    char p1[] = "11111111";
    char u2[] = "bbbbbbbb";
    char p2[] = "22222222";
    char u3[] = "cccccccc";
    char p3[] = "33333333";
    char u4[] = "dddddddd";
    char p4[] = "44444444";
    //writeToDatabase(u1, p1);
    //writeToDatabase(u2, p2);
    //writeToDatabase(u3, p3);
    //writeToDatabase(u4, p4);

    int check = checkPW(u1, "1234567");
    printf("%d vs -1 \n", check);
    printf("\n\n");
    check = checkPW(u1, p1);
    printf("%d vs 0\n", check);
    printf("\n\n");
    check = checkPW("somewrongusername", "wrongpw");
    printf("%d vs -2\n", check);
    printf("\n\n");

    check = checkPW(u3, p3);
    printf("%d vs 0\n");
    return 0;


}