#include <kernel.h>
#include <taihen.h>

static tai_hook_ref_t hook_ref[2];
static SceUID hook_id[2];

#define WEBKIT_NAME "SceWebKitModule"
#define UA_FILE_PATH "ux0:data/external/webcore/user_agent.txt"

static char newUA[1024] = { 0 };

int setUA_patched(int unk1, char *ua)
{
	return TAI_CONTINUE(int, hook_ref[1], unk1, newUA);
}

SceUID	sceKernelLoadStartModule_patched(
	const char				*moduleFileName,
	SceSize					args,
	const void				*argp,
	SceUInt32				flags,
	const SceKernelLoadModuleOpt	*pOpt,
	int						*pRes
)
{
	SceUID res = TAI_CONTINUE(SceUID, hook_ref[0], moduleFileName, args, argp, flags, pOpt, pRes);
	if (res > 0 && sceClibStrstr(moduleFileName, WEBKIT_NAME)) {
		hook_id[1] = taiHookFunctionImport(&hook_ref[1], TAI_MAIN_MODULE, 0x026D334F, 0x7AB4C254, setUA_patched);
		taiHookRelease(hook_id[0], hook_ref[0]);
		hook_id[0] = -1;
	}
	return res;
}

int module_start(SceSize args, const void * argp)
{
	hook_id[0] = -1;
	hook_id[1] = -1;

	SceUID fd = sceIoOpen(UA_FILE_PATH, SCE_O_RDONLY, 0);
	if (fd <= 0) {
		return SCE_KERNEL_START_SUCCESS;
	}

	sceIoRead(fd, newUA, sizeof(newUA));
	sceIoClose(fd);

	hook_id[0] = taiHookFunctionImport(&hook_ref[0], TAI_MAIN_MODULE, 0xCAE9ACE6, 0x2DCC4AFA, sceKernelLoadStartModule_patched);

	return SCE_KERNEL_START_SUCCESS;
}

int module_stop(SceSize args, const void * argp)
{
	if (hook_id[0] > 0) taiHookRelease(hook_id[0], hook_ref[0]);
	if (hook_id[1] > 0) taiHookRelease(hook_id[1], hook_ref[1]);
	return SCE_KERNEL_STOP_SUCCESS;
}