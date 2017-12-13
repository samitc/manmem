#ifndef _PERFORMANCE_H_
#define _PERFORMANCE_H_
#include <fstream>
#include <chrono>
extern std::ofstream perfFile;
void writePerformance(const char* funcName,double time,int iteration);
#define startPerformance auto \
    startTestMTimePerf = std::chrono::high_resolution_clock::now();
#define endPerformance(funcName,iteration) \
    auto endTestMTimePerf = std::chrono::high_resolution_clock::now();\
    std::chrono::duration<double> timePerfdiff = endTestMTimePerf-startTestMTimePerf;\
    writePerformance(funcName,timePerfdiff.count(),iteration);
#endif
