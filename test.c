
#include "server/server.h"


int main(void){
    char username1[] = "zhongj17";
    char password1[] = "1234567";


    char username2[] = "justinzhong";
    char password2[] = "abcdefg";


    writeToDatabase(username1, password1);
    writeToDatabase(username2, password2);
    return 0;


}