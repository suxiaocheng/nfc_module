#include <stdio.h>
#include <stdlib.h>
#include "debug.h"
#include "rc.h"

char InitializeSystem()
{
	char ret = MI_OK;
	ret = PcdReset();
	if (ret == MI_ERR) {
		return ret;
	}
	PcdAntennaOff(); 
	PcdAntennaOn();

	return ret;
}

int main(int argc, char *argv[])
{
	int ret = 0;
	char status;
	unsigned char snr, buf[16];
	unsigned char DefaultKey[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
	unsigned char TagType[2];
	unsigned char SelectedSnr[4]; 

	DEBUG("program start\n");
	ret = setup_spi(1000);
	if (ret < 0) {
		ERR("setup spi fail\n");
		exit(0);
	} else {
		DEBUG("Setup spi pass\n");
	}

	if (InitializeSystem() != MI_OK) {
		ERR("Init system fail\n");
		return -1;
	}

	while (1)
	{
		DEBUG("Step1: start\n");
		status= PcdRequest(REQ_ALL,TagType);
		if(!status)
		{
			DEBUG("Step2: TagType: 0x%x%x", TagType[1], TagType[0]);
			status = PcdAnticoll(SelectedSnr);
			if(!status)
			{
				DEBUG("Step3: serial num: 0x%x%x%x%x\n", SelectedSnr[3], SelectedSnr[2], SelectedSnr[1], SelectedSnr[0]);
				status=PcdSelect(SelectedSnr);
				if(!status)
				{
					DEBUG("Step4: Pcd select ok\n");
					snr = 1;  //扇区号1
					status = PcdAuthState(KEYA, (snr*4+3), DefaultKey, SelectedSnr);// 校验1扇区密码，密码位于每一扇区第3块
					if(!status)
					{
						DEBUG("Setup5: Enter auth state\n");
						status = PcdRead((snr*4+0), buf);  // 读卡，读取1扇区0块数据到buf[0]-buf[16] 
						//status = PcdWrite((snr*4+0), buf);  // 写卡，将buf[0]-buf[16]写入1扇区0块
						if(!status)
						{
							DEBUG("Setup6: Read card ok:\n");
							dump_memory(buf, 16);
							//读写成功，点亮LED
							WaitCardOff();
							DEBUG("Setup7: END\n");
						}
					}
				}
			}
		}
		sleep(1);
	}

	exit_spi();
	return 0;
}
