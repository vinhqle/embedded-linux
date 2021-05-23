#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

typedef enum EM_COMM_MSG_TYPE{
	OPT_LED_CTRL_REQ		= 0x3001,
}EM_COMM_MSG_TYPE;

typedef struct st_Comm_Header
{
	unsigned int dwMessageID;
	unsigned int dwTotalMsgLen;
}st_Comm_Header;

typedef struct st_Body_Led_Ctrl_Req {
	unsigned int uiFuncID;
	unsigned int uiSeqNo;
    unsigned int uiLedState;
    unsigned int uiPwm;
}st_Body_Led_Ctrl_Req;

typedef struct st_Led_Ctrl_Req {
	st_Comm_Header header;
    st_Body_Led_Ctrl_Req msg;
}st_Led_Ctrl_Req;

int main (void){
    
    while(1){
        int fd = open("/dev/ex04_file_operation_device", O_WRONLY);
        if (fd == -1) {
            perror("Unable to open /dev/ex04_file_operation_device");
            exit(1);
        }

        st_Led_Ctrl_Req lcReq;
        lcReq.header.dwMessageID = OPT_LED_CTRL_REQ;
        lcReq.header.dwTotalMsgLen = sizeof(st_Led_Ctrl_Req);
        lcReq.msg.uiFuncID = 0;
        lcReq.msg.uiSeqNo = 0;
        lcReq.msg.uiLedState = 0;
        lcReq.msg.uiPwm = 0;

        char* byteSnd = (char*)&lcReq;
        if (write(fd, byteSnd, sizeof(st_Led_Ctrl_Req)) != sizeof(st_Led_Ctrl_Req)) {
            perror("Error writing to /dev/ex04_file_operation_device");
            close(fd);
            exit(1);
        }
        close(fd);

        sleep(1);

        fd = open("/dev/ex04_file_operation_device", O_WRONLY);
        if (fd == -1) {
            perror("Unable to open /dev/ex04_file_operation_device");
            exit(1);
        }

        lcReq.header.dwMessageID = OPT_LED_CTRL_REQ;
        lcReq.header.dwTotalMsgLen = sizeof(st_Led_Ctrl_Req);
        lcReq.msg.uiFuncID = 0;
        lcReq.msg.uiSeqNo = 0;
        lcReq.msg.uiLedState = 1;
        lcReq.msg.uiPwm = 0;

        byteSnd = (char*)&lcReq;
        if (write(fd, byteSnd, sizeof(st_Led_Ctrl_Req)) != sizeof(st_Led_Ctrl_Req)) {
            perror("Error writing to /dev/ex04_file_operation_device");
            close(fd);
            exit(1);
        }
        close(fd);

        sleep(1);
    }

    return 0;
}