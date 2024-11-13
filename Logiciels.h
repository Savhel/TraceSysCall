#ifndef LOGICIEL_H
#define LOGICIEL_H

#include <string>
#include <windows.h>

class Logiciel {
public:
    std::string nom;
    DWORD idProcessus;
    DWORD nbThreads;
    DWORD idParent;
    LONG priorité;
    SIZE_T memoireUtilisee;
    ULONGLONG tempsProcesseur;

    Logiciel() : nom(""), idProcessus(0), nbThreads(0), idParent(0), priorité(0), memoireUtilisee(0), tempsProcesseur(0) {}
    Logiciel(std::string n, DWORD idP, DWORD nbT, DWORD idPar, LONG prio, SIZE_T memUtil, ULONGLONG cpuTime)
        : nom(n), idProcessus(idP), nbThreads(nbT), idParent(idPar), priorité(prio), memoireUtilisee(memUtil), tempsProcesseur(cpuTime) {}
};

#endif // LOGICIEL_H
