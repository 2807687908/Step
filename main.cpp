#include <iostream>
#include "RobotCom.hpp"
#include <thread>
#include <atomic>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <cstdlib>
using namespace STEPROBOTSDK; //定义客户端
using namespace std;
RobotComClient robotClient;
atomic<bool> new_output(false); //定义一个原子布尔变量，用于表示是否有新的输出
mutex mtx; //定义一个互斥锁
condition_variable cv; //定义一个条件变量

// 模拟主任务的工作函数
void worker_function() {
    // 模拟长时间计算或操作（实际替换为你的业务逻辑）
    this_thread::sleep_for(chrono::seconds(15)); // 模拟业务逻辑运行时间
    
    // 这里应该有你的输出逻辑，输出时设置 new_output 为 true
    // 例如：
    cout << "任务执行中..." << endl;
    new_output.store(true); // 标记有新的输出行
    cv.notify_one(); // 通知监控线程
}

// 监控线程函数
void monitor_function() {
    unique_lock<mutex> lock(mtx);
    // 等待 1000 秒或直到有新的输出行产生
    if (cv.wait_for(lock, chrono::seconds(1000), [] { return new_output.load(); })) {
        cout << "检测到新的输出行，继续执行..." << endl;
        new_output.store(false); // 重置标记
    } else {
        cout << "1000 秒内无新输出行，强行停止程序运行..." << endl;
        exit(EXIT_FAILURE); // 强行停止程序
    }
}

