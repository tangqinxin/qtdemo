#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include "GalaxyIncludes.h"
#include "cameraevent.h"
#include<iostream>
int main(int argc, char *argv[])
{
    using namespace  std;
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    IGXFactory::GetInstance().Init();

        //枚举设备
        GxIAPICPP::gxdeviceinfo_vector vectorDeviceInfo;
        IGXFactory::GetInstance().UpdateDeviceList(1000, vectorDeviceInfo);
        for (uint32_t i = 0; i < vectorDeviceInfo.size(); i++){
            cout << vectorDeviceInfo[i].GetVendorName() << endl;
            cout << vectorDeviceInfo[i].GetModelName() << endl;
            //更多设备信息详见IGXDeviceInfo接口
        }

        //打开设备
        if (vectorDeviceInfo.size() > 0) {
            //打开链表中的第一个设备
            CGXDevicePointer objDevicePtr;

            GxIAPICPP::gxstring strSN = vectorDeviceInfo[0].GetSN();

            objDevicePtr = IGXFactory::GetInstance().OpenDeviceBySN(strSN, GX_ACCESS_EXCLUSIVE);//目前只能通过strSN打开

            objDevicePtr->Close();//关闭相机并释放资源
        }



        //
        //声明事件回调对象指针

           IDeviceOfflineEventHandler* pDeviceOfflineEventHandler = NULL;///<掉线事件回调对象

           IFeatureEventHandler* pFeatureEventHandler = NULL;///<远端设备事件回调对象

           ICaptureEventHandler* pCaptureEventHandler = NULL;///<采集回调对象



           //初始化

           IGXFactory::GetInstance().Init();
           try

           {

               do

               {

                  //枚举设备

                  gxdeviceinfo_vector vectorDeviceInfo;

                  IGXFactory::GetInstance().UpdateDeviceList(1000, vectorDeviceInfo);

                  if (0 == vectorDeviceInfo.size())

                  {

                      cout<<"无可用设备!"<<endl;

                      break;

                  }



                  //打开第一台设备以及设备下面第一个流

                  CGXDevicePointer ObjDevicePtr = IGXFactory::GetInstance().OpenDeviceBySN(

                        vectorDeviceInfo[0].GetSN(),

                        GX_ACCESS_EXCLUSIVE);

                  CGXStreamPointer ObjStreamPtr = ObjDevicePtr->OpenStream(0);



                  //注册设备掉线事件【目前只有千兆网系列相机支持此事件通知】

                  GX_DEVICE_OFFLINE_CALLBACK_HANDLE hDeviceOffline = NULL;

                  pDeviceOfflineEventHandler = new CSampleDeviceOfflineEventHandler();

                  hDeviceOffline = ObjDevicePtr->RegisterDeviceOfflineCallback(pDeviceOfflineEventHandler, NULL);



                  //获取远端设备属性控制器

        CGXFeatureControlPointer ObjFeatureControlPtr = ObjDevicePtr->GetRemoteFeatureControl();



        //设置曝光时间(示例中写死us,只是示例,并不代表真正可工作参数)

        ObjFeatureControlPtr->GetFloatFeature("ExposureTime")->SetValue(20000);


                  //注册回调采集

                  pCaptureEventHandler = new CSampleCaptureEventHandler();

                  ObjStreamPtr->RegisterCaptureCallback(pCaptureEventHandler, NULL);



                  //发送开采命令

                  ObjStreamPtr->StartGrab();

                  ObjFeatureControlPtr->GetCommandFeature("AcquisitionStart")->Execute();



                  //此时开采成功,控制台打印信息,直到输入任意键继续

                  getchar();



                  //发送停采命令

                  ObjFeatureControlPtr->GetCommandFeature("AcquisitionStop")->Execute();

                  ObjStreamPtr->StopGrab();



                  //注销采集回调

                  ObjStreamPtr->UnregisterCaptureCallback();

                  //注销远端设备事件

                  //ObjFeatureControlPtr->UnregisterFeatureCallback(hFeatureEvent);

                  //注销设备掉线事件

                  ObjDevicePtr->UnregisterDeviceOfflineCallback(hDeviceOffline);



                  //释放资源

                  ObjStreamPtr->Close();

                  ObjDevicePtr->Close();

               } while (0);

           }

           catch(CGalaxyException&e)

           {

               cout<<"错误码: "<<e.GetErrorCode() <<endl;

               cout<<"错误描述信息: "<<e.what() <<endl;

           }

           catch(std::exception&e)

           {

               cout<<"错误描述信息: "<<e.what() <<endl;

           }



           //反初始化库

           IGXFactory::GetInstance().Uninit();



           //销毁事件回调指针

           if (NULL != pCaptureEventHandler)

           {

               delete pCaptureEventHandler;

               pCaptureEventHandler = NULL;

           }

           if (NULL != pDeviceOfflineEventHandler)

           {

               delete pDeviceOfflineEventHandler;

               pDeviceOfflineEventHandler = NULL;

           }

           if (NULL != pFeatureEventHandler)

           {

               delete pFeatureEventHandler;

               pFeatureEventHandler = NULL;

           }
        //
    return a.exec();
}
