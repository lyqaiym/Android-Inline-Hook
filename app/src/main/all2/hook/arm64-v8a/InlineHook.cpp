#include <vector>
#include <dlfcn.h>

#ifndef PAGE_SIZE
#define PAGE_SIZE 4096
#endif

#define PAGE_START(addr)    (~(PAGE_SIZE - 1) & (addr))
#define SET_BIT0(addr)        (addr | 1)
#define CLEAR_BIT0(addr)    (addr & 0xFFFFFFFE)
#define TEST_BIT0(addr)        (addr & 1)

#define ACTION_ENABLE    0
#define ACTION_DISABLE    1

extern "C"
{
#include "Ihook.h"
#include "hooktest.h"
}

//void ModifyIBored() __attribute__((constructor));
//void before_main() __attribute__((constructor));

typedef std::vector<INLINE_HOOK_INFO *> InlineHookInfoPVec;
static InlineHookInfoPVec gs_vecInlineHookInfo;     //管理HOOK点

void before_main() {
    LOGI("Hook is auto loaded!\n");
}

/**
 * 对外inline hook接口，负责管理inline hook信息
 * @param  pHookAddr     要hook的地址
 * @param  onCallBack    要插入的回调函数
 * @return               inlinehook是否设置成功（已经设置过，重复设置返回false）
 */
bool InlineHook(void *pHookAddr, void (*onCallBack)(struct user_pt_regs *)) {
    bool bRet = false;
    LOGI("InlineHook:pHookAddr=%p",pHookAddr);

    if (pHookAddr == NULL || onCallBack == NULL) {
        return bRet;
    }
    unsigned long ulPageSize = sysconf(_SC_PAGESIZE); //得到页的大小
    int iProtect = PROT_READ | PROT_WRITE | PROT_EXEC;
    unsigned long ulNewPageStartAddress = (unsigned long)(pHookAddr) & ~(ulPageSize - 1);
    int iRet = mprotect((void*)ulNewPageStartAddress, ulPageSize, iProtect);
    if(-1 == iRet)
    {
        LOGI("InlineHook:mprotect error:%s", strerror(errno));
        return bRet;
    }
    INLINE_HOOK_INFO *pstInlineHook = new INLINE_HOOK_INFO();
    pstInlineHook->pHookAddr = pHookAddr;
    pstInlineHook->onCallBack = onCallBack;

    if (HookArm(pstInlineHook) == false) {
        LOGI("HookArm fail.");
        delete pstInlineHook;
        return bRet;
    }


    gs_vecInlineHookInfo.push_back(pstInlineHook);
    return true;
}

/**
 * 对外接口，用于取消inline hook
 * @param  pHookAddr 要取消inline hook的位置
 * @return           是否取消成功（不存在返回取消失败）
 */
bool UnInlineHook(void *pHookAddr) {
    bool bRet = false;

    if (pHookAddr == NULL) {
        return bRet;
    }

    InlineHookInfoPVec::iterator itr = gs_vecInlineHookInfo.begin();
    InlineHookInfoPVec::iterator itrend = gs_vecInlineHookInfo.end();

    for (; itr != itrend; ++itr) {
        if (pHookAddr == (*itr)->pHookAddr) {
            INLINE_HOOK_INFO *pTargetInlineHookInfo = (*itr);

            gs_vecInlineHookInfo.erase(itr);
            if (pTargetInlineHookInfo->pStubShellCodeAddr != NULL) {
                delete pTargetInlineHookInfo->pStubShellCodeAddr;
            }
            if (pTargetInlineHookInfo->ppOldFuncAddr) {
                delete *(pTargetInlineHookInfo->ppOldFuncAddr);
            }
            delete pTargetInlineHookInfo;
            bRet = true;
        }
    }

    return bRet;
}

/**
 * 针对IBored应用，通过inline hook改变游戏逻辑的测试函数
 */
void ModifyIBored(char *__filename, int target_offset,callback callback1) {
    LOGI("In IHook's ModifyIBored.");
    void *pModuleBaseAddr = GetModuleBaseAddr(-1, __filename); //目标so的名称
    if (pModuleBaseAddr == 0) {
        pModuleBaseAddr = dlopen(__filename, RTLD_NOW);
    }
    if (pModuleBaseAddr == 0) {
        LOGI("get module base error.");
        return;
    }

    uint64_t uiHookAddr = (uint64_t) pModuleBaseAddr + target_offset; //真实Hook的内存地址

    LOGI("ModifyIBored:uiHookAddr=%ld",uiHookAddr);
    InlineHook((void *) (uiHookAddr), callback1); //*第二个参数就是Hook想要插入的功能处理函数*
}

bool UnInlineHook(char *__filename, int target_offset){
    LOGI("UnInlineHook ModifyIBored.");
    void *pModuleBaseAddr = GetModuleBaseAddr(-1, __filename); //目标so的名称
    if (pModuleBaseAddr == 0) {
        LOGI("get module base error.");
        return false;
    }

    uint64_t uiHookAddr = (uint64_t) pModuleBaseAddr + target_offset; //真实Hook的内存地址
    LOGI("UnInlineHook:uiHookAddr=%ld",uiHookAddr);
    UnInlineHook((void *)uiHookAddr);
    return true;
}