int main() {
    string sdkversion,projectname,programname,teachrefsysname,toolname;
    MessageData errormassage;
    CartDynState cartdynstate;
    OPERATIONMODE operationmode;
    REFSYS jogrefsys;
    PROGRAMSTATE programstate;
    PROGRAMMODE programmode;

    
  sdkversion = robotClient.SDKVersion();
    cout<<"当前SDK版本为："<<sdkversion<<endl;
    
system("ping 192.168.77.1 -c 2");
    
    int result = 0;//初始化，连接服务器
    result = robotClient.init("192.168.77.1", 30312);//检查初始化是否成功
    if (result != 0)
  {
   std::cout <<"初始化失败，结果为: "<< result << std::endl;
  }   //先注释检查初始化的部分，检验SDK版本功能是否能够正常运行（应该是正常的，未接入机器的话应该不会有输出结果）
  else
  {
   std::cout <<"初始化成功" << std::endl;
  }
//初始化是重点之一

// 创建工作线程和监控线程
    thread worker_thread(worker_function);
    thread monitor_thread(monitor_function);
   std::cout <<"初始化成功" << std::endl;

    
    int i;
    int enablestate = robotClient.getMotorEnableState();// 获取使能状态
   std::cout <<"初始化成功" << std::endl;

    RobotDI robotdi={};
    for (int i = 0; i < 64; i++) {
         robotdi.m_DI[i] = robotClient.getRobotDI().m_DI[i]; //获取数字输入
    }//重点之一
   std::cout <<"初始化成功1" << std::endl;
    /*
    int di1 = robotdi.m_DI[0] & 0x01; //读取DI1的值,1为TRUE，0为FALSE
    int di10 = (robotdi.m_DI[0] >> 9) & 0x01;//读取DI10的值,1为TRUE，0为FALSE
    int di20 = (robotdi.m_DI[0] >> 19) & 0x01;//读取DI20的值,1为TRUE，0为FALSE
    int di33 = (robotdi.m_DI[1]) & 0x01;//读取 DI33 的值,1 为 TRUE，0 为 FALSE
    */     //以上是读取数据注入的值，可以根据需要修改

    RobotDO robotdo={};
    for (int i = 0; i < 64; i++) {
         robotdo.m_DO[i] = robotClient.getRobotDO().m_DO[i]; //获取数字输出
    }//重点之一
   std::cout <<"初始化成功2" << std::endl;
    /*
    int do1 = robotdo.m_DO[0] & 0x01; //读取DO1的值,1为TRUE，0为FALSE
    int do10 = (robotdo.m_DO[0] >> 9) & 0x01;//读取DO10的值,1为TRUE，0为FALSE
    int do20 = (robotdo.m_DO[0] >> 19) & 0x01;//读取DO20的值,1为TRUE，0为FALSE
    int do33 = (robotdo.m_DO[1]) & 0x01;//读取 DO33 的值,1 为 TRUE，0 为 FALSE
    */     //以上是读取数据输出的值，可以根据需要修改

    RobotAI robotai={};
    for (int i = 0; i < 32; i++) {
         robotai.m_AI[i] = robotClient.getRobotAI().m_AI[i]; //获取模拟输入
    }//重点之一
   std::cout <<"初始化成功3" << std::endl;
    /*
    short ai1 = robotai.m_AI[0]; //读取AI1的值
    */     //以上是读取模拟输入的值，可以根据需要修改

    RobotAO robotao={};
    for (int i = 0; i < 32; i++) {
         robotao.m_AO[i] = robotClient.getRobotAO().m_AO[i]; //获取模拟输出
    }//重点之一
   std::cout <<"初始化成功4" << std::endl;
    /*
    short ao1 = robotao.m_AO[0]; //读取AO1的值
    */     //以上是读取模拟输出的值，可以根据需要修改

    RobotInfoStruct robotinfo = {}; //获取机器人信息
    string robotname,robotNo;
    result = robotClient.RequestRobotInfoCmd(robotinfo);
    if(result != 0) 
    {
        cout<<"获取机器人信息失败,结果为："<<result<<endl;
        
   std::cout <<"初始化成功5" << std::endl;
    }
    else
    {
        robotname = robotinfo.m_Name;//机器人名称赋值
        robotNo = robotinfo.m_RobotSNo;//机器人序号赋值
        
   std::cout <<"初始化成功6" << std::endl;
    }

    ElecBoxInfoStruct elecboxinfo = {}; //定义控制柜信息结构体变量
    string elecboxname,elecboxNo;
    result = robotClient.RequestElecBoxInfoCmd(elecboxinfo);
    if(result != 0)
    {
        cout<<"获取控制柜信息失败,结果为："<<result<<endl;
   std::cout <<"初始化成功7" << std::endl;
    }
    else
    {
        elecboxname = elecboxinfo.m_ElecBoxName;//控制柜名称赋值
        elecboxNo = elecboxinfo.m_ElecBoxSNo;//控制柜序号赋值
   std::cout <<"初始化成功8" << std::endl;
    }

    //上使能
    result = 0; //当前机器人未使能
    if (robotClient.getMotorEnableState() == 0)
    {
        result = robotClient.EnableMotorCmd(); //上使能
        if (result != 0) //检查是否成功
        {
            cout << "上使能执行失败，结果为:" << result << endl;
            std::cout <<"初始化成功9" << std::endl;
            
        }
        else
        {
            for (int i = 0; i < 20; i++)
            {
                if (robotClient.getMotorEnableState() == 1)
                {
                    break; //如果为使能状态，跳出for循环
                }
                sleep(50);//50ms后再次获取状态
            }
            std::cout <<"初始化成功10" << std::endl;
            sleep(300);//使能到其他操作的间隔时间为300ms，少于这个时间无法执行下一步
        }
    }

    //断使能
    result = 0;//当前机器人使能状态
    if (robotClient.getMotorEnableState() == 1)
    {
        result = robotClient.EnableMotorCmd(); //断使能
        if( result != 0) //检查是否成功
        {
            cout << "断使能执行失败，结果为:" << result << endl;
            std::cout <<"初始化成功11" << std::endl;
        }
        else
        {
            for (int i = 0; i < 20; i++)
            {
                if (robotClient.getMotorEnableState() == 0)
                {
                    break; //如果为断使能状态，跳出for循环
                }
                sleep(50);//50ms后再次获取状态
            }
            std::cout <<"初始化成功12" << std::endl;
        }
    }

    //设置手动模式
    result = 0; //当前机器人未处于手动模式
    result = robotClient.SetModeCmd(MANUAL); //操作模式设置为手动
    if(result != 0)
    {
        cout<<"设置手动模式失败，结果为："<<result<<endl;
        std::cout <<"初始化成功13" << std::endl;
    }
    else
    {
        for (int i = 0; i < 20; i++)
        {
            if (robotClient.getOperationMode() == eManualHigh)
            {
                break; //如果为手动模式，跳出for循环
            }
            sleep(50);//50ms后再次获取状态
        }
        std::cout <<"初始化成功14" << std::endl;
    }
/*
    //设置自动模式
    result = 0; //当前机器人未处于自动模式
    result = robotClient.SetModeCmd(AUTO); //操作模式设置为自动
    if(result != 0)
    {
        cout<<"设置自动模式失败，结果为："<<result<<endl;
    }
    else
    {
        for (int i = 0; i < 20; i++)
        {
            if (robotClient.getOperationMode() == eAutomatic)
            {
                break; //如果为自动模式，跳出for循环
            } sleep(50);//50ms后再次获取状态
        }
       
    }

    //非外部启动
    result = 0; //当前机器人未处于非外部启动模式
    if(robotClient.getOperationMode() != eAutomaticExternal)
    {
        if(robotClient.getMotorEnableState() == 1)//机器人使能状态
        {
            result = robotClient.SetModeCmd(START);//启动加载的程序
            if(result != 0)
            {
                cout<<"非外部启动失败，结果为："<<result<<endl;
            }
            else
            {
                for (i=0;i<20;i++)
                {
                    if(robotClient.getProgramState()==eRun)
                    {
                        break; //如果为运行状态，跳出for循环
                    }
                    sleep(50);//50ms后再次获取状态
                }
            }
        }
    }

    //非外部停止
    result = 0; //当前机器人未处于非外部停止模式
    if(robotClient.getOperationMode() != eAutomaticExternal)
    {
        if(robotClient.getProgramState()==eRun)//程序运行状态
        {
            result = robotClient.SetModeCmd(STOP);//停止加载的程序
            if(result != 0)
            {
                cout<<"非外部停止失败，结果为："<<result<<endl;
            }
            else
            {
                for (i=0;i<20;i++)
                {
                    if(robotClient.getProgramState()==eStop)
                    {
                        break; //如果为停止状态，跳出for循环
                    }
                    sleep(50);//50ms后再次获取状态
                }
            }
        }
    }*/
/*
    //手动模式下点动机器人
    result = 0;
    if(robotClient.getOperationMode() != eManualHigh)
    {
        result = robotClient.SetModeCmd(MANUAL);//操作模式设置为手动
        if(result != 0)
        {
            cout<<"操作失败，结果为："<<result<<endl;
            std::cout <<"初始化成功15" << std::endl;
        }
        else 
        {
            for (int i=0;i<20;i++)
            {
                if(robotClient.getOperationMode() == eManualHigh)
                {
                    break; //如果为手动模式，跳出for循环
                }
                sleep(50);//50ms后再次获取状态
            }
            std::cout <<"初始化成功16" << std::endl;
        }
    }*/

    
    /*    //设置点动系，如果不是关节点动坐标系（这里根据需求设置），设置为关节
    if (robotClient.getJogRefSys() != eJoints)
    {
        result = robotClient.JogRefSystemCmd("JOINTS"); //设置点动坐标系为关节点动坐标系
        if (result != 0)
        {
            cout << "设置点动坐标系失败，结果为:" << result << endl;
            std::cout <<"初始化成功17" << std::endl;
        }
        else
        {
            for (int i = 0; i < 20; i++)
            {
                if (robotClient.getJogRefSys() == eJoints)
                {
                    break; //如果为关节点动坐标系，跳出for循环
                }
                sleep(50);//50ms后再次获取状态
            }
            std::cout <<"初始化成功18" << std::endl;
        }
    }
        */
    //上使能，当前机器人未使能状态
    if(robotClient.getMotorEnableState() == 0)
    {
        result = robotClient.EnableMotorCmd(); //上使能
        if( result != 0) //检查是否成功
        {
            cout << "上使能执行失败，结果为: "<< result << endl;
            std::cout <<"初始化成功19" << std::endl;
        }
        else
        {
            for (int i = 0; i < 20; i++)
            {
                if (robotClient.getMotorEnableState() == 1)
                {
                    break; //如果为使能状态，跳出for循环
                }
                sleep(50);//50ms后再次获取状态
            }
            sleep(300);//等待300ms
            std::cout <<"初始化成功20" << std::endl;
        }    
    }
    //点动机器人关节1，正方向运行后停止
    result = robotClient.SetModeCmd(POSITIVE1);
    if(result != 0)
    {
        cout<<"点动J1+失败,结果为："<<result<<endl;
        std::cout <<"初始化成功21" << std::endl;
    }
    else
    {
        sleep(3);
        result = robotClient.SetModeCmd(STOP);//停止点动
        if(result != 0)
        {
            cout<<"停止点动失败，结果为："<<result<<endl;
            std::cout <<"初始化成功22" << std::endl;
        }
        else
        {
            for(i=0;i<20;i++)
            {
                if(robotClient.getProgramState() == eStop)
                {
                    break; //如果为停止状态，跳出for循环
                }
                sleep(50);//50ms后再次获取状态
            }
            std::cout <<"初始化成功23" << std::endl;
        }
    }
    //点动机器人关节2，负方向运行后停止
    result = robotClient.SetModeCmd(NEGATIVE2);
    if(result != 0)
    {
        cout<<"点动J2-失败,结果为："<<result<<endl;
        std::cout <<"初始化成功24" << std::endl;
    }
    else
    {
        sleep(5);//程序执行需要时间
        result = robotClient.SetModeCmd(STOP);//停止点动
        if(result != 0)
        {
            cout<<"停止点动失败，结果为："<<result<<endl;
            std::cout <<"初始化成功25" << std::endl;
        }
        else
        {
            for(i=0;i<20;i++)
            {
                if(robotClient.getProgramState() == eStop)
                {
                    break; //如果为停止状态，跳出for循环
                }
                sleep(50);//50ms后再次获取状态
            }
            std::cout <<"初始化成功26" << std::endl;
        }
    }
    //点动机器人关节1，负方向运行后停止
    result = robotClient.SetModeCmd(NEGATIVE1);
    if(result != 0)
    {
        cout<<"点动J1-失败,结果为："<<result<<endl;
        std::cout <<"初始化成功27" << std::endl;
    }
    else
    {
        sleep(3);
        result = robotClient.SetModeCmd(STOP);//停止点动
        if(result != 0)
        {
            cout<<"停止点动失败，结果为："<<result<<endl;
            std::cout <<"初始化成功28" << std::endl;
        }
        else
        {
            for(i=0;i<20;i++)
            {
                if(robotClient.getProgramState() == eStop)
                {
                    break; //如果为停止状态，跳出for循环
                }
                sleep(50);//50ms后再次获取状态
            }
            std::cout <<"初始化成功29" << std::endl;
        }
    }
    //点动机器人关节2，正方向运行后停止
    result = robotClient.SetModeCmd(POSITIVE2);
    if(result != 0)
    {
        cout<<"点动J2+失败,结果为："<<result<<endl;
        std::cout <<"初始化成功30" << std::endl;
    }
    else
    {
        sleep(5);//程序执行需要时间
        result = robotClient.SetModeCmd(STOP);//停止点动
        if(result != 0)
        {
            cout<<"停止点动失败，结果为："<<result<<endl;
            }
    }
    //点动机器人关节3，正方向运行后停止
    result = robotClient.SetModeCmd(POSITIVE3);
    if(result != 0)
    {
        cout<<"点动J3+失败,结果为："<<result<<endl;
    }
    else
    {
        sleep(2);
        result = robotClient.SetModeCmd(STOP);//停止点动
        if(result != 0)
        {
            cout<<"停止点动失败，结果为："<<result<<endl;
        }
    }
    //点动机器人关节3，负方向运行后停止
    result = robotClient.SetModeCmd(NEGATIVE3);
    if(result != 0)
    {
        cout<<"点动J3-失败,结果为："<<result<<endl;
    }
    else
    {
        sleep(2);//程序执行需要时间
        result = robotClient.SetModeCmd(STOP);//停止点动
        if(result != 0)
        {
            cout<<"停止点动失败，结果为："<<result<<endl;
        }
    }
    //点动机器人关节4，正方向运行后停止
    result = robotClient.SetModeCmd(POSITIVE4);
    if(result != 0)
    {
        cout<<"点动J4+失败,结果为："<<result<<endl;
    }
    else
    {
        sleep(2);
        result = robotClient.SetModeCmd(STOP);//停止点动
        if(result != 0)
        {
            cout<<"停止点动失败，结果为："<<result<<endl;
        }
    }
    //点动机器人关节4，负方向运行后停止
    result = robotClient.SetModeCmd(NEGATIVE4);
    if(result != 0)
    {
        cout<<"点动J4-失败,结果为："<<result<<endl;
    }
    else
    {
        sleep(2);//程序执行需要时间
        result = robotClient.SetModeCmd(STOP);//停止点动
        if(result != 0)
        {
            cout<<"停止点动失败，结果为："<<result<<endl;
        }
    }
    //点动机器人关节5，正方向运行后停止
    result = robotClient.SetModeCmd(POSITIVE5);
    if(result != 0)
    {
        cout<<"点动J5+失败,结果为："<<result<<endl;
    }
    else
    {
        sleep(2);
        result = robotClient.SetModeCmd(STOP);//停止点动
        if(result != 0)
        {
            cout<<"停止点动失败，结果为："<<result<<endl;
        }
    }
    //点动机器人关节5，负方向运行后停止
    result = robotClient.SetModeCmd(NEGATIVE5);
    if(result != 0)
    {
        cout<<"点动J5-失败,结果为："<<result<<endl;
    }
    else
    {
        sleep(2);//程序执行需要时间
        result = robotClient.SetModeCmd(STOP);//停止点动
        if(result != 0)
        {
            cout<<"停止点动失败，结果为："<<result<<endl;
        }
    }
    //点动机器人关节6，正方向运行后停止
    result = robotClient.SetModeCmd(POSITIVE6);
    if(result != 0)
    {
        cout<<"点动J6+失败,结果为："<<result<<endl;
    }
    else
    {
        sleep(2);
        result = robotClient.SetModeCmd(STOP);//停止点动
        if(result != 0)
        {
            cout<<"停止点动失败，结果为："<<result<<endl;
        }
    }
    //点动机器人关节6，负方向运行后停止
    result = robotClient.SetModeCmd(NEGATIVE6);
    if(result != 0)
    {
        cout<<"点动J6-失败,结果为："<<result<<endl;
    }
    else
    {
        sleep(2);//程序执行需要时间
        result = robotClient.SetModeCmd(STOP);//停止点动
        if(result != 0)
        {
            cout<<"停止点动失败，结果为："<<result<<endl;
        }
    }
    //根据需求断使能
    if(robotClient.getMotorEnableState() == 1)
    {
        result = robotClient.EnableMotorCmd();//断使能
        if(result != 0)
        {
            cout<<"断使能失败，结果为："<<result<<endl;
        }
        else
        {
            for(i=0;i<20;i++)
            {
                if(robotClient.getMotorEnableState() == 0)
                {
                    break; //如果为断使能状态，跳出for循环
                }
                sleep(50);//50ms后再次获取状态
            }
        }
    }

    //加载程序，当前必须没程序加载
    result=0;
    string project="project0";//示教器已有该项目
    string program="program0";//示教器已有该程序
    vector<int> errorIine; //新建错误行号变量
    result = robotClient.ProgramLoadCmd(project, program, errorIine); //加载程序
    if(result != 0)
    {
        cout<<"加载程序失败，结果为："<<result<<endl;
    }
    else
    {
        for(i=0;i<20;i++)
        {
            if (robotClient.getProgramName() == program)
            {
                break; //如果程序名为加载程序名状态，跳出for循环
            }
            sleep(50);//50ms后再次获取状态
        }
    }//如加载的程序有语法错误，返回具体错误行号,行号为-1 时，错误信息较多，请到示教器上查看

    //卸载程序
    result =0;
    project="project0";
    program="program0";
    result = robotClient.ProgramKillCmd(project, program); //卸载程序
    if(result != 0)
    {
        cout<<"卸载程序失败，结果为："<<result<<endl;
    }
    else
    {
        for(i=0;i<20;i++)
        {
            if (robotClient.getProgramName() == program)
            {
                break; //如果程序名为卸载程序名状态，跳出for循环
            }
            sleep(50);//50ms后再次获取状态
        }
    }

    //清除错误
    result =0;
    result = robotClient.AllAlarmConfirmCmd(); //清除错误
    if(result != 0)
    {
        cout<<"清除错误失败，结果为："<<result<<endl;
    }

    //设置用户坐标系
    result = 0;
    string refName="ROBOTBASE";//示教器中已有该用户坐标系
    result = robotClient.ReferenceSystemSetCmd(refName); //设置用户为：ROBOTBASE
    if (result != 0)
    {
        cout << "设置用户坐标系失败，结果为:" << result << endl;
    }
    else
    {
        for (int i = 0; i < 20; i++)
        {
            if (robotClient.getTeachRefSysNam() == refName)
            {
                break; //如果为设置的用户坐标系，跳出for循环
            }
            sleep(50);//50ms后再次获取状态
    }

    //设置工具坐标系
    result = 0;
    string tool="Default";//示教器中已有该工具
    result = robotClient.ToolSetCmd(tool); //设置工具为：Default
    if (result != 0)
    {
        cout << "设置工具坐标系失败，结果为:" << result << endl;
    }
    else
    {
        for (int i = 0; i < 20; i++)
        {
            if (robotClient.getToolName() == tool)
            {
                break; //如果为设置的工具，跳出for循环
            }
            sleep(50);//50ms后再次获取状态
        }
    }

    //设置全局速度
    result = 0;
    double ori = 10;//ori为0.1~100的double值，1以上四舍五入成整
    result = robotClient.OverrideSetCmd(ori); //设置全局速度为10%
    if (result != 0)
    {
        cout << "设置全局速度失败，结果为:" << result << endl;
    }
    else
    {
        for (int i = 0; i < 20; i++)
        {
            if (robotClient.getOverride() == ori)
            {
                break; //如果为设置的全局速度，跳出for循环
            }
            sleep(50);//50ms后再次获取状态
    }

    //设置点动坐标系
    result = 0;
    result = robotClient.JogRefSystemCmd("JOINTS");
    if (result != 0)
    {
        cout<<"设置点动坐标系失败，结果为："<<result<<endl;
    }
    else
    {
        for (int i = 0; i < 20; i++)
        {
            if (robotClient.getJogRefSys() == eJoints)
            {
                break; //如果为设置的点动坐标系，跳出for循环
            }
            sleep(50);//50ms后再次获取状态
        }
    }

    //设置当前运行模式
    result = 0;
    int mode =0;
    result = robotClient.ProgramRunModeCmd(mode); //设置当前运行模式为：连续
    if (result != 0)
    {
        cout<<"设置当前运行模式失败，结果为："<<result<<endl;
    }
    else
    {
        for(int i=0;i<20;i++)
        {
            if (robotClient.getProgramMode() == eContinue)
            {
                break; //如果为设置的当前运行模式，跳出for循环
            }
            sleep(50);//50ms后再次获取状态
        }
    }

    //设置程序行号
    result = 0;
    int linNum =5;
    result = robotClient.SetpcCmd(linNum); //设置行号为5
    if(result != 0)
    {
        cout<<"设置程序行号失败，结果为："<<result<<endl;
    }
    else
    {
        for (int i=0;i<20;i++)
        {
            if (robotClient.getCurrentLine() == linNum)
            {
                break; //如果为设置的行号，跳出for循环
            }
            sleep(50);//50ms后再次获取状态
        }
    }

    //设置DI
    //强制DI状态
    result = 0;
    int port = 5;
    int data = 1;
    result = robotClient.DigitalInputSimulationCmd(port,data);//强制DI6为TRUE
    if(result != 0)
    {
        cout<<"设置DI失败,结果为："<<result<<endl;
    }
    else
    {
        for (int i=0;i<20;i++)
        {
            if ((robotClient.getRobotDI().m_DI[0] >> 5 & 0x01) == data)
            {
                break; //如果为设置的DI状态，跳出for循环
            }
            sleep(50);//50ms后再次获取状态
        }
    }
    //取消强制DI，会复位DI
    result = 0;
    port = 5;
    result = robotClient.DigitalInputSimulationRemovalCmd(port);//取消DI6强制选项
    if(result != 0)
    {
        cout<<"取消强制DI失败,结果为："<<result<<endl;
    }
    else
    {
        for(int i=0;i<20;i++)
        {
            if ((robotClient.getRobotDI().m_DIMask[0] >> 5 & 0x01) == data)
            {
                break; //如果为设置的DI状态，跳出for循环
            }
            sleep(50);//50ms后再次获取状态
        }
    }

    //设置DO
    //强制DO状态
    result = 0;
    port = 5;
    data = 1;
    result = robotClient.DigitalOutputEnforceCmd(port,data);//强制DO6为TRUE
    if(result != 0)
    {
        cout<<"设置DO失败,结果为："<<result<<endl;
    }
    else
    {
        for(int i=0;i<20;i++)
        {
            if ((robotClient.getRobotDO().m_DO[0] >> 5 & 0x01) == data)
            {
                break; //如果为设置的DO状态，跳出for循环
            }
            sleep(50);//50ms后再次获取状态
        }
    }
    //取消强制DO，会复位DO
    result = 0;
    port = 5;
    result = robotClient.DigitalOutputEnforceRemovalCmd(port);//取消DO6强制选项
    if(result != 0)
    {
        cout<<"取消强制DO失败,结果为："<<result<<endl;
    }
    else
    {
        for(int i=0;i<20;i++)
        {
            if ((robotClient.getRobotDO().m_DOMask[0] >> 5 & 0x01) == 0)
            {
                break; //如果为设置的DO状态，跳出for循环
            }
            sleep(50);//50ms后再次获取状态
        }
    }

    //设置AI
    //强制AI状态
    result = 0;
    port = 5;
    data = 1;
    result = robotClient.AnalogInputSimulationCmd(port,data);//强制AI6为10
    if(result != 0)
    {
        cout<<"设置AI失败,结果为："<<result<<endl;
    }
    else
    {
        for(int i=0;i<20;i++)
        {
            if (robotClient.getRobotAI().m_AI[port] == data)
            {
                break; //如果为设置的AI状态，跳出for循环
            }
            sleep(50);//50ms后再次获取状态
        }
    }
    //取消强制AI，会复位AI
    result = 0;
    port = 5;
    result = robotClient.AnalogInputSimulationRemovalCmd(port);//取消AI6强制选项
    if(result != 0)
    {
        cout<<"取消强制AI失败,结果为："<<result<<endl;
    }
    else
    {
        for(int i=0;i<20;i++)
        {
            if ((robotClient.getRobotAI().m_AIMask == 0))
            {
                break; //如果为设置的AI状态，跳出for循环
            }
            sleep(50);//50ms后再次获取状态
        }
    }

    //设置AO
    //强制AO状态
    result = 0;
    port = 5;
    data = 1;
    result = robotClient.AnalogOutputEnforceCmd(port,data);//强制AO6为10
    if(result != 0)
    {
        cout<<"设置AO失败,结果为："<<result<<endl;
    }
    else
    {
        for(int i=0;i<20;i++)
        {
            if (robotClient.getRobotAO().m_AO[port] == data)
            {
                break; //如果为设置的AO状态，跳出for循环
            }
            sleep(50);//50ms后再次获取状态
        }
    }
    //取消强制AO，会复位AO
    result = 0;
    port = 5;
    result = robotClient.AnalogOutputEnforceRemovalCmd(port);//取消AO6强制选项
    if(result != 0)
    {
        cout<<"取消强制AO失败,结果为："<<result<<endl;
    }
    else
    {
        for(int i=0;i<20;i++)
        {
            if ((robotClient.getRobotAO().m_AOMask == 0))
            {
                break; //如果为设置的AO状态，跳出for循环
            }
            sleep(50);//50ms后再次获取状态
        }
    }

    //修改全局变量时，工程名为“_global”，程序名为“_project”
/*
    //修改bool值
    result = 0;
    project = "project0";
    program = "program0";
    string variable = "bool0"; //示教器中已有该变量
    bool valuebool = true;
    result = robotClient.VariableBoolModifyCmd(project, program, variable, valuebool);//修改bool0为TRUE
    if(result != 0)
    {
        cout<<"修改bool值失败,结果为："<<result<<endl;
    }
    //在线修改bool值
    result = 0;
    project = "project0";
    program = "program0";
    variable = "bool0"; 
    valuebool = true;
    result = robotClient.ProcessVariableBoolModifyCmd(project, program, variable,valuebool);//修改bool0为TRUE
    if(result != 0)
    {
        cout<<"在线修改bool值失败,结果为："<<result<<endl;
    }

    //修改int值
    result = 0;
    project = "project0";
    program = "program0";
     variable = "int0"; //示教器中已有该变量
    int valueint = 10;
    result = robotClient.VariableIntModifyCmd(project, program, variable, valueint);//修改int0为10
    if(result != 0)
    {
        cout<<"修改int值失败,结果为："<<result<<endl;
    }
    //在线修改int值
    result = 0;
    project = "project0";
    program = "program0";
    variable = "int0";
    valueint = 10;
    result = robotClient.ProcessVariableIntModifyCmd(project, program, variable,valueint);//修改int0为10
    if(result != 0)
    {
        cout<<"在线修改int值失败,结果为："<<result<<endl;
    }

    //修改real值
    result = 0;
    project = "project0";
    program = "program0";
    variable = "real0"; //示教器中已有该变量
    float valuereal = 10.01;
    result = robotClient.VariableRealModifyCmd(project, program, variable, valuereal);//修改real0为10.01
    if(result != 0)
    {
        cout<<"修改real值失败,结果为："<<result<<endl;
    }
    //在线修改real值
    result = 0;
    project = "project0";
    program = "program0";
    variable = "real0";
    valuereal = 10.01;
    result = robotClient.ProcessVariableRealModifyCmd(project, program, variable,valuereal);//修改real0为10.01
    if(result != 0)
    {
        cout<<"在线修改real值失败,结果为："<<result<<endl;
    }

    //修改string值
    result = 0;
    project = "project0";
    program = "program0";
    variable = "string0"; //示教器中已有该变量
    string valuestring = "STEP Robot";
    result = robotClient.VariableStringModifyCmd(project, program, variable, valuestring);//修改string0为STEP Robot
    if(result != 0)
    {
        cout<<"修改string值失败,结果为："<<result<<endl;
    }
    //在线修改string值
    result = 0;
    project = "project0";
    program = "program0";
    variable = "string0";
    valuestring = "STEP Robot";
    result = robotClient.ProcessVariableStringModifyCmd(project, program, variable,valuestring);//修改string0为STEP Robot
    if(result != 0)
    {
        cout<<"在线修改string值失败,结果为："<<result<<endl;
    }

    //修改axispos值
    result = 0;
    project = "project0";
    program = "program0";
    variable = "ap0"; //示教器中已有该变量
    AXISPOS valueaxispos = { 10,-10,10,-10,10,-10,0,0,0,0,0,0 };
    result = robotClient.VariableAxisposModifyCmd(project, program, variable, valueaxispos);//修改ap0为J1=10°,J2=-10°,J3=10°,J4=-10°,J5=10°,J6=-10°,附加轴均为0°
    if(result != 0)
    {
        cout<<"修改axispos值失败,结果为："<<result<<endl;
    }
    //在线修改axispos值
    result = 0;
    project = "project0";
    program = "program0";
    variable = "ap0";
    valueaxispos = { 10,-10,10,-10,10,-10,0,0,0,0,0,0 };
    result = robotClient.ProcessVariableAxisposModifyCmd(project, program, variable,valueaxispos);//修改ap0为J1=10°,J2=-10°,J3=10°,J4=-10°,J5=10°,J6=-10°,附加轴均为0°
    if(result != 0)
    {
        cout<<"在线修改axispos值失败,结果为："<<result<<endl;
    }

    //修改cartpos值
    result = 0;
    project = "project0";
    program = "program0";
    variable = "cp0"; //示教器中已有该变量
    CARTPOS valuecartpos = {300,0,500,-180,0,0,0,0,0,0,0,0,0};
    result = robotClient.VariableCartposModifyCmd(project, program, variable, valuecartpos);//修改cp0为X = 300,Y = 0,Z = 500,A = -180,B = 0,C = 0,附加轴均为0°,mode=0
    if(result != 0)
    {
        cout<<"修改cartpos值失败,结果为："<<result<<endl;
    }
    //在线修改cartpos值
    result = 0;
    project = "project0";
    program = "program0";
    variable = "cp0";
    valuecartpos = {300,0,500,-180,0,0,0,0,0,0,0,0,0};
    result = robotClient.ProcessVariableCartposModifyCmd(project, program, variable,valuecartpos);//修改cp0为X = 300,Y = 0,Z = 500,A = -180,B = 0,C = 0,附加轴均为0°,mode=0
    if(result != 0)
    {
        cout<<"在线修改cartpos值失败,结果为："<<result<<endl;
    }

    //修改robotaxispos值
    result = 0;
    project = "project0";
    program = "program0";
    variable = "rap0"; //示教器中已有该变量
    JointsPos valuerobotaxispos = { 10,-10,10,-10,10,-10};
    result = robotClient.VariableRobotAxisposModifyCmd(project, program, variable, valuerobotaxispos);//修改rap0为J1=10°,J2=-10°,J3=10°,J4=-10°,J5=10°,J6=-10°
    if(result != 0)
    {
        cout<<"修改robotaxispos值失败,结果为："<<result<<endl;
    }
    //在线修改robotaxispos值
    result = 0;
    project = "project0";
    program = "program0";
    variable = "rap0";
    JointsPos valuerobotaxispos1 = { 10,-10,10,-10,10,-10};
    result = robotClient.ProcessVariableRobotAxisposModifyCmd(project, program, variable,valuerobotaxispos);//修改rap0为J1=10°,J2=-10°,J3=10°,J4=-10°,J5=10°,J6=-10°
    if(result != 0)
    {
        cout<<"在线修改robotaxispos值失败,结果为："<<result<<endl;
    }

    //修改robotcartpos值
    result = 0;
    project = "project0";
    program = "program0";
    variable = "rcp0"; //示教器中已有该变量
    RobotCartPos valuerobotcartpos = {300,0,500,-180,0,0,0};
    result = robotClient.VariableRobotCartposModifyCmd(project, program, variable, valuerobotcartpos);//修改rcp0为X = 300,Y = 0,Z = 500,A = -180,B = 0,C = 0,mode=0
    if(result != 0)
    {
        cout<<"修改robotcartpos值失败,结果为："<<result<<endl;
    }
    //在线修改robotcartpos值
    result = 0;
    project = "project0";
    program = "program0";
    variable = "rcp0";
    RobotCartPos valuerobotcartpos1 = {300,0,500,-180,0,0,0};
    result = robotClient.ProcessVariableRobotCartposModifyCmd(project, program, variable,valuerobotcartpos1);//修改rcp0为X = 300,Y = 0,Z = 500,A = -180,B = 0,C = 0,mode=0
    if(result != 0)
    {
        cout<<"在线修改robotcartpos值失败,结果为："<<result<<endl;
    }

    //修改dynamic值
    result = 0;
    project = "project0";
    program = "program0";
    variable = "dyn0"; //示教器中已有该变量
    SDynamicPercent valuedynamic;
    valuedynamic.m_SegmentDynamic.m_Vel = 2000; //位置速度
    valuedynamic.m_SegmentDynamic.m_Acc = 6000; //位置加速度
    valuedynamic.m_SegmentDynamic.m_Dec = 6000; //位置减速度
    valuedynamic.m_SegmentDynamic.m_Jerk = 60000; //位置加加速度
    valuedynamic.m_OriDynamic.m_Vel = 200;//旋转速度
    valuedynamic.m_OriDynamic.m_Acc = 750; //旋转加速度
    valuedynamic.m_OriDynamic.m_Dec = 750; //旋转减速度
    valuedynamic.m_OriDynamic.m_Jerk = 7500; //旋转加加速度
    valuedynamic.m_JointPercent.m_Vel = 100;//关节速度
    valuedynamic.m_JointPercent.m_Acc = 200;//关节加速度
    valuedynamic.m_JointPercent.m_Dec = 200;//关节减速度
    valuedynamic.m_JointPercent.m_Jerk = 200;//关节加加速度
    result = robotClient.VariableDynamicModifyCmd(project, program, variable, valuedynamic);//修改dynamic值为位置速度2000，位置加速度6000，位置减速度6000，位置加加速度60000，旋转速度200，旋转加速度750，旋转减速度750，旋转加加速度7500，关节速度100，关节加速度200，关节减速度200，关节加加速度200
    if(result != 0)
    {
        cout<<"修改dynamic值失败,结果为："<<result<<endl;
    }
    //在线修改dynamic值
    result = 0;
    project = "project0";
    program = "program0";
    variable = "dyn0";
    SDynamicPercent valuedynamic1;
    valuedynamic.m_SegmentDynamic.m_Vel = 2000; //位置速度
    valuedynamic.m_SegmentDynamic.m_Acc = 6000; //位置加速度
    valuedynamic.m_SegmentDynamic.m_Dec = 6000; //位置减速度
    valuedynamic.m_SegmentDynamic.m_Jerk = 60000; //位置加加速度
    valuedynamic.m_OriDynamic.m_Vel = 200;//旋转速度
    valuedynamic.m_OriDynamic.m_Acc = 750; //旋转加速度
    valuedynamic.m_OriDynamic.m_Dec = 750; //旋转减速度
    valuedynamic.m_OriDynamic.m_Jerk = 7500; //旋转加加速度
    valuedynamic.m_JointPercent.m_Vel = 100;//关节速度
    valuedynamic.m_JointPercent.m_Acc = 200;//关节加速度
    valuedynamic.m_JointPercent.m_Dec = 200;//关节减速度
    valuedynamic.m_JointPercent.m_Jerk = 200;//关节加加速度
    result = robotClient.ProcessVariableDynamicModifyCmd(project, program, variable, valuedynamic1);//修改dynamic值为位置速度2000，位置加速度6000，位置减速度6000，位置加加速度60000，旋转速度200，旋转加速度750，旋转减速度750，旋转加加速度7500，关节速度100，关节加速度200，关节减速度200，关节加加速度200
    if(result != 0)
    {
        cout<<"在线修改dynamic值失败,结果为："<<result<<endl;
    }

    //修改arcondata值
    result = 0;
    project = "project0";
    program = "program0";
    variable = "arcon0"; //示教器中已有该变量
    ArcOnData valuearcondata;
    valuearcondata.m_preFlowtime = 100; //前段流动时间
    valuearcondata.m_arconCurrent = 2; //电流
    valuearcondata.m_arconVoltage = 5; //电压
    valuearcondata.m_arconTime = 100; //前段延时时间
    valuearcondata.m_BeforeArcOn = 10; //前段弧上
    valuearcondata.m_BeforeWireOut = 20; //前段出丝
    result = robotClient.VariableArcOnDataModifyCmd(project, program, variable, valuearcondata);//修改arcon0值为前段流动时间100，电流2，电压5，前段延时时间100，前段弧上10，前段出丝20
    if(result != 0)
    {
        cout<<"修改arcondata值失败,结果为："<<result<<endl;
    }
    //在线修改arcondata值
    result = 0;
    project = "project0";
    program = "program0";
    variable = "arcon0";
    ArcOnData valuearcondata1;
    valuearcondata1.m_preFlowtime = 100; //前段流动时间
    valuearcondata1.m_arconCurrent = 2; //电流
    valuearcondata1.m_arconVoltage = 5; //电压
    valuearcondata1.m_arconTime = 100; //前段延时时间
    valuearcondata1.m_BeforeArcOn = 10; //前段弧上
    valuearcondata1.m_BeforeWireOut = 20; //前段出丝
    result = robotClient.ProcessVariableArcOnDataModifyCmd(project, program, variable, valuearcondata1);//修改arcon0值为前段流动时间100，电流2，电压5，前段延时时间100，前段弧上10，前段出丝20
    if(result != 0)
    {
        cout<<"在线修改arcondata值失败,结果为："<<result<<endl;
    }

    //修改arcoffdata值
    result = 0;
    project = "project0";
    program = "program0";
    variable = "arcoff0"; //示教器中已有该变量
    ArcOffData valuearcoffdata={1,2,3,4,5};
    result = robotClient.VariableArcOffDataModifyCmd(project, program, variable, valuearcoffdata);
    if(result != 0)
    {
        cout<<"修改arcoffdata值失败,结果为："<<result<<endl;
    }
    //在线修改arcoffdata值
    result = 0;
    project = "project0";
    program = "program0";
    variable = "arcoff0";
    ArcOffData valuearcoffdata1={1,2,3,4,5};
    result = robotClient.ProcessVariableArcOffDataModifyCmd(project, program, variable, valuearcoffdata1);
    if(result != 0)
    {
        cout<<"在线修改arcoffdata值失败,结果为："<<result<<endl;
    }

    //修改arcdata值
    result = 0;
    project = "project0";
    program = "program0";
    variable = "arc0"; //示教器中已有该变量
    ArcData valuearcdata={1,2,3,4};
    result = robotClient.VariableArcDataModifyCmd(project, program, variable, valuearcdata);
    if(result != 0)
    {
        cout<<"修改arcdata值失败,结果为："<<result<<endl;
    }
    //在线修改arcdata值
    result = 0;
    project = "project0";
    program = "program0";
    variable = "arc0";
    ArcData valuearcdata1={1,2,3,4};
    result = robotClient.ProcessVariableArcDataModifyCmd(project, program, variable, valuearcdata1);
    if(result != 0)
    {
        cout<<"在线修改arcdata值失败,结果为："<<result<<endl;
    }

    //修改arcretrydata值
    result = 0;
    project = "project0";
    program = "program0";
    variable = "retry0"; //示教器中已有该变量
    ArcRetryData valuearcretrydata={1000, 0,1000,50,0, 0,"TRUE",7,50,9};
    result = robotClient.VariableArcRetryDataModifyCmd(project, program, variable, valuearcretrydata);
    if(result != 0)
    {
        cout<<"修改arcretrydata值失败,结果为："<<result<<endl;
    }
    //在线修改arcretrydata值
    result = 0;
    project = "project0";
    program = "program0";
    variable = "retry0";
    ArcRetryData valuearcretrydata1={1000, 0,1000,50,0, 0,"TRUE",7,50,9};
    result = robotClient.ProcessVariableArcRetryDataModifyCmd(project, program, variable, valuearcretrydata1);
    if(result != 0)
    {
        cout<<"在线修改arcretrydata值失败,结果为："<<result<<endl;
    }

    //修改arcsegdata值
    result = 0;
    project = "project0";
    program = "program0";
    variable = "arcsegdata0"; //示教器中已有该变量
    ArcSegData valuearcsegdata={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21};
    result = robotClient.VariableArcSegDataModifyCmd(project, program, variable, valuearcsegdata);
    if(result != 0)
    {
        cout<<"修改arcsegdata值失败,结果为："<<result<<endl;
    }
    //在线修改arcsegdata值
    result = 0;
    project = "project0";
    program = "program0";
    variable = "arcsegdata0";
    ArcSegData valuearcsegdata1={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21};
    result = robotClient.ProcessVariableArcSegDataModifyCmd(project, program, variable, valuearcsegdata1);
    if(result != 0)
    {
        cout<<"在线修改arcsegdata值失败,结果为："<<result<<endl;
    }

    //修改arcconfigdata值
    result = 0;
    project = "project0";
    program = "program0";
    variable = "conf0"; //示教器中已有该变量
    double arcconfigdata[14] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };  */

    cartdynstate = robotClient.getActCartDynState(); //获取动态信息
    double vel = cartdynstate.m_Vel; //获取速度
    double acc = cartdynstate.m_Acc; //获取加速度
    double dec = cartdynstate.m_Dec; //获取减速度
    double jerk = cartdynstate.m_Jerk; //获取加加速度

    operationmode = robotClient.getOperationMode(); //获取操作模式

    jogrefsys = robotClient.getJogRefSys(); //获取点动坐标系

    double override = robotClient.getOverride(); //获取全局速度

    programstate = robotClient.getProgramState(); //获取程序运行状态

    programmode = robotClient.getProgramMode(); //获取程序运行模式

    int currentline = robotClient.getCurrentLine(); //获取当前所在行


  projectname = robotClient.getProjectName();  //重点之一
  programname = robotClient.getProgramName();  //重点之一
  teachrefsysname = robotClient.getTeachRefSysNam();
  toolname = robotClient.getToolName();
  errormassage = robotClient.getMessageData();
  
  AXISPOS currentaxispos = robotClient.getAxisPos(); //获取当前笛卡尔位置
  double posJ1 = currentaxispos.m_Joint[0]; //X坐标
  double posJ2 = currentaxispos.m_Joint[1]; //Y坐标
  double posJ3 = currentaxispos.m_Joint[2]; //Z坐标
  double posJ4 = currentaxispos.m_Joint[3]; //A坐标
  double posJ5 = currentaxispos.m_Joint[4]; //B坐标
  double posJ6 = currentaxispos.m_Joint[5]; //C坐标
  double posAux1 = currentaxispos.m_AuxJoint[0]; //附加轴1位置
  double posAux2 = currentaxispos.m_AuxJoint[1]; //附加轴2位置
  double posAux3 = currentaxispos.m_AuxJoint[2]; //附加轴3位置
  double posAux4 = currentaxispos.m_AuxJoint[3]; //附加轴4位置
  double posAux5 = currentaxispos.m_AuxJoint[4]; //附加轴5位置
  double posAux6 = currentaxispos.m_AuxJoint[5]; //附加轴6位置

  RobotCartPos cartposworld = robotClient.getCartPosWorld();//获取当前世界笛卡尔位置
  double posX = cartposworld.cart[0]; //X坐标
  double posY = cartposworld.cart[1]; //Y坐标
  double posZ = cartposworld.cart[2]; //Z坐标
  double posA = cartposworld.cart[3]; //A坐标
  double posB = cartposworld.cart[4]; //B坐标
  double posC = cartposworld.cart[5]; //C坐标
  unsigned int Mode = cartposworld.m_Mode; //坐标系模式

  RobotCartPos cartposbase = robotClient.getCartPosRobotBase();//获取当前基坐标系笛卡尔位置

  RobotCartPos cartuserposbase = robotClient.getCartPosUserRef();//获取当前用户坐标系笛卡尔位置

  HomoMatrix cartposr = robotClient.getCartPosR();//获取当前带旋转矩阵的笛卡尔位置

  int runningtime = robotClient.getRobotRunningTime(); //获取运行时间

  int lifetime = robotClient.getLifeTime(); //获取开机时间

  ARCWELDINGMODE arcmode = robotClient.getWeldingMode();//获取焊接模式

// 分离工作线程，让其独立运行
    worker_thread.detach();

    // 等待监控线程结束
    monitor_thread.join();

    return 0;

}
}
}