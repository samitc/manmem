#ifndef _PERFORMANCE_H_
#define _PERFORMANCE_H_
#include <fstream>
#include <chrono>
extern std::ofstream perfFile;
void writePerformance(const char* funcName,double time,int iteration);
#define startPerformance \
    auto endTestMTimePerf = std::chrono::high_resolution_clock::now();\
    auto startTestMTimePerf = endTestMTimePerf;\
    std::chrono::duration<double> timePerfdiff = std::chrono::duration<double>(0);
#define resumePerformance \
    startTestMTimePerf = std::chrono::high_resolution_clock::now();
#define puasePerformance \
    endTestMTimePerf = std::chrono::high_resolution_clock::now();\
    timePerfdiff += endTestMTimePerf-startTestMTimePerf;\
    startTestMTimePerf=endTestMTimePerf;
#define endPerformance(funcName,iteration) \
    puasePerformance \
    writePerformance(funcName,timePerfdiff.count(),iteration);
#endif
