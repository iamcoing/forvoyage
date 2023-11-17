#include <iostream>

using namespace std;

struct CarDV_LdcChannelAttr_s
{
    char CfgFilePath[256];
    union
    {
        char SensorCalibBinPath[256];
        struct
        {
            char Xmap[256];
            char Ymap[256];
        };
    };
};

static CarDV_LdcChannelAttr_s t1111 = {"test1", "test2", "test3"};

int main(int argc, const char * argv[])
{
    cout << t1111.CfgFilePath << " " 
    << t1111.Xmap << " " << t1111.Ymap << " " 
    << t1111.SensorCalibBinPath << endl;

    return 0;
}