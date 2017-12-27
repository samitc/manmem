#ifndef _PERFORMANCE_H_
#define _PERFORMANCE_H_
#include <fstream>
#include <chrono>
extern std::ofstream perfFile;
void writePerformance(const char* funcName,double time,int iteration);
#define startPerformance(name) \
    auto endTestMTimePerf##name = std::chrono::high_resolution_clock::now();\
    auto startTestMTimePerf##name = endTestMTimePerf##name;\
    std::chrono::duration<double> timePerfdiff##name = std::chrono::duration<double>(0);
#define resumePerformance(name) \
    startTestMTimePerf##name = std::chrono::high_resolution_clock::now();
#define puasePerformance(name) \
    endTestMTimePerf##name = std::chrono::high_resolution_clock::now();\
    timePerfdiff##name += endTestMTimePerf##name-startTestMTimePerf##name;\
    startTestMTimePerf##name=endTestMTimePerf##name;
#define endPerformance(name,funcName,iteration) \
    puasePerformance(name) \
    writePerformance(funcName,timePerfdiff##name.count(),iteration);
#endif
