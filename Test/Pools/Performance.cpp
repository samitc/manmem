#include "Performance.h"
std::ofstream perfFile("perf.out", std::ofstream::out);
void writePerformance(const char *funcName,double time,int iteration)
{
    perfFile << funcName << "=" << time/iteration << std::endl;
    perfFile.flush();
}
