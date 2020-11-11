
#include "server/server.h"


int main(int argc, char ** argv){
    char username1[] = "zhongj17";
    char password1[] = "1234567";
    char username2[] = "justinzhong";
    char password2[] = "abcdefg";
    char username3[] = "a_username";
    char password3[] = "a_password";
    writeToDatabase(username1, password1);
    writeToDatabase(username2, password2);
    writeTodatabase(username3, password3);

    printf("%d vs 1 \n", checkPW("zhongj17", "1234567"));
    printf("%d vs -1\n", checkPW("justinzhong", "somewrongpw"));
    printf("%d vs -2\n", checkPW("somewrongusername", "wrongpw"));
    return 0;


}