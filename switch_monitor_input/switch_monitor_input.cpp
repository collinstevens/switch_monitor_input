#include <windows.h>
#include <lowlevelmonitorconfigurationapi.h>
#include <highlevelmonitorconfigurationapi.h>
#include <physicalmonitorenumerationapi.h>
#include <iostream>
#include <vector>

BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC, LPRECT, LPARAM)
{
    DWORD cPhysicalMonitors;
    if (!GetNumberOfPhysicalMonitorsFromHMONITOR(hMonitor, &cPhysicalMonitors) || cPhysicalMonitors == 0)
    {
        std::cerr << "Failed to get number of physical monitors.\n";
        return TRUE;
    }

    LPPHYSICAL_MONITOR pPhysicalMonitors = (LPPHYSICAL_MONITOR)malloc(cPhysicalMonitors* sizeof(PHYSICAL_MONITOR));
    if (pPhysicalMonitors == NULL) return FALSE;
    if (!GetPhysicalMonitorsFromHMONITOR(hMonitor, cPhysicalMonitors, pPhysicalMonitors))
    {
        std::cerr << "Failed to get physical monitors.\n";
        return TRUE;
    }

    PHYSICAL_MONITOR physicalMonitor = pPhysicalMonitors[0];

    //DWORD cchStringLength = 0;
    //if (GetCapabilitiesStringLength(physicalMonitor.hPhysicalMonitor, &cchStringLength))
    //{
    //    std::vector<char> caps(cchStringLength + 1);
    //    if (CapabilitiesRequestAndCapabilitiesReply(physicalMonitor.hPhysicalMonitor, caps.data(), cchStringLength))
    //    {
    //        std::cout << "Capabilities:\n" << caps.data() << "\n";
    //    }
    //}
    //else
    //{
    //    std::cout << "Capabilities string not available.\n";
    //}

    if (SetVCPFeature(physicalMonitor.hPhysicalMonitor, 0x60, 0x0F))
    {
        //std::cout << "Input set successfully.\n";
    }
    else
    {
        std::cerr << "Failed to set input.\n";
    }

    free(pPhysicalMonitors);

    return TRUE;
}

int main()  
{  
   // Call EnumDisplayMonitors with the MonitorEnumProc callback
   if (!EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, 0))  
   {  
       std::cerr << "Failed to enumerate monitors.\n";  
       return 1;  
   }  

   return 0;  
}