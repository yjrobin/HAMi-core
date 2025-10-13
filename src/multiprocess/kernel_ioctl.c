#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/sched.h>

#include "include/log_utils.h"

#define NV_IOCTL_MAGIC      'F'
#define NV_IOCTL_BASE       200
// yj start
#define NV_4PD_VGPU_CTL              (NV_IOCTL_BASE + 19)
// yj end

typedef struct nv_4pd_vgpu_ctl
{
    u_int32_t   mem_limit;
} nv_4pd_vgpu_ctl_t;

#define WR_VALUE _IOW(NV_IOCTL_MAGIC,NV_4PD_VGPU_CTL,nv_4pd_vgpu_ctl_t*)

int write_kernel(int gpu_id, size_t limit)
{
        int fd;
        nv_4pd_vgpu_ctl_t vgpu_ctl;
        int ret;
        char device_name[20];
	char *s = getenv("CUDA_KERNEL_VALIDATION");
	if ((s == NULL) || (strlen(s)==0)){
  	      return 0;
	}
        sprintf(device_name, "/dev/nvidia%d", gpu_id);
        //LOG_MSG("Opening driver: %s", device_name);
        fd = open(device_name, O_RDWR);
        if(fd < 0) {
                //printf("Cannot open device file...\n");
                return 0;
        }
	vgpu_ctl.mem_limit = limit >> 20;
        LOG_MSG("Writing to driver %d %u\n",gpu_id, vgpu_ctl.mem_limit);
        if (ret = ioctl(fd, WR_VALUE, &vgpu_ctl))
        {
            LOG_ERROR("ioctl return error : %d\n", ret);
        }
        close(fd);
	return 0;
}