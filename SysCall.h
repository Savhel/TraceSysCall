#ifndef SYSCALL_H
#define SYSCALL_H

#include <windows.h>
#include <tlhelp32.h>
#include <psapi.h>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include "Logiciels.h"

class Syscall {
public:
    std::unordered_map<DWORD, Logiciel> logiciels;

    void ListRunningProcesses() {
        HANDLE hProcessSnap;
        PROCESSENTRY32 pe32;

        hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (hProcessSnap == INVALID_HANDLE_VALUE) {
            std::cerr << "CreateToolhelp32Snapshot (of processes) failed." << std::endl;
            return;
        }

        pe32.dwSize = sizeof(PROCESSENTRY32);

        if (!Process32First(hProcessSnap, &pe32)) {
            std::cerr << "Process32First failed." << std::endl;
            CloseHandle(hProcessSnap);
            return;
        }

        do {
            SIZE_T memUtil = 0;
            ULONGLONG cpuTime = 0;

            HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pe32.th32ProcessID);
            if (hProcess != NULL) {
                PROCESS_MEMORY_COUNTERS pmc;
                if (GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc))) {
                    memUtil = pmc.WorkingSetSize;
                }

                FILETIME ftCreation, ftExit, ftKernel, ftUser;
                if (GetProcessTimes(hProcess, &ftCreation, &ftExit, &ftKernel, &ftUser)) {
                    ULARGE_INTEGER userSystemTime;
                    userSystemTime.LowPart = ftUser.dwLowDateTime;
                    userSystemTime.HighPart = ftUser.dwHighDateTime;
                    cpuTime = userSystemTime.QuadPart;
                }
                CloseHandle(hProcess);
            }

            if (logiciels.find(pe32.th32ProcessID) == logiciels.end()) {
                Logiciel logiciel(pe32.szExeFile, pe32.th32ProcessID, pe32.cntThreads, pe32.th32ParentProcessID, pe32.pcPriClassBase, memUtil, cpuTime);
                logiciels[pe32.th32ProcessID] = logiciel;
                AfficherLogiciel(logiciel);
            } else {
                Logiciel& logiciel = logiciels[pe32.th32ProcessID];
                bool changed = false;

                if (logiciel.memoireUtilisee != memUtil) {
                    logiciel.memoireUtilisee = memUtil;
                    changed = true;
                }
                if (logiciel.tempsProcesseur != cpuTime) {
                    logiciel.tempsProcesseur = cpuTime;
                    changed = true;
                }

                if (changed) {
                    AfficherLogiciel(logiciel);
                }
            }
        } while (Process32Next(hProcessSnap, &pe32));

        CloseHandle(hProcessSnap);
    }

    void AfficherLogiciel(const Logiciel& logiciel) {
        // Conversion du temps processeur en secondes (en supposant que la fréquence du processeur est de 1 GHz)
        double tempsProcesseurSec = static_cast<double>(logiciel.tempsProcesseur) / 10000000.0;

        std::cout << "\nNOM DU PROCESSUS: " << logiciel.nom;
        std::cout << "\n  ID du processus        = 0x" << std::hex << logiciel.idProcessus;
        std::cout << "\n  Nombre de threads      = " << std::dec << logiciel.nbThreads;
        std::cout << "\n  ID du processus parent = 0x" << std::hex << logiciel.idParent;
        std::cout << "\n  Priorité de base       = " << logiciel.priorité;
        std::cout << "\n  Mémoire utilisée       = " << std::dec << logiciel.memoireUtilisee << " octets";
        std::cout << "\n  Temps processeur       = " << tempsProcesseurSec << " secondes" << std::endl;
    }
};

#endif // SYSCALL_H
