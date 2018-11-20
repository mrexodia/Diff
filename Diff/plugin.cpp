#include "plugin.h"

bool readAllBytes(const char* szFileName, std::vector<unsigned char>& data)
{
    auto result = false;
    auto hFile = CreateFileA(szFileName, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, 0);
    if (hFile != INVALID_HANDLE_VALUE)
    {
        data.resize(GetFileSize(hFile, nullptr));
        DWORD read = 0;
        result = !!ReadFile(hFile, data.data(), (DWORD)data.size(), &read, nullptr);
        CloseHandle(hFile);
    }
    return result;
}

static bool cbDiff(int argc, char* argv[])
{
    if (argc < 3)
    {
        dputs("usage: diff addr, file");
        return false;
    }
    auto addr = Eval(argv[1]);
    duint size = 0;
    auto base = DbgMemFindBaseAddr(addr, &size);
    if (!base)
    {
        dprintf("failed to get base+size for %p\n", addr);
        return false;
    }
    std::vector<unsigned char> memData, fileData;
    memData.resize(size);
    if (!DbgMemRead(base, memData.data(), size))
    {
        dprintf("failed to read memory %p[%p]\n", base, size);
        return false;
    }
    if (!readAllBytes(argv[2], fileData))
    {
        dprintf("failed to read file '%s'\n", argv[2]);
        return false;
    }
    if (fileData.size() != memData.size())
    {
        dprintf("sections are not same size %p != %p\n", fileData.size(), memData.size());
    }
    GuiReferenceInitialize("diff");
	uint32_t differentBytes = 0;
    for (size_t i = 0; i < min(memData.size(), fileData.size()); i++)
    {
		if (memData[i] != fileData[i])
		{
			DbgFunctions()->MemPatch(base + i, &fileData[i], 1);
			differentBytes++;
		}
    }
    dprintf("success, %u different bytes found!\n", differentBytes);
    GuiUpdateAllViews();
    return true;
}

//Initialize your plugin data here.
bool pluginInit(PLUG_INITSTRUCT* initStruct)
{
    _plugin_registercommand(pluginHandle, "diff", cbDiff, true);
    return true; //Return false to cancel loading the plugin.
}

//Deinitialize your plugin data here.
void pluginStop()
{
}

//Do GUI/Menu related things here.
void pluginSetup()
{
}
