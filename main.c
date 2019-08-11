#include <stdio.h>
#include <stdlib.h>
#include "debug.h"
#include "rc.h"

void InitializeSystem()
{
	PcdReset();
	PcdAntennaOff(); 
	PcdAntennaOn();  
}

int main(int argc, char *argv[])
{
	int ret = 0;
	char status;
	unsigned char snr, buf[16];
	unsigned char DefaultKey[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
	unsigned char TagType[2];
	unsigned char SelectedSnr[4]; 

	ret = setup_spi(1000);
	if (ret < 0) {
		exit(0);
	}

	InitializeSystem();

	while (1)
	{
		status= PcdRequest(REQ_ALL,TagType);
		if(!status)
		{
			status = PcdAnticoll(SelectedSnr);
			if(!status)
			{
				status=PcdSelect(SelectedSnr);
				if(!status)
				{
					snr = 1;  //扇区号1
					status = PcdAuthState(KEYA, (snr*4+3), DefaultKey, SelectedSnr);// 校验1扇区密码，密码位于每一扇区第3块
					if(!status)
					{
						status = PcdRead((snr*4+0), buf);  // 读卡，读取1扇区0块数据到buf[0]-buf[16] 
						//status = PcdWrite((snr*4+0), buf);  // 写卡，将buf[0]-buf[16]写入1扇区0块
						if(!status)
						{
							//读写成功，点亮LED
							WaitCardOff();
							}
						}
					}
				}
		}	
	}



	exit_spi();
	return 0;
}